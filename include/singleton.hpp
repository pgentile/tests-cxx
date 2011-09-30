#ifndef SINGLETON_H
#define SINGLETON_H

#include "non-copyable.hpp"

namespace patterns
{

	template<class T>
	class Singleton: private NonCopyable
	{
	
		public:

			static T& instance(void) {
				return _instance;
			}
	
		private:
		
			static T _instance;	
	
	};
	
	template<typename T>
	T Singleton<T>::_instance;

}

#endif
