#include "loader.hpp"
#include "hart.hpp"
#include "shared.hpp"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <fstream>



int ELF_File::init(Hart& hart)
{
    if (load_file())
    {
        std::cerr << "!!! FAILED TO LOAD FILE FROM FILEPATH '" << filepath << "' !!!" << std::endl;
        return 1; 
    }
    if (verify() == false)
    {
        std::cerr << "!!! FAILED TO VERIFY ELF FILE !!!" << std::endl;
        return 1;
    }

    //  extract entry point data
    e_entry = extract_u64(0x18);
    e_flags = extract_u32(0x30);
    e_ehsize = extract_u16(0x34);
    //  program header variables:
    e_phoff = extract_u64(0x20);
    e_phnum = extract_u16(0x38);
    e_phentsize = extract_u16(0x36);
    //  section header variables:
    e_shoff = extract_u64(0x28);
    e_shentsize = extract_u16(0x3a);
    e_shnum = extract_u16(0x3c);

    
    //  extract program headers
    //  begins at e_phoff
    for (int ph_idx = 0; ph_idx < e_phnum; ++ph_idx)
    {
        u64 base_offset = e_phoff + (e_phentsize * ph_idx);
        ProgramHeader ph{};
        ph.p_type = extract_u32(base_offset);
        if (ph.p_type != 0x1) continue; //  1 = load, so if not then don't load
        ph.p_flags = extract_u32(base_offset+0x4);
        ph.p_offset = extract_u64(base_offset+0x8);
        ph.p_vaddr = extract_u64(base_offset+0x10);
        ph.p_paddr = extract_u64(base_offset+0x18);
        ph.p_filesz = extract_u64(base_offset+0x20);
        ph.p_memsz = extract_u64(base_offset+0x28);
        ph.p_align = extract_u64(base_offset+0x30);

        //  verify bounds are correct otherwise return 1:
        if (ph.p_paddr < MEM_START_ADDR) return 1;
        if (ph.p_paddr >= MEM_MAX_ADDR ) return 1;

        if (ph.p_paddr + ph.p_memsz-1 < MEM_START_ADDR) return 1;
        if (ph.p_paddr + ph.p_memsz-1 >= MEM_MAX_ADDR) return 1;

        if (ph.p_offset >= buffer.size()) return 1;
        if (ph.p_offset+ ph.p_filesz -1 >= buffer.size()) return 1;

        //  copy p_filesz bytes starting from p_offset into memory starting at 
        //  address p_paddr
        u64 memory_addr = hart.to_index(ph.p_paddr);
        std::copy(
            buffer.begin()+ph.p_offset,
            buffer.begin()+ph.p_offset+ph.p_filesz,
            hart.memory.begin() + memory_addr
        );
        // zero out memory
        std::fill(
            hart.memory.begin() + memory_addr + ph.p_filesz,
            hart.memory.begin() + memory_addr + ph.p_memsz,
            0
        );

    } 
    //  extract section headers
    /*
        e_shoff = start 
        e_shentsize = size
        e_shnum = count
    */
    for(u64 sh_idx=0; sh_idx <e_shnum; ++sh_idx)
    {
        u64 base_addr = e_shoff + (e_shentsize*sh_idx);
        //  sh_name skipped
        u32 sh_type = extract_u32(base_addr+0x4);
        if (sh_type != 0x2) continue;
        //  sh_flags skipped
        // u64 sh_addr = extract_u64(base_addr + 0x10); // address of section in mem
        u64 sh_offset = extract_u64(base_addr+0x18); // offset of section in FILE
        u64 sh_size = extract_u64(base_addr+0x20); // section size in bytes
        // entry # that stores the string table 
        u32 sh_link = extract_u32(base_addr+0x28); 
        // u64 string_zxtable_base_addr = e_shoff + (e_shentsize * sh_link);
        //  sh_info skipped
        //  sh_addralign skipped
        u64 sh_entsize = extract_u64(base_addr+0x38);
        //  symbol table found
        u64 symbol_count = sh_size / sh_entsize;

        //  get base address of string table:
        u64 str_table_base_addr = e_shoff + (e_shentsize*sh_link);
        u64 str_table_sh_offset = extract_u64(str_table_base_addr+0x18);

        //  iterate over SYMBOL TABLE entries:
        for(u64 st_idx = 0; st_idx <symbol_count; ++st_idx)
        {
            u64 symbol_base_addr = sh_offset + (st_idx * sh_entsize);
            u32 st_name = extract_u32(symbol_base_addr);
            // u8 st_info = extract_u8(symbol_base_addr+0x4);
            u64 st_value = extract_u64(symbol_base_addr+0x8);
            // u64 st_size = extract_u64(symbol_base_addr+0x10);
            u64 symbol_string_ptr = str_table_sh_offset + st_name;
            //  create the string
            std::string name = "";
            while (buffer[symbol_string_ptr] != '\0')
            {
                char c= static_cast<char>(buffer[symbol_string_ptr]);
                name.push_back(c);
                symbol_string_ptr++;
            }
            if (name == "tohost")
            {
                //  we have found our target symbol
                tohost_addr = st_value;
                break;
            }
        }
        break;
    }

    if (tohost_addr == 0)
    {
        std::cerr << "!!! 'tohost' SYMBOL NOT FOUND IN ELF !!!"<< std::endl;
        return 1;
    }

    return 0;
}

