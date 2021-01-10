#ifndef H_VM_HEAP
#define H_VM_HEAP

#include <stdlib.h>

#include "object.h"

// can't be any longer than 256, because it needs to be addressed in one byte
#define HEAP_SIZE 256

typedef struct vm_heap {
    vm_obj items[HEAP_SIZE];
} vm_heap;

#endif
