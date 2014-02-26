#ifndef UTIL_TRY_HPP
#define UTIL_TRY_HPP

#include <exception>
#include <memory>
#include <type_traits>
#include <functional>

#include <util/Optional.hpp>


namespace util {
    

template<typename T>
class TryImpl {

public:

    virtual ~TryImpl() noexcept {
    }
    
    virtual T& get() = 0;
    
    virtual T const& get() const = 0;
    
    virtual bool isSuccess() const = 0;
    
    virtual std::exception_ptr getException() const = 0;
    
};


template<typename T>
class SuccessImpl final: public TryImpl<T> {

public:

    SuccessImpl(T const& value):
        _value(value)
    {
    }

    SuccessImpl(T&& value):
        _value(std::move(value))
    {
    }

    virtual ~SuccessImpl() noexcept {
    }
    
    virtual T& get() override {
        return _value;
    }
    
    virtual T const& get() const override {
        return _value;
    }
    
    virtual bool isSuccess() const override {
        return true;
    }
    
    virtual std::exception_ptr getException() const override {
        return std::exception_ptr();
    }

private:

    T _value;

};


template<typename T>
class FailureImpl final: public TryImpl<T> {

public:
    
    FailureImpl(std::exception_ptr exc):
        _exc(exc)
    {
        assert(static_cast<bool>(exc));
    }

    virtual ~FailureImpl() noexcept {
    }
    
    [[ noreturn ]] virtual T& get() override {
        std::rethrow_exception(_exc);
    }
    
    [[ noreturn ]] virtual T const& get() const override {
        std::rethrow_exception(_exc);
    }
    
    virtual bool isSuccess() const override {
        return false;
    }
    
    virtual std::exception_ptr getException() const override {
        return _exc;
    }

private:
    
    std::exception_ptr _exc;

};


template<typename T>
class Try final {
    
public:
    
    typedef T type;
    
    Try(T const& value): Try(new SuccessImpl<T>(value)) {
    }
    
    Try(T&& value): Try(new SuccessImpl<T>(std::move(value))) {
    }
    
    Try(std::exception_ptr exc): Try(new FailureImpl<T>(exc)) {
    }
    
    Try(Try const& src): Try(nullptr) {
        if (src) {
            _impl.reset(new SuccessImpl<T>(*src));
        }
        else {
            std::exception_ptr exc = src.getException();
            _impl.reset(new FailureImpl<T>(exc));
        }
    }
    
    Try(Try&& src):
        _impl(std::move(src._impl))
    {
    }
    
    ~Try() noexcept {
    }
    
    Try& operator =(Try&& src) {
        _impl = std::move(src._impl);
        return *this;
    }
    
    Try& operator =(Try const& src) {
        if (src) {
            _impl.reset(new SuccessImpl<T>(*src));
        }
        else {
            std::exception_ptr exc = src.getException();
            _impl.reset(new FailureImpl<T>(exc));
        }
        return *this;
    }
    
    T& operator *() {
        return _impl->get();
    }
    
    T const& operator *() const {
        return _impl->get();
    }

    explicit operator bool() const {
        return _impl->isSuccess();
    }
    
    bool operator !() const {
        return !(_impl->isSuccess());
    }
    
    explicit operator Optional<T>() const {
        if (_impl->isSuccess()) {
            return Optional<T>(_impl->get());
        }
        return Optional<T>();
    }

    std::exception_ptr getException() const {
        return _impl->getException();
    }
    
    template<typename F, typename R = typename std::result_of<F(T const&)>::type>
    Try<R> apply(F const& func) const {
        if (_impl->isSuccess()) {
            return Try(func(_impl->get()));
        }
        return Try(_impl->getException());
    }
    
    template<typename F, typename R = typename std::result_of<F(T const&)>::type::type>
    Try<R> flatMap(F const& func) const {
        if (_impl->isSuccess()) {
            return func(_impl->get());
        }
        return Try<R>(_impl->getException());
    }
    
    template<typename I>
    static std::function<Try<T>(I) noexcept> of(std::function<T(I)> func) {
        return [func] (I const& value) noexcept {
            try {
                return Try<T>(func(value));
            }
            catch (...) {
                return Try<T>(std::current_exception());
            }
        };
    }

private:
    
    Try(TryImpl<T>* impl):
        _impl(impl)
    {
    }
    
    std::unique_ptr<TryImpl<T>> _impl;
    
};


}

#endif
