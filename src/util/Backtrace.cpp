#include "util/Backtrace.hpp"

#include <dlfcn.h>
#include <execinfo.h>
#include <iostream>
#include <algorithm>

#include <boost/numeric/conversion/cast.hpp>

#include "util/reflection.hpp"

using namespace std;
using namespace util;
using namespace util::reflection;
using boost::numeric_cast;


#define BACKTRACE_MAX_SIZE 100


namespace {
    
	Optional<StackElement> createStackElement(void* addr) {
		Dl_info dynLinkInfo;
		if (dladdr(addr, &dynLinkInfo) != 0) {
			if (dynLinkInfo.dli_saddr != NULL) {
                string name = demangleName(dynLinkInfo.dli_sname);
                StackElement element(
                    dynLinkInfo.dli_fname,
				    dynLinkInfo.dli_fbase,
					name,
					dynLinkInfo.dli_saddr
                );
				return Optional<StackElement>(move(element));
			}
		}	
		return Optional<StackElement>();
	}

}


namespace util {
	
	// Classe Backtrace
	
	Backtrace::Backtrace(Optional<unsigned int> strip) {
		void* allAddr[BACKTRACE_MAX_SIZE];
		unsigned int nbEntries = numeric_cast<unsigned int>(backtrace(allAddr, BACKTRACE_MAX_SIZE));
	
	    // Ajouter + 1 pour exclure le constructeur du backtrace
        unsigned int realStrip = strip.apply([] (unsigned int n) { return n + 1; }).orDefault(0);
	
		for (unsigned int i = realStrip; i < nbEntries; i++) {
			void* addr = allAddr[i];
			Optional<StackElement> element = createStackElement(addr);
			if (element) {
			    _elements.push_back(move(*element));
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
	
	ostream& operator <<(ostream& out, const Backtrace& backtrace) {
        unsigned int index = 0;
        for (StackElement const& element: backtrace.elements()) {
            out << index << ": " << element << endl;
			index++;
        }
		return out;
	}
	
	
	// Classe StackElement
	
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
