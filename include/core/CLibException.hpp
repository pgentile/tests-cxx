#ifndef CORE_CLIB_EXCEPTION_HPP
#define CORE_CLIB_EXCEPTION_HPP

#include "core/Exception.hpp"

namespace core {
	
	class CLibException: public Exception
	{
	
	public:
		
		static void throwOnError(int errnum);
		
		CLibException(int errnum);
		
		CLibException(const CLibException& src);
		
		virtual ~CLibException() throw();
		
		int errnum() const;

	private:
		
		static string _createMessage(int errnum);
		
		int _errnum;

	};
	
	inline int CLibException::errnum() const {
		return _errnum;
	}
	
}

#endif
