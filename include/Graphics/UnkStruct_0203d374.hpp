#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

class ModelInstance;

// A posed 3D object (0x54 bytes, ctor 0203d374, destructor 0203d3e8): set up with func_0203d4dc, moved with
// func_0203db70/func_0203db94, updated with func_0203d988 and drawn with func_0203dab0.
class UnkStruct_0203d374 {
public:
    /* 00 */ PAD(0x00, 0x04);
    /* 04 */ VecFx32 mPosition;
    /* 10 */ u16 mUnk_10;
    /* 12 */ u16 mUnk_12;
    /* 14 */ u16 mUnk_14;
    /* 16 */ PAD(0x16, 0x28);
    /* 28 */ ModelInstance *mUnk_28;
    /* 2c */ PAD(0x2c, 0x49);
    /* 49 */ u8 mUnk_49;
    /* 4a */ u8 mUnk_4a;
    /* 4b */ PAD(0x4b, 0x54);
    /* 54 */

    UnkStruct_0203d374();
    ~UnkStruct_0203d374();
    void func_0203d4c4(u32 unk1);
    void func_0203d4dc(u32 unk1, u32 unk2, u32 unk3);
    void func_0203db70(const VecFx32 *pos);
    void func_0203db94(const u16 *rot);

    // Inline copies of the position / rotation setters (func_0203db70 / func_0203db94 are their out-of-line copies).
    void SetPosition(const VecFx32 *pos) {
        mPosition.x = pos->x;
        mPosition.y = pos->y;
        mPosition.z = pos->z;
    }
    void SetRotation(const u16 *rot) {
        mUnk_10 = rot[0];
        mUnk_12 = rot[1];
        mUnk_14 = rot[2];
    }
    void func_0203d6e0(u32 unk1, u32 unk2, u32 unk3);
    BOOL func_0203d6f0();
    void func_0203d6f8();
    void func_0203d988();
    void func_0203dab0();
};
