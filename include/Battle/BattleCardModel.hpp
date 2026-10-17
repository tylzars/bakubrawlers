#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Scene/SceneBase.hpp"

class EffectManager;
class Camera;
class BattleRealBaku;
class ModelLightGroup;

// One effect or bone attachment of a card model (0x28 bytes, tables at 0227f050 and following).
struct BattleCardModelPart {
    /* 00 */ s32 mFrame; // animation frame that starts the effect (0 = frame 0x1000)
    /* 04 */ VecFx32 mPos;
    /* 10 */ u16 mRot[3];
    /* 16 */ u8 mScript; // effect script
    union {
        /* 17 */ u8 mUnk_17;
        struct {
            /* 17 */ u8 mUnk_17_0 : 1;
            /* 17 */ u8 mKind : 7; // 0 = fixed position, 1 = follows a bone, 2 = rotated, 3 = bone and aimed
        };
    };
    /* 18 */ const char *mName; // bone name
    /* 1c */ VecFx32 mTarget;
    /* 28 */
};

// Card entry of the tables at 0228acc0 (0x10 bytes).
struct BattleCardModelEntry {
    /* 00 */ u8 mUnk_00;
    /* 01 */ u8 mCount; // number of parts
    /* 02 */ PAD(0x02, 0x04);
    /* 04 */ BattleCardModelPart *mParts;
    /* 08 */ u16 mUnk_08;
    /* 0a */ u16 mUnk_0a;
    /* 0c */ u32 mUnk_0c;
    /* 10 */
};

// Window showing the real bakugan model of a card (realb%02d + effects): vtable 0228acb4, 0x98 bytes.
class BattleCardModel : public SceneBase {
public:
    /* 0c */ EffectManager *mEffects;
    /* 10 */ Camera *mCamera;
    /* 14 */ BattleRealBaku *mBaku;
    /* 18 */ void *mUnk_18; // camera animation file
    /* 1c */ BattleCardModelEntry *mEntry;
    /* 20 */ BattleCardModelPart *mParts;
    /* 24 */ u32 mUnk_24;
    /* 28 */ ModelLightGroup *mUnk_28;
    /* 2c */ VecFx32 mPos[5];
    /* 68 */ void *mTextBuffer;
    /* 6c */ void *mTextBuffer2;
    /* 70 */ u32 mTextSize;
    /* 74 */ u32 mUnk_74;
    /* 78 */ s32 mLastFrame;
    /* 7c */ u16 mHandles[5];
    /* 86 */ u16 mTextWidth;
    /* 88 */ u8 mId;
    /* 89 */ u8 mState;
    /* 8a */ u8 mType;
    /* 8b */ u8 mCount;
    /* 8c */ u8 mUnk_8c;
    /* 8d */ u8 mUnk_8d;
    /* 8e */ u8 mBones[5];
    /* 93 */ u8 mUnk_93;
    /* 94 */ u8 mUnk_94;
    /* 95 */ u8 mPasses;
    /* 96 */ u8 mUnk_96;
    /* 97 */ PAD(0x97, 0x98);
    /* 98 */

    BattleCardModel(EffectManager *effects, Camera *camera, u32 a, u32 b, u32 c, u32 d);
    virtual ~BattleCardModel();
    virtual void Update();
    void func_ov007_022725f0(u32 type, u32 id, u32 c);
    BOOL func_ov007_0227292c();
    void func_ov007_022734d8();
    void func_ov007_02273718();
    void func_ov007_022737e0();
    void func_ov007_02273a4c(u32 index);
    void func_ov007_02273be8();
    void func_ov007_02273ec0();
    void func_ov007_02274238();

    // Part `index` of the card's table.
    BattleCardModelPart *Part(u8 index) {
        BattleCardModelPart *part = mParts;
        part += index;
        return part;
    }
};

void func_ov007_02273e3c(BattleCardModel *model); // releases the model
