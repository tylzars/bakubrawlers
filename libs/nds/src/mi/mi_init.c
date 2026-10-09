#include "types.h"
extern void MI_StopDma(int dmaNo);
extern void MI_SetWramBank(u8 bank);
void func_02007f18(void) {
    MI_SetWramBank(3);
    MI_StopDma(0);
}
