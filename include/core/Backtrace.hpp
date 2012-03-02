#ifndef CORE_BACKTRACE_HPP
#define CORE_BACKTRACE_HPP

#include "core/StackElement.hpp"

#include <string>
#include <vector>

namespace core {
	
	using namespace std;
	
	class Backtrace
	{
		
	public:
		
		Backtrace(bool initElements = true);

		Backtrace(const Backtrace& src);
		
		~Backtrace();

		const vector<StackElement*>& elements() const;

	private:
		
		StackElement* _createElement(void* addr);
		
		vector<StackElement*> _elements;
	
	};
	
	inline const vector<StackElement*>& Backtrace::elements() const {
		return _elements;
	}
	
}

#endif
