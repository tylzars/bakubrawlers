#include <nitro/internal.h>
extern u16 data_020bdc08[];



void GX_SetBankForBG(int bg) {
    data_020bdc08[0] = (u16) ((data_020bdc08[0] | data_020bdc08[1]) & ~bg);
    data_020bdc08[1] = (u16) bg;
    switch (bg) {
    case 8:
        reg_GX_VRAMCNT_D = 0x81;
        break;
    case 12:
        reg_GX_VRAMCNT_D = 0x89;
    case 4:
        reg_GX_VRAMCNT_C = 0x81;
        break;
    case 14:
        reg_GX_VRAMCNT_D = 0x91;
    case 6:
        reg_GX_VRAMCNT_C = 0x89;
    case 2:
        reg_GX_VRAMCNT_B = 0x81;
        break;
    case 15:
        reg_GX_VRAMCNT_D = 0x99;
    case 7:
        reg_GX_VRAMCNT_C = 0x91;
    case 3:
        reg_GX_VRAMCNT_B = 0x89;
    case 1:
        reg_GX_VRAMCNT_A = 0x81;
        break;
    case 11:
        *(vu8 *) 0x04000240 = 0x81;
        *(vu8 *) 0x04000241 = 0x89;
        *(vu8 *) 0x04000243 = 0x91;
        break;
    case 13:
        reg_GX_VRAMCNT_D = 0x91;
    case 5:
        *(vu8 *) 0x04000240 = 0x81;
        *(vu8 *) 0x04000242 = 0x89;
        break;
    case 9:
        *(vu8 *) 0x04000240 = 0x81;
        *(vu8 *) 0x04000243 = 0x89;
        break;
    case 10:
        *(vu8 *) 0x04000241 = 0x81;
        *(vu8 *) 0x04000243 = 0x89;
        break;
    case 0x70:
        reg_GX_VRAMCNT_G = 0x99;
    case 0x30:
        reg_GX_VRAMCNT_F = 0x91;
    case 16:
        reg_GX_VRAMCNT_E = 0x81;
        break;
    case 0x50:
        *(vu8 *) 0x04000246 = 0x91;
        *(vu8 *) 0x04000244 = 0x81;
        break;
    case 0x60:
        reg_GX_VRAMCNT_G = 0x89;
    case 32:
        reg_GX_VRAMCNT_F = 0x81;
        break;
    case 0x40:
        reg_GX_VRAMCNT_G = 0x81;
        break;
    }
    func_02001fb8(data_020bdc08[0]);
}

void GX_SetBankForOBJ(int obj) {
    data_020bdc08[0] = (u16) ((data_020bdc08[0] | data_020bdc08[2]) & ~obj);
    data_020bdc08[2] = (u16) obj;
    switch (obj) {
    case 0:
        break;
    case 3:
        reg_GX_VRAMCNT_B = 0x8a;
    case 1:
        reg_GX_VRAMCNT_A = 0x82;
        break;
    case 2:
        reg_GX_VRAMCNT_B = 0x82;
        break;
    case 0x70:
        reg_GX_VRAMCNT_G = 0x9a;
    case 0x30:
        reg_GX_VRAMCNT_F = 0x92;
    case 0x10:
        reg_GX_VRAMCNT_E = 0x82;
        break;
    case 0x50:
        *(vu8 *) 0x04000246 = 0x92;
        *(vu8 *) 0x04000244 = 0x82;
        break;
    case 0x60:
        reg_GX_VRAMCNT_G = 0x8a;
    case 0x20:
        reg_GX_VRAMCNT_F = 0x82;
        break;
    case 0x40:
        reg_GX_VRAMCNT_G = 0x82;
        break;
    }
    func_02001fb8(data_020bdc08[0]);
}

void GX_SetBankForBGExtPltt(int bg) {
    data_020bdc08[0] = (u16) ((data_020bdc08[0] | data_020bdc08[7]) & ~bg);
    data_020bdc08[7] = (u16) bg;
    switch (bg) {
    case 0x10:
        reg_GX_DISPCNT_ |= 0x40000000;
        reg_GX_VRAMCNT_E = 0x84;
        break;
    case 0x40:
        reg_GX_DISPCNT_ |= 0x40000000;
        reg_GX_VRAMCNT_G = 0x8c;
        break;
    case 0x60:
        reg_GX_VRAMCNT_G = 0x8c;
    case 0x20:
        reg_GX_VRAMCNT_F = 0x84;
        reg_GX_DISPCNT_ |= 0x40000000;
        break;
    case 0:
        reg_GX_DISPCNT_ &= ~0x40000000;
        break;
    }
    func_02001fb8(data_020bdc08[0]);
}

void GX_SetBankForOBJExtPltt(int obj) {
    data_020bdc08[0] = (u16) ((data_020bdc08[0] | data_020bdc08[8]) & ~obj);
    data_020bdc08[8] = (u16) obj;
    switch (obj) {
    case 0x20:
        reg_GX_DISPCNT_ |= 0x80000000;
        reg_GX_VRAMCNT_F = 0x85;
        break;
    case 0x40:
        reg_GX_DISPCNT_ |= 0x80000000;
        reg_GX_VRAMCNT_G = 0x85;
        break;
    case 0:
        reg_GX_DISPCNT_ &= ~0x80000000;
        break;
    }
    func_02001fb8(data_020bdc08[0]);
}

