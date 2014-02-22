#ifndef UTIL_EXCEPTION_SAFE_HPP
#define UTIL_EXCEPTION_SAFE_HPP

#include <exception>


namespace util {
namespace exceptionsafe {
    
    void aboutException(char const* funcName, std::exception const* e);
    
} // namespace exceptionsafe
} // namespace util


#define EXCEPTION_SAFE_BEGIN() \
	try \
	{

#define EXCEPTION_SAFE_END() \
	} \
	catch (const ::std::exception& _caughtException ## __LINE__) { \
	    try { \
            ::util::exceptionsafe::aboutException(__func__, &_caughtException ## __LINE__); \
		} \
		catch (...) {} \
	} \
	catch (...) { \
	    try { \
            ::util::exceptionsafe::aboutException(__func__, nullptr); \
		} \
		catch (...) {} \
	}

#endif
