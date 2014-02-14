#ifndef UTIL_SCOPE_GUARD_HPP
#define UTIL_SCOPE_GUARD_HPP

#include <functional>
#include <memory>

#include "patterns/ExceptionSafe.hpp"


namespace util
{

    class ScopeGuard final
    {
        
    public:
        
        ScopeGuard(std::function<void()> cleaner):
            _cleaner(cleaner)
        {
        }
        
        ScopeGuard(ScopeGuard&& src):
            _cleaner(src._cleaner)
        {
            src._cleaner = nullptr;
        }
        
        ScopeGuard(ScopeGuard const&) = delete;
        
        ~ScopeGuard() throw () {
            if (_cleaner != nullptr) {
                EXCEPTION_SAFE_BEGIN();
                _cleaner();
                EXCEPTION_SAFE_END();
            }
        }
        
        ScopeGuard& operator =(ScopeGuard&& src) {
            _cleaner = src._cleaner;
            src._cleaner = nullptr;
            return *this;
        }
        
        ScopeGuard& operator =(ScopeGuard const&) = delete;
        
        void dismiss() {
            _cleaner = nullptr;
        }

    private:
        
        std::function<void()> _cleaner;

    };
    
    
    template<typename T, typename C>
    ScopeGuard makeFuncGuard(T* target, C cleaner) {
        return ScopeGuard([=] { cleaner(*target); });
    }
    
    
    template<typename T, typename C>
    ScopeGuard makeObjGuard(T* target, C cleaner) {
        return ScopeGuard([=] { (target->*cleaner)(); });
    }
    
    
    template<typename T, typename D = std::default_delete<T>>
    ScopeGuard makeDeleteGuard(T** target, D deleter = D()) {
        return ScopeGuard([target, &deleter] { deleter(*target); *target = nullptr; });
    }
    

    template<typename T, typename D = std::default_delete<T[]>>
    ScopeGuard makeDeleteArrayGuard(T** target, D deleter = D()) {
        return makeDeleteGuard(target, deleter);
    }

}

#endif
