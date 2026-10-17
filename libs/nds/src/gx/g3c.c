#include <nitro/internal.h>

void G3_LoadMtx43(const MtxFx43 *mtx) {
    reg_G3X_GXFIFO = 0x17;
    MI_Send48B(mtx, &reg_G3X_GXFIFO);
}

void G3_MultMtx43(const MtxFx43 *mtx) {
    reg_G3X_GXFIFO = 0x19;
    MI_Send48B(mtx, &reg_G3X_GXFIFO);
}

void G3_MultMtx33(const MtxFx33 *mtx) {
    reg_G3X_GXFIFO = 0x1a;
    MI_Copy36B(mtx, &reg_G3X_GXFIFO);
}

void G3_Shininess(const u32 *table) {
    reg_G3X_GXFIFO = 0x34;
    MI_Send128B(table, &reg_G3X_GXFIFO);
}
