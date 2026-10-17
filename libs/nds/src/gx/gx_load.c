#include <nitro/internal.h>

void *G2_GetBG0ScrPtr(void);
void *G2S_GetBG0ScrPtr(void);
void *G2_GetBG1ScrPtr(void);
void *G2S_GetBG1ScrPtr(void);
void *G2_GetBG2ScrPtr(void);
void *G2S_GetBG2ScrPtr(void);
void *G2_GetBG3ScrPtr(void);
void *G2S_GetBG3ScrPtr(void);
void *G2_GetBG0CharPtr(void);
void *G2S_GetBG0CharPtr(void);
void *G2_GetBG1CharPtr(void);
void *G2S_GetBG1CharPtr(void);
void *G2_GetBG2CharPtr(void);
void *G2S_GetBG2CharPtr(void);
void *G2_GetBG3CharPtr(void);
void *G2S_GetBG3CharPtr(void);

// The four palette loaders. Their names follow gUploadFuncs (UploadQueue.cpp): slots 0-14 are screen 0 (GX_) and
// 15-29 screen 1 (GXS_ or GX_LoadSub..., GX_LoadTex / GX_LoadTexPltt on both; screen 0 / 1 as SpriteScreen::FlushOam's
// GX_LoadOAM / GXS_LoadOAM split), slot % 15 is the kind. Slot 6 (kind 6, BG palette) writes 0x05000000 and slot 21
// 0x05000400; slot 5 (kind 5, OBJ palette) writes 0x05000200 and slot 20 0x05000600. So each screen's palettes sit
// in one 0x400 block, as GX_LoadOAM / GXS_LoadOAM below (0x07000000 / 0x07000400), and MoviePlayer.cpp clears
// 0x5000000 and 0x5000400 together. Kept in address order.
void GX_LoadBGPltt(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, (void *) (0x05000000 + offset), size);
    } else {
        MIi_CpuCopy16(src, (void *) (0x05000000 + offset), size);
    }
}

void GXS_LoadBGPltt(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, (void *) (0x05000400 + offset), size);
    } else {
        MIi_CpuCopy16(src, (void *) (0x05000400 + offset), size);
    }
}

void GX_LoadOBJPltt(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, (void *) (0x05000200 + offset), size);
    } else {
        MIi_CpuCopy16(src, (void *) (0x05000200 + offset), size);
    }
}

void GXS_LoadOBJPltt(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, (void *) (0x05000600 + offset), size);
    } else {
        MIi_CpuCopy16(src, (void *) (0x05000600 + offset), size);
    }
}

void GX_LoadOAM(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, (void *) (0x07000000 + offset), size);
    } else {
        MIi_CpuCopy32(src, (void *) (0x07000000 + offset), size);
    }
}

void GXS_LoadOAM(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, (void *) (0x07000400 + offset), size);
    } else {
        MIi_CpuCopy32(src, (void *) (0x07000400 + offset), size);
    }
}

void GX_LoadOBJ(const void *src, u32 offset, u32 size) {
    u32 dst = 0x06400000;
    dst += offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, (void *) dst, size);
    } else {
        MIi_CpuCopy32(src, (void *) dst, size);
    }
}

void GXS_LoadOBJ(const void *src, u32 offset, u32 size) {
    u32 dst = 0x06600000;
    dst += offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, (void *) dst, size);
    } else {
        MIi_CpuCopy32(src, (void *) dst, size);
    }
}

void GX_LoadBG0Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG0ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy16(src, dst, size);
    }
}

void GXS_LoadBG0Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG0ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy16(src, dst, size);
    }
}

void GX_LoadBG1Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG1ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy16(src, dst, size);
    }
}

void GXS_LoadBG1Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG1ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy16(src, dst, size);
    }
}

void GX_LoadBG2Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG2ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy16(src, dst, size);
    }
}

void GXS_LoadBG2Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG2ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy16(src, dst, size);
    }
}

void GX_LoadBG3Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG3ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy16(src, dst, size);
    }
}

void GXS_LoadBG3Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG3ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        MI_DmaCopy16(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy16(src, dst, size);
    }
}

void GX_LoadBG0Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG0CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

void GXS_LoadBG0Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG0CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

void GX_LoadBG1Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG1CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

void GXS_LoadBG1Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG1CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

void GX_LoadBG2Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG2CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

void GXS_LoadBG2Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG2CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

void GX_LoadBG3Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG3CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}

void GXS_LoadBG3Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG3CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        MI_DmaCopy32(data_020a4f24, src, dst, size);
    } else {
        MIi_CpuCopy32(src, dst, size);
    }
}
