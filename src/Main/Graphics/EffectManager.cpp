#include "Graphics/Effect.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/ModelInternal.hpp"
#include "System/FileSystem.hpp"
#include "Game/Random.hpp"
#include <nitro/math.h>

#include <nitro/fx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *ptr);
void operator delete[](void *ptr);

static EffectManager *sManager;

Effect::Effect(EffectManager *manager, u32 group, u8 *script) {
    mCursor       = script;
    mMtx          = data_0220e86c;
    mPos.x        = 0;
    mPos.y        = 0;
    mPos.z        = 0;
    mScale.x      = 0x1000;
    mScale.y      = 0x1000;
    mScale.z      = 0x1000;
    mRot[0]       = 0;
    mRot[1]       = 0;
    mRot[2]       = 0;
    mMtxSet       = 1;
    mScaleSet     = 1;
    mSavedPos.x   = 0;
    mSavedPos.y   = 0;
    mSavedPos.z   = 0;
    mSavedScale.x = 0x1000;
    mSavedScale.y = 0x1000;
    mSavedScale.z = 0x1000;
    mSavedRot[0]  = 0;
    mSavedRot[1]  = 0;
    mSavedRot[2]  = 0;
    mModelPos     = mSavedPos;
    mModelScale   = mSavedScale;
    mModelRot[0]  = mSavedRot[0];
    mModelRot[1]  = mSavedRot[1];
    mModelRot[2]  = mSavedRot[2];
    MI_CpuClear32(mModels, sizeof(mModels));
    mGroup     = group;
    mWaiting   = 0;
    mLoopDepth = 0;
    MI_CpuClearFast(mAnims, sizeof(mAnims));
    mSavedLoop = 0;
    mLoop      = 0;
    mSavedAnimSpeed = 0x1000;
    mAnimSpeed      = 0x1000;
    mFrame          = 0;
    sManager        = manager;
}

Effect::~Effect() {
    for (u8 i = 0; i < 10; i++) {
        ModelInstance *model = mModels[i];
        if (model != NULL) {
            model->mUnk_68 = 1;
            delete model;
        }
    }
}

