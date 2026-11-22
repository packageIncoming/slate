//  Mert Isik
//  header file defining a hart
//  little-endian all throughout
#pragma once
#include "csrfile.hpp"
#include "regfile.hpp"
#include "shared.hpp"
#include "memory.hpp"

enum class AccessType
{
    LOAD, STORE, FETCH
};

class Hart 
{
    friend class ELF_File;

    private:
    u64 pc{0};   //  program counter
    u64 tohost_ram_addr{0};
    RegFile registers{};
    Memory memory{};   //  memory
    CSRFile csr{};
    u8 privilege{3};  //  privilege level. 0 = user 1=supervisor 3=machine
    
    void trap(u64 cause, u64 tval);

    public:
    //  getters and setters:
    void set_pc(u64 val){ pc = val;}
    u64 get_pc() {return pc;}

    auto get_mem_begin(){return memory.begin();}
    auto get_mem_end() {return memory.end();}

    void set_tohost(u64 addr) {tohost_ram_addr = addr;}
    u64 get_tohost() {return tohost_ram_addr;}

    //  translates a RAM addr to a memory[] addr 
    //  (subtract MEM_START_ADDR from ram to get mem)
    u64 to_index(u64 ram_addr) { return memory.to_index(ram_addr);}

    //  load/store/fetch:
    u64 access(u64 addr, u64 width, AccessType type, u64 data =0 );

    Hart() {};
    int run();

};