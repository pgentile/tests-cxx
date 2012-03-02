#ifndef PATTERNS_EXCEPTION_SAFE_H
#define PATTERNS_EXCEPTION_SAFE_H

#include <exception>
#include <iostream>

#define EXCEPTION_SAFE_BEGIN() \
	try \
	{

#define EXCEPTION_SAFE_END() \
	} \
	catch (const std::exception& catchedException ## __LINE__) { \
		std::cerr << __FUNCTION__ << ": Catched exception" << std::endl; \
		std::cerr << __FUNCTION__ << ": Reason: " << catchedException ## __LINE__.what() << std::endl; \
	}

#endif
