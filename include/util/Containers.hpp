#ifndef UTIL_CONTAINERS_HPP
#define UTIL_CONTAINERS_HPP

#include "util/DeleteObjPointer.hpp"

#include <algorithm>
#include <iostream>

namespace util {
	
	using namespace std;
	
	class Containers
	{
		
	public:
		
		template<typename C, typename F>
		static F apply(const C& container, F func) {
			typedef typename C::const_iterator I;
			I iteratorBegin = container.begin();
			I iteratorEnd = container.end();
			return for_each(iteratorBegin, iteratorEnd, func);
		}
		
		template<typename C>
		static void deleteObjPointers(const C& container) {
			typedef typename C::value_type PT;
			Containers::apply(container, DeleteObjPointer<PT>());
		}
		
		template<typename C>
		static void copyElements(const C& source, C& dest) {
			typedef typename C::const_iterator I;
			typedef typename C::value_type PT;
			I iterator;
			I iteratorEnd = source.end();
			for (iterator = source.begin(); iterator != iteratorEnd; iterator++) {
				PT element = *iterator;
				PT elementCopy = _copyPointerValue(element);
				dest.push_back(elementCopy);
			}
		}
		
	private:
		
		Containers();
		
		~Containers();
		
		template<typename T>
		static T* _copyPointerValue(const T* source) {
			cout << "Copy " << source << endl;
			return new T(*source);
		}

	};
	
}

#endif
