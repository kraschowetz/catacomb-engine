#include <cat/physics/2d/collision_checker.hpp>

#include "cat/core/components/c_world_transform.hpp"
#include "cat/physics/2d/components/c_collider_aabb.hpp"

namespace cat::physics2D
{

bool CollisionChecker::check_aabb_aabb(
    const void* ptr_a, const void* ptr_b, 
    const cWorldTransform& mat_a, const cWorldTransform& mat_b
)
{
    const cColliderAABB& aabb_a = *(static_cast<const cColliderAABB*>(ptr_a));
    const cColliderAABB& aabb_b = *(static_cast<const cColliderAABB*>(ptr_b));

    if (!((aabb_a.mask & aabb_b.layer) && (aabb_b.mask & aabb_a.layer)))
        return false;

    const glm::vec2 a_half_extents = 
        aabb_a.half_extents * glm::vec2{mat_a.get_scale()};
    const glm::vec2 b_half_extents = 
        aabb_b.half_extents * glm::vec2{mat_b.get_scale()};

    const glm::vec2 a_position = mat_a.get_position();
    const glm::vec2 b_position = mat_b.get_position();

    return (
        a_position.x - a_half_extents.x <= b_position.x + b_half_extents.x &&
        a_position.x + a_half_extents.x >= b_position.x - b_half_extents.x &&
        a_position.y - a_half_extents.y <= b_position.y + b_half_extents.y &&
        a_position.y + a_half_extents.y >= b_position.y - b_half_extents.y 
    );
}

}
