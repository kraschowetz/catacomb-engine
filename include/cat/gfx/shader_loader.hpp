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
        const std::string& path
    ) const THROWS;

    hash_t hash(
        const std::string& path
    ) const noexcept;

    void unload(Shader*) const;

    Shader load_basic(eBasicShaderType type);

private:
    std::hash<std::string> m_hasher;
};

}
