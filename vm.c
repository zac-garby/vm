#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "src/type.h"
#include "src/object.h"
#include "src/heap.h"
#include "src/namespace.h"
#include "src/stack.h"
#include "src/stackframe.h"
#include "src/callstack.h"
#include "src/thread.h"

#define MAX(a, b) (a)>(b)?(a):(b);

void print_debugger(vm_thread *thread);
void print_code_line(int line, vm_stackframe *frame);
void print_stack_line(int line, vm_thread *thread);
void print_namespace_line(int line, vm_namespace *ns, vm_thread *thread);

int main() {
    vm_funcobj main, inc;
    
    //vm_obj *the_arg = malloc(sizeof(vm_obj));
    //vm_new_int(the_arg, 10);
    
    main.name = "main";
    main.arity = 0;
    main.code = malloc(22);
    main.code_length = 8;
    main.consts = malloc(sizeof(vm_obj) * 3);
    main.num_consts = 3;
    main.names = malloc(sizeof(char*) * 2);
    main.num_names = 1;
    main.names[0] = "x";

    main.code[0] = I_LOAD_CONST;
    main.code[1] = 0;
    main.code[2] = I_STORE_GLOBAL;
    main.code[3] = 0;
    main.code[4] = I_LOAD_GLOBAL;
    main.code[5] = 0;
    main.code[6] = I_STORE_LOCAL;
    main.code[7] = 0;
    main.code[8] = I_LT;
    main.code[9] = I_JUMP_FALSE;
    main.code[10] = 21;
    main.code[11] = I_DEBUG;
    main.code[12] = I_LOAD_LOCAL;
    main.code[13] = 0;
    main.code[14] = I_LOAD_CONST;
    main.code[15] = 2;
    main.code[16] = I_ADD;
    main.code[17] = I_STORE_LOCAL;
    main.code[18] = 0;
    main.code[19] = I_JUMP;
    main.code[20] = 4;
    main.code[21] = I_DEBUG;

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
    vm_new_int(&main.consts[1], 5);
    vm_new_int(&main.consts[2], 10);

    vm_thread th = vm_new_thread(0);
    vm_namespace_declare(&th.globals, 0, "foo", vm_heap_claim(&th.heap));
    vm_namespace_declare(&th.globals, 1, "bar", vm_heap_claim(&th.heap));
    vm_stackframe sf = vm_new_stackframe(&main, &th.heap);
    vm_callstack_push(&th.callstack, sf);

    printf("\e[2J\e[;H");
    do {
        printf("\n \e[2;31m------- ^ stdout ^ -------- \e[0m\n\n");
        print_debugger(&th);
        getchar();
        printf("\e[2J\e[;H");
    } while (vm_thread_step(&th) != 1);
    
    return 0;
}

void print_debugger(vm_thread *thread) {
    vm_stackframe *frame = vm_callstack_top(&thread->callstack);
    unsigned int n_instrs = num_instructions(frame->code, frame->code_length);
    unsigned int top_lines = MAX(n_instrs, (unsigned int) frame->stack.top);
    int num_locals = vm_count_declared(&frame->names);
    int num_globals = vm_count_declared(&thread->globals);
    int bottom_lines = MAX(num_locals, num_globals);

    printf("\e[0;33min stackframe \"\e[1;32m%s\e[0;33m\"\n", frame->name);

    printf("\e[0m\e[4;33m                                                                \e[0m\n");
    printf("\e[1;7;33m CODE                           │ STACK                          \e[0m\n");
    for (unsigned int i = 0; i < top_lines; i++) {
        print_code_line(i, frame);
        print_stack_line(i, thread);
        printf("\n");
    }
    printf("\e[4;33m                                │                               \e[0m\n");

    printf("\e[1;7;33m LOCALS                         │ GLOBALS                        \e[0m\n");
    for (int i = 0; i < bottom_lines; i++) {
        print_namespace_line(i, &frame->names, thread);
        printf("│");
        print_namespace_line(i, &thread->globals, thread);
        printf("\n");
    }
    printf("\e[4;33m                                │                               \e[0m\n");
    
    printf("\e[0m");
}

void print_namespace_line(int line, vm_namespace *ns, vm_thread *thread) {
    int i = 0;
    for (int j = 0; j < line; j++) {
        for (i++; !vm_namespace_declared(ns, i); i++);
    }

    if (i >= 256) {
        printf("\e[0;33m                                ");
        return;
    }

    if (vm_namespace_defined(ns, i)) {
        vm_heap_ptr ptr = vm_namespace_get_ptr(ns, i);
        char *name = vm_namespace_get_name(ns, i);
        vm_obj *obj = vm_heap_retrieve(&thread->heap, ptr);
        char *obj_str = vm_debug_obj(obj);
        printf("\e[0;33m %10s \e[2m= \e[0;33m%-10s (\e[2m#%3d\e[0;33m) ", name, obj_str, ptr);
        free(obj_str);
    } else {
        char *name = vm_namespace_get_name(ns, i);
        printf("\e[0;33m %10s \e[2m= undefined         \e[0;33m", name);
    }
}

void print_code_line(int line, vm_stackframe *frame) {
    unsigned int i = 0;
    for (int j = 0; j < line; j++) {
        byte instr = frame->code[i];
        i += 1 + VM_N_ARGS[instr];
    }

    if (i >= frame->code_length) {
        printf("\e[0;33m     \t                      \t│");
        return;
    }
    
    byte instr = frame->code[i];
    int n_args = VM_N_ARGS[instr];
    bool has_arg = n_args != 0 && n_args != NA;
    uint16_t arg = 0;
    char *instr_s = vm_show_bytecode(instr);
    
    if (has_arg) {
        for (int j = 0; j < n_args; j++) {
            i++;
            arg |= ((uint16_t) frame->code[i]) << (8 * j);
        }
    }
    
    if (frame->cur == i - n_args) {
        printf("\e[1;33m>>");
    } else {
        printf("\e[0;33m  ");
    }
    
    if (has_arg) {
        printf(" % 4d\t%-16s (%-3d)\t│",
               i - n_args, instr_s, arg);
    } else {
        printf(" % 4d\t%-16s \t│",
               i, instr_s);
    }
}

void print_stack_line(int line, vm_thread *thread) {
    vm_stackframe *frame = vm_callstack_top(&thread->callstack);
    if (line >= frame->stack.top) {
        printf("  \e[2;36m---\e[0m");
        return;
    }
    
    vm_stack_item *item = &frame->stack.items[line];
    vm_obj *obj = vm_stack_item_val(item, &thread->heap);
    char *str = vm_debug_obj(obj);
    char *si = item->is_heap_ref ? "on heap" : "on stack";
    printf("  \e[2;36m%d. \e[0m\e[1;96m%s \e[0;36m(%s, %s)",
           line, str, vm_show_type(obj->type), si);
    free(str);
}
