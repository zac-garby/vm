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
    vm_funcobj main, inc;
    
    //vm_obj *the_arg = malloc(sizeof(vm_obj));
    //vm_new_int(the_arg, 10);
    
    main.name = "main";
    main.arity = 0;
    main.code = malloc(13);
    main.code_length = 5;
    main.consts = malloc(sizeof(vm_obj) * 2);
    main.num_consts = 2;
    main.names = malloc(sizeof(char*) * 2);
    main.num_names = 2;
    main.names[0] = "x";
    main.names[1] = "y";

    main.code[0] = I_LOAD_CONST;
    main.code[1] = 0;
    main.code[2] = I_STORE_LOCAL;
    main.code[3] = 0;
    main.code[4] = I_DEBUG;
    main.code[5] = 0;
    main.code[6] = I_LOAD_CONST;
    main.code[7] = 1;
    main.code[8] = I_CALL;
    main.code[9] = I_LOAD_LOCAL;
    main.code[10] = 0;
    main.code[11] = I_POP;
    main.code[12] = I_DEBUG;

    inc.name = "inc";
    inc.arity = 1;
    inc.code = malloc(9);
    inc.code_length = 9;
    inc.consts = malloc(sizeof(vm_obj) * 1);
    inc.num_consts = 1;
    inc.names = malloc(sizeof(char*) * 1);
    inc.num_names = 1;
    inc.names[0] = "arg";

    inc.code[0] = I_DEBUG;
    inc.code[1] = I_LOAD_LOCAL;
    inc.code[2] = 0;
    inc.code[3] = I_LOAD_CONST;
    inc.code[4] = 0;
    inc.code[5] = I_DEBUG;
    inc.code[6] = I_LIST_APPEND;
    inc.code[7] = I_DEBUG;
    inc.code[8] = I_RETURN;

    vm_new_int(&inc.consts[0], 1);
    vm_new_int(&main.consts[0], 5);
    vm_new_func(&main.consts[1], inc);

    vm_thread th = vm_new_thread(0);
    vm_stackframe sf = vm_new_stackframe(&main, &th.heap);
    vm_callstack_push(&th.callstack, sf);

    for (;;) {
        int status = vm_thread_step(&th);
        printf("status = %d\n", status);
        if (status == 1) break;
    }
    
    return 0;
}
