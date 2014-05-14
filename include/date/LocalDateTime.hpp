#ifndef DATE_LOCALDATETIME_HPP
#define DATE_LOCALDATETIME_HPP

#include <cstdint>
#include <iostream>
#include <boost/numeric/conversion/cast.hpp>

#include "LocalDate.hpp"
#include "Time.hpp"
#include "Month.hpp"


namespace date {

class LocalDateTime final
{
    
public:
    
    explicit LocalDateTime(uint64_t value);
    
    LocalDateTime(LocalDate const& date, Time const& time);
    
    LocalDateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second);
    
    LocalDateTime(uint32_t year, Month month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second);
    
    uint32_t year() const {
        return boost::numeric_cast<uint32_t>(_value / 10000000000UL);
    }
    
    Month month() const {
        return boost::numeric_cast<Month>((_value / 100000000UL) % 100UL);
    }
    
    uint32_t day() const {
        return boost::numeric_cast<uint32_t>((_value / 1000000UL) % 100UL);
    }
    
    uint32_t hour() const {
        return boost::numeric_cast<uint32_t>((_value / 10000UL) % 100UL);
    }
    
    uint32_t minute() const {
        return boost::numeric_cast<uint32_t>((_value / 100UL) % 100UL);
    }
    
    uint32_t second() const {
        return boost::numeric_cast<uint32_t>(_value % 100UL);
    }
    
    LocalDate date() const {
        return LocalDate(boost::numeric_cast<uint32_t>(_value / 1000000UL));
    }
    
    Time time() const {
        return Time(boost::numeric_cast<uint32_t>(_value % 1000000UL));
    }
    
    bool operator ==(LocalDateTime const& other) const {
        return _value == other._value;
    }
    
    bool operator !=(LocalDateTime const& other) const {
        return _value != other._value;
    }
    
    bool operator <(LocalDateTime const& other) const {
        return _value < other._value;
    }
    
    bool operator >(LocalDateTime const& other) const {
        return _value > other._value;
    }
    
    bool operator <=(LocalDateTime const& other) const {
        return _value <= other._value;
    }
    
    bool operator >=(LocalDateTime const& other) const {
        return _value >= other._value;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const LocalDateTime& obj);
    
private:
    
    uint64_t _value;

};

}

#endif
