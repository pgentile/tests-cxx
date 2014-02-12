#ifndef LOGGER_LOG_HPP
#define LOGGER_LOG_HPP

#include <iostream>
#include <sstream>

namespace log
{
	
	using namespace std;

	class Log final
	{
		
	public:
		
		Log();
		
        Log(Log const&) = delete;
        Log& operator =(Log const&) = delete;
		
		template<typename T>
		Log& operator <<(const T& value)
		{
			_message << value;
			return *this;
		}
		
		Log& operator <<(ostream& (*pf)(ostream&));
		
		Log& operator <<(ios& (*pf)(ios&));
		
		Log& operator <<(ios_base& (*pf)(ios_base&));
		
		friend ostream& operator <<(ostream& out, const Log& log);
	
	private:
		
		ostringstream _message;
	
	};
	
	class Logging
	{
		
	public:
		
		Logging()
		{
		}
		
		template<typename T>
		Log operator <<(const T& value)
		{
			Log log;
			log << value;
			return log;
		}
		
	private:
		
	};

}

#endif
