//  Mert Isik
//  header file defining a hart
//  little-endian all throughout
#pragma once
#include "loader.hpp"
#include "shared.hpp"
#include "memory.hpp"
#include <array>


class Hart 
{
    friend class ELF_File;

    private:
    u64 pc{0};   //  program counter
    std::array<u64,32> registers{};  //  x0 to x31, general purpose registers
    Memory memory{};   //  memory
    //  TODO add CSRs 
    u64 privilege{3};  //  privilege level. 0 = user 1=supervisor 3=machine


    public:
    //  translates a RAM addr to a memory[] addr 
    //  (subtract MEM_START_ADDR from ram to get mem)
    u64 to_index(u64 ram_addr) { return memory.to_index(ram_addr);}
    Hart();
    int run();

};