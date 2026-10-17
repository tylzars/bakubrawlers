#include "types.h"
extern void MI_StopDma(int dmaNo);
extern void MI_SetWramBank(u8 bank);
void MI_Init(void) {
    MI_SetWramBank(3);
    MI_StopDma(0);
}
