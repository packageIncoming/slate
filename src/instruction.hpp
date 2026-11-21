#pragma once 
#include "shared.hpp"

enum OPCODE : u8
{
    INVALID   = 0b0000000,
    LOAD      = 0b0000011,
    MISC_MEM  = 0b0001111,
    OP_IMM    = 0b0010011,
    AUIPC     = 0b0010111,
    OP_IMM_32 = 0b0011011,
    STORE     = 0b0100011,
    OP        = 0b0110011,
    LUI       = 0b0110111,
    OP_32     = 0b0111011,
    BRANCH    = 0b1100011,
    JALR      = 0b1100111,
    JAL       = 0b1101111,
    SYSTEM    = 0b1110011,
};



struct Instruction 
{
    OPCODE op{INVALID};
    u8 rd{0};
    u8 funct3{0};
    u8 rs1{0};
    u8 rs2{0};
    u32 imm{0};
    u8 funct7{0};
};

namespace Decoder
{

Instruction decode(u32 data);

};
