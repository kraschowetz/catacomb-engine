#include "cat/gfx/shader.hpp"
#include "cat/util/memory.hpp"
#include "cat/core/resource_manager.hpp"
#include "cat/core/input_manager.hpp"
#include "cat/gfx/gfx_engine.hpp"
#include "cat/gfx/vertex_buffer.hpp"
#include "cat/gfx/vertex_layout.hpp"
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

void _render_triangle()
{
    using namespace cat;

    VertexLayout layout;
    // position
    layout.push_f32(3);
    // color
    layout.push_f32(3);

    static VertexBuffer vbo{6 * sizeof(float), 3, eBufferType::VERTEX};
    static VertexArray vao{};

    constexpr f32 POSITION_BUFFER[9] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    const f32 COLOR_BUFFER[9] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

    // const u32 ids[3] = {0, 1, 2};

    vbo.buffer(POSITION_BUFFER, layout, 0);
    vbo.buffer(COLOR_BUFFER, layout, 1);

    vao.attr(vbo, layout);
    vao.bind();

    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
}

int main(int argc, char** argv)
{
    using namespace cat;

    GfxEngine::get();

    ResourceManager& resource_manager = CoreEngine::get().get_resource_manager();

    resource_manager.register_resource<Shader, ShaderLoader>();
    resource_manager.register_resource<Texture, TextureLoader>();

    Shared<Shader> shader = resource_manager
        .load<Shader, ShaderLoader>(
            "./res/shader.csl"
        );

    Shader& s1 = GfxEngine::get().get_basic_shader(eBasicShaderType::UNLIT_2D);

    SpriteAtlas atlas = {
        resource_manager.load<Texture, TextureLoader>("res/sprite.png"),
        glm::ivec2{8, 8}
    };

    cSprite sprite = atlas.get_sprite({0, 0});
    
    // bare-bones game loop
    while(!CoreEngine::get().get_input_manager().has_queued_exit())
    {
        CAT_BENCH_SCOPE("update loop", bench_marker);

        CoreEngine::get().update();

        if(CoreEngine::get().get_input_manager().is_key_just_released(eKeyType::SPACE))
        {
            LOG_TEXT("A has been pressed\n");
        }

        cTransform transform{
            .position = {},
            .scale = {1, 1, 1},
            .rotation = {},
        };

        GfxEngine::get().get_sprite_renderer().render_sprite(sprite, transform);

        // shader->bind();
        s1.bind();

        _render_triangle();

        GfxEngine::get().display();
    }

    { // ECS sample
        // create ECS & register components
        ECS& ecs = CoreEngine::get().get_ecs();
        ecs.register_component_index<i64>();
        
        // creante entities and add components
        EntityID entity_a = ecs.create_entity();
        ecs.add_component(entity_a, 12);

        // create a view and iterate trhough components
        auto view = ecs.view<int>();
        view.foreach([](int i){LOG_TEXTF("%d\n", i);});
        
        // delete components & entities
        ecs.remove_component<int>(entity_a);
        ecs.delete_entity(entity_a);
    }
    
    CAT_BENCH_DISPLAY(BENCHMARK_IN_SECONDS);
}
