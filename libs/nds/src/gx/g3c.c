#include <nitro/internal.h>

void G3_LoadMtx43(const MtxFx43 *mtx) {
    reg_G3X_GXFIFO = 0x17;
    func_020047d8(mtx, &reg_G3X_GXFIFO);
}

void G3_MultMtx43(const MtxFx43 *mtx) {
    reg_G3X_GXFIFO = 0x19;
    func_020047d8(mtx, &reg_G3X_GXFIFO);
}

void G3_MultMtx33(const MtxFx33 *mtx) {
    reg_G3X_GXFIFO = 0x1a;
    MI_Copy36B(mtx, &reg_G3X_GXFIFO);
}

void G3_Shininess(const u32 *table) {
    reg_G3X_GXFIFO = 0x34;
    func_020047fc(table, &reg_G3X_GXFIFO);
}
