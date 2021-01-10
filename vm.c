#include <stdio.h>
#include <stdlib.h>

#include "src/type.h"
#include "src/object.h"

int main() {
    vm_obj *my_str = malloc(sizeof(vm_obj));
    vm_new_str(my_str, "Hello, world!");

    char *o = vm_show_obj(my_str);
    printf("%s\n", o);
    free(o);

    vm_free_obj(my_str);
    
    return 0;
}
