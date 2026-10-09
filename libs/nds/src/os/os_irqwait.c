#include <nitro/private/os_types.h>

extern void OS_Halt(void);
extern OSIntrMode OS_EnableInterrupts(void);

#define OS_IRQ_CHECK (*(vu32 *) (HW_DTCM + 0x3ff8))

void OS_WaitIrq(BOOL clear, u32 irqFlags) {
    OSIntrMode e = OS_DisableInterrupts();
    u16 ime = REG_IME;
    REG_IME = 1;
    if (clear) {
        OS_IRQ_CHECK &= ~irqFlags;
    }
    while ((irqFlags & OS_IRQ_CHECK) == 0) {
        OS_Halt();
        OS_EnableInterrupts();
        OS_DisableInterrupts();
    }
    OS_IRQ_CHECK &= ~irqFlags;
    (void) REG_IME;
    REG_IME = ime;
    OS_RestoreInterrupts(e);
}
