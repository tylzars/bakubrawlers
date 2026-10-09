#include <nitro/private/os_types.h>

extern void OS_Halt(void);

void OS_Terminate(void) {
    while (1) {
        OS_DisableInterrupts();
        OS_Halt();
    }
}
