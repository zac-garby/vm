#ifndef H_VM_NAMESPACE
#define H_VM_NAMESPACE

#define VM_NAMESPACE_SIZE 256

#include <stdint.h>

#include "heap.h"

typedef uint8_t vm_name;

typedef struct vm_namespace {
    vm_heap_ptr ptrs[VM_NAMESPACE_SIZE];
    char *names[VM_NAMESPACE_SIZE];
} vm_namespace;

vm_namespace vm_new_namespace();

#endif
