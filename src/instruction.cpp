#include "instruction.hpp"
#include "shared.hpp"
#include <iostream>

namespace Decoder
{
Instruction decode(u32 data)
{
    Instruction instr{};

    instr.op = static_cast<OPCODE>(bits(data, 0, 6));
    instr.rd = bits(data,7,11);
    instr.funct3 = bits(data,12,14);
    instr.rs1 = bits(data,15,19);
    instr.rs2 = bits(data,20,24);
    instr.funct7 = bits(data,25,31);

    //  for instructions that encode an imm we have to extract them according to the spec:
    switch(instr.op)
    {
        // I type:
        case LOAD:
        case OP_IMM:
        case OP_IMM_32:
        case JALR:
        case MISC_MEM:
        instr.imm = sext(static_cast<u64>(bits(data,20,31)), 11);
        break;
        //  SYSTEM is also I type but not sign extended
        case SYSTEM:
        instr.imm = bits(data,20,31);
        break;
        // S type:
        case STORE:
        {
            u64 raw = static_cast<u64>(bits(data,7,11)) | static_cast<u64>(bits(data,25,31) << 5);
            instr.imm = sext(raw,11);
            break;
        }
        // B type:
        case BRANCH:
        {
            u64 raw = static_cast<u64>(bits(data,))
        }
        break;
        // J type:
        case JAL:
        break;
        // U type:
        case LUI:
        case AUIPC:
        break;

        default:
            break;
    }
    return instr;
}
};
