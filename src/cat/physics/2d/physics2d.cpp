#include "cat/core/components/c_world_transform.hpp"
#include "cat/core/core_engine.hpp"
#include "cat/core/ecs.hpp"
#include "cat/physics/2d/components/c_collider_aabb.hpp"
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

void Physics2D::update_entity(EntityID entity)
{
    Watcher<cColliderAABB> aabb = CoreEngine::get().get_ecs()
        .get_component<cColliderAABB>(entity);

    ASSERT(aabb != nullptr, "entity {} does not have a collider AABB!", entity);

    m_spatial_grid.update(entity, *aabb);
}

bool Physics2D::is_colliding(EntityID entity) const
{
    ECS& ecs = CoreEngine::get().get_ecs();

    ASSERT(ecs.has_component<cWorldTransform>(entity));
    ASSERT(ecs.has_component<cWorldTransform>(entity));

    Watcher<cColliderAABB> aabb = ecs.get_component<cColliderAABB>(entity);
    Watcher<cWorldTransform> transform = ecs.get_component<cWorldTransform>(entity);

    bool collided = false;

    auto view = ecs.view<cColliderAABB, cWorldTransform>();

    view.foreach(
        [entity, &collided, aabb, transform](EntityID id, 
            cColliderAABB& other_aabb,
            cWorldTransform& other_transform)
        {
            if(id == entity)
                return;
            
            if(!collided)
                collided = CollisionChecker::check_aabb_aabb(
                    aabb.get(),
                    &other_aabb,
                    *transform,
                    other_transform
                );
        }
    );

    return collided;
}
