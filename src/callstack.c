#include "callstack.h"

vm_callstack vm_new_callstack() {
    vm_callstack cs;
    cs.top = 0;
    return cs;
}

bool vm_callstack_push(vm_callstack *stack, vm_stackframe frame) {
    if (stack->top >= VM_CALLSTACK_SIZE) {
        return false;
    }

    stack->frames[stack->top++] = frame;
    
    return true;
}

bool vm_callstack_pop(vm_callstack *stack) {
    if (stack->top <= 0) {
        return false;
    }

    stack->top--;

    // maybe do a bit of freeing here

    return true;
}

vm_stackframe *vm_callstack_top(vm_callstack *stack) {
    if (stack->top <= 0) {
        return NULL;
    }

    return &stack->frames[stack->top - 1];
}
