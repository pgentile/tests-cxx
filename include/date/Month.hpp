#ifndef DATE_MONTH_HPP
#define DATE_MONTH_HPP

#include <cstdint>
#include <iostream>
#include <functional>


namespace date {

enum class Month: std::uint32_t
{
    JANUARY = 1,
    FEBRUARY = 2,
    MARCH = 3,
    APRIL = 4,
    MAY = 5,
    JUNE = 6,
    JULY = 7,
    AUGUST = 8,
    SEPTEMBER = 9,
    OCTOBER = 10,
    NOVEMBER = 11,
    DECEMBER = 12
};

}

#endif
