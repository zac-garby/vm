#include "stackframe.h"

vm_stackframe vm_new_stackframe(vm_funcobj *fn, vm_heap *heap) {
    vm_stackframe frame;

    frame.name = fn->name;
    frame.stack = vm_new_stack();
    frame.names = vm_new_namespace();

    frame.code = fn->code;
    frame.code_length = fn->code_length;
    frame.cur = 0;

    frame.consts = malloc(sizeof(vm_obj) * fn->num_consts);
    memcpy(frame.consts, fn->consts, sizeof(vm_obj) * fn->num_consts);
    frame.num_consts = fn->num_consts;

    // for all non-parameter names, allocate some memory
    for (int i = 0; i < fn->num_names; i++) {
        vm_heap_ptr ptr = i < fn->arity ? 0 : vm_heap_claim(heap);
        vm_namespace_register(&frame.names, fn->names[i], ptr);
    }
    
    return frame;
}

void vm_stackframe_arg(vm_stackframe *sf, vm_heap *heap,
                       vm_stack_item *item, vm_name n) {
    vm_heap_ptr ptr;
    
    if (item->is_heap_ref) {
        ptr = item->data.heap_ref;
    } else {
        ptr = vm_heap_claim(heap);
        vm_heap_store(heap, ptr, item->data.obj);
    }

    sf->names.ptrs[n] = ptr;
}
