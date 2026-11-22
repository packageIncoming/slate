#include "memory.hpp"
#include <iostream>
#include <stdexcept>


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

void Memory::write8(u64 ram_addr, u8 data)
{
    //  write 8 bits at a time:
    u64 base_index = to_index(ram_addr);
    for(int i =0; i < 1; ++i)
    {
        buffer[base_index+i] = data & 0b11111111;
        data = data >> 8;
    }
}
void Memory::write16(u64 ram_addr, u16 data)
{
    //  write 8 bits at a time:
    u64 base_index = to_index(ram_addr);
    for(int i =0; i < 2; ++i)
    {
        buffer[base_index+i] = data & 0b11111111;
        data = data >> 8;
    }
}

void Memory::write32(u64 ram_addr, u32 data)
{
    //  write 8 bits at a time:
    u64 base_index = to_index(ram_addr);
    for(int i =0; i < 4; ++i)
    {
        buffer[base_index+i] = data & 0b11111111;
        data = data >> 8;
    }
}   

void Memory::write64(u64 ram_addr, u64 data)
{
    //  write 8 bits at a time:
    u64 base_index = to_index(ram_addr);
    for(int i =0; i < 8; ++i)
    {
        buffer[base_index+i] = data & 0b11111111;
        data = data >> 8;
    }
}


u64 Memory::access(u64 ram_addr, u64 width, MemAccessClass access_class,u64 data)
{
    //  verify in bounds
    if (ram_addr < MEM_START_ADDR || ram_addr >= MEM_MAX_ADDR || (to_index(ram_addr) + width / 8 > MEM_SIZE))
    {
        std::cerr << "!!! ACCESSING OUT OF BOUNDS MEMORY !!!\n";
        throw MemFault{};
        return 0;
    }   
    if (width != 8u && width != 16u && width != 32u && width != 64u)
    {
        std::cerr << "!!! INVALID ACCESS WIDTH !!!\n";
        throw MemFault{};
        return 0; 
    }
    if (access_class == MemAccessClass::READ)
    {
        switch (width)
        {
            case 8u: return read8(ram_addr);
            case 16u: return read16(ram_addr);
            case 32u: return read32(ram_addr);
            case 64u: return read64(ram_addr);
            default: throw std::logic_error("invalid width");
        }
    }
    else {
        switch(width)
        {
            case 8u:  write8(ram_addr,  static_cast<u8>(data)); return 0;
            case 16u: write16(ram_addr, static_cast<u16>(data)); return 0;
            case 32u: write32(ram_addr, static_cast<u32>(data)); return 0;
            case 64u: write64(ram_addr, data); return 0;
            default: throw std::logic_error("invalid width");
        }
    }
}


Memory::Memory()
{
    buffer = std::vector<u8>(MEM_SIZE,0x0);
}