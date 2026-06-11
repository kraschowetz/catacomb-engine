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

Shader ShaderLoader::load(
    const std::string& path
) const THROWS
{
    csl::ShaderSource source = csl::split_file(path);
    
    // TODO: allow to choose the shader foundation to be used (e.g: 2D or 3D)
    std::string vert_src;
        vert_src.append(csl::PREAMBLE);
        vert_src.append(csl::BASIC_UNLIT_VERTEX_2D);
        vert_src.append(source.vertex);

    std::string frag_src;
        frag_src.append(csl::PREAMBLE);
        frag_src.append(csl::BASIC_UNLIT_FRAGMENT_2D);
        frag_src.append(source.fragment);

    u32 vs = _compile(vert_src, GL_VERTEX_SHADER);
    u32 fs = _compile(frag_src, GL_FRAGMENT_SHADER);

    u32 program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return Shader{ program };
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

    // TODO: check if this could be automated
    
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

    i32 linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    LOG_TEXTF("vertsrc:\n%s\n", src.vertex.c_str());
    LOG_TEXTF("fragsrc:\n%s\n", src.fragment.c_str());

    CAT_ASSERT(linked);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return Shader{ program };
}
