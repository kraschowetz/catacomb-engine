#include <cat/gfx/text/font_loader.hpp>

#include "cat/error.hpp"
#include "cat/gfx/gfx_util.hpp"
#include "cat/gfx/text/font.hpp"
#include "cat/gfx/text/msdf_schema.hpp"

#include <cerrno>
#include <cstddef>
#include <glaze/json/read.hpp>
#include <string>

#include <stb/stb_image.h>

using namespace cat;

static Texture _load_font_texture(const std::string& path) THROWS
{
    glm::ivec2 texture_size;
    u32 handle;

    byte_t* raw_bytes = stbi_load(
        path.c_str(),
        &texture_size.x,
        &texture_size.y,
        NULL,
        STBI_rgb_alpha
    );

    if(!raw_bytes)
    {
        throw Exception{eErrorCode::FILE_NOT_FOUND};
    }

    glGenTextures(1, &handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        texture_size.x,
        texture_size.y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        raw_bytes
    );

    stbi_image_free(raw_bytes);

    return Texture{texture_size, handle, GL_TEXTURE_2D};
}

void FontLoader::build_font_glyph_table(Font& font, const MSDFAtlasJson& raw) const
{
    const f32 atlas_width = static_cast<f32>(raw.atlas.width);
    const f32 atlas_height = static_cast<f32>(raw.atlas.height);

    for(const auto& glyph : raw.glyphs)
    {
        GlyphInfo info;
        info.advance = glyph.advance;

        if(glyph.planeBounds && glyph.atlasBounds)
        {
            info.plane_min = {glyph.planeBounds->left, glyph.planeBounds->bottom};
            info.plane_max = {glyph.planeBounds->right, glyph.planeBounds->top};
            info.uv_min = {
                glyph.atlasBounds->left / atlas_width,
                glyph.atlasBounds->bottom / atlas_height
            };
            info.uv_max = {
                glyph.atlasBounds->right / atlas_width,
                glyph.atlasBounds->top / atlas_height
            };
            info.has_ink = true;
        }

        font.m_glyphs.emplace(glyph.unicode, info);
    };
}

Font FontLoader::load(
    const std::string& image_path,
    const std::string& json_path
) const THROWS 
{
    MSDFAtlasJson raw;
    auto error = glz::read_file_json(raw, json_path, std::string{});
    
    if(error) throw Exception{eErrorCode::FILE_NOT_FOUND};

    Font font;

    font.m_atlas = _load_font_texture(image_path);
    font.m_pixel_range = raw.atlas.distanceRange;
    font.m_line_height = raw.metrics.lineSize;
    font.m_ascender = raw.metrics.ascender;
    font.m_descender = raw.metrics.descender;

    build_font_glyph_table(font, raw);
    
    return font;
}

hash_t FontLoader::hash(
    const std::string& image_path,
    const std::string& json_path
) const noexcept
{
    return m_hasher(image_path) ^ m_hasher(json_path);
}

void FontLoader::unload(Font* font) const
{
    u32 temp_handle = font->m_atlas.get_handle();
    glDeleteTextures(1, &temp_handle);
    font->m_atlas.get_handle_ref() = 0;
}
