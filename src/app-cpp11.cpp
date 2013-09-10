#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

using namespace std;


string separator() {
    return string(50, '-');
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
        cout << separator() << endl;
        cout << "Tests rvalues" << endl;
        cout << separator() << endl << endl;
        
        Value v1(0);
        Value v2 = move(createValue());
        cout << "v1 = @ " << &v1 << endl;
        cout << "v2 = @ " << &v2 << endl;
        
        cout << "Value of v1: " << v1.get() << " @ " << &v1 << endl;
        cout << "Value of v2: " << v2.get() << " @ " << &v2 << endl;
        
        cout << endl << separator() << endl << endl;
    }
    
}


// Test containers

namespace testcontainers {
    
    void test() {
        cout << separator() << endl;
        cout << "Tests containers" << endl;
        cout << separator() << endl << endl;
        
        vector<int> ints = { 1, 2, 3 };
        for (decltype(ints)::const_reference v: ints) {
            cout << "Dans ints : " << v << endl;
        }
    
        cout << endl << separator() << endl << endl;
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
        cout << separator() << endl;
        cout << "Tests threads" << endl;
        cout << separator() << endl << endl;
        
        sayHello();
    
        cout << endl << separator() << endl << endl;
    }
    
}


int main(void) {
    testrvalues::test();
    testcontainers::test();
    testthreads::test();
    return 0;
}
