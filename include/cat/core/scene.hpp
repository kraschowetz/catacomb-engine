#pragma once

#include "cat/core/components/c_transform.hpp"
#include "cat/core/ecs.hpp"
#include <cat/core/components/c_world_transform.hpp>
#include <cat/core/components/c_world_hierarchy.hpp>

namespace cat
{

class Scene
{
public:
    void set_parent_transform(EntityID child, EntityID parent);
    EntityID create_entity(EntityID parent = NULL_ENTITY);
    void update();

private:
    void update_transform_system();
    void update_transform_subtree(
        EntityID e, const glm::mat4& parent_world, bool dirty);
    void make_dirty(EntityID entity);

private:
    u32 m_id;
};

}
