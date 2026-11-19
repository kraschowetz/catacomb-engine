#pragma once

#include <cat/core/ecs.hpp>
#include <cat/physics/2d/components/c_collider_aabb.hpp>
#include <unordered_map>
#include <set>

namespace cat::physics2D
{

struct SpatialGrid
{
public:
    void insert(EntityID entity, const cColliderAABB& box);

    void update(EntityID entity, const cColliderAABB& box);

private:
    hash_t hash_cell(i32 x, i32 y) const;

private:
    std::unordered_map<hash_t, std::set<EntityID>> m_cells;
    std::unordered_map<EntityID, std::set<hash_t>> m_entity_to_cell;

    static constexpr f32 CELL_SIZE = 64.f;
};

}
