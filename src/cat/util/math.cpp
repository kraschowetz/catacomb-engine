#include <cat/util/math.hpp>

namespace cat {

glm::quat look_towards(const glm::vec3& direction, const glm::vec3& up)
{
    glm::vec3 _forward = glm::normalize(direction);
    glm::vec3 _up = glm::normalize(up);

    // orthogonal basis
    glm::vec3 _right = glm::cross(_up, _forward);
    _up = glm::cross(_forward, _right);

    glm::mat3 mat = {
        _right.x, _right.y, _right.z,
        _up.x, _up.y, _up.z,
        _forward.x, _forward.y, _forward.z,
    };

    return glm::quat_cast(mat);
}

}
