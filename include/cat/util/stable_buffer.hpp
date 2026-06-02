#pragma once

#include <array>
#include <cat/core/memory.hpp>
#include <cat/config.hpp>
#include <deque>
#include <type_traits>

// brief: defines a StableBuffer struct;
// a StableBuffer is, roughly, a deque of arrays. It is designed this way
// to minimize reallocs, so it would be a safe owner for pointers (eg.
// catacomb's ResourceManager uses StableBuffers under the hood)
//
// each insertion creates a memory block with size matchin its template argument
// and that block is never reallocated and its only freed when its fully empty

namespace cat
{

// helper struct, represents a deleter that does nothing
struct NoopDeleter
{
    template <typename T>
    void operator()(T*) {}
};

template <typename T, u64 BlockSize, typename Deleter = NoopDeleter>
struct StableBuffer
{
    static_assert(BlockSize > 0, "BlockSize must be greater than 0");

public:
    StableBuffer() = default;

    // assing a deleter ctor
    explicit StableBuffer(Deleter deleter) : m_deleter(std::move(deleter)) {}
    
    // inserts an element & returns a SharedPtr to the inserted obj
    // need an rvalue as the StableBuffer should always own its elements
    Shared<T> insert(T&& obj)
    {
        for(u64 i = 0; i < m_blocks.size(); ++i)
        {
            if(!m_blocks[i].has_free_slot()) continue;

            return make_shared(m_blocks[i], std::move(obj));
        }

        // no block is free
        m_blocks.emplace_back(m_last_block_hash++);

        return make_shared(m_blocks.back(), std::move(obj));
    }

    u64 block_count() const { return m_blocks.size(); }

private:

    // @brief: define a memory block for a StableBuffer
    struct Block
    {
    public:
        Block()
        {
            // fill m_free_slots
            for(u64 i = 0; i < BlockSize; ++i)
            {
                m_free_slots[i] = i;
            }
        }

        Block(hash_t hash)
        : m_id(hash)
        {
            // fill m_free_slots
            for(u64 i = 0; i < BlockSize; ++i)
            {
                m_free_slots[i] = i;
            }
        }

        bool has_free_slot() const
        {
            return m_free_count > 0;
        }

        bool empty() const
        {
            return m_free_count == BlockSize;
        }

        // @brief: write `obj` into slot and return {`slot`,`ptr`}
        std::pair<u64, T*> insert(T&& obj)
        {
            CAT_ASSERT(has_free_slot());

            u64 slot = m_free_slots[--m_free_count];

            T* ptr = reinterpret_cast<T*>(&m_buffer[slot]);
            new (ptr) T(std::move(obj));

            return {slot, reinterpret_cast<T*>(&m_buffer[slot])};
        }

        void erase(u64 slot)
        {
            m_free_slots[m_free_count++] = slot;
        }

        bool operator==(const Block& other) const
        {
            return this->m_id == other.m_id;
        }

    private:
        // raw memeory storage to avoid calling ctors & dtors
        using Storage = std::aligned_storage_t<sizeof(T), alignof(T)>;
        std::array<Storage, BlockSize> m_buffer;
        
        std::array<u64, BlockSize> m_free_slots;
        u64 m_free_count = BlockSize;
        hash_t m_id;
    };

    // helper method: inserts an obj into a block & return its ptr
    Shared<T> make_shared(Block& block, T&& obj)
    {
        auto [slot, ptr] = block.insert(std::move(obj));

        return Shared<T>(ptr, 
            // custom deleter, should call `Deleter`, obj dtor and check if memory block could be freed
            [this, &block, slot](T* resource)
            {
                if constexpr(!std::is_same_v<Deleter, NoopDeleter>)
                {
                    m_deleter(resource);
                }

                resource->~T();
                
                block.erase(slot);
                
                // check if block could be freed
                if(block.empty())
                {
                    std::erase_if(m_blocks, [&block](Block& _block){ return _block == block;});
                }
            }
        );
    }

    // use a deque so no block reallocs would happen
    std::deque<Block> m_blocks;
    hash_t m_last_block_hash = 0;

        // zero-size if NoopDeleter, thanks to [[no_unique_address]]
    [[no_unique_address]] Deleter m_deleter;
};

}
