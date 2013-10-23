#ifndef CORE_BACKTRACE_HPP
#define CORE_BACKTRACE_HPP

#include <string>
#include <vector>

#include <boost/optional.hpp>


namespace core {
	
	using namespace std;
	
	class StackElement;
	
	class Backtrace
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
		
		boost::optional<StackElement> _createElement(void* addr);
		
		vector<StackElement> _elements;
	
	};
	
	ostream& operator <<(ostream& out, const Backtrace& backtrace);
	
}

#endif
