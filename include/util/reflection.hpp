#ifndef UTIL_REFLECTION_HPP
#define UTIL_REFLECTION_HPP

#include <string>
#include <typeinfo>


namespace util {
namespace reflection {
    
	
	std::string getRealTypeName(const std::type_info& typeInfo);
		
	std::string demangleName(const char* name);
    
    template<typename T>
    std::string getRealTypeName() {
        return getRealTypeName(typeid(T));
    }
    
} // namespace reflection
} // namespace util

#endif
