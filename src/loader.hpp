#pragma once
#include "elf.hpp"
#include "shared.hpp"


namespace Loader
{
    struct LoaderResult
    {
        u8 valid{0};
        u64 entry_addr{0};
        u64 tohost_addr{0};
    };

    LoaderResult load_elf(ELF_File elf);

    int load()
}