#pragma once

#include <memory>
#include <functional>

namespace cat
{

struct FunctionPtr
{
public:
    FunctionPtr() : m_function(nullptr) {}

    template<typename CallableT>
    FunctionPtr(CallableT&& callable)
        : m_function(
            std::make_unique<Function<CallableT>>(std::forward<CallableT>(callable))
        )
    {
        static_assert(std::is_invocable_v<CallableT>, "callable must be invocable");
    }
    
    void operator()() const
    {
        (*m_function)();
    }

    operator bool() const
    {
        return m_function != nullptr;
    }
    
private:
    struct FunctionConcept
    {
        virtual ~FunctionConcept() = default;
        virtual void operator()() const = 0;
    };

    template<typename CallableT>
    struct Function : FunctionConcept
    {
        Function(CallableT&& c)
            : callable(std::forward<CallableT>(c))
        {}
        
        void operator()() const override {
            std::invoke(std::forward<const CallableT>(callable));
        }
        
        CallableT callable;
    };
    
    std::unique_ptr<FunctionConcept> m_function;
};

}
