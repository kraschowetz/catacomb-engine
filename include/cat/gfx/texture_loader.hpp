#pragma once

#include <cat/gfx/texture.hpp>
#include <string>

namespace cat
{

struct TextureLoader
{
public:
    TextureLoader();

    Texture load(const std::string& path) const THROWS;
    hash_t hash(const std::string& path) const noexcept;
    void unload(Texture* texture) const;
    
private:
    std::hash<std::string> m_hasher;
};

}
