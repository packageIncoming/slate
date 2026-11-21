#pragma once
#include "elf.hpp"


namespace Loader
{
    int load(Hart& hart, ELF_File& elf);
}