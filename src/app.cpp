// Quelques tests en C++

#include "logger/Logger.hpp"
#include "ndbm/RawStoreFile.hpp"
#include "util/Backtrace.hpp"
#include "util/Reflection.hpp"
#include "util/ExceptionSafe.hpp"
#include "util/Singleton.hpp"

#include <cstddef>
#include <cerrno>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <cstdint>

#include <sys/stat.h>

#include <boost/numeric/conversion/converter.hpp>

#include <boost/utility.hpp>


using namespace std;

using namespace util;

using boost::numeric::converter;
using boost::noncopyable;


template<typename T>
T* alloc(size_t count = 1, bool zero = false) {
	void* pt = NULL;
	if (zero) {
		pt = calloc(count, sizeof(T));
	} else {
		pt = malloc(count * sizeof(T));
	}
	return static_cast<T*>(pt);
}

class MutableInteger
{
	
public:
	
	MutableInteger(int value):
			_value(value)
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
		out << "    ";
	}
	return out;
}


inline Indent indent(unsigned int level = 1) {
	return Indent(level);
}



class Node: public enable_shared_from_this<Node>, private noncopyable {

public:
	
	class TreeView {
	
	public:
		
		TreeView(shared_ptr<Node const> node, bool showParent, unsigned int level):
				_node(node),
				_showParent(showParent),
				_level(level)
		{
		}
		
		TreeView(TreeView const & src):
				_node(src._node),
				_showParent(src._showParent),
				_level(src._level)
		{
		}
		
		TreeView& operator =(TreeView const & src) {
			_node = src._node;
			_showParent = src._showParent;
			_level = src._level;
			return *this;
		}
		
	private:
		
		shared_ptr<Node const> _node;
		
		bool _showParent;
		
		unsigned int _level;
		
		friend ostream& operator <<(ostream& out, TreeView const & treeView);
		
	};

	explicit Node(string const & name):
			_name(name),
			_parent(),
			_children()
	{
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
			shared_ptr<Node> const & current = *it;
			if (current == child) {
				child->_parent.reset();
				_children.erase(it);
				removed = true;
				break;
			}
		}
		return removed;
	}
	
	string const & getName() const {
		return _name;
	}
	
	shared_ptr<Node> getParent() const {
		return _parent.lock();
	}

	vector<shared_ptr<Node> > const & getChildren() const {
		return _children;
	}
	
	TreeView treeView(bool showParent = false, unsigned int level = 0) const {
		return TreeView(shared_from_this(), showParent, level);
	}
	
private:
	
	string const _name;

	weak_ptr<Node> _parent;

	vector<shared_ptr<Node> > _children;
	
	friend ostream& operator <<(ostream& out, Node const & node);

};

ostream& operator <<(ostream& out, Node const & node) {
	return out << "Node " << &node << " '" << node._name << "'";
}

ostream& operator <<(ostream& out, Node::TreeView const & treeView) {
	shared_ptr<Node const> node = treeView._node;
	unsigned int level = treeView._level;
	bool showParent = treeView._showParent;
	
	out << indent(level) << *node << " {" << endl;
	
	out << indent(level + 1) << "name = '" << node->getName() << "'" << endl;
	
	if (showParent || level == 0) {
		out << indent(level + 1) << "parent = ";
		shared_ptr<Node> parent = node->getParent();
		if (parent) {
			out << *parent;
		}
		else {
			out << "<no parent>";
		}
		out << endl;
	}
	
	out << indent(level + 1) << "children = ";
	vector<shared_ptr<Node> > const & children = node->getChildren();
	if (children.empty()) {
		out << "<empty>";
	}
	else {
		out << "[" << endl;
		for (auto child: children) {
			out << child->treeView(showParent, level + 2) << endl;
		}
		out << indent(level + 1) << "]";
	}
	out << endl;
	
	out << indent(level) << "}";
	
	return out;
}


template<typename T, typename ElemHash = hash<T> >
struct IterableHash
{

	template<typename I>
	size_t operator ()(I const & iterable) const {
		ElemHash hasher;
		size_t result = 0;
		bool empty = true;
		for (auto elem: iterable) {
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
	void printIterable(I const & iterable, ostream& out) {
		out << "[";
		for (auto elem: iterable) {
			out << elem << ", ";
		}
		out << "]";
	}

}


shared_ptr<Node> createTree() {
	shared_ptr<Node> node1 = make_shared<Node>("A");
	shared_ptr<Node> node2 = make_shared<Node>("B");
	shared_ptr<Node> node3 = make_shared<Node>("C");
	shared_ptr<Node> node4 = make_shared<Node>("D");
	shared_ptr<Node> node5 = make_shared<Node>("E");
	shared_ptr<Node> node6 = make_shared<Node>("F");
	
	node1->addChild(node2);
	node1->addChild(node3);
	node2->addChild(node4);
	node2->addChild(node5);
	node1->addChild(node6);

	return node1;
}


class Papa {

public:
	
	Papa(): _x(0) {
	}

	virtual ~Papa() {
		cout << "Destruction Papa" << endl;
	}
	
	Papa& operator =(Papa const & src) {
		cout << "Appel a " << __PRETTY_FUNCTION__ << endl;
		_x = src._x;
		return *this;
	}

private:
	
	int _x;

};


class Enfant: public Papa {

public:
	
	Enfant(): _y(0) {
	}
	
	Enfant& operator =(Enfant const & src) {
		cout << "Appel a " << __PRETTY_FUNCTION__ << endl;
		Papa::operator =(src);
		// Papa* daddy = this;
		// *daddy = src;
		_y = src._y;
		return *this;
	}
	
	virtual ~Enfant() {
		cout << "Destruction Enfant" << endl;
	}

private:
	
	int _y;

};


class PetitEnfant: public Enfant {

public:
	
	PetitEnfant(): _z(0) {
	}
	
	virtual ~PetitEnfant() {
		cout << "Destruction PetitEnfant" << endl;
	}

private:
	
	int _z;

};


class TestNoThrow {
	
public:
	
	TestNoThrow() {
		
	}

	~TestNoThrow() throw() {
		EXCEPTION_SAFE_BEGIN();
		stringstream message;
		message << __func__ << " : Test...";
		throw std::runtime_error(message.str().c_str());
		EXCEPTION_SAFE_END();
	}
	
};


class ExampleSingleton: public Singleton<ExampleSingleton> {
    
public:
    
    ExampleSingleton() {
        cout << __PRETTY_FUNCTION__ << endl;
    }
    
    virtual ~ExampleSingleton() {
        cout << __PRETTY_FUNCTION__ << endl;
    }
    
};


int main() {
	// shared_ptr<Node> rootNode = createTree();
	// cout << endl << rootNode->treeView() << endl << endl;
	
	{
		TestNoThrow nt;
	}
	
	PetitEnfant z;
	
	Enfant* test = new PetitEnfant();
	PetitEnfant* test2 = dynamic_cast<PetitEnfant*>(test);
	*test2 = z;
	cout << "test = " << test << endl;
	delete test;
	
	cout << "sizeof(Papa)        = " << sizeof(Papa) << endl;
	cout << "sizeof(Enfant)      = " << sizeof(Enfant) << endl;
	cout << "sizeof(PetitEnfant) = " << sizeof(PetitEnfant) << endl;
    cout << endl;

    cout << "Backtrace :" << endl;
    Backtrace backtrace;
    cout << backtrace << endl;
    
    cout << endl;
    
    ExampleSingleton& example = ExampleSingleton::instance();
    cout << &example << endl;
	
	return 0;
}
