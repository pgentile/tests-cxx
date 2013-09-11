#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <typeinfo>

using namespace std;


string separator(char c) {
    return string(70, c);
}


// Tests rvalue and mode semantics

namespace testrvalues {

    class Value
    {
        
    public:
        
        Value(): _x(-1) {
        }
        
        explicit Value(int x): _x(x) {
            cout << "Value @ " << this << ": Default constructor, for " << x << endl;
        }
        
        Value(Value const& src): _x(src._x) {
            cout << "Value @ " << this << ": Copy constructor, from " << src._x << endl;
        }
        
        Value(Value&& src): _x(src._x) {
            src._x = -1;
            cout << "Value @ " << this << ": Move constructor, from " << src._x << endl;
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
        return Value(999);
    }
    
    void test() {
        Value v1(0);
        Value v2 = move(createValue());
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
    void printAlignOf(char const* typeName) {
        cout << "Alignment / size of " << typeName << ": "
             << alignof(T) << " / " << sizeof(T) << endl;
    }
    
    void test() {
        PRINT_ALIGN_OF(char);
        PRINT_ALIGN_OF(int8_t);
        PRINT_ALIGN_OF(int16_t);
        PRINT_ALIGN_OF(int32_t);
        PRINT_ALIGN_OF(int64_t);
    }
    
}


template<typename T>
void runTest(char const* title, T func) {
    cout << separator('=') << endl;
    cout << title << endl;
    cout << separator('=') << endl << endl;

    func();
    
    cout << endl << separator('-') << endl << endl;
}


int main(void) {
    runTest("Test rvalues", &testrvalues::test);
    runTest("Test containers", &testcontainers::test);
    runTest("Test threads", &testthreads::test);
    runTest("Test align", &testalign::test);
    return 0;
}
