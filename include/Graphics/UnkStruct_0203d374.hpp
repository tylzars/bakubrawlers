#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Graphics/ModelTransform.hpp"

class ModelInstance;

// One of the two sub-records at +38 of UnkStruct_0203d374 (8 bytes).
struct UnkStruct_0203d374_38 {
    /* 00 */ u32 mUnk_00;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u8 mUnk_06;
    /* 07 */ PAD(0x07, 0x08);
    /* 08 */
};

// A posed Bakugan sphere model (0x54 bytes, ctor 0203d374, destructor 0203d3e8): a ModelTransform plus the loading
// state of the sphere model (kind, value, quality mode). Set up with func_0203d4dc, moved with
// func_0203db70/func_0203db94, updated with func_0203d988 and drawn with func_0203dab0.
class UnkStruct_0203d374 : public ModelTransform {
public:
    /* 00 ModelTransform (position at 04, rotation at 10, scale at 18, model at 28) */
    /* 2c */ u32 mUnk_2c; // step counter of the spin-in effect (mode 6)
    /* 30 */ PAD(0x30, 0x32);
    /* 32 */ u8 mUnk_32;
    /* 33 */ u8 mUnk_33;
    /* 34 */ u8 mUnk_34;
    /* 35 */ PAD(0x35, 0x38);
    /* 38 */ UnkStruct_0203d374_38 mUnk_38[2];
    /* 48 */ u8 mUnk_48; // set by the mode that keeps the model alive
    /* 49 */ u8 mUnk_49; // animation running
    /* 4a */ u8 mUnk_4a; // visible
    /* 4b */ u8 mUnk_4b;
    /* 4c */ u8 mUnk_4c; // mode: 1/2 low quality, 3/4 animated, 5/6 static, 6 spins in
    /* 4d */ u8 mUnk_4d; // value (palette variant)
    /* 4e */ u8 mUnk_4e; // step of the mode 6 effect
    /* 4f */ u8 mUnk_4f; // kind (index into the sphere table at 020a1074)
    /* 50 */ u8 mUnk_50; // kind/value/mode saved for func_0203d6f8
    /* 51 */ u8 mUnk_51;
    /* 52 */ u8 mUnk_52;
    /* 53 */ PAD(0x53, 0x54);
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
        mRotX = rot[0];
        mRotY = rot[1];
        mRotZ = rot[2];
    }
    void func_0203d6e0(u32 unk1, u32 unk2, u32 unk3);
    BOOL func_0203d6f0();
    void func_0203d6f8();
    void func_0203d988();
    void func_0203d9e0();
    void func_0203dab0();
    BOOL func_0203d710(u8 kind);
};

extern "C" void func_0203d45c(UnkStruct_0203d374 *pose); // frees the model
void func_0203db8c(void *obj, fx32 x, fx32 y, fx32 z); // sets the position
