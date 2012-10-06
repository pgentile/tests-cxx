#ifndef THREADING_THREAD_H
#define THREADING_THREAD_H

#include <pthread.h>

#include "threading/AtomicCounter.hpp"
#include "patterns/NonCopyable.hpp"
#include "patterns/Singleton.hpp"

namespace threading
{
	
	using namespace patterns;
	
	class Thread: private NonCopyable
	{
		public:
			Thread();
					
			virtual ~Thread();
		
			void start();
		
			void cancel();
		
			void join();
		
			unsigned long id() const;
			
			friend ostream& operator <<(ostream& out, const Thread& thread);
	
		protected:	
		
			virtual void run() = 0;
		
			void checkCancelled();

		private:
			
			static void* _runThread(void* arg);
		
			void _checkRC(int result);

			const unsigned long _id;
		
			pthread_t _thread;
		
			pthread_attr_t _threadAttrs;
			
			class InstanceCounter: public Singleton<InstanceCounter>, public AtomicCounter<unsigned long>
			{

				friend class Singleton<InstanceCounter>;

				private:
					
					InstanceCounter();

					virtual ~InstanceCounter();

			};
		
	};
	
	inline unsigned long Thread::id() const {
		return _id;
	}
	
	inline Thread::InstanceCounter::InstanceCounter()
	{
	}

	inline Thread::InstanceCounter::~InstanceCounter()
	{
	}

}

#endif
