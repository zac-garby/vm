#ifndef H_VM_OBJECT
#define H_VM_OBJECT

#include "type.h"

typedef struct vm_obj {
    vm_type type;
    void *data;
} vm_obj;

#endif
