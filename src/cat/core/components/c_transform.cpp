#include <cat/core/components/c_transform.hpp>

#include "cat/util/math.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

using namespace cat;

glm::mat4 cTransform::as_mat4(const cTransform& self)
{
    glm::mat4 mat{1};
    mat = glm::translate(mat, self.position);
    mat = glm::scale(mat, self.scale);

    glm::vec3 euler = glm::eulerAngles(self.rotation);
    mat = glm::rotate(mat, euler.x, glm::vec3{1, 0, 0});
    mat = glm::rotate(mat, euler.y, glm::vec3{0, 1, 0});
    mat = glm::rotate(mat, euler.z, glm::vec3{0, 0, 1});

    return mat;
}

cTransform cTransform::from_mat4(const glm::mat4 &mat)
{
    cTransform trans;

    trans.position = glm::vec3 {
        mat[3][0],
        mat[3][1],
        mat[3][2]
    };

    glm::vec3 up = glm::normalize( glm::vec3{
        mat[1][0], 
        mat[1][1], 
        mat[1][2]
    });
    glm::vec3 forward = glm::normalize(glm::vec3{
        mat[2][0], 
        mat[2][1], 
        mat[2][2]
    });

    trans.rotation = look_towards(forward, up);

    glm::mat3 rotation_scale_mat(
       mat[0][0], mat[0][1], mat[0][2],
       mat[1][0], mat[1][1], mat[1][2],
       mat[2][0], mat[2][1], mat[2][2]
    );

    glm::mat3 inverse_rotation = glm::mat3(glm::mat4_cast(glm::inverse(trans.rotation)));

    glm::mat3 scale_skew = rotation_scale_mat * inverse_rotation;

    trans.scale = glm::vec3{
        scale_skew[0][0],
            scale_skew[1][1],
            scale_skew[2][2]
    };

    return trans;
}

cTransform cTransform::inverse(const cTransform &self)
{
    cTransform inv;

    inv.rotation = glm::inverse(self.rotation);

    inv.scale.x = fabs(self.scale.x) < VEC3_EPSILON ? 0.f : 1.f / self.scale.x;
    inv.scale.y = fabs(self.scale.y) < VEC3_EPSILON ? 0.f : 1.f / self.scale.y;
    inv.scale.z = fabs(self.scale.z) < VEC3_EPSILON ? 0.f : 1.f / self.scale.z;

    glm::vec3 inv_translation = self.position * -1.f;
    inv.position = inv.rotation * (inv.scale * inv_translation);

    return inv;
}
