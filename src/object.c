#include "object.h"

void vm_new_int(vm_obj *dest, int value) {
    int *data = malloc(sizeof(int));
    *data = value;
    
    dest->type = VM_INT;
    dest->data = data;
}

void vm_new_char(vm_obj *dest, char value) {
    char *data = malloc(sizeof(char));
    *data = value;
    
    dest->type = VM_CHAR;
    dest->data = data;
}

void vm_new_str(vm_obj *dest, char *value) {
    int length = (int) strlen(value);

    vm_strobj *str = malloc(sizeof(vm_strobj));
    str->data = malloc(length);
    str->length = length;
    str->capacity = length;
    strncpy(str->data, value, length);

    dest->type = VM_STR;
    dest->data = str;
}

void vm_new_bool(vm_obj *dest, int value) {
    int *data = malloc(sizeof(int));
    *data = value ? 1 : 0;
    
    dest->type = VM_BOOL;
    dest->data = data;
}

void vm_new_float(vm_obj *dest, double value) {
    double *data = malloc(sizeof(double));
    *data = value;
    
    dest->type = VM_FLOAT;
    dest->data = data;
}

void vm_new_list(vm_obj *dest, int capacity) {
    vm_listobj *list = malloc(sizeof(vm_listobj));
    list->data = malloc(sizeof(vm_obj*) * capacity);
    list->length = 0;
    list->capacity = capacity;

    dest->type = VM_LIST;
    dest->data = list;
}

void vm_free_obj(vm_obj *obj) {
    vm_type t = obj->type;

    switch (t) {
    case VM_INT:
    case VM_CHAR:
    case VM_BOOL:
    case VM_FLOAT:
        free(obj->data);
        break;
        
    case VM_STR: {
        vm_strobj *strobj = (vm_strobj*) obj->data;
        free(strobj->data);
        free(strobj);
        break;
    }

    case VM_LIST: {
        vm_listobj *list = (vm_listobj*) obj->data;

        for (int i = 0; i < list->length; i++) {
            vm_obj *elem = list->data[i];
            vm_free_obj(elem);
        }

        free(list);
        
        break;
    }

    default:
        printf("vm_free_obj not yet implemented for type %s\n", vm_show_type(t));
    }

    free(obj);
}

char *vm_show_obj(vm_obj *obj) {
    vm_type t = obj->type;
    char *str;
    
    switch (t) {
    case VM_INT: {
        int value = *((int*) obj->data);
        int len = snprintf(NULL, 0, "%d", value);
        str = malloc(len + 1);
        sprintf(str, "%d", value);
        return str;
    }

    case VM_CHAR: {
        char value = *((char*) obj->data);
        str = malloc(4);
        sprintf(str, "'%c'", value);
        return str;
    }

    case VM_STR: {
        vm_strobj *strobj = (vm_strobj*) obj->data;
        str = malloc(strobj->length + 3);
        sprintf(str, "\"%.*s\"", strobj->length, strobj->data);
        return str;
    }

    case VM_BOOL: {
        int value = *((int*) obj->data);
        if (value == 0) {
            str = malloc(6); // can't just return "false", because it has to be on heap
            strcpy(str, "false");
        } else {
            str = malloc(5);
            strcpy(str, "true");
        }
        return str;
    }

    case VM_FLOAT: {
        double value = *((double*) obj->data);
        int len = snprintf(NULL, 0, "%lf", value);
        str = malloc(len + 1);
        sprintf(str, "%lf", value);
        return str;
    }

    case VM_LIST: {
        vm_listobj *list = (vm_listobj*) obj->data;
        int offset = 1, alloc = 16;
        str = malloc(16);
        sprintf(str, "[");

        for (int i = 0; i < list->length; i++) {
            vm_obj *elem = list->data[i];
            char *elem_str = vm_show_obj(elem);
            int elem_len = (int) strlen(elem_str);
            int new_alloc = alloc;

            while (offset + elem_len + 2 >= new_alloc) {
                // add new memory until there's enough space for this element plus
                // a closing bracket (and \0).
                new_alloc += 16;
            }

            if (new_alloc != alloc) {
                str = realloc(str, new_alloc);
            }
            
            strcat(str, elem_str);
            offset += elem_len;
            free(elem_str);

            if (i + 1 < list->length) {
                strcat(str, ", ");
                offset += 2;
            }
        }

        strcat(str, "]");
        
        return str;
    }

    default:
        printf("vm_show_obj for type %s not yet implemented!\n", vm_show_type(t));
        return NULL;
    }
}

int vm_list_append(vm_obj *obj, vm_obj *elem) {
    vm_listobj *list;

    if (obj->type != VM_LIST) {
        return 1;
    }

    list = (vm_listobj*) obj->data;
    
    if (list->length >= list->capacity) {
        list->capacity += 8;
        list->data = realloc(list->data, sizeof(vm_obj*) * list->capacity);
    }

    list->data[list->length++] = elem;

    return 0;
}
