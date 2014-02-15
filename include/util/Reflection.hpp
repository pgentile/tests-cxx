#ifndef UTIL_REFLECTION_HPP
#define UTIL_REFLECTION_HPP

#include <string>
#include <typeinfo>


namespace util {
	
	std::string getRealTypeName(const std::type_info& typeInfo);
		
	std::string demangleName(const char* name);

}

#endif
