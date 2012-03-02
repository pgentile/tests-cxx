#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "patterns/NonCopyable.hpp"
#include "threading/Mutex.hpp"
#include "threading/Thread.hpp"


namespace logger
{
	
	using namespace std;
	using namespace patterns;
	using namespace threading;
	
	class Level: private NonCopyable
	{
		friend ostream& operator<<(ostream& out, const Level& level);
		
		public:
			Level(const string& name, unsigned int value);
			
			bool operator <(const Level& other) const;
			
			bool operator ==(const Level& other) const;
			
			const string& name() const;
		
			unsigned int value() const;
			
			static const Level all;
			static const Level debug;
			static const Level info;
			static const Level warn;
			static const Level error;
			static const Level fatal;
			static const Level none;

		private:
			const string _name;
			unsigned int _value;
		
	};
	
	inline const string& Level::name() const
	{
		return _name;
	}

	inline unsigned int Level::value() const
	{
		return _value;
	}
	
	ostream& operator<<(ostream& out, const Level& level);
	
	class Event: private NonCopyable
	{
		
		public:
			
			enum Kind {
				LOG_EVENT,
				SHUTDOWN	
			};
			
			Event(Kind kind);
						
			Kind kind() const;
		
		private:
			Kind _kind;
		
	};
	
	inline Event::Kind Event::kind() const {
		return _kind;
	}
	
	ostream& operator<<(ostream& out, const Event& event);
	
	class LogEvent: public Event
	{
		public:
			LogEvent(const Level& level, const string& message);
			
			const Level& level() const;
		
			const string& message() const;

		private:
			const Level& _level;
			string _message;
	};
	
	inline const Level& LogEvent::level() const {
		return _level;
	}

	inline const string& LogEvent::message() const {
		return _message;
	}
	
	class ShutdownEvent: public Event
	{
		public:
			ShutdownEvent();

	};
	
	class EventQueue: private NonCopyable
	{
		
		public:
			EventQueue(unsigned int size);
			~EventQueue();
			
			bool publish(Event* event);
			
			void extract(vector<Event*>& output);
		
		private:
			unsigned int _size;
			unsigned int _count;
			Event** _events;
			Mutex _mutex;
			Mutex::Condition _publishedCond;
		
	};

	class Consumer: public Thread
	{
		public:
			Consumer(EventQueue& queue);
					
			virtual void run();
		
		private:
			EventQueue& _queue;
			vector<Event*> _extractedEvents;
	};

	class LoggerManager: private NonCopyable
	{
		public:
			LoggerManager(const Level& threshold = Level::all, unsigned int queueSize = defaultQueueSize);
			~LoggerManager();

			void log(const Level& level, const string& message);
			
			static const unsigned int defaultQueueSize = 10000;

		private:
			const Level& _threshold;
			EventQueue _queue;
			Consumer _consumer;
			
	};
	
}

#endif
