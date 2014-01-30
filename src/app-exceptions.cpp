#include <iostream>
#include <sstream>
#include <ios>
#include <string>
#include <exception>

#include "patterns/ExceptionSafe.hpp"

using namespace std;


class X {
  
public:
    
    virtual ~X() throw() {
        EXCEPTION_SAFE_BEGIN();
        cout << __func__ << endl;
        throw exception();
        EXCEPTION_SAFE_END();
    }
    
};


class Y: public X {
    
public:
    
    virtual ~Y() throw() {
        cout << __func__ << endl;
    }
    
};



int main(int argc, char const* argv[]) {
    Y y = Y();
    cout << __func__ << ": addr(y) = " << hex << &y << dec << endl;
	return 0;
}
