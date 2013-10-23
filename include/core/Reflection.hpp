#ifndef CORE_REFLECTION_HPP
#define CORE_REFLECTION_HPP

#include <string>
#include <typeinfo>

namespace core {
	
	using namespace std;
	
	class Reflection
	{
		
	public:

		static string getRealTypeName(const type_info& typeInfo);
		
		static string demangleName(const char* name);

	private:
		
		Reflection();
		
		~Reflection();
		
	};
	
}

#endif
