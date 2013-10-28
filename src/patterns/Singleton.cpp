#include "patterns/Singleton.hpp"

#include <mutex>

using namespace std;


namespace patterns
{

	void registerDestructor(void (*destructorFunc)(void))
	{
		atexit(destructorFunc);
	}

}
