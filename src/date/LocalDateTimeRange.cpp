#include "date/LocalDateTimeRange.hpp"

#include <stdexcept>
#include <sstream>

using namespace std;
using namespace util;


namespace date {

LocalDateTimeRange::LocalDateTimeRange(LocalDateTime const& start):
    _start(start)
{
}

LocalDateTimeRange::LocalDateTimeRange(LocalDateTime const& start, LocalDateTime const& end):
    _start(start),
    _end(end)
{
    if (end < _start) {
        stringstream message;
        message << end << " est inferieur a " << _start;
        throw invalid_argument(message.str());
    }
}

LocalDateTimeRange::LocalDateTimeRange(LocalDateTime const& start, Optional<LocalDateTime> const& end):
    _start(start),
    _end(end)
{
    if (end && *end < _start) {
        stringstream message;
        message << end << " est inferieur a " << _start;
        throw invalid_argument(message.str());
    }
}

LocalDateTimeRange::LocalDateTimeRange(LocalDateTimeRange const& src):
    _start(src._start),
    _end(src._end)
{
}

LocalDateTimeRange& LocalDateTimeRange::operator =(LocalDateTimeRange const& src) {
    _start = src._start;
    _end = src._end;
    return *this;
}

bool LocalDateTimeRange::contains(LocalDateTime const& date) const {
    return _start <= date && (!_end || date < *_end);
}

void LocalDateTimeRange::close(LocalDateTime const& end) {
    if (_end) {
        stringstream message;
        message << "Deja ferme";
        throw logic_error(message.str());
    }
    if (end < _start) {
        stringstream message;
        message << end << " est inferieur a " << _start;
        throw invalid_argument(message.str());
    }
    _end = end;
}

ostream& operator<<(ostream& out, const LocalDateTimeRange& obj) {
    out << "LocalDateTimeRange(";
    if (obj._end) {
        out << "from " << obj._start << " to " << *(obj._end);
    }
    else {
        out << "starting from " << obj._start;
    }
    out << ')';
    return out;
}

}
