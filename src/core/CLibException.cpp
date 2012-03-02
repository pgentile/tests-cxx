#include "core/CLibException.hpp"

#include <cstring>
#include <sstream>

using namespace std;

namespace core {
	
	void CLibException::throwOnError(int errnum) {
		if (errnum != 0) {
			throw CLibException(errnum);
		}
	}

	CLibException::CLibException(int errnum):
			Exception(_createMessage(errnum)),
			_errnum(errnum)
	{
	}
	
	CLibException::CLibException(const CLibException& src):
		Exception(src.what()),
		_errnum(src._errnum)
	{
	}
	
	CLibException::~CLibException() throw() {
	}
	
	string CLibException::_createMessage(int errnum) {
		ostringstream message;
		message << "errno = " << errnum << ": " << strerror(errnum);
		return message.str();
	}
	
}
