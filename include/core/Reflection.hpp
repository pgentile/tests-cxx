#ifndef CORE_REFLECTION_HPP
#define CORE_REFLECTION_HPP

#include <string>
#include <typeinfo>


namespace core {
	
	std::string getRealTypeName(const std::type_info& typeInfo);
		
	std::string demangleName(const char* name);

}

#endif
