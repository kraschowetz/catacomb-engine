#pragma once

#include <glm/glm.hpp>              // IWYU pragma: export
#include <glm/fwd.hpp>              // IWYU pragma: export
#include <glm/gtc/quaternion.hpp>   // IWYU pragma: export
#include <glm/geometric.hpp>        // IWYU pragma: export
#include <math.h>                   // IWYU pragma: export

#ifndef VEC3_EPSILON
    #define VEC3_EPSILON 0.000001f
#endif

namespace cat
{

extern glm::quat look_towards(const glm::vec3& direction, const glm::vec3& up);

}
