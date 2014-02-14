#include "date/LocalDateTimeRange.hpp"

using namespace util;


namespace date {

LocalDateTimeRange::LocalDateTimeRange(LocalDateTime start):
    _start(start)
{
}

LocalDateTimeRange::LocalDateTimeRange(LocalDateTime start, LocalDateTime end):
    _start(start),
    _end(end)
{
}

LocalDateTimeRange::LocalDateTimeRange(LocalDateTime start, Optional<LocalDateTime> end):
    _start(start),
    _end(end)
{
}

bool LocalDateTimeRange::contains(LocalDateTime const& date) const {
    return _start.getValue() <= date.getValue() && (!_end || date.getValue() < _end->getValue());
}

}
