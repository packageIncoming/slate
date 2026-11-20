#pragma once 
#include "shared.hpp"

struct Instruction 
{
    u8 opcode;

};



Instruction decode(u64 data);
    