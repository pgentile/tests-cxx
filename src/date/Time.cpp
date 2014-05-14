#include "date/Time.hpp"

#include <iomanip>

#include "util/ScopeGuard.hpp"

using namespace std;


namespace date {


Time const Time::MIDNIGHT(0);


Time::Time(uint32_t value): _value(value)
{
}


Time::Time(uint32_t hour, uint32_t minute, uint32_t second):
    _value(hour * 10000U + minute * 100U + second)
{
    
}


ostream& operator<<(ostream& out, const Time& obj)
{
    out << setw(2) << setfill('0') << obj.hour()
        << ':' << setw(2) << setfill('0') << obj.minute()
        << ':' << setw(2) << setfill('0') << obj.second();
    return out;
}

}
