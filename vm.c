#include <stdio.h>
#include <stdlib.h>

#include "src/type.h"
#include "src/object.h"
#include "src/heap.h"

int main() {
    vm_heap heap = vm_new_heap();

    vm_obj *my_string = malloc(sizeof(vm_obj));
    vm_new_str(my_string, "Hello, world!");

    vm_heap_ptr addr = vm_heap_claim(&heap);   
    vm_heap_store(&heap, addr, my_string);
    free(my_string);

    vm_obj *retr = vm_heap_retrieve(&heap, addr);
    printf(" --> %s\n", vm_show_obj(retr));

    vm_free_obj(retr, false);
    vm_heap_release(&heap, addr);
    
    return 0;
}
