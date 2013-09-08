#ifndef CORE_EXCEPTION_HPP
#define CORE_EXCEPTION_HPP

#include "core/Backtrace.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <thread>


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
		
		const char* what() const throw() {
    		return _what.c_str();
		}
		
		const Backtrace& backtrace() const {
            return _backtrace;
		}
		
		const std::thread::id& thread() const {
            return _thread;
		}
		
	private:
		
		string _what;
		
		Backtrace _backtrace;
		
		std::thread::id _thread;

	};
	
	ostream& operator <<(ostream& out, const Exception& exception);

}

#endif
