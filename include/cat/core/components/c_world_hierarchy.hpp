#pragma once

#include <cat/core/ecs.hpp>

namespace cat
{

struct cWorldHierarchy
{
    EntityID parent = NULL_ENTITY;
    EntityID first_child = NULL_ENTITY;
    // relative tho "this"
    EntityID next_sibling = NULL_ENTITY;
    EntityID prev_sibling = NULL_ENTITY;
};

}
