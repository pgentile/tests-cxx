#ifndef THREADING_READ_WRITE_LOCK_H
#define THREADING_READ_WRITE_LOCK_H

#include <pthread.h>

#include <iostream>
#include <stdexcept>

#include "macros.h"
#include "patterns/NonCopyable.hpp"

namespace threading {
	
	using namespace patterns;
	using namespace std;

	class ReadWriteLock: private NonCopyable
	{
	
	public:
		
		ReadWriteLock();
		
		~ReadWriteLock();
		
		void lockRead();
		
		void lockWrite();
		
		void unlock();
		
		class ReadLock: private NonCopyable
		{
			
		public:
			
			ReadLock(ReadWriteLock& lock);
			
			~ReadLock();
		
		private:
			
			ReadWriteLock& _lock;
			
		};
		
		class WriteLock: private NonCopyable
		{
			
		public:
			
			WriteLock(ReadWriteLock& lock);
			
			~WriteLock();
			
		private:

			ReadWriteLock& _lock;
				
		};

	private:
		
		void _checkRC(int result);
		
		pthread_rwlock_t _lock;
			
	};

}

#define SCOPED_READ_LOCK(rwLock) ::threading::ReadWriteLock::ReadLock ANONYMOUS_VAR(readLock)(rwLock);
#define SCOPED_WRITE_LOCK(rwLock) ::threading::ReadWriteLock::WriteLock ANONYMOUS_VAR(writeLock)(rwLock);

#endif
