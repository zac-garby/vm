#ifndef H_VM_LINKED_LIST
#define H_VM_LINKED_LIST

#include <stdlib.h>

typedef struct vm_ll_node {
    void *data;
    struct vm_ll_node *next;
} vm_ll_node;

typedef struct vm_ll_list {
    vm_ll_node *head;
} vm_ll_list;

vm_ll_list vm_ll_new();
void vm_ll_free(vm_ll_list *list); // frees node structs AND their *data

void vm_ll_append(vm_ll_list *list, void *data);
void *vm_ll_pop_head(vm_ll_list *list);

#endif
