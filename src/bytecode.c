#include "bytecode.h"

uint8_t VM_N_ARGS[256] = {
    // stack instructions
    0, 0, 0, 0, 0 ,NA,NA,NA,NA,NA,

    // data instructions
    1, 1, 1, 1, 1, 1, NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,
    
    // operation instructions
    0, 0, 0, 0, NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,NA,
    
    // control instructions
    1, 2, 1, 2, 1,
};
