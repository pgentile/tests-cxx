#include <iostream>
#include <sstream>
#include <ios>
#include <string>
#include <exception>

using namespace std;


class X {
  
public:
    
    virtual ~X() throw() {
        cout << __func__ << endl;
        throw exception();
    }
    
};


class Y: public X {
    
public:
    
    virtual ~Y() throw() {
        cout << __func__ << endl;
    }
    
};



int main(int argc, char const* argv[]) {
    Y();
	return 0;
}
