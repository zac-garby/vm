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
    int code_length;
    
    vm_obj *consts;
    int num_consts;
} vm_stackframe;

vm_stackframe vm_new_stackframe(vm_funcobj *fn, vm_heap *heap);

#endif
