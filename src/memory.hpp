#pragma once
#include "shared.hpp"
#include <exception>
#include <vector>
//  constants:
inline constexpr u64 MEM_SIZE = 1 << 27; // 128 MiB
inline constexpr u64 MEM_START_ADDR = 0x80000000;
inline constexpr u64 MEM_MAX_ADDR = MEM_START_ADDR + MEM_SIZE;  //NOT INCLUSIVE, THIS IS THE FIRST INVALID ADDR

enum class AccessClass:u8
{
    READ = 0, WRITE, 
};

class Memory
{   
    private:
    std::vector<u8> buffer;

    u8 extract8(u64 index);
    u16 extract16(u64 index);
    u32 extract32(u64 index);
    u64 extract64(u64 index);

    u8  read8(u64 ram_addr) {return extract8(to_index(ram_addr));}
    u16 read16(u64 ram_addr){return extract16(to_index(ram_addr));}
    u32 read32(u64 ram_addr){return extract32(to_index(ram_addr));}
    u64 read64(u64 ram_addr){return extract64(to_index(ram_addr));}

    void write8(u64 ram_addr, u8 data);
    void write16(u64 ram_addr, u16 data);
    void write32(u64 ram_addr, u32 data);
    void write64(u64 ram_addr, u64 data);

    public:

    u64 access(u64 ram_addr, u64 width, AccessClass access_class,u64 data=0);
    auto begin() {return buffer.begin();}
    auto end() {return buffer.end();}
    u64 to_index(u64 addr) {return addr - MEM_START_ADDR;}

    Memory();   //  constructor
};