#pragma once

#include <cat/config.hpp>
#include <string>

namespace cat
{

class Shader;

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
