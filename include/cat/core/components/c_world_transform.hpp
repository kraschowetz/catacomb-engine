#pragma once

#include "cat/core/component_registry.hpp"
#include <cat/util/math.hpp>

namespace cat
{

struct cWorldTransform
{
    glm::mat4 matrix {1};
    bool dirty = true;

    glm::vec3 get_position() const;
};

CAT_REGISTER_COMPONENT_TYPE(cWorldTransform);

}
