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
#include <boost/utility.hpp>


using namespace std;
using namespace std::tr1;

using namespace core;
using namespace util;

using boost::numeric::converter;
using boost::shared_ptr;
using boost::weak_ptr;
using boost::make_shared;
using boost::enable_shared_from_this;
using boost::noncopyable;


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


class Indent {

public:
	
	explicit Indent(unsigned int level): _level(level) {
		
	}
	
	Indent(Indent const & src): _level(src._level) {
		
	}
	
	~Indent() {
		
	}
	
	Indent& operator =(Indent const & src) {
		_level = src._level;
		return *this;
	}
	
private:
	
	unsigned int _level;
	
	friend ostream& operator <<(ostream& out, Indent const & indent);
	
};


ostream& operator <<(ostream& out, Indent const & indent) {
	for (unsigned int i = 0; i < indent._level; ++i) {
		out << '\t';
	}
	return out;
}


Indent indent(unsigned int level) {
	return Indent(level);
}



class Node: public enable_shared_from_this<Node>, private noncopyable {

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
		shared_ptr<Node> childParent = child->getParent();
		if (childParent) {
			childParent->removeChild(child);
		}
		
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
	
	void printAsTree(ostream& out, unsigned int level = 0) {
		out << indent(level) << "Node " << this << " {" << endl;
		
		if (_children.empty()) {
			out << indent(level + 1) << "children = <empty>" << endl;
		}
		else {
			out << indent(level + 1) << "children = [" << endl;
			BOOST_FOREACH(shared_ptr<Node> child, _children) {
				child->printAsTree(out, level + 2);
			}
			out << indent(level + 1) << "]" << endl;
		}
		
		
		out << indent(level) << "}" << endl;
	}

	vector<shared_ptr<Node> > const & getChildren() const {
		return _children;
	}

private:

	weak_ptr<Node> _parent;

	vector<shared_ptr<Node> > _children;
	
	friend ostream& operator <<(ostream& out, Node const & node);

};

ostream& operator <<(ostream& out, Node const & node)
{
	out << "Node " << &node << " [";
	out << "parent = " << node._parent.lock();
	out << ", nb children = " << node._children.size();
	out << "]";
	return out;
}


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
	shared_ptr<Node> node3(new Node());
	
	cout << "node1 = ";
	node1->printAsTree(cout);
	cout << endl;
	
	cout << "addChild(...)" << endl;
	node1->addChild(node2);
	node2->addChild(node3);
	cout << "node1 = ";
	node1->printAsTree(cout);
	cout << endl;
	
	cout << "removeChild(...)" << endl;
	node1->addChild(node3);
	cout << "node1 = ";
	node1->printAsTree(cout);
	cout << endl;
	
	cout << "Fin" << endl;
	return 0;
}
