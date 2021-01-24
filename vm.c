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
    main.code = malloc(7);
    main.code_length = 7;
    main.consts = malloc(sizeof(vm_obj) * 1);
    main.num_consts = 1;
    main.names = malloc(sizeof(char*) * 1);
    main.num_names = 1;

    main.code[0] = I_LOAD_CONST;
    main.code[1] = 0;
    main.code[2] = I_LOAD_LOCAL;
    main.code[3] = 0;
    main.code[4] = I_DEBUG;
    main.code[5] = I_ADD;
    main.code[6] = I_DEBUG;
    main.code[7] = 1;

    vm_new_int(&main.consts[0], 1);

    main.names[0] = "x";

    vm_thread th = vm_new_thread(0);
    vm_stackframe sf = vm_new_stackframe(&main, &th.heap);
    vm_stackframe_arg(&sf, &th.heap, the_arg, 0);
    vm_callstack_push(&th.callstack, sf);

    printf("ptr = %d\n", sf.names.ptrs[0]);
    
    printf("step = %d\n", vm_thread_step(&th));
    printf("step = %d\n", vm_thread_step(&th));
    printf("step = %d\n", vm_thread_step(&th));
    printf("step = %d\n", vm_thread_step(&th));
    printf("step = %d\n", vm_thread_step(&th));
    printf("step = %d\n", vm_thread_step(&th));
    printf("step = %d\n", vm_thread_step(&th));
    
    return 0;
}
