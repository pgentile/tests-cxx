#ifndef DATE_LOCALDATE_HPP
#define DATE_LOCALDATE_HPP

#include <cstdint>
#include <iostream>
#include <functional>
#include <boost/numeric/conversion/cast.hpp>

#include "Month.hpp"


namespace date {

class LocalDate final
{
    
public:
    
    explicit LocalDate(uint32_t value);
    
    LocalDate(uint32_t year, uint32_t month, uint32_t day);
    
    LocalDate(uint32_t year, Month month, uint32_t day);
    
    uint32_t year() const {
        return _value / 10000U;
    }
    
    Month month() const {
        return boost::numeric_cast<Month>((_value / 100U) % 100U);
    }
    
    uint32_t day() const {
        return _value % 100U;
    }
    
    explicit operator uint32_t() const {
        return _value;
    }
    
    bool operator ==(LocalDate const& other) const {
        return _value == other._value;
    }
    
    bool operator !=(LocalDate const& other) const {
        return _value != other._value;
    }
    
    bool operator <(LocalDate const& other) const {
        return _value < other._value;
    }
    
    bool operator >(LocalDate const& other) const {
        return _value > other._value;
    }
    
    bool operator <=(LocalDate const& other) const {
        return _value <= other._value;
    }
    
    bool operator >=(LocalDate const& other) const {
        return _value >= other._value;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const LocalDate& obj);

private:
    
    uint32_t _value;

};

}


namespace std {
    
    template<>
    struct hash<date::LocalDate> {

        size_t operator()(date::LocalDate const& date) {
            return static_cast<uint32_t>(date);
        }

    };

}

#endif
