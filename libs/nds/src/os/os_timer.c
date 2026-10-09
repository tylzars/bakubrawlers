#include <nitro/private/sdk.h>

static u16 timerReserved;   // 0x020be018

void OSi_SetTimerReserved(int timerNum) {
    timerReserved |= 1 << timerNum;
}
