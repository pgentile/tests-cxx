#ifndef THREAD_H
#define THREAD_H

extern "C" {
#include <pthread.h>
}

#include "non-copyable.hpp"

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
		
			void _checkRC(int result);

			const unsigned long _id;
		
			pthread_t _thread;
		
			pthread_attr_t _threadAttrs;
		
	};

}

#endif
