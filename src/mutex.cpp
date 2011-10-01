#include "mutex.hpp"

extern "C"
{
#include <pthread.h>
#include <errno.h>
}

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace threading
{
	
	using namespace std;
	
	// Class Mutex
	
	Mutex::Mutex(void)
	{
		//cout << "Creating mutex " << &_ptMutex << endl;
		
		int result = pthread_mutex_init(&_ptMutex, NULL);
		_checkRC(result);
	}
	
	Mutex::~Mutex(void)
	{
		int result = pthread_mutex_destroy(&_ptMutex);
		try {
			_checkRC(result);
		} catch (const exception& e) {
			cerr << "Mutex::~Mutex() - Catched exception: " << e.what() << endl;
		}
		
		//cout << "Destructing mutex " << &_ptMutex << endl;
	}

	void Mutex::lock(void)
	{
		//cout << "Locking mutex " << &_ptMutex << endl;

		int result = pthread_mutex_lock(&_ptMutex);
		_checkRC(result);
	}
	
	bool Mutex::tryLock(void)
	{
		//cout << "Trying to lock mutex " << &_ptMutex << endl;
		
		bool locked = false;
		int result = pthread_mutex_trylock(&_ptMutex);
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

	void Mutex::release(void)
	{
		//cout << "Releasing mutex " << &_ptMutex << endl;

		int result = pthread_mutex_unlock(&_ptMutex);
		_checkRC(result);
	}
	
	void Mutex::_checkRC(int result)
	{
		if (result != 0) {
			ostringstream message;
			char* specificMessage = strerror(result);
			message << "Error on mutex " << &_ptMutex << ": code " << result << ", message = " << specificMessage;
			throw runtime_error(message.str());
		}
	}
	
	// Class MutexLock
	
	MutexLock::MutexLock(Mutex& mutex): _mutex(mutex)
	{
		_mutex.lock();
	}
	
	MutexLock::~MutexLock()
	{
		try {
			_mutex.release();
		} catch (const exception& e) {
			cerr << "MutexLock::~MutexLock() - Catched exception: " << e.what() << endl;
		}
	}
	
	// Class Condition
	
	Condition::Condition(Mutex& mutex):
			_mutex(mutex)
	{
		pthread_cond_init(&_cond, NULL);
	}
	
	void Condition::wait(void)
	{
		//cout << "Waiting on mutex " << &_mutex._ptMutex << endl;
		pthread_cond_wait(&_cond, &_mutex._ptMutex);
	}
	
	void Condition::signal(void)
	{
		//cout << "Signaling mutex " << &_mutex._ptMutex << endl;
		pthread_cond_signal(&_cond);
	}
		
	void Condition::broadcast(void)
	{
		pthread_cond_broadcast(&_cond);
	}

	Condition::~Condition()
	{
		pthread_cond_destroy(&_cond);
	}
	
}
