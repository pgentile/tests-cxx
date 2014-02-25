#include <iostream>
#include <typeinfo>

#include "util/Try.hpp"
#include "util/reflection.hpp"


using namespace std;
using namespace util;


double tx(int x) {
    return x * 21.333;
}


int main() {
    {
        Try<int> tryInt(5);
    
        cout << "Try = " << static_cast<Optional<int>>(tryInt) << endl;
    
        if (tryInt) {
            cout << "Try = " << *tryInt << endl;
        }
    }
    
    {
        auto transformMe = Try<double>::of<int>(&tx);
        cout << "Transformed: " << *(transformMe(5)) << endl;
    }
    
    return 0;
}
