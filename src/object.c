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

vm_obj vm_new_bool(int value) {int *data = malloc(sizeof(int));
    *data = value ? 1 : 0;
    vm_obj obj = {VM_BOOL, data};
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

    case VM_CHAR: {
        char value = *((char*) obj.data);
        str = malloc(4);
        sprintf(str, "'%c'", value);
        return str;
    }

    case VM_BOOL: {
        int value = *((int*) obj.data);
        if (value == 0) {
            str = malloc(6); // can't just return "false", because it has to be on heap
            strcpy(str, "false");
        } else {
            str = malloc(5);
            strcpy(str, "true");
        }
        return str;
    }

    default:
        printf("vm_show_obj for type %s not yet implemented!\n", vm_show_type(t));
        return NULL;
    }
}
