#include "core/CLibException.hpp"
#include "core/Exception.hpp"
#include "core/Handle.hpp"
#include "core/WeakHandle.hpp"
#include "logger/Logger.hpp"
#include "util/Optional.hpp"
#include "ndbm/RawStoreFile.hpp"

#include <cerrno>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <iterator>
#include <tr1/unordered_map>

#include <sys/stat.h>

#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>


using namespace std;
using namespace std::tr1;

using namespace core;
using namespace util;

using boost::numeric::converter;
using boost::shared_ptr;
using boost::make_shared;

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

class MutableInteger
{
	
public:
	
	MutableInteger(int value):
			_value(value)
	{
	}
	
	~MutableInteger()
	{
	}
	
	int value() const {
		return _value;
	}
	
	void value(int value) {
		_value = value;
	}

private:
	
	int _value;

};


class Node {

public:

	Node() {
	}

	Node(const Node& src) {
	}

	~Node() {
	}

	Node& operator =(const Node& src) {
		return *this;
	}

	shared_ptr<Node> getParent() const {
		return _parent;
	}

	void setParent(const shared_ptr<Node>& parent) {
		_parent = parent;
	}

	vector<shared_ptr<Node> > getChildren() const {
		return _children;
	}

	void setChildren(const vector<shared_ptr<Node> >& children) {
		_children = children;
	}

private:

	shared_ptr<Node> _parent;

	vector<shared_ptr<Node> > _children;

};


template<typename T, typename ElemHash = hash<T> >
struct IterableHash
{

	template<typename I>
	size_t operator ()(const I& iterable) const {
		ElemHash hasher;
		size_t result = 0;
		bool empty = true;
		BOOST_FOREACH(const T& elem, iterable) {
			if (empty) {
				empty = false;
				result = 1;
			}
			result = 31 * result + hasher(elem);
		}			
		return result;
	}

};

namespace std {

template<typename I>
void printIterable(const I& iterable, ostream& out) {
	typedef typename I::const_reference R;
	out << "[";
	BOOST_FOREACH(R elem, iterable) {
		out << elem << ", ";
	}
	out << "]";
}

}


int main()
{
	typedef unordered_map<vector<string>, string, IterableHash<string> > CustomHashMap;
	typedef CustomHashMap::value_type CustomPair;
	
	vector<string> key1;
	key1.push_back("test1");
	key1.push_back("test2");
	key1.push_back("test3");
	
	string value1 = "ABC";
	
	pair<vector<string>, string> entry1(key1, value1);
	
	CustomHashMap hashMap;
	hashMap.insert(entry1);
	
	BOOST_FOREACH(const CustomPair& p, hashMap) {
		std::cout << "Entry : ";
		printIterable(p.first, std::cout);
		std::cout << " -> " << p.second << endl;
	}
	
	shared_ptr<MutableInteger> x1(new MutableInteger(5));
	shared_ptr<const MutableInteger> x2 = x1;
	
	vector<shared_ptr<MutableInteger> > ints;
	ints.push_back(make_shared<MutableInteger>(30495));
	ints.push_back(make_shared<MutableInteger>(98374));
	ints.push_back(make_shared<MutableInteger>(75843));
	ints.push_back(make_shared<MutableInteger>(10875));
	
	cout << "Contenu de ints :" << endl;
	BOOST_FOREACH(const shared_ptr<MutableInteger>& x, ints) {
		cout << "\t- " << x->value() << " (ref count = " << x.use_count() << ")" << endl;
	}
	
	cout << "Nb de references sur x1 : " << x1.use_count() << endl;
	cout << "Travail avec x1 : " << x1->value() << endl;
	cout << "Travail avec x2 : " << x2->value() << endl;
	cout << "Convertion de nombre : " << converter<double, uint32_t>::convert(1897) << endl;
	
	ndbm::RawStoreFile rawStoreFile("/tmp/ndbmTest", O_RDWR | O_CREAT, 0644);
	rawStoreFile.open();
	
	const string key = "ABC";
	const string value = "DEF";
	
	datum keyStruct;
	keyStruct.dptr = const_cast<char*>(key.c_str());
	keyStruct.dsize = key.size();
	
	datum valueStruct = rawStoreFile.fetch(keyStruct);
	if (valueStruct.dptr == NULL) {
		cout << "Cle " << key << " non definie" << endl;
		
		datum newValueStruct;
		newValueStruct.dptr = const_cast<char*>(value.c_str());
		newValueStruct.dsize = value.size();
		rawStoreFile.store(keyStruct, newValueStruct, DBM_REPLACE);
	} else {
		const string value(reinterpret_cast<char*>(valueStruct.dptr), valueStruct.dsize);
		cout << "Cle " << key << " definie, valeur = " << value << endl;
	}
	
	rawStoreFile.close();
	
	exception ex1;
	Exception ex2("Exception perso 2");
	Exception ex3("Exception perso 3");
	ex2 = ex3;
	cout << "ex2: " << ex2 << endl;
	
	return 0;
}
