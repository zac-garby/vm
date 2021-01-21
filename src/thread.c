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
    
    printf("instr = %d, nargs = %d, arg = %d\n", instr, VM_N_ARGS[instr], arg);
    
    return 0;
}
