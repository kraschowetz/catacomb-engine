#pragma once

#include <cat/config.hpp>
#include <cat/util/memory.hpp>

#include <limits>
#include <utility>
#include <vector>
#include <array>

namespace cat
{

class iSparseSet
{
public:
	virtual ~iSparseSet() = default;

	virtual void erase(u64) = 0;
	virtual void clear() = 0;
	virtual u64 size() const = 0;
	virtual bool contains_entity(u64) const = 0;
	virtual std::vector<u64> get_entity_list() = 0;
};

// @brief: an actual sparse set, maps u64 to T
template <typename T>
class SparseSet : public iSparseSet
{
public:
	SparseSet()
	{
		m_dense.reserve(INITIAL_DENSE_SIZE);
		m_dense_to_entity.reserve(INITIAL_DENSE_SIZE);
	}

    template<typename U>
	Watcher<T> set(u64 id, U&& obj)
	{
		u64 index = get_dense_index(id);

		// overwrite existing object
		if(index != NULLID)
		{
			m_dense[index] = std::forward<U>(obj);
			m_dense_to_entity[index] = id;

			return &m_dense[index];
		}
		
		// insert new index in the back of 'm_dense'
		set_dense_index(id, m_dense.size());

		m_dense.push_back(std::forward<U>(obj));
		m_dense_to_entity.push_back(id);

		return &m_dense.back();
	}

	Watcher<T> get(u64 id)
	{
		u64 index = get_dense_index(id);
		
		if(index == NULLID) return nullptr;

		return &m_dense[index];
	}

	void erase(u64 id) override
	{
		u64 index = get_dense_index(id);

		if(m_dense.empty() || index == NULLID) return;

		// swap dense.back() with the data to be deleted
			
		set_dense_index(m_dense_to_entity.back(), index);
		set_dense_index(id, NULLID);

		std::swap(m_dense[index], m_dense.back());
		std::swap(m_dense_to_entity[index], m_dense_to_entity.back());

		m_dense.pop_back();
		m_dense_to_entity.pop_back();
	}

	u64 size() const override
	{
		return m_dense.size();
	}

	std::vector<u64> get_entity_list() override
	{
		return m_dense_to_entity;
	}

	bool contains_entity(u64 id) const override
	{
		return get_dense_index(id) != NULLID;
	}

	void clear() override
	{
		m_dense.clear();
		m_dense_to_entity.clear();
		m_sparse_pages.clear();
	}

	bool empty() const
	{
		return m_dense.empty();
	}

	const std::vector<T>& data() const
	{
		return m_dense;
	}

private:
	static constexpr u64 INITIAL_DENSE_SIZE = 1024;

	static constexpr u64 SPARSE_SIZE = 2048;
	static constexpr u64 NULLID = std::numeric_limits<u64>::max();

	using Sparse = std::array<u64, SPARSE_SIZE>;

	std::vector<Sparse> m_sparse_pages;
	std::vector<T> m_dense;
	std::vector<u64> m_dense_to_entity; // maps each dense index to an u64
	
	// @brief: maps u64 to index in m_dense
	inline void set_dense_index(u64 id, u64 index)
	{
		u64 page = id / SPARSE_SIZE;
		u64 sparse_index = id % SPARSE_SIZE;

		if(page >= m_sparse_pages.size())
		{
			m_sparse_pages.resize(page + 1);
			m_sparse_pages[page].fill(NULLID);
		}

		m_sparse_pages[page][sparse_index] = index;
	}

	inline u64 get_dense_index(u64 id) const
	{
		u64 page = id / SPARSE_SIZE;
		u64 sparse_index = id % SPARSE_SIZE;

		if(page >= m_sparse_pages.size()) return NULLID;

		return m_sparse_pages[page][sparse_index];
	}
};
}
