#include "thread.h"

vm_thread vm_new_thread(vm_thread_id id) {
    vm_thread th;

    th.id = id;
    th.heap = vm_new_heap();
    th.callstack = vm_new_callstack();
    th.globals = vm_new_namespace();
    
    return th;
}

int vm_thread_step(vm_thread *thread) {
    if (thread->callstack.top <= 0) {
        // there are no frames to execute.
        return -1;
    }
    
    vm_stackframe *frame = vm_callstack_top(&thread->callstack);

    if (frame->cur >= frame->code_length) {
        // return from the function.
        // more needs to be done here.
        vm_callstack_pop(&thread->callstack);
        return 1;
    }

    byte instr = frame->code[frame->cur++];
    uint8_t nargs = VM_N_ARGS[instr];
    uint16_t arg = 0;

    if (nargs == 1) {
        arg = (uint16_t) frame->code[frame->cur++];
    } else if (nargs == 2) {
        // low byte comes first
        arg = frame->code[frame->cur] | ((uint16_t) frame->code[frame->cur+1] << 8);
        frame->cur += 2;
    } else if (nargs == NA) {
        printf("unrecognised instruction %d\n", instr);
        return -2;
    }
    
    switch (instr) {
    case I_NOOP:
        goto ok;

    case I_DEBUG: {
        printf("\e[1;32mDEBUG\e[0;33m\n");
        printf("in thread %d\n", thread->id);
        printf("frame '%s' (next cur=%d, len=%d, consts=%d)\n",
               frame->name, frame->cur, frame->code_length, frame->num_consts);
        printf("stack (%d items)\n", frame->stack.top);
        for (int i = 0; i < frame->stack.top; i++) {
            vm_stack_item *item = &frame->stack.items[frame->stack.top - 1 - i];
            vm_obj *obj = vm_stack_item_val(item, &thread->heap);
            char *str = vm_debug_obj(obj);
            char *si = item->is_heap_ref ? "on heap" : "on stack";
            printf("  %d. %s (%s, %s)\n", i, str, vm_show_type(obj->type), si);
            free(str);
        }
        printf("callstack (%d items)\n", thread->callstack.top);
        for (int i = 0; i < thread->callstack.top; i++) {
            vm_stackframe *frame = &thread->callstack.frames[thread->callstack.top-1-i];
            printf("  %d. %s (cur=%d)\n", i, frame->name, frame->cur);
        }
        printf("\e[1;32mEND DEBUG\e[0m\n");
        goto ok;
    }

    case I_LOAD_CONST: {
        if (arg >= frame->num_consts) {
            printf("constant %d out of bounds\n", arg);
            goto error;
        }

        if (vm_stack_full(&frame->stack)) {
            printf("data stack overflow\n");
            goto error;
        }
        
        vm_stack_push_local(&frame->stack, &frame->consts[arg]);
        goto ok;
    }

    case I_LOAD_LOCAL: {
        if (!vm_namespace_valid(&frame->names, arg)) {
            printf("name out of bounds\n");
            goto error;
        }

        if (vm_stack_full(&frame->stack)) {
            printf("data stack overflow\n");
            goto error;
        }
        
        vm_heap_ptr ptr = vm_namespace_get_ptr(&frame->names, arg);
        vm_stack_push_heap_ref(&frame->stack, ptr);
        goto ok;
    }

    case I_STORE_LOCAL: {
        if (!vm_namespace_valid(&frame->names, arg)) {
            printf("name out of bounds\n");
            goto error;
        }

        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow\n");
            goto error;
        }

        vm_heap_ptr ptr = vm_namespace_get_ptr(&frame->names, arg);
        vm_stack_item top = vm_stack_pop(&frame->stack);
        vm_obj *obj = vm_stack_item_val(&top, &thread->heap);
        vm_heap_store(&thread->heap, ptr, obj);
        goto ok;
    }

    case I_NEW_LIST: {
        if (vm_stack_full(&frame->stack)) {
            printf("data stack overflow\n");
            goto error;
        }

        vm_obj *ls = malloc(sizeof(vm_obj));
        vm_new_list(ls, arg);
        vm_stack_push_local(&frame->stack, ls);
        goto ok;
    }

    case I_ADD: {
        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow\n");
            goto error;
        }

        vm_stack_item right = vm_stack_pop(&frame->stack);
        vm_stack_item left = vm_stack_pop(&frame->stack);

        vm_obj *lobj = vm_stack_item_val(&left, &thread->heap);
        vm_obj *robj = vm_stack_item_val(&right, &thread->heap);

        // TODO: move the actual addition logic elsewhere
        if (lobj->type != VM_INT || robj->type != VM_INT) {
            printf("attempted to add non-integers\n");
            goto error;
        }

        int sum = *((int*) lobj->data) + *((int*) robj->data);
        vm_obj *res = malloc(sizeof(vm_obj));
        vm_new_int(res, sum);
        vm_stack_push_local(&frame->stack, res);
        goto ok;
    }
        
    default:
        printf("instruction %d not implemented\n", instr);
        return -2;
    }

    error:
    return -3;
    
    ok:
    return 0;
}
