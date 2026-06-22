#include <cat/core/components/c_transform.hpp>

#include "cat/core/components/c_world_transform.hpp"
#include "cat/core/core_engine.hpp"
#include "cat/util/math.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/trigonometric.hpp"

namespace cat
{

glm::mat4 cTransform::as_mat4() const
{
    glm::mat4 mat{1};
    mat = glm::translate(mat, this->position);
    mat = glm::scale(mat, this->scale);

    glm::vec3 euler = glm::eulerAngles(this->rotation);
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

cTransform cTransform::inverse() const
{
    cTransform inv;

    inv.rotation = glm::inverse(this->rotation);

    inv.scale.x = fabs(this->scale.x) < VEC3_EPSILON ? 0.f : 1.f / this->scale.x;
    inv.scale.y = fabs(this->scale.y) < VEC3_EPSILON ? 0.f : 1.f / this->scale.y;
    inv.scale.z = fabs(this->scale.z) < VEC3_EPSILON ? 0.f : 1.f / this->scale.z;

    glm::vec3 inv_translation = this->position * -1.f;
    inv.position = inv.rotation * (inv.scale * inv_translation);

    return inv;
}

static ECS& _get_ecs()
{
    return CoreEngine::get().get_ecs();
}

void translate_transform(EntityID entity, const glm::vec3& delta)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->position += delta;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void scale_transform(EntityID entity, const glm::vec3& delta)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->scale += delta;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void rotate_transform(EntityID entity, const glm::quat& delta)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->rotation += delta;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void translate_transform(EntityID entity, const glm::vec2& delta)
{
    ECS& ecs = _get_ecs();

    glm::vec3 _delta = {delta, 0.0};
    
    ecs.get_component<cTransform>(entity)->position += _delta;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void scale_transform(EntityID entity, const glm::vec2& delta)
{
    ECS& ecs = _get_ecs();

    glm::vec3 _delta = {delta, 0.0};
    
    ecs.get_component<cTransform>(entity)->scale += _delta;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void rotate_transform(EntityID entity, f32 delta)
{
    ECS& ecs = _get_ecs();

    glm::quat _delta = glm::quat{glm::vec3{0.f, 0.f, glm::radians(delta)}};
    
    ecs.get_component<cTransform>(entity)->rotation += _delta;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void set_transform_position(EntityID entity, const glm::vec3& val)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->position = val;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void set_transform_scale(EntityID entity, const glm::vec3& val)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->scale = val;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void set_transform_rotation(EntityID entity, const glm::quat& val)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->rotation = val;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void set_transform_position(EntityID entity, const glm::vec2& val)
{
    ECS& ecs = _get_ecs();
    
    ecs.get_component<cTransform>(entity)->position = glm::vec3{val, 0.f};
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void set_transform_scale(EntityID entity, const glm::vec2& val)
{
    ECS& ecs = _get_ecs();

    ecs.get_component<cTransform>(entity)->scale = glm::vec3{val, 0.f};
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

void set_transform_rotation(EntityID entity, f32 val)
{
    ECS& ecs = _get_ecs();

    glm::quat _val = glm::quat{glm::vec3{0.f, 0.f, glm::radians(val)}};
    
    ecs.get_component<cTransform>(entity)->rotation = _val;
    ecs.get_component<cWorldTransform>(entity)->dirty = true;
}

}
