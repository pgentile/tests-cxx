#include "threading/ReadWriteLock.hpp"

#include <cerrno>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <stdexcept>
#include <cstring>

#include "util/exceptionsafe.hpp"

namespace threading
{
	
	using namespace std;
	
	ReadWriteLock::ReadWriteLock()
	{
		int result = pthread_rwlock_init(&_lock, NULL);
		_checkRC(result);
	}
	
	ReadWriteLock::~ReadWriteLock()
	{
		EXCEPTION_SAFE_BEGIN();
		int result = pthread_rwlock_destroy(&_lock);
		_checkRC(result);
		EXCEPTION_SAFE_END();
	}
	
	void ReadWriteLock::lockRead()
	{
		cout << this << ": lock read" << endl;
		int result = pthread_rwlock_rdlock(&_lock);
		_checkRC(result);
	}
	
	void ReadWriteLock::lockWrite()
	{
		cout << this << ": lock write" << endl;
		int result = pthread_rwlock_wrlock(&_lock);
		_checkRC(result);
	}
	
	void ReadWriteLock::unlock()
	{
		cout << this << ": unlock" << endl;
		int result = pthread_rwlock_unlock(&_lock);
		_checkRC(result);
	}
	
	void ReadWriteLock::_checkRC(int result)
	{
		if (result != 0) {
			ostringstream message;
			char* specificMessage = strerror(result);
			message << "Error on read/write lock " << this << ": code " << result << ", message = " << specificMessage;
			throw runtime_error(message.str());
		}
	}
	
	ReadWriteLock::ReadLock::ReadLock(ReadWriteLock& lock): _lock(lock)
	{
		_lock.lockRead();
	}
	
	ReadWriteLock::ReadLock::~ReadLock()
	{
		EXCEPTION_SAFE_BEGIN();
		_lock.unlock();
		EXCEPTION_SAFE_END();
	}	
	
	ReadWriteLock::WriteLock::WriteLock(ReadWriteLock& lock): _lock(lock)
	{
		_lock.lockWrite();
	}
	
	ReadWriteLock::WriteLock::~WriteLock()
	{
		EXCEPTION_SAFE_BEGIN();
		_lock.unlock();
		EXCEPTION_SAFE_END();
	}
	
}
