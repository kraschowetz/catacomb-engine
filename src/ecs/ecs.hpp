#ifndef ECS_H
#define ECS_H

#include <array>
#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <memory>
#include <type_traits>
#include <functional>
#include <typeinfo>
#include "../util/util.hpp"

using EntityID = u64;

static constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();

constexpr u64 MAX_ENTITIES = NULL_ENTITY;


// should be a multiple of 32
constexpr u64 MAX_COMPONENTS = 64;

class V_SparseSet {
public:
	virtual ~V_SparseSet() = default;
	virtual void remove(EntityID) = 0;
	virtual void clear() = 0;
	virtual u64 size() = 0;
	virtual bool has(EntityID id) = 0;
	virtual std::vector<EntityID> entity_list() = 0;
};

template <class... Types>
struct TypeList {
	using TypeTuple = std::tuple<Types...>;

	template <u64 Index>
	using get = std::tuple_element_t<Index, TypeTuple>;

	static constexpr u64 size = sizeof...(Types);
};

template <typename T>
class SparseSet: public V_SparseSet {
private:

	static constexpr u64 SPARSE_MAX_SIZE = 2048;
	static constexpr u64 tombstone = std::numeric_limits<u64>::max();

	using Sparse = std::array<u64, SPARSE_MAX_SIZE>;

	std::vector<Sparse> sparse_pages;

	std::vector<T> dense;
	std::vector<EntityID> dense_to_entity;

	inline void set_dense_index(EntityID id, u64 index) {
		u64 page = id / SPARSE_MAX_SIZE;
		u64 local_index = id % SPARSE_MAX_SIZE;

		if (page >= sparse_pages.size()) {
			sparse_pages.resize(page + 1);
			sparse_pages[page].fill(tombstone);
		}

		Sparse& sparse = sparse_pages[page];

		sparse[local_index] = index;
	}

	inline u64 get_dense_index(EntityID id) {
		u64 page = id / SPARSE_MAX_SIZE;
		u64 local_index = id % SPARSE_MAX_SIZE;

		if (page < sparse_pages.size()) {
			Sparse& sparse = sparse_pages[page];
			return sparse[local_index];
		}

		return tombstone;
	}

public:

	SparseSet() {
		constexpr u64 reserved = 512;
		dense.reserve(reserved);
		dense_to_entity.reserve(reserved);
	}

	T* set(EntityID id, T obj) {
		u64 index = get_dense_index(id);
		if (index != tombstone) {
			dense[index] = obj;
			dense_to_entity[index] = id;

			return &dense[index];
		}

		set_dense_index(id, dense.size());

		dense.push_back(obj);
		dense_to_entity.push_back(id);

		return &dense.back();
	}

	T* get(EntityID id) {
		u64 index = get_dense_index(id);
		if(index != tombstone) {
			return &dense[index];
		}
		return nullptr;
	}

	T& get_ref(EntityID id) {
		u64 index = get_dense_index(id);

		ASSERT(index != tombstone, "invalid entity ID");

		return dense[index];
	}

	void remove(EntityID id) override {

		u64 deleted_index = get_dense_index(id);

		if (dense.empty() || deleted_index == tombstone) {
			return;
		}

		set_dense_index(dense_to_entity.back(), deleted_index);
		set_dense_index(id, tombstone);

		std::swap(dense.back(), dense[deleted_index]);
		std::swap(dense_to_entity.back(), dense_to_entity[deleted_index]);

		dense.pop_back();
		dense_to_entity.pop_back();
	}

	u64 size() override {
		return dense.size();
	}

	std::vector<EntityID> entity_list() override {
		return dense_to_entity;
	}

	bool has(EntityID id) override {
		return get_dense_index(id) != tombstone;
	}

	void clear() override {
		dense.clear();
		sparse_pages.clear();
		dense_to_entity.clear();
	}

	bool empty() const {
		return dense.empty();
	}

	const std::vector<T>& data() const {
		return dense;
	}
};

template<typename...>
class View;

#define ASSERT_VALID_ENTITY(id) \
ASSERT(id != NULL_ENTITY, "NULL_ENTITY cannot be operated on by the ECS") \
ASSERT(id < max_entity_id && id >= 0, "Invalid entity ID out of bounds: " << id); \
ASSERT(entity_masks.get(id) != nullptr, "Attempting to access inactive entity with ID: " << id);

class ECS {
private:

	template<typename...>
	friend class View;

