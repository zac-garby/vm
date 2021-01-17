#ifndef H_VM_CALLSTACK
#define H_VM_CALLSTACK

#include <stdbool.h>

#include "stackframe.h"

#define VM_CALLSTACK_SIZE 256

typedef struct vm_callstack {
    vm_stackframe frames[VM_CALLSTACK_SIZE];
    int top; // a pointer to the space just *after* the top stack item
} vm_callstack;

vm_callstack vm_new_callstack();

// push/pop functions. return true on success
bool vm_callstack_push(vm_callstack *stack, vm_stackframe frame);
bool vm_callstack_pop(vm_callstack *stack);

// peeks the top of the stack. returns NULL if empty
vm_stackframe *vm_callstack_top(vm_callstack *stack);

#endif
