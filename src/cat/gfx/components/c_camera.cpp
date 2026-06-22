#include <cat/gfx/components/c_camera.hpp>

#include <cat/core/components/c_transform.hpp>
#include "cat/core/components/c_world_transform.hpp"
#include "cat/gfx/gfx_engine.hpp"

using namespace cat;

void cCamera::bind(const cTransform& transform)
{
    Watcher<RenderContext> ctx = 
        GfxEngine::get().get_render_context(this->render_context_handle);

    // TODO: switch (this->type)

    ctx->view = glm::translate(
        glm::mat4{1.f},
        -glm::vec3{
            transform.position.x - ((f32)this->size.x / 2),
            transform.position.y - ((f32)this->size.y / 2),
            transform.position.z
        }
    );
    ctx->projection = this->projection;
}

void cCamera::bind(const cWorldTransform& transform)
{
    Watcher<RenderContext> ctx = 
        GfxEngine::get().get_render_context(this->render_context_handle);

    // TODO: switch (this->type)

    glm::vec3 position = transform.get_position();

    ctx->view = glm::translate(
        glm::mat4{1.f},
        -glm::vec3{
            position.x - ((f32)this->size.x / 2),
            position.y - ((f32)this->size.y / 2),
            position.z
        }
    );
    ctx->projection = this->projection;
}

cCamera cCamera::create_ortho(const glm::ivec2 &size)
{
    return cCamera{
        .projection = glm::ortho(
            0.f,
            (f32) size.x,
            0.f,
            (f32) size.y,
            -1.f,
            1.f
        ),
        .size = size,
        .render_context_handle = GfxEngine::MAIN_2D_CONTEXT,
        .type = eCameraType::ORTHOGRAPHIC
    };
}
