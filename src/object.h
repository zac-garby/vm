#ifndef H_VM_OBJECT
#define H_VM_OBJECT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "type.h"

typedef unsigned int vm_heap_ptr;

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
    vm_heap_ptr *data; // list of heap pointers
    int length; // the amount of elements in the list
    int capacity; // the max possible length without allocating more memory
} vm_listobj;

void vm_new_int(vm_obj *dest, int value);
void vm_new_char(vm_obj *dest, char value);
void vm_new_str(vm_obj *dest, char *value);
void vm_new_bool(vm_obj *dest, int value);
void vm_new_float(vm_obj *dest, double value);
void vm_new_list(vm_obj *dest, int capacity);

// frees the memory associated with a vm_obj
void vm_free_obj(vm_obj *obj);

// give a string representation of an object.
// this string will have to be freed by the caller.
char *vm_debug_obj(vm_obj *obj);

int vm_list_append(vm_obj *list, vm_heap_ptr elem);

#endif
