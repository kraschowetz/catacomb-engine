#include "cat/core/input.hpp"
#include "cat/core/memory.hpp"
#include "cat/core/resource_manager.hpp"
#include "cat/core/input_manager.hpp"
#include "cat/util/logger.hpp"
#include "cat/ecs/ecs.hpp"
#include <cat/core/engine.hpp>
#include <cat/util/benchmark.hpp>
#include <cat/gfx/vertex_array.hpp>
#include <cat/gfx/gfx_util.hpp>
#include <cat/gfx/shader_loader.hpp>
#include <functional>

#include <string>
#include <unistd.h>

void _render_triangle(cat::VertexBuffer& vbo, cat::VertexArray& vao)
{
    using namespace cat;
    VertexLayout layout;
    // position
    layout.push_f32(3);
    // color
    layout.push_f32(3);

    constexpr f32 POSITION_BUFFER[9] = {
        -1, -1, 0,
        1, -1, 0,
        0, 1, 0
    };

    constexpr f32 COLOR_BUFFER[9] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

    constexpr u32 ids[3] = {0, 1, 2};

    vbo.buffer(POSITION_BUFFER, layout, 0);
    vbo.buffer(COLOR_BUFFER, layout, 1);

    vao.attr(vbo, layout);
    vao.bind();

    GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, ids));
}

struct ResourceSample
{
    u64 data;

    ResourceSample(const std::string& s)
    {
        data = std::hash<std::string>{}(s);
    }
};

struct ResourceSampleLoader
{
    ResourceSample load(const std::string& s)
    {
        LOG_TEXT("laoding resource!\n");
        return ResourceSample(s);
    }

    hash_t hash(const std::string& s)
    {
        return std::hash<std::string>{}(s);
    }

    void unload(ResourceSample* r)
    {
        LOG_TEXT("unlaoding resource!\n");
        r->data = 0;
    }

};

int main(int argc, char** argv)
{
    LOG_TEXT("hello\n");

    cat::Engine::init();

    cat::ResourceManager::get().register_resource<cat::Shader, cat::ShaderLoader>();
    cat::Shared<cat::Shader> resource = cat::ResourceManager::get()
        .load<cat::Shader, cat::ShaderLoader>(
            "pretend/this/is/a/file",
            ""
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
