#include "linked_list.h"

vm_ll_list vm_ll_new() {
    vm_ll_list list;
    list.head = NULL;
    return list;
}

void vm_ll_free(vm_ll_list *list) {
    vm_ll_node *node = list->head;

    while (node) {
        vm_ll_node *next = node->next;
        free(node->data);
        free(node);
        node = next;
    }
}

void vm_ll_append(vm_ll_list *list, void *data) {
    vm_ll_node *node = malloc(sizeof(vm_ll_node));
    node->data = data;
    node->next = NULL;
    
    if (list->head) {
        vm_ll_node *last = list->head;
        while (last->next) last = last->next;
        last->next = node;
    } else {
        list->head = node;
    }
}

void *vm_ll_pop_head(vm_ll_list *list) {
    if (list->head == NULL) {
        return NULL;
    }

    vm_ll_node *head = list->head;
    void* data = head->data;
    list->head = head->next;
    free(head);

    return data;
}
