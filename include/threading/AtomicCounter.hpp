#ifndef ATOMIC_COUNTER_H
#define ATOMIC_COUNTER_H

#include "mutex.hpp"

namespace threading
{
	
	template <typename T>
	class AtomicCounter
	{
		public:

			T increment(void)
			{
				MutexLock lock(_mutex);
				_value++;
				return _value;
			}
			
			void set(T value)
			{
				MutexLock lock(_mutex);
				_value = value;
			}
			
			T value(void)
			{
				MutexLock lock(_mutex);
				return _value;
			}

		private:
			T _value;
			Mutex _mutex;
	};
	
}

#endif
