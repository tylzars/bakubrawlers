#include <nitro/private/os_types.h>

extern void OSi_ExceptionHandler(void);   // 0x020068f4 (asm)

// 0x020bdf6c
static struct {
    u32 buf;
    u32 unk04;
    u32 unk08;
} exceptionState;

void OS_InitException(void) {
    u32 buf = *(vu32 *) 0x27ffd9c;
    if (buf >= 0x2600000 && buf < 0x2800000) {
        exceptionState.buf = buf;
    } else {
        exceptionState.buf = 0;
    }
    if (exceptionState.buf == 0) {
        vu8 *dtcm = (vu8 *) 0x27e3000;
        *(vu32 *) 0x27ffd9c = (u32) OSi_ExceptionHandler;
        *(vu32 *) (dtcm + 0xfdc) = (u32) OSi_ExceptionHandler;
    }
    exceptionState.unk08 = 0;
}
