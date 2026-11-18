#include <cat/physics/2d/physics2d.hpp>

#include <cat/physics/2d/collision_checker.hpp>

using namespace cat::physics2D;

Physics2D& Physics2D::get()
{
    static Physics2D instance;
    return instance;
}

Physics2D::Physics2D()
{
    init_collision_test_dispatch_table();
}

void Physics2D::init_collision_test_dispatch_table()
{
    register_test<cColliderAABB, cColliderAABB>(&CollisionChecker::check_aabb_aabb);
}
