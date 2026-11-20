#pragma once
#include "shared.hpp"
#include <string>
#include <vector>

//  forward declarations
class Hart;

//  loads elf files into a helper class

struct ProgramHeader
{
    u32 p_type{0};
    u32 p_flags{0};
    u64 p_offset{0};
    u64 p_vaddr{0};
    u64 p_paddr{0};
    u64 p_filesz{0};
    u64 p_memsz{0};
    u64 p_align{0};
};

struct SectionHeader
{
    u64 base_addr{0};
    u32 sh_type {0};
    u64 sh_offset{0};
    u64 sh_size{0};
    u32 sh_link{0}; 
    u64 sh_entsize{0};
};

class ELF_File
{
    private:
    std::vector<u8> buffer; //  stores the raw bytes, little-endian
    std::string filepath;   //  path to file
    std::vector<ProgramHeader> program_headers; 
    std::vector<SectionHeader> section_headers; 


    bool loaded = false;    //  whether the file has been loaded

    //  extracted from ELF header:
    u64 e_entry{};
    u64 e_phoff{};
    u64 e_shoff{};
    u32 e_flags{};
    u16 e_ehsize{};
    u16 e_phentsize{};
    u16 e_phnum{};
    u16 e_shentsize{};
    u16 e_shnum{};
    u64 tohost_addr{0};

    //  loads a file from filepath; ret 0 on success 1 on fail
    int load_file();
    //  verifies if ELF file is ready to be used
    bool verify();         
    //  extracts a value starting at (start +offset) from buffer start
    u64 extract_u64(u64 offset);
    u32 extract_u32(u64 offset);
    u16 extract_u16(u64 offset);
    u8 extract_u8(u64 offset);
 
    
    public:
    //  public entrypoint to initialize; ret 1 if unsuccessful, 0 if successful (instance ready to use)
    int init(); 
    u64 get_tohost_addr() {return tohost_addr;}
    u64 get_entry() {return e_entry;}
    //  constructor
    ELF_File(std::string fp) : filepath(fp) {}
};