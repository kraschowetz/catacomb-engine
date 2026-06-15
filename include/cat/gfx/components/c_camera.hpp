#pragma once

#include <cat/config.hpp>
#include <cat/util/math.hpp>

namespace cat
{

enum eCameraType : u8
{
    ORTHOGRAPHIC,
    PERSPECTIVE
};

struct cCamera
{
    glm::mat4 projection;
    glm::ivec2 size;

    hash_t render_context_handle;
    eCameraType type;


    static void bind(const cCamera& self, const struct cTransform& transform);
};

}
