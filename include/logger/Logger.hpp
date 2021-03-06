#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>


namespace logger
{
	
	using namespace std;
	
	class Level final
	{
		friend ostream& operator<<(ostream& out, const Level& level);
		
		public:
			Level(const string& name, unsigned int value);
			
            Level(Level const&) = delete;
            Level& operator =(Level const&) = delete;
			
			bool operator <(const Level& other) const;
			
			bool operator ==(const Level& other) const;
			
			const string& name() const
        	{
        		return _name;
        	}
		
			unsigned int value() const
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
	
	class Event
	{
		
		public:
			
			enum Kind {
				LOG_EVENT,
				SHUTDOWN	
			};
			
			Event(Kind kind);
			
            Event(Event const&) = delete;
            Event& operator =(Event const&) = delete;
						
			Kind kind() const {
        		return _kind;
        	}
		
		private:
			Kind _kind;
		
	};
	
	ostream& operator<<(ostream& out, const Event& event);
	
	
	class LogEvent: public Event
	{
		public:
			LogEvent(const Level& level, const string& message, const thread::id& threadId);
			
			const Level& level() const {
                return _level;
			}
		
			const string& message() const {
                return _message;
			}
			
            const thread::id threadId() const {
                return _threadId;
            }

		private:
			const Level& _level;
			string _message;
            thread::id _threadId;
	};
	
	class ShutdownEvent: public Event
	{
		public:
			ShutdownEvent();

	};
	
	class EventQueue final
	{
		
		public:
			EventQueue(unsigned int size);
			~EventQueue();
			
            EventQueue(EventQueue const&) = delete;
            EventQueue& operator =(EventQueue const&) = delete;
			
			bool publish(Event* event);
			
			void extract(vector<Event*>& output);
		
		private:
			unsigned int _size;
			unsigned int _count;
			Event** _events;
			mutex _mutex;
			condition_variable _publishedCond;
		
	};

	class Consumer final
	{
		public:
			Consumer(EventQueue& queue);
					
			virtual void run();
		
		private:
			EventQueue& _queue;
			vector<Event*> _extractedEvents;
	};

	class LoggerManager final
	{
		public:
			LoggerManager(const Level& threshold = Level::all, unsigned int queueSize = defaultQueueSize);
			~LoggerManager();
			
			
            LoggerManager(LoggerManager const&) = delete;
            LoggerManager& operator =(LoggerManager const&) = delete;

			void log(const Level& level, const string& message);
			
			static const unsigned int defaultQueueSize = 10000;

		private:
			const Level& _threshold;
			EventQueue _queue;
            Consumer _consumer;
			thread _consumerThread;
			
	};
	
}

#endif
