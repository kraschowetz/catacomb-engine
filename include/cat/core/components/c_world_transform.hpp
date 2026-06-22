#pragma once

#include <cat/util/math.hpp>

namespace cat
{

struct cWorldTransform
{
    glm::mat4 matrix {1};
    bool dirty = true;

    glm::vec3 get_position() const;
};

}
