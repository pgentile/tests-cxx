#include "core/CLibException.hpp"

#include <cstring>
#include <cerrno>
#include <sstream>

using namespace std;

namespace core {
	
	void CLibException::throwOnError() {
		if (errno != 0) {
			throw CLibException(errno);
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
