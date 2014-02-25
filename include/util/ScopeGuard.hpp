#ifndef UTIL_SCOPE_GUARD_HPP
#define UTIL_SCOPE_GUARD_HPP

#include <functional>
#include <memory>

#include "util/exceptionsafe.hpp"
#include "macros.h"


#include <iostream>

#define SCOPE_GUARD(WHAT) ::util::ScopeGuard ANONYMOUS_VAR(_scopeGuard)(WHAT)

#define MAKE_SIMPLE_SCOPE_GUARD(WHAT) ::util::ScopeGuard([&] () { WHAT; })

#define SIMPLE_SCOPE_GUARD(WHAT) SCOPE_GUARD([&] () { WHAT; })


namespace util
{

    class ScopeGuard final
    {
        
    public:    
        
        ScopeGuard():
            _cleaner(nullptr)
        {
        }
        
        explicit ScopeGuard(std::function<void()> const& cleaner):
            _cleaner(cleaner)
        {
        }
        
        explicit ScopeGuard(std::function<void()>&& cleaner):
            _cleaner(std::move(cleaner))
        {
        }
        
        ScopeGuard(ScopeGuard&& src):
            _cleaner(std::move(src._cleaner))
        {
        }
        
        ScopeGuard(ScopeGuard const&) = delete;
        
        ~ScopeGuard() noexcept {
            if (_cleaner) {
                EXCEPTION_SAFE_BEGIN();
                _cleaner();
                EXCEPTION_SAFE_END();
            }
        }
        
        ScopeGuard& operator =(ScopeGuard&& src) {
            _cleaner = std::move(src._cleaner);
            return *this;
        }
        
        ScopeGuard& operator =(ScopeGuard const&) = delete;
        
        void dismiss() {
            _cleaner = nullptr;
        }

    private:
        
        std::function<void()> _cleaner;

    };

}

#endif
