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

    void bind(const struct cWorldTransform& transform);
    void bind(const struct cTransform& transform);

    static cCamera create_ortho(const glm::ivec2& size);
};

}
