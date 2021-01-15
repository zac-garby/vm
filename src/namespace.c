#include "namespace.h"

vm_namespace vm_new_namespace() {
    vm_namespace ns;

    for (int i = 0; i < VM_NAMESPACE_SIZE; i++) {
        ns.ptrs[i] = -1;
        ns.names[i] = NULL;
    }

    return ns;
}

