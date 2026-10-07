#pragma once

#include "global.h"
#include "types.h"

#include "Graphics/Model.hpp"
#include "Scene/SceneBase.hpp"

#include <nitro/fx.h>

class EffectManager;

// One model of an effect that is playing its animation (8 bytes, EffectScript::mAnims).
struct EffectAnim {
    /* 00 */ u8 mLoop; // restart the animation when it ends, otherwise drop the model
    /* 01 */ u8 mAnim;
    /* 02 */ u16 mFrame;
    /* 04 */ s32 mSpeed;
    /* 08 */
};

// A running effect: a small script of byte codes that creates models, moves them and animates them (0x61c bytes,
// ctor 0202ab24, destroyed with 0202ac6c). func_0202acb4 runs the script until it waits or ends and then updates
// the models; it returns FALSE when the effect is over.
class Effect {
public:
    /* 000 */ u8 *mCursor; // script position
    /* 004 */ MtxFx43 mMtx;
    /* 034 */ VecFx32 mPos;
    /* 040 */ VecFx32 mScale;
    /* 04c */ VecFx32 mSavedPos; // value kept by the script for the next models (the "keep" flag of the op)
    /* 058 */ VecFx32 mSavedScale;
    /* 064 */ VecFx32 mModelPos; // applied to the next model that is created
    /* 070 */ VecFx32 mModelScale;
    /* 07c */ MtxFx43 mWorld[10];
    /* 25c */ VecFx32 mWorldScale[10];
    /* 2d4 */ MtxFx43 mLocal[10];
    /* 4b4 */ VecFx32 mLocalScale[10];
    /* 52c */ ModelInstance *mModels[10];
    /* 554 */ EffectAnim mAnims[10];
    /* 5a4 */ s32 mAnimSpeed;
    /* 5a8 */ s32 mSavedAnimSpeed;
    /* 5ac */ u8 *mLoopStart[10];
    /* 5d4 */ u16 mLoopCount[10];
    /* 5e8 */ u16 mLoopDepth;
    /* 5ea */ u16 mRot[3];
    /* 5f0 */ u16 mSavedRot[3];
    /* 5f6 */ u16 mModelRot[3];
    /* 5fc */ u16 mWait;
    /* 5fe */ u16 mFrame;
    /* 600 */ u8 mMtxSet;   // 1 when mMtx was given as a matrix
    /* 601 */ u8 mScaleSet; // likewise for the scale
    /* 602 */ u8 mWorldDone[10];
    /* 60c */ u8 mScaleDone[10];
    /* 616 */ u8 mGroup;
    /* 617 */ u8 mRunning;
    /* 618 */ u8 mWaiting;
    /* 619 */ u8 mAnimEnded;
    /* 61a */ u8 mLoop;
    /* 61b */ u8 mSavedLoop;
    /* 61c */

    Effect(EffectManager *manager, u32 group, u8 *script);
    ~Effect();
    BOOL func_0202acb4();
    void func_0202b500(VecFx32 *pos);
    void func_0202b524(VecFx32 *scale);
    void func_0202b548(u16 *rot);
    void func_0202b570(MtxFx43 *mtx);
    void func_0202b5c4(VecFx32 *out);
    u16 func_0202b5f4();
    s32 func_0202b63c();
    s32 func_0202b668();
    u16 func_0202b8c4();
    u32 func_0202b8f0();
};

// Layout of a loaded .esb file; the offsets are turned into pointers when it is loaded.
struct EffectFile {
    /* 00 */ u32 mUnk_00;
    /* 04 */ u32 mNumModels;
    /* 08 */ char *mNames; // model names, one after the other
    /* 0c */ u32 mNumScripts;
    /* 10 */ u8 *mScripts[1];
};

// The effect list of a battle scene: three loaded sets (.esb with its models, textures and animations), 64 effect
// slots. Handles are the slot number + 1.
class EffectManager : public SceneBase {
public:
    /* 0c */ EffectFile *mFiles[3];
    /* 18 */ NitroTexture *mTextures[3];
    /* 24 */ ModelAnimData *mAnimData[3];
    /* 30 */ NitroModel **mModels[3];
    /* 3c */ Effect **mEffects;
    /* 40 */ u16 mNext;
    /* 42 */ u8 mTags[64];
    /* 82 */ u8 mPasses;
    /* 83 */ PAD(0x83, 0x84);
    /* 84 */

    EffectManager();                       // 0202b938
    virtual ~EffectManager();              // 0202b9cc, deleting 0202ba14
    virtual void Update();                 // 0202bf84
    ModelInstance *func_0202be14(u32 group, u32 index);
    void func_0202be68();
    void func_0202bf94(u32 tag);
    void func_0202c024(u32 passes);
};

void func_0202ba64(EffectManager *manager, u32 slot, const char *name);
void func_0202bc58(EffectManager *manager, u32 slot);
u16 func_0202bd40(EffectManager *manager, u32 group, u32 script, u32 position, u32 tag);
void func_0202beb8(EffectManager *manager, u16 handle);
void func_0202bef4(EffectManager *manager, u16 handle, VecFx32 *position);
void func_0202bf18(EffectManager *manager, u16 handle, VecFx32 *scale);
void func_0202bf3c(EffectManager *manager, u16 handle, u16 *rotation);
void func_0202bf60(EffectManager *manager, u16 handle, MtxFx43 *mtx);
