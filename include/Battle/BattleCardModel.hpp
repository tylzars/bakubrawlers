#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Scene/SceneBase.hpp"

class EffectManager;
class Camera;
class BattleRealBaku;
class ModelLightGroup;

// How BattleCardModel::UpdateEffects places a part's effect (BattleCardModelPart::mKind).
#define CARD_MODEL_PART_KIND_POS         0 // at mPos
#define CARD_MODEL_PART_KIND_BONE        1 // mPos taken through the matrix of the bone mName (mBones)
#define CARD_MODEL_PART_KIND_ROT         2 // matrix from mRot and mPos
#define CARD_MODEL_PART_KIND_BONE_TARGET 3 // as BONE, then rotated toward mTarget

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
            /* 17 */ u8 mKind : 7; // CARD_MODEL_PART_KIND_*
        };
    };
    /* 18 */ const char *mName; // bone name
    /* 1c */ VecFx32 mTarget;
    /* 28 */
};

// Card entry of the tables at 0228acc0 (0x10 bytes).
struct BattleCardModelEntry {
    /* 00 */ u8 mRestartFrame;
    /* 01 */ u8 mCount; // number of parts
    /* 02 */ PAD(0x02, 0x04);
    /* 04 */ BattleCardModelPart *mParts;
    /* 08 */ u16 mCamKeyStartFrame;
    /* 0a */ u16 mCamKeyEndFrame;
    /* 0c */ u32 mCamKeys;
    /* 10 */
};

// Window showing the real bakugan model of a card (realb%02d + effects): vtable 0228acb4, 0x98 bytes.
class BattleCardModel : public SceneBase {
public:
    /* 0c */ EffectManager *mEffects;
    /* 10 */ Camera *mCamera;
    /* 14 */ BattleRealBaku *mBaku;
    /* 18 */ void *mCamFile; // camera animation file
    /* 1c */ BattleCardModelEntry *mEntry;
    /* 20 */ BattleCardModelPart *mParts;
    /* 24 */ u32 mCurrentCamKey; // key in use: advanced after each use, back to mEntry->mCamKeys at {-1, -1}; 0 = stopped
    /* 28 */ ModelLightGroup *mLights;
    /* 2c */ VecFx32 mPos[5];
    /* 68 */ void *mTextBuffer;
    /* 6c */ void *mTextBuffer2;
    /* 70 */ u32 mTextBufferSize;
    /* 74 */ u32 mGlyphCount;
    /* 78 */ s32 mLastFrame;
    /* 7c */ u16 mHandles[5];
    /* 86 */ u16 mTextWidth;
    /* 88 */ u8 mId;
    /* 89 */ u8 mState;
    /* 8a */ u8 mType;
    /* 8b */ u8 mCount;
    /* 8c */ u8 mBakuganId;
    /* 8d */ u8 mVariant;
    /* 8e */ u8 mBones[5];
    /* 93 */ u8 mAnimRestarted;
    /* 94 */ u8 mUseRealb20;
    /* 95 */ u8 mPasses;
    /* 96 */ u8 mUnk_96;
    /* 97 */ PAD(0x97, 0x98);
    /* 98 */

    BattleCardModel(EffectManager *effects, Camera *camera, u32 a, u32 b, u32 c, u32 d);
    virtual ~BattleCardModel();
    virtual void Update();
    void Init(u32 type, u32 id, u32 c);
    BOOL UpdateState();
    void LoadGraphics(); // effect groups realb%02d and union, the .cam path, mBaku, mLastFrame, mBones, mLights
    void StartEffectsAtFrame(); // nothing once mAnimRestarted is set
    void UpdateEffects();
    void MoveEffectToBone(u32 index);
    void func_ov007_02273be8();
    void LoadNameText();
    void DrawName();

    // Part `index` of the card's table.
    BattleCardModelPart *Part(u8 index) {
        BattleCardModelPart *part = mParts;
        part += index;
        return part;
    }
};

void CloseCardModel(BattleCardModel *model); // nothing once mState >= 3; else state 3 and sFade[0] = 3; frees nothing
