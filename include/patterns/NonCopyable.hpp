#ifndef NON_COPYABLE
#define NON_COPYABLE

namespace patterns
{

	class NonCopyable
	{
		
		protected:
			inline NonCopyable(void)
			{
			}
			
			inline virtual ~NonCopyable()
			{
			}
	
		private:
			NonCopyable(const NonCopyable& base);
			
			NonCopyable& operator=(NonCopyable& base);
	
	};

}

#endif
