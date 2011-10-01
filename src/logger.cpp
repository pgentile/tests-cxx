#include "logger.hpp"

extern "C"
{
#include <pthread.h>
}

#include <iostream>
#include <memory>
#include <limits>

namespace logger
{

	using namespace std;
	using namespace logger;
	
	// Class Level
	
	Level::Level(const string& name, unsigned int value):
			_name(name), _value(value)
	{
	}
	
	bool Level::operator<(const Level& other) const
	{
		return _value < other._value;
	}
	
	ostream& operator<<(ostream& out, const Level& level)
	{
		out << level._name;
		return out;
	}
	
	const Level Level::all("ALL", numeric_limits<unsigned int>::min());
	const Level Level::debug("DEBUG", 500);
	const Level Level::info("INFO", 1000);
	const Level Level::warn("WARN", 1500);
	const Level Level::error("ERROR", 2000);
	const Level Level::fatal("FATAL", 2500);
	const Level Level::none("NONE", numeric_limits<unsigned int>::max());
	
	// Class Consumer

	void Consumer::run()
	{		
		bool running = true;
		
		MutexLock lock(_mutex);
		while (running) {
			// Waiting signal
			_publishedCond.wait();
			
			// Consume pending events
			while (!_pendingEvents.empty()) {
				auto_ptr<Event> event(_pendingEvents.front()); // Delete event on scope exit
				_pendingEvents.pop();
				
				Event::Kind kind = event->kind();
				if (kind == Event::LOG_EVENT) {
					LogEvent* logEvent = static_cast<LogEvent*>(event.get());
					cout << "LOGGER EVENT: (" << logEvent->level() << ") " << logEvent->message() << endl;
				} else if (kind == Event::SHUTDOWN) {
					running = false;
				}
			}
		}
	}
	
	Consumer::Consumer(queue<Event*>& pendingEvents, Mutex& mutex, Condition& publishedCond):
			_pendingEvents(pendingEvents),
			_mutex(mutex),
			_publishedCond(publishedCond)
	{	
	}
	
	// Class Event
	
	Event::Event(Kind kind):
			_kind(kind)
	{
	}
	
	// Class LogEvent
	
	LogEvent::LogEvent(const Level& level, const string& message) :
			Event(LOG_EVENT), _level(level), _message(message)
	{
	}
	
	// Class ShutdownEvent
	
	ShutdownEvent::ShutdownEvent(void): Event(SHUTDOWN)
	{
	}
	
	// Class LoggerManager

	LoggerManager::LoggerManager(const Level& threshold):
			_threshold(threshold),
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
	
	void LoggerManager::log(const Level& level, const string& message)
	{
		if (level >= _threshold) {
			Event* event = new LogEvent(level, message);
			_publishEvent(event);
		}
	}
	
	void LoggerManager::_publishEvent(Event* event)
	{
		MutexLock lock(_mutex);
		_pendingEvents.push(event);
		_publishedCond.signal();
	}
	
}
