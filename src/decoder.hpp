#pragma once 
#include "shared.hpp"

enum class OPCODE: u8 
{
    LOAD    = 0b0000011,
    STORE   = 0b0100011,
};



struct Instruction 
{

};



Instruction decode(u64 data);
