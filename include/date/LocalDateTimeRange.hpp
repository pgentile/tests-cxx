#ifndef DATE_LOCALDATETIMERANGE_HPP
#define DATE_LOCALDATETIMERANGE_HPP

#include <stdint.h>
#include "boost/optional.hpp"

#include "date/LocalDateTime.hpp"


namespace date {

class LocalDateTimeRange
{
    
public:
    
    explicit LocalDateTimeRange(LocalDateTime start);
    
    LocalDateTimeRange(LocalDateTime start, LocalDateTime end);
    
    LocalDateTimeRange(LocalDateTime start, boost::optional<LocalDateTime> end);

    LocalDateTime const& getStart() const {
        return _start;
    }
    
    boost::optional<LocalDateTime> const& getEnd() const {
        return _end;
    }
    
    bool contains(LocalDateTime const& date) const;

private:
    
    LocalDateTime _start;
    
    boost::optional<LocalDateTime> _end;

};

}

#endif
