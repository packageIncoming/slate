#pragma once
#include "shared.hpp"
enum class CSR: u16
{
    SATP = 0x180,
    MSTATUS = 0x300,
    MEDELEG = 0x302,
    MIDELEG  = 0x303,
    MIE = 0x304,
    MTVEC = 0x305,
    MEPC = 0x341,
    MCAUSE=  0x342,
    PMPCFG0 = 0x3a0,
    PMPADDR0 = 0x3b0,
    MHARTID = 0xf14,
    MNSTATUS = 0x744 
};

class CSRFile
{
    private:
    u64 satp = 0;
    u64 mstatus = 0;
    u64 medeleg = 0;
    u64 mideleg = 0;
    u64 mie = 0;
    u64 mtvec = 0;
    u64 mepc = 0;
    u64 mcause = 0;
    u64 pmpcfg0 = 0;
    u64 pmpaddr0 = 0;
    u64 mhartid = 0;
    u64 mnstatus = 0;
    public:
    
};