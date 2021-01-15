#ifndef H_VM_BYTECODE
#define H_VM_BYTECODE

typedef unsigned char byte;

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
#define I_NEW_LIST 13

// operation instructions
#define I_ADD 40
#define I_SUB 41
#define I_MUL 42
#define I_DIV 43

// control instructions
#define I_JUMP 80
#define I_LJUMP 81
#define I_JUMP_IF 82
#define I_LJUMP_IF 83
#define I_RETURN 84

#endif
