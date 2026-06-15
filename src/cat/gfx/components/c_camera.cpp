#include <cat/gfx/components/c_camera.hpp>

#include <cat/core/components/c_transform.hpp>
#include "cat/gfx/gfx_engine.hpp"

using namespace cat;

void cCamera::bind(const cCamera &self, const cTransform& transform)
{
    Watcher<RenderContext> ctx = 
        GfxEngine::get().get_render_context(self.render_context_handle);

    // TODO: switch (self.type)

    ctx->view = glm::translate(
        glm::mat4{1.f},
        -glm::vec3{
            transform.position.x - ((f32)self.size.x / 2),
            transform.position.y - ((f32)self.size.y / 2),
            transform.position.z
        }
    );
    ctx->projection = self.projection;
}

