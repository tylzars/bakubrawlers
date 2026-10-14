#include <nitro/internal.h>

u32 GX_DisableBankForBGExtPltt(void);
u32 GX_DisableBankForOBJExtPltt(void);
u32 GX_DisableBankForSubBGExtPltt(void);
u32 GX_DisableBankForSubOBJExtPltt(void);

typedef struct {
    u32 subBGBank;
    u32 objVram;
    u32 objBank;
    u32 bgOffset;
    u32 bgVram;
    u32 bgBank;
    u32 subOBJBank;
} GXExtPlttState;
extern GXExtPlttState data_020bdc24;

void GX_BeginLoadBGExtPltt(void) {
    int bank = GX_DisableBankForBGExtPltt();
    data_020bdc24.bgBank = bank;
    switch (bank) {
    case 0:
        break;
    case 0x10:
        data_020bdc24.bgVram = 0x06880000;
        data_020bdc24.bgOffset = 0;
        break;
    case 0x40:
        data_020bdc24.bgVram = 0x06894000;
        data_020bdc24.bgOffset = 0x4000;
        break;
    case 0x20:
    case 0x60:
        data_020bdc24.bgVram = 0x06890000;
        data_020bdc24.bgOffset = 0;
        break;
    }
}

void GX_LoadBGExtPltt(const void *src, u32 offset, u32 size) {
    u32 dst = data_020bdc24.bgVram + offset - data_020bdc24.bgOffset;
    if (data_020a4f24 != -1) {
        MI_DmaCopy32Async(data_020a4f24, src, (void *) dst, size, 0, 0);
    } else {
        MIi_CpuCopy32(src, (void *) dst, size);
    }
}

void GX_EndLoadBGExtPltt(void) {
    if (data_020a4f24 != -1) {
        func_02007804(data_020a4f24);
    }
    GX_SetBankForBGExtPltt(data_020bdc24.bgBank);
    data_020bdc24.bgBank = 0;
    data_020bdc24.bgVram = 0;
    data_020bdc24.bgOffset = 0;
}

void GX_BeginLoadOBJExtPltt(void) {
    int bank = GX_DisableBankForOBJExtPltt();
    data_020bdc24.objBank = bank;
    switch (bank) {
    case 0:
        break;
    case 0x40:
        data_020bdc24.objVram = 0x06894000;
        break;
    case 0x20:
        data_020bdc24.objVram = 0x06890000;
        break;
    }
}

void GX_LoadOBJExtPltt(const void *src, u32 offset, u32 size) {
    u32 dst = data_020bdc24.objVram;
    if (data_020a4f24 != -1) {
        MI_DmaCopy32Async(data_020a4f24, src, (void *) (dst + offset), size, 0, 0);
    } else {
        MIi_CpuCopy32(src, (void *) (dst + offset), size);
    }
}

void GX_EndLoadOBJExtPltt(void) {
    if (data_020a4f24 != -1) {
        func_02007804(data_020a4f24);
    }
    GX_SetBankForOBJExtPltt(data_020bdc24.objBank);
    data_020bdc24.objBank = 0;
    data_020bdc24.objVram = 0;
}

void GX_BeginLoadSubBGExtPltt(void) {
    data_020bdc24.subBGBank = GX_DisableBankForSubBGExtPltt();
}

void GX_LoadSubBGExtPltt(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1) {
        MI_DmaCopy32Async(data_020a4f24, src, (void *) (0x06898000 + offset), size, 0, 0);
    } else {
        MIi_CpuCopy32(src, (void *) (0x06898000 + offset), size);
    }
}

void GX_EndLoadSubBGExtPltt(void) {
    if (data_020a4f24 != -1) {
        func_02007804(data_020a4f24);
    }
    GX_SetBankForSubBGExtPltt(data_020bdc24.subBGBank);
    data_020bdc24.subBGBank = 0;
}

void GX_BeginLoadSubOBJExtPltt(void) {
    data_020bdc24.subOBJBank = GX_DisableBankForSubOBJExtPltt();
}

void GX_LoadSubOBJExtPltt(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1) {
        MI_DmaCopy32Async(data_020a4f24, src, (void *) (0x068a0000 + offset), size, 0, 0);
    } else {
        MIi_CpuCopy32(src, (void *) (0x068a0000 + offset), size);
    }
}

void GX_EndLoadSubOBJExtPltt(void) {
    if (data_020a4f24 != -1) {
        func_02007804(data_020a4f24);
    }
    GX_SetBankForSubOBJExtPltt(data_020bdc24.subOBJBank);
    data_020bdc24.subOBJBank = 0;
}
