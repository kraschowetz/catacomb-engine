#pragma once

#include <cat/config.hpp>
#include <cat/core/glz_meta.hpp>
#include <cat/core/component_registry.hpp>

namespace cat::physics2D
{

struct cColliderAABB
{
    glm::vec2 half_extents;
    u32 layer = 1;
    u32 mask = 0xFFFFFFFF;
};

CAT_REGISTER_COMPONENT_TYPE(cColliderAABB);

}
