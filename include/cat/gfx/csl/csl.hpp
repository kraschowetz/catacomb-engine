#pragma once

#include <cat/config.hpp>

#include <string>

#include "hook.hpp"                     // IWYU pragma: export

#include "basic_shaders/preamble.hpp"   // IWYU pragma: export
#include "basic_shaders/unlit_2d.hpp"   // IWYU pragma: export

// TODO: @uniform directive
// TODO: @output directive (should be a contract!)
// TODO: @include directive

#define CAT_CSL_VERTEX_SECTION "@section VERTEX"
#define CAT_CSL_FRAGMENT_SECTION "@section FRAGMENT"
#define CAT_CSL_TARGET_DIRECTIVE "@target"

namespace cat
{

enum class eBasicShaderType : u8
{
    UNLIT_2D,
    COUNT
};

}

constexpr const char CSL_BASIC_SHADER_PATH[] = "__CSL__BASIC__";

// csl stands for Catacomb Shading Language
// this is just a neat wrapper for glsl and the implementation of an ubershader
namespace cat::csl
{

struct BasicShaderDef
{
    const char* vertex;
    const char* fragment;
};

inline constexpr BasicShaderDef BASIC_SHADERS_LIST[] = {
    {BASIC_UNLIT_2D_VERTEX, BASIC_UNLIT_2D_FRAGMENT}
};

struct TargetDef
{
    const char* name;
    eBasicShaderType type;
};

inline constexpr TargetDef SHADER_TARGET_LOOKUP_TABLE[]
{
    {"UNLIT_2D", eBasicShaderType::UNLIT_2D}
};

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
    eBasicShaderType base_behaviour = eBasicShaderType::COUNT;
};

// brief: reads a csl file & split it into hooks for vertex & fragment shader
extern ShaderSource split_file(const std::string& path);

extern bool has_hook(const std::string& source, const char* hook_signature);

}
