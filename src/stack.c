#include "stack.h"

vm_stack vm_new_stack() {
    vm_stack stack;
    stack.top = 0;
    return stack;
}

bool vm_stack_empty(vm_stack *stack) {
    return stack->top <= 0;
}

bool vm_stack_full(vm_stack *stack) {
    return stack->top >= VM_STACK_SIZE;
}

vm_stack_item vm_stack_pop(vm_stack *stack) {
    return stack->items[--stack->top];
}

void vm_stack_push_heap_ref(vm_stack *stack, vm_heap_ptr ptr) {
    vm_stack_item *item = &stack->items[stack->top++];
    item->is_heap_ref = true;
    item->data.heap_ref = ptr;
}

void vm_stack_push_local(vm_stack *stack, vm_obj *obj) {
    vm_stack_item *item = &stack->items[stack->top++];
    item->is_heap_ref = false;
    item->data.obj = obj;
}