#ifndef CORE_REFLECTION_HPP
#define CORE_REFLECTION_HPP

#include <string>
#include <typeinfo>

namespace core {
	
	using namespace std;
	
	class Reflection
	{
		
	public:

		static const string getRealTypeName(const type_info& typeInfo);
		
		static const string demangleName(const char* name);

	private:
		
		Reflection();
		
		~Reflection();
		
	};
	
}

#endif
