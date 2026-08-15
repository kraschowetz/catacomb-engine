#include <cat/gfx/components/c_camera.hpp>

#include <cat/core/components/c_transform.hpp>
#include <glaze/reflection/get_name.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "cat/core/components/c_world_transform.hpp"
#include "cat/gfx/gfx_engine.hpp"

using namespace cat;

void cCamera::bind(const cTransform& transform)
{
    Watcher<RenderContext> ctx = 
        GfxEngine::get().get_render_context(this->render_context_handle);

    // TODO: switch (this->type)

    switch(this->type)
    {
        case eCameraType::ORTHOGRAPHIC:
            ctx->view = glm::translate(
                glm::mat4{1.f},
                -glm::vec3{
                    transform.position.x - ((f32)this->size.x / 2),
                    transform.position.y - ((f32)this->size.y / 2),
                    transform.position.z
                }
            );
        break;
        case eCameraType::PERSPECTIVE:
            glm::mat4 camera_matrix = glm::translate(glm::mat4{1.f}, transform.position)
                * glm::mat4_cast(transform.rotation);

            ctx->view = glm::inverse(camera_matrix);
        break;
    }
    ctx->projection = this->projection;
}

void cCamera::bind(const cWorldTransform& transform)
{
    Watcher<RenderContext> ctx = 
        GfxEngine::get().get_render_context(this->render_context_handle);

    // TODO: switch (this->type)

    glm::vec3 position = transform.get_position();
    
    switch(this->type)
    {
        case eCameraType::ORTHOGRAPHIC:
            ctx->view = glm::translate(
                glm::mat4{1.f},
                -glm::vec3{
                    position.x - ((f32)this->size.x / 2),
                    position.y - ((f32)this->size.y / 2),
                    position.z
                }
            );
        break;
        case eCameraType::PERSPECTIVE:
            const glm::mat4& mat = transform.matrix;
            glm::vec3 right   = glm::normalize(glm::vec3(mat[0]));
            glm::vec3 up      = glm::normalize(glm::vec3(mat[1]));
            glm::vec3 forward = glm::normalize(glm::vec3(mat[2]));

            glm::mat4 camera_matrix {
                glm::vec4{right, 0.f},
                glm::vec4{up, 0.f},
                glm::vec4{forward, 0.f},
                glm::vec4{position, 0.f},
            };

            ctx->view = glm::inverse(camera_matrix);
        break;
    }

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

cCamera cCamera::create_perspective(u8 fov, f32 aspect, const glm::ivec2& size)
{
    return cCamera {
        .projection = glm::perspective((f32) fov, aspect, 0.01f, 1000.f),
        .size = size,
        .render_context_handle = GfxEngine::MAIN_3D_CONTEXT,
        .type = eCameraType::PERSPECTIVE
    };
}
