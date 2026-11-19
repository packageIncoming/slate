//  Mert Isik
//  header file defining a hart
//  little-endian all throughout
#pragma once
#include "loader.hpp"
#include "shared.hpp"
//  constants:
inline constexpr u64 MEM_SIZE = 1 << 27; // 128 MiB
inline constexpr u64 MEM_START_ADDR = 0x80000000;
inline constexpr u64 MEM_MAX_ADDR = MEM_START_ADDR + MEM_SIZE;  //NOT INCLUSIVE, THIS IS THE FIRST INVALID ADDR

class Hart 
{
    friend class ELF_File;

    private:
    u64 pc{0};   //  program counter
    std::vector<u64> registers;  //  x0 to x31, general purpose registers
    std::vector<u64> memory;   //  memory
    //  TODO add CSRs 
    u64 privilege{0};  //  privilege level. 0 = user 1=supervisor 3=machine


    public:
    //  translates a RAM addr to a memory[] addr 
    //  (subtract MEM_START_ADDR from ram to get mem)
    u64 mem_addr(u64 ram_addr);
    Hart();
    int run();

};