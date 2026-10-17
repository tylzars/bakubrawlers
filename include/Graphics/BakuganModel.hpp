#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Graphics/ModelTransform.hpp"

class ModelInstance;

// One of the two sub-records at +38 of BakuganModel (8 bytes).
struct UnkStruct_0203d374_38 {
    /* 00 */ u32 mUnk_00;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u8 mUnk_06;
    /* 07 */ PAD(0x07, 0x08);
    /* 08 */
};

// A posed Bakugan sphere model (0x54 bytes, ctor 0203d374, destructor 0203d3e8): a ModelTransform plus the loading
// state of the sphere model (kind, value, quality mode). Set up with Setup, moved with
// func_0203db70/func_0203db94, updated with Update and drawn with DrawModel.
class BakuganModel : public ModelTransform {
public:
    /* 00 ModelTransform (position at 04, rotation at 10, scale at 18, model at 28) */
    /* 2c */ u32 mSpinTurnCount; // full mRotX turns done in UpdateSpin's step 0 (0..3)
    /* 30 */ PAD(0x30, 0x32);
    /* 32 */ u8 mUnk_32;
    /* 33 */ u8 mUnk_33;
    /* 34 */ u8 mUnk_34;
    /* 35 */ PAD(0x35, 0x38);
    /* 38 */ UnkStruct_0203d374_38 mUnk_38[2];
    /* 48 */ u8 mUnk_48; // set by the mode that keeps the model alive
    /* 49 */ u8 mAnimUpdateEnabled; // gates Update's animation advance; separate from ModelTransform::mAnimEnabled
    /* 4a */ u8 mVisible; // visible
    /* 4b */ u8 mUnk_4b;
    /* 4c */ u8 mMode; // mode: 1/2 low quality, 3/4 animated, 5/6 static, 6 spins in
    /* 4d */ u8 mVariant; // value (palette variant)
    /* 4e */ u8 mSpinStep; // step of the mode 6 effect
    /* 4f */ u8 mKind; // kind (index into the sphere table at 020a1074)
    /* 50 */ u8 mStoredKind; // kind/value/mode saved for RunStoredSetup
    /* 51 */ u8 mStoredVariant;
    /* 52 */ u8 mStoredMode;
    /* 53 */ PAD(0x53, 0x54);
    /* 54 */

    BakuganModel();
    ~BakuganModel();
    void SetupMode(u32 unk1); // Setup with the current kind and variant
    void Setup(u32 unk1, u32 unk2, u32 unk3);
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
    void StoreSetup(u32 unk1, u32 unk2, u32 unk3);
    BOOL func_0203d6f0();
    void RunStoredSetup();
    void Update();
    void UpdateSpin();
    void DrawModel();
    BOOL LoadModel(u8 kind); // FALSE without loading while mUnk_48 is set; deletes the previous model first
};

extern "C" void FreeBakuganModelInstance(BakuganModel *pose); // frees the model
void SetTransformPosition(void *obj, fx32 x, fx32 y, fx32 z); // sets the position
