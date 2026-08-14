#pragma once

#include "cat/error.hpp"
#include "component_io.hpp"
#include "ecs.hpp"
#include "glaze/json/generic_fwd.hpp"

namespace cat
{

using ComponentFactoryFN = void(*)(ECS&, EntityID, const glz::generic&);

inline std::unordered_map<std::string, ComponentFactoryFN>& get_component_registry()
{
    static std::unordered_map<std::string, ComponentFactoryFN> registry;
    return registry;
}

template <typename T>
void deserialize_component(ECS& ecs, EntityID entity, const glz::generic& object) THROWS
{
    static_assert(LoadableComponent<T>);

    T component{};

    if constexpr(HasFromJson<T>)
    {
        component = T::from_json(object);
    }
    else
    {
        auto error_code = glz::read_json(component, object);

        if(error_code)
        {
            throw Exception{"failed to serialize json"};
        }
    }

    ecs.add_component<T>(entity, std::move(component));
}

// helper struct to register a component once, even if its header is
// included in many files
template <typename T>
struct ComponentRegisterHelper
{
    explicit ComponentRegisterHelper(std::string_view name)
    {
        get_component_registry().emplace(std::string(name), &deserialize_component<T>);
    }
};

}

#define CAT_REGISTER_COMPONENT_TYPE(type)   \
    inline cat::ComponentRegisterHelper<type> _cat_register_type##type{#type}
