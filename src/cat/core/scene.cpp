#include "cat/core/components/c_world_transform.hpp"
#include "cat/util/memory.hpp"
#include "glm/ext/vector_float3.hpp"
#include <cat/core/scene.hpp>

#include <cat/core/ecs.hpp>
#include <cat/core/core_engine.hpp>
#include <cat/core/components/c_world_hierarchy.hpp>
#include <cat/core/components/c_transform.hpp>
#include <cat/core/components/c_scene_tag.hpp>

using namespace cat;

static ECS& _get_ecs()
{
    return CoreEngine::get().get_ecs();
}

void Scene::set_parent_transform(EntityID child, EntityID parent)
{
    ECS& ecs = _get_ecs();

    Watcher<cWorldHierarchy> child_hierarchy = 
        ecs.get_component<cWorldHierarchy>(child);

    if(child_hierarchy->parent != NULL_ENTITY)
    {
        Watcher<cWorldHierarchy> old_parent_hierarchy = 
            ecs.get_component<cWorldHierarchy>(child_hierarchy->parent);

        // remove child from its siblings linked list
        if(child_hierarchy->prev_sibling != NULL_ENTITY)
        {
            ecs.get_component<cWorldHierarchy>(child_hierarchy->prev_sibling)
                ->next_sibling = child_hierarchy->next_sibling;
        }
        else {
            old_parent_hierarchy->first_child = child_hierarchy->next_sibling;
        }

        if(child_hierarchy->next_sibling != NULL_ENTITY)
        {
            ecs.get_component<cWorldHierarchy>(child_hierarchy->next_sibling)
                ->prev_sibling = child_hierarchy->prev_sibling;
        }
    }

    child_hierarchy->parent = parent;
    child_hierarchy->prev_sibling = NULL_ENTITY;
    child_hierarchy->next_sibling = NULL_ENTITY;

    Watcher<cWorldHierarchy> new_parent_hierarchy = 
        ecs.get_component<cWorldHierarchy>(parent);

    child_hierarchy->next_sibling = new_parent_hierarchy->first_child;
    if(new_parent_hierarchy->first_child != NULL_ENTITY)
    {
        ecs.get_component<cWorldHierarchy>(new_parent_hierarchy->first_child)
            ->prev_sibling = child;
    }
    new_parent_hierarchy->first_child = child;
    
    make_dirty(child);
}

EntityID Scene::create_entity(EntityID parent)
{
    ECS& ecs = _get_ecs();

    EntityID entity = ecs.create_entity();
    ecs.add_component<cTransform>(entity, cTransform{
        .position = {},
        .scale = glm::vec3{1.f},
        .rotation = {}
    });
    ecs.add_component<cWorldTransform>(entity);
    ecs.add_component<cWorldHierarchy>(entity);
    ecs.add_component<cSceneTag>(entity, {m_id});

    if(parent != NULL_ENTITY)
    {
        set_parent_transform(entity, parent);
    }

    return entity;
}

void Scene::update()
{
    update_transform_system();
}

void Scene::update_transform_system()
{
    auto view = _get_ecs()
        .view<cWorldHierarchy, cTransform, cWorldTransform>();

    view.foreach(
        [this]
        (EntityID entity, const cWorldHierarchy& hierarchy,
            const cTransform&, const cWorldTransform&
        ){
        if(hierarchy.parent == NULL_ENTITY)
        {
            update_transform_subtree(entity, glm::mat4{1.f}, true);
        }
    });
}

void Scene::update_transform_subtree(
    EntityID e, const glm::mat4& parent_world, bool dirty)
{
    ECS& ecs = _get_ecs();
    Watcher<cWorldTransform> world = ecs.get_component<cWorldTransform>(e);

    bool is_dirty = dirty || world->dirty;

    if(is_dirty)
    {
        glm::mat4 local = ecs.get_component<cTransform>(e)->as_mat4();
        world->matrix = parent_world * local;
        world->dirty = false;
    }

    Watcher<cWorldHierarchy> hierarchy = ecs.get_component<cWorldHierarchy>(e);

    for(
        EntityID c = hierarchy->first_child;
        c != NULL_ENTITY;
        c = ecs.get_component<cWorldHierarchy>(c)->next_sibling
    )
    {
        update_transform_subtree(c, world->matrix, is_dirty);
    }
}


void Scene::make_dirty(EntityID entity)
{
    CoreEngine::get().get_ecs().get_component<cWorldTransform>(entity)->dirty = true;
}

void Scene::translate_transform(EntityID entity, const glm::vec3& delta)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->position += delta;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void Scene::scale_transform(EntityID entity, const glm::vec3& delta)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->scale += delta;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void Scene::rotate_transform(EntityID entity, const glm::quat& delta)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->rotation += delta;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void Scene::set_transform_position(EntityID entity, const glm::vec3& val)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->position = val;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void Scene::set_transform_scale(EntityID entity, const glm::vec3& val)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->scale = val;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void Scene::set_transform_rotation(EntityID entity, const glm::quat& val)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->rotation = val;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}
