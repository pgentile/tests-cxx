#ifndef UTIL_EXCEPTION_SAFE_HPP
#define UTIL_EXCEPTION_SAFE_HPP

#include <exception>
#include <iostream>
#include <string>

#include "core/Backtrace.hpp"


#define EXCEPTION_SAFE_BACKTRACE() \
    std::cerr << std::string(80, '=') << std::endl; \
    core::Backtrace _backtrace; \
    std::cerr << _backtrace; \
    std::cerr << std::string(80, '=') << std::endl;

#define EXCEPTION_SAFE_BEGIN() \
	try \
	{

#define EXCEPTION_SAFE_END() \
	} \
	catch (const std::exception& _caughtException) { \
	    try { \
		    std::cerr << __func__ << ": Caught exception in a safe block" << std::endl; \
		    std::cerr << __func__ << ": Reason: " << _caughtException.what() << std::endl; \
            EXCEPTION_SAFE_BACKTRACE(); \
		} \
		catch (...) {} \
	} \
	catch (...) { \
	    try { \
		    std::cerr << __func__ << ": Caught unknown exception in a safe block" << std::endl; \
            EXCEPTION_SAFE_BACKTRACE(); \
		} \
		catch (...) {} \
	}

#endif
