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
		static F apply(C& container, const F func) {
			typedef typename C::iterator I;
			I iteratorBegin = container.begin();
			I iteratorEnd = container.end();
			return for_each(iteratorBegin, iteratorEnd, func);
		}
		
		template<typename C, typename F>
		static F apply(const C& container, const F func) {
			typedef typename C::const_iterator I;
			I iteratorBegin = container.begin();
			I iteratorEnd = container.end();
			return for_each(iteratorBegin, iteratorEnd, func);
		}
		
		template<typename C>
		static void deletePointedObjects(C& container) {
			typedef typename C::value_type PT;
			apply(container, DeleteObjPointer<PT>());
		}
		
		template<typename C>
		static void copyElements(const C& source, C& dest) {
			for (auto element: source) {
				decltype(element) elementCopy = _copyPointerValue(element);
				dest.push_back(elementCopy);
			}
		}
		
	private:
		
		Containers();
		
		~Containers();
		
		template<typename T>
		static T* _copyPointerValue(const T* source) {
			return new T(*source);
		}

	};
	
}

#endif
