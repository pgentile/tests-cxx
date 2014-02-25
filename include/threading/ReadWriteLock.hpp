#ifndef THREADING_READ_WRITE_LOCK_HPP
#define THREADING_READ_WRITE_LOCK_HPP

#include <pthread.h>

#include <iostream>
#include <stdexcept>

#include "macros.h"


namespace threading {
	
	using namespace std;

	class ReadWriteLock
	{
	
	public:
		
		ReadWriteLock();
		
		~ReadWriteLock() noexcept;
		
        ReadWriteLock(ReadWriteLock const&) = delete;
        ReadWriteLock& operator =(ReadWriteLock const&) = delete;
        
		void lockRead();
		
		void lockWrite();
		
		void unlock();
		
		class ReadLock
		{
			
		public:
			
			ReadLock(ReadWriteLock& lock);
			
			~ReadLock() noexcept;
			
            ReadLock(ReadLock const&) = delete;
            ReadLock& operator =(ReadLock const&) = delete;
		
		private:
			
			ReadWriteLock& _lock;
			
		};
		
		class WriteLock
		{
			
		public:
			
			WriteLock(ReadWriteLock& lock);
			
			~WriteLock() noexcept;
			
            WriteLock(WriteLock const&) = delete;
            WriteLock& operator =(WriteLock const&) = delete;
			
		private:

			ReadWriteLock& _lock;
				
		};

	private:
		
		void _checkRC(int result);
		
		pthread_rwlock_t _lock;
			
	};

}

#define SCOPED_READ_LOCK(rwLock) ::threading::ReadWriteLock::ReadLock ANONYMOUS_VAR(readLock)(rwLock)
#define SCOPED_WRITE_LOCK(rwLock) ::threading::ReadWriteLock::WriteLock ANONYMOUS_VAR(writeLock)(rwLock)

#endif
