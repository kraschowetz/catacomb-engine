#pragma once

#include "cat/config.hpp"
#include "cat/util/memory.hpp"
#include "cat/util/logger.hpp"
#include <algorithm>
#include <bitset>
#include <functional>
#include <limits>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include <cat/util/sparse_set.hpp>

namespace cat
{
using EntityID = u64;

static constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();

// @brief: how many entities may be 'alive' simultaneously
static constexpr EntityID MAX_ENTITIES = NULL_ENTITY;

// @brief: how many components an entity can have.
// should be a multiple of '32' due to bitset overallocations
static constexpr u64 MAX_COMPONENTS = 64;

template <class... Types>
struct TypeList
{
	using TypeTuple = std::tuple<Types...>;

	template <u64 index>
	using get = std::tuple_element_t<index, TypeTuple>;

	static constexpr u64 size = sizeof...(Types);
};

template<typename... Components>
class SimpleView;

class ECS
{
public:
	template<typename Component>
	u64 register_component_index()
	{
		m_component_ids.push_back(typeid(Component));
		m_component_pools.push_back(
			std::make_unique<SparseSet<Component>>()
		);
		return m_component_ids.size() - 1;
	}

	template<typename Component>
	u64 get_component_index()
	{
		std::type_index tid = typeid(Component);

		for(u64 i = 0; i < m_component_ids.size(); ++i)
		{
			if(m_component_ids[i] == tid)
			{
				return i;
			}
		}

		return NULLID;
	}

	template<typename Component>
	u64 get_or_register_component_index()
	{
		std::type_index tid = typeid(Component);
		
		for(u64 i = 0; i < m_component_ids.size(); ++i)
		{
			if(m_component_ids[i] == tid)
			{
				return i;
			}
		}

		// no component found; need to register
		m_component_ids.push_back(typeid(Component));
		m_component_pools.push_back(
			std::make_unique<SparseSet<Component>>()
		);
		return m_component_ids.size() - 1;
	}

	template<typename Component>
	Watcher<iSparseSet> get_component_pool_ptr()
	{
		u64 index = get_or_register_component_index<Component>();
		return m_component_pools[index];
	}

	template<typename Component>
	Watcher<Component> add_component(EntityID entity, const Component& comp = {})
	{
		u64 index = get_or_register_component_index<Component>();
		
		SparseSet<Component>* sparse_set = reinterpret_cast<SparseSet<Component>*>(
			m_component_pools[index].get()
		);

		Watcher<Component> added = sparse_set->set(entity, comp);

		// set component bit to 1
		m_entity_masks.get(entity).get()[index] = 0;

		return added;
	}

	template<typename Component>
	void remove_component(EntityID entity)
	{
		u64 index = get_component_index<Component>();

		// Component's pool doesen't even exists
		if(index == NULLID) return;

		m_component_pools[index]->erase(entity);

		// set component bit to 0
		ComponentMask& mask = *m_entity_masks.get(entity);
		mask[index] = 0;
	}

	EntityID create_entity()
	{
		EntityID id = NULL_ENTITY;

		if(m_available_entities.empty())
		{
			CAT_ASSERT(m_last_entity_id < MAX_ENTITIES);
			id = m_last_entity_id++;
		}
		else 
		{
			id = m_available_entities.back();
			m_available_entities.pop_back();
		}

		// create an entity mask
		m_entity_masks.set(id, ComponentMask{});

		return id;
	}

	void delete_entity(EntityID& entity)
	{
		ComponentMask& mask = *m_entity_masks.get(entity);

		for(u32 i = 0; i < MAX_COMPONENTS; ++i)
		{
			if(mask[i] == 0) continue;

			m_component_pools[i]->erase(entity);
		}

		m_entity_masks.erase(entity);
		m_available_entities.push_back(entity);

		entity = NULL_ENTITY;
	}

	template<typename Component>
	bool has_component(EntityID id)
	{
		ComponentMask& mask = *m_entity_masks.get(id);

		return mask[get_component_index<Component>()];
	}

	template<typename... Components>
	SimpleView<Components...> view()
	{
		return {this};
	}

private:
	template<typename...>
	friend class SimpleView;

	using ComponentMask = std::bitset<MAX_COMPONENTS>;

