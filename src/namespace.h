#ifndef H_VM_NAMESPACE
#define H_VM_NAMESPACE

#define VM_NAMESPACE_SIZE 256

#include <stdint.h>
#include <stdbool.h>

#include "heap.h"

typedef uint8_t vm_name;

typedef struct vm_namespace {
    vm_heap_ptr ptrs[VM_NAMESPACE_SIZE];
    char *names[VM_NAMESPACE_SIZE];
    bool defined[VM_NAMESPACE_SIZE];
} vm_namespace;

vm_namespace vm_new_namespace();
void vm_namespace_declare(vm_namespace *ns, vm_name num,
                          char *name, vm_heap_ptr ptr);
bool vm_namespace_declared(vm_namespace *ns, vm_name n);
bool vm_namespace_defined(vm_namespace *ns, vm_name n);
char *vm_namespace_get_name(vm_namespace *ns, vm_name n);
vm_heap_ptr vm_namespace_get_ptr(vm_namespace *ns, vm_name n);

#endif
