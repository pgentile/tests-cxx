#ifndef UTIL_DELETE_OBJ_POINTER_HPP
#define UTIL_DELETE_OBJ_POINTER_HPP

namespace util {
	
	template<typename PT>
	class DeleteObjPointer
	{
		
	public:

		void operator()(PT pt) {
			delete pt;
		}
		
	};

}

#endif