#define reg_GX_DISP3DCNT (*(vu16 *) 0x04000060)

void GX_SetBankForTex(int tex) {
    data_020bdc08[0] = (u16) ((data_020bdc08[0] | data_020bdc08[4]) & ~tex);
    data_020bdc08[4] = (u16) tex;
    if (tex == 0) {
        reg_GX_DISP3DCNT = (u16) (reg_GX_DISP3DCNT & 0xcffe);
    } else {
        reg_GX_DISP3DCNT = (u16) ((reg_GX_DISP3DCNT & ~0x3000) | 1);
        switch (tex) {
        case 5:
            reg_GX_VRAMCNT_A = 0x83;
            reg_GX_VRAMCNT_C = 0x8b;
            break;
        case 9:
            reg_GX_VRAMCNT_A = 0x83;
            reg_GX_VRAMCNT_D = 0x8b;
            break;
        case 10:
            *(vu8 *) 0x04000241 = 0x83;
            *(vu8 *) 0x04000243 = 0x8b;
            break;
        case 11:
            reg_GX_VRAMCNT_A = 0x83;
            *(vu8 *) 0x04000241 = 0x8b;
            *(vu8 *) 0x04000243 = 0x93;
            break;
        case 13:
            reg_GX_VRAMCNT_A = 0x83;
            *(vu8 *) 0x04000242 = 0x8b;
            *(vu8 *) 0x04000243 = 0x93;
            break;
        case 8:
            reg_GX_VRAMCNT_D = 0x83;
            break;
        case 12:
            reg_GX_VRAMCNT_D = 0x8b;
        case 4:
            reg_GX_VRAMCNT_C = 0x83;
            break;
        case 14:
            reg_GX_VRAMCNT_D = 0x93;
        case 6:
            reg_GX_VRAMCNT_C = 0x8b;
        case 2:
            reg_GX_VRAMCNT_B = 0x83;
            break;
        case 15:
            reg_GX_VRAMCNT_D = 0x9b;
        case 7:
            reg_GX_VRAMCNT_C = 0x93;
        case 3:
            reg_GX_VRAMCNT_B = 0x8b;
        case 1:
            reg_GX_VRAMCNT_A = 0x83;
            break;
        }
    }
    func_02001fb8(data_020bdc08[0]);
}

void GX_SetBankForTexPltt(int pltt) {
    data_020bdc08[0] = (u16) ((data_020bdc08[0] | data_020bdc08[5]) & ~pltt);
    data_020bdc08[5] = (u16) pltt;
    switch (pltt) {
    case 0x60:
        reg_GX_VRAMCNT_G = 0x8b;
    case 0x20:
        reg_GX_VRAMCNT_F = 0x83;
        break;
    case 0x40:
        reg_GX_VRAMCNT_G = 0x83;
        break;
    case 0x70:
        reg_GX_VRAMCNT_G = 0x9b;
    case 0x30:
        reg_GX_VRAMCNT_F = 0x93;
    case 0x10:
        reg_GX_VRAMCNT_E = 0x83;
    case 0:
        break;
    }
    func_02001fb8(data_020bdc08[0]);
}

void GX_SetBankForLCDC(int lcdc) {
    data_020bdc08[0] |= lcdc;
    func_02001fb8(lcdc);
}

void GX_SetBankForSubBG(int bg) {
    data_020bdc08[0] = (u16) ((data_020bdc08[0] | data_020bdc08[9]) & ~bg);
    data_020bdc08[9] = (u16) bg;
    switch (bg) {
    case 0:
        break;
    case 4:
        reg_GX_VRAMCNT_C = 0x84;
        break;
    case 0x180:
        reg_GX_VRAMCNT_I = 0x81;
    case 0x80:
        reg_GX_VRAMCNT_H = 0x81;
        break;
    }
    func_02001fb8(data_020bdc08[0]);
}

void GX_SetBankForSubOBJ(int obj) {
    data_020bdc08[0] = (u16) ((data_020bdc08[0] | data_020bdc08[10]) & ~obj);
    data_020bdc08[10] = (u16) obj;
    switch (obj) {
    case 0:
        break;
    case 8:
        reg_GX_VRAMCNT_D = 0x84;
        break;
    case 0x100:
        reg_GX_VRAMCNT_I = 0x82;
        break;
    }
    func_02001fb8(data_020bdc08[0]);
}

void GX_SetBankForSubBGExtPltt(int bg) {
    data_020bdc08[0] = (u16) ((data_020bdc08[0] | data_020bdc08[11]) & ~bg);
    data_020bdc08[11] = (u16) bg;
    switch (bg) {
    case 0x80:
        reg_GXS_DISPCNT |= 0x40000000;
        reg_GX_VRAMCNT_H = 0x82;
        break;
    case 0:
        reg_GXS_DISPCNT &= ~0x40000000;
        break;
    }
    func_02001fb8(data_020bdc08[0]);
}

