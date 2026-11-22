#include "hart.hpp"
#include "instruction.hpp"
#include "memory.hpp"
#include <cstring>

int Hart::run()
{
    while (memory.access(tohost_ram_addr, 64, AccessClass::READ) != 0)
    {
        //  fetch pc
        u32 raw_instr = static_cast<u32>(memory.access(pc, 32, AccessClass::READ));
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
                switch(instr.funct3)
                {
                    case 0b000: // fence
                    {
                        break;
                    }
                    case 0b001: // fence.i
                    {
                        break;
                    }
                }
                break;
            }
            case OP_IMM:
            {
                switch(instr.funct3)
                {
                    case 0b000: // addi
                    {
                        break;
                    }
                    case 0b001: // slli (check bits[31:26] == 0b000000; 6-bit shamt in [25:20])
                    {
                        break;
                    }
                    case 0b010: // slti
                    {
                        break;
                    }
                    case 0b011: // sltiu
                    {
                        break;
                    }
                    case 0b100: // xori
                    {
                        break;
                    }
                    case 0b101: // srli (bits[31:26] == 0b000000) / srai (bits[31:26] == 0b010000)
                    {
                        break;
                    }
                    case 0b110: // ori
                    {
                        break;
                    }
                    case 0b111: // andi
                    {
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case AUIPC:
            {
                break;
            }
            case OP_IMM_32:
            {
                switch(instr.funct3)
                {
                    case 0b000: // addiw
                    {
                        break;
                    }
                    case 0b001: // slliw (funct7 == 0b0000000; 5-bit shamt in [24:20])
                    {
                        break;
                    }
                    case 0b101: // srliw (funct7 == 0b0000000) / sraiw (funct7 == 0b0100000)
                    {
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case STORE:
            {
                switch(instr.funct3)
                {
                    case 0b000: // sb
                    {
                        break;
                    }
                    case 0b001: // sh
                    {
                        break;
                    }
                    case 0b010: // sw
                    {
                        break;
                    }
                    case 0b011: // sd
                    {
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case OP:
            {
                switch(instr.funct3)
                {
                    case 0b000: // add (funct7 == 0b0000000) / sub (funct7 == 0b0100000)
                    {
                        break;
                    }
                    case 0b001: // sll
                    {
                        break;
                    }
                    case 0b010: // slt
                    {
                        break;
                    }
                    case 0b011: // sltu
                    {
                        break;
                    }
                    case 0b100: // xor
                    {
                        break;
                    }
                    case 0b101: // srl (funct7 == 0b0000000) / sra (funct7 == 0b0100000)
                    {
                        break;
                    }
                    case 0b110: // or
                    {
                        break;
                    }
                    case 0b111: // and
                    {
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case LUI:
            {
                break;
            }
            case OP_32:
            {
                switch(instr.funct3)
                {
                    case 0b000: // addw (funct7 == 0b0000000) / subw (funct7 == 0b0100000)
                    {
                        break;
                    }
                    case 0b001: // sllw
                    {
                        break;
                    }
                    case 0b101: // srlw (funct7 == 0b0000000) / sraw (funct7 == 0b0100000)
                    {
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case BRANCH:
            {
                switch(instr.funct3)
                {
                    case 0b000: // beq
                    {
                        break;
                    }
                    case 0b001: // bne
                    {
                        break;
                    }
                    case 0b100: // blt
                    {
                        break;
                    }
                    case 0b101: // bge
                    {
                        break;
                    }
                    case 0b110: // bltu
                    {
                        break;
                    }
                    case 0b111: // bgeu
                    {
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case JALR:
            {
                switch(instr.funct3)
                {
                    case 0b000: // jalr
                    {
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case JAL:
            {
                break;
            }
            case SYSTEM:
            {
                switch(instr.funct3)
                {
                    case 0b000: // ecall (0x00000073) / mret (0x30200073)
                    {
                        break;
                    }
                    case 0b001: // csrrw
                    {
                        break;
                    }
                    case 0b010: // csrrs
                    {
                        break;
                    }
                    case 0b011: // csrrc
                    {
                        break;
                    }
                    case 0b101: // csrrwi
                    {
                        break;
                    }
                    case 0b110: // csrrsi
                    {
                        break;
                    }
                    case 0b111: // csrrci
                    {
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            default:
            {
                //  unrecognized opcode
                break;
            }
        }

        //  execute it
    }
    return 0;
}