#include "threading/ReadWriteLock.hpp"

#include <errno.h>
#include <pthread.h>

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace threading
{
	
	using namespace std;
	
	ReadWriteLock::ReadWriteLock(void)
	{
		pthread_rwlock_init(&_lock, NULL);
	}
	
	ReadWriteLock::~ReadWriteLock()
	{
		pthread_rwlock_destroy(&_lock);
	}
	
	void ReadWriteLock::lockRead(void)
	{
		pthread_rwlock_rdlock(&_lock);
	}
	
	void ReadWriteLock::lockWrite(void)
	{
		pthread_rwlock_wrlock(&_lock);
	}
	
	void ReadWriteLock::unlock(void)
	{
		pthread_rwlock_unlock(&_lock);
	}
	
}
