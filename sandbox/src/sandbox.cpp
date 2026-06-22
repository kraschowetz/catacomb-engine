#include "cat/core/components/c_scene_tag.hpp"
#include "cat/core/components/c_transform.hpp"
#include "cat/core/scene.hpp"
#include "cat/gfx/components/c_camera.hpp"
#include "cat/gfx/components/c_sprite.hpp"
#include "cat/gfx/shader.hpp"
#include "cat/util/chrono.hpp"
#include "cat/util/memory.hpp"
#include "cat/core/resource_manager.hpp"
#include "cat/core/input_manager.hpp"
#include "cat/gfx/gfx_engine.hpp"
#include "cat/gfx/vertex_buffer.hpp"
#include "cat/util/logger.hpp"
#include "cat/core/ecs.hpp"
#include <cat/core/core_engine.hpp>
#include <cat/util/benchmark.hpp>
#include <cat/gfx/vertex_array.hpp>
#include <cat/gfx/gfx_util.hpp>
#include <cat/gfx/shader_loader.hpp>
#include <cat/gfx/texture_loader.hpp>
#include <cat/gfx/sprite_atlas.hpp>

#include <cat/util/cconf.hpp>

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
    Shader& basic_shader = GfxEngine::get().get_basic_shader(eBasicShaderType::UNLIT_2D);

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
    ecs.register_component_index<cWorldTransform>();
    ecs.register_component_index<cWorldHierarchy>();
    ecs.register_component_index<cSceneTag>();

    Scene scene;

    EntityID entity = scene.create_entity();
    
    ecs.add_component<cSprite>(entity, sprite);

    ecs.add_component<cCamera>(entity, cCamera::create_ortho({800, 600}));

    seconds_t last_time = CoreEngine::get().get_chrono().current_seconds();

    set_transform_scale(entity, glm::vec2{4.f, 4.f});
    set_transform_rotation(entity, 45.f);

    // bare-bones game loop
    while(!CoreEngine::get().get_input_manager().has_queued_exit())
    {
        CAT_BENCH_SCOPE("update loop", bench_marker);

        CoreEngine::get().update();
        scene.update();

        if(CoreEngine::get().get_input_manager().is_key_just_released(eKeyType::SPACE))
        {
            LOG_TEXT("A has been pressed\n");
        }

        auto camera_view = ecs.view<cCamera, cWorldTransform>();

        camera_view.foreach([](cCamera& cam, cWorldTransform& trans){
            cam.bind(trans);
        });

        basic_shader.bind();
        basic_shader.set_texture_atlas(atlas);
        GfxEngine::get().bind_render_context(GfxEngine::MAIN_2D_CONTEXT, basic_shader);

        GfxEngine::get().prepare(eRenderPass::MAIN_2D);

        auto sprite_view = ecs.view<cSprite, cWorldTransform>();
        sprite_view.foreach([](cSprite& spr, cWorldTransform& trans){
            GfxEngine::get().get_sprite_renderer().render_sprite(spr, trans);
        });

        // csl_shader->set_uniform("u_my_uniform", 1);

        GfxEngine::get().display();

        // print fps each second
        if(CoreEngine::get().get_chrono().current_seconds() >= last_time + 1.f)
        {
            last_time = CoreEngine::get().get_chrono().current_seconds();
            LOG_TEXTF("FPS: %u\n", CoreEngine::get().get_chrono().get_fps());
        }
    }

    CAT_BENCH_DISPLAY(BENCHMARK_IN_SECONDS);
}
