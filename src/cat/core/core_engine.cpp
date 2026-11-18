#include "cat/core/component_registry.hpp"
#include "cat/core/components/c_scene_tag.hpp"
#include "cat/core/ecs.hpp"
#include "cat/core/json_scene_loader.hpp"
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
    m_current_active_scene = nullptr;
}

CoreEngine::~CoreEngine()
{
    m_loaded_scenes.clear();
}

void CoreEngine::update()
{
    m_chrono.update();
    m_input_manager.update();

    if(m_current_active_scene)
        m_current_active_scene->update();
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
    ASSERT(m_current_active_scene != nullptr, "no scene set as active");
    
    return *m_current_active_scene;
}

Scene& CoreEngine::create_scene()
{
    m_loaded_scenes.emplace_back(m_last_scene_id++);
    return m_loaded_scenes.back();
}

Scene& CoreEngine::load_scene(const std::string& path)
{
    SceneJson scene_json = read_scene_json(path);
    Scene& scene = create_scene();

    auto& registry = get_component_registry();

    for(EntityJson& entity_json : scene_json.entities)
    {
        EntityID entity = scene.create_entity();

        for(auto& [component_name, object] : entity_json)
        {
            auto it = registry.find(component_name);

            if(it == registry.end())
            {
                LOG_ERR("unknown component %s\n", component_name.c_str());
                continue;
            }

            it->second(m_ecs, entity, object);
        }
    }

    return scene;
}

void CoreEngine::unload_scene(u32 scene_id)
{
    auto view = m_ecs.view<cSceneTag>();
    std::list<EntityID> entities_to_delete;

    view.foreach(
        [scene_id, &entities_to_delete](EntityID entity, cSceneTag& scene_tag)
    {
        if(scene_tag.tag == scene_id)
            entities_to_delete.emplace_back(entity);
    });

    for(EntityID id : entities_to_delete)
    {
        m_ecs.delete_entity(id);
    }
}

void CoreEngine::set_active_scene(u32 scene_id)
{
    for(Scene& scene : m_loaded_scenes)
    {
        if(scene.get_scene_id() == scene_id)
        {
            m_current_active_scene = &scene;
            return;
        }
    }

    throw Exception{cat::eErrorCode::FAILED, "scene id not found"};
}
