#ifndef H_VM_HEAP
#define H_VM_HEAP

#include <stdlib.h>

#include "object.h"
#include "linked_list.h"

#define VM_HEAP_SIZE 256

typedef struct vm_heap {
    // a HEAP_SIZE length array of objects
    vm_obj items[VM_HEAP_SIZE];

    // a linked list containing all of the available heap addresses
    vm_ll_list free_addresses;
} vm_heap;

vm_heap vm_new_heap();

// store an object at the given location in the heap.
// a shallow copy is made so the original is left intact.
void vm_heap_store_index(vm_heap *heap, int i, vm_obj *obj);

// retrieve the object at the given location in the heap.
// will return NULL if the index is out of bounds.
vm_obj *vm_heap_retrieve_index(vm_heap *heap, int i);

// claims a new address in the heap, if there is space. if no
// space is left, -1 is returned.
int vm_heap_claim(vm_heap *heap);

// releases a previously claimed address back into the heap.
void vm_heap_release(vm_heap *heap, int addr);

#endif
