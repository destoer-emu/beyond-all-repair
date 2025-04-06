#include <destoer/destoer.h>

namespace beyond_all_repair
{
using namespace destoer;

static constexpr int R0 = 0;
static constexpr int AT = 1;
static constexpr int V0 = 2;
static constexpr int V1 = 3;
static constexpr int A0 = 4;
static constexpr int A1 = 5;
static constexpr int A2 = 6;
static constexpr int A3 = 7;
static constexpr int T0 = 8;
static constexpr int T1 = 9;
static constexpr int T2 = 10;
static constexpr int T3 = 11;
static constexpr int T4 = 12;
static constexpr int T5 = 13;
static constexpr int T6 = 14;
static constexpr int T7 = 15;
static constexpr int S0 = 16;
static constexpr int S1 = 17;
static constexpr int S2 = 18;
static constexpr int S3 = 19;
static constexpr int S4 = 20;
static constexpr int S5 = 21;
static constexpr int S6 = 22;
static constexpr int S7 = 23;
static constexpr int T8 = 24;
static constexpr int T9 = 25;
static constexpr int K0 = 26;
static constexpr int K1 = 27;
static constexpr int GP = 28;
static constexpr int SP = 29;
static constexpr int FP = 30; // also reffered to as fp (frame pointer)
static constexpr int RA = 31;

static constexpr u32 REG_NAMES_SIZE = 32;

// cop 0 regs
static constexpr int INDEX = 0;
static constexpr int RANDOM = 1;
static constexpr int ENTRY_LO_ZERO = 2;
static constexpr int ENTRY_LO_ONE = 3;
static constexpr int CONTEXT = 4;
static constexpr int PAGE_MASK = 5;
static constexpr int WIRED = 6;

static constexpr int BAD_VADDR = 8;
static constexpr int COUNT = 9;
static constexpr int ENTRY_HI = 10;
static constexpr int COMPARE = 11;
static constexpr int STATUS = 12;

static constexpr int CAUSE = 13;
static constexpr int EPC = 14;
static constexpr int PRID = 15;
static constexpr int CONFIG = 16;
static constexpr int LLADDR = 17;
static constexpr int WATCH_LO = 18;
static constexpr int WATCH_HI = 19;
static constexpr int XCONFIG = 20;

static constexpr int PARITY_ERROR = 26;
static constexpr int CACHE_ERROR = 27;
static constexpr int TAGLO = 28;
static constexpr int TAGHI = 29;

static constexpr int ERROR_EPC = 30;



static constexpr u32 MIPS_INSTR_SIZE = sizeof(u32);

// exceptions
static constexpr u32 INTERRUPT = 0;
static constexpr u32 TLBM = 1;
static constexpr u32 TLBL = 2;
static constexpr u32 TLBS = 3;
static constexpr u32 SYSCALL = 8;
static constexpr u32 BREAK = 9;
static constexpr u32 COP_UNUSABLE = 11;
static constexpr u32 TRAP = 13;

extern const char *COP0_NAMES[32];

static constexpr u32 REG_MASK = 0b111'11;
extern const char *REG_NAMES[REG_MASK + 1];



static constexpr u32 INSTR_TYPE_MASK = 0b111'111;
static constexpr u32 FUNCT_MASK = 0b111'111;
static constexpr u32 REGIMM_MASK = 0b111'11;
static constexpr u32 COP0_RS_MASK = 0b111'11;
static constexpr u32 COP0_FUNCT_MASK = 0b111'111;

enum MIPS_VER
{
    MIPS1,
    MIPS2,
    MIPS3,
    MIPS4,
};

}