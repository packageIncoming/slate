#include "hart.hpp"
#include "shared.hpp" 
#include <cstring>

Hart::Hart()
{
    registers.resize(32,0);
    memory.resize(MEM_SIZE,0);
    pc = 0;
    privilege = 0;
}

u64 Hart::mem_addr(u64 ram_addr)
{
    return ram_addr - MEM_START_ADDR;
}

int Hart::run()
{
    return 0;
}