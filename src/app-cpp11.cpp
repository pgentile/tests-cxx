#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <typeinfo>
#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <sstream>
#include <type_traits>
#include <functional>

#include "util/Optional.hpp"
#include "date/LocalDate.hpp"
#include "date/LocalDateTime.hpp"
#include "date/LocalDateTimeRange.hpp"


using namespace std;
using util::Optional;


string separator(char c) {
    return string(70, c);
}


// Tests rvalue and mode semantics

namespace testrvalues {

    class Value
    {
        
    public:
        
        explicit Value(int x): _x(x) {
            cout << "Value @ " << this << ": Explicit constructor, for " << x << endl;
        }
        
        Value(Value const& src): _x(src._x) {
            cout << "Value @ " << this << ": Copy constructor, from " << &src << endl;
        }
        
        Value(Value&& src): _x(src._x) {
            src._x = -1;
            cout << "Value @ " << this << ": Move constructor, from " << &src << endl;
        }
        
        ~Value() {
            cout << "Value @ " << this << ": Delete" << endl;
        }
        
        Value& operator =(Value const& src) {
            _x = src._x;
            cout << "Value @ " << this << ": Copy operator" << endl;
            return *this;
        }
        
        Value& operator =(Value&& src) {
            _x = src._x;
            cout << "Value @ " << this << ": Move operator" << endl;
            return *this;
        }        
        
        int get() const {
            return _x;
        }
        
        void set(int x) {
            _x = x;
        }
        
    private:
        
        int _x;
        
    };
    
    Value createValue() {
        Value created(999);
        cout << "created value: " << created.get() << " @ " << &created << endl;
        return created;
    }
    
    void test() {
        Value v1(0);
        Value v2(move(createValue()));
        cout << "v1 = @ " << &v1 << endl;
        cout << "v2 = @ " << &v2 << endl;
        
        cout << "Value of v1: " << v1.get() << " @ " << &v1 << endl;
        cout << "Value of v2: " << v2.get() << " @ " << &v2 << endl;
    }
    
}


// Test containers

namespace testcontainers {
    
    void test() {
        vector<int> ints = { 1, 2, 3 };
        for (decltype(ints)::const_reference v: ints) {
            cout << "Dans ints : " << v << endl;
        }
    }
    
}


// Test threads

namespace testthreads {
    
    thread createHelloThread(mutex& outMutex, int n) {
        auto task = [&outMutex](int n) {
            lock_guard<mutex> lock(outMutex);
            cout << "[" << this_thread::get_id() << "] I'm a thread, #" << n << "!" << endl;
        };
        return thread(task, n);
    }
    
    
    void sayHello() {
        mutex outMutex;
        vector<thread> helloThreads;
        for (int i = 0; i < 10; i++) {
            helloThreads.push_back(createHelloThread(outMutex, i + 1));
        }
        for (thread& helloThread: helloThreads) {
            helloThread.join();
        }
    }
    
    void test() {
        sayHello();
    }
    
}


// Test align

#define PRINT_ALIGN_OF(type) (printAlignOf<type>(#type))


namespace testalign {
    
    template<typename T>
    struct RefForNonIntegral {
        
        typedef typename conditional<is_integral<T>::value, T, typename add_lvalue_reference<T>::type>::type type;
        
    };
    
    class TestValue {
        
    public:
        
        TestValue(int v): _v(v) {
            cout << "@ " << this << ": TestValue(int v) with v = " << v << endl;
        }
        
        TestValue(TestValue const& src): _v(src._v) {
            cout << "@ " << this << ": TestValue(TestValue const& src) with src = " << src << endl;
        }
        
        TestValue(TestValue&& src): _v(src._v) {
            cout << "@ " << this << ": TestValue(TestValue&& src) with src = " << src << endl;
        }
        
        ~TestValue() {
            cout << "@ " << this << ": ~TestValue() with this->_v = " << _v << endl;
        }
        
        TestValue& operator =(TestValue const& src) {
            cout << "@ " << this << ": operator =(TestValue const& src) with src = " << src << endl;
            _v = src._v;
            
            return *this;
        }
        
        TestValue& operator =(TestValue&& src) {
            cout << "@ " << this << ": operator =(TestValue&& src) with src = " << src << endl;
            _v = src._v;
            
            return *this;
        }
        
        long v() const {
            return _v;
        }
        
