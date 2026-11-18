#include "cat/util/math.hpp"
#include <cat/core/components/c_world_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

using namespace cat;

glm::vec3 cWorldTransform::get_position() const
{
    return glm::vec3 {
        matrix[3][0],
        matrix[3][1],
        matrix[3][2]
    };
}

glm::vec3 cWorldTransform::get_scale() const
{
    glm::vec3 up = glm::normalize(
        glm::vec3
        {
            matrix[1][0],
            matrix[1][1],
            matrix[1][2]
        }
    );
    glm::vec3 forward = glm::normalize(
        glm::vec3
        {
            matrix[2][0],
            matrix[2][1],
            matrix[2][2]
        }
    );

    glm::quat rotation = look_towards(forward, up);

    glm::mat3 rotation_scale_matrix(
        matrix[0][0], matrix[0][1], matrix[0][3],
        matrix[1][0], matrix[1][1], matrix[1][3],
        matrix[2][0], matrix[2][1], matrix[2][3]
    );

    glm::mat3 inverse_rotation = glm::mat3(glm::mat4_cast(glm::inverse(rotation)));

    glm::mat3 scale_skew = inverse_rotation * rotation_scale_matrix;

    return glm::vec3{
        scale_skew[0][0],
        scale_skew[1][1],
        scale_skew[2][2]
    };
}
