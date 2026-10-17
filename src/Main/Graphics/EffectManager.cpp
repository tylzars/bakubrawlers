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
    mMtx          = gIdentityMtx43;
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
            model->mOwnFlags = 1;
            delete model;
        }
    }
}

BOOL Effect::Update() {
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
                mModels[slot] = sManager->CreateModelInstance(mGroup, id);
                mModels[slot]->StartAnimWithIncrement(keep, 0, mAnimSpeed);
                u16 frame = mFrame;
                if (frame != 0) {
                    mModels[slot]->SetAnimFrame(FX32_CONST((float) frame));
                }
                mLocal[slot] = gIdentityMtx43;
                if (mModelRot[1] != 0) {
                    MTX_ApplyRotY43(&mLocal[slot], mModelRot[1]);
                }
                if (mModelRot[0] != 0) {
                    MTX_ApplyRotX43(&mLocal[slot], mModelRot[0]);
                }
                if (mModelRot[2] != 0) {
                    MTX_ApplyRotZ43(&mLocal[slot], mModelRot[2]);
                }
                MTX_SetTranslation43(&mLocal[slot], &mModelPos);
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
                ReadVecArg(&mModelScale);
                if (keep == 1) {
                    mSavedScale = mModelScale;
                }
                break;
            }
            case 4: {
                keep = *mCursor++;
                ReadVecArg(&mModelPos);
                if (keep == 1) {
                    mSavedPos = mModelPos;
                }
                break;
            }
            case 5: {
                keep = *mCursor++;
                mModelRot[0] = (u16) ((float) ReadU16Arg() * 65536.0f / 360.0f);
                mModelRot[1] = (u16) ((float) ReadU16Arg() * 65536.0f / 360.0f);
                mModelRot[2] = (u16) ((float) ReadU16Arg() * 65536.0f / 360.0f);
                if (keep == 1) {
                    mSavedRot[0] = mModelRot[0];
                    mSavedRot[1] = mModelRot[1];
                    mSavedRot[2] = mModelRot[2];
                }
                break;
            }
            case 7:
                mLoopCount[mLoopDepth] = ReadU16Arg();
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
                mLoop = ReadU16Arg();
                if (keep == 1) {
                    mSavedLoop = mLoop;
                }
                break;
            }
            case 11: {
                keep = *mCursor++;
                mAnimSpeed = ReadS32Arg();
                if (keep == 1) {
                    mSavedAnimSpeed = mAnimSpeed;
                }
                break;
            }
            case 12:
                mFrame = ReadU16Arg();
                break;
            case 9:
                mWait    = ReadU16Arg();
                mWaiting = 1;
                run      = FALSE;
                break;
            }
        } while (run);
    }
    if (mMtxSet == 0) {
        mMtx = gIdentityMtx43;
        if (mRot[1] != 0) {
            MTX_ApplyRotY43(&mMtx, mRot[1]);
        }
        if (mRot[0] != 0) {
            MTX_ApplyRotX43(&mMtx, mRot[0]);
        }
        if (mRot[2] != 0) {
            MTX_ApplyRotZ43(&mMtx, mRot[2]);
        }
        MTX_SetTranslation43(&mMtx, &mPos);
    }
    mRunning = 0;
    for (u8 i = 0; i < 10; i++) {
        if (mModels[i] == NULL) {
            continue;
        }
        if ((mMtxSet & mWorldDone[i]) == 0) {
            MTX_Concat43(&mLocal[i], &mMtx, &mWorld[i]);
            mWorldDone[i] = 1;
        }
        if ((mScaleSet & mScaleDone[i]) == 0) {
            mWorldScale[i].x = FX_Mul(mScale.x, mLocalScale[i].x);
            mWorldScale[i].y = FX_Mul(mScale.y, mLocalScale[i].y);
            mWorldScale[i].z = FX_Mul(mScale.z, mLocalScale[i].z);
            mScaleDone[i]    = 1;
        }
        mModels[i]->Draw(&mWorld[i], &mWorldScale[i]);
        mAnimEnded = mModels[i]->AdvanceAnim() == 0;
        if (mAnimEnded != 0) {
            if (mAnims[i].mLoop != 0) {
                mModels[i]->StartAnimWithIncrement(mAnims[i].mAnim, 0, mAnims[i].mSpeed);
                u16 frame = mAnims[i].mFrame;
                if (frame != 0) {
                    mModels[i]->SetAnimFrame(FX32_CONST((float) frame));
                }
            } else {
                ModelInstance *model = mModels[i];
                model->mOwnFlags       = 1;
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

void Effect::SetPosition(VecFx32 *pos) {
    mPos       = *pos;
    mMtxSet    = 0;
}

void Effect::SetScale(VecFx32 *scale) {
    mScale     = *scale;
    mScaleSet  = 0;
}

void Effect::SetRotation(u16 *rot) {
    mRot[0] = rot[0];
    mRot[1] = rot[1];
    mRot[2] = rot[2];
    mMtxSet = 0;
}

void Effect::SetMatrix(MtxFx43 *mtx) {
    mMtx    = *mtx;
    mMtxSet = 1;
    u8 i    = 0;
    do {
        mWorldDone[i] = 0;
        i++;
    } while (i < 10);
}

void Effect::ReadVecArg(VecFx32 *out) {
    out->x = ReadS32Arg();
    out->y = ReadS32Arg();
    out->z = ReadS32Arg();
}

u16 Effect::ReadU16Arg() {
    if (*mCursor++ == 0) {
        return ReadU16();
    }
    return (u16) ((float) ReadRandom() / 4096.0f);
}

s32 Effect::ReadS32Arg() {
    if (*mCursor++ == 0) {
        return ReadU32();
    }
    return ReadRandom();
}

s32 Effect::ReadRandom() {
    u32 rawMin = ReadU32();
    u32 rawMax = ReadU32();
    u32 rawStep = ReadU32();
    float min  = *(float *) &rawMin;
    float max  = *(float *) &rawMax;
    float step = *(float *) &rawStep;
    float zero = 0.0f;
    if (step != zero) {
        u32 r     = MATH_Rand32(&gRandom, (u32) ((max - min) / step) + 1);
        float rnd = r;
        return FX32_CONST(min + rnd * step);
    }
    float t = (float) MATH_Rand32(&gRandom, 100001) / 100000.0f;
    return FX32_CONST(min + t * (max - min));
}

u16 Effect::ReadU16() {
    u8 lo = *mCursor++;
    u8 hi = *mCursor++;
    return lo | (hi << 8);
}

u32 Effect::ReadU32() {
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
    FreeAllEffects();
    delete[] mEffects;
    for (u8 i = 0; i < 3; i++) {
        FreeEffectGroup(this, i);
    }
}

void LoadEffectGroup(EffectManager *manager, u32 slot, const char *name) {
    char path[0x80];
    ChangeDir("/effect");
    sprintf(path, "%s.esb", name);
    EffectFile *file = (EffectFile *) LoadFile(path, 0, 0, 0, 0, 0);
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
    i = 0;
    do {
        sprintf(path, "%s.nsbmd", modelName);
        manager->mModels[slot][i] = new NitroModel(path, manager->mTextures[slot]);
        while (*modelName++ != 0) {
        }
        i++;
    } while (i < manager->mFiles[slot]->mNumModels);
    manager->mTextures[slot]->FreeFile();
    ChangeDir("/");
}

void FreeEffectGroup(EffectManager *manager, u32 slot) {
    if (manager->mFiles[slot] == NULL) {
        return;
    }
    u8 i = 0;
    do {
        NitroModel *model = manager->mModels[slot][i];
        if (model != NULL) {
            model->Release();
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
        texture->Release();
        delete texture;
    }
    manager->mTextures[slot] = NULL;
    OS_Free(manager->mFiles[slot]);
    manager->mFiles[slot] = NULL;
}

u16 StartEffect(EffectManager *manager, u32 group, u32 script, u32 position, u32 tag) {
    u16 start;
    u16 i;
    start = manager->mNext;
    i     = manager->mNext;
    do {
        if (manager->mEffects[i] == NULL) {
            Effect *effect       = new Effect(manager, group, manager->mFiles[group]->mScripts[script]);
            manager->mEffects[i] = effect;
            manager->mTags[i]    = tag;
            s32 handle           = i + 1;
            manager->mNext       = handle % 64;
            if (position != 0) {
                SetEffectPosition(manager, handle, (VecFx32 *) position);
            }
            return handle;
        }
        i = (i + 1) % 64;
    } while (start != i);
    return 0;
}

ModelInstance *EffectManager::CreateModelInstance(u32 group, u32 index) {
    ModelAnim *anim = new ModelAnim(mAnimData[group], mModels[group][index]);
    ModelInstance *model = mModels[group][index]->CreateInstance(anim);
    model->mUseBoxTest = 0;
    return model;
}

void EffectManager::FreeAllEffects() {
    for (u16 i = 0; i < 64; i++) {
        Effect *effect = mEffects[i];
        if (effect != NULL) {
            effect->~Effect();
            operator delete(effect);
        }
        mEffects[i] = NULL;
    }
}

void FreeEffect(EffectManager *manager, u16 handle) {
    u32 i          = handle - 1;
    Effect *effect = manager->mEffects[i];
    if (effect != NULL) {
        effect->~Effect();
        operator delete(effect);
    }
    manager->mEffects[i] = NULL;
}

void SetEffectPosition(EffectManager *manager, u16 handle, VecFx32 *position) {
    Effect *effect = manager->mEffects[handle - 1];
    if (effect == NULL) {
        return;
    }
    effect->SetPosition(position);
}

void SetEffectScale(EffectManager *manager, u16 handle, VecFx32 *scale) {
    Effect *effect = manager->mEffects[handle - 1];
    if (effect == NULL) {
        return;
    }
    effect->SetScale(scale);
}

void SetEffectRotation(EffectManager *manager, u16 handle, u16 *rotation) {
    Effect *effect = manager->mEffects[handle - 1];
    if (effect == NULL) {
        return;
    }
    effect->SetRotation(rotation);
}

void SetEffectMatrix(EffectManager *manager, u16 handle, MtxFx43 *mtx) {
    Effect *effect = manager->mEffects[handle - 1];
    if (effect == NULL) {
        return;
    }
    effect->SetMatrix(mtx);
}

void EffectManager::Update() {
    UpdateTagged(0);
}

void EffectManager::UpdateTagged(u32 tag) {
    u8 passes = mPasses;
    u8 i;
    do {
        i = 0;
        do {
            if (mEffects[i] != NULL && tag == mTags[i] && mEffects[i]->Update() == 0) {
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

void EffectManager::SetPasses(u32 passes) {
    mPasses = passes;
}
