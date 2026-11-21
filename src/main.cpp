#include <iostream>
#include "main.hpp"
#include "loader.hpp"
#include "elf.hpp"
#include "hart.hpp"

void print_help(bool error) {
    std::ostream& out = error ? std::cerr : std::cout;
    
    out << "slate: RISC-V Emulator that boots Linux.\n"
        << "Options:\n"
        << "\t--help            : Print this help text\n"
        << "\t--info [filepath] : Get info on ELF file\n" 
        << "\t--run [filepath]  : Run an ELF file\n" 
        ;
}

int main(int argc, char* argv[])
{
    //  parse args
    Args args{};
    if (args.parse(argc,argv))
    {
        std::cerr << "!!! INVALID ARGUMENT PROVIDED !!!" << std::endl;
        print_help(true);
        return 2;
    }
    if (args.print_help)
    {
        print_help(false);
        return 0;
    }

    Hart hart{};

    if (args.info_filepath != "")
    {
        ELF_File elf(args.info_filepath);
        if (!elf.is_ready())
        {
            std::cerr << "!!! FAILED TO INITIALIZE ELF FILE !!!" <<std::endl;
            return 1;
        }
        std::cout << "entry 0x";
        std::cout << std::hex << elf.get_entry() << std::endl;
        std::cout << "tohost 0x";
        std::cout << std::hex << elf.get_tohost_addr() << std::endl;
        return 0;
    }
    if (args.run_filepath != "")
    {
        ELF_File elf(args.run_filepath);
        if (!elf.is_ready())
        {
            std::cerr << "!!! FAILED TO INITIALIZE ELF FILE !!!" <<std::endl;
            return 1;
        }
        //  use loader to load into hart:
        Loader::load(hart,elf);
        hart.run();
    }


    return 0;
}