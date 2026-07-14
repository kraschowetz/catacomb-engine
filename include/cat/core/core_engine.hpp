#pragma once

#include "cat/core/scene.hpp"
#include <cat/config.hpp>
#include <cat/util/memory.hpp>
#include <cat/core/ecs.hpp>
#include <cat/core/input_manager.hpp>
#include <cat/core/resource_manager.hpp>
#include <cat/util/chrono.hpp>

namespace cat
{

class CoreEngine
{
public:
    // brief: get a reference to a global instance of `CoreEngine`
    // this method also creates the global instance on its first call
    static CoreEngine& get();

    CoreEngine();
    ~CoreEngine();

    void update();

    const Chrono& get_chrono() const;
    const InputManager& get_input_manager() const;
    ResourceManager& get_resource_manager();
    ECS& get_ecs();

    Scene& get_current_scene();
    Scene& create_scene();
    void set_active_scene(u32 scene_id);

private:
    Chrono m_chrono;
    InputManager m_input_manager;
    ResourceManager m_resource_manager;
    std::vector<Scene> m_loaded_scenes;
    ECS m_ecs;

    u32 m_current_active_scene;
    u32 m_last_scene_id;
};

}
