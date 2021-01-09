#include <stdio.h>

#include "src/type.h"

int main() {
    printf("my type: %s\n", vm_show_type(VM_CHAR));
    return 0;
}
