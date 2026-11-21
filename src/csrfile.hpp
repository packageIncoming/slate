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

    // Getters
    u64 get_satp() const { return satp; }
    u64 get_mstatus() const { return mstatus; }
    u64 get_medeleg() const { return medeleg; }
    u64 get_mideleg() const { return mideleg; }
    u64 get_mie() const { return mie; }
    u64 get_mtvec() const { return mtvec; }
    u64 get_mepc() const { return mepc; }
    u64 get_mcause() const { return mcause; }
    u64 get_pmpcfg0() const { return pmpcfg0; }
    u64 get_pmpaddr0() const { return pmpaddr0; }
    u64 get_mhartid() const { return mhartid; }
    u64 get_mnstatus() const { return mnstatus; }

    // Setters
    void set_satp(u64 val) { satp = val; }
    void set_mstatus(u64 val) { mstatus = val; }
    void set_medeleg(u64 val) { medeleg = val; }
    void set_mideleg(u64 val) { mideleg = val; }
    void set_mie(u64 val) { mie = val; }
    void set_mtvec(u64 val) { mtvec = val; }
    void set_mepc(u64 val) { mepc = val; }
    void set_mcause(u64 val) { mcause = val; }
    void set_pmpcfg0(u64 val) { pmpcfg0 = val; }
    void set_pmpaddr0(u64 val) { pmpaddr0 = val; }
    void set_mhartid(u64 val) { mhartid = val; }
    void set_mnstatus(u64 val) { mnstatus = val; }

    //  more general read/write based on 16 bit addr of the CSR:
    u64 read(u16 addr);
    void write(u16 addr, u64 value);
    
};