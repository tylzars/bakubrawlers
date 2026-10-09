#include <nitro/private/os_types.h>

extern void OS_Terminate(void);
extern void OSi_EnterDmaCallback(int dmaNo, void (*callback)(void *), void *arg);   // 0x02004ad4
extern void func_01ffce90(int dmaNo, u32 src, void *dest, u32 ctrl);   // ITCM, starts DMA and waits
extern void func_01ffcdfc(int dmaNo, const void *src, void *dest, u32 ctrl);   // ITCM
extern void func_01ffcdbc(int dmaNo, const void *src, void *dest, u32 ctrl);   // ITCM
extern void func_01ffce6c(int dmaNo, u32 src, void *dest, u32 ctrl);   // ITCM
extern void func_02007804(int dmaNo);
extern void MI_StopDma(int dmaNo);
extern void MIi_CheckDma0SourceAddress(int dmaNo, u32 src, u32 size, u32 dir);

#define MI_DMA_BASE 0x4000000
#define MI_DMA_CNT(n) (*(vu32 *) (MI_DMA_BASE + ((n) * 3 + 2) * 4 + 0xb0))
#define MI_DMA_FILL(n) (*(vu32 *) (MI_DMA_BASE + (n) * 4 + 0xe0))

void MI_DmaFill32(int dmaNo, void *dest, u32 data, u32 size) {
    vu32 *dmaCntp;
    if (size == 0) {
        return;
    }
    dmaCntp = &MI_DMA_CNT(dmaNo);
    while (*dmaCntp & 0x80000000) {
    }
    {
        OSIntrMode e = OS_DisableInterrupts();
        MI_DMA_FILL(dmaNo) = data;
        func_01ffce90(dmaNo, (u32) &MI_DMA_FILL(dmaNo), dest, (size / 4) | 0x85000000);
        OS_RestoreInterrupts(e);
    }
    while (*dmaCntp & 0x80000000) {
    }
}

void MI_DmaCopy32(int dmaNo, const void *src, void *dest, u32 size) {
    vu32 *dmaCntp;
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, 0);
    if (size == 0) {
        return;
    }
    dmaCntp = &MI_DMA_CNT(dmaNo);
    while (*dmaCntp & 0x80000000) {
    }
    func_01ffcdfc(dmaNo, src, dest, (size / 4) | 0x84000000);
    while (*dmaCntp & 0x80000000) {
    }
}

void MI_DmaCopy16(int dmaNo, const void *src, void *dest, u32 size) {
    vu32 *dmaCntp;
    if (size == 0) {
        return;
    }
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, 0);
    dmaCntp = &MI_DMA_CNT(dmaNo);
    while (*dmaCntp & 0x80000000) {
    }
    func_01ffcdfc(dmaNo, src, dest, (size / 2) | 0x80000000);
    while (*dmaCntp & 0x80000000) {
    }
}

void MI_DmaFill32Async(int dmaNo, void *dest, u32 data, u32 size, void (*callback)(void *), void *arg) {
    if (size == 0) {
        if (callback != NULL) {
            callback(arg);
        }
        return;
    }
    func_02007804(dmaNo);
    if (callback != NULL) {
        OSIntrMode e;
        OSi_EnterDmaCallback(dmaNo, callback, arg);
        e = OS_DisableInterrupts();
        MI_DMA_FILL(dmaNo) = data;
        func_01ffce6c(dmaNo, (u32) &MI_DMA_FILL(dmaNo), dest, (size / 4) | 0xc5000000);
        OS_RestoreInterrupts(e);
    } else {
        OSIntrMode e = OS_DisableInterrupts();
        MI_DMA_FILL(dmaNo) = data;
        func_01ffce6c(dmaNo, (u32) &MI_DMA_FILL(dmaNo), dest, (size / 4) | 0x85000000);
        OS_RestoreInterrupts(e);
    }
}

void MI_DmaCopy32Async(int dmaNo, const void *src, void *dest, u32 size, void (*callback)(void *), void *arg) {
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, 0);
    if (size == 0) {
        if (callback != NULL) {
            callback(arg);
        }
        return;
    }
    func_02007804(dmaNo);
    if (callback != NULL) {
        OSi_EnterDmaCallback(dmaNo, callback, arg);
        func_01ffcdbc(dmaNo, src, dest, (size / 4) | 0xc4000000);
    } else {
        func_01ffcdbc(dmaNo, src, dest, (size / 4) | 0x84000000);
    }
}

void func_02007804(int dmaNo) {
    OSIntrMode e = OS_DisableInterrupts();
    vu32 *dmaCntp = &MI_DMA_CNT(dmaNo);
    while (*dmaCntp & 0x80000000) {
    }
    if (dmaNo == 0) {
        vu32 *dma = (vu32 *) (MI_DMA_BASE + dmaNo * 12 + 0xb0);
        dma[0] = 0;
        dma[1] = 0;
        dma[2] = 0x81400001;
    }
    OS_RestoreInterrupts(e);
}

void MI_StopDma(int dmaNo) {
    OSIntrMode e = OS_DisableInterrupts();
    vu16 *cntH = (vu16 *) (MI_DMA_BASE + (dmaNo * 6 + 5) * 2 + 0xb0);
    *cntH &= ~0x3a00;
    *cntH &= ~0x8000;
    (void) *cntH;
    (void) *cntH;
    if (dmaNo == 0) {
        vu32 *dma = (vu32 *) (MI_DMA_BASE + dmaNo * 12 + 0xb0);
        dma[0] = 0;
        dma[1] = 0;
        dma[2] = 0x81400001;
    }
    OS_RestoreInterrupts(e);
}

void MI_StopAllDma(void) {
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
}

void MIi_CheckAnotherAutoDMA(int dmaNo, u32 mode) {
    int i;
    vu32 *cntp = (vu32 *) 0x40000b8;
    for (i = 0; i < 3; i++, cntp += 3) {
        u32 cnt;
        if (i == dmaNo) {
            continue;
        }
        cnt = *cntp;
        if (!(cnt & 0x80000000)) {
            continue;
        }
        cnt &= 0x38000000;
        if (cnt == mode) {
            continue;
        }
        if (cnt == 0x8000000 && mode == 0x10000000) {
            continue;
        }
        if (cnt == 0x10000000 && mode == 0x8000000) {
            continue;
        }
        if (cnt == 0x18000000 || cnt == 0x20000000 || cnt == 0x28000000 || cnt == 0x30000000
            || cnt == 0x38000000 || cnt == 0x8000000 || cnt == 0x10000000) {
            OS_Terminate();
        }
    }
}

void MIi_CheckDma0SourceAddress(int dmaNo, u32 src, u32 size, u32 dir) {
    u32 srcHi;
    u32 endHi;
    if (dmaNo != 0) {
        return;
    }
    srcHi = src & 0xff000000;
    if (dir != 0) {
        if (dir == 0x800000) {
            src -= size;
        }
    } else {
        src += size;
    }
    endHi = src & 0xff000000;
    if (srcHi == 0x4000000 || srcHi >= 0x8000000 || endHi == 0x4000000 || endHi >= 0x8000000) {
        OS_Terminate();
    }
}
