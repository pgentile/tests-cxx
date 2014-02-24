#ifndef UTIL_OPTIONAL_HPP
#define UTIL_OPTIONAL_HPP

#include <iostream>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <cassert>
#include <utility>

#include "util/exceptionsafe.hpp"


namespace util
{

    template<typename T>
    class Optional final {
        
    public:
        
        typedef T type;
        
        Optional():
            _defined(false),
            _storage()
        {
        }

        Optional(Optional const& src):
            _defined(src._defined),
            _storage()
        {
            if (_defined) {
                new (&_storage) T(src.ref());
            }
        }
        
        Optional(Optional&& src):
            _defined(src._defined),
            _storage()
        {
            if (_defined) {
                new (&_storage) T(std::move(src.ref()));
            }
        }

        Optional(T const& v):
            _defined(true),
            _storage()
        {
            new (&_storage) T(v);
        }

        Optional(T&& v):
            _defined(true),
            _storage()
        {
            new (&_storage) T(std::move(v));
        }

        ~Optional() {
            EXCEPTION_SAFE_BEGIN();
            reset();
            EXCEPTION_SAFE_END();
        }
        
        Optional& operator =(Optional const& src) {
            if (src._defined) {
                if (_defined) {
                    ref() = src.ref();
                }
                else {
                    new (&_storage) T(src.ref());
                    _defined = true;
                }
            }
            else if (_defined) {
                pt()->~T();
                _defined = false;
            }
            return *this;
        }
        
        Optional& operator =(Optional&& src) {
            if (src._defined) {
                if (_defined) {
                    ref() = std::move(src.ref());
                }
                else {
                    new (&_storage) T(std::move(src.ref()));
                    _defined = true;
                }
            }
            else if (_defined) {
                pt()->~T();
                _defined = false;
            }
            return *this;
        }
        
        Optional& operator =(T const& v) {
            if (_defined) {
                ref() = v;
            }
            else {
                new (&_storage) T(v);
                _defined = true;
            }
            return *this;
        }
        
        Optional& operator =(T&& v) {
            if (_defined) {
                ref() = std::move(v);
            }
            else {
                new (&_storage) T(std::move(v));
                _defined = true;
            }
            return *this;
        }
        
        T& operator *() {
            assert(_defined);
            return ref();
        }
        
        T const& operator *() const {
            assert(_defined);
            return ref();
        }
        
        T* operator ->() {
            assert(_defined);
            return pt();
        }
        
        T const* operator ->() const {
            assert(_defined);
            return pt();
        }

        bool operator ==(Optional const other) const {
            bool result = false;
            if (this == &other) {
                result = true;
            }
            else if (_defined && other._defined) {
                result = ref() == other.ref();
            }
            else if (!_defined && !other._defined) {
                result = true;
            }
            return result;
        }
        
        bool operator <(Optional const other) const {
            bool result = false;
            if (!_defined && other._defined) {
                result = true;
            }
            else if (_defined && other._defined) {
                result = **this < *other;
            }
            return result;
        }
        
        explicit operator bool() const {
            return _defined;
        }
        
        bool operator!() const {
            return !_defined;
        }
        
        template<typename TT>
        operator Optional<TT>() const {
            if (_defined) {
                return Optional<TT>(static_cast<TT>(ref()));
            }
            else {
                return Optional<TT>();
            }
        }
        
        void reset() {
            if (_defined) {
                pt()->~T();
                _defined = false;
            }
        }
        
        void swap(Optional& other) {
            if (this != &other) {
                if (_defined && other._defined) {
                    ::std::swap(ref(), other.ref());
                }
                else if (_defined) {
                    other.ref() = std::move(ref());
                    other._defined = true;
                    _defined = false;
                }
                else if (other._defined) {
                    ref() = std::move(other.ref());
                    _defined = true;
                    other._defined = false;
                }
            }
        }
        
        template<typename F, typename R = typename std::result_of<F(T const&)>::type>
        Optional<R> apply(F func) const {
            if (_defined) {
                return Optional<R>(func(ref()));
            }
            return Optional<R>();
        }
        
        template<typename F, typename R = typename std::result_of<F(T const&)>::type::type>
        Optional<R> flatMap(F func) const {
            if (_defined) {
                return func(ref());
            }
            return Optional<R>();
        }
        
        T& orDefault(T& def) {
            if (_defined) {
                return ref();
            }
            else {
                return def;
            }
        }

        T const& orDefault(T const& def) const {
            if (_defined) {
                return ref();
            }
            else {
                return def;
            }
        }

    private:
        
        bool _defined;
        
        typename std::aligned_storage<sizeof(T), alignof(T)>::type _storage;
        
        T* pt() {
            return reinterpret_cast<T*>(&_storage);
        }
        
        T const* pt() const {
            return reinterpret_cast<T const*>(&_storage);
        }
        
        T& ref() {
            return *pt();
        }
        
        T const& ref() const {
            return *pt();
        }
        
    };

}

namespace std {
    
    template<typename T>
    ostream& operator <<(ostream& out, util::Optional<T> const& optional) {
        out << "Optional(defined = ";
        if (optional) {
            out << "true, value = " << *optional;
        }
        else {
            out << "false";
        }
        out << ')';
        return out;
    }
    
    template<typename T>
    struct hash<util::Optional<T>> {

        size_t operator()(util::Optional<T> const& optional) {
            size_t const prime = 31;
            size_t result = 1;
            result = prime * result + (optional ? 1 : 0);
            result = prime * result + (optional ? hash(*optional) : 0);
            return result;
        }

    };
    
    template<typename T>
    void swap(util::Optional<T>& left, util::Optional<T>& right) {
        left.swap(right);
    }
    
}

#endif
