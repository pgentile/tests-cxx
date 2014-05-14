#include "date/LocalDate.hpp"

#include <iomanip>

#include "util/ScopeGuard.hpp"

using namespace std;


namespace date {

LocalDate::LocalDate(uint32_t value): _value(value)
{
}


LocalDate::LocalDate(uint32_t year, uint32_t month, uint32_t day):
    _value(year * 10000U + month * 100U + day)
{
}


LocalDate::LocalDate(uint32_t year, Month month, uint32_t day):
    LocalDate(year, static_cast<uint32_t>(month), day)
{
}


ostream& operator<<(ostream& out, const LocalDate& obj)
{
    out << setw(4) << setfill('0') << obj.year()
        << '-' << setw(2) << setfill('0') << static_cast<uint32_t>(obj.month())
        << '-' << setw(2) << setfill('0') << obj.day();
    return out;
}

}
