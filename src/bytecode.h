#ifndef H_VM_BYTECODE
#define H_VM_BYTECODE

#include <stdint.h>

// the amount of args that a non-existent instruction has.
// just a placeholder.
#define NA 255

typedef uint8_t byte;

// stack instructions
#define I_NOOP 0
#define I_DUP 1
#define I_POP 2
#define I_SWAP 3

// data instructions
#define I_LOAD_CONST 10
#define I_LOAD_LOCAL 11
#define I_STORE_LOCAL 12
#define I_LOAD_GLOBAL 13
#define I_STORE_GLOBAL 14
#define I_NEW_LIST 15

// operation instructions
#define I_ADD 30
#define I_SUB 31
#define I_MUL 32
#define I_DIV 33

// control instructions
#define I_JUMP 50
#define I_LJUMP 51
#define I_JUMP_IF 52
#define I_LJUMP_IF 53
#define I_RETURN 54

uint8_t VM_N_ARGS[256];

#endif
