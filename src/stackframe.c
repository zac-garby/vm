#include "stackframe.h"

vm_stackframe vm_new_stackframe(vm_funcobj *fn, vm_heap *heap) {
    vm_stackframe frame;

    frame.name = fn->name;
    frame.stack = vm_new_stack();
    frame.names = vm_new_namespace();

    frame.code = fn->code;
    frame.code_length = fn->code_length;

    frame.consts = malloc(sizeof(vm_obj) * fn->num_consts);
    memcpy(frame.consts, fn->consts, sizeof(vm_obj) * fn->num_consts);
    frame.num_consts = fn->num_consts;

    for (int i = 0; i < fn->num_names; i++) {
        vm_heap_ptr ptr = vm_heap_claim(heap);
        frame.names.names[i] = fn->names[i];
        frame.names.ptrs[i] = ptr;
    }
    
    return frame;
}
