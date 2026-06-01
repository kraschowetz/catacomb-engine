#include <cat/gfx/shader_loader.hpp>

#include <cat/gfx/gfx_util.hpp>
#include <cat/gfx/shader.hpp>
#include <cat/error.hpp>

#include <cstdlib>
#include <fstream>
#include <sstream>


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

static u32 _compile(const std::string& path, GLenum type)
{
    std::ifstream file(path);

    if(!file.is_open())
    {
        std::stringstream stream;
        stream << "failed to load shader (" << path << ")!";
        throw Exception {
            eErrorCode::FILE_NOT_FOUND,
            stream.str()
        };
    }

    std::string line, code;

    while (std::getline(file, line))
    {
        code.append(line + "\n");
    }

    file.close();

    u32 shader_handle = glCreateShader(type);

    const char* code_ptr = code.c_str();
    
    glShaderSource(shader_handle, 1, &code_ptr, NULL);
    glCompileShader(shader_handle);

    i32 compiled;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
        LOG_SHADER_COMPILE_ERROR(code.c_str(), shader_handle, path.c_str());
    }

    return shader_handle;
}

Shader ShaderLoader::load(
    const std::string &vertex, 
    const std::string &fragment
) const THROWS
{
    u32 vs = _compile(vertex, GL_VERTEX_SHADER);
    u32 fs = _compile(fragment, GL_FRAGMENT_SHADER);

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
    const std::string &vertex, 
    const std::string &fragment
) const noexcept
{
    return m_hasher(vertex) ^ m_hasher(fragment);
}

void ShaderLoader::unload(Shader* shader) const
{
    glDeleteProgram(shader->get_handle());
}
