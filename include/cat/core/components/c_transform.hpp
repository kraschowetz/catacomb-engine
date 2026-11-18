#pragma once

#include <cat/config.hpp>
#include <cat/util/math.hpp>    // IWYU pragma: export
#include <cat/core/ecs.hpp>
#include <cat/util/memory.hpp>

#include <cat/core/component_io.hpp>
#include <cat/core/glz_meta.hpp>

#include <cat/core/component_registry.hpp>

namespace cat
{

struct cTransform
{
	glm::vec3 position{0.f}; // pixels in 2D, meters in 3D
	glm::vec3 scale{1.f};
	glm::quat rotation{glm::vec3{0.f, 0.f, 0.f}};

    // using this instead of a ctor to keep the c-style ctor
    static cTransform from_mat4(const glm::mat4& mat);

    cTransform inverse() const;
    glm::mat4 as_mat4() const;

};

CAT_REGISTER_COMPONENT_TYPE(cTransform);

static_assert(GlazeReflectable<cTransform>);

// use these methods when dealing with transforms with parenting
// they will handle all backend related with the cWorldTransform components

// brief: translate a 3D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: translation delta
void translate_transform(EntityID entity, const glm::vec3& delta);

// brief: scale a 3D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: scale delta
void scale_transform(EntityID entity, const glm::vec3& delta);

// brief: rotation a 3D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: rotation delta
void rotate_transform(EntityID entity, const glm::quat& delta);

// brief: translate a 2D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: translation delta
void translate_transform(EntityID entity, const glm::vec2& delta);

// brief: scale a 2D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: scale delta
void scale_transform(EntityID entity, const glm::vec2& delta);

// brief: rotate a 2D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: rotation delta in degrees
void rotate_transform(EntityID entity, f32 delta);

// brief: set the translation of a 3D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: new translation
void set_transform_position(EntityID entity, const glm::vec3& val);

// brief: set the scale of a 3D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: new scale
void set_transform_scale(EntityID entity, const glm::vec3& val);

// brief: set the rotation of a 3D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: new rotation
void set_transform_rotation(EntityID entity, const glm::quat& val);

// brief: set the translation of a 2D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: new translation
void set_transform_position(EntityID entity, const glm::vec2& val);

// brief: set the scale of a 2D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: new scale
void set_transform_scale(EntityID entity, const glm::vec2& val);

// brief: set the rotation of a 2D transform and resolve parenting operations
// param1: entity that contains the transform
// param2: new rotation, in degrees
void set_transform_rotation(EntityID entity, f32 val);

}