    private:
        
        long _v;
        
        friend ostream& operator <<(ostream& out, TestValue const& testValue);
        
    };
    
    ostream& operator <<(ostream& out, TestValue const& testValue) {
        return out << "TestValue(" << testValue.v() << ")";
    }
    
    template<typename T>
    void printAlignOf(char const* typeName) {
        cout << "Alignment / size of " << typeName << ": "
             << alignof(T) << " / " << sizeof(T) << endl;
    }
    
    void printAlignments() {
        cout << "Alignments :" << endl;
        PRINT_ALIGN_OF(char);
        PRINT_ALIGN_OF(int8_t);
        PRINT_ALIGN_OF(int16_t);
        PRINT_ALIGN_OF(int32_t);
        PRINT_ALIGN_OF(int64_t);
        PRINT_ALIGN_OF(float);
        PRINT_ALIGN_OF(double);
        PRINT_ALIGN_OF(bool);
        PRINT_ALIGN_OF(int);
        PRINT_ALIGN_OF(long);
        PRINT_ALIGN_OF(long long);
        PRINT_ALIGN_OF(long double);
        PRINT_ALIGN_OF(Optional<bool>);
        PRINT_ALIGN_OF(Optional<short>);
        PRINT_ALIGN_OF(Optional<int>);
        PRINT_ALIGN_OF(Optional<long>);
        
        typedef typename aligned_storage<sizeof(char), alignof(max_align_t)>::type Aligned;
        PRINT_ALIGN_OF(Aligned);
    }
    
    void charArrayAsFloat() {
        alignas(float) char aligned[sizeof(float)];
        float* f = reinterpret_cast<float*>(aligned);
        *f = 45.6f;
        cout << "Float: " << *f << endl;
        cout << "Bytes:";
        for (char c: aligned) {
            cout << " 0x" << hex << static_cast<int>(c) << dec;
        }
        cout << endl;
    }
    
    void placementNew() {
        alignas(string) char aligned[sizeof(string)];
        cout << "sizeof(aligned) = " << sizeof(aligned) << endl;
        cout << "@ aligned = " << &aligned << endl;
        
        string* pt = new (aligned) string("Hello");
        cout << "@ pt = " << pt << endl;
        cout << "*pt = " << *pt << endl;
        
        pt->~string();
    }
    
    void testOptional() {
        {
            Optional<int> x(6);
            Optional<int> y;
            Optional<int> z(7);
            
            Optional<long> a(z);

            cout << "x: " << x << endl;
            cout << "y: " << y << endl;
            cout << "z: " << z << endl;
            cout << "a: " << a << endl;

            y = x;
            z = x;

            cout << "x: " << x << endl;
            cout << "y: " << y << endl;
            cout << "z: " << z << endl;

            x.reset();
            y.reset();

            cout << "x: " << x << endl;
            cout << "y: " << y << endl;
            cout << "z: " << z << endl;

            x = 90;
            Optional<double> xDouble = x.apply([] (int v) { return v * 1.123; });
            cout << "Test X: " << xDouble << endl;
        }
        
        cout << endl;
        
        {
            Optional<TestValue> x;
            Optional<TestValue> y(x);
            Optional<TestValue> z(1);
            x = 2;
            z.reset();
            z = 3;
        }
        
        cout << endl;
        
        {
            Optional<TestValue> x(1);
            Optional<TestValue> y(2);
            
            cout << "x = " << x << ", y = " << y << endl;
            
            swap(x, y);
            
            cout << "x = " << x << ", y = " << y << endl;
        }
        
        {
            cout << "sizeof(TestValue) = " << sizeof(TestValue) << endl;
            cout << "sizeof(Optional<TestValue>) = " << sizeof(Optional<TestValue>) << endl;
            cout << "sizeof(bool) = " << sizeof(bool) << endl;
            cout << "sizeof(Optional<bool>) = " << sizeof(Optional<bool>) << endl;
            cout << "sizeof(int) = " << sizeof(int) << endl;
            cout << "sizeof(Optional<int>) = " << sizeof(Optional<int>) << endl;
            cout << "sizeof(long) = " << sizeof(long) << endl;
            cout << "sizeof(Optional<long>) = " << sizeof(Optional<long>) << endl;
            cout << "sizeof(long long) = " << sizeof(long long) << endl;
            cout << "sizeof(Optional<long long>) = " << sizeof(Optional<long long>) << endl;
        }
    }
    
