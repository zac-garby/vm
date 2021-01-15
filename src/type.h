#ifndef H_VM_TYPE
#define H_VM_TYPE

#include <stdio.h>
#include <stdlib.h>

typedef enum vm_type {
    VM_INT,
    VM_CHAR,
    VM_STR,
    VM_BOOL,
    VM_FLOAT,
    VM_LIST,
    VM_TUPLE,
    VM_NULLABLE,
    VM_FUNC,
    VM_THREAD,
} vm_type;

char *vm_show_type(vm_type t);

#endif
