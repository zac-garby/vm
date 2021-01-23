#ifndef H_VM_STACKFRAME
#define H_VM_STACKFRAME

#include <stdlib.h>

#include "object.h"
#include "stack.h"
#include "namespace.h"

typedef struct vm_stackframe {
    char *name;
    vm_stack stack;
    vm_namespace names;
    
    byte *code;
    unsigned int code_length;
    unsigned int cur; // the current instruction
    
    vm_obj *consts;
    int num_consts;
} vm_stackframe;

vm_stackframe vm_new_stackframe(vm_funcobj *fn, vm_heap *heap);

// copies an object as the nth argument to a stackframe.
// the object will be copied into the heap. it's assumed that
// the heap space for this argument has already been allocated
// (which will be the case if vm_new_stackframe was used.)
void vm_stackframe_arg(vm_stackframe *sf, vm_heap *heap,
                       vm_obj *obj, vm_name n);

#endif
