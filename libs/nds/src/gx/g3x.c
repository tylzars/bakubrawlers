#include <nitro/internal.h>

void G3X_Init(void) {
    G3X_ClearFifo();
    reg_G3X_END_VTXS = 0;
    while (reg_G3X_GXSTAT & 0x08000000) {
    }
    reg_GX_DISP3DCNT = 0;
    reg_G3X_GXSTAT = 0;
    reg_G2_BG0OFS = 0;
    reg_GX_DISP3DCNT |= 0x2000;
    reg_GX_DISP3DCNT |= 0x1000;
    reg_GX_DISP3DCNT &= ~0x3002;
    reg_GX_DISP3DCNT = (u16) ((reg_GX_DISP3DCNT & ~0x3000) | 0x10);
    reg_GX_DISP3DCNT &= 0xcffb;
    reg_G3X_GXSTAT |= 0x8000;
    reg_G3X_GXSTAT = (reg_G3X_GXSTAT & ~0xc0000000) | 0x80000000;
    G3X_InitMtxStack();
    reg_G3X_CLEAR_COLOR = 0;
    *(vu16 *) 0x04000354 = 0x7fff;
    *(vu16 *) 0x04000356 = 0;
    *(vu32 *) 0x04000358 = 0;
    *(vu16 *) 0x0400035c = 0;
    reg_G2_BG0CNT &= ~3;
    G3X_InitTable();
    *(vu32 *) 0x040004a4 = 0x1f0080;
    *(vu32 *) 0x040004a8 = 0;
    *(vu32 *) 0x040004ac = 0;
}

void GXi_NopClearFifo128_(volatile void *dst);

void G3X_ClearFifo(void) {
    GXi_NopClearFifo128_(&reg_G3X_GXFIFO);
    while (reg_G3X_GXSTAT & 0x08000000) {
    }
}

void G3X_InitMtxStack(void) {
    u32 pv;
    u32 pj;
    reg_G3X_GXSTAT |= 0x8000;
    while (G3X_GetMtxStackLevelPV(&pv) != 0) {
    }
    while (G3X_GetMtxStackLevelPJ(&pj) != 0) {
    }
    reg_G3X_MTX_MODE = 3;
    reg_G3X_MTX_IDENTITY = 0;
    reg_G3X_MTX_MODE = 0;
    if (pj != 0) {
        reg_G3X_MTX_POP = pj;
    }
    reg_G3X_MTX_IDENTITY = 0;
    reg_G3X_MTX_MODE = 2;
    reg_G3X_MTX_POP = pv;
    reg_G3X_MTX_IDENTITY = 0;
}

int G3X_GetClipMtx(MtxFx44 *dst) {
    if (reg_G3X_GXSTAT & 0x08000000) {
        return -1;
    }
    MI_Copy64B((const void *) 0x04000640, dst);
    return 0;
}

int G3X_GetVectorMtx(MtxFx33 *dst) {
    if (reg_G3X_GXSTAT & 0x08000000) {
        return -1;
    }
    MI_Copy36B((const void *) 0x04000680, dst);
    return 0;
}

void G3X_SetEdgeColorTable(const u16 *rgb) {
    MIi_CpuCopy16(rgb, (void *) 0x04000330, 0x10);
}

void G3X_SetToonTable(const u16 *rgb) {
    MIi_CpuCopy16(rgb, (void *) 0x04000380, 0x40);
}

void G3X_SetClearColor(int rgb, int alpha, int depth, int polygonID, BOOL fog) {
    u32 v = rgb | (alpha << 16) | (polygonID << 24);
    if (fog) {
        v |= 0x8000;
    }
    reg_G3X_CLEAR_COLOR = v;
    *(vu16 *) 0x04000354 = (u16) depth;
}

void G3X_InitTable(void) {
    int i;
    if (data_020a4f24 != -1) {
        MI_DmaFill32Async(data_020a4f24, (void *) 0x04000330, 0, 0x10, 0, 0);
        MI_DmaFill32(data_020a4f24, (void *) 0x04000360, 0, 0x60);
    } else {
        MIi_CpuClear32(0, (void *) 0x04000330, 0x10);
        MIi_CpuClear32(0, (void *) 0x04000360, 0x60);
    }
    for (i = 0; i < 32; i++) {
        *(vu32 *) 0x040004d0 = 0;
    }
}

int G3X_GetMtxStackLevelPV(u32 *level) {
    if (reg_G3X_GXSTAT & 0x4000) {
        return -1;
    }
    *level = (reg_G3X_GXSTAT & 0x1f00) >> 8;
    return 0;
}

int G3X_GetMtxStackLevelPJ(u32 *level) {
    if (reg_G3X_GXSTAT & 0x4000) {
        return -1;
    }
    *level = (reg_G3X_GXSTAT & 0x2000) >> 13;
    return 0;
}

int G3X_GetBoxTestResult(u32 *result) {
    if (reg_G3X_GXSTAT & 1) {
        return -1;
    }
    *result = reg_G3X_GXSTAT & 2;
    return 0;
}

void WriteBG0Offset(u32 value) {
    reg_G2_BG0OFS = value;
}

// GXi_NopClearFifo128_: r0 points at a write-only FIFO port. Writes 32 groups of
// four zero words (128 words in all) to the same address with 4-register
// block stores, then returns.
// Not compiled: zeroes r1, r2, r3 and ip and stores them with 32 `stmia r0, {r1, r2, r3, ip}`. In every form tried
// the zeros share one register: stores of 0 to four consecutive words compile to one `mov r1, #0` and four `str`
// (with `register` locals too), and different constants also reuse one register, so no stm can form (merged stores
// need distinct registers: `p[0] = a; p[1] = b; p[2] = c;` gives `stm r0, {r1, r2, r3}`). A struct copy from a zeroed
// local moves the data through r0-r3 from a base moved off r0. At -O0 the `register` locals live in r4-r7 and no
// stores are combined.
asm void GXi_NopClearFifo128_(volatile void *fifo) {
    mov r1, #0
    mov r2, #0
    mov r3, #0
    mov ip, #0
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    stmia r0, {r1, r2, r3, ip}
    bx lr
}
