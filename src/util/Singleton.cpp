#include "util/Singleton.hpp"

#include <mutex>

using namespace std;


namespace util
{

	void registerDestructor(void (*destructorFunc)(void))
	{
		atexit(destructorFunc);
	}

}
