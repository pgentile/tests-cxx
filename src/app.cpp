#include <iostream>

#include "core/CLibException.hpp"
#include "core/Exception.hpp"
#include "logger/Logger.hpp"
#include "util/Optional.hpp"

#include <cerrno>
#include <cstdlib>
#include <cmath>
#include <cfloat>

using namespace std;
using namespace std::rel_ops;

using namespace core;
using namespace util;

template<typename T>
T* alloc(size_t count = 1, bool zero = false) {
	void* pt = NULL;
	if (zero) {
		pt = calloc(count, sizeof(T));
	} else {
		pt = malloc(count * sizeof(T));
	}
	return (T*) pt;
}


int main()
{	
	/*
	try {
		Optional<int> x = Optional<int>::undefined;
		x.checkDefined();
	} catch (const Exception& e) {
		cout << e << endl;
	}
	*/
	
	/*
	int* x = alloc<int>(1, true);
	cout << "x = " << *x << endl;
	*/
	
	const logger::Level& level1 = logger::Level::info;
	const logger::Level& level2 = logger::Level::debug;
	if (level1 > level2) {
		cout << level1 << " > " << level2 << endl;
	}
	
	cout << "Debut du programme" << endl;
	
	try {
		
		double x = -1;
		cout << "sqrt(" << x << ") = " << sqrt(x) << endl;
		cout << "errno = " << errno << endl;
		CLibException::throwOnError(errno);
	} catch (const Exception& e) {
		cout << e << endl;
	}
	
	cout << "Arret du programme" << endl;
	
	return 0;
}
