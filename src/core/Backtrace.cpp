#include "core/Backtrace.hpp"

#include "core/StackElement.hpp"
#include "core/Reflection.hpp"
#include "util/Containers.hpp"

#include <dlfcn.h>
#include <execinfo.h>
#include <iostream>
#include <algorithm>

using boost::optional;


#define BACKTRACE_MAX_SIZE 100


namespace core {
	
	using namespace std;
	using namespace util;
	
	Backtrace::Backtrace(bool initElements) {
		if (initElements) {
			void* allAddr[BACKTRACE_MAX_SIZE];
			int nbEntries = backtrace(allAddr, BACKTRACE_MAX_SIZE);
		
			for (int i = 0; i < nbEntries; i++) {
				void* addr = allAddr[i];
				optional<StackElement> element = _createElement(addr);
				if (element) {
					_elements.push_back(*element);
				}
			}
		}
	}
	
	Backtrace::Backtrace(const Backtrace& src):
	    _elements(src._elements)
	{
	}
	
	Backtrace::Backtrace(Backtrace&& src):
	    _elements(move(src._elements))
	{
	}
    
	Backtrace::~Backtrace() {
	}
	
	Backtrace& Backtrace::operator =(const Backtrace& src) {
		_elements.clear();
        copy(src._elements.cbegin(), src._elements.cend(), _elements.begin());
		return *this;
	}
	
	Backtrace& Backtrace::operator =(Backtrace&& src) {
        _elements = move(src._elements);
        return *this;
	}
	
	optional<StackElement> Backtrace::_createElement(void* addr) {
		Dl_info dynLinkInfo;
		if (dladdr(addr, &dynLinkInfo) != 0) {
			if (dynLinkInfo.dli_saddr != NULL) {
                StackElement element(
                    dynLinkInfo.dli_fname,
				    dynLinkInfo.dli_fbase,
					Reflection::demangleName(dynLinkInfo.dli_sname),
					dynLinkInfo.dli_saddr
                );
				return optional<StackElement>(element);
			}
		}	
		return optional<StackElement>();
	}
	
	ostream& operator <<(ostream& out, const Backtrace& backtrace) {
        unsigned int index = 0;
        for (StackElement const& element: backtrace.elements()) {
            out << index << ": " << element << endl;
			index++;
        }
		return out;
	}
	
}
