#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP

#include <cstdint>
#include <iostream>
#include <functional>


namespace date {

class Time final
{
    
public:
    
    static Time const MIDNIGHT;
    
    explicit Time(uint32_t value);
    
    Time(uint32_t hour, uint32_t minute, uint32_t second);
    
    uint32_t hour() const {
        return (_value / 10000U) % 100U;
    }
    
    uint32_t minute() const {
        return (_value / 100U) % 100U;
    }
    
    uint32_t second() const {
        return _value % 100U;
    }
    
    uint32_t numRepr() const {
        return _value;
    }
    
    bool operator ==(Time const& other) const {
        return _value == other._value;
    }
    
    bool operator !=(Time const& other) const {
        return _value != other._value;
    }
    
    bool operator <(Time const& other) const {
        return _value < other._value;
    }
    
    bool operator >(Time const& other) const {
        return _value > other._value;
    }
    
    bool operator <=(Time const& other) const {
        return _value <= other._value;
    }
    
    bool operator >=(Time const& other) const {
        return _value >= other._value;
    }
    
private:
    
    uint32_t _value;

};

}


namespace std {
    
    template<>
    struct hash<date::Time> {

        size_t operator()(date::Time const& date) {
            return date.numRepr();
        }

    };

}

#endif
