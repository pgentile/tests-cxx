#include "core/Exception.hpp"
#include "core/Reflection.hpp"

using namespace std;

namespace core {
	
	Exception::Exception(const string& what): exception(), _what(what) {
	}
	
	Exception::Exception(const exception& ex): exception() {
		_what += "Wrapped from ";
		_what += Reflection::getRealTypeName(typeid(ex));

		const char* what = ex.what();
		if (what != NULL) {
			_what += ": ";
			_what += ex.what();
		}
	}
	
	Exception::Exception(const Exception& src):
		_what(src._what),
		_backtrace(src._backtrace)
	{
	}
	
	Exception::~Exception() throw() {
	}
	
	const string Exception::thrownClassName() const {
		return Reflection::getRealTypeName(typeid(*this));
	}
	
	ostream& operator <<(ostream& out, const Exception& exception) {
		out << exception.thrownClassName() << ": " << exception.what() << endl;
		out << endl;
		
		out << "Backtrace:";
		const vector<StackElement*>& elements = exception.backtrace().elements();
		vector<StackElement*>::const_iterator elementIt;
		vector<StackElement*>::const_iterator elementItEnd = elements.end();
		unsigned int index = 0;
		for (elementIt = elements.begin(); elementIt != elementItEnd; elementIt++) {
			StackElement* element = *elementIt;
			out << endl << '\t' << index << ": " << *element;
			index++;
		}
		
		return out;
	}
	
}
