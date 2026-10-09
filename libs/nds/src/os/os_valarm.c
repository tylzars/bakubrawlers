#include <nitro/private/os_types.h>

extern u32 OS_DisableIrqMask(u32 mask);   // 0x02004bc0

// 0x020be038
static struct {
    u16 initialized;
    u32 head;
    u32 tail;
    u32 unk0c;
    u32 unk10;
} valarmState;

void OS_InitVAlarm(void) {
    if (valarmState.initialized) {
        return;
    }
    valarmState.initialized = TRUE;
    valarmState.unk0c = 0;
    valarmState.unk10 = 0;
    OS_DisableIrqMask(4);
    valarmState.tail = 0;
    valarmState.head = 0;
}
