#include "date/LocalDateTimeRange.hpp"

using boost::optional;


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

LocalDateTimeRange::LocalDateTimeRange(LocalDateTime start, optional<LocalDateTime> end):
    _start(start),
    _end(end)
{
}

bool LocalDateTimeRange::contains(LocalDateTime const& date) const {
    return false; // FIXME
}

}
