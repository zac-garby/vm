#include "object.h"

vm_obj vm_new_int(int value) {
    int *data = malloc(sizeof(int));
    *data = value;
    vm_obj obj = {VM_INT, data};
    return obj;
}

vm_obj vm_new_char(char value) {
    char *data = malloc(sizeof(char));
    *data = value;
    vm_obj obj = {VM_CHAR, data};
    return obj;
}
