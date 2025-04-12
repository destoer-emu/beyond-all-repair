#pragma once
#include <lib.h>
#include <destoer/destoer.h>
#include <mips.h>
#include <decode.h>

namespace beyond_all_repair
{
using namespace destoer;

struct Block
{
    u64 func = 0;

    u64 addr = 0;
    u32 size = 0;

    // what other blocks does this exit to?
    std::vector<u64> exit;

    std::vector<u64> references;

    // default to LOC_%08x but some may have proper names
    std::string name;

    u64 hash = 0;
};

static constexpr u32 SIZE_UNK = 0xffff'ffff;

static constexpr u64 PC_UNK = 0xffff'ffff'ffff'ffff;

struct Func
{
    std::string name;
    u64 addr = 0;
    u32 size = SIZE_UNK;

    std::vector<u64> references;

    // sorted blocks
    std::set<u64> block_list;

    // which local branches havent been parsed yet?
    std::vector<u64> branch_target;

    b32 external = false;
};


struct Program;
// TODO: this should probably allow reading different sizes
using READ_FUNC = b32 (*)(Program& program, u64 addr, void* out,u32 size);

struct Opcode
{
    u32 op = 0;
    u32 type = 0;
    u32 rd = 0;
    u32 rs = 0;
    u32 rt = 0;
    u16 imm = 0;
};

struct Config
{
    b32 disassemble = false;
    b32 print_external = false;
    b32 print_addr = false;
    b32 print_opcodes = false;
    b32 gen = false;
    b32 linear = false;
    b32 gen_blank = false;
};

// TODO: need a way to get a function by block from an arbitary address
// but we aernt going to worry about this just yet
struct Program
{
    // program entry point
    u64 entry_point = 0;
    
    // TODO: replace this with a more generic abstract interpretation mechanism for now
    // just hard code this
    u64 gp = 0;

    // what endianess is the program?
    b32 is_be = false;    

    // function to perfrom address translation
    READ_FUNC read_func;

    void* data;

    // what targets have not been checked yet?
    std::vector<u64> func_target;

    // list of functions
    std::map<u64,Func> func_lookup; 

    std::map<std::string,u64> func_name_lookup;   

    // list of blocks
    std::map<u64, Block> block_lookup;
};

template<typename access_type>
b32 read_program(Program& program, u64 addr, access_type* out)
{
    static_assert(sizeof(access_type) <= sizeof(u64));

    const b32 success = program.read_func(program,addr,out, sizeof(access_type));

	if(program.is_be)
	{
		*out = bswap<access_type>(*out);
	}

    return success;
}



Program make_program(u64 entry_point,b32 is_be, READ_FUNC func,void* data);
Func& add_func(Program& program,u64 target, u64 pc,u32 size,const std::string& name, b32 external = false);
void disassemble_func(Program& program, Func& func);
void disassemble_console(Program& program);
Func& add_func_no_context(Program& program, u64 target);
void clear_references(Program& program);
void print_console_func_mips(Program& program, const Func &func, const Config& config, u64 pc);

std::string default_loc_name(u64 addr);
std::string default_func_name(u64 addr);
std::string loc_name(Program& program, u64 addr);

void add_block(Program& program,Func& func, u64 target, u64 pc, const std::string& name);
b32 block_exists(Program& program, u64 addr);
b32 func_exists(Program& program, u64 addr);
void local_call(Program& program,Func& func,u64 target, u64 pc);


u32 get_opcode_type(u32 opcode);
u32 get_fs(const Opcode& opcode);
u32 get_ft(const Opcode& opcode);
u32 get_fd(const Opcode& opcode);

u32 calc_base_table_offset(const Opcode& opcode);
u32 calc_cop0_table_offset(const Opcode& opcode);
u32 calc_cop1_table_offset(const Opcode& opcode);
u32 calc_cop2_table_offset(const Opcode& opcode);

Opcode make_opcode(const u32 op);

u32 instr_idx(const Opcode& opcode, u32 version);

u64 abs(u64 v);

std::string disass_mips(Program& program, u64 addr, const Opcode &opcode);
}