#pragma once 
#include "shared.hpp"
namespace Decoder
{

struct Instruction 
{
    u8 opcode;
    
};

Instruction decode(u64 data);

};
