#include <iostream>
#include <sstream>
#include <ios>
#include <string>
#include <exception>
#include <stdexcept>

#include "util/exceptionsafe.hpp"

using namespace std;


class X {
  
public:
    
    virtual ~X() throw() {
        EXCEPTION_SAFE_BEGIN();
        cout << __func__ << endl;
        throw runtime_error("Ca marche pas");
        EXCEPTION_SAFE_END();
    }
    
};


class Y {
    
public:
    
    void run() {
        X x;
        throw runtime_error("Banane");
    }
    
};



int main() {
    try {
        Y y;
        y.run();
    	return 0;
    }
    catch (exception const& e) {
        cout << __func__ << ": erreur attrapee : " << e.what() << endl;
    }
    return 1;
}
