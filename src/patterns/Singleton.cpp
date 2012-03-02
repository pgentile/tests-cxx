#include "patterns/Singleton.hpp"

#include "threading/Mutex.hpp"

namespace patterns
{
	
	using namespace threading;

	static Mutex exitMutex;

	void registerDestructor(void (*destructorFunc)(void))
	{
		Mutex::Lock lock(exitMutex);
		atexit(destructorFunc);
	}

}
