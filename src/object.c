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

char *vm_show_obj(vm_obj obj) {
    vm_type t = obj.type;
    char *str;
    
    switch (t) {
    case VM_INT: {
        int value = *((int*) obj.data);
        int len = snprintf(NULL, 0, "%d", value);
        str = malloc(sizeof(char) * (len + 1));
        sprintf(str, "%d", value);
        return str;
    }

    case VM_CHAR:
        str = malloc(1);
        str[0] = *((char*) obj.data);
        str[1] = '\0';
        return str;

    default:
        printf("vm_show_obj for type %s not yet implemented!\n", vm_show_type(t));
        return NULL;
    }
}
