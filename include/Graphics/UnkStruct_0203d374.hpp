#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

// A posed 3D object (0x54 bytes, ctor 0203d374, destructor 0203d3e8): set up with func_0203d4dc, moved with
// func_0203db70/func_0203db94, updated with func_0203d988 and drawn with func_0203dab0.
class UnkStruct_0203d374 {
public:
    /* 00 */ PAD(0x00, 0x04);
    /* 04 */ VecFx32 mPosition;
    /* 10 */ u16 mUnk_10;
    /* 12 */ u16 mUnk_12;
    /* 14 */ u16 mUnk_14;
    /* 16 */ PAD(0x16, 0x49);
    /* 49 */ u8 mUnk_49;
    /* 4a */ PAD(0x4a, 0x54);
    /* 54 */

    UnkStruct_0203d374();
    ~UnkStruct_0203d374();
    void func_0203d4dc(u32 unk1, u32 unk2, u32 unk3);
    void func_0203db70(const VecFx32 *pos);
    void func_0203db94(const u16 *rot);
    void func_0203d988();
    void func_0203dab0();
};
