#include "type.h"

char *vm_show_type(vm_type t) {
    switch (t) {
    case VM_INT:
        return "int";
    case VM_CHAR:
        return "char";
    case VM_STR:
        return "str";
    case VM_BOOL:
        return "bool";
    case VM_FLOAT:
        return "float";
    case VM_LIST:
        return "list";
    case VM_TUPLE:
        return "tuple";
    case VM_NULLABLE:
        return "nullable";
    case VM_FUNCTION:
        return "function";
    case VM_THREAD:
        return "thread";
    default:
        printf("INVALID STATE: vm_show_type called on unrecognised vm_type: %d\n", t);
        exit(1);
    }
}
