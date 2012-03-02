#include "core/Backtrace.hpp"

#include "core/Reflection.hpp"
#include "util/Containers.hpp"

#include <dlfcn.h>
#include <execinfo.h>
#include <iostream>

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
				StackElement* element = _createElement(addr);
				if (element != NULL) {
					_elements.push_back(element);
				}
			}
		}
	}
	
	Backtrace::Backtrace(const Backtrace& src) {
		Containers::copyElements(src._elements, _elements);
	}
	
	Backtrace::~Backtrace() {
		Containers::deleteObjPointers(_elements);
	}
	
	StackElement* Backtrace::_createElement(void* addr) {
		StackElement* element = NULL;
		Dl_info dynLinkInfo;
		if (dladdr(addr, &dynLinkInfo) != 0) {
			if (dynLinkInfo.dli_saddr != NULL) {
				string symbolName = Reflection::demangleName(dynLinkInfo.dli_sname);
				element = new StackElement(dynLinkInfo.dli_fname, dynLinkInfo.dli_fbase,
						symbolName, dynLinkInfo.dli_saddr);
			}
		}	
		return element;
	}
	
	ostream& operator <<(ostream& out, const Backtrace& backtrace) {
		const vector<StackElement*>& elements = backtrace.elements();
		vector<StackElement*>::const_iterator elementIt;
		vector<StackElement*>::const_iterator elementItEnd = elements.end();
		unsigned int index = 0;
		for (elementIt = elements.begin(); elementIt != elementItEnd; elementIt++) {
			StackElement* element = *elementIt;
			out << index << ": " << *element << endl;
			index++;
		}
		return out;
	}
	
}
