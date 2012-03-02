#ifndef THREADING_ATOMIC_COUNTER_H
#define THREADING_ATOMIC_COUNTER_H

#include "threading/Mutex.hpp"

namespace threading
{
	
	template <typename T>
	class AtomicCounter
	{
	
	public:
		
		AtomicCounter()
		{
		}
		
		AtomicCounter(const T value)
		{
			set(value);
		}
		
		T add(const T added)
		{
			Mutex::Lock lock(_mutex);
			_value += added;
			return _value;
		}

		T substract(const T substracted)
		{
			Mutex::Lock lock(_mutex);
			_value -= substracted;
			return _value;
		}
		
		T increment()
		{
			Mutex::Lock lock(_mutex);
			_value++;
			return _value;
		}
		
		T decrement()
		{
			Mutex::Lock lock(_mutex);
			_value--;
			return _value;
		}
		
		void set(const T value)
		{
			Mutex::Lock lock(_mutex);
			_value = value;
			return _value;
		}
		
	private:
		
		T _value;
		
		Mutex _mutex;
		
	};
	
}

#endif