	using ComponentMask = std::bitset<MAX_COMPONENTS>;


	std::vector<EntityID> available_entities;
	SparseSet<ComponentMask> entity_masks;

	// debug;
	SparseSet<std::string> entity_names;


	std::vector<std::unique_ptr<V_SparseSet>> component_pools;

	// debug;
	inline static std::vector<std::string> component_names;


	EntityID max_entity_id = 0;

private:

	static u64 next_component_index(std::string typeName) {
		static u64 id = 0;
		component_names.push_back(typeName);
		return id++;
	};

	template <typename T>
	static u64 component_index() {
			static u64 ind = next_component_index(typeid(T).name());
	    return ind;
	};

	template <typename T>
	u64 register_component_index() {
		u64 index = component_index<T>();

		if (index >= component_pools.size() || !component_pools[index])
			register_component<T>();

		ASSERT(index < component_pools.size() && index >= 0,
			"Type index out of bounds for component" << typeid(T).name() << "'");

		return index;
	}

	template <typename T>
	V_SparseSet* component_pool_ptr() {
		u64 index = register_component_index<T>();
		return component_pools[index].get();
	}

	template <typename T>
	SparseSet<T>& component_pool() {
		V_SparseSet* generic = component_pool_ptr<T>();
		SparseSet<T>* pool = static_cast<SparseSet<T>*>(generic);

		return *pool;
	}

	template <typename Component>
	void set_component_bit(ComponentMask& mask, bool val) {
		u64 pos = component_index<Component>();
		mask[pos] = val;
	}

	template <typename Component>
	ComponentMask::reference component_bit(ComponentMask& mask) {
		u64 pos = component_index<Component>();
		return mask[pos];
	}

	ComponentMask& entity_mask(EntityID id) {
		ComponentMask* mask = entity_masks.get(id);
		ASSERT(mask, "entity has no component mask");
		return *mask;
	}

	template <typename... Components>
	ComponentMask generic_mask() {
		ComponentMask mask;
		(set_component_bit<Components>(mask, 1), ...);
		return mask;
	}

public:

	ECS() = default;

	template <typename T>
	static int define() {
		static int index = 0;
		return index;
	}

	void reset() {
		available_entities.clear();
		entity_masks.clear();
		entity_names.clear();
		component_pools.clear();
		max_entity_id = 0;
	}

	EntityID create_entity(std::string name = "") {
		EntityID id = NULL_ENTITY;

		if (available_entities.size() == 0) {
			ASSERT(max_entity_id < MAX_ENTITIES, "Entity limit exceeded");
			id = max_entity_id++;
		}
		else {
			id = available_entities.back();
			available_entities.pop_back();
		}

		ASSERT(id != NULL_ENTITY, "Cannot create entity with null ID");

		entity_masks.set(id, {});

		if (!name.empty()) {
			entity_names.set(id, name);
		}

		return id;
	}

	std::string entity_name(EntityID id) {
		ASSERT_VALID_ENTITY(id);

		auto name = entity_names.get(id);
		if (name) {
			return *name;
		}

		return "";
	}

	void delete_entity(EntityID& id) {
		ASSERT_VALID_ENTITY(id);

		std::string name = entity_name(id);
		ComponentMask& mask = entity_mask(id);

		for (u64 i = 0; i < MAX_COMPONENTS; i++) {
			if (mask[i] == 1) {
				component_pools[i]->remove(id);
			}
		}

		entity_masks.remove(id);
		entity_names.remove(id);
		available_entities.push_back(id);

		id = NULL_ENTITY;
	}

	template <typename T>
	void register_component() {
		ASSERT(component_pools.size() <= MAX_COMPONENTS,
			"Exceeded max number of registered components");

		u64 id = component_index<T>();
		if (id >= component_pools.size())
			component_pools.resize(id + 1);

		ASSERT(!component_pools[id],
			"Attempting to register component '" << typeid(T).name() << "' twice");

		component_pools[id] = std::make_unique<SparseSet<T>>();
	}

	template <typename T>
	T& add(EntityID id, T&& component = {}) {
		ASSERT_VALID_ENTITY(id);

		SparseSet<T>& pool = component_pool<T>();

		if (pool.get(id))
			return *pool.set(id, std::move(component));

		ComponentMask& mask = entity_mask(id);

		set_component_bit<T>(mask, 1);

		return *pool.set(id, std::move(component));
	}

