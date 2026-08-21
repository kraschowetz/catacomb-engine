#include "cat/core/components/c_transform.hpp"
#include "cat/core/scene.hpp"
#include "cat/gfx/components/c_camera.hpp"
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

#include <cat/gfx/font.hpp>
#include <cat/gfx/font_loader.hpp>

#include <unistd.h>

int main(int argc, char** argv)
{
    using namespace cat;

    // load resources
    ResourceManager& resource_manager = CoreEngine::get().get_resource_manager();

    resource_manager.register_resource<Shader, ShaderLoader>();
    resource_manager.register_resource<Texture, TextureLoader>();
    resource_manager.register_resource<Font, FontLoader>();

    Shader& text_shader = GfxEngine::get().get_basic_shader(eBasicShaderType::TEXT_2D);
    Shader& sprite_shader = GfxEngine::get().get_basic_shader(eBasicShaderType::UNLIT_2D);
    Shared<Shader> csl_shader = resource_manager.load<Shader, ShaderLoader>(
        "./res/shader.csl"
    );

    Shared<Font> font = resource_manager.load<Font, FontLoader>(
        "./res/font_atlas.png", "./res/font_atlas.json"
    );

    SpriteAtlas atlas = {
        resource_manager.load<Texture, TextureLoader>("res/sprite.png"),
        glm::ivec2{8, 8}
    };

    ECS& ecs = CoreEngine::get().get_ecs();

    Scene& scene = CoreEngine::get().load_scene("res/scene.json");
    CoreEngine::get().set_active_scene(scene.get_scene_id());

    EntityID camera = scene.create_entity();
    EntityID text = scene.create_entity();
    EntityID sprite = scene.create_entity();

    ecs.add_component<cCamera>(camera, cCamera::create_ortho(
        {800, 600},
        -1000.f,
        1000.f,
        eRenderPass::MAIN_2D
    ));
    ecs.add_component<cText>(text, cText{"ola, mundo!", font});

    Watcher<cSprite> sprite_component = 
        ecs.add_component<cSprite>(sprite, atlas.get_sprite({0, 0}));
    sprite_component->shader = csl_shader;

    seconds_t last_time = CoreEngine::get().get_chrono().current_seconds();

    set_transform_scale(sprite, glm::vec2{4.f, 4.f});
    set_transform_position(sprite, glm::vec2{0.f, 0.f});

    set_transform_position(text, glm::vec3{0.f, 0.f, 0.f});
    set_transform_rotation(text, 15.f);
    set_transform_scale(text, glm::vec2{2, 2});

    GfxEngine::get().create_render_context(eRenderPass::UI_TEXT, &text_shader);
    GfxEngine::get().create_render_context(eRenderPass::MAIN_2D, &sprite_shader);

    // bare-bones game loop
    while(!CoreEngine::get().get_input_manager().has_queued_exit())
    {
        CAT_BENCH_SCOPE("update loop", bench_marker);

        CoreEngine::get().update();

        if(CoreEngine::get().get_input_manager().is_key_just_released(eKeyType::SPACE))
        {
            LOG_TEXT("SPACE has been pressed");
        }

        GfxEngine::get().prepare(eRenderPass::MAIN_2D);

        auto camera_view = ecs.view<cCamera, cWorldTransform>();

        camera_view.foreach([](cCamera& cam, cWorldTransform& trans){
            cam.bind(trans);
            cam.copy_to_context(eRenderPass::UI_TEXT);
        });

        auto sprite_view = ecs.view<cSprite, cWorldTransform>();
        sprite_view.foreach([](cSprite& spr, cWorldTransform& trans){
            GfxEngine::get().get_sprite_renderer().render_sprite(spr, trans);
        });

        GfxEngine::get().prepare(eRenderPass::UI_TEXT);

        auto text_view = ecs.view<cText, cTransform>();
        text_view.foreach([](cText& text, cTransform& trans){
            GfxEngine::get().get_text_renderer().render_text(text, trans);
        });

        GfxEngine::get().display();

        // print fps each second
        if(CoreEngine::get().get_chrono().current_seconds() >= last_time + 1.f)
        {
            last_time = CoreEngine::get().get_chrono().current_seconds();
            LOG_TEXT("FPS: {}", CoreEngine::get().get_chrono().get_fps());
        }
    }

    CoreEngine::get().unload_scene(scene.get_scene_id());

    CAT_BENCH_DISPLAY(BENCHMARK_IN_SECONDS);
}
