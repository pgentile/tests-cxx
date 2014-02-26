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
    
    {
        Try<Optional<int>> tryOptional(Optional<int>{1});
        
        cout << "Try = " << *tryOptional << endl;
        
        Try<int> tryInt = tryOptional.flatMap([] (Optional<int> const& optional) {
            if (optional) {
                return Try<int>(*optional);
            }
            return Try<int>(make_exception_ptr(runtime_error("Undefined")));
        });
        
        cout << "FlatMap: " << *tryInt << endl;
    }
    
    return 0;
}
