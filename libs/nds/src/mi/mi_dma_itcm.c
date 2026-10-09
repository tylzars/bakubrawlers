// MI DMA register setup helpers in ITCM: 0x01ffcdbc-0x01ffcef0. Compiler: 2.0/sp1p5.
#include <nitro/internal.h>

extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
#define DMA0_SRC (*(vu32 *) 0x040000b0)

void func_01ffcdbc(u32 n, u32 src, u32 dst, u32 cnt) {
    u32 old = OS_DisableInterrupts();
    vu32 *d = (vu32 *) (0x40000b0 + n * 12);
    d[0] = src;
    d[1] = dst;
    d[2] = cnt;
    OS_RestoreInterrupts(old);
}

void func_01ffcdfc(u32 n, u32 src, u32 dst, u32 cnt) {
    u32 old = OS_DisableInterrupts();
    vu32 *d = (vu32 *) (0x40000b0 + n * 12);
    d[0] = src;
    d[1] = dst;
    d[2] = cnt;
    (void) DMA0_SRC;
    (void) DMA0_SRC;
    if (n == 0) {
        d[0] = 0;
        d[1] = 0;
        d[2] = 0x81400001;
    }
    OS_RestoreInterrupts(old);
}

void func_01ffce6c(u32 n, u32 src, u32 dst, u32 cnt) {
    vu32 *d = (vu32 *) (0x40000b0 + n * 12);
    d[0] = src;
    d[1] = dst;
    d[2] = cnt;
}

void func_01ffce90(u32 n, u32 src, u32 dst, u32 cnt) {
    vu32 *d = (vu32 *) (0x40000b0 + n * 12);
    d[0] = src;
    d[1] = dst;
    d[2] = cnt;
    (void) DMA0_SRC;
    (void) DMA0_SRC;
    if (n == 0) {
        d[0] = 0;
        d[1] = 0;
        d[2] = 0x81400001;
    }
    (void) DMA0_SRC;
    (void) DMA0_SRC;
}
