#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <sstream>

namespace log
{
	
	using namespace std;

	class Log
	{
		
	public:
		
		Log(void);
		
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

		Log(const Log& base);
		
		ostringstream _message;
	
	};
	
	class Logging
	{
		
	public:
		
		Logging(void)
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
