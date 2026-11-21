#include "hart.hpp"
#include "instruction.hpp"
#include <cstring>

int Hart::run()
{
    while (memory.read64(pc) == 0)
    {
        //  fetch pc
        u32 raw_instr = memory.read32(pc);
        //  decode it
        Instruction instr = Decoder::decode(raw_instr);

        switch(instr.op)
        {

            case LOAD:
            {
                switch(instr.funct3)
                {
                    case 0b000: // lb
                    {
                        break;
                    }
                    case 0b001: // lh
                    {
                        break;
                    }
                    case 0b010: //lw
                    {
                        break;
                    }
                    case 0b011:// ld
                    {
                        break;
                    }
                    case 0b100: //lbu
                    {
                        break;
                    }
                    case 0b101:// lhu
                    {
                        break;
                    }
                    case 0b110: // lwu
                    {
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case MISC_MEM:
            {
                break;
            }
            case OP_IMM:
            {
                break;
            }
            case AUIPC:
            {
                break;
            }
            case OP_IMM_32:
            {
                break;
            }
            case STORE:
            {
                break;
            }
            case OP:
            {
                break;
            }
            case LUI:
            {
                break;
            }
            case OP_32:
            {
                break;
            }
            case BRANCH:
            {
                break;
            }
            case JALR:
            {
                break;
            }
            case JAL:
            {
                break;
            }
            case SYSTEM:
            {
                break;
            }
            default:
                //failed
                break;
        }

        //  execute it
    }
    return 0;
}