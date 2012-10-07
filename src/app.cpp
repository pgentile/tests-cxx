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
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>


using namespace std;
using namespace std::tr1;

using namespace core;
using namespace util;

using boost::numeric::converter;
using boost::shared_ptr;
using boost::weak_ptr;
using boost::make_shared;
using boost::enable_shared_from_this;


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


class Node: public enable_shared_from_this<Node> {

public:

	Node(): _parent(), _children() {
		cout << "Creation noeud " << this << endl;
	}

	~Node() {
		cout << "Suppression noeud " << this << endl;
	}

	shared_ptr<Node> getParent() const {
		return _parent.lock();
	}
	
	void addChild(shared_ptr<Node> child) {
		child->_parent = shared_from_this();
		_children.push_back(child);
	}
	
	bool removeChild(shared_ptr<Node> child) {
		bool removed = false;
		vector<shared_ptr<Node> >::iterator it, begin = _children.begin(), end = _children.end();
		for (it = begin; it != end; ++it) {
			shared_ptr<Node> current(*it);
			if (current == child) {
				child->_parent.reset();
				_children.erase(it);
				removed = true;
				break;
			}
		}
		return removed;
	}

	const vector<shared_ptr<Node> >& getChildren() const {
		return _children;
	}

private:
	
	Node(const Node& src);

	Node& operator =(const Node& src);

	weak_ptr<Node> _parent;

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

int main() {
	cout << "Debut" << endl;
	
	shared_ptr<Node> node1(new Node());
	shared_ptr<Node> node2(new Node());
	node1->addChild(node2);
	node1->removeChild(node2);
	
	cout << "Fin" << endl;
	return 0;
}
