#pragma once 
#include "shared.hpp"



enum OPCODE: u8
{

};



struct Instruction 
{
    OPCODE op;
    
};


namespace Decoder
{


Instruction decode(u64 data);

};
