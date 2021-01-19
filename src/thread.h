#ifndef H_VM_THREAD
#define H_VM_THREAD

#include "object.h"
#include "heap.h"
#include "callstack.h"

typedef struct vm_thread {
    vm_thread_id id;
    vm_heap heap;
    vm_callstack callstack;
    vm_namespace globals;
} vm_thread;

vm_thread vm_new_thread(vm_thread_id id);

#endif
