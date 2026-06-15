#pragma once

#include <cat/util/math.hpp>
#include <cat/util/util.hpp>

#include <cat/gfx/csl/csl.hpp>  // IWYU pragma: export

#include <unordered_map>

namespace cat {


class Shader
{
public:
    Shader(u32 handle);
    Shader();

    ~Shader() = default;

    NO_COPY(Shader);

    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    u32 get_handle() const;
    u32& get_handle_ref();

    void bind() const;
    void unbind() const;

    void set_model_matrix(const glm::mat4& val) const;
    void set_view_matrix(const glm::mat4& val) const;
    void set_projection_matrix(const glm::mat4& val) const;

    void set_uniform(const std::string& name, f32 val) const;
    void set_uniform(const std::string& name, i32 val) const;
    void set_uniform(const std::string& name, const glm::mat4& val) const;
    void set_uniform(const std::string& name, const glm::mat3& val) const;
    void set_uniform(const std::string& name, const glm::vec4& val) const;
    void set_uniform(const std::string& name, const glm::vec3& val) const;
    void set_uniform(const std::string& name, const glm::vec2& val) const;

    // TODO: implement uniform blocks
    
public:
    struct UniformCache
    {
        i32 model_matrix        = -1;
        i32 view_matrix         = -1;
        i32 projection_matrix   = -1;

        std::unordered_map<hash_t, i32> user_uniforms;
    };

private:
    i32 get_uniform_location(const std::string& name) const;

private:
    u32 m_handle;

    UniformCache m_uniform_cache;

    friend struct ShaderLoader;
};

}
