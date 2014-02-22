#include "util/exceptionsafe.hpp"

#include <string>
#include <iostream>
#include <mutex>

#include "util/Backtrace.hpp"

using namespace std;
using namespace util;


namespace {

    recursive_mutex CERR_MUTEX;
    
    
    string const SEPARATOR(80, '=');
    
    
    void showBacktrace(unsigned int strip) {
        cerr << SEPARATOR << endl;
        cerr << Backtrace(strip);
        cerr << SEPARATOR << endl;
    }
    
}


namespace util {
namespace exceptionsafe {
    
    void aboutException(char const* funcName, exception const* e) {
        lock_guard<recursive_mutex> lock(CERR_MUTEX);
        
        if (e) {
            cerr << funcName << ": Caught exception" << endl;
            cerr << funcName << ":   Reason: " << e->what() << endl;
        }
        else {
            cerr << funcName << ": Caught unknown exception" << endl;
        }
        showBacktrace(2);
    }
    
} // namespace exceptionsafe
} // namespace util
