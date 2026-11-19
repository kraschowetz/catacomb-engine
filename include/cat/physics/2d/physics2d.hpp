#pragma once

#include "cat/core/ecs.hpp"
#include "spatial_grid.hpp"
#include "collision_type.hpp"
#include <cat/util/util.hpp>
#include "cat/core/components/c_world_transform.hpp"

namespace cat::physics2D
{

using CollisionTestFn = 
    bool(*)(const void*, const void*, const cWorldTransform&, const cWorldTransform&);
constexpr u64 SHAPE_COUNT = enum_val(eShapeType::COUNT);

inline CollisionTestFn COLLISION_TEST_DISPATCH_TABLE[SHAPE_COUNT][SHAPE_COUNT] = {};

class Physics2D
{
public:
    static Physics2D& get();

    void update_entity(EntityID entity);

private:
    template<typename A, typename B>
    void register_test(CollisionTestFn fn)
    {
        COLLISION_TEST_DISPATCH_TABLE[shape_index<A>()][shape_index<B>()] = fn;
    }

    void init_collision_test_dispatch_table();

    Physics2D();

private:
    SpatialGrid m_spatial_grid;
};

}
