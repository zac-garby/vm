#include "namespace.h"

vm_namespace vm_new_namespace() {
    vm_namespace ns;

    ns.num = 0;

    return ns;
}

vm_name vm_namespace_register(vm_namespace *ns,
                              char *name, vm_heap_ptr ptr) {
    ns->names[ns->num] = name;
    ns->ptrs[ns->num] = ptr;
    return ns->num++;
}

bool vm_namespace_valid(vm_namespace *ns, vm_name n) {
    return n < ns->num;
}

char *vm_namespace_get_name(vm_namespace *ns, vm_name n) {
    if (n >= ns->num) return NULL;

    return ns->names[n];
}

vm_heap_ptr vm_namespace_get_ptr(vm_namespace *ns, vm_name n) {
    if (n >= ns->num) return -1;

    return ns->ptrs[n];
}
