#pragma once

#include <cat/config.hpp>
#include <cat/core/glz_meta.hpp>
#include <cat/core/component_registry.hpp>

namespace cat::physics2D
{

struct cColliderAABB
{
    glm::vec2 half_extents;
    bitmask_32_t layer = 1;
    bitmask_32_t mask = 0xFFFFFFFF;
};

CAT_REGISTER_COMPONENT_TYPE(cColliderAABB);

}
