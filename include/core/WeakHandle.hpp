#ifndef CORE_WEAK_HANDLE_HPP
#define CORE_WEAK_HANDLE_HPP

#include "core/Exception.hpp"

#include <iostream>

namespace core {
	
	using namespace std;
	
	template<class T>
	class Handle;

	template<class T>
	class WeakHandle {

	public:
	
		WeakHandle(const WeakHandle& source):
				_pt(source._pt),
				_count(source._count)
		{
			cout << "Referencer WeakHandle sur " << _pt << endl;
		}

		WeakHandle(const Handle<T>& source):
				_pt(source._pt),
				_count(source._count)
		{
			cout << "Referencer WeakHandle sur " << _pt << endl;
		}
	
		~WeakHandle() {
			cout << "Detruire WeakHandle sur " << _pt << endl;
		}

		WeakHandle& operator=(const WeakHandle& source) {
			if (_pt != source._pt) {
				_pt = source._pt;
				_count = source._count;
			}
			return *this;
		}

		WeakHandle& operator=(const Handle<T>& source) {
			if (_pt != source._pt) {
				_pt = source._pt;
				_count = source._count;
			}
			return *this;
		}
	
		T& operator*() {
			return *_pt;
		}

		const T& operator*() const {
			return *_pt;
		}
	
		T* operator->() {
			return _pt;
		}
	
		const T* operator->() const {
			return _pt;
		}

	private:

		T* _pt;
	
		unsigned int* _count;

	};
	
}

#endif
