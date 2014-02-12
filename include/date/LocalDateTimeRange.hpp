#ifndef DATE_LOCALDATETIMERANGE_HPP
#define DATE_LOCALDATETIMERANGE_HPP

#include "boost/optional.hpp"

#include "util/Optional.hpp"
#include "date/LocalDateTime.hpp"


namespace date {

class LocalDateTimeRange final
{
    
public:
    
    explicit LocalDateTimeRange(LocalDateTime start);
    
    LocalDateTimeRange(LocalDateTime start, LocalDateTime end);
    
    LocalDateTimeRange(LocalDateTime start, util::Optional<LocalDateTime> end);

    LocalDateTime const& getStart() const {
        return _start;
    }
    
    util::Optional<LocalDateTime> const& getEnd() const {
        return _end;
    }
    
    bool contains(LocalDateTime const& date) const;

private:
    
    LocalDateTime _start;
    
    util::Optional<LocalDateTime> _end;

};

}

#endif
