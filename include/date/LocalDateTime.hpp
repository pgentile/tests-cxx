#ifndef DATE_LOCALDATETIME_HPP
#define DATE_LOCALDATETIME_HPP

#include <cstdint>


namespace date {

class LocalDateTime final
{
    
public:
    
    explicit LocalDateTime(uint64_t value);
    
    uint64_t getValue() const {
        return _value;
    }

private:
    
    uint64_t _value;

};

}

#endif
