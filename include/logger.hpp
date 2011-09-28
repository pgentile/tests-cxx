#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <queue>

#include "thread.hpp"
#include "mutex.hpp"

namespace logger
{
	
	using namespace threading;

	class LogEvent
	{
		public:
			LogEvent(pthread_t threadId, const string& message);
			virtual ~LogEvent();
		
			inline const pthread_t threadId(void) {
				return _threadId;
			}
		
			inline const string& message(void) {
				return _message;
			}

		private:	
			pthread_t _threadId;
			string _message;
	};

	class LogEventConsumer: public Thread
	{
		public:
			LogEventConsumer(queue<LogEvent*>& pendingEvents, Mutex& mutex, Condition& publishedCond);
		
			virtual void run();
		
			virtual ~LogEventConsumer();

		private:
			queue<LogEvent*>& _pendingEvents;
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
			void _publishEvent(LogEvent* event);

		private:
			queue<LogEvent*> _pendingEvents;
			Mutex _mutex;
			Condition _publishedCond;

			LogEventConsumer _consumer;
			
	};
	
}

#endif
