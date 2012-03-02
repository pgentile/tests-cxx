#ifndef UTIL_UNDEFINED_ERROR_HPP
#define UTIL_UNDEFINED_ERROR_HPP

#include "core/Exception.hpp"

namespace util
{

	class UndefinedError: public core::Exception
	{
		
	public:
		
		UndefinedError();
		
		virtual ~UndefinedError() throw();

	};

}

#endif
