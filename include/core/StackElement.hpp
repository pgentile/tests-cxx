#ifndef CORE_STACK_ELEMENT_HPP
#define CORE_STACK_ELEMENT_HPP

#include <iostream>
#include <string>

namespace core {
	
	using namespace std;
	
	class StackElement final
	{
		
	public:
		
		StackElement(const string& sharedObjectPath, void* sharedObjectAddr, const string& symbolName, void* symbolAddr);
		
        StackElement(StackElement&& source);
		
		StackElement(const StackElement& source);
		
		~StackElement();
		
		StackElement& operator =(const StackElement& source);
		
		StackElement& operator =(StackElement&& source);
		
		const string& sharedObjectPath() const;
		
		const void* sharedObjectAddr() const;
		
		const string& symbolName() const;
		
		const void* symbolAddr() const;

	private:
		
		string _sharedObjectPath;
		
		void* _sharedObjectAddr;
		
		string _symbolName;
		
		void* _symbolAddr;
		
	};
	
	ostream& operator <<(ostream& out, const StackElement& stackElement);
	
	inline const string& StackElement::sharedObjectPath() const {
		return _sharedObjectPath;
	}
	
	inline const void* StackElement::sharedObjectAddr() const {
		return _sharedObjectAddr;
	}
	
	inline const string& StackElement::symbolName() const {
		return _symbolName;
	}
	
	inline const void* StackElement::symbolAddr() const {
		return _symbolAddr;
	}
	
}

#endif
