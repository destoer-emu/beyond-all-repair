#pragma once 
#include <destoer.h>

namespace beyond_all_repair 
{
using namespace destoer;

// base offsets
static constexpr u32 BASE_OFFSET = 0;
static constexpr u32 SPECIAL_OFFSET = BASE_OFFSET + INSTR_TYPE_MASK + 1;
static constexpr u32 REGIMM_OFFSET = SPECIAL_OFFSET + FUNCT_MASK + 1;


// cop0 offsets
static constexpr u32 COP0_RS_OFFSET = REGIMM_OFFSET + REGIMM_MASK + 1;
static constexpr u32 COP0_FUNCT_OFFSET = COP0_RS_OFFSET + COP0_RS_MASK + 1;
static constexpr u32 COP0_END = COP0_FUNCT_OFFSET + COP0_FUNCT_MASK + 1;



// cop1 offsets
static constexpr u32 COP1_RS_SHIFT = 21;
static constexpr u32 COP1_RS_MASK = 0b111'11;

static constexpr u32 COP1_BC_MASK = 0b111'11;
static constexpr u32 COP1_BC_SHIFT = 16;

static constexpr u32 COP1_FMT_SHIFT = 0;
static constexpr u32 COP1_FMT_MASK = 0b111'111;

static constexpr u32 COP1_RS_OFFSET = COP0_END;
static constexpr u32 COP1_BC_OFFSET = COP1_RS_OFFSET + COP1_RS_MASK + 1;

static constexpr u32 COP1_FMT_S_OFFSET = COP1_BC_OFFSET + COP1_BC_MASK + 1;
static constexpr u32 COP1_FMT_D_OFFSET = COP1_FMT_S_OFFSET + COP1_FMT_MASK + 1;
static constexpr u32 COP1_FMT_W_OFFSET = COP1_FMT_D_OFFSET + COP1_FMT_MASK + 1;
static constexpr u32 COP1_FMT_L_OFFSET = COP1_FMT_W_OFFSET + COP1_FMT_MASK + 1;



static constexpr u32 LAST_OFFSET = COP1_FMT_L_OFFSET;
static constexpr u32 LAST_MASK = COP1_FMT_MASK;

// NOTE: generally every 4 or 8 instrs have the same format.

// NOTE: this is kept as one contiguous table rather than seperate ones to keep it easy to dump and to avoid fracturing memory

// NOTE: extra entry for chain failure
static constexpr u32 INSTR_TABLE_SIZE = LAST_OFFSET + LAST_MASK + 2;


static constexpr u32 REGIMM_SHIFT = 16;
static constexpr u32 SPECIAL_SHIFT = 0;
static constexpr u32 BASE_SHIFT = 26;
}