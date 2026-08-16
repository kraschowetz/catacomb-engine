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

            ctx->view = glm::inverse(camera_matrix);
            break;
        }
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

            ctx->view = glm::inverse(camera_matrix);
            break;
        }
        case eCameraType::PERSPECTIVE:

            glm::mat4 camera_matrix {
                glm::vec4{right, 0.f},
                glm::vec4{up, 0.f},
                glm::vec4{forward, 0.f},
                glm::vec4{position, 1.f},
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
