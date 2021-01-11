#ifndef H_VM_BYTECODE
#define H_VM_BYTECODE

typedef unsigned char byte;

// stack instructions
#define I_NOOP 0
#define I_DUP 1
#define I_POP 2
#define I_SWAP 3

// data instructions
#define I_CONST 10
#define I_LOAD_VAR 11
#define I_STORE_VAR 12
#define I_NEW_LIST 13

// operation instructions
#define I_ADD 20
#define I_SUB 21
#define I_MUL 22
#define I_DIV 23

#endif
