#pragma once

#include "cat/core/components/c_transform.hpp"
#include "cat/core/ecs.hpp"
#include <cat/core/components/c_world_transform.hpp>
#include <cat/core/components/c_world_hierarchy.hpp>
#include <cat/util/util.hpp>
#include <vector>

namespace cat
{

class Scene
{
public:
    Scene() = default;
    Scene(u32 id) : m_id(id) {}
    ~Scene();

    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;

    NO_COPY(Scene);

    void set_parent_transform(EntityID child, EntityID parent);
    EntityID create_entity(EntityID parent = NULL_ENTITY);
    void update();
    u32 get_scene_id() const;

private:
    void update_transform_system();
    void update_transform_subtree(
        EntityID e, const glm::mat4& parent_world, bool dirty);
    void make_dirty(EntityID entity);

private:
    std::vector<EntityID> m_root_entities;
    u32 m_id;
};

}

namespace std
{

template<> 
struct hash<cat::Scene>
{
    hash_t operator()(const cat::Scene& s)
    {
        return s.get_scene_id();
    }
};

}
