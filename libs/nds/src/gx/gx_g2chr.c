#include <nitro/internal.h>

void *G2_GetBG0CharPtr(void) {
    int chr = (reg_G2_BG0CNT & 0x3c) >> 2;
    u32 base = (reg_GX_DISPCNT_ & 0x07000000) >> 24;
    return (void *) (HW_BG_VRAM + (base << 16) + (chr << 14));
}

void *G2S_GetBG0CharPtr(void) {
    int chr = (reg_G2S_DB_BG0CNT & 0x3c) >> 2;
    return (void *) (HW_DB_BG_VRAM + (chr << 14));
}

void *G2_GetBG1CharPtr(void) {
    int chr = (reg_G2_BG1CNT & 0x3c) >> 2;
    u32 base = (reg_GX_DISPCNT_ & 0x07000000) >> 24;
    return (void *) (HW_BG_VRAM + (base << 16) + (chr << 14));
}

void *G2S_GetBG1CharPtr(void) {
    int chr = (reg_G2S_DB_BG1CNT & 0x3c) >> 2;
    return (void *) (HW_DB_BG_VRAM + (chr << 14));
}

void *G2_GetBG2CharPtr(void) {
    int mode = reg_GX_DISPCNT_ & 7;
    u32 cnt = reg_G2_BG2CNT;
    if (mode < 5 || !(cnt & 0x80)) {
        return (void *) (HW_BG_VRAM + (((reg_GX_DISPCNT_ & 0x07000000) >> 24) << 16) + (((cnt & 0x3c) >> 2) << 14));
    }
    return 0;
}

void *G2S_GetBG2CharPtr(void) {
    int mode = reg_GXS_DISPCNT & 7;
    u32 cnt = reg_G2S_DB_BG2CNT;
    if (mode < 5 || !(cnt & 0x80)) {
        return (void *) (HW_DB_BG_VRAM + (((cnt & 0x3c) >> 2) << 14));
    }
    return 0;
}

void *G2_GetBG3CharPtr(void) {
    int mode = reg_GX_DISPCNT_ & 7;
    u32 cnt = reg_G2_BG3CNT;
    if (mode < 3 || (mode < 6 && !(cnt & 0x80))) {
        return (void *) (HW_BG_VRAM + (((reg_GX_DISPCNT_ & 0x07000000) >> 24) << 16) + (((cnt & 0x3c) >> 2) << 14));
    }
    return 0;
}

void *G2S_GetBG3CharPtr(void) {
    int mode = reg_GXS_DISPCNT & 7;
    u32 cnt = reg_G2S_DB_BG3CNT;
    if (mode < 3 || (mode < 6 && !(cnt & 0x80))) {
        return (void *) (HW_DB_BG_VRAM + (((cnt & 0x3c) >> 2) << 14));
    }
    return 0;
}
