#pragma once

#include <cat/util/math.hpp>
#include <cat/util/util.hpp>

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

    void set_uniform(const std::string& name, f32 val) const;
    void set_uniform(const std::string& name, i32 val) const;
    void set_uniform(const std::string& name, const glm::mat4& val) const;
    void set_uniform(const std::string& name, const glm::mat3& val) const;
    void set_uniform(const std::string& name, const glm::vec4& val) const;
    void set_uniform(const std::string& name, const glm::vec3& val) const;
    void set_uniform(const std::string& name, const glm::vec2& val) const;

    // TODO: implement uniform blocks

private:
    u32 m_handle;
};

}
