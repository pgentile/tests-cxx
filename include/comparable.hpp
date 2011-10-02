#ifndef COMPARABLE_H
#define COMPARABLE_H

namespace patterns
{
	
	template<class T>
	class Comparable
	{
		public:
			
			virtual bool operator<(const T& other) const = 0;
			
			virtual bool operator==(const T& other) const = 0;
			
			bool operator<=(const T& other) const
			{
				// a <= b <=> not(not(a <= b)) <=> not(a > b) <=> not(b < a)
				return !(other < _ref());
			}
			
			bool operator>(const T& other) const
			{
				// a > b <=> b < a
				return other < _ref();
			}
			
			bool operator >=(const T& other) const
			{
				// a >= b <=> not(not(a => b)) <=> not(a < b)
				return !(_ref() < other);
			}
			
		private:
			
			inline const T& _ref(void) const {
				return static_cast<const T&>(*this);
			}
		
	};
	
}

#endif
