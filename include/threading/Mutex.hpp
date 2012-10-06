#ifndef THREADING_MUTEX_H
#define THREADING_MUTEX_H

#include <pthread.h>

#include <iostream>
#include <stdexcept>

#include "macros.h"
#include "patterns/NonCopyable.hpp"

namespace threading {
	
	using namespace patterns;
	using namespace std;
	
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
	class Mutex: private NonCopyable
	{
		
		public:
			
			Mutex();
		
			void lock();
		
			bool tryLock();
		
			void release();
		
			virtual ~Mutex();
			
			/**
			 * Lock on a mutex
			 */
			class Lock: private NonCopyable
			{

				public:

					Lock(Mutex& mutex);

					virtual ~Lock();

				private:
					Mutex& _mutex;

			};
			
			class Condition: private NonCopyable
			{

				public:

					Condition(Mutex& mutex);

					virtual ~Condition();

					void wait();

					void signal();

					void broadcast();

					inline Mutex& mutex() {
						return _mutex;
					}

				private:

					Mutex& _mutex;
					pthread_cond_t _cond;

			};
		
		private:
			
			void _checkRC(int result);
		
			pthread_mutex_t _mutex;
		
	};
	
}

#define SCOPED_MUTEX_LOCK(mutex) ::threading::Mutex::Lock ANONYMOUS_VAR(lock)(mutex)

#endif
