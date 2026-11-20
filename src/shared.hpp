#pragma once
#include <concepts>
#include <cstdint>
#include <limits>

//  aliases:
using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

//  utility functions

//  sign extend data  into new width of T, original of width data_width 
template<std::unsigned_integral T>
constexpr T sext(T data, unsigned  data_width)
{
    constexpr unsigned  new_width = static_cast<T>(std::numeric_limits<T>::digits);
    if (data_width >= new_width) return data;   //  cannot extend further
    return (
        data >> (data_width-1) & T{1} ?
            data | ~((T{1} << data_width) -1 ) :
            data
        );

}

// extract bits lo to hi, inclusive both ends, zero indexed
template<std::unsigned_integral T>
constexpr T bits(T data, unsigned lo, unsigned hi)
{
    constexpr unsigned  maxbits = static_cast<T>(std::numeric_limits<T>::digits)-1;
    if (hi<lo || hi > maxbits) return T{0};  //  invalid
    if (hi == maxbits) return data;
    return (data >> lo) & ((T{1} << (hi-lo+1)) - 1);
}