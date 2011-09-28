#include "logger.hpp"

extern "C"
{
#include <pthread.h>
}

#include <iostream>
#include <memory>

namespace logger
{

	using namespace std;
	using namespace logger;

	void LogEventConsumer::run()
	{
		cout << "Consuming logs..." << endl;
		
		MutexLock lock(_mutex);
		while (!_pendingEvents.empty()) {
			
			// Consume pending events
			while (!_pendingEvents.empty()) {
				auto_ptr<LogEvent> event(_pendingEvents.front()); // Delete on scope exit
				cout << "LOGGER EVENT: Thread " << event->threadId() << ": " << event->message() << endl;
				_pendingEvents.pop();
			}
			
			// Waiting signal
			_publishedCond.wait();
		}
	}
	
	LogEventConsumer::LogEventConsumer(queue<LogEvent*>& pendingEvents, Mutex& mutex, Condition& publishedCond):
			_pendingEvents(pendingEvents),
			_mutex(mutex),
			_publishedCond(publishedCond)
	{
		
	}
	
	LogEventConsumer::~LogEventConsumer(void)
	{
	}
	
	
	LogEvent::LogEvent(pthread_t threadId, const string& message) :
			_threadId(threadId), _message(message)
	{
	}
	
	LogEvent::~LogEvent()
	{
	}

	LoggerManager::LoggerManager(void):
			_publishedCond(_mutex),
	 		_consumer(_pendingEvents, _mutex, _publishedCond)
	{
		_consumer.start();
	}
	
	void LoggerManager::log(string& message)
	{
		pthread_t threadId = pthread_self();
		LogEvent* event = new LogEvent(threadId, message);
		_publishEvent(event);
	}
	
	void LoggerManager::log(const char* message)
	{
		pthread_t threadId = pthread_self();
		string messageStr = message;
		LogEvent* event = new LogEvent(threadId, messageStr);
		_publishEvent(event);
	}
	
	void LoggerManager::_publishEvent(LogEvent* event)
	{
		{
			MutexLock lock(_mutex);
			_pendingEvents.push(event);
			_publishedCond.signal();
		}
	}

	LoggerManager::~LoggerManager()
	{
	}
	
}
