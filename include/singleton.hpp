#ifndef SINGLETON_H
#define SINGLETON_H

namespace patterns
{

	template<typename T>
	class Singleton
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
