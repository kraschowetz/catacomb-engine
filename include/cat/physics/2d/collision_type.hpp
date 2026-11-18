#pragma once

#include <cat/config.hpp>
#include <cat/physics/2d/components/c_collider_aabb.hpp>
#include <cat/util/util.hpp>

namespace cat::physics2D
{

enum class eShapeType : u8
{
    AABB = 0,
    CIRCLE,
    COUNT
};

template<typename T>
struct ShapeTraits;

template<>
struct ShapeTraits<cColliderAABB>
{
    static constexpr eShapeType type = eShapeType::AABB;
};

template<typename T>
inline constexpr u64 shape_index()
{
    return enum_val(ShapeTraits<T>::type);
}

}
