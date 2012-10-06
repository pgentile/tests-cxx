#ifndef CORE_HANDLE_HPP
#define CORE_HANDLE_HPP

#include <iostream>

namespace core {
	
	using namespace std;
	
	template<class T>
	class WeakHandle;

	template<class T>
	class Handle {
		
		friend class WeakHandle<T>;

	public:
	
		Handle(T* pt):
				_pt(pt),
				_count(new unsigned int(1))
		{
			cout << "Creer Handle sur " << _pt << endl;
		}
		
		Handle(const Handle& source):
				_pt(source._pt),
				_count(source._count)
		{
			cout << "Referencer Handle sur " << _pt << endl;
				
			source.use();
		}
	
		~Handle() {
			cout << "Detruire Handle sur " << _pt << endl;
		
			unuse();
		}

		Handle& operator=(const Handle& source) {
			if (_pt != source._pt) {
				unuse();
				
				source.use();
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
	
		void use() const throw() {
			(*_count)++;
			
			cout << "Incrementation du pointeur " << _pt << " : " << *_count << endl;
		}
	
		void unuse() throw() {
			(*_count)--;
			
			cout << "Decrementation du pointeur " << _pt << " : " << *_count << endl;
		
			if (*_count == 0) {
				cout << "Destruction de l'objet " << _pt << endl;
				
				delete _pt;
				_pt = NULL;	
				
				delete _count;
			}
		}

		T* _pt;
	
		mutable unsigned int* _count;

	};
	
}

#endif