    void test() {
        printAlignments();
        // charArrayAsFloat();
        // placementNew();
        // testOptional();
    }
    
}


// Test attributes

namespace testattributes {
    
    [[noreturn]] void fuck() {
        exit(EXIT_FAILURE);
    }
    
}


// Test templates

namespace testtemplates {
    
    template<typename... A>
    inline void pass(const A&...) {
    }
    
    template<typename... A>
    void print(const A&... args) {
        cout << string(10, '.') << endl;
        
        //(operator <<)(cout, args)...;
        
        pass((cout << args << ' ')...);
        cout << endl;
        cout << string(10, '.') << endl;
    }
    
    template<typename... A>
    string concat(string const& sep, const A&... args) {
        stringstream concatenatedStream;
        pass((concatenatedStream << args << sep)...);
        
        string concatenated = concatenatedStream.str();
        if (sizeof...(A)) {
            concatenated.erase(concatenated.length() - sep.size());
        }
        return concatenated;
    }
    
    void test() {
        cout << concat(", ", 'A', 2, 3.4, "BCD") << endl;
    }
    
}


// Test misc

namespace testmisc {
    
    
    void aboutNullprt() {
        int* x = nullptr;
        cout << "@ in x: " << x << endl;
    }
    
    void test() {
        aboutNullprt();
    }
    
}


// Test allocators

namespace testallocators {
    
    template<typename T, size_t S>
    class StackAllocator
    {
        
    public:
        
        typedef T value_type;
        
        template<typename O>
        struct rebind {
            
            typedef StackAllocator<O, S> other;
            
        };
        
        StackAllocator() {
        }
        
        T* allocate(size_t n) {
            cout << "Allocate " << n << " elements" << endl;
            if (_currentSize + n > S) {
                throw bad_alloc();
            }
            T* pt = reinterpret_cast<T*>(&_storage) + _currentSize;
            _currentSize += n;
            return pt;
        }
        
        void deallocate(T* /*ptr*/, size_t /*n*/) {
            
        }
        
        size_t max_size() const {
            return S;
        }
        
    private:
        
        typename aligned_storage<sizeof(T), alignof(T)>::type _storage[S];
        
        size_t _currentSize;
        
    };
    
    
    void useMyAllocator() {
        vector<int, StackAllocator<int, 10>> v = {1, 2, 3};
        v.push_back(4);
        for (int x: v) {
            cout << "Valeur dans x : " << x << endl;
        }
    }
    
    void test() {
        useMyAllocator();
    }
    
}


// Tests programmation fonctionnelle

namespace testfunctional {
    
    void moveFunction() {
        typedef function<void(string const&)> F;
        
        F showName1 = [](string const& name) { cout << "Mon nom est " << name << endl; };
        F showName2;
        
        cout << "Etat showName1 : " << static_cast<bool>(showName1) << endl;
        cout << "Etat showName2 : " << static_cast<bool>(showName2) << endl;
        
        showName2 = std::move(showName1);
        
        cout << "Etat showName1 : " << static_cast<bool>(showName1) << endl;
        cout << "Etat showName2 : " << static_cast<bool>(showName2) << endl;
    }
    
    void test() {
        moveFunction();
    }
    
}


template<typename F>
void runTest(char const* title, F const& func) {
    cout << separator('=') << endl;
    cout << title << endl;
    cout << separator('=') << endl << endl;

    func();
    
    cout << endl << separator('-') << endl << endl;
}


int main(void) {
    // runTest("Test rvalues", &testrvalues::test);
    // runTest("Test containers", &testcontainers::test);
    // runTest("Test threads", &testthreads::test);
    // runTest("Test align", &testalign::test);
    // runTest("Test templates", &testtemplates::test);
    // runTest("Test misc", &testmisc::test);
    // runTest("Test allocators", &testallocators::test);
    runTest("Test programmation fonctionnelle", &testfunctional::test);
    
    // cout << "dateTime = " << date::LocalDateTime(2013, 6, 7, 8, 20, 44) << endl;
    // cout << "date     = " << date::LocalDate(2014, 6, 7) << endl;
    // cout << "range    = " << date::LocalDateTimeRange(date::LocalDateTime(2012, 6, 7, 8, 20, 44)) << endl;
    
    return 0;
}
