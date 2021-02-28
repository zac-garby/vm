#include "heap.h"

vm_heap vm_new_heap() {
    vm_heap heap;

    heap.free_addresses = vm_ll_new();

    for (vm_heap_ptr i = 0; i < VM_HEAP_SIZE; i++) {
        vm_heap_release(&heap, i);
    }
    
    return heap;
}

void vm_heap_store(vm_heap *heap, vm_heap_ptr p, vm_obj *obj) {
    memcpy(&heap->items[p], obj, sizeof(vm_obj));
}

vm_obj *vm_heap_retrieve(vm_heap *heap, vm_heap_ptr p) {
    if (p >= VM_HEAP_SIZE) return NULL;
    return &heap->items[p];
}

vm_heap_ptr vm_heap_claim(vm_heap *heap) {
    vm_heap_ptr *data = vm_ll_pop_head(&heap->free_addresses);

    if (data) {
        vm_heap_ptr p = *data;
        free(data);
        return p;
    } else {
        return -1;
    }
}

void vm_heap_release(vm_heap *heap, vm_heap_ptr p) {
    vm_heap_ptr *data = malloc(sizeof(vm_heap_ptr));
    *data = p;
    vm_ll_append(&heap->free_addresses, data);
}

int vm_heap_gc(vm_heap *heap) {
    return 0;
}
