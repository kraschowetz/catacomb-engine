#pragma once

#include <cat/config.hpp>
#include <optional>
#include <string>
#include <vector>

namespace cat
{

// these structs break the naming conventions to automatically reflect with MSDF json

struct MSDFBounds
{
    f32 left, bottom, right, top;
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
    std::string type;
    f32 distanceRange;
    f32 distanceRangeMiddle;
    i32 size;
    i32 width;
    i32 height;
    std::string yOrigin;
};

struct MSDFMetrics
{
    f32 emSize;
    f32 lineHeight;
    f32 ascender;
    f32 descender;
    f32 underlineY;
    f32 underlineThickness;
};

struct MSDFKerningPair
{
    u32 unicode1;
    u32 unicode2;
    f32 advance;
};

struct MSDFAtlasJson
{
    MSDFAtlasMeta atlas;
    MSDFMetrics metrics;
    std::vector<MSDFGlyph> glyphs;
    std::vector<MSDFKerningPair> kerning;
};

}
