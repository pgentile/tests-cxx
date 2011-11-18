#include "patterns/Singleton.hpp"

#include <pthread.h>

namespace patterns
{

	static pthread_mutex_t exitMutex;

	static pthread_once_t exitMutexOnceControl = PTHREAD_ONCE_INIT;

	static void deleteExitMutex(void)
	{
		pthread_mutex_destroy(&exitMutex);
	}

	static void createExitMutex(void)
	{
		pthread_mutex_init(&exitMutex, NULL);
		atexit(deleteExitMutex);
	}

	void registerDestructor(void (*destructorFunc)(void))
	{
		pthread_once(&exitMutexOnceControl, createExitMutex);
		pthread_mutex_lock(&exitMutex);
		atexit(destructorFunc);
		pthread_mutex_unlock(&exitMutex);
	}

}
