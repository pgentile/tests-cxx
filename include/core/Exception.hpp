#ifndef CORE_EXCEPTION_HPP
#define CORE_EXCEPTION_HPP

#include "core/Backtrace.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace core {
	
	using namespace std;
	
	class Exception: public exception
	{
		
	public:
		
		Exception(const string& what);
		
		Exception(const exception& ex);

		Exception(const Exception& src);
		
		virtual ~Exception() throw();
		
		const string thrownClassName() const;
		
		const char* what() const throw();
		
		const Backtrace& backtrace() const;
		
	private:
		
		string _what;
		
		const Backtrace _backtrace;

	};
	
	ostream& operator <<(ostream& out, const Exception& exception);
	
	inline const char* Exception::what() const throw() {
		return _what.c_str();
	}
	
	inline const Backtrace& Exception::backtrace() const {
		return _backtrace;
	}

}

#endif
