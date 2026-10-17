#include <nitro/private/snd_h.h>

typedef struct {
    u32 flags;               // 0x00
    u32 field04;             // 0x04
    u16 field08;
    u16 field0a;
    u8 pad[0x14];
    struct {
        s16 var[16];         // 0x20
        u32 pos;             // 0x40
    } player[16];
    s16 global[16];          // 0x260
} SNDSharedWork;

SNDSharedWork *SNDi_SharedWork;   // 0x020bfc00

u32 func_02008b1c(void) {
    DC_InvalidateRange(&SNDi_SharedWork->field04, 4);
    return SNDi_SharedWork->field04;
}

u32 func_02008b48(void) {
    DC_InvalidateRange(SNDi_SharedWork, 4);
    return SNDi_SharedWork->flags;
}

void InitSharedWork(void *work) {
    SNDSharedWork *w = (SNDSharedWork *) work;
    int i;
    int j;
    w->field04 = 0;
    w->field08 = 0;
    w->field0a = 0;
    w->flags = 0;
    for (i = 0; i < 16; i++) {
        w->player[i].pos = 0;
        for (j = 0; j < 16; j++) {
            w->player[i].var[j] = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        w->global[i] = -1;
    }
    DC_FlushRange(w, 0x280);
}
