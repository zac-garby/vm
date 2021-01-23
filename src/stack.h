#ifndef H_VM_STACK
#define H_VM_STACK

#include <stdbool.h>

#include "object.h"
#include "heap.h"

#define VM_STACK_SIZE 256

typedef struct vm_stack_item {
    union {
        vm_heap_ptr heap_ref;
        vm_obj *obj;
    } data;
    bool is_heap_ref;
} vm_stack_item;

typedef struct vm_stack {
    vm_stack_item items[VM_STACK_SIZE];
    int top; // a pointer to the space just *after* the top stack item
} vm_stack;

vm_stack vm_new_stack();
bool vm_stack_empty(vm_stack *stack);
bool vm_stack_full(vm_stack *stack);
vm_stack_item vm_stack_pop(vm_stack *stack);
void vm_stack_push_heap_ref(vm_stack *stack, vm_heap_ptr ptr);
void vm_stack_push_local(vm_stack *stack, vm_obj *obj);
vm_obj *vm_stack_item_val(vm_stack_item *item, vm_heap *heap);

#endif
