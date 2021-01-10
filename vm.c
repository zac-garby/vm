#include <stdio.h>
#include <stdlib.h>

#include "src/type.h"
#include "src/object.h"
#include "src/heap.h"

int main() {
    vm_heap heap = vm_new_heap();

    vm_obj *my_string = malloc(sizeof(vm_obj));
    vm_new_str(my_string, "Hello, world!");
    vm_heap_store_index(&heap, 0, my_string);

    vm_obj *retr = vm_heap_retrieve_index(&heap, 0);
    printf(" --> %s\n", vm_show_obj(retr));
    
    return 0;
}
