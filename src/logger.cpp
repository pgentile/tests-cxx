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
	
	// Class Level
	
	const Level Level::debug(0);
	const Level Level::info(500);
	const Level Level::warn(1000);
	const Level Level::error(1500);
	const Level Level::fatal(2000);
	
	// Class Consumer

	void Consumer::run()
	{
		cout << "Consuming logs..." << endl;
		
		bool running = true;
		
		MutexLock lock(_mutex);
		while (running) {
			// Waiting signal
			_publishedCond.wait();
			
			// Consume pending events
			while (!_pendingEvents.empty()) {
				auto_ptr<Event> event(_pendingEvents.front()); // Delete event on scope exit
				Event::Kind kind = event->kind();
				if (kind == Event::LOG_EVENT) {
					LogEvent* logEvent = static_cast<LogEvent*>(event.get());
					cout << "LOGGER EVENT: " << logEvent->message() << endl;
				} else if (kind == Event::SHUTDOWN) {
					running = false;
					cout << "Shutdown received" << endl;
				}

				_pendingEvents.pop();
			}
		}
	}
	
	Consumer::Consumer(queue<Event*>& pendingEvents, Mutex& mutex, Condition& publishedCond):
			_pendingEvents(pendingEvents),
			_mutex(mutex),
			_publishedCond(publishedCond)
	{
		
	}
	
	Consumer::~Consumer(void)
	{
	}
	
	// Class Event
	
	Event::Event(Kind kind) {
		_kind = kind;
	}
	
	// Class LogEvent
	
	LogEvent::LogEvent(const string& message) :
			Event(LOG_EVENT),
			_message(message)
	{
	}
	
	// Class ShutdownEvent
	
	ShutdownEvent::ShutdownEvent(void):
			Event(LOG_EVENT)
	{
	}
	
	// Class LoggerManager

	LoggerManager::LoggerManager(void):
			_publishedCond(_mutex),
	 		_consumer(_pendingEvents, _mutex, _publishedCond)
	{
		_consumer.start();
	}

	LoggerManager::~LoggerManager()
	{
		Event* event = new ShutdownEvent();
		_publishEvent(event);
		
		_consumer.join();
	}
	
	void LoggerManager::log(string& message)
	{
		Event* event = new LogEvent(message);
		_publishEvent(event);
	}
	
	void LoggerManager::log(const char* message)
	{
		string messageStr = message;
		log(messageStr);
	}
	
	void LoggerManager::_publishEvent(Event* event)
	{
		MutexLock lock(_mutex);
		_pendingEvents.push(event);
		_publishedCond.signal();
	}
	
}
