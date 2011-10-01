#ifndef COMPARABLE_H
#define COMPARABLE_H

namespace patterns
{
	
	template<class T>
	class Comparable
	{
		public:
			
			virtual bool operator<(const T& other) const = 0;
						
			bool operator<=(const T& other) const
			{
				// a <= b <=> not(not(a <= b)) <=> not(a > b) <=> not(b < a)
				return !(other < *this);
			}

			bool operator>(const T& other) const
			{
				// a > b <=> b < a
				return other < *this;
			}
			
			bool operator >=(const T& other) const
			{
				// a >= b <=> not(not(a => b)) <=> not(a < b)
				return !(*this < other);
			}
		
	};
	
}

#endif