int ELF_File::load_file()
{
    if (loaded) return 0;   //  already done
    if (filepath == "") return 1;   //  invalid path, was never set
    //  try opening file
    std::ifstream file(filepath,std::ios::binary);
    if (!file) {return 1;}  //  failed to open
    
    //  get size & reserve
    const auto size = std::filesystem::file_size(filepath);
    buffer.resize(size);
    
    if (size > 0)
    {
        if (!file.read(reinterpret_cast<char*>(buffer.data()),size))
        {
            return 1;   //  fail
        }
    }

    loaded = true;
    return 0;
}

bool ELF_File::verify()
{
    //  ensure file was loaded
    if (!loaded)
    {
        std::cerr << "!!! CANNOT VERIFY ELF IF NOT LOADED !!!" << std::endl;
        return false;
    }
    //  ensure file big enough (at least 64 bytes for the ELF header)
    if (buffer.size() < 64LL)
    {
        std::cerr << "!!! ELF MUST BE AT LEAST 64 BYTES !!!" << std::endl;
        return false;   
    }   
    //  check magic 
    u32 magic = extract_u32(0x0);
    if (magic != 0x464c457f ) 
    {
        std::cerr << "!!! INVALID MAGIC NUMBERS !!!" <<std::endl;
        return false;
    }
    //  ensure class = 2 (offset 0x04)
    if (extract_u8(0x4) != 0x2)
    {
        std::cerr << "!!! INVALID CLASS (MUST BE 0x2) !!!" << std::endl;
        return false;
    }
    //  ensure data =1
    if (extract_u8(0x5) != 0x1)
    {
        std::cerr << "!!! ELF IS NOT LITTLE-ENDIAN !!!"<< std::endl;
        return false;
    }
    //  ensure e_machine = f3 00
    u16 e_machine = extract_u16(0x12);
    if (e_machine != 0x00f3)
    {
        std::cerr << "!!! ELF HAS INVALID e_machine VALUE !!!" << std::endl;
        return false;
    }
    return true;
}

u64 ELF_File::extract_u64(u64 offset )
{
    u64 out = 0;

    //  buffer is in bytes so we extract byte by byte little-endian
    auto start = buffer.begin() + offset;
    for(u64 i = 0; i < 8; ++i)
    {
        out |= static_cast<u64>(static_cast<u8>(*(start + i))) << (i * 8);
    }

    return out;
}

u32 ELF_File::extract_u32(u64 offset )
{
    u64 out = 0;

    //  buffer is in bytes so we extract byte by byte little-endian
    auto start = buffer.begin() + offset;
    for(u64 i = 0; i < 4; ++i)
    {
        out |= static_cast<u64>(static_cast<u8>(*(start + i))) << (i * 8);
    }

    return out;
}

u16 ELF_File::extract_u16(u64 offset )
{
    u64 out = 0;

    //  buffer is in bytes so we extract byte by byte little-endian
    auto start = buffer.begin() + offset;
    for(u64 i = 0; i < 2; ++i)
    {
        out |= static_cast<u64>(static_cast<u8>(*(start + i))) << (i * 8);
    }

    return out;
}

u8 ELF_File::extract_u8(u64 offset )
{
    u64 out = 0;

    //  buffer is in bytes so we extract byte by byte little-endian
    auto start = buffer.begin() + offset;
    for(u64 i = 0; i < 1; ++i)
    {
        out |= static_cast<u64>(static_cast<u8>(*(start + i))) << (i * 8);
    }

    return out;
}