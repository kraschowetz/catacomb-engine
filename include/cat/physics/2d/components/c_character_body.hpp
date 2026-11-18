#pragma once

#include "c_collider_aabb.hpp"
#include "../collision_type.hpp"

namespace cat::physics2D
{

struct cCharacterBody
{
public:
    glm::vec2 get_linear_velocity() const;

private:
    Watcher<cColliderAABB> m_aabb;
    glm::vec2 m_linear_velocity;

    eShapeType m_collision_shape_type;
};

}
