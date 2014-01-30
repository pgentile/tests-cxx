#ifndef UTIL_OPTIONAL_HPP
#define UTIL_OPTIONAL_HPP

#include <iostream>
#include <memory>
#include <algorithm>
#include <type_traits>



namespace util
{
    
    using std::cout;
    using std::endl;


    template<typename T>
    class Optional {
        
    public:
        
        Optional(): _defined(false) {
            cout << "CONSTRUCTOR: Optional()" << endl;
        }

        Optional(Optional const& src): _defined(src._defined) {
            cout << "CONSTRUCTOR: Optional(Optional const& src) with src = " << src << endl;
            if (_defined) {
                new (&_storage) T(*src);
            }
        }
        
        Optional(Optional&& src): _defined(src._defined) {
            cout << "CONSTRUCTOR: Optional(Optional&& src) with src = " << src << endl;
            if (_defined) {
                new (&_storage) T(std::move(*src));
            }
        }

        Optional(T const& v): _defined(true) {
            cout << "CONSTRUCTOR: Optional(T const& v) with v = " << v << endl;
            new (&_storage) T(v);
        }

        Optional(T&& v): _defined(true) {
            cout << "CONSTRUCTOR: Optional(T && v) with v = " << v << endl;
            new (&_storage) T(std::move(v));
        }
    
        ~Optional() {
            reset();
        }
        
        Optional& operator =(Optional const& src) {
            reset();
            _defined = src._defined;
            if (_defined) {
                new (&_storage) T(*src);
            }
            return *this;
        }
        
        Optional& operator =(Optional&& src) {
            reset();
            _defined = src._defined;
            if (_defined) {
                new (&_storage) T(std::move(*src));
                src._defined = false;
            }
            return *this;
        }
        
        Optional& operator =(T const& v) {
            reset();
            _defined = true;
            new (&_storage) T(v);
            return *this;
        }
        
        Optional& operator =(T&& v) {
            reset();
            _defined = true;
            new (&_storage) T(std::move(v));
            return *this;
        }
        
        T& operator *() {
            // FIXME Assert defined !!!
            T* pt = reinterpret_cast<T*>(&_storage);
            return *pt;
        }
        
        T const& operator *() const {
            // FIXME Assert defined !!!
            T const* pt = reinterpret_cast<T const*>(&_storage);
            return *pt;
        }
        
        T* operator ->() {
            // FIXME Assert defined !!!
            return reinterpret_cast<T*>(&_storage);
        }
        
        T const* operator ->() const {
            // FIXME Assert defined !!!
            return reinterpret_cast<T const*>(&_storage);
        }

        bool operator ==(Optional<T> const other) const {
            bool result = false;
            if (_defined && other._defined) {
                result = (**this == *other);
            }
            else if (!_defined && !other._defined) {
                result = true;
            }
            return result;
        }
        
        explicit operator bool() const {
            return _defined;
        }
        
        void reset() {
            if (_defined) {
                T* pt = reinterpret_cast<T*>(&_storage);
                pt->~T();
                _defined = false;
            }
        }
        
        template<typename F, typename R = typename std::result_of<F(T const&)>::type>
        Optional<R> apply(F func) const {
            if (_defined) {
                return Optional<R>(func(**this));
            }
            else {
                return Optional<R>();
            }
        }

    private:
        
        bool _defined;
        
        typename std::aligned_storage<sizeof(T), alignof(T)>::type _storage;
        
    };
    
    
    template<typename T>
    std::ostream& operator <<(std::ostream& out, Optional<T> const& optional) {
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

}

#endif
