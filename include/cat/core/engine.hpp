#pragma once

#include <cat/config.hpp>
#include <cat/core/memory.hpp>
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

private:
    Chrono m_chrono;
    InputManager m_input_manager;
    ResourceManager m_resource_manager;
    ECS m_ecs;
};

}
