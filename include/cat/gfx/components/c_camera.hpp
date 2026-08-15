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
    static cCamera create_perspective(u8 fov, f32 aspect, const glm::ivec2& size);
};

// TODO:create a from_json method (if needed)

}
