#include "core/StackElement.hpp"

#include <algorithm>


namespace core {
	
	using namespace std;
	
	StackElement::StackElement(const string& sharedObjectPath, void* sharedObjectAddr,
			const string& symbolName, void* symbolAddr):
	 		_sharedObjectPath(sharedObjectPath),
			_sharedObjectAddr(sharedObjectAddr),
	 		_symbolName(symbolName),
			_symbolAddr(symbolAddr)
	{
	}
	
	StackElement::StackElement(StackElement&& source):
	    _sharedObjectPath(move(source._sharedObjectPath)),
		_sharedObjectAddr(source._sharedObjectAddr),
	 	_symbolName(move(source._symbolName)),
		_symbolAddr(source._symbolAddr)
	{
	}
	
	StackElement::StackElement(const StackElement& source):
			_sharedObjectPath(source._sharedObjectPath),
			_sharedObjectAddr(source._sharedObjectAddr),
			_symbolName(source._symbolName),
			_symbolAddr(source._symbolAddr)
	{
	}
	
	StackElement::~StackElement() {
		
	}
	
	StackElement& StackElement::operator =(const StackElement& source) {
		_sharedObjectPath = source._sharedObjectPath;
		_sharedObjectAddr = source._sharedObjectAddr;
		_symbolName = source._symbolName;
		_symbolAddr = source._symbolAddr;
		return *this;
	}
	
	StackElement& StackElement::operator =(StackElement&& source) {
	    _sharedObjectPath = move(source._sharedObjectPath);
		_sharedObjectAddr = source._sharedObjectAddr;
		_symbolName = move(source._symbolName);
		_symbolAddr = source._symbolAddr;
        return *this;
	}
	
	ostream& operator <<(ostream& out, const StackElement& element) {
		out << element.sharedObjectPath() << ": " << element.symbolName();
		return out;
	}
	
}