void GX_SetBankForSubOBJExtPltt(int obj) {
    data_020bdc08[0] = (u16) ((data_020bdc08[0] | data_020bdc08[12]) & ~obj);
    data_020bdc08[12] = (u16) obj;
    switch (obj) {
    case 0x100:
        reg_GXS_DISPCNT |= 0x80000000;
        reg_GX_VRAMCNT_I = 0x83;
        break;
    case 0:
        reg_GXS_DISPCNT &= ~0x80000000;
        break;
    }
    func_02001fb8(data_020bdc08[0]);
}

u16 func_02002af4(u16 *banks) {
    u16 v = *banks;
    *banks = 0;
    data_020bdc08[0] |= v;
    func_02001fb8(v);
    return v;
}

void GX_DisableBankForBG(void) {
    func_02002af4(&data_020bdc08[1]);
}
void GX_DisableBankForOBJ(void) {
    func_02002af4(&data_020bdc08[2]);
}
void GX_DisableBankForBGExtPltt(void) {
    reg_GX_DISPCNT_ &= ~0x40000000;
    func_02002af4(&data_020bdc08[7]);
}
void GX_DisableBankForOBJExtPltt(void) {
    reg_GX_DISPCNT_ &= ~0x80000000;
    func_02002af4(&data_020bdc08[8]);
}
void GX_DisableBankForTex(void) {
    func_02002af4(&data_020bdc08[4]);
}
void GX_DisableBankForTexPltt(void) {
    func_02002af4(&data_020bdc08[5]);
}
void GX_DisableBankForSubBG(void) {
    func_02002af4(&data_020bdc08[9]);
}
void GX_DisableBankForSubOBJ(void) {
    func_02002af4(&data_020bdc08[10]);
}
void GX_DisableBankForSubBGExtPltt(void) {
    reg_GXS_DISPCNT &= ~0x40000000;
    func_02002af4(&data_020bdc08[11]);
}
void GX_DisableBankForSubOBJExtPltt(void) {
    reg_GXS_DISPCNT &= ~0x80000000;
    func_02002af4(&data_020bdc08[12]);
}

u32 func_02002c38(u16 *banks) {
    u32 v = *banks;
    *banks = 0;
    if (v & 1) {
        reg_GX_VRAMCNT_A = 0;
    }
    if (v & 2) {
        reg_GX_VRAMCNT_B = 0;
    }
    if (v & 4) {
        reg_GX_VRAMCNT_C = 0;
    }
    if (v & 8) {
        reg_GX_VRAMCNT_D = 0;
    }
    if (v & 0x10) {
        reg_GX_VRAMCNT_E = 0;
    }
    if (v & 0x20) {
        reg_GX_VRAMCNT_F = 0;
    }
    if (v & 0x40) {
        reg_GX_VRAMCNT_G = 0;
    }
    if (v & 0x80) {
        reg_GX_VRAMCNT_H = 0;
    }
    if (v & 0x100) {
        reg_GX_VRAMCNT_I = 0;
    }
    func_02007498(v, data_020bdc06[0]);
    return v;
}

void GX_ResetBankForBG(void) {
    func_02002c38(&data_020bdc08[1]);
}
void GX_ResetBankForOBJ(void) {
    func_02002c38(&data_020bdc08[2]);
}
void GX_ResetBankForBGExtPltt(void) {
    reg_GX_DISPCNT_ &= ~0x40000000;
    func_02002c38(&data_020bdc08[7]);
}
void GX_ResetBankForOBJExtPltt(void) {
    reg_GX_DISPCNT_ &= ~0x80000000;
    func_02002c38(&data_020bdc08[8]);
}
void GX_ResetBankForTex(void) {
    func_02002c38(&data_020bdc08[4]);
}
void GX_ResetBankForTexPltt(void) {
    func_02002c38(&data_020bdc08[5]);
}
// These two reset bank fields 6 and 3, which no function in this file assigns; the resets of the sub-screen BG
// and OBJ fields (9 and 10) are func_02002dec and func_02002e00 below, so the two names may belong to those.
void GX_ResetBankForSubBG(void) {
    func_02002c38(&data_020bdc08[6]);
}
void GX_ResetBankForSubOBJ(void) {
    func_02002c38(&data_020bdc08[3]);
}
void func_02002dd8(void) {
    func_02002c38(&data_020bdc08[0]);
}
void func_02002dec(void) {
    func_02002c38(&data_020bdc08[9]);
}



void func_02002e00(void) {
    func_02002c38(&data_020bdc08[10]);
}

void GX_ResetBankForSubBGExtPltt(void) {
    reg_GXS_DISPCNT &= ~0x40000000;
    func_02002c38(&data_020bdc08[11]);
}

void GX_ResetBankForSubOBJExtPltt(void) {
    reg_GXS_DISPCNT &= ~0x80000000;
    func_02002c38(&data_020bdc08[12]);
}