BOOL Effect::func_0202acb4() {
    BOOL result = TRUE;
    u8 keep;
    if (mWaiting != 0) {
        if (mWait != 0) {
            mWait--;
            if (mWait == 0) {
                mWaiting = 0;
                MI_CpuClearFast(mAnims, sizeof(mAnims));
            }
        } else if (mAnimEnded != 0) {
            MI_CpuClearFast(mAnims, sizeof(mAnims));
            mWaiting = 0;
            MI_CpuClearFast(mAnims, sizeof(mAnims));
        }
    }
    if (mWaiting == 0) {
        BOOL run = TRUE;
        do {
            u8 op = *mCursor++;
            switch (op) {
            case 1:
                run    = FALSE;
                result = FALSE;
                break;
            case 2: {
                u8 slot;
                for (slot = 0; slot < 10; slot++) {
                    if (mModels[slot] == NULL) {
                        break;
                    }
                }
                u8 id   = *mCursor++;
                keep = *mCursor++;
                mModels[slot] = sManager->func_0202be14(mGroup, id);
                mModels[slot]->func_02028e54(keep, 0, mAnimSpeed);
                u16 frame = mFrame;
                if (frame != 0) {
                    mModels[slot]->func_02028e70(FX32_CONST((float) frame));
                }
                mLocal[slot] = data_0220e86c;
                if (mModelRot[1] != 0) {
                    func_01ffd0e0(&mLocal[slot], mModelRot[1]);
                }
                if (mModelRot[0] != 0) {
                    func_01ffd088(&mLocal[slot], mModelRot[0]);
                }
                if (mModelRot[2] != 0) {
                    func_01ffd138(&mLocal[slot], mModelRot[2]);
                }
                func_01ffcf64(&mLocal[slot], &mModelPos);
                mLocalScale[slot]  = mModelScale;
                mWorldDone[slot]   = 0;
                mScaleDone[slot]   = 0;
                mAnims[slot].mLoop = mLoop;
                mAnims[slot].mAnim   = keep;
                mAnims[slot].mFrame  = mFrame;
                mAnims[slot].mSpeed  = mAnimSpeed;
                mModelScale          = mSavedScale;
                mModelPos            = mSavedPos;
                mModelRot[0]         = mSavedRot[0];
                mModelRot[1]         = mSavedRot[1];
                mModelRot[2]         = mSavedRot[2];
                mLoop           = mSavedLoop;
                mAnimSpeed           = mSavedAnimSpeed;
                mFrame               = 0;
                break;
            }
            case 3: {
                keep = *mCursor++;
                func_0202b5c4(&mModelScale);
                if (keep == 1) {
                    mSavedScale = mModelScale;
                }
                break;
            }
            case 4: {
                keep = *mCursor++;
                func_0202b5c4(&mModelPos);
                if (keep == 1) {
                    mSavedPos = mModelPos;
                }
                break;
            }
            case 5: {
                keep = *mCursor++;
                mModelRot[0] = (u16) ((float) func_0202b5f4() * 65536.0f / 360.0f);
                mModelRot[1] = (u16) ((float) func_0202b5f4() * 65536.0f / 360.0f);
                mModelRot[2] = (u16) ((float) func_0202b5f4() * 65536.0f / 360.0f);
                if (keep == 1) {
                    mSavedRot[0] = mModelRot[0];
                    mSavedRot[1] = mModelRot[1];
                    mSavedRot[2] = mModelRot[2];
                }
                break;
            }
            case 7:
                mLoopCount[mLoopDepth] = func_0202b5f4();
                mLoopStart[mLoopDepth] = mCursor;
                mLoopDepth++;
                break;
            case 8:
                if (mLoopCount[mLoopDepth - 1] == 0) {
                    mCursor = mLoopStart[mLoopDepth - 1];
                } else {
                    mLoopCount[mLoopDepth - 1]--;
                    if (mLoopCount[mLoopDepth - 1] == 0) {
                        mLoopDepth--;
                    } else {
                        mCursor = mLoopStart[mLoopDepth - 1];
                    }
                }
                break;
            case 10: {
                keep = *mCursor++;
                mLoop = func_0202b5f4();
                if (keep == 1) {
                    mSavedLoop = mLoop;
                }
                break;
            }
            case 11: {
                keep = *mCursor++;
                mAnimSpeed = func_0202b63c();
                if (keep == 1) {
                    mSavedAnimSpeed = mAnimSpeed;
                }
                break;
            }
            case 12:
                mFrame = func_0202b5f4();
                break;
            case 9:
                mWait    = func_0202b5f4();
                mWaiting = 1;
                run      = FALSE;
                break;
            }
        } while (run);
    }
    if (mMtxSet == 0) {
        mMtx = data_0220e86c;
        if (mRot[1] != 0) {
            func_01ffd0e0(&mMtx, mRot[1]);
        }
        if (mRot[0] != 0) {
            func_01ffd088(&mMtx, mRot[0]);
        }
        if (mRot[2] != 0) {
            func_01ffd138(&mMtx, mRot[2]);
        }
        func_01ffcf64(&mMtx, &mPos);
    }
    mRunning = 0;
    for (u8 i = 0; i < 10; i++) {
        if (mModels[i] == NULL) {
            continue;
        }
        if ((mMtxSet & mWorldDone[i]) == 0) {
            func_01ff89c0(&mLocal[i], &mMtx, &mWorld[i]);
            mWorldDone[i] = 1;
        }
        if ((mScaleSet & mScaleDone[i]) == 0) {
            mWorldScale[i].x = FX_Mul(mScale.x, mLocalScale[i].x);
            mWorldScale[i].y = FX_Mul(mScale.y, mLocalScale[i].y);
            mWorldScale[i].z = FX_Mul(mScale.z, mLocalScale[i].z);
            mScaleDone[i]    = 1;
        }
        mModels[i]->func_02029174(&mWorld[i], &mWorldScale[i]);
        mAnimEnded = mModels[i]->func_02028d80() == 0;
        if (mAnimEnded != 0) {
            if (mAnims[i].mLoop != 0) {
                mModels[i]->func_02028e54(mAnims[i].mAnim, 0, mAnims[i].mSpeed);
                u16 frame = mAnims[i].mFrame;
                if (frame != 0) {
                    mModels[i]->func_02028e70(FX32_CONST((float) frame));
                }
            } else {
                ModelInstance *model = mModels[i];
                model->mUnk_68       = 1;
                delete model;
                mModels[i] = NULL;
            }
        } else {
            mRunning++;
        }
    }
    mMtxSet   = 1;
    mScaleSet = 1;
    return result;
}

