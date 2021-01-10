#include "heap.h"

vm_heap vm_new_heap() {
    vm_heap heap;
    heap.items = malloc(sizeof(vm_obj) * HEAP_SIZE);
    return heap;
}

void vm_heap_store_index(vm_heap *heap, int i, vm_obj *obj) {
    memcpy(&heap->items[i], obj, sizeof(vm_obj));
}

vm_obj *vm_heap_retrieve_index(vm_heap *heap, int i) {
    if (i < 0 || i >= HEAP_SIZE) return NULL;
    return &heap->items[i];
}
