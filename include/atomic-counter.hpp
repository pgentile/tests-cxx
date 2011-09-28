#ifndef ATOMIC_COUNTER_H
#define ATOMIC_COUNTER_H

#include "mutex.hpp"

namespace threading {
	
	template <typename T>
	class AtomicCounter
	{
	public:

		T incrementAndGet(void);

	private:
		T _counter;
		Mutex _counterMutex;
	};
	
	template<typename T>
	T AtomicCounter<T>::incrementAndGet(void) {
		MutexLock lock(_counterMutex);
		return ++_counter;
	}
	
}

#endif
