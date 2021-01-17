#include <stdio.h>
#include <stdlib.h>

#include "src/type.h"
#include "src/object.h"
#include "src/heap.h"
#include "src/namespace.h"
#include "src/stack.h"
#include "src/stackframe.h"
#include "src/callstack.h"

int main() {
    vm_heap heap = vm_new_heap();
    vm_funcobj main;
    
    main.name = "add1";
    main.arity = 1;
    main.code = malloc(6);
    main.code_length = 6;
    main.consts = malloc(sizeof(vm_obj) * 1);
    main.num_consts = 1;
    main.names = malloc(sizeof(char*) * 1);
    main.num_names = 1;

    main.code[0] = I_LOAD_CONST;
    main.code[1] = I_LOAD_LOCAL;
    main.code[2] = 0;
    main.code[3] = I_ADD;
    main.code[4] = I_RETURN;
    main.code[5] = 1;

    vm_new_int(&main.consts[0], 1);

    main.names[0] = "x";

    vm_callstack cs = vm_new_callstack();
    vm_callstack_push(&cs, vm_new_stackframe(&main, &heap));
    
    return 0;
}
