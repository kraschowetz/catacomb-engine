#include <cat/physics/2d/spatial_grid.hpp>

#include "cat/core/components/c_world_transform.hpp"
#include "cat/core/core_engine.hpp"

using namespace cat::physics2D;

void SpatialGrid::insert(EntityID entity, const cColliderAABB& box)
{
    Watcher<cWorldTransform> transform = CoreEngine::get().get_ecs()
        .get_component<cWorldTransform>(entity);

    ASSERT(
            transform != nullptr, 
            R"(attempted to insert an entity without a WorldTransform 
            component in a SpatialGrid2D)"
    );

    glm::vec2 position = transform->get_position();

    i32 min_x = static_cast<i32>(
        std::floor(position.x - box.half_extents.x) / CELL_SIZE
    );
    i32 min_y = static_cast<i32>(
        std::floor(position.y - box.half_extents.y) / CELL_SIZE
    );
    i32 max_x = static_cast<i32>(
        std::floor(position.x + box.half_extents.x) / CELL_SIZE
    );
    i32 max_y = static_cast<i32>(
        std::floor(position.y + box.half_extents.y) / CELL_SIZE
    );

    for(i32 x = min_x; x < max_x; ++x)
    {
        for(i32 y = min_y; y < max_y; ++y)
        {
            m_cells[hash_cell(x, y)].push_back(entity);
        }
    }
}

hash_t SpatialGrid::hash_cell(i32 x, i32 y) const
{
    return (hash_t(u32(x)) << 32 | u32(y));
}
