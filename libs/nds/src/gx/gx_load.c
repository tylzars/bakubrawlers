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

void GX_LoadBGPltt(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, (void *) (0x05000000 + offset), size);
    } else {
        func_01ff94e0(src, (void *) (0x05000000 + offset), size);
    }
}

void GX_LoadOBJPltt(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, (void *) (0x05000400 + offset), size);
    } else {
        func_01ff94e0(src, (void *) (0x05000400 + offset), size);
    }
}

void GXS_LoadBGPltt(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, (void *) (0x05000200 + offset), size);
    } else {
        func_01ff94e0(src, (void *) (0x05000200 + offset), size);
    }
}

void GXS_LoadOBJPltt(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, (void *) (0x05000600 + offset), size);
    } else {
        func_01ff94e0(src, (void *) (0x05000600 + offset), size);
    }
}

void GX_LoadOAM(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, (void *) (0x07000000 + offset), size);
    } else {
        func_01ff9510(src, (void *) (0x07000000 + offset), size);
    }
}

void GXS_LoadOAM(const void *src, u32 offset, u32 size) {
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, (void *) (0x07000400 + offset), size);
    } else {
        func_01ff9510(src, (void *) (0x07000400 + offset), size);
    }
}

void GX_LoadOBJ(const void *src, u32 offset, u32 size) {
    u32 dst = 0x06400000;
    dst += offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, (void *) dst, size);
    } else {
        func_01ff9510(src, (void *) dst, size);
    }
}

void GXS_LoadOBJ(const void *src, u32 offset, u32 size) {
    u32 dst = 0x06600000;
    dst += offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, (void *) dst, size);
    } else {
        func_01ff9510(src, (void *) dst, size);
    }
}

void GX_LoadBG0Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG0ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, dst, size);
    } else {
        func_01ff94e0(src, dst, size);
    }
}

void GXS_LoadBG0Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG0ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, dst, size);
    } else {
        func_01ff94e0(src, dst, size);
    }
}

void GX_LoadBG1Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG1ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, dst, size);
    } else {
        func_01ff94e0(src, dst, size);
    }
}

void GXS_LoadBG1Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG1ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, dst, size);
    } else {
        func_01ff94e0(src, dst, size);
    }
}

void GX_LoadBG2Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG2ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, dst, size);
    } else {
        func_01ff94e0(src, dst, size);
    }
}

void GXS_LoadBG2Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG2ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, dst, size);
    } else {
        func_01ff94e0(src, dst, size);
    }
}

void GX_LoadBG3Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG3ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, dst, size);
    } else {
        func_01ff94e0(src, dst, size);
    }
}

void GXS_LoadBG3Scr(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG3ScrPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x1c) {
        func_0200763c(data_020a4f24, src, dst, size);
    } else {
        func_01ff94e0(src, dst, size);
    }
}

void GX_LoadBG0Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG0CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, dst, size);
    } else {
        func_01ff9510(src, dst, size);
    }
}

void GXS_LoadBG0Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG0CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, dst, size);
    } else {
        func_01ff9510(src, dst, size);
    }
}

void GX_LoadBG1Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG1CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, dst, size);
    } else {
        func_01ff9510(src, dst, size);
    }
}

void GXS_LoadBG1Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG1CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, dst, size);
    } else {
        func_01ff9510(src, dst, size);
    }
}

void GX_LoadBG2Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG2CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, dst, size);
    } else {
        func_01ff9510(src, dst, size);
    }
}

void GXS_LoadBG2Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG2CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, dst, size);
    } else {
        func_01ff9510(src, dst, size);
    }
}

void GX_LoadBG3Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2_GetBG3CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, dst, size);
    } else {
        func_01ff9510(src, dst, size);
    }
}

void GXS_LoadBG3Char(const void *src, u32 offset, u32 size) {
    void *dst = (u8 *) G2S_GetBG3CharPtr() + offset;
    if (data_020a4f24 != -1 && size > 0x30) {
        func_020075cc(data_020a4f24, src, dst, size);
    } else {
        func_01ff9510(src, dst, size);
    }
}
