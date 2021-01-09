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

vm_obj vm_new_int(int value);
vm_obj vm_new_char(char value);
vm_obj vm_new_bool(int value);
vm_obj vm_new_float(double value);

char *vm_show_obj(vm_obj obj);

#endif
