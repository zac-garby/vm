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

    case I_DUP: {
        vm_stack_item top = vm_stack_peek(&frame->stack);
        
        if (top.is_heap_ref) {
            vm_stack_push(&frame->stack, top);
        } else {
            vm_obj *new = malloc(sizeof(vm_obj));
            *new = *top.data.obj;
            vm_stack_push_local(&frame->stack, new);
        }
        
        goto ok;
    }

    case I_POP:
        vm_stack_pop(&frame->stack);
        goto ok;

    case I_SWAP: {
        vm_stack_item a = vm_stack_pop(&frame->stack);
        vm_stack_item b = vm_stack_pop(&frame->stack);

        vm_stack_push(&frame->stack, a);
        vm_stack_push(&frame->stack, b);
        
        goto ok;
    }

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
        printf("locals\n");
        for (int i = 0; i < VM_NAMESPACE_SIZE; i++) {
            if (vm_namespace_defined(&frame->names, i)) {
                vm_heap_ptr ptr = vm_namespace_get_ptr(&frame->names, i);
                char *name = vm_namespace_get_name(&frame->names, i);
                vm_obj *obj = vm_heap_retrieve(&thread->heap, ptr);
                char *obj_str = vm_debug_obj(obj);
                printf("  %d. %s = %s (at #%d)\n", i, name, obj_str, ptr);
                free(obj_str);
            } else if (vm_namespace_declared(&frame->names, i)) {
                char *name = vm_namespace_get_name(&frame->names, i);
                printf("  %d. %s = <undefined>\n", i, name);
            }
        }
        printf("globals\n");
        for (int i = 0; i < VM_NAMESPACE_SIZE; i++) {
            if (vm_namespace_defined(&thread->globals, i)) {
                vm_heap_ptr ptr = vm_namespace_get_ptr(&thread->globals, i);
                char *name = vm_namespace_get_name(&thread->globals, i);
                vm_obj *obj = vm_heap_retrieve(&thread->heap, ptr);
                char *obj_str = vm_debug_obj(obj);
                printf("  %d. %s = %s (at #%d)\n", i, name, obj_str, ptr);
                free(obj_str);
            } else if (vm_namespace_declared(&thread->globals, i)) {
                char *name = vm_namespace_get_name(&thread->globals, i);
                printf("  %d. %s = <undefined>\n", i, name);
            }
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
            printf("constant %d out of bounds (in load const)\n", arg);
            goto error;
        }

        if (vm_stack_full(&frame->stack)) {
            printf("data stack overflow (in load const)\n");
            goto error;
        }
        
        vm_stack_push_local(&frame->stack, &frame->consts[arg]);
        goto ok;
    }

    case I_LOAD_LOCAL: {
        if (!vm_namespace_defined(&frame->names, arg)) {
            printf("name %d undefined or undeclared (in load local)\n", arg);
            goto error;
        }

        if (vm_stack_full(&frame->stack)) {
            printf("data stack overflow (in load local)\n");
            goto error;
        }
        
        vm_heap_ptr ptr = vm_namespace_get_ptr(&frame->names, arg);
        vm_stack_push_heap_ref(&frame->stack, ptr);
        goto ok;
    }

    case I_STORE_LOCAL: {
        if (!vm_namespace_declared(&frame->names, arg)) {
            printf("name %d does not exist (in store local)\n", arg);
            goto error;
        }

        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow (in store local)\n");
            goto error;
        }

        vm_heap_ptr ptr = vm_namespace_get_ptr(&frame->names, arg);
        vm_stack_item top = vm_stack_pop(&frame->stack);
        vm_obj *obj = vm_stack_item_val(&top, &thread->heap);
        vm_heap_store(&thread->heap, ptr, obj);
        frame->names.defined[arg] = true;

        goto ok;
    }

    case I_LOAD_GLOBAL: {
        if (!vm_namespace_defined(&thread->globals, arg)) {
            printf("name %d is undefined or undeclared (in load global)\n", arg);
            goto error;
        }

        if (vm_stack_full(&frame->stack)) {
            printf("data stack overflow (in load global)\n");
            goto error;
        }

        vm_heap_ptr ptr = vm_namespace_get_ptr(&thread->globals, arg);
        vm_stack_push_heap_ref(&frame->stack, ptr);
        goto ok;
    }

    case I_STORE_GLOBAL: {
        if (!vm_namespace_declared(&thread->globals, arg)) {
            printf("name %d does not exist (in store global)\n", arg);
            goto error;
        }

        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow (in store global)\n");
            goto error;
        }

        vm_heap_ptr ptr = vm_namespace_get_ptr(&thread->globals, arg);
        vm_stack_item top = vm_stack_pop(&frame->stack);
        vm_obj *obj = vm_stack_item_val(&top, &thread->heap);
        vm_heap_store(&thread->heap, ptr, obj);
        thread->globals.defined[arg] = true;

        goto ok;
    }

    case I_NEW_LIST: {
        if (vm_stack_full(&frame->stack)) {
            printf("data stack overflow (in new list)\n");
            goto error;
        }

        vm_obj *ls = malloc(sizeof(vm_obj));
        vm_new_list(ls, arg);
        vm_stack_push_local(&frame->stack, ls);
        goto ok;
    }

    case I_ADD:
    case I_SUB:
    case I_MUL:
    case I_DIV:
    case I_LT:
    case I_GT:
    case I_LTE:
    case I_GTE: {
        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow (in numeric binop %d)\n", instr);
            goto error;
        }

        vm_stack_item right = vm_stack_pop(&frame->stack);
        vm_stack_item left = vm_stack_pop(&frame->stack);

        vm_obj *lobj = vm_stack_item_val(&left, &thread->heap);
        vm_obj *robj = vm_stack_item_val(&right, &thread->heap);

        if (lobj->type == VM_INT && robj->type == VM_INT) {
            binop_int(instr, *((int*) lobj->data), *((int*) robj->data),
                      &frame->stack);
            goto ok;
        } else if (lobj->type == VM_FLOAT && robj->type == VM_FLOAT) {
            binop_int(instr, *((double*) lobj->data), *((double*) robj->data),
                      &frame->stack);
            goto ok;
        } else {
            printf("invalid binary operation between %s and %s\n",
                   vm_show_type(lobj->type), vm_show_type(robj->type));
            goto error;
        }
    }

    case I_EQ: {
        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow (in eq)\n");
            goto error;
        }

        vm_stack_item right = vm_stack_pop(&frame->stack);

        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow (in eq)\n");
            goto error;
        }

        vm_stack_item left = vm_stack_pop(&frame->stack);

        vm_obj *lobj = vm_stack_item_val(&left, &thread->heap);
        vm_obj *robj = vm_stack_item_val(&right, &thread->heap);

        vm_obj *ret = malloc(sizeof(vm_obj));
        vm_new_bool(ret, vm_obj_equal(lobj, robj, &thread->heap));
        vm_stack_push_local(&frame->stack, ret);
        goto ok;
    }

    case I_NEQ: {
        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow (in neq)\n");
            goto error;
        }

        vm_stack_item right = vm_stack_pop(&frame->stack);

        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow (in neq)\n");
            goto error;
        }

        vm_stack_item left = vm_stack_pop(&frame->stack);

        vm_obj *lobj = vm_stack_item_val(&left, &thread->heap);
        vm_obj *robj = vm_stack_item_val(&right, &thread->heap);

        vm_obj *ret = malloc(sizeof(vm_obj));
        vm_new_bool(ret, !vm_obj_equal(lobj, robj, &thread->heap));
        vm_stack_push_local(&frame->stack, ret);
        goto ok;
    }
        
    case I_LIST_APPEND: {
        if (frame->stack.top < 2) {
            printf("data stack underflow (in append)\n");
            goto error;
        }

        vm_stack_item obj_item = vm_stack_pop(&frame->stack);
        vm_stack_item ls_item = vm_stack_pop(&frame->stack);
        
        vm_obj *ls = vm_stack_item_val(&ls_item, &thread->heap);

        if (ls->type != VM_LIST) {
            printf("cannot append to non-lists\n");
            goto error;
        }

        vm_heap_ptr ptr;
        if (obj_item.is_heap_ref) {
            ptr = obj_item.data.heap_ref;
        } else {
            ptr = vm_heap_claim(&thread->heap);
            vm_heap_store(&thread->heap, ptr, obj_item.data.obj);
        }

        vm_list_append(ls, ptr);
        vm_stack_push(&frame->stack, ls_item);
        goto ok;
    }

    case I_JUMP:
    case I_LJUMP:
        if (arg >= frame->code_length) {
            printf("jump %d out of bounds (in jump)\n", arg);
            goto error;
        }

        frame->cur = arg;
        goto ok;

    case I_JUMP_FALSE:
    case I_LJUMP_FALSE: {
        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow (in jump-false)\n");
            goto error;
        }

        vm_stack_item item = vm_stack_pop(&frame->stack);
        vm_obj *obj = vm_stack_item_val(&item, &thread->heap);

        if (obj->type != VM_BOOL) {
            printf("conditions must be booleans, got %s\n",
                   vm_show_type(obj->type));
            goto error;
        }

        if (!*((bool*) obj->data)) {
            if (arg >= frame->code_length) {
                printf("jump %d out of bounds (in jump-false)\n", arg);
                goto error;
            }

            frame->cur = arg;
        }

        goto ok;
    }

    case I_RETURN: {
        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow (in return)\n");
            goto error;
        }

        vm_stack_item item = vm_stack_pop(&frame->stack);

        // there won't ever be an error here, because we know there's a
        // frame on the stack.
        vm_callstack_pop(&thread->callstack);

        vm_stackframe *next = vm_callstack_top(&thread->callstack);
        if (!next) {
            printf("callstack underflow (in return)\n");
            goto error;
        }

        vm_stack_push(&next->stack, item);

        goto ok;
    }

    case I_CALL: {
        if (vm_stack_empty(&frame->stack)) {
            printf("data stack underflow. no function to call\n");
            goto error;
        }

        vm_stack_item fn_item = vm_stack_pop(&frame->stack);
        vm_obj *fn = vm_stack_item_val(&fn_item, &thread->heap);

        if (fn->type != VM_FUNC) {
            printf("cannot call a non-function\n");
            goto error;
        }

        vm_funcobj *fnobj = (vm_funcobj*) fn->data;
        vm_stackframe sf = vm_new_stackframe(fnobj, &thread->heap);

        for (int i = 0; i < fnobj->arity; i++) {
            if (vm_stack_empty(&frame->stack)) {
                printf("data stack underflow. not enough args\n");
                goto error;
            }

            vm_stack_item arg_item = vm_stack_pop(&frame->stack);
            vm_stackframe_arg(&sf, &thread->heap, &arg_item, i);
        }

        if (!vm_callstack_push(&thread->callstack, sf)) {
            printf("stack overflow. too many calls!\n");
            goto error;
        }
        
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

void binop_int(byte instr, int l, int r, vm_stack *stack) {
    int result;
    int cmp = -1;

    switch (instr) {
    case I_ADD: result = l + r; break;
    case I_SUB: result = l - r; break;
    case I_MUL: result = l * r; break;
    case I_DIV: result = l / r; break;
    case I_LT: cmp = l < r; break;
    case I_GT: cmp = l > r; break;
    case I_LTE: cmp = l <= r; break;
    case I_GTE: cmp = l >= r; break;
    // one of these cases will always match
    }
    
    vm_obj *res = malloc(sizeof(vm_obj));
    if (cmp >= 0) {
        vm_new_bool(res, cmp);
    } else {
        vm_new_int(res, result);
    }
    vm_stack_push_local(stack, res);
}

void binop_float(byte instr, double l, double r, vm_stack *stack) {
    double result;
    int cmp = -1;

    switch (instr) {
    case I_ADD: result = l + r; break;
    case I_SUB: result = l - r; break;
    case I_MUL: result = l * r; break;
    case I_DIV: result = l / r; break;
    case I_LT: cmp = l < r; break;
    case I_GT: cmp = l > r; break;
    case I_LTE: cmp = l <= r; break;
    case I_GTE: cmp = l >= r; break;
    // one of these cases will always match
    }
    
    vm_obj *res = malloc(sizeof(vm_obj));
    if (cmp >= 0) {
        vm_new_bool(res, cmp);
    } else {
        vm_new_float(res, result);
    }
    vm_stack_push_local(stack, res);
}

bool vm_obj_equal(vm_obj *a, vm_obj *b, vm_heap *heap) {
    if (a->type != b->type) return false;

    void *ad = a->data, *bd = b->data;

    switch (a->type) {
    case VM_INT:
        return *((int*) ad) == *((int*) bd);
    case VM_CHAR:
        return *((char*) ad) == *((char*) bd);
    case VM_STR: {
        vm_strobj *as = ad, *bs = bd;
        if (as->length != bs->length) return false;
        return (strncmp(as->data, bs->data, as->length)) == 0;
    }
    case VM_BOOL:
        return *((char*) ad) == *((char*) bd);
    case VM_FLOAT:
        return *((char*) ad) == *((char*) bd);
    case VM_LIST: {
        vm_listobj *al = ad, *bl = bd;
        if (al->length != bl->length) return false;
        for (int i = 0; i < al->length; i++) {
            vm_obj *x, *y;
            x = vm_heap_retrieve(heap, al->data[i]);
            y = vm_heap_retrieve(heap, bl->data[i]);
            if (!vm_obj_equal(x, y, heap)) return false;
        }
        return true;
    }
    case VM_TUPLE:
    case VM_NULLABLE:
    case VM_FUNC:
    case VM_THREAD:
        return false;
    }
}