	static constexpr u64 NULLID = std::numeric_limits<u64>::max();

	// stores entities created, but not used
	std::vector<EntityID> m_available_entities;

	// component masks for each entity
	SparseSet<ComponentMask> m_entity_masks;
	
	// stores each component sparsesets
	std::vector<Unique<iSparseSet>> m_component_pools;

	// highest EntityID
	EntityID m_last_entity_id = 0;
	
	// each 'type_index' id inside the vector correspond
	// to it's bit in a 'ComponentMask'
	std::vector<std::type_index> m_component_ids;
private:
	
};

template<typename... Components>
class SimpleView
{
public:
	using ForeachFunc = std::function<void(Components&...)>;
	using ForeachFuncWithID = std::function<void(EntityID, Components&...)>;

	SimpleView(Watcher<ECS> ecs)
		: m_ecs{ecs}, m_view_pools{ecs->get_component_pool_ptr<Components>()...}
	{
		CAT_ASSERT(ComponentTypes::size == m_view_pools.size());

		auto smallest_pool = std::min_element(
			m_view_pools.begin(),
			m_view_pools.end(),
			[](Watcher<iSparseSet> a, Watcher<iSparseSet> b)
			{
				return a->size() < b->size();
			}
		);

		CAT_ASSERT(smallest_pool != m_view_pools.end());

		m_smallest_sparse_set = *smallest_pool;
	}

	template<typename... ExcludedComponents>
	SimpleView& without()
	{
		m_excluded_pools = {m_ecs->get_component_pool_ptr<ExcludedComponents>()...};
		return *this;
	}

	void foreach(ForeachFunc func)
	{
		foreach_impl(func);
	}

	void foreach(ForeachFuncWithID func)
	{
		foreach_impl(func);
	}

private:
	using ComponentTypes = TypeList<Components...>;

	Watcher<ECS> m_ecs;

	std::array<Watcher<iSparseSet>, sizeof...(Components)> m_view_pools;
	std::vector<Watcher<iSparseSet>> m_excluded_pools;

	Watcher<iSparseSet> m_smallest_sparse_set;
	
	// @brief: check if all viewed pools contain an entity
	bool all_contain(EntityID id)
	{
		return std::all_of(
			m_view_pools.begin(),
			m_view_pools.end(),
			[id](Watcher<iSparseSet> pool)
			{
				return pool->contains_entity(id);
			}
		);
	}

	// @brief: check if and entity only have components in viewed pools
	bool not_excluded(EntityID id)
	{
		return std::none_of(
			m_excluded_pools.begin(),
			m_excluded_pools.end(),
			[id](Watcher<iSparseSet> pool)
			{
				return pool->contains_entity(id);
			}
		);
	}

	// @brief: get component-specific pools via compile-time indices
	template<u64 Index>
	auto get_pool_at()
	{
		using ComponentType = typename ComponentTypes::template get<Index>;

		return Watcher<SparseSet<ComponentType>>(
			reinterpret_cast<SparseSet<ComponentType>*>(m_view_pools[Index].get())
		);
	}

	template<u64... Indices>
	auto make_component_tuple(EntityID id, std::index_sequence<Indices...>)
	{
		return std::make_tuple((std::ref(*get_pool_at<Indices>()->get(id)))...);
	}

	// private implementation of foreach loops
	template <typename Func>
	void foreach_impl(Func func)
	{
		constexpr auto indices = std::make_index_sequence<sizeof...(Components)>{};
		
		// iterate over smallest
		// using a copy of smalest.entity_list for safe deletions (or insertion, who knows...)
		for(EntityID id : m_smallest_sparse_set->get_entity_list())
		{
			if(all_contain(id) && not_excluded(id))
			{
				// for methods like my_method(EntityID, ComponentA, ComponentB)
				if constexpr(std::is_invocable_v<Func, EntityID, Components&...>)
				{
					std::apply(
						func, 
						std::tuple_cat(
							std::make_tuple(id), 
							make_component_tuple(id, indices)
						)
					);
				}
				else if constexpr(std::is_invocable_v<Func, Components&...>)
				{
					std::apply(func, make_component_tuple(id, indices));
				}
				else {
					LOG_ERR("bad lambda args!\n");
					CAT_ASSERT(false);     
			        }
			}
		}
	}
};

}
