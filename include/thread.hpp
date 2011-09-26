#ifndef THREAD_H
#define THREAD_H

extern "C" {
#include <pthread.h>
}

namespace threading {
	
	class Thread
	{
	public:
		Thread(void);
		
		virtual ~Thread(void);
		
		virtual void run() = 0;
		
		void start(void);
		
		void cancel(void);
		
		void join(void);
	
	protected:
		
		void _checkCancelled(void);

	private:
		
		void _checkRC(int result);
		
		Thread(const Thread&);
		
		Thread& operator=(const Thread&);
		
		pthread_t _thread;
		
		pthread_attr_t _threadAttrs;
		
	};

}

#endif
