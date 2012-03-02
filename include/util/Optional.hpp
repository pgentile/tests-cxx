#ifndef UTIL_OPTIONAL_HPP
#define UTIL_OPTIONAL_HPP

#include "util/UndefinedError.hpp"

#include <iostream>
#include <typeinfo>

namespace util
{
	
	using namespace std;

	template<typename T>
	class Optional
	{
		
		typedef T ValueType;
	
	public:

		template<typename A>
		Optional(const A& a): _defined(true), _value(a)
		{
			cout << "Constructor " << __PRETTY_FUNCTION__ << " called" << endl;
		}

		template<typename A, typename B>
		Optional(const A& a, const B& b): _defined(true), _value(a, b)
		{
			cout << "Constructor " << __PRETTY_FUNCTION__ << " called" << endl;
		}

		template<typename A, typename B, typename C>
		Optional(const A& a, const B& b, const C& c): _defined(true), _value(a, b, c)
		{
			cout << "Constructor " << __PRETTY_FUNCTION__ << " called" << endl;
		}

		Optional(const Optional& other): _defined(other._defined), _value(other._value)
		{
			cout << "Constructor " << __PRETTY_FUNCTION__ << " called" << endl;
		}
		
		~Optional()
		{
			cout << "Destructor " << __PRETTY_FUNCTION__ << " called" << endl;
		}
		
		bool isDefined() const
		{
			return _defined;
		}
		
		void checkDefined() const
		{
			if (!_defined) {
				throw UndefinedError();
			}
		}

		Optional* operator =(const Optional& other)
		{
			_defined = other._defined;
			if (_defined) {
				_value = other._value;
			}
			return this;
		}

		Optional* operator =(const T value)
		{
			_defined = true;
			_value = value;
			return this;
		}
		
		T operator *() const
		{
			checkDefined();
			return _value;
		}
		
		operator T() const
		{
			checkDefined();
			return _value;
		}

		bool operator ==(const Optional& other)
		{
			bool equal = false;
			if (_defined && other._defined) {
				equal = (_value == other._value);
			} else if (!_defined && !other._defined) {
				equal = true;
			}
			return equal;
		}
	
		bool operator !=(const Optional& other) const
		{
			return !(*this == other);
		}
		
		bool operator ==(const T value) const
		{
			return _defined && _value == value;
		}

		bool operator !=(const T value) const
		{
			return !_defined || (_value != value);
		}
		
		friend ostream& operator <<(ostream& out, const Optional& optional)
		{
			if (optional._defined) {
				out << optional._value;
			} else {
				out << "UNDEFINED";
			}
			return out;
		}
		
		static const Optional<T> undefined;

	private:

		Optional(): _defined(false)
		{
				cout << "Constructor " << __PRETTY_FUNCTION__ << " called for undefined" << endl;
		}
		
		bool _defined;
		
		T _value;

	};
	
	template<typename T>
	const Optional<T> Optional<T>::undefined;

}

#endif