void Effect::func_0202b500(VecFx32 *pos) {
    mPos       = *pos;
    mMtxSet    = 0;
}

void Effect::func_0202b524(VecFx32 *scale) {
    mScale     = *scale;
    mScaleSet  = 0;
}

void Effect::func_0202b548(u16 *rot) {
    mRot[0] = rot[0];
    mRot[1] = rot[1];
    mRot[2] = rot[2];
    mMtxSet = 0;
}

void Effect::func_0202b570(MtxFx43 *mtx) {
    mMtx    = *mtx;
    mMtxSet = 1;
    u8 i    = 0;
    do {
        mWorldDone[i] = 0;
        i++;
    } while (i < 10);
}

void Effect::func_0202b5c4(VecFx32 *out) {
    out->x = func_0202b63c();
    out->y = func_0202b63c();
    out->z = func_0202b63c();
}

u16 Effect::func_0202b5f4() {
    if (*mCursor++ == 0) {
        return func_0202b8c4();
    }
    return (u16) ((float) func_0202b668() / 4096.0f);
}

s32 Effect::func_0202b63c() {
    if (*mCursor++ == 0) {
        return func_0202b8f0();
    }
    return func_0202b668();
}

s32 Effect::func_0202b668() {
    u32 rawMin = func_0202b8f0();
    u32 rawMax = func_0202b8f0();
    u32 rawStep = func_0202b8f0();
    float min  = *(float *) &rawMin;
    float max  = *(float *) &rawMax;
    float step = *(float *) &rawStep;
    float zero = 0.0f;
    if (step != zero) {
        u32 r     = MATH_Rand32(&data_020d42e8, (u32) ((max - min) / step) + 1);
        float rnd = r;
        return FX32_CONST(min + rnd * step);
    }
    float t = (float) MATH_Rand32(&data_020d42e8, 100001) / 100000.0f;
    return FX32_CONST(min + t * (max - min));
}

u16 Effect::func_0202b8c4() {
    u8 lo = *mCursor++;
    u8 hi = *mCursor++;
    return lo | (hi << 8);
}

