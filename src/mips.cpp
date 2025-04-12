#include <beyond_all_repair.h>

namespace beyond_all_repair
{

enum class instr_type
{
    reg_rd_rs_rt,
    reg_rd_rt_rs,
    reg_rs_rt,
    reg_rd_rs,
    reg_rd_rt,
    reg_rt_rd,
    reg_rd,
    reg_rs,
    shift,

    trap,

    imm_signed,
    imm_unsigned,
    imm_rs,

    store,
    store_float,
    store_cop2,

    load,
    load_float,
    load_cop2,

    branch_rs_rt,
    branch_rs,
    branch_reg,
    jump,

    bit_op,

    float_rt_fs,
    float_fd_fs,
    float_fd_fs_ft,
    float_fs_ft,
    branch_cop_cond,

    cop2_rt_rs,

    implicit,

    // note: not a real fmt
    mips_class,

    unk,
};

b32 is_mem_access(instr_type type)
{
    switch(type)
    {
        case instr_type::store: return true;
        case instr_type::store_float: return true;
        case instr_type::store_cop2: return true;

        case instr_type::load: return true;
        case instr_type::load_float: return true;
        case instr_type::load_cop2: return true;

        default: return false;
    }
}

const char *REG_NAMES[REG_MASK + 1]
{
    "$zero", 
    "$at", // assembler tempoary
    "$v0", // return value or expression evaluation
    "$v1",
    "$a0", // args
    "$a1",
    "$a2",
    "$a3",
    "$t0",
    "$t1", // temp
    "$t2",
    "$t3",
    "$t4",
    "$t5",
    "$t6",
    "$t7",
    "$s0",
    "$s1", // saved regs
    "$s2",
    "$s3",
    "$s4",
    "$s5",
    "$s6",
    "$s7",
    "$t8",
    "$t9",
    "$k0", // kernel reserved
    "$k1",
    "$gp", // global pointer 
    "$sp", // stack pointer 
    "$fp", // frame pointer
    "$ra", // return address 
};


const char *COP0_NAMES[32]
{
    "index",
    "random",
    "entryLo0",
    "entryLo1",
    "context",
    "pageMask",
    "Wired",
    "r7",
    "badVAddr",
    "count",
    "entryHi",
    "compare",
    "status",
    "cause",
    "EPC",
    "PRID",
    "config",
    "LLAddr",
    "watchLo",
    "watchHi",
    "xContext",
    "r21",
    "r22",
    "r23",
    "r24",
    "r25",
    "parityError",
    "cacheError",
    "TagLo",
    "TagHi",
    "errorEPC",
    "r31",
};


using DISASS_FUNC = std::string (*)(Program& program, u64 addr, const Opcode& opcode);

// return true if block is finished!
using MARK_FUNC = b32 (*)(Program& program,Func& func,  Block& block, u64 addr, const Opcode& opcode);



u64 branch_addr(u64 pc, s16 imm)
{
    return pc  + (sign_extend_type<s32,s16>(imm) << 2);
}


u64 jump_addr(u64 pc, u32 opcode)
{
    const u64 jump_mask = u64(0x03ff'ffff);

    return (u64(opcode & jump_mask) << u64(2)) | (pc & ~jump_mask);
}

u32 read_sa(u32 opcode)
{
    return (opcode >> 6) & 0b11'111;
}  


struct Instr;
using CHAIN_FUNC = const Instr* (*)(const Opcode& opcode, u32 version);

struct Instr
{
    const char *name;
    instr_type fmt;
    MARK_FUNC mark_func;
    DISASS_FUNC disass_func;

    // what version was this added in
    u32 version;

    // fptr that will give back the correct instr field
    CHAIN_FUNC chain = nullptr;
};

const Instr* decode_instr(const Opcode& opcode, u32 version);
const Instr* decode_regimm(const Opcode& opcode,u32 version);
const Instr* decode_special(const Opcode& opcode,u32 version);
const Instr* decode_cop0(const Opcode& opcode, u32 version);
const Instr* decode_cop1(const Opcode& opcode, u32 version);
const Instr* decode_cop2(const Opcode& opcode, u32 version);


}


#include "disass_mips.cpp"
#include "mark_mips.cpp"
#include "decode_mips.cpp"

namespace beyond_all_repair
{

// TODO: make this cache the section

// TODO: how do we deal with delay slots?
void mark_block_mips(Program& program,Func& func,Block& block)
{
    u64 addr = block.addr;
    b32 done = false;
    
    while(!done)
    {
        u32 op;

        // check read is in range
        if(!read_program(program,addr, &op))
        {
            print("Warning addr 0x{:x} out of range for block\n",addr);
            done = true;
            break;
        }

        else
        {
            addr += MIPS_INSTR_SIZE;
            block.size += MIPS_INSTR_SIZE;

            // we have hit the start of another basic block due to a branch over another
            if(block_exists(program,addr))
            {
                done = true;
                break;
            }
        }

        const auto opcode = make_opcode(op);
        const Instr* instr = decode_instr(opcode,MIPS3);

        if(instr->mark_func)
        {
            done = instr->mark_func(program,func,block,addr,opcode);
        }
    }

    block.hash = hash_block(program,block);
}

void print_console_func_mips(Program& program, const Func &func, const Config& config, u64 pc)
{
    for(u64 block_start : func.block_list)
    {
        const auto& block = program.block_lookup[block_start];

        print("{}:\n",block.name);

        // print the block
        for(u64 addr = block.addr; addr != block.addr + block.size; )
        {
            u32 op;

            // check read is in range
            if(!read_program(program,addr, &op))
            {
                print("Warning addr 0x{:x} out of range for block\n",addr);
                return;
            }


            const b32 cur_pc = pc == addr;

            addr += MIPS_INSTR_SIZE;

            const auto opcode = make_opcode(op);    

            print("   ");

            if(config.print_addr)
            {
                print("0x{:08x}: ",(addr - MIPS_INSTR_SIZE));
            }

            if(config.print_opcodes)
            {
                print("0x{:08x} ",op);
            }

            // print the actual instruction
            print("{}",disass_mips(program,addr,opcode));

            if(cur_pc)
            {               
                print(" <-----");
            }

            print("\n");
        }
    }
}

void print_console_mips(Program& program, Config& config)
{
    for(const auto &[key, func] : program.func_lookup)
    {
        if(!config.print_external && func.external)
        {
            continue;
        }

        print_console_func_mips(program,func,config,PC_UNK);
        puts("\n");
    }
}

void print_console_linear_mips(Program& program, Config& config)
{
    for(const auto &[key, func] : program.func_lookup)
    {
        if(!config.print_external && func.external)
        {
            continue;
        }


        print("{}:\n",func.name);

        // print the block
        for(u64 addr = func.addr; addr != func.addr + func.size; )
        {
            u32 op;

            // check read is in range
            if(!read_program(program,addr, &op))
            {
                print("Warning addr 0x{:x} out of range for block\n",addr);
                return;
            }

            addr += MIPS_INSTR_SIZE;

            const auto opcode = make_opcode(op);    

            print("   ");

            if(config.print_addr)
            {
                print("0x{:08x}: ",(addr - MIPS_INSTR_SIZE));
            }

            if(config.print_opcodes)
            {
                print("0x{:08x} \n",op);
            }

            // print the actual instruction
            print("{}\n",disass_mips(program,addr,opcode));
        }
    
        puts("\n");
    }    
}


u32 get_opcode_type(u32 opcode)
{
    return (opcode >> 26) & INSTR_TYPE_MASK;
}

u32 get_fs(const Opcode& opcode)
{
    return opcode.rd;
}

u32 get_ft(const Opcode& opcode)
{
    return opcode.rt;
}

u32 get_fd(const Opcode& opcode)
{
    return (opcode.op >> 6) & REG_MASK;
}

}