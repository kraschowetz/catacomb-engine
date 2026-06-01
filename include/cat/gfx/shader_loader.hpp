#pragma once

#include <cat/config.hpp>
#include <cat/gfx/shader.hpp>
#include <string>

namespace cat
{

struct ShaderLoader
{
public:
    Shader load(
        const std::string& vertex, 
        const std::string& fragment
    ) const THROWS;

    hash_t hash(
        const std::string& vertex, 
        const std::string& fragment
    ) const noexcept;

    void unload(Shader*) const;

private:
    std::hash<std::string> m_hasher;
};

}
