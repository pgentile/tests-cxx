#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <queue>

#include "thread.hpp"
#include "mutex.hpp"
#include "non-copyable.hpp"
#include "comparable.hpp"

namespace logger
{
	
	using namespace patterns;
	using namespace threading;
	
	class Level: public Comparable<Level>, private NonCopyable
	{
		public:
			Level(const string& name, unsigned int value);
		
			inline unsigned int value(void) const
			{
				return _value;
			}

			virtual bool operator<(const Level& other) const;
			
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
	
	class Event: private NonCopyable
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
			LogEvent(const Level& level, const string& message);
			
			inline const Level& level(void) const {
				return _level;
			}
		
			inline const string& message(void) const {
				return _message;
			}

		private:
			const Level& _level;
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
		
		private:
			queue<Event*>& _pendingEvents;
			Mutex& _mutex;
			Condition& _publishedCond;
	};

	class LoggerManager: private NonCopyable
	{
		public:
			LoggerManager(const Level& threshold = Level::all);
			virtual ~LoggerManager();

			void log(const Level& level, const string& message);

		protected:
			void _publishEvent(Event* event);

		private:
			const Level& _threshold;
			queue<Event*> _pendingEvents;
			Mutex _mutex;
			Condition _publishedCond;

			Consumer _consumer;
			
	};
	
}

#endif
