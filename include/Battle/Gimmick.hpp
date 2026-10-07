#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/ModelTransform.hpp"
#include "Scene/SceneBase.hpp"

struct GimmickSetData;
// 0x800 byte object table of a set file.
struct GimmickSetBlock {
    u32 mData[0x200];
};

// Loaded data of the SetData_%02d_%02d.bin files (0x824+ bytes), created by func_ov007_02271ca8.
struct GimmickSetData {
    /* 000 */ GimmickSetData *mPrev; // list node: previous
    /* 004 */ GimmickSetData *mNext;
    /* 008 */ u16 mUnk_08; // collision groups this set belongs to
    /* 00a */ u16 mUnk_0a; // collision groups it reacts to
    /* 00c */ u16 mUnk_0c; // first 4 bytes of the file (count at +e)
    /* 00e */ u16 mUnk_0e;
    /* 010 */ GimmickSetBlock mUnk_10; // object records
    /* 810 */ VecFx32 mUnk_810;
    /* 81c */ u16 mRotX;
    /* 81e */ u16 mRotY;
    /* 820 */ u16 mRotZ;
    /* 822 */ u8 mUnk_822; // own slot index
    /* 823 */ u8 mUnk_823;
    /* 824 */ u8 mUnk_824[8];
    /* 82c */ u8 mUnk_82c[8];
    /* 834 */ u8 mUnk_834; // slot in the owner's table + 1 (0: unused)
    /* 835 */ PAD(0x835, 0x838);
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
