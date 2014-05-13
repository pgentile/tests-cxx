#ifndef DATE_LOCALDATETIMERANGE_HPP
#define DATE_LOCALDATETIMERANGE_HPP

#include "boost/optional.hpp"

#include <iostream>
#include "util/Optional.hpp"
#include "date/LocalDateTime.hpp"


namespace date {

class LocalDateTimeRange final
{
    
public:
    
    explicit LocalDateTimeRange(LocalDateTime const& start);
    
    LocalDateTimeRange(LocalDateTime const& start, LocalDateTime const& end);
    
    LocalDateTimeRange(LocalDateTime const& start, util::Optional<LocalDateTime> const& end);
    
    LocalDateTimeRange(LocalDateTimeRange const& src);
    
    LocalDateTimeRange& operator =(LocalDateTimeRange const& src);
    
    bool operator ==(LocalDateTimeRange const& other) const {
        return _start == other._start && _end == other._end;
    }
    
    bool operator !=(LocalDateTimeRange const& other) const {
        return !(*this == other);
    }

    LocalDateTime const& getStart() const {
        return _start;
    }
    
    util::Optional<LocalDateTime> const& getEnd() const {
        return _end;
    }
    
    bool isClosed() const {
        return (bool) _end;
    }
    
    bool contains(LocalDateTime const& date) const;
    
    void close(LocalDateTime const& end);
    
    friend std::ostream& operator<<(std::ostream& out, const LocalDateTimeRange& obj);

private:
    
    LocalDateTime _start;
    
    util::Optional<LocalDateTime> _end;

};

}

#endif
