#pragma once

#include "cat/util/memory.hpp"
#include <cat/util/dense_map.hpp>
#include <cat/util/stable_buffer.hpp>
#include <memory>
#include <typeindex>
#include <utility>

namespace cat
{

// basic ResourceLoder conncept; requiers `void unload(Resource*)`
//
// loaders are split to avoid issues with templates
// but a single struct may apply both concpets
template<typename L, typename ResT>
concept ResourceLoader = requires(L loader, ResT* res)
{
    {loader.unload(res)};
};

// "advanced" ResourceLoader concept;
// requires hash_t hash(...) & Resource load(...)
//
// loaders are split to avoid issues with templates
// but a single struct may apply both concpets
template<typename L, typename ResT, typename... Args>
concept ResourceLoaderFor = requires(L loader, ResT* ptr, Args... args) {
    { loader.hash(args...) } -> std::convertible_to<hash_t>;
    { loader.load(args...) } -> std::same_as<ResT>;
};

// type-erased ResourcePool interface
struct iResourcePool
{
    virtual ~iResourcePool() = default;

    virtual void clear() = 0;
    constexpr virtual u64 get_block_size() const = 0;
};

// concrete ResourcePool
// holds a StableBuffer (blocksize set via template) of a Resource
// ResourcePool calls `LoaderT` for loading, hashing & unloading resources
template <typename ResT, ResourceLoader<ResT> LoaderT, u64 BlockSize = 16>
struct ResourcePool : public iResourcePool
{
public:
    // wrapper for `LoaderT.unload(ResT*)`
    // used as the deleter of the StableBuffer
    struct LoaderDeleter
    {
        LoaderT* loader;
        explicit LoaderDeleter(LoaderT* l) : loader{l} {};

        void operator()(ResT* res) {loader->unload(res);}
    };

    constexpr u64 get_block_size() const override { return BlockSize; }

    ResourcePool() : m_buffer{LoaderDeleter(&m_loader)} {}

    // `args` are just the arguments for this specific loader hash getter
    // usually just the resource path as a `const std::string&`
    template<typename... Args>
    Shared<ResT> load_or_get(Args&& ...args) THROWS
    {
        hash_t hash = m_loader.hash(args...);

        if(m_ptr_map.contains(hash))
        {
            if(auto p = m_ptr_map.get(hash)->lock())
            {
                return p;
            }
        }

        // resource not loaded
        Shared<ResT> shared = m_buffer.insert(m_loader.load(std::forward<Args>(args)...));
        m_ptr_map.insert(hash, Weak<ResT>(shared));

        return shared;
    }

    void clear() override
    {
        m_ptr_map.clear();
        // TODO: add clear for storage_buffer_t
    }

private:
    LoaderT m_loader;
    DenseMap<hash_t, Weak<ResT>> m_ptr_map;
    StableBuffer<ResT, BlockSize, LoaderDeleter> m_buffer;
};

// global resource manager
// handles resource loading, storing & unloading
// holds a map of concrete ResourcePools
class ResourceManager
{
public:
    // brief: register a component pool
    // tparam `ResourceT`: typename of resource that will be registered
    // tparam `LoaderT`: ResourceLoader concept for ResourceT
    // tpatam `BlockSize`: (optional) memory block size
    template<typename ResourceT, ResourceLoader<ResourceT> LoaderT, u64 BlockSize = 16>
    void register_resource()
    {
        hash_t hash = std::hash<std::type_index>{}(typeid(ResourceT));

        m_resource_pools.insert(
            hash,
            std::make_unique<ResourcePool<ResourceT, LoaderT, BlockSize>>()
        );
    }

    // brief: load or get a loaded resource
    // return: shared ptr to resource
    // tparam `ResourceT`: typename of resource
    // tparam `LoaderT`: typename of resourceLoader<ResourceT> concept impl
    // tparam `Args...`: aditional `LoaderT::load()` arguments
    template<typename ResourceT, ResourceLoader<ResourceT> LoaderT, typename ...Args>
        requires ResourceLoaderFor<LoaderT, ResourceT, Args...>
    Shared<ResourceT> load(Args&&... args)
    {
        hash_t hash = std::hash<std::type_index>{}(typeid(ResourceT));

        CAT_ASSERT(m_resource_pools.contains(hash));

        iResourcePool* interface_ptr = m_resource_pools.get(hash).get()->get();
        ResourcePool<ResourceT, LoaderT>* pool = reinterpret_cast<ResourcePool<ResourceT, LoaderT>*>(interface_ptr);

        return pool->load_or_get(std::forward<Args>(args)...);
    }

private:
    DenseMap<hash_t, Unique<iResourcePool>> m_resource_pools;
};

}
