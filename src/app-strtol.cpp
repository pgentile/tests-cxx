#include <iostream>
#include <sstream>
#include <ios>
#include <string>

using namespace std;


namespace {
	
	template<class T>
	bool parseString(istringstream& stream, T& value) {
		stream >> value;
		return !stream.fail();
	}
	
	template<>
	bool parseString(istringstream& stream, bool& value) {
		stream >> boolalpha >> value >> noboolalpha;
		return !stream.fail();
	}

	template<class T>
	bool parseString(string const& s, T& value) {
		istringstream stream(s);
		return parseString(stream, value);
	}

}


int main(int argc, char const* argv[]) {
	for (int i = 1; i < argc; i++) {
		cout << "Arg " << i << ": " << argv[i] << endl;
		
		bool v = 0;
		if (parseString(argv[i], v)) {
			cout << "  Parsed arg: " << v << endl;
		}
	}
	return 0;
}
