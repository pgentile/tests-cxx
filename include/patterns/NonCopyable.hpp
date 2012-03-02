#ifndef PATTERNS_NON_COPYABLE_H
#define PATTERNS_NON_COPYABLE_H

namespace patterns
{

	class NonCopyable
	{
		
		protected:
			NonCopyable();
			
			virtual ~NonCopyable();
	
		private:
			NonCopyable(const NonCopyable& base);
			
			NonCopyable& operator=(NonCopyable& base);
	
	};
	
	inline NonCopyable::NonCopyable()
	{
	}
	
	inline NonCopyable::~NonCopyable()
	{
	}

}

#endif
