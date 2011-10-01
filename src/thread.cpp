#include "thread.hpp"
#include "singleton.hpp"
#include "atomic-counter.hpp"

#include <sstream>
#include <stdexcept>

namespace threading
{
	
	using namespace std;
	using namespace patterns;
	
	class ThreadInstanceCounter: public Singleton<ThreadInstanceCounter>, public AtomicCounter<unsigned long>
	{

		friend class Singleton<ThreadInstanceCounter>;
		
		private:
			inline ThreadInstanceCounter()
			{
			}
		
			inline virtual ~ThreadInstanceCounter()
			{
			}

	};
	
	extern "C"
	{
		
		static void* _runThread(void* arg)
		{
			Thread* thread = (Thread*) arg;
			thread->run();
			pthread_exit(NULL);
			return NULL;
		}
		
	}
	
	Thread::Thread(void):
			_id(ThreadInstanceCounter::instance().increment())
	{
		int result;
		
		result = pthread_attr_init(&_threadAttrs);
		_checkRC(result);
		
		result = pthread_attr_setdetachstate(&_threadAttrs, PTHREAD_CREATE_JOINABLE);
		_checkRC(result);
	}

	void Thread::start(void)
	{
		void* threadArg = (void*) this;
		int result = pthread_create(&_thread, &_threadAttrs, _runThread, threadArg);
		_checkRC(result);
	}
	
	void Thread::join(void)
	{
		int result = pthread_join(_thread, NULL);
		_checkRC(result);
	}
	
	void Thread::cancel(void)
	{
		int result = pthread_cancel(_thread);
		_checkRC(result);
	}
	
	Thread::~Thread(void)
	{
		int result = pthread_attr_destroy(&_threadAttrs);
		_checkRC(result);
	}
	
	void Thread::_checkCancelled(void)
	{
		pthread_testcancel();
	}

	void Thread::_checkRC(int result)
	{
		if (result != 0) {
			ostringstream message;
			message << "Error on thread " << _id << ": code " << result;
			throw runtime_error(message.str());
		}
	}
	
}
