#ifndef MUTEX_H
#define MUTEX_H

extern "C" {
#include <pthread.h>
}

#include <iostream>
#include <stdexcept>

namespace threading {

	using namespace std;
	
	class MutexLock;
	class Condition;
	
	/**
	 * Mutex
	 *
	 * <p>Protecting a critical section:</p>
	 *
	 * <pre>
	 * Mutex mutex;
	 * {
	 *    	MutexLock lock(mutex);
	 *      // Critical section here
	 * }
	 * </pre>
	 *
	 * @see MutexLock
	 */
	class Mutex
	{
		
		public:
			Mutex(void);
		
			void lock(void);
		
			bool tryLock(void);
		
			void release(void);
		
			virtual ~Mutex(void);
		
		private:
			void _checkRC(int result);
		
			pthread_mutex_t _ptMutex;
		
			friend class MutexLock;
			friend class Condition;
		
	};
	
	/**
	 * Lock on a mutex
	 */
	class MutexLock
	{
		
		public:
			MutexLock(Mutex& mutex);
		
			virtual ~MutexLock();

		private:
			Mutex& _mutex;
		
	};
	
	class Condition
	{
	
		public:
			Condition(Mutex& mutex);
		
			void wait(void);
		
			void signal(void);
		
			void broadcast(void);
		
			virtual ~Condition();
		
			inline Mutex& mutex(void) {
				return _mutex;
			}
		
		private:
		
			Mutex& _mutex;
			pthread_cond_t _cond;
		
	};
	
}

#endif
