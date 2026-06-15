#pragma once

#include <cat/config.hpp>
#include <cat/util/math.hpp>    // IWYU pragma: export
#include <cat/util/memory.hpp>

namespace cat
{

struct cTransform
{
	glm::vec3 position;	// position in meters
	glm::vec3 scale;
	glm::quat rotation;

    static glm::mat4 as_mat4(const cTransform& self);
    // using this instead of a ctor to keep the c-style ctor
    static cTransform from_mat4(const glm::mat4& mat);

    static cTransform inverse(const cTransform& self);
};

}
