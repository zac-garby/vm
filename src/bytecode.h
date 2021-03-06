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
#define I_DEBUG 4

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
#define I_EQ 34
#define I_NEQ 35
#define I_LT 36
#define I_GT 37
#define I_LTE 38
#define I_GTE 39
#define I_LIST_APPEND 40

// control instructions
#define I_JUMP 50
#define I_LJUMP 51
#define I_JUMP_FALSE 52
#define I_LJUMP_FALSE 53
#define I_RETURN 54
#define I_CALL 55

uint8_t VM_N_ARGS[256];
char *vm_show_bytecode(byte instr);
unsigned int num_instructions(byte *code, int n_bytes);

#endif
