#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <queue>

#include "thread.hpp"
#include "mutex.hpp"

namespace logger
{
	
	using namespace threading;
	
	class Level
	{
		public:
			inline Level(unsigned int value)
			{
				_value = value;
			}
		
			inline unsigned int value(void) const {
				return _value;
			}
			
			static const Level debug;
			static const Level info;
			static const Level warn;
			static const Level error;
			static const Level fatal;

		private:
			unsigned int _value;
		
	};
	
	class Event
	{
		
		public:
			
			enum Kind {
				LOG_EVENT,
				SHUTDOWN	
			};
			
			Event(Kind kind);
			
			inline Kind kind(void) const {
				return _kind;
			}
		
		private:
			Kind _kind;
		
	};
	
	class LogEvent: public Event
	{
		public:
			LogEvent(const string& message);
		
			inline const string& message(void) const {
				return _message;
			}

		private:	
			string _message;
	};
	
	class ShutdownEvent: public Event
	{
		public:
			ShutdownEvent(void);

	};

	class Consumer: public Thread
	{
		public:
			Consumer(queue<Event*>& pendingEvents, Mutex& mutex, Condition& publishedCond);
		
			virtual void run();
		
			virtual ~Consumer();

		private:
			queue<Event*>& _pendingEvents;
			Mutex& _mutex;
			Condition& _publishedCond;
	};

	class LoggerManager
	{
		public:
			LoggerManager(void);
			virtual ~LoggerManager();

			void log(string& message);
			void log(const char* message);

		protected:
			void _publishEvent(Event* event);

		private:
			queue<Event*> _pendingEvents;
			Mutex _mutex;
			Condition _publishedCond;

			Consumer _consumer;
			
	};
	
}

#endif
