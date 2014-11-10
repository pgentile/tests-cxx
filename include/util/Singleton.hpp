#ifndef UTIL_SINGLETON_HPP
#define UTIL_SINGLETON_HPP

#include <exception>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>


namespace util
{	

	void registerDestructor(void (*destructorFunc)(void));
	

	/**
	 * Thread-safe singleton
	 */
	template<class T>
	class Singleton
	{
			
		public:

			static T& instance()
			{
                std::call_once(_onceFlag, _createInstance);
                if (_exception) {
                    std::rethrow_exception(_exception);
                }
				return *_instance;
			}
			
            Singleton(Singleton const&) = delete;
            Singleton& operator =(Singleton const&) = delete;
		
		protected:
			
			Singleton()
			{
			}
			
			virtual ~Singleton()
			{
			}
	
		private:
			
			static void _createInstance()
			{
				try {
					_instance = new T();
					registerDestructor(_deleteInstance);
				}
				catch (...) {
                    _exception = std::current_exception();
					_deleteInstance();
				}
			}
			
			static void _deleteInstance()
			{
				T* target = _instance.exchange(nullptr);
				delete target;
			}
			
			static std::once_flag _onceFlag;
					
			static std::atomic<T*> _instance;
			
			static std::exception_ptr _exception;
	
	};
	
	template<class T>
    std::once_flag Singleton<T>::_onceFlag;
	
	template<class T>
    std::atomic<T*> Singleton<T>::_instance(nullptr);
	
	template<class T>
	std::exception_ptr Singleton<T>::_exception;

}

#endif