	template <typename T>
	T& get(EntityID id) {
		ASSERT_VALID_ENTITY(id);

		SparseSet<T>& pool = component_pool<T>();
		T* component = pool.get(id);

		ASSERT(component, "missing component");

		return *component;
	}

	template <typename T>
	T* get_ptr(EntityID id) {
		ASSERT_VALID_ENTITY(id);

		SparseSet<T>& pool = component_pool<T>();
		return pool.get(id);
	}

	template <typename T>
	void remove(EntityID id) {
		ASSERT_VALID_ENTITY(id);

		SparseSet<T>& pool = component_pool<T>();

		if (!pool.get(id)) return;

		ComponentMask& mask = entity_mask(id);
		set_component_bit<T>(mask, 0);

		pool.remove(id);
	}

	template <typename... Ts>
	bool has(EntityID id) {
		auto& mask = entity_mask(id);
		return (component_bit<Ts>(mask) && ...);
	}

	template <typename... Ts>
	bool has_any(EntityID id) {
		return (has<Ts>(id) || ...);
	}

	template <typename... Components>
	View<Components...> view() {
		return { this };
	}

	u64 entity_count() {
		return entity_masks.size();
	}

	u64 pool_count() {
		return component_pools.size();
	}
};

template <typename... Components>
class View {
private:

	using componentTypes = TypeList<Components...>;

	ECS* ecs;

	std::array<V_SparseSet*, sizeof...(Components)> view_pools;
	std::vector<V_SparseSet*> excluded_pools;

	V_SparseSet* smallest = nullptr;

	bool all_has(EntityID id) {
		return std::all_of(view_pools.begin(), view_pools.end(), [id](V_SparseSet* pool) {
			return pool->has(id);
		});
	}

	bool not_excluded(EntityID id) {
		if (excluded_pools.empty()) return true;
		return std::none_of(excluded_pools.begin(), excluded_pools.end(), [id](V_SparseSet* pool) {
			return pool->has(id);
		});
	}

	template <u64 Index>
	auto pool_at() {
		using componentType = typename componentTypes::template get<Index>;
		return static_cast<SparseSet<componentType>*>(view_pools[Index]);
	}

	template <u64... Indices>
	auto make_comp_tuple(EntityID id, std::index_sequence<Indices...>) {
		return std::make_tuple((std::ref(pool_at<Indices>()->get_ref(id)))...);
	}

	template <typename Func>
	void foreach_impl(Func func) {
		constexpr auto ids = std::make_index_sequence<sizeof...(Components)>{};

		for (EntityID id : smallest->entity_list()) {
			if (all_has(id) && not_excluded(id)) {

				if constexpr (std::is_invocable_v<Func, EntityID, Components&...>) {
					std::apply(func, std::tuple_cat(std::make_tuple(id), make_comp_tuple(id, ids)));
				}

				else if constexpr (std::is_invocable_v<Func, Components&...>) {
					std::apply(func, make_comp_tuple(id, ids));
				}

				else {
					THROW_ERR(
						"Bad lambda provided to .foreach(), parameter pack does not match lambda args"
					);
				}
			}
		}
	}

public:

	using ForeachFunc = std::function<void(Components&...)>;
	using ForeachFuncID = std::function<void(EntityID, Components&...)>;

	View(ECS* ecs) :
		ecs(ecs), view_pools{ ecs->component_pool_ptr<Components>()... }
	{
		ASSERT(componentTypes::size == view_pools.size(), "Component type list and pool array size mismatch");

		auto smallest_pool = std::min_element(view_pools.begin(), view_pools.end(),
			[](V_SparseSet* a, V_SparseSet* b) { return a->size() < b->size(); }
		);

		ASSERT(smallest_pool != view_pools.end(), "Initializing invalid/empty view");

		smallest = *smallest_pool;
	}

	template <typename... ExcludedComponents>
	View& without() {
		excluded_pools = { ecs->component_pool_ptr<ExcludedComponents>()... };
		return *this;
	}

	void foreach(ForeachFunc func) {
		foreachImpl(func);
	}

	void foreach(ForeachFuncID func) {
		foreach_impl(func);
	}

	struct Pack {
		EntityID id;
		std::tuple<Components&...> components;
	};

	std::vector<Pack> getPacked() {
		constexpr auto ids = std::make_index_sequence<sizeof...(Components)>{};
		std::vector<Pack> result;

		for (EntityID id : smallest->entity_list())
			if (all_has(id))
				result.push_back({ id, make_comp_tuple(id, ids) });
		return result;
	}


};

#endif
