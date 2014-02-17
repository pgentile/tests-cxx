#include "util/reflection.hpp"

#include <memory>
#include <cstdlib>
#include <cxxabi.h>

using namespace std;


namespace {
    
    struct FreePointer {
        
        template<typename T>
        void operator ()(T* pt) {
            free(pt);
        }
        
    };
    
}


namespace util {
namespace reflection {

	string getRealTypeName(const type_info& typeInfo) {
		const char* name = typeInfo.name();
		return demangleName(name);
	}
	
	
	string demangleName(const char* name) {
		int abiStatus;
        unique_ptr<char, FreePointer> realName(abi::__cxa_demangle(name, NULL, NULL, &abiStatus));
		if (abiStatus == 0) {
            return realName.get();
		}
        return name;
	}

} // namespace reflection
} // namespace util
