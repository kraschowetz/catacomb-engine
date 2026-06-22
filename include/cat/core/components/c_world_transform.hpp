#pragma once

#include <cat/util/math.hpp>

namespace cat
{

struct cWorldTransform
{
    glm::mat4 matrix {1};
    bool dirty = true;
};

}
