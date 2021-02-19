#include "namespace.h"

vm_namespace vm_new_namespace() {
    vm_namespace ns;

    for (int i = 0; i < VM_NAMESPACE_SIZE; i++) {
        ns.names[i] = NULL;
        ns.ptrs[i] = -1;
        ns.defined[i] = false;
    }

    return ns;
}

// declare a name in the namespace, but don't give it a heap pointer.
// this will leave it declared but undefined (i.e. no value.)
void vm_namespace_declare(vm_namespace *ns, vm_name num,
                          char *name, vm_heap_ptr ptr) {
    ns->names[num] = name;
    ns->ptrs[num] = ptr;
    ns->defined[num] = false;
}

bool vm_namespace_declared(vm_namespace *ns, vm_name n) {
    return ns->names[n] != NULL;
}

bool vm_namespace_defined(vm_namespace *ns, vm_name n) {
    return ns->defined[n];
}

char *vm_namespace_get_name(vm_namespace *ns, vm_name n) {
    return ns->names[n];
}

vm_heap_ptr vm_namespace_get_ptr(vm_namespace *ns, vm_name n) {
    return ns->ptrs[n];
}

int vm_count_declared(vm_namespace *ns) {
    int count = 0;

    for (int i = 0; i < VM_NAMESPACE_SIZE; i++) {
        if (vm_namespace_declared(ns, i)) {
            count++;
        }
    }
    
    return count;
}
