#ifndef CORE_EXCEPTION_HPP
#define CORE_EXCEPTION_HPP

#include "core/Backtrace.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <pthread.h>

namespace core {
	
	using namespace std;
	
	class Exception: public exception
	{
		
	public:
		
		explicit Exception(const string& what);
		
		explicit Exception(const exception& ex);

		Exception(const Exception& src);
		
		virtual ~Exception() throw();
		
		Exception& operator =(const Exception& src);
		
		const string thrownClassName() const;
		
		const char* what() const throw();
		
		const Backtrace& backtrace() const;
		
		const pthread_t& thread() const;
		
	private:
		
		string _what;
		
		Backtrace _backtrace;
		
		pthread_t _thread;

	};
	
	ostream& operator <<(ostream& out, const Exception& exception);
	
	inline const char* Exception::what() const throw() {
		return _what.c_str();
	}
	
	inline const Backtrace& Exception::backtrace() const {
		return _backtrace;
	}
	
	inline const pthread_t& Exception::thread() const {
		return _thread;
	}

}

#endif
