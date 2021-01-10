#ifndef H_VM_OBJECT
#define H_VM_OBJECT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"

typedef struct vm_obj {
    vm_type type;
    void *data;
} vm_obj;

typedef struct vm_strobj {
    char *data; // the string's data. NOT null terminated
    int length; // the amount of visible characters in the string
    int capacity; // no. bytes allocated
} vm_strobj;

typedef struct vm_listobj {
    vm_obj **data; // the internal data array. list of pointers
    int length; // the amount of elements in the list
    int capacity; // the max possible length without allocating more memory
} vm_listobj;

vm_obj *vm_new_int(int value);
vm_obj *vm_new_char(char value);
vm_obj *vm_new_str(char *value);
vm_obj *vm_new_bool(int value);
vm_obj *vm_new_float(double value);
vm_obj *vm_new_list(int capacity);

// frees the memory associated with a vm_obj
void vm_free_obj(vm_obj *obj);

char *vm_show_obj(vm_obj *obj);

int vm_list_append(vm_obj *list, vm_obj *elem);

#endif
