#include "cat/util/util.hpp"
#include <cat/gfx/shader_loader.hpp>

#include <cat/gfx/gfx_engine.hpp>

#include <cat/gfx/gfx_util.hpp>
#include <cat/gfx/shader.hpp>
#include <cat/error.hpp>

#include <cat/gfx/csl/csl.hpp>

#include <cstdlib>


using namespace cat;

#ifdef DEVELOP
    #define LOG_SHADER_COMPILE_ERROR(_code, _handle, _path)                             \
        do {                                                                            \
        LOG_ERRF("%s\n", _code);                                                        \
        _log_err(_handle, "compiling", _path, glGetShaderInfoLog, glGetShaderiv);       \
        throw Exception{"failed to compile shader"};                                    \
        } while(false)
#else
    throw Exception{"failed to compile shader"}
#endif

static void _log_err(
    u32 handle, const char* verb, const char* path,
    void (*get_log)(GLuint, GLsizei, GLsizei*, GLchar*),
    void (*get_iv)(GLuint, GLenum, GLint*)
)
{
    GLint log_len;
    get_iv(handle, GL_INFO_LOG_LENGTH, &log_len);

    char* log_text = reinterpret_cast<char*>(
        calloc(1, static_cast<size_t>(log_len))
    );
    get_log(handle, log_len, NULL, log_text);

    fprintf(stderr, "error %s shader at %s:\n%s\n", verb, path, log_text);

    free(log_text);
}

static u32 _compile(const std::string& src, GLenum type)
{
    if(!GfxEngine::is_loaded())
    {
        GfxEngine::get();
    }

    u32 shader_handle = glCreateShader(type);

    const char* code_ptr = src.c_str();
    
    glShaderSource(shader_handle, 1, &code_ptr, NULL);
    glCompileShader(shader_handle);

    i32 compiled;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
        LOG_SHADER_COMPILE_ERROR(src.c_str(), shader_handle, "TODO: fix path");
    }

    return shader_handle;
}

Shader::UniformCache _populate_uniform_cache(u32 handle, const csl::ShaderSource& src)
{
    Shader::UniformCache cache;

    cache.model_matrix = 
        glGetUniformLocation(handle, "u_model_matrix");
    cache.view_matrix = 
        glGetUniformLocation(handle, "u_view_matrix");
    cache.projection_matrix =
        glGetUniformLocation(handle, "u_projection_matrix");

    cache.texture_atlas =
        glGetUniformLocation(handle, "u_texture_atlas");

    for(const std::string& uname : src.custom_uniforms)
    {
        cache.user_uniforms[std::hash<std::string>{}(uname)] =
            glGetUniformLocation(handle, uname.c_str());
    }

    return cache;
}

Shader ShaderLoader::load(
    const std::string& path
) const THROWS
{
    csl::ShaderSource source = csl::split_file(path);
    
    std::string vert_src;
        vert_src.append(csl::PREAMBLE);
        vert_src.append(
            csl::BASIC_SHADERS_LIST[enum_val(source.base_behaviour)].vertex
        );
        vert_src.append(source.vertex);

    std::string frag_src;
        frag_src.append(csl::PREAMBLE);
        frag_src.append(
            csl::BASIC_SHADERS_LIST[enum_val(source.base_behaviour)].fragment
        );
        frag_src.append(source.fragment);

    u32 vs = _compile(vert_src, GL_VERTEX_SHADER);
    u32 fs = _compile(frag_src, GL_FRAGMENT_SHADER);

    u32 program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

#ifdef DEVELOP
    i32 linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    CAT_ASSERT(linked);
#endif

    glDeleteShader(vs);
    glDeleteShader(fs);

    Shader shader{ program };
    shader.m_uniform_cache = _populate_uniform_cache(
        shader.get_handle(),
        source
    );

    return shader;
}

hash_t ShaderLoader::hash(
    const std::string& path 
) const noexcept
{
    return m_hasher(path);
}

void ShaderLoader::unload(Shader* shader) const
{
    glDeleteProgram(shader->get_handle());
}

Shader ShaderLoader::load_basic(eBasicShaderType type)
{
    // call split_file to automatically resolve all hooks
    csl::ShaderSource resolved_hooks = csl::split_file(CSL_BASIC_SHADER_PATH);
    csl::ShaderSource src;

    csl::BasicShaderDef def = csl::BASIC_SHADERS_LIST[enum_val(type)];

    src.vertex.append(csl::PREAMBLE);
    src.vertex.append(def.vertex);
    src.vertex.append(resolved_hooks.vertex);
    src.fragment.append(csl::PREAMBLE);
    src.fragment.append(def.fragment);
    src.fragment.append(resolved_hooks.fragment);

    u32 vs = _compile(src.vertex, GL_VERTEX_SHADER);
    u32 fs = _compile(src.fragment, GL_FRAGMENT_SHADER);

    u32 program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

#ifdef DEVELOP
    i32 linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    CAT_ASSERT(linked);
#endif

    glDeleteShader(vs);
    glDeleteShader(fs);

    Shader shader{ program };
    shader.m_uniform_cache = _populate_uniform_cache(
        shader.get_handle(),
        {}
    );

    return shader;
}

