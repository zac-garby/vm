#include <stdio.h>
#include <stdlib.h>

#include "src/type.h"
#include "src/object.h"

int main() {
    vm_obj *my_string = vm_new_str("Hello World");
    vm_obj *my_int = vm_new_int(5);
    
    vm_obj *my_obj = vm_new_list(2);
    
    vm_list_append(my_obj, my_string);
    vm_list_append(my_obj, my_int);

    char *str1 = vm_show_obj(my_obj);

    printf("my_obj = %s\n", str1);

    free(str1);
    vm_free_obj(my_obj);
    
    return 0;
}
