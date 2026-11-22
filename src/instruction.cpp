#include "instruction.hpp"
#include "shared.hpp"

namespace Decoder
{
Instruction decode(u32 data)    //  instr.imm is sign extended to u64
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
        case OPCODE::LOAD:
        case OPCODE::OP_IMM:
        case OPCODE::OP_IMM_32:
        case OPCODE::JALR:
        case OPCODE::MISC_MEM:
        instr.imm = sext(static_cast<u64>(bits(data,20,31)), 12);
        break;
        //  SYSTEM is also I type but not sign extended
        case OPCODE::SYSTEM:
        instr.imm = bits(data,20,31);
        break;
        // S type:
        case OPCODE::STORE:
        {
            u64 raw = static_cast<u64>(bits(data,7,11)) | static_cast<u64>(bits(data,25,31) << 5);
            instr.imm = sext(raw,12);
            break;
        }
        // B type:
        case OPCODE::BRANCH:
        {
            //  get imm[4:1 | 11]
            u64 raw0 = (static_cast<u64>(bits(data, 8, 11)) << 1) | (static_cast<u64>(bits(data, 7, 7)) << 11);
            //  get imm[12:10 | 5]
            u64 raw1 = (static_cast<u64>(bits(data, 25, 30)) << 5) | (static_cast<u64>(bits(data, 31, 31)) << 12);
            instr.imm = sext(raw0 | raw1, 13);
            break;
        }
        // J type:
        case OPCODE::JAL: {
            u64 imm_20    = static_cast<u64>(bits(data, 31, 31)) << 20;
            u64 imm_19_12 = static_cast<u64>(bits(data, 12, 19)) << 12;
            u64 imm_11    = static_cast<u64>(bits(data, 20, 20)) << 11;
            u64 imm_10_1  = static_cast<u64>(bits(data, 21, 30)) << 1;

            instr.imm = sext(imm_20 | imm_19_12 | imm_11 | imm_10_1, 21);
            break;
        }
        // U type:
        case OPCODE::LUI:
        case OPCODE::AUIPC: {
            u64 imm_u = static_cast<u64>(bits(data, 12, 31)) << 12;
            instr.imm = sext(imm_u, 32);
            break;
        }
        default:
            break;
    }
    return instr;
}
};
