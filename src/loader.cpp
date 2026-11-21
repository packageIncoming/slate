#include "loader.hpp"
#include "elf.hpp"
#include "hart.hpp"
#include <iostream>


int Loader::load(Hart& hart, ELF_File& elf)
{
    if (!elf.is_ready())
    {
        //  did not load properly or something went wrong, don't load.
        return 1;
    }
    //  elf stores its to_host addr and its e_entry addr
    hart.set_tohost(elf.get_tohost_addr());
    hart.set_pc(elf.get_entry());

    //  iterate over programheaders in ELF & load their segments into
    //  memory of the hart:
    for(const ProgramHeader& ph: elf.get_program_headers())
    {
        //  verify bounds are correct otherwise return 1:
        if (ph.p_paddr < MEM_START_ADDR) return 1;
        if (ph.p_paddr >= MEM_MAX_ADDR ) return 1;

        if (ph.p_paddr + ph.p_memsz-1 < MEM_START_ADDR) return 1;
        if (ph.p_paddr + ph.p_memsz-1 >= MEM_MAX_ADDR) return 1;

        if (ph.p_offset >= elf.size()) return 1;
        if (ph.p_offset+ ph.p_filesz -1 >= elf.size()) return 1;

        //  copy p_filesz bytes starting from p_offset into memory starting at 
        //  address p_paddr
        u64 memory_addr = hart.to_index(ph.p_paddr);
        std::copy(
            elf.buff_begin()+ph.p_offset,
            elf.buff_begin()+ph.p_offset+ph.p_filesz,
            hart.get_mem_begin() + memory_addr
        );
        // zero out memory
        std::fill(
            hart.get_mem_begin() + memory_addr + ph.p_filesz,
            hart.get_mem_begin() + memory_addr + ph.p_memsz,
            0
        );
    }

    std::cout << "done loading :)\n";
    return 0;
}