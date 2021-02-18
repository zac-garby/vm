#include "bytecode.h"

uint8_t VM_N_ARGS[256] = {
    // stack instructions
    0, 0, 0, 0, 0, NA,NA,NA,NA,NA,

    // data instructions
    1, 1, 1, 1, 1, 1, NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,
    
    // operation instructions
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NA,NA,NA,NA,NA,NA,NA,NA,NA,
    
    // control instructions
    1, 2, 1, 2, 0, 0
};

char *vm_show_bytecode(byte instr) {
    switch (instr) {
    case I_NOOP: return "NOOP";
    case I_DUP: return "DUP";
    case I_POP: return "POP";
    case I_SWAP: return "SWAP";
    case I_DEBUG: return "DEBUG";
    case I_LOAD_CONST: return "LOAD_CONST";
    case I_LOAD_LOCAL: return "LOAD_LOCAL";
    case I_STORE_LOCAL: return "STORE_LOCAL";
    case I_LOAD_GLOBAL: return "LOAD_GLOBAL";
    case I_STORE_GLOBAL: return "STORE_GLOBAL";
    case I_NEW_LIST: return "NEW_LIST";
    case I_ADD: return "ADD";
    case I_SUB: return "SUB";
    case I_MUL: return "MUL";
    case I_DIV: return "DIV";
    case I_EQ: return "EQ";
    case I_NEQ: return "NEQ";
    case I_LT: return "LT";
    case I_GT: return "GT";
    case I_LTE: return "LTE";
    case I_GTE: return "GTE";
    case I_LIST_APPEND: return "LIST_APPEND";
    case I_JUMP: return "JUMP";
    case I_LJUMP: return "LJUMP";
    case I_JUMP_FALSE: return "JUMP_FALSE";
    case I_LJUMP_FALSE: return "LJUMP_FALSE";
    case I_RETURN: return "RETURN";
    case I_CALL: return "CALL";
    default: return "<uh oh>";
    }
}
