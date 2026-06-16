#include "cat/core/components/c_transform.hpp"
#include "cat/gfx/components/c_camera.hpp"
#include "cat/gfx/components/c_sprite.hpp"
#include "cat/gfx/shader.hpp"
#include "cat/util/memory.hpp"
#include "cat/core/resource_manager.hpp"
#include "cat/core/input_manager.hpp"
#include "cat/gfx/gfx_engine.hpp"
#include "cat/gfx/vertex_buffer.hpp"
#include "cat/util/logger.hpp"
#include "cat/core/ecs.hpp"
#include <cat/core/engine.hpp>
#include <cat/util/benchmark.hpp>
#include <cat/gfx/vertex_array.hpp>
#include <cat/gfx/gfx_util.hpp>
#include <cat/gfx/shader_loader.hpp>
#include <cat/gfx/texture_loader.hpp>
#include <cat/gfx/sprite_atlas.hpp>

#include <unistd.h>

int main(int argc, char** argv)
{
    using namespace cat;

    GfxEngine::get();

    // load resources
    ResourceManager& resource_manager = CoreEngine::get().get_resource_manager();

    resource_manager.register_resource<Shader, ShaderLoader>();
    resource_manager.register_resource<Texture, TextureLoader>();

    Shared<Shader> csl_shader = resource_manager
        .load<Shader, ShaderLoader>(
            "./res/shader.csl"
        );

    // could also use a basic shader like this
    // Shader& basic_shader = GfxEngine::get().get_basic_shader(eBasicShaderType::UNLIT_2D);

    SpriteAtlas atlas = {
        resource_manager.load<Texture, TextureLoader>("res/sprite.png"),
        glm::ivec2{8, 8}
    };

    cSprite sprite = atlas.get_sprite({0, 0});

    // register entities
    ECS& ecs = CoreEngine::get().get_ecs();
    ecs.register_component_index<cSprite>();
    ecs.register_component_index<cTransform>();
    ecs.register_component_index<cCamera>();

    EntityID entity = ecs.create_entity();

    ecs.add_component<cTransform>(entity, {
        .position{4, 4, 0},
        .scale{1},
        .rotation{}
    });
    ecs.add_component<cSprite>(entity, sprite);

    ecs.add_component<cCamera>(entity, {
        .projection = glm::ortho(0.f, 800.f, 0.f, 600.f, -1.f, 1.f),
        .size = {800, 600},
        .render_context_handle = GfxEngine::MAIN_2D_CONTEXT,
        .type = eCameraType::ORTHOGRAPHIC,
    });

    // bare-bones game loop
    while(!CoreEngine::get().get_input_manager().has_queued_exit())
    {
        CAT_BENCH_SCOPE("update loop", bench_marker);

        CoreEngine::get().update();

        if(CoreEngine::get().get_input_manager().is_key_just_released(eKeyType::SPACE))
        {
            LOG_TEXT("A has been pressed\n");
        }

        auto camera_view = ecs.view<cCamera, cTransform>();

        camera_view.foreach([](cCamera& cam, cTransform& trans){
            cCamera::bind(cam, trans);
        });

        auto sprite_view = ecs.view<cSprite, cTransform>();

        sprite_view.foreach([](cSprite& spr, cTransform& trans){
            GfxEngine::get().get_sprite_renderer().render_sprite(spr, trans);
        });

        csl_shader->bind();
        GfxEngine::get().bind_render_context(GfxEngine::MAIN_2D_CONTEXT, *csl_shader);

        csl_shader->set_uniform("u_my_uniform", 1);

        GfxEngine::get().display();
    }

    CAT_BENCH_DISPLAY(BENCHMARK_IN_SECONDS);
}
