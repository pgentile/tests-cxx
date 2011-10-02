#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>

#include "thread.hpp"
#include "mutex.hpp"
#include "non-copyable.hpp"
#include "comparable.hpp"

namespace logger
{
	
	using namespace std;
	using namespace patterns;
	using namespace threading;
	
	class Level: public Comparable<Level>, private NonCopyable
	{
		friend ostream& operator<<(ostream& out, const Level& level);
		
		public:
			Level(const string& name, unsigned int value);
			
			virtual bool operator<(const Level& other) const;
			
			bool operator==(const Level& other) const;
			
			inline const string& name(void) const
			{
				return _name;
			}
		
			inline unsigned int value(void) const
			{
				return _value;
			}
			
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
	
	ostream& operator<<(ostream& out, const Level& level);
	
	class Event: public Comparable<Event>, private NonCopyable
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
			
			virtual bool operator<(const Event& other) const;
			
			virtual bool operator==(const Event& other) const;
			
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
			
			virtual bool operator<(const Event& other) const;
			
			virtual bool operator==(const Event& other) const;

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
			Condition _publishedCond;
		
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
			virtual ~LoggerManager();

			void log(const Level& level, const string& message);
			
			static const unsigned int defaultQueueSize = 10000;

		private:
			const Level& _threshold;
			EventQueue _queue;
			Consumer _consumer;
			
	};
	
}

#endif
