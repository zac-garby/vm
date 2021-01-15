#ifndef H_VM_OBJECT
#define H_VM_OBJECT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "type.h"
#include "bytecode.h"

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

typedef struct vm_funcobj {
    char *name; // the name of this function
    int arity; // the number of arguments this function takes

    byte *code; // the bytecode for this function to execute
    int code_length; // the amount of bytes in the bytecode (max 65536)

    vm_obj *consts; // the constants that this function uses
    int num_consts; // the amount of constants in this function (max 256)

    char **names; // the variable names to be used in this function
    int num_names; // the amount of variable names in this function (max 256)
} vm_funcobj;

void vm_new_int(vm_obj *dest, int value);
void vm_new_char(vm_obj *dest, char value);
void vm_new_str(vm_obj *dest, char *value);
void vm_new_bool(vm_obj *dest, int value);
void vm_new_float(vm_obj *dest, double value);
void vm_new_list(vm_obj *dest, int capacity);
void vm_new_func(vm_obj *dest, vm_funcobj fn);

// frees the memory associated with a vm_obj
void vm_free_obj(vm_obj *obj);

// give a string representation of an object.
// this string will have to be freed by the caller.
char *vm_debug_obj(vm_obj *obj);

int vm_list_append(vm_obj *list, vm_heap_ptr elem);

#endif
