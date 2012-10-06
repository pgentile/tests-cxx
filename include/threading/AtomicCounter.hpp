#ifndef THREADING_ATOMIC_COUNTER_H
#define THREADING_ATOMIC_COUNTER_H

#include "threading/Mutex.hpp"

namespace threading
{
	
	template <typename T>
	class AtomicCounter
	{
	
	public:
		
		AtomicCounter(): _value(0) {
		}
		
		AtomicCounter(const T value): _value(value) {
		}
		
		T add(const T added)
		{
			SCOPED_MUTEX_LOCK(_mutex);
			_value += added;
			return _value;
		}

		T substract(const T substracted)
		{
			SCOPED_MUTEX_LOCK(_mutex);
			_value -= substracted;
			return _value;
		}
		
		T increment()
		{
			SCOPED_MUTEX_LOCK(_mutex);
			_value++;
			return _value;
		}
		
		T decrement()
		{
			SCOPED_MUTEX_LOCK(_mutex);
			_value--;
			return _value;
		}
		
		T get() {
			SCOPED_MUTEX_LOCK(_mutex);
			return _value;
		}
		
		void set(const T value)
		{
			SCOPED_MUTEX_LOCK(_mutex);
			_value = value;
		}
		
	private:
		
		T _value;
		
		mutable Mutex _mutex;
		
	};
	
}

#endif
