#ifndef DATE_LOCALDATE_HPP
#define DATE_LOCALDATE_HPP

#include <cstdint>
#include <iostream>


namespace date {

class LocalDate final
{
    
public:
    
    explicit LocalDate(uint32_t value);
    
    LocalDate(uint32_t year, uint32_t month, uint32_t day);
    
    uint32_t year() const {
        return _value / 10000U;
    }
    
    uint32_t month() const {
        return (_value / 100U) % 100U;
    }
    
    uint32_t day() const {
        return _value % 100U;
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

#endif
