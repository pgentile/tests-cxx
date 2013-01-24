#include "threading/Mutex.hpp"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <pthread.h>

#include "patterns/ExceptionSafe.hpp"

namespace threading
{
	
	using namespace std;
	
	// Class Mutex
	
	Mutex::Mutex()
	{
		//cout << "Creating mutex " << &_ptMutex << endl;
		
		int result = pthread_mutex_init(&_mutex, NULL);
		_checkRC(result);
	}
	
	Mutex::~Mutex()
	{
		EXCEPTION_SAFE_BEGIN();
		int result = pthread_mutex_destroy(&_mutex);
		_checkRC(result);
		EXCEPTION_SAFE_END();
	}

	void Mutex::lock()
	{
		//cout << "Locking mutex " << &_ptMutex << endl;

		int result = pthread_mutex_lock(&_mutex);
		_checkRC(result);
	}
	
	bool Mutex::tryLock()
	{
		//cout << "Trying to lock mutex " << &_ptMutex << endl;
		
		bool locked = false;
		int result = pthread_mutex_trylock(&_mutex);
		switch (result) {
			case 0:
				locked = true;
				break;
			case EBUSY:
				// Can't lock, another thread owns the mutex
				break;
			default:
				_checkRC(result);
				break;
		}
		return locked;
	}

	void Mutex::release()
	{
		int result = pthread_mutex_unlock(&_mutex);
		_checkRC(result);
	}
	
	void Mutex::_checkRC(int result)
	{
		if (result != 0) {
			ostringstream message;
			char* specificMessage = strerror(result);
			message << "Error on mutex " << this << ": code " << result << ", message = " << specificMessage;
			throw runtime_error(message.str());
		}
	}
	
	// Class MutexLock
	
	Mutex::Lock::Lock(Mutex& mutex): _mutex(mutex)
	{
		_mutex.lock();
	}
	
	Mutex::Lock::~Lock()
	{
		EXCEPTION_SAFE_BEGIN();
		_mutex.release();
		EXCEPTION_SAFE_END();
	}
	
	// Class Condition
	
	Mutex::Condition::Condition(Mutex& mutex): _mutex(mutex)
	{
		pthread_cond_init(&_cond, NULL);
	}

	Mutex::Condition::~Condition()
	{
		pthread_cond_destroy(&_cond);
	}
	
	void Mutex::Condition::wait()
	{
		pthread_cond_wait(&_cond, &_mutex._mutex);
	}
	
	void Mutex::Condition::signal()
	{
		pthread_cond_signal(&_cond);
	}
		
	void Mutex::Condition::broadcast()
	{
		pthread_cond_broadcast(&_cond);
	}
	
}
