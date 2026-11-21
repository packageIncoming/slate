#include "csrfile.hpp"

u64 CSRFile::read(u16 addr)
{
    //  convert and return based on the correct getter
    switch (static_cast<CSR>(addr))
    {
    case CSR::SATP:     return get_satp();
    case CSR::MSTATUS:  return get_mstatus();
    case CSR::MEDELEG:  return get_medeleg();
    case CSR::MIDELEG:  return get_mideleg();
    case CSR::MIE:      return get_mie();
    case CSR::MTVEC:    return get_mtvec();
    case CSR::MEPC:     return get_mepc();
    case CSR::MCAUSE:   return get_mcause();
    case CSR::PMPCFG0:  return get_pmpcfg0();
    case CSR::PMPADDR0: return get_pmpaddr0();
    case CSR::MHARTID:  return get_mhartid();
    case CSR::MNSTATUS: return get_mnstatus();
    default:            return 0; // or handle illegal CSR read
    }
}

void CSRFile::write(u16 addr, u64 val)
{
    switch (static_cast<CSR>(addr))
    {
    case CSR::SATP:     set_satp(val); break;
    case CSR::MSTATUS:  set_mstatus(val); break;
    case CSR::MEDELEG:  set_medeleg(val); break;
    case CSR::MIDELEG:  set_mideleg(val); break;
    case CSR::MIE:      set_mie(val); break;
    case CSR::MTVEC:    set_mtvec(val); break;
    case CSR::MEPC:     set_mepc(val); break;
    case CSR::MCAUSE:   set_mcause(val); break;
    case CSR::PMPCFG0:  set_pmpcfg0(val); break;
    case CSR::PMPADDR0: set_pmpaddr0(val); break;
    case CSR::MHARTID:  break; // read-only in RISC-V spec
    case CSR::MNSTATUS: set_mnstatus(val); break;
    default:            break; // or handle illegal CSR write
    }
}
