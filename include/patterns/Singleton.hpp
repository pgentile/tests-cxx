#ifndef SINGLETON_H
#define SINGLETON_H

#include <exception>
#include <pthread.h>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <string>

#include "patterns/NonCopyable.hpp"

namespace patterns
{
	
	using namespace std;
	
	
	class UnexistingSingletonError: public runtime_error
	{
		
	public:
		
		inline UnexistingSingletonError(const string reason): runtime_error(_createMessage(reason))
		{
		}
	
	private:
		
		static string _createMessage(const string& reason)
		{
			ostringstream message;
			message << "Can't create singleton: " << reason;
			return message.str();
		}
		
	};
	
	
	extern void registerDestructor(void (*destructorFunc)(void));
	

	/**
	 * Thread-safe singleton
	 */
	template<class T>
	class Singleton: private NonCopyable
	{
	
		public:

			static T& instance(void)
			{
				pthread_once(&_onceControl, _createInstance);
				if (_instance == NULL) {
					throw UnexistingSingletonError(_error);
				}
				return *_instance;
			}
	
		private:
			
			static void _createInstance(void)
			{
				try {
					_instance = new T();
					registerDestructor(_deleteInstance);
				} catch (exception& e) {
					_error = e.what();
					
					delete _instance;
					_instance = NULL;
				}
			}
			
			static void _deleteInstance(void)
			{
				delete _instance;
			}
			
			static pthread_once_t _onceControl;
					
			static T* _instance;
			
			static string _error;
	
	};
	
	template<class T>
	pthread_once_t Singleton<T>::_onceControl = PTHREAD_ONCE_INIT;
	
	template<class T>
	T* Singleton<T>::_instance = NULL;
	
	template<class T>
	string Singleton<T>::_error;

}

#endif
