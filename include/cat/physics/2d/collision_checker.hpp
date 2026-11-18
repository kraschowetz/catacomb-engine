#pragma once

#include <cat/util/math.hpp>
#include "cat/core/components/c_world_transform.hpp"

#define COLLISION_CHECK_ARGS    \
    const void*, const void*, const cWorldTransform&, const cWorldTransform&

namespace cat::physics2D
{

struct CollisionChecker
{
    static bool check_aabb_aabb(COLLISION_CHECK_ARGS);
};

}
