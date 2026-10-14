#include <nitro/internal.h>

u32 GX_DisableBankForTex(void);
u32 GX_DisableBankForTexPltt(void);

typedef struct {
    u32 unk0;
    u32 texAddr;
    u32 plttAddr;
    u32 plttBank;
    u32 unk10;
    u32 texBank;
    u32 tex2Addr;
    u32 texLimit;
} GXTexState;
extern GXTexState data_020bdc40;
extern const u16 data_0208f73c[][3]; // per bank layout: first address, second address, first limit (in 4KB)
extern u16 data_0208f72c[];

void GX_BeginLoadTex(void) {
    u32 bank = GX_DisableBankForTex();
    u32 a = data_0208f73c[bank][0];
    u32 b = data_0208f73c[bank][1];
    u32 c = data_0208f73c[bank][2];
    data_020bdc40.texBank = bank;
    data_020bdc40.texAddr = a << 12;
    data_020bdc40.tex2Addr = b << 12;
    data_020bdc40.texLimit = c << 12;
}

void GX_LoadTex(const void *src, u32 offset, u32 size) {
    u32 tex2 = data_020bdc40.tex2Addr;
    u32 dst;
    u32 limit;
    u32 first;
    u32 base;
    if (tex2 == 0) {
        dst = data_020bdc40.texAddr + offset;
    } else {
        limit = data_020bdc40.texLimit;
        if (offset + size < limit) {
            dst = data_020bdc40.texAddr + offset;
        } else if (offset >= limit) {
            dst = tex2 + offset - limit;
        } else {
            first = limit - offset;
            base = data_020bdc40.texAddr;
            if (data_020a4f24 != -1 && first > 0x30) {
                MI_DmaCopy32(data_020a4f24, src, (void *) (base + offset), first);
            } else {
                func_01ff9510(src, (void *) (base + offset), first);
            }
            if (data_020a4f24 != -1) {
                MI_DmaCopy32Async(data_020a4f24, (const u8 *) src + first, (void *) tex2, size - first, 0, 0);
            } else {
                func_01ff9510((const u8 *) src + first, (void *) tex2, size - first);
            }
            return;
        }
    }
    if (data_020a4f24 != -1) {
        MI_DmaCopy32Async(data_020a4f24, src, (void *) dst, size, 0, 0);
    } else {
        func_01ff9510(src, (void *) dst, size);
    }
}

void GX_EndLoadTex(void) {
    if (data_020a4f24 != -1) {
        func_02007804(data_020a4f24);
    }
    GX_SetBankForTex(data_020bdc40.texBank);
    data_020bdc40.texLimit = 0;
    data_020bdc40.tex2Addr = 0;
    data_020bdc40.texAddr = 0;
    data_020bdc40.texBank = 0;
}

void GX_BeginLoadTexPltt(void) {
    u32 bank = GX_DisableBankForTexPltt();
    data_020bdc40.plttBank = bank;
    data_020bdc40.plttAddr = data_0208f72c[(int) bank >> 4] << 12;
}

void GX_LoadTexPltt(const void *src, u32 offset, u32 size) {
    u32 dst = data_020bdc40.plttAddr;
    if (data_020a4f24 != -1) {
        MI_DmaCopy32Async(data_020a4f24, src, (void *) (dst + offset), size, 0, 0);
    } else {
        func_01ff9510(src, (void *) (dst + offset), size);
    }
}

void GX_EndLoadTexPltt(void) {
    if (data_020a4f24 != -1) {
        func_02007804(data_020a4f24);
    }
    GX_SetBankForTexPltt(data_020bdc40.plttBank);
    data_020bdc40.plttBank = 0;
    data_020bdc40.plttAddr = 0;
}
