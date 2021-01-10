#include "heap.h"

vm_heap vm_new_heap() {
    vm_heap heap;
    heap->items = malloc(sizeof(vm_obj) * HEAP_SIZE);
    return heap;
}

