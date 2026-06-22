#include <cat/core/components/c_world_transform.hpp>

using namespace cat;

glm::vec3 cWorldTransform::get_position() const
{
    return glm::vec3 {
        matrix[3][0],
        matrix[3][1],
        matrix[3][2]
    };
}
