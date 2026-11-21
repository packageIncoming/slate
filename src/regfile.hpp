#pragma once
#include "shared.hpp"
#include <array>

enum class Register: u8
{
    x0 = 0,
    x1 = 1,
    x2 = 2,
    x3 = 3,
    x4 = 4,
    x5 = 5,
    x6 = 6,
    x7 = 7,
    x8 = 8,
    x9 = 9,
    x10 = 10,
    x11 = 11,
    x12 = 12,
    x13 = 13,
    x14 = 14,
    x15 = 15,
    x16 = 16,
    x17 = 17,
    x18 = 18,
    x19 = 19,
    x20 = 20,
    x21 = 21,
    x22 = 22,
    x23 = 23,
    x24 = 24,
    x25 = 25,
    x26 = 26,
    x27 = 27,
    x28 = 28,
    x29 = 29,
    x30 = 30,
    x31 = 31
};

class RegFile
{
    private:
    std::array<u64,32> registers{0};

    public:
    u64 get(Register r)
    {
        if (r == Register::x0) return 0;
        return registers[static_cast<u8>(r)]; 
    }
    void set(Register r, u64 val)
    {
        if (r == Register::x0) return;
        registers[static_cast<u8>(r)] = val;
    }
};