#ifndef PATTERNS_SINGLETON_H
#define PATTERNS_SINGLETON_H

#include <exception>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>


namespace patterns
{	
	
	class UnexistingSingletonError: public std::runtime_error
	{
		
	public:
		
		inline UnexistingSingletonError(const std::string& reason): runtime_error(_createMessage(reason))
		{
		}
	
	private:
		
		static std::string _createMessage(const std::string& reason)
		{
			return "Can't create singleton: " + reason;
		}
		
	};
	
	
	extern void registerDestructor(void (*destructorFunc)(void));
	

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
                T* instancePt = _instance;
				if (instancePt == nullptr) {
					throw UnexistingSingletonError(_error);
				}
				return *instancePt;
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
				} catch (const std::exception& e) {
					_error = e.what();
					delete _instance;
					_instance = NULL;
				} catch (...) {
                    _error = "Can't create singleton";
					delete _instance;
					_instance = NULL;
				}
			}
			
			static void _deleteInstance()
			{
				delete _instance;
			}
			
			static std::once_flag _onceFlag;
					
			static std::atomic<T*> _instance;
			
			static std::string _error;
	
	};
	
	template<class T>
    std::once_flag Singleton<T>::_onceFlag;
	
	template<class T>
    std::atomic<T*> Singleton<T>::_instance(nullptr);
	
	template<class T>
	std::string Singleton<T>::_error;

}

#endif
