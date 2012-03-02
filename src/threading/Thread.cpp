#include "threading/Thread.hpp"

#include <sstream>
#include <stdexcept>

#include "patterns/ExceptionSafe.hpp"

namespace threading
{
	
	using namespace std;
	using namespace patterns;

	Thread::Thread(): _id(InstanceCounter::instance().increment())
	{
		int result;
		
		result = pthread_attr_init(&_threadAttrs);
		_checkRC(result);
		
		result = pthread_attr_setdetachstate(&_threadAttrs, PTHREAD_CREATE_JOINABLE);
		_checkRC(result);
	}

	Thread::~Thread()
	{
		EXCEPTION_SAFE_BEGIN();
		int result = pthread_attr_destroy(&_threadAttrs);
		_checkRC(result);
		EXCEPTION_SAFE_END();
	}

	void Thread::start()
	{
		void* threadArg = (void*) this;
		int result = pthread_create(&_thread, &_threadAttrs, _runThread, threadArg);
		_checkRC(result);
	}
	
	void Thread::join()
	{
		int result = pthread_join(_thread, NULL);
		_checkRC(result);
	}
	
	void Thread::cancel()
	{
		int result = pthread_cancel(_thread);
		_checkRC(result);
	}
	
	void Thread::checkCancelled()
	{
		pthread_testcancel();
	}

	void* Thread::_runThread(void* arg)
	{
		Thread* thread = (Thread*) arg;
		thread->run();
		pthread_exit(NULL);
		return NULL;
	}

	void Thread::_checkRC(int result)
	{
		if (result != 0) {
			ostringstream message;
			message << "Error on thread " << _id << ": code " << result;
			throw runtime_error(message.str());
		}
	}
	
	ostream& operator <<(ostream& out, const Thread& thread)
	{
		return out << "Thread #" << thread._id;
	}
	
}
