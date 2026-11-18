#pragma once

#include <cat/gfx/font.hpp>

namespace cat
{

struct MSDFAtlasJson;

struct FontLoader
{
public:
    Font load(
        const std::string& image_path,
        const std::string& json_path
    ) const THROWS;

    hash_t hash(
        const std::string& image_path,
        const std::string& json_path
    ) const noexcept;

    void unload(Font* font) const;
private:
        void build_font_glyph_table(Font& font, const MSDFAtlasJson& raw) const;

private:
    std::hash<std::string> m_hasher;
};

}
