#include <cat/gfx/components/c_camera.hpp>

#include <cat/core/components/c_transform.hpp>
#include <glaze/reflection/get_name.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include "cat/core/components/c_world_transform.hpp"
#include "cat/gfx/gfx_engine.hpp"

using namespace cat;

void cCamera::bind(const cTransform& transform)
{
    Watcher<RenderContext> ctx = 
        GfxEngine::get().get_render_context(this->render_context_handle);

    switch(this->type)
    {
        case eCameraType::ORTHOGRAPHIC:
        {
            glm::vec3 half_size{(f32)this->size.x / 2.f, (f32)this->size.y / 2.f, 0.f};

            glm::mat4 camera_matrix =
                glm::translate(glm::mat4{1.f}, transform.position)
                * glm::mat4_cast(transform.rotation)
                * glm::translate(glm::mat4{1.f}, -half_size);

            ctx->set_view(glm::inverse(camera_matrix));
            break;
        }
        case eCameraType::PERSPECTIVE:
            glm::mat4 camera_matrix = glm::translate(glm::mat4{1.f}, transform.position)
                * glm::mat4_cast(transform.rotation);

            ctx->set_view(glm::inverse(camera_matrix));
        break;
    }
    ctx->set_projection(this->projection);
}

void cCamera::bind(const cWorldTransform& transform)
{
    Watcher<RenderContext> ctx = 
        GfxEngine::get().get_render_context(this->render_context_handle);

    glm::vec3 position = transform.get_position();

    const glm::mat4& mat = transform.matrix;
    glm::vec3 right   = glm::normalize(glm::vec3(mat[0]));
    glm::vec3 up      = glm::normalize(glm::vec3(mat[1]));
    glm::vec3 forward = glm::cross(right, up); // glm::normalize(glm::vec3(mat[2]));

    glm::mat4 rotation{
        glm::vec4{right, 0.f},
        glm::vec4{up, 0.f},
        glm::vec4{forward, 0.f},
        glm::vec4{0.f, 0.f, 0.f, 1.f},
    };
    
    switch(this->type)
    {
        case eCameraType::ORTHOGRAPHIC:
        {
            glm::vec3 half_size{(f32)this->size.x / 2.f, (f32)this->size.y / 2.f, 0.f};

            glm::mat4 camera_matrix = 
                    glm::translate(glm::mat4{1.f}, position)
                    * rotation
                    * glm::translate(glm::mat4{1.f}, -half_size);

            ctx->set_view(glm::inverse(camera_matrix));
            break;
        }
        case eCameraType::PERSPECTIVE:

            glm::mat4 camera_matrix {
                glm::vec4{right, 0.f},
                glm::vec4{up, 0.f},
                glm::vec4{forward, 0.f},
                glm::vec4{position, 1.f},
            };

            ctx->set_view(glm::inverse(camera_matrix));
        break;
    }

    ctx->set_projection(this->projection);
}

void cCamera::copy_to_context(eRenderPass context) const
{
    Watcher<RenderContext> src_ctx = 
        GfxEngine::get().get_render_context(this->render_context_handle);
    Watcher<RenderContext> dest_ctx = 
        GfxEngine::get().get_render_context(context);

    dest_ctx->set_view(src_ctx->get_view());
    dest_ctx->set_projection(src_ctx->get_projection());
}

cCamera cCamera::create_ortho(
    const glm::ivec2 &size,
    f32 near,
    f32 far,
    eRenderPass context
)
{
    return cCamera{
        .projection = glm::ortho(
            0.f,
            (f32) size.x,
            0.f,
            (f32) size.y,
            near,
            far
        ),
        .size = size,
        .render_context_handle = context,
        .type = eCameraType::ORTHOGRAPHIC
    };
}

cCamera cCamera::create_perspective(
    u8 fov, 
    const glm::ivec2& size,
    f32 near,
    f32 far,
    eRenderPass context
)
{
    f32 aspect = (f32)(size.x) / (f32)(size.y);

    return cCamera {
        .projection = glm::perspective((f32) fov, aspect, near, far),
        .size = size,
        .render_context_handle = context,
        .type = eCameraType::PERSPECTIVE
    };
}
