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
    
    void operator()()
    {
        (*m_function)();
    }
    
private:
    struct FunctionConcept
    {
        virtual ~FunctionConcept() = default;
        virtual void operator()() = 0;
    };

    template<typename CallableT>
    struct Function : FunctionConcept
    {
        Function(CallableT&& c)
            : callable(std::forward<CallableT>(c))
        {}
        
        void operator()() override {
            std::invoke(std::forward<CallableT>(callable));
        }
        
        CallableT callable;
    };
    
    std::unique_ptr<FunctionConcept> m_function;
};

}
