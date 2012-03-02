#include "logger/Logger.hpp"

#include <pthread.h>

#include <limits>
#include <memory>
#include <iostream>

namespace logger
{

	using namespace std;
	using namespace std::rel_ops;
	using namespace logger;
	
	// Class Level
	
	Level::Level(const string& name, unsigned int value):
			_name(name), _value(value)
	{
	}
	
	bool Level::operator ==(const Level& other) const
	{
		return _value == other._value;
	}

	bool Level::operator <(const Level& other) const
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
		//cout << "Demarrage consommateur" << endl;
		
		bool running = true;
		
		while (running) {
			_extractedEvents.clear();
			_queue.extract(_extractedEvents);
			
			for (vector<Event*>::iterator it = _extractedEvents.begin(); it != _extractedEvents.end(); ++it) {
				auto_ptr<Event> event(*it); // Supprimer l'evenement en sortie de scope
				Event::Kind kind = event->kind();
				if (kind == Event::LOG_EVENT) {
					LogEvent* logEvent = static_cast<LogEvent*>(event.get());
					cout << "LOGGER EVENT: (" << logEvent->level() << ") " << logEvent->message() << endl;
				} else if (kind == Event::SHUTDOWN) {
					running = false;
				}
			}
		}
		
		//cout << "Arret consommateur" << endl;
	}
	
	Consumer::Consumer(EventQueue& queue): _queue(queue)
	{
	}
	
	// Class Event
	
	Event::Event(Kind kind): _kind(kind)
	{
	}
	
	ostream& operator<<(ostream& out, const Event& event)
	{
		out << "Event[kind = " << event.kind() << "]";
		return out;
	}
	
	// Class EventQueue
	
	EventQueue::EventQueue(unsigned int size):
			_size(size), _count(0),
			_events(new Event*[size]),
			_publishedCond(_mutex)
	{
	}
	
	bool EventQueue::publish(Event* event)
	{
		bool published = false;
		Mutex::Lock lock(_mutex);
		
		//cout << "Before publish: count=" << _count << ", size=" << _size << endl;
		
		// Ajouter le nouvel element, si possible
		if (_count < _size) {
			//cout << "Published event at position " << _count << endl;
			_events[_count] = event;
			_count++;
			published = true;
		} else {	
			cerr << "Event queue full, dropping new event " << *event << endl;
		}
		
		_publishedCond.signal();
		
		//cout << "After publish: count=" << _count << ", size=" << _size << endl << endl;
		
		return published;
	}
	
	void EventQueue::extract(vector<Event*>& output)
	{
		Mutex::Lock lock(_mutex);
		
		if (_count > 0) {
			// Copier les pointeurs dans output
			for (unsigned int i = 0; i < _count; i++) {
				output.push_back(_events[i]);
			}
			_count = 0;
		} else {
			// Attendre l'arrivee d'un evenement
			_publishedCond.wait();
		}
	}
	
	EventQueue::~EventQueue()
	{
		// Supprimer les evenements restants
		for (unsigned int i = 0; i < _count; i++) {
			cout << "Dropping event at position " << i << endl;
			delete _events[i];
		}
		
		delete[] _events;
	}
	
	// Class LogEvent
	
	LogEvent::LogEvent(const Level& level, const string& message):
			Event(LOG_EVENT), _level(level), _message(message)
	{
	}
	
	// Class ShutdownEvent
	
	ShutdownEvent::ShutdownEvent(): Event(SHUTDOWN)
	{
	}
	
	// Class LoggerManager

	LoggerManager::LoggerManager(const Level& threshold, unsigned int queueSize):
			_threshold(threshold),
			_queue(queueSize),
	 		_consumer(_queue)
	{
		_consumer.start();
	}

	LoggerManager::~LoggerManager()
	{
		Event* event = new ShutdownEvent();
		_queue.publish(event);
		
		_consumer.join();
	}
	
	void LoggerManager::log(const Level& level, const string& message)
	{
		if (level >= _threshold) {
			Event* event = new LogEvent(level, message);
			_queue.publish(event);
		}
	}

}
