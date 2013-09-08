#include "logger/Log.hpp"

#include <pthread.h>
#include <stdlib.h>

namespace log
{
	
	static ostream& (*const endlPf)(ostream&) = endl;
	
	Log::Log()
	{
		_message << '[' << getpid() << " - " << this_thread::get_id() << "] ";
	}
	
	Log& Log::operator <<(ostream& (*pf)(ostream&))
	{
		if (pf == endlPf) {
			_message << " * VALIDATION PAR ENDL";
		}
		pf(_message);
		return *this;
	}
	
	Log& Log::operator <<(ios& (*pf)(ios&))
	{
		pf(_message);
		return *this;
	}
	
	Log& Log::operator <<(ios_base& (*pf)(ios_base&))
	{
		pf(_message);
		return *this;
	}
	
	ostream& operator <<(ostream& out, const Log& log)
	{
		out << log._message.str();
		return out;
	}
	
}
