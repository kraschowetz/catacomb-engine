#pragma once

#include <cat/util/sparse_set.hpp>
#include <functional>

namespace cat
{

template<typename Key, typename Value>
class DenseMap
{
public:
    template<typename U>
    void insert(Key key, U&& value)
    {
        u64 id = get_or_create_id(key);
        m_set.set(id, std::forward<U>(value));
    }

    void erase(Key key)
    {
        auto it = m_key_to_id.find(key);
        if(it == m_key_to_id.end()) return;
        m_set.erase(it->second);
        m_key_to_id.erase(it);
    }

    Watcher<Value> get(Key key)
    {
        auto it = m_key_to_id.find(key);
        if(it == m_key_to_id.end()) return nullptr;
        return m_set.get(it->second);
    }

    bool contains(Key key)
    {
        return m_key_to_id.count(key) > 0;
    }

    std::vector<Value>& data()
    {
        return m_set.data();
    }

    void clear()
    {
        m_set.clear();
        m_key_to_id.clear();
        m_next_id = 0;
    }

private:
    u64 get_or_create_id(const Key& key)
    {
        auto it = m_key_to_id.find(key);
        if(it != m_key_to_id.end()) return it->second;

        u64 id = m_next_id++;
        m_key_to_id[key] = id;
        return id;
    }

    u64 m_next_id = 1;
    std::unordered_map<Key, u64> m_key_to_id;
    SparseSet<Value> m_set;
};

}
