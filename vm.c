#include <stdio.h>
#include <stdlib.h>

#include "src/type.h"
#include "src/object.h"
#include "src/heap.h"

int main() {
    vm_heap heap = vm_new_heap();

    vm_heap_ptr list_p = vm_heap_claim(&heap);
    vm_new_list(vm_heap_retrieve(&heap, list_p), 8);

    vm_heap_ptr x_p = vm_heap_claim(&heap);
    vm_heap_ptr y_p = vm_heap_claim(&heap);
    vm_heap_ptr z_p = vm_heap_claim(&heap);
    vm_new_str(vm_heap_retrieve(&heap, x_p), "foo");
    vm_new_str(vm_heap_retrieve(&heap, y_p), "bar");
    vm_new_str(vm_heap_retrieve(&heap, z_p), "baz");

    vm_list_append(vm_heap_retrieve(&heap, list_p), x_p);
    vm_list_append(vm_heap_retrieve(&heap, list_p), y_p);
    vm_list_append(vm_heap_retrieve(&heap, list_p), z_p);

    printf("%s\n", vm_debug_obj(vm_heap_retrieve(&heap, list_p)));
    
    return 0;
}
