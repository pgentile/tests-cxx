#include "util/UndefinedError.hpp"

using core::Exception;
using std::ostream;

namespace util
{

	UndefinedError::UndefinedError(): Exception("Not defined") {
		
	}
	
	UndefinedError::~UndefinedError() throw() {
		
	}

}
