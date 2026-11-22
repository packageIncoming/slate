#include "hart.hpp"
#include "instruction.hpp"
#include "memory.hpp"
#include "shared.hpp"
#include <cstring>



int Hart::run()
{
    while (memory.access(tohost_ram_addr, 64, AccessClass::READ) == 0)
    {
        //  fetch pc
        u32 raw_instr = static_cast<u32>(memory.access(pc, 32, AccessClass::READ));
        //  decode it
        Instruction instr = Decoder::decode(raw_instr);
        u64 next_pc = pc+4;
        switch(instr.op)
        {
            case LOAD:
            {
                u64 addr = registers.get(instr.rs1) + instr.imm;
                u64 result = 0;
                switch(instr.funct3)
                {
                    case 0b000: // lb
                    {
                        u64 val =(memory.access(addr, 8, AccessClass::READ));
                        result = sext(val,8);
                        break;
                    }
                    case 0b001: // lh
                    {
                        u64 val =(memory.access(addr, 16, AccessClass::READ));
                        result = sext(val,16);
                        break;
                    }
                    case 0b010: //lw
                    {
                        u64 val =(memory.access(addr, 32, AccessClass::READ));
                        result = sext(val,32);
                        break;
                    }
                    case 0b011:// ld
                    {
                        u64 val =(memory.access(addr, 64, AccessClass::READ));
                        result = sext(val,64);
                        break;
                    }
                    case 0b100: //lbu
                    {
                        u64 val =(memory.access(addr, 8, AccessClass::READ));
                        result = val;
                        break;
                    }
                    case 0b101:// lhu
                    {
                        u64 val =(memory.access(addr, 16, AccessClass::READ));
                        result = val;
                        break;
                    }
                    case 0b110: // lwu
                    {
                        u64 val =(memory.access(addr, 32, AccessClass::READ));
                        result = val;
                        break;
                    }
                    default:
                        //  illegal
                        break;
                }
                //  now update the value 
                registers.set(instr.rd,result);
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
                    default:
                    {
                        //illegal
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
                        u64 val = registers.get(instr.rs1) + instr.imm;
                        registers.set(instr.rd,val);
                        break;
                    }
                    case 0b001: // slli (check bits[31:26] == 0b000000; 6-bit shamt in [25:20])
                    {
                        if (bits(raw_instr, 26, 31) != 0b000000) {
                            // raise illegal instruction exception (cause 2)
                            break;
                        }
                        u8 shamt = bits(raw_instr,20,25);
                        u64 x = registers.get(instr.rs1);
                        registers.set(instr.rd,x << shamt);
                        break;
                    }
                    case 0b010: // slti
                    {
                        i64 x = static_cast<i64>(registers.get(instr.rs1));
                        i64 imm = static_cast<i64>(instr.imm);
                        if (x < imm)
                        {
                            registers.set(instr.rd,1);
                        }
                        else {
                            registers.set(instr.rd,0);
                        }
                        break;
                    }
                    case 0b011: // sltiu
                    {
                        u64 x = registers.get(instr.rs1);
                        u64 imm = instr.imm;
                        if (x < imm)
                        {
                            registers.set(instr.rd,1);
                        }
                        else {
                            registers.set(instr.rd,0);
                        }
                        break;
                    }
                    case 0b100: // xori
                    {
                        u64 x = registers.get(instr.rs1);
                        u64 imm = instr.imm;
                        registers.set(instr.rd, x ^ imm);
                        break;
                    }
                    case 0b101: // srli (bits[31:26] == 0b000000) / srai (bits[31:26] == 0b010000)
                    {
                        unsigned shamt = bits(raw_instr, 20, 25);
                        u64 x = registers.get(instr.rs1);
                        u64 top6 = bits(raw_instr, 26, 31);

                        if (top6 == 0b000000) {
                            // srli
                            registers.set(instr.rd, x >> shamt);
                        } else if (top6 == 0b010000) {
                            // srai
                            registers.set(instr.rd, static_cast<u64>(static_cast<i64>(x) >> shamt));
                        } else {
                            // reserved/unrecognized encoding -> illegal instruction exception (cause 2)
                        }
                        break;
                    }
                    case 0b110: // ori
                    {
                        u64 x = registers.get(instr.rs1);
                        u64 imm = instr.imm;
                        registers.set(instr.rd, x | imm);
                        break;
                    }
                    case 0b111: // andi
                    {
                        u64 x = registers.get(instr.rs1);
                        u64 imm = instr.imm;
                        registers.set(instr.rd, x & imm);
                        break;
                    }
                    default:
                        //illegal
                        break;
                }
                break;
            }
            case AUIPC:
            {
                registers.set(instr.rd ,pc + instr.imm);
                break;
            }
            case OP_IMM_32:
            {
                switch(instr.funct3)
                {
                    case 0b000: // addiw
                    {
                        //  get low 32 of rs1
                        //  get low 32 of imm
                        // add
                        // sign extend
                        u32 x = static_cast<u32>(registers.get(instr.rs1));
                        u32 imm = static_cast<u32>(instr.imm);  // already sign extended
                        u32 res = x+imm;
                        registers.set(instr.rd,sext(static_cast<u64>(res), 32));
                        break;
                    }
                    case 0b001: // slliw (funct7 == 0b0000000; 5-bit shamt in [24:20])
                    {
                        if (instr.funct7 == 0b0000000)
                        {
                            u32 x  = static_cast<u32>(registers.get(instr.rs1));
                            unsigned shamt = bits(raw_instr,20,24);
                            //  lshift x
                            registers.set(
                                instr.rd,
                                sext(static_cast<u64>(x << shamt),32)
                            );
                        }
                        else
                        {
                            //illegal
                        }
                        break;
                    }
                    case 0b101: // srliw (funct7 == 0b0000000) / sraiw (funct7 == 0b0100000)
                    {
                        if (instr.funct7 == 0b0000000)
                        {
                            //srliw
                            u32 x  = static_cast<u32>(registers.get(instr.rs1));
                            unsigned shamt = bits(raw_instr,20,24);
                            //  rshift x
                            registers.set(
                                instr.rd,
                                sext(static_cast<u64>(x >> shamt),32)
                            );
                        }
                        else if (instr.funct7 == 0b0100000)
                        {
                            // sraiw
                            u32 x  = static_cast<u32>(registers.get(instr.rs1));
                            unsigned shamt = bits(raw_instr,20,24);
                            //  arithmetic shift on the 32-bit word
                            i32 res = static_cast<i32>(x) >> shamt;
                            registers.set(
                                instr.rd,
                                static_cast<u64>(static_cast<i64>(res))
                            );
                        }
                        else
                        {
                            //  illegal
                        }
                        break;
                    }
                    default:
                        //illegal
                        break;
                }
                break;
            }
            case STORE:
            {
                u64 imm = instr.imm;
                u64 addr = imm + registers.get(instr.rs1);
                switch(instr.funct3)
                {
                    case 0b000: // sb
                    {
                        memory.access(addr, 8, AccessClass::WRITE,registers.get(instr.rs2));
                        break;
                    }
                    case 0b001: // sh
                    {
                        memory.access(addr, 16, AccessClass::WRITE,registers.get(instr.rs2));
                        break;
                    }
                    case 0b010: // sw
                    {
                        memory.access(addr, 32, AccessClass::WRITE,registers.get(instr.rs2));
                        break;
                    }
                    case 0b011: // sd
                    {
                        memory.access(addr, 64, AccessClass::WRITE,registers.get(instr.rs2));
                        break;
                    }
                    default:
                        // illegal
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
                        u64 x = registers.get(instr.rs1);
                        u64 y = registers.get(instr.rs2);
                        if (instr.funct7 == 0b0000000) {
                            // add
                            registers.set(instr.rd, x + y);
                        } else if (instr.funct7 == 0b0100000) {
                            // sub
                            registers.set(instr.rd, x - y);
                        } else {
                            // illegal instruction exception (cause 2)
                        }
                        break;
                    }
                    case 0b001: // sll
                    {
                        if (instr.funct7 != 0b0000000) {
                            // illegal instruction exception (cause 2)
                            break;
                        }
                        u8 shamt = registers.get(instr.rs2) & 0b111111;
                        registers.set(instr.rd, registers.get(instr.rs1) << shamt);
                        break;
                    }
                    case 0b010: // slt
                    {
                        if (instr.funct7 != 0b0000000) {
                            // illegal instruction exception (cause 2)
                            break;
                        }
                        i64 x = static_cast<i64>(registers.get(instr.rs1));
                        i64 y = static_cast<i64>(registers.get(instr.rs2));
                        registers.set(instr.rd, x < y ? 1 : 0);
                        break;
                    }
                    case 0b011: // sltu
                    {
                        if (instr.funct7 != 0b0000000) {
                            // illegal instruction exception (cause 2)
                            break;
                        }
                        u64 x = registers.get(instr.rs1);
                        u64 y = registers.get(instr.rs2);
                        registers.set(instr.rd, x < y ? 1 : 0);
                        break;
                    }
                    case 0b100: // xor
                    {
                        if (instr.funct7 != 0b0000000) {
                            // illegal instruction exception (cause 2)
                            break;
                        }
                        registers.set(instr.rd, registers.get(instr.rs1) ^ registers.get(instr.rs2));
                        break;
                    }
                    case 0b101: // srl (funct7 == 0b0000000) / sra (funct7 == 0b0100000)
                    {
                        u64 x = registers.get(instr.rs1);
                        u8 shamt = registers.get(instr.rs2) & 0b111111;
                        if (instr.funct7 == 0b0000000) {
                            // srl
                            registers.set(instr.rd, x >> shamt);
                        } else if (instr.funct7 == 0b0100000) {
                            // sra
                            registers.set(instr.rd, static_cast<u64>(static_cast<i64>(x) >> shamt));
                        } else {
                            // illegal instruction exception (cause 2)
                        }
                        break;
                    }
                    case 0b110: // or
                    {
                        if (instr.funct7 != 0b0000000) {
                            // illegal instruction exception (cause 2)
                            break;
                        }
                        registers.set(instr.rd, registers.get(instr.rs1) | registers.get(instr.rs2));
                        break;
                    }
                    case 0b111: // and
                    {
                        if (instr.funct7 != 0b0000000) {
                            // illegal instruction exception (cause 2)
                            break;
                        }
                        registers.set(instr.rd, registers.get(instr.rs1) & registers.get(instr.rs2));
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case LUI:
            {
                registers.set(instr.rd , instr.imm);
                break;
            }
            case OP_32:
            {
                switch(instr.funct3)
                {
                    case 0b000: // addw (funct7 == 0b0000000) / subw (funct7 == 0b0100000)
                    {
                        if (instr.funct7 == 0b0000000)
                        {
                            //  addw
                            u32 x1 = static_cast<u32>(registers.get(instr.rs1));
                            u32 x2 = static_cast<u32>(registers.get(instr.rs2));
                            registers.set(
                                instr.rd,
                                sext(static_cast<u64>(x1 + x2),32)
                            );
                        }
                        else if (instr.funct7 == 0b0100000)
                        {
                            //subw
                            u32 x1 = static_cast<u32>(registers.get(instr.rs1));
                            u32 x2 = static_cast<u32>(registers.get(instr.rs2));
                            registers.set(
                                instr.rd,
                                sext(static_cast<u64>(x1 - x2),32)
                            );
                        }
                        else
                        {
                            //  illegal
                        }
                        break;
                    }
                    case 0b001: // sllw
                    {
                        if (instr.funct7 == 0b0000000)
                        {
                            u32 x = static_cast<u32>(registers.get(instr.rs1));
                            u8 shamt = registers.get(instr.rs2) & 0b11111;
                            x = x << shamt;
                            registers.set(
                                instr.rd,
                                sext(static_cast<u64>(x),32)
                            );
                        }
                        else
                        {
                            //  illegal
                        }

                        break;
                    }
                    case 0b101: // srlw (funct7 == 0b0000000) / sraw (funct7 == 0b0100000)
                    {
                        u32 x = static_cast<u32>(registers.get(instr.rs1));
                        u8 shamt = registers.get(instr.rs2) & 0b11111;

                        if (instr.funct7 == 0b0000000) {
                            // srlw
                            registers.set(instr.rd, sext(static_cast<u64>(x >> shamt), 32));
                        } else if (instr.funct7 == 0b0100000) {
                            // sraw
                            i32 res = static_cast<i32>(x) >> shamt;
                            registers.set(instr.rd, static_cast<u64>(static_cast<i64>(res)));
                        } else {
                            // illegal instruction exception (cause 2)
                        }
                        break;
                    }
                    default:
                        //  illegal
                        break;
                }
                break;
            }
            case BRANCH:
            {
                u64 true_pc = pc + instr.imm;
                if (true_pc % 4 != 0)
                {
                    //  misaligned, illegal
                    pc+=4;
                    continue;
                }
                switch(instr.funct3)
                {
                    case 0b000: // beq
                    {
                        if (registers.get(instr.rs1) == registers.get(instr.rs2))
                        {
                            next_pc = true_pc;
                        }
                        break;
                    }
                    case 0b001: // bne
                    {
                        if (registers.get(instr.rs1) != registers.get(instr.rs2))
                        {
                            next_pc = true_pc;
                        }
                        break;
                    }
                    case 0b100: // blt
                    {
                        if (static_cast<i64>(registers.get(instr.rs1)) < static_cast<i64>(registers.get(instr.rs2)))
                        {
                            next_pc = true_pc;
                        }
                        break;
                    }
                    case 0b101: // bge
                    {
                        if (static_cast<i64>(registers.get(instr.rs1)) >= static_cast<i64>(registers.get(instr.rs2)))
                        {
                            next_pc = true_pc;
                        }
                        break;
                    }
                    case 0b110: // bltu
                    {
                        if (registers.get(instr.rs1) < registers.get(instr.rs2))
                        {
                            next_pc = true_pc;
                        }
                        break;
                    }
                    case 0b111: // bgeu
                    {
                        if (registers.get(instr.rs1) >= registers.get(instr.rs2))
                        {
                            next_pc = true_pc;
                        }
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
                        u64 target = (registers.get(instr.rs1) + instr.imm) & ~u64{1};
                        if (target % 4 != 0)
                        {
                            //  illegal, misaligned target

                        }
                        registers.set(instr.rd,pc+4);
                        next_pc = target;
                    }
                    default:
                        // illegal
                        break;
                }
                break;
            }
            case JAL:
            {
                registers.set(instr.rd,pc+4);
                if ((pc+ instr.imm) % 4 != 0)
                {
                    //  illegal, misaligned target
                }
                next_pc = pc + instr.imm;
            }
            case SYSTEM:
            {

                switch(instr.funct3)
                {
                    case 0b000: // ecall (0x00000073) / mret (0x30200073)
                    {
                        if (raw_instr == 0x00000073)
                        {
                            //  ecall
                        }
                        else if (raw_instr == 0x30200073)
                        {
                            //  mret
                        }
                        else 
                        {
                            //  illegal

                        }
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
            pc = next_pc;
        }

    }
    return memory.access(tohost_ram_addr, 64, AccessClass::READ) >> 1 ;
}