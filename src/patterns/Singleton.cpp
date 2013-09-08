#include "patterns/Singleton.hpp"

#include <mutex>

using namespace std;


namespace patterns
{

	static mutex exitMutex;

	void registerDestructor(void (*destructorFunc)(void))
	{
	    lock_guard<mutex> lock(exitMutex);
		atexit(destructorFunc);
	}

}
