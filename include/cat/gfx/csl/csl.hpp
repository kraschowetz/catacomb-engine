#pragma once

#include <string>

#include "hook.hpp"         // IWYU pragma: export

#include "preamble.hpp"     // IWYU pragma: export
#include "standard_2d.hpp"  // IWYU pragma: export

#define CAT_CSL_VERTEX_SECTION "@section VERTEX"
#define CAT_CSL_FRAGMENT_SECTION "@section FRAGMENT"

// csl stands for Catacomb Shading Language
// this is just a neat wrapper for glsl and the implementation of an ubershader
namespace cat::csl
{

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
};

// brief: reads a csl file & split it into hooks for vertex & fragment shader
extern ShaderSource split_file(const std::string& path);

extern bool has_hook(const std::string& source, const char* hook_signature);

}
