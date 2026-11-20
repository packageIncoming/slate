#include "memory.hpp"


u64 Memory::extract64(u64 offset )
{
    u64 out = 0;
    auto start = buffer.begin() + offset;
    for(u64 i = 0; i < 8; ++i)
    {
        out |= static_cast<u64>(static_cast<u8>(*(start + i))) << (i * 8);
    }
    return out;
}

u32 Memory::extract32(u64 offset )
{
    u64 out = 0;
    auto start = buffer.begin() + offset;
    for(u64 i = 0; i < 4; ++i)
    {
        out |= static_cast<u64>(static_cast<u8>(*(start + i))) << (i * 8);
    }
    return out;
}

u16 Memory::extract16(u64 offset )
{
    u64 out = 0;
    auto start = buffer.begin() + offset;
    for(u64 i = 0; i < 2; ++i)
    {
        out |= static_cast<u64>(static_cast<u8>(*(start + i))) << (i * 8);
    }
    return out;
}

u8 Memory::extract8(u64 offset )
{
    u64 out = 0;
    auto start = buffer.begin() + offset;
    for(u64 i = 0; i < 1; ++i)
    {
        out |= static_cast<u64>(static_cast<u8>(*(start + i))) << (i * 8);
    }
    return out;
}


Memory::Memory()
{
    buffer = std::vector<u8>(MEM_SIZE,0x0);
}