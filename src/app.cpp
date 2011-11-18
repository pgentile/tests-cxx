#include <iostream>

#include "optional/Optional.hpp"


using namespace optional;
using namespace std;


template<typename T>
class TestConstructor
{

public:
	TestConstructor(const Optional<T> x)
	{
		cout << "TestConstructor: " << x << endl;
	}
	
	~TestConstructor()
	{
	}

};

int main(void)
{
	TestConstructor<unsigned int> tc(5);
		
	return 0;
}
