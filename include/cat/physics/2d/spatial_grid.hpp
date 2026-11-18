#pragma once

#include <cat/core/ecs.hpp>
#include <cat/physics/2d/components/c_collider_aabb.hpp>
#include <unordered_map>

namespace cat::physics2D
{

struct SpatialGrid
{
public:
    void insert(EntityID entity, const cColliderAABB& box);

private:
    hash_t hash_cell(i32 x, i32 y) const;

private:
    std::unordered_map<hash_t, std::vector<EntityID>> m_cells;

    static constexpr f32 CELL_SIZE = 64.f;
};

}
