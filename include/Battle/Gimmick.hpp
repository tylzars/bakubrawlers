#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/ModelTransform.hpp"
#include "Scene/SceneBase.hpp"

// Loaded data of the SetData_%02d_%02d.bin files (0x824+ bytes), created by func_ov007_02271ca8.
struct GimmickSetData {
    /* 000 */ PAD(0x000, 0x823);
    /* 823 */ u8 mUnk_823;
};

// Base of the field gimmick objects of the battle overlay (vtable 0228729c, 0x46 bytes in the derived classes).
// The ModelTransform sits at +0c (constructed first), the SceneBase at +00.
class Gimmick : public ModelTransform, public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c ModelTransform */
    /* 38 */ u8 mState;
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ u8 mUnk_3b;
    /* 3c */ GimmickSetData *mUnk_3c;
    /* 40 */ s32 mUnk_40;
    /* 44 */ u8 mUnk_44;
    /* 45 */

    Gimmick(u8 a, u8 b);
    void func_ov007_0222ea28(int id, int count); // loads the SetData file `id` into mUnk_3c
    BOOL func_ov007_0222ea84(int radius);       // TRUE when mPosition is within radius of a marked player entry
    virtual ~Gimmick();
    /* 08 */ virtual void Update() = 0;
    /* 0c */ virtual void Load() = 0;
    /* 10 */ virtual void Unload() = 0;
    /* 14 */ virtual void Finish() = 0;
    /* 18 */ virtual void vfunc_18() {
        if (mUnk_40) {
            mUnk_44++;
        }
    }
};
