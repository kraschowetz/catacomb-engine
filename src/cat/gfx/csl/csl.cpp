#include "cat/gfx/csl/basic_shaders/unlit_2d.hpp"
#include "cat/gfx/csl/hook.hpp"
#include <cat/gfx/csl/csl.hpp>

#include <cat/error.hpp>

#include <fstream>
#include <span>
#include <sstream>
#include <string>

namespace cat::csl
{

// TODO: store default shaders outside of the core::ResourceManager

static void _resolve_hooks(std::string& src, std::span<const Hook> codex)
{
    for(const Hook& hook : codex)
    {
        if(!has_hook(src, hook.signature))
        {
            src.append(hook.signature);
            src.append(hook.stub_body);
        }
    }
}

void _resolve_target(ShaderSource& source, std::string_view name)
{
    for(const TargetDef& def : SHADER_TARGET_LOOKUP_TABLE)
    {
        if(name == def.name)
        {
            source.base_behaviour = def.type;
            return;
        }
    }
}

ShaderSource split_file(const std::string& path)
{
    // TODO: array-proof this
    
    ShaderSource result;

    // check if trying to load a default shader
    // if so, ignore all csl processing
    if(path.find(CSL_BASIC_SHADER_PATH) != std::string::npos)
    {
        _resolve_hooks(result.vertex, hooks::vertex_hooks);
        _resolve_hooks(result.fragment, hooks::fragment_hooks);
        return result;
    }

    std::string current_line;
    std::string* current_buffer = nullptr;
    
    std::ifstream file(path);

    if(!file.is_open())
    {
        std::stringstream stream;

        stream << "failed to load shader (" << path << ")!";

        throw Exception{ 
            eErrorCode::FILE_NOT_FOUND,
            stream.str()
        };
    }

    while(std::getline(file, current_line))
    {
        if(current_line == CAT_CSL_VERTEX_SECTION)
        {
            current_buffer = &result.vertex;
            continue;
        }
        else if(current_line == CAT_CSL_FRAGMENT_SECTION)
        {
            current_buffer = &result.fragment;
            continue;
        }
        else if(current_line.find(CAT_CSL_TARGET_DIRECTIVE) != std::string::npos)
        {
            constexpr u64 DIRECTIVE_SIZE = 8;
            std::string_view name = std::string_view(current_line).substr(DIRECTIVE_SIZE);
            _resolve_target(result, name);
            continue;
        }

        // ignore anything before the first section declaration
        if(!current_buffer) continue;

        current_buffer->append(current_line + '\n');
    }

    _resolve_hooks(result.vertex, hooks::vertex_hooks);
    _resolve_hooks(result.fragment, hooks::fragment_hooks);

    return result;
}

bool has_hook(const std::string &source, const char* hook_signature)
{
    return source.find(hook_signature) != std::string::npos;
}

}
