#include <stdio.h>
#include <stdlib.h>

#include "src/type.h"
#include "src/object.h"
#include "src/heap.h"
#include "src/namespace.h"
#include "src/stack.h"
#include "src/stackframe.h"
#include "src/callstack.h"
#include "src/thread.h"

int main() {
    vm_funcobj main;
    
    vm_obj *the_arg = malloc(sizeof(vm_obj));
    vm_new_int(the_arg, 10);
    
    main.name = "add1";
    main.arity = 1;
    main.code = malloc(11);
    main.code_length = 3;
    main.consts = malloc(sizeof(vm_obj) * 1);
    main.num_consts = 1;
    main.names = malloc(sizeof(char*) * 2);
    main.num_names = 2;

    main.code[0] = I_NEW_LIST;
    main.code[1] = 5;
    main.code[2] = I_DEBUG;

    vm_new_int(&main.consts[0], 1);

    main.names[0] = "x";
    main.names[1] = "y";

    vm_thread th = vm_new_thread(0);
    vm_stackframe sf = vm_new_stackframe(&main, &th.heap);
    vm_stackframe_arg(&sf, &th.heap, the_arg, 0);
    vm_callstack_push(&th.callstack, sf);

    for (;;) {
        int status = vm_thread_step(&th);
        printf("status = %d\n", status);
        if (status == 1) break;
    }
    
    return 0;
}
