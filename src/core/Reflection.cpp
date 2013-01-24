#include "core/Reflection.hpp"

#include <cxxabi.h>

namespace core {

	const string Reflection::getRealTypeName(const type_info& typeInfo) {
		const char* name = typeInfo.name();
		return demangleName(name);
	}
	
	const string Reflection::demangleName(const char* name) {
		int abiStatus;
		char* realNamePt = abi::__cxa_demangle(name, NULL, NULL, &abiStatus);
		string realName;
		if (abiStatus == 0) {
			realName = realNamePt;
		} else {
			realName = name;
		}
		free(realNamePt);
		return realName;
	}

}
