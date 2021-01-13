#include "heap.h"

vm_heap vm_new_heap() {
    vm_heap heap;

    heap.free_addresses = vm_ll_new();

    for (int i = 0; i < VM_HEAP_SIZE; i++) {
        int *addr = malloc(sizeof(int));
        *addr = i;
        vm_ll_append(&heap.free_addresses, addr);
    }
    
    return heap;
}

void vm_heap_store_index(vm_heap *heap, int i, vm_obj *obj) {
    memcpy(&heap->items[i], obj, sizeof(vm_obj));
}

vm_obj *vm_heap_retrieve_index(vm_heap *heap, int i) {
    if (i < 0 || i >= VM_HEAP_SIZE) return NULL;
    return &heap->items[i];
}

int vm_heap_claim(vm_heap *heap) {
    int *data = vm_ll_pop_head(&heap->free_addresses);

    if (data) {
        int addr = *data;
        free(data);
        return addr;
    } else {
        return -1;
    }
}

void vm_heap_release(vm_heap *heap, int addr) {
    int *data = malloc(sizeof(int));
    *data = addr;
    vm_ll_append(&heap->free_addresses, data);
}
