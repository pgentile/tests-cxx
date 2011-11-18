#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>

#include <iostream>
#include <stdexcept>

#include "patterns/NonCopyable.hpp"

namespace threading {
	
	using namespace patterns;
	using namespace std;

	class ReadWriteLock: private NonCopyable
	{
	
	public:
		
		ReadWriteLock(void);
		
		virtual ~ReadWriteLock();
		
		void lockRead(void);
		
		void lockWrite(void);
		
		void unlock(void);

	private:
		
		pthread_rwlock_t _lock;
			
	};

}

#endif
