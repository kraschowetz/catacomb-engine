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