u32 Effect::func_0202b8f0() {
    u8 b0 = *mCursor++;
    u8 b1 = *mCursor++;
    u8 b2 = *mCursor++;
    u8 b3 = *mCursor++;
    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

EffectManager::EffectManager() : SceneBase(1000, 0) {
    mNext    = 0;
    mEffects = new Effect *[64];
    MI_CpuClear32(mEffects, 0x100);
    MI_CpuClear32(mFiles, sizeof(mFiles));
    MI_CpuClear32(mTextures, sizeof(mTextures));
    MI_CpuClear32(mAnimData, sizeof(mAnimData));
    MI_CpuClear32(mModels, sizeof(mModels));
    mPasses = 1;
}

EffectManager::~EffectManager() {
    func_0202be68();
    delete[] mEffects;
    for (u8 i = 0; i < 3; i++) {
        func_0202bc58(this, i);
    }
}

// non-matching: 12 words, register naming in the model-loading loop.
void func_0202ba64(EffectManager *manager, u32 slot, const char *name) {
    char path[0x80];
    ChangeDir("/effect");
    sprintf(path, "%s.esb", name);
    EffectFile *file = (EffectFile *) func_02023504(path, 0, 0, 0, 0, 0);
    manager->mFiles[slot] = file;
    file->mNames += (u32) file;
    u8 i                  = 0;
    do {
        EffectFile *f = manager->mFiles[slot];
        f->mScripts[i] += (u32) f;
        i++;
    } while (i < manager->mFiles[slot]->mNumScripts);
    sprintf(path, "%s.nsbtx", name);
    NitroTexture *texture = new NitroTexture(path);
    manager->mTextures[slot] = texture;
    sprintf(path, "%s_data.bin", name);
    manager->mAnimData[slot] = new ModelAnimData(path);
    manager->mModels[slot]   = new NitroModel *[manager->mFiles[slot]->mNumModels];
    MI_CpuClear32(manager->mModels[slot], manager->mFiles[slot]->mNumModels * 4);
    char *modelName = manager->mFiles[slot]->mNames;
    u8 j            = 0;
    do {
        sprintf(path, "%s.nsbmd", modelName);
        manager->mModels[slot][j] = new NitroModel(path, manager->mTextures[slot]);
        while (*modelName++ != 0) {
        }
        j++;
    } while (j < manager->mFiles[slot]->mNumModels);
    manager->mTextures[slot]->func_02028344();
    ChangeDir("/");
}

void func_0202bc58(EffectManager *manager, u32 slot) {
    if (manager->mFiles[slot] == NULL) {
        return;
    }
    u8 i = 0;
    do {
        NitroModel *model = manager->mModels[slot][i];
        if (model != NULL) {
            model->func_02028af0();
            delete model;
        }
        manager->mModels[slot][i] = NULL;
        i++;
    } while (i < manager->mFiles[slot]->mNumModels);
    delete manager->mModels[slot];
    manager->mModels[slot] = NULL;
    ModelAnimData *anim    = manager->mAnimData[slot];
    if (anim != NULL) {
        anim->~ModelAnimData();
        operator delete(anim);
    }
    manager->mAnimData[slot] = NULL;
    NitroTexture *texture    = manager->mTextures[slot];
    if (texture != NULL) {
        texture->func_0202832c();
        delete texture;
    }
    manager->mTextures[slot] = NULL;
    OS_Free(manager->mFiles[slot]);
    manager->mFiles[slot] = NULL;
}

// non-matching: 5 words, registers of start and the effect list; the original also pushes r9.
u16 func_0202bd40(EffectManager *manager, u32 group, u32 script, u32 position, u32 tag) {
    u16 start        = manager->mNext;
    u16 i            = start;
    do {
        if (manager->mEffects[i] == NULL) {
            Effect *effect       = new Effect(manager, group, manager->mFiles[group]->mScripts[script]);
            manager->mEffects[i] = effect;
            manager->mTags[i]    = tag;
            s32 handle           = i + 1;
            manager->mNext       = handle % 64;
            if (position != 0) {
                func_0202bef4(manager, handle, (VecFx32 *) position);
            }
            return handle;
        }
        i = (i + 1) % 64;
    } while (start != i);
    return 0;
}

ModelInstance *EffectManager::func_0202be14(u32 group, u32 index) {
    ModelAnim *anim = new ModelAnim(mAnimData[group], mModels[group][index]);
    ModelInstance *model = mModels[group][index]->func_02028b78(anim);
    model->mUnk_69 = 0;
    return model;
}

void EffectManager::func_0202be68() {
    for (u16 i = 0; i < 64; i++) {
        Effect *effect = mEffects[i];
        if (effect != NULL) {
            effect->~Effect();
            operator delete(effect);
        }
        mEffects[i] = NULL;
    }
}

void func_0202beb8(EffectManager *manager, u16 handle) {
    u32 i          = handle - 1;
    Effect *effect = manager->mEffects[i];
    if (effect != NULL) {
        effect->~Effect();
        operator delete(effect);
    }
    manager->mEffects[i] = NULL;
}

void func_0202bef4(EffectManager *manager, u16 handle, VecFx32 *position) {
    Effect *effect = manager->mEffects[handle - 1];
    if (effect == NULL) {
        return;
    }
    effect->func_0202b500(position);
}

void func_0202bf18(EffectManager *manager, u16 handle, VecFx32 *scale) {
    Effect *effect = manager->mEffects[handle - 1];
    if (effect == NULL) {
        return;
    }
    effect->func_0202b524(scale);
}

void func_0202bf3c(EffectManager *manager, u16 handle, u16 *rotation) {
    Effect *effect = manager->mEffects[handle - 1];
    if (effect == NULL) {
        return;
    }
    effect->func_0202b548(rotation);
}

void func_0202bf60(EffectManager *manager, u16 handle, MtxFx43 *mtx) {
    Effect *effect = manager->mEffects[handle - 1];
    if (effect == NULL) {
        return;
    }
    effect->func_0202b570(mtx);
}

void EffectManager::Update() {
    func_0202bf94(0);
}

void EffectManager::func_0202bf94(u32 tag) {
    u8 passes = mPasses;
    u8 i;
    do {
        i = 0;
        do {
            if (mEffects[i] != NULL && tag == mTags[i] && mEffects[i]->func_0202acb4() == 0) {
                Effect *effect = mEffects[i];
                if (effect != NULL) {
                    effect->~Effect();
                    operator delete(effect);
                }
                mEffects[i] = NULL;
            }
            i++;
        } while (i < 64);
        passes--;
    } while (passes != 0);
}

void EffectManager::func_0202c024(u32 passes) {
    mPasses = passes;
}
