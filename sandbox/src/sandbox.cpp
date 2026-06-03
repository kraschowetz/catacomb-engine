#include "cat/core/input.hpp"
#include "cat/core/memory.hpp"
#include "cat/core/resource_manager.hpp"
#include "cat/core/input_manager.hpp"
#include "cat/gfx/vertex_buffer.hpp"
#include "cat/gfx/vertex_layout.hpp"
#include "cat/util/logger.hpp"
#include "cat/ecs/ecs.hpp"
#include <cat/core/engine.hpp>
#include <cat/util/benchmark.hpp>
#include <cat/gfx/vertex_array.hpp>
#include <cat/gfx/gfx_util.hpp>
#include <cat/gfx/shader_loader.hpp>

#include <unistd.h>

void _raw_render_triangle()
{
    static bool initialized = false;
    static u32 vao, vbo;

    if(!initialized)
    {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        initialized = true;
    }

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void _render_triangle()
{
    using namespace cat;

    VertexLayout layout;
    // position
    layout.push_f32(3);
    // color
    layout.push_f32(3);

    static cat::VertexBuffer vbo{6 * sizeof(float), 3, cat::eBufferType::VERTEX};
    static cat::VertexArray vao{};

    constexpr f32 POSITION_BUFFER[9] = {
        -1, -1, 0,
        1, -1, 0,
        0, 1, 0
    };

    const f32 COLOR_BUFFER[9] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

    const u32 ids[3] = {0, 1, 2};

    vbo.buffer(POSITION_BUFFER, layout, 0);
    vbo.buffer(COLOR_BUFFER, layout, 1);

    vao.attr(vbo, layout);
    vao.bind();

    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
}

int main(int argc, char** argv)
{
    cat::Engine::init();

    cat::ResourceManager::get().register_resource<cat::Shader, cat::ShaderLoader>();
    cat::Shared<cat::Shader> shader = cat::ResourceManager::get()
        .load<cat::Shader, cat::ShaderLoader>(
            "./res/shader.vert",
            "./res/shader.frag"
        );

    
    // bare-bones game loop
    while(!cat::input::has_queued_exit())
    {
        CAT_BENCH_SCOPE("update loop", bench_marker);

        cat::Engine::update();  

        if(cat::input::is_key_just_released(cat::eKeyType::SPACE))
        {
            LOG_TEXT("A has been pressed\n");
        }

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        shader->bind();

        _render_triangle();

        cat::Engine::display();
    }

    { // ECS sample
        // create ECS & register components
        cat::ECS ecs;
        ecs.register_component_index<i64>();
        
        // creante entities and add components
        cat::EntityID entity_a = ecs.create_entity();
        ecs.add_component(entity_a, 12);

        // create a view and iterate trhough components
        auto view = ecs.view<int>();
        view.foreach([](int i){LOG_TEXTF("%d\n", i);});
        
        // delete components & entities
        ecs.remove_component<int>(entity_a);
        ecs.delete_entity(entity_a);
    }
    
    CAT_BENCH_DISPLAY(BENCHMARK_IN_SECONDS);
    cat::Engine::quit();
}
