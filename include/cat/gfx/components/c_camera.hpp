#pragma once

#include "cat/gfx/gfx_engine.hpp"
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

    eRenderPass render_context_handle;
    eCameraType type;

    void bind(const struct cWorldTransform& transform);
    void bind(const struct cTransform& transform);

    static cCamera create_ortho(
        const glm::ivec2& size,
        f32 near = DEFAULT_ORTHO_NEAR,
        f32 far = DEFAULT_ORTHO_FAR,
        eRenderPass context = eRenderPass::MAIN_2D
    );
    static cCamera create_perspective(
        u8 fov,
        const glm::ivec2& size,
        f32 near = DEFAULT_PERSPECTIVE_NEAR,
        f32 far = DEFAULT_PERSPECTIVE_FAR,
        eRenderPass context = eRenderPass::MAIN_3D
    );

    static constexpr f32 DEFAULT_ORTHO_NEAR         = -1000.f;
    static constexpr f32 DEFAULT_ORTHO_FAR          = 1000.f;
    static constexpr f32 DEFAULT_PERSPECTIVE_NEAR   = 0.0001f;
    static constexpr f32 DEFAULT_PERSPECTIVE_FAR    = 1000.f;
};

// TODO:create a from_json method (if needed)

}
