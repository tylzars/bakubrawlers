#include <nitro/internal.h>

extern u16 data_020bdc04[]; // [0] display mode, [1] VRAM lock id
typedef struct {
    u16 dispOn;
    s32 dmaNo; // data_020a4f20.dmaNo, NOT_USE = -1
} GxDispState;
extern GxDispState data_020a4f20;


void GX_Init(void) {
    reg_GX_POWCNT |= 0x8000;
    reg_GX_POWCNT = (u16) ((reg_GX_POWCNT & ~0x20e) | 0xe | 0x200);
    reg_GX_POWCNT |= 1;
    GX_InitGXState();
    if (data_020bdc04[1] == 0) {
        do {
            u32 id = func_02004f54();
            if (id == -3) {
                func_02007520();
            }
            data_020bdc04[1] = (u16) id;
        } while (data_020bdc04[1] == 0);
    }
    reg_GX_DISPSTAT = 0;
    reg_GX_DISPCNT_ = 0;
    if (data_020a4f20.dmaNo != -1) {
        func_0200754c(data_020a4f20.dmaNo, (void *) 0x04000008, 0, 0x60);
        *(vu16 *) 0x0400006c = 0;
        func_0200754c(data_020a4f20.dmaNo, (void *) 0x04001000, 0, 0x70);
    } else {
        MIi_CpuClear32(0, (void *) 0x04000008, 0x60);
        *(vu16 *) 0x0400006c = 0;
        MIi_CpuClear32(0, (void *) 0x04001000, 0x70);
    }
    *(vu16 *) 0x04000020 = 0x100;
    *(vu16 *) 0x04000026 = 0x100;
    *(vu16 *) 0x04000030 = 0x100;
    *(vu16 *) 0x04000036 = 0x100;
    *(vu16 *) 0x04001020 = 0x100;
    *(vu16 *) 0x04001026 = 0x100;
    *(vu16 *) 0x04001030 = 0x100;
    *(vu16 *) 0x04001036 = 0x100;
}

u16 GX_HBlankIntr(BOOL enable) {
    u16 old = reg_GX_DISPSTAT & 0x10;
    if (enable) {
        reg_GX_DISPSTAT |= 0x10;
    } else {
        reg_GX_DISPSTAT &= ~0x10;
    }
    return old;
}

u16 GX_VBlankIntr(BOOL enable) {
    u16 old = reg_GX_DISPSTAT & 8;
    if (enable) {
        reg_GX_DISPSTAT |= 8;
    } else {
        reg_GX_DISPSTAT &= ~8;
    }
    return old;
}

void GX_DispOn(void) {
    u16 mode = data_020bdc04[0];
    data_020a4f20.dispOn = 1;
    if (mode == 0) {
        reg_GX_DISPCNT_ |= 0x10000;
    } else {
        reg_GX_DISPCNT_ = (reg_GX_DISPCNT_ & ~0x30000) | (mode << 16);
    }
}

void GX_SetGraphicsMode(u32 dispMode, u32 bgMode, u32 bg0_2d3d) {
    u16 on = data_020a4f20.dispOn;
    u32 cur = reg_GX_DISPCNT_;
    data_020bdc04[0] = (u16) dispMode;
    if (on == 0) {
        dispMode = 0;
    }
    reg_GX_DISPCNT_ = (cur & 0xfff0fff0) | (dispMode << 16) | bgMode | (bg0_2d3d << 3);
    if (data_020bdc04[0] == 0) {
        data_020a4f20.dispOn = 0;
    }
}

void GXS_SetGraphicsMode(u32 mode) {
    reg_GXS_DISPCNT = (reg_GXS_DISPCNT & ~7) | mode;
}

void G2x_SetMasterBrightness_(vu16 *reg, int brightness) {
    if (brightness == 0) {
        *reg = 0;
    } else if (brightness > 0) {
        *reg = (u16) (brightness | 0x4000);
    } else {
        *reg = (u16) (-brightness | 0x8000);
    }
}

int G2x_GetMasterBrightness_(vu16 *reg) {
    u16 m = *reg & 0xc000;
    if (m == 0) {
        return 0;
    } else if (m == 0x4000) {
        return *reg & 0x1f;
    } else if (m == 0x8000) {
        return -(*reg & 0x1f);
    } else {
        return 0;
    }
}

void GX_InitGXState(void) {
    data_020bdc08[0] = 0;
    data_020bdc08[1] = 0;
    data_020bdc08[2] = 0;
    data_020bdc08[3] = 0;
    data_020bdc08[4] = 0;
    data_020bdc08[5] = 0;
    data_020bdc08[6] = 0;
    data_020bdc08[7] = 0;
    data_020bdc08[8] = 0;
    data_020bdc08[9] = 0;
    data_020bdc08[10] = 0;
    data_020bdc08[11] = 0;
    data_020bdc08[12] = 0;
    *(vu32 *) 0x04000240 = 0;
    *(vu8 *) 0x04000244 = 0;
    *(vu8 *) 0x04000245 = 0;
    *(vu8 *) 0x04000246 = 0;
    *(vu16 *) 0x04000248 = 0;
}

void func_02001fb8(int banks) {
    if (banks & 1) {
        reg_GX_VRAMCNT_A = 0x80;
    }
    if (banks & 2) {
        reg_GX_VRAMCNT_B = 0x80;
    }
    if (banks & 4) {
        reg_GX_VRAMCNT_C = 0x80;
    }
    if (banks & 8) {
        reg_GX_VRAMCNT_D = 0x80;
    }
    if (banks & 0x10) {
        reg_GX_VRAMCNT_E = 0x80;
    }
    if (banks & 0x20) {
        reg_GX_VRAMCNT_F = 0x80;
    }
    if (banks & 0x40) {
        reg_GX_VRAMCNT_G = 0x80;
    }
    if (banks & 0x80) {
        reg_GX_VRAMCNT_H = 0x80;
    }
    if (banks & 0x100) {
        reg_GX_VRAMCNT_I = 0x80;
    }
}
