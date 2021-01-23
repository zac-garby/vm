#ifndef H_VM_THREAD
#define H_VM_THREAD

#include <stdint.h>

#include "object.h"
#include "heap.h"
#include "callstack.h"
#include "bytecode.h"

typedef struct vm_thread {
    vm_thread_id id;
    vm_heap heap;
    vm_callstack callstack;
    vm_namespace globals;
} vm_thread;

vm_thread vm_new_thread(vm_thread_id id);

// executes the next instruction in the top stack frame.
// or, if there are no instructions remaining, pop the frame and return 1.
// if there are no frames to execute, return -1.
// if an unrecognised instruction is found, return -2.
// if there was an error while executing the instruction, return -3.
// if an instruction was executed successfuly, return 0.
int vm_thread_step(vm_thread *thread);

#endif
