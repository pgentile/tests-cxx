#include "core/Exception.hpp"
#include "core/Reflection.hpp"
#include "core/StackElement.hpp"

using namespace std;


namespace core {
	
	Exception::Exception(const string& what): exception(), _what(what), _thread(this_thread::get_id()) {
	}
	
	Exception::Exception(const exception& ex): exception() {
		_what += "Wrapped from ";
		_what += Reflection::getRealTypeName(typeid(ex));

		const char* what = ex.what();
		if (what != NULL) {
			_what += ": ";
			_what += ex.what();
		}
		
		_thread = this_thread::get_id();
	}
	
	Exception::Exception(const Exception& src):
		_what(src._what),
		_backtrace(src._backtrace),
		_thread(src._thread)
	{
	}
	
	Exception::~Exception() throw() {
	}
	
	Exception& Exception::operator =(const Exception& src) {
		_what = src._what;
		_backtrace = src._backtrace;
		_thread = src._thread;
		return *this;
	}
	
	const string Exception::thrownClassName() const {
		return Reflection::getRealTypeName(typeid(*this));
	}
	
	ostream& operator <<(ostream& out, const Exception& exception) {
		out << exception.thrownClassName() << " (from thread " << exception.thread() << "): " << exception.what() << endl;
		out << endl;
		
		out << "Backtrace:";
		unsigned int index = 0;
		for (StackElement const& element: exception.backtrace().elements()) {
			out << endl << '\t' << index << ": " << element;
			index++;
		}
		
		return out;
	}
	
}
