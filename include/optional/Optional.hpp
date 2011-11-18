#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include <iostream>
#include <stdexcept>

namespace optional
{
	
	using namespace std;
	
	class UndefinedError: public runtime_error
	{
		
	public:
		
		UndefinedError(void): runtime_error("Not defined")
		{
		}

	};

	template<typename T>
	class Optional
	{
	
	public:

		Optional(const T value): _defined(true), _value(value)
		{
			cout << "Build optional with " << value << endl;
		}

		Optional(const Optional& other): _defined(other._defined), _value(other._value)
		{
		}
		
		bool isDefined(void) const
		{
			return _defined;
		}
		
		void checkDefined(void) const
		{
			if (!_defined)
			{
				throw UndefinedError();
			}
		}

		Optional* operator =(const Optional& other)
		{
			cout << "Affect optional with other optional " << other << endl;
			_defined = other._defined;
			if (_defined) {
				_value = other._value;
			}
			return this;
		}

		Optional* operator =(const T value)
		{
			cout << "Affect optional with " << value << endl;
			_defined = true;
			_value = value;
			return this;
		}
		
		T operator *(void) const
		{
			checkDefined();
			return _value;
		}
		
		operator T(void) const
		{
			checkDefined();
			return _value;
		}

		bool operator ==(const Optional& other)
		{
			bool equal = false;
			if (_defined && other._defined)
			{
				equal = (_value == other._value);
			}
			else if (!_defined && !other._defined)
			{
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
			return !(*this == value);
		}
		
		friend ostream& operator <<(ostream& out, const Optional& optional)
		{
			if (optional._defined)
			{
				out << optional._value;
			}
			else
			{
				out << "UNDEFINED";
			}
			return out;
		}
		
		static const Optional<T> undefined;

	private:

		Optional(void): _defined(false)
		{
			cout << "Undefined optional undefined" << endl;
		}
		
		bool _defined;
		
		T _value;

	};
	
	template<typename T>
	const Optional<T> Optional<T>::undefined;

}

#endif
