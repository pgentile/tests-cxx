#ifndef UTIL_BACKTRACE_HPP
#define UTIL_BACKTRACE_HPP

#include <string>
#include <vector>

#include "util/Optional.hpp"


namespace util {
	
	using namespace std;
	
    class StackElement;
		
	class Backtrace final
	{
		
	public:
		
		explicit Backtrace(bool initElements = true);

		Backtrace(const Backtrace& src);
		
		Backtrace(Backtrace&& src);
		
		~Backtrace();
		
		Backtrace& operator =(const Backtrace& src);
		
		Backtrace& operator =(Backtrace&& src);
        
        const vector<StackElement>& elements() const {
            return _elements;
		}

	private:

		vector<StackElement> _elements;
	
	};
	
	ostream& operator <<(ostream& out, const Backtrace& backtrace);
	
	
	class StackElement final
	{
		
	public:
		
		StackElement(const std::string& sharedObjectPath, void* sharedObjectAddr, const std::string& symbolName, void* symbolAddr);
		
        StackElement(StackElement&& source);
		
		StackElement(const StackElement& source);
		
		~StackElement();
		
		StackElement& operator =(const StackElement& source);
		
		StackElement& operator =(StackElement&& source);
		
    	const std::string& sharedObjectPath() const {
    		return _sharedObjectPath;
    	}
	
    	const void* sharedObjectAddr() const {
    		return _sharedObjectAddr;
    	}
	
    	const std::string& symbolName() const {
    		return _symbolName;
    	}
	
    	const void* symbolAddr() const {
    		return _symbolAddr;
    	}

	private:
		
		std::string _sharedObjectPath;
		
		void* _sharedObjectAddr;
		
		std::string _symbolName;
		
		void* _symbolAddr;
		
	};
	
	std::ostream& operator <<(std::ostream& out, const StackElement& stackElement);

}

#endif
