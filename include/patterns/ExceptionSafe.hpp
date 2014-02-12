#ifndef PATTERNS_EXCEPTION_SAFE_H
#define PATTERNS_EXCEPTION_SAFE_H

#include <exception>
#include <iostream>


#define EXCEPTION_SAFE_BEGIN() \
	try \
	{

#define EXCEPTION_SAFE_END() \
	} \
	catch (const std::exception& _caughtException) { \
	    try { \
		    std::cerr << __func__ << ": Caught exception" << std::endl; \
		    std::cerr << __func__ << ": Reason: " << _caughtException.what() << std::endl; \
		} \
		catch (...) {} \
	} \
	catch (...) { \
	    try { \
		    std::cerr << __func__ << ": Caught unknown exception" << std::endl; \
		} \
		catch (...) {} \
	}

#endif
