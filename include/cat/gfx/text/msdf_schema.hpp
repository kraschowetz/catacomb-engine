#pragma once

#include <cat/config.hpp>
#include <optional>
#include <vector>

namespace cat
{

// these structs break the naming conventions to automatically reflect with MSDF json

struct MSDFBounds
{
    f32 left, right, top, bottom;
};

struct MSDFGlyph
{
    u32 unicode;
    f32 advance;
    std::optional<MSDFBounds> planeBounds;
    std::optional<MSDFBounds> atlasBounds;
};

struct MSDFAtlasMeta
{
    f32 distanceRange;
    i32 width, height;
};

struct MSDFMetrics
{
    f32 emSize, lineSize, ascender, descender;
};

struct MSDFAtlasJson
{
    MSDFAtlasMeta atlas;
    MSDFMetrics metrics;
    std::vector<MSDFGlyph> glyphs;
};

}
