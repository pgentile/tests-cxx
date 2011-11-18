#ifndef THREAD_H
#define THREAD_H

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
			Thread(void);
		
			virtual ~Thread(void);
		
			virtual void run() = 0;
		
			void start(void);
		
			void cancel(void);
		
			void join(void);
		
			inline unsigned long id(void) const {
				return _id;
			}
	
		protected:
		
			void _checkCancelled(void);

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
					inline InstanceCounter()
					{
					}

					inline virtual ~InstanceCounter()
					{
					}

			};
		
	};

}

#endif
