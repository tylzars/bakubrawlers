#include <nitro/internal.h>

void *G2_GetBG0ScrPtr(void) {
    int scr = (reg_G2_BG0CNT & 0x1f00) >> 8;
    u32 base = (reg_GX_DISPCNT_ & 0x38000000) >> 27;
    return (void *) (HW_BG_VRAM + (base << 16) + (scr << 11));
}

void *G2S_GetBG0ScrPtr(void) {
    int scr = (reg_G2S_DB_BG0CNT & 0x1f00) >> 8;
    return (void *) (HW_DB_BG_VRAM + (scr << 11));
}

void *G2_GetBG1ScrPtr(void) {
    int scr = (reg_G2_BG1CNT & 0x1f00) >> 8;
    u32 base = (reg_GX_DISPCNT_ & 0x38000000) >> 27;
    return (void *) (HW_BG_VRAM + (base << 16) + (scr << 11));
}

void *G2S_GetBG1ScrPtr(void) {
    int scr = (reg_G2S_DB_BG1CNT & 0x1f00) >> 8;
    return (void *) (HW_DB_BG_VRAM + (scr << 11));
}

void *G2_GetBG2ScrPtr(void) {
    u32 mode = reg_GX_DISPCNT_ & 7;
    u32 cnt = reg_G2_BG2CNT;
    u32 base = ((reg_GX_DISPCNT_ & 0x38000000) >> 27) << 16;
    u32 scr = (cnt & 0x1f00) >> 8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return (void *) (HW_BG_VRAM + base + (scr << 11));
    case 5:
        if (cnt & 0x80) {
            return (void *) (HW_BG_VRAM + (scr << 14));
        }
        return (void *) (HW_BG_VRAM + base + (scr << 11));
    case 6:
        return (void *) HW_BG_VRAM;
    default:
        return 0;
    }
}

void *G2S_GetBG2ScrPtr(void) {
    u32 mode = reg_GXS_DISPCNT & 7;
    u32 cnt = reg_G2S_DB_BG2CNT;
    u32 scr = (cnt & 0x1f00) >> 8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return (void *) (HW_DB_BG_VRAM + (scr << 11));
    case 5:
        if (cnt & 0x80) {
            return (void *) (HW_DB_BG_VRAM + (scr << 14));
        }
        return (void *) (HW_DB_BG_VRAM + (scr << 11));
    case 6:
        return 0;
    default:
        return 0;
    }
}

void *G2_GetBG3ScrPtr(void) {
    u32 mode = reg_GX_DISPCNT_ & 7;
    u32 cnt = reg_G2_BG3CNT;
    u32 base = ((reg_GX_DISPCNT_ & 0x38000000) >> 27) << 16;
    u32 scr = (cnt & 0x1f00) >> 8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        return (void *) (HW_BG_VRAM + base + (scr << 11));
    case 3:
    case 4:
    case 5:
        if (cnt & 0x80) {
            return (void *) (HW_BG_VRAM + (scr << 14));
        }
        return (void *) (HW_BG_VRAM + base + (scr << 11));
    case 6:
        return 0;
    default:
        return 0;
    }
}

void *G2S_GetBG3ScrPtr(void) {
    u32 mode = reg_GXS_DISPCNT & 7;
    u32 cnt = reg_G2S_DB_BG3CNT;
    u32 scr = (cnt & 0x1f00) >> 8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        return (void *) (HW_DB_BG_VRAM + (scr << 11));
    case 3:
    case 4:
    case 5:
        if (cnt & 0x80) {
            return (void *) (HW_DB_BG_VRAM + (scr << 14));
        }
        return (void *) (HW_DB_BG_VRAM + (scr << 11));
    case 6:
        return 0;
    default:
        return 0;
    }
}
