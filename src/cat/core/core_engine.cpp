#include "cat/error.hpp"
#include <cat/core/core_engine.hpp>

#include <cat/core/input_manager.hpp>
#include <cat/util/chrono.hpp>
#include <cat/core/resource_manager.hpp>

using namespace cat;

/*static*/ CoreEngine& CoreEngine::get()
{
    static CoreEngine instance;
    return instance;
}

CoreEngine::CoreEngine()
{
}

CoreEngine::~CoreEngine()
{
    m_loaded_scenes.clear();
}

void CoreEngine::update()
{
    m_chrono.update();
    m_input_manager.update();
}

const Chrono& CoreEngine::get_chrono() const
{
    return m_chrono;
}

const InputManager& CoreEngine::get_input_manager() const
{
    return m_input_manager;
}

ResourceManager& CoreEngine::get_resource_manager()
{
    return m_resource_manager;
}

ECS& CoreEngine::get_ecs()
{
    return m_ecs;
}

Scene& CoreEngine::get_current_scene()
{
    for(Scene& s : m_loaded_scenes)
    {
        if(s.get_scene_id() == m_current_active_scene)
            return s;
    }

    throw Exception{eErrorCode::UNKNOWN, "no scene loaded"};
}

Scene& CoreEngine::create_scene()
{
    m_loaded_scenes.emplace_back(m_last_scene_id++);
    return m_loaded_scenes[m_loaded_scenes.size() - 1];
}

void CoreEngine::set_active_scene(u32 scene_id)
{
    m_current_active_scene = scene_id;
}
