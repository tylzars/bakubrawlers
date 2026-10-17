#include "Battle/BattleCardModel.hpp"
#include <nitro/fx.h>

#include "Battle/BattleBase.hpp"
#include "Text/TextManager.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/ModelResource.hpp"
#include "System/SoundManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameTask.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Graphics/Camera.hpp"

// Game system singleton (instance 0x020c1884, destructor 0x020219c0); this file reads only mWirelessWaitFrames.
class GameSystem {
public:
    /* 00 */ PAD(0x00, 0x15);
    /* 15 */ u8 mWirelessWaitFrames;
    /* 16 */ PAD(0x16, 0x18);
    /* 18 */

    ~GameSystem();

    static GameSystem &Get() {
        static GameSystem instance;
        return instance;
    }
};

#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern const u8 data_ov007_0227c428[];

// Copies the active camera into the 3D engine state and the camera manager's snapshot of it.
static inline void SnapshotCamera() {
    CameraManager &manager        = CameraManager::Get();
    Camera *camera                = manager.mCameras[manager.mCameraIndex];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
    manager.mView[manager.mCameraIndex]       = data_02211374.mView;
    manager.mMtxA[manager.mCameraIndex]       = *func_02032710();
    manager.mMtxB[manager.mCameraIndex]       = *func_02032604();
    manager.mMtxC[manager.mCameraIndex]       = *func_020323b8();
}

struct BattleCardModelKey {
    s32 mX;
    s32 mY;
};

// Whole number of frames in 20.12 fixed point, rounded (as a float, the caller converts).
static inline float FramesToFloat(u32 frames) {
    fx32 raw = frames << 12;
    if (frames != 0) {
        return (float) raw + 0.5f;
    }
    return (float) raw - 0.5f;
}

extern const u8 gBakuganNumbers[];
extern BattleCardModelEntry data_ov007_02282318[];
extern BattleCardModelEntry data_ov007_02282738[];
extern BattleCardModelEntry data_ov007_02282b58[];
extern BattleCardModelEntry data_ov007_02282f78[];
extern BattleCardModelEntry data_ov007_02283398[];
extern BattleCardModelEntry data_ov007_022837b8[];
extern BattleCardModelEntry data_ov007_02283bd8[];
extern BattleCardModelEntry data_ov007_02283ff8[];
extern BattleCardModelEntry data_ov007_02284418[];
extern BattleCardModelEntry data_ov007_02284838[];
extern BattleCardModelEntry data_ov007_02284c58[];
extern BattleCardModelEntry data_ov007_02285078[];
extern BattleCardModelEntry data_ov007_02285498[];
BattleCardModelEntry *data_ov007_0228acc0[13] = {
    data_ov007_02282318,
    data_ov007_02282738,
    data_ov007_02282b58,
    data_ov007_02282f78,
    data_ov007_02283398,
    data_ov007_022837b8,
    data_ov007_02283bd8,
    data_ov007_02283ff8,
    data_ov007_02284418,
    data_ov007_02284838,
    data_ov007_02284c58,
    data_ov007_02285078,
    data_ov007_02285498,
};
extern const VecFx32 gCardModelScale;
extern MtxFx43 gIdentityMtx43;
extern const u8 data_ov007_0227c418[];
extern const u8 data_ov007_0227c438[];
extern const u8 data_ov007_0227c3f0[];
extern u32 data_ov007_0227c0d0[];
extern u32 data_ov007_0227c0e4[];
extern u32 data_ov007_0227c0f8[];
extern u32 data_ov007_0227c10c[];
extern u32 data_ov007_0227c120[];
extern u32 data_ov007_0227c134[];
extern u32 data_ov007_0227c148[];
extern u32 data_ov007_0227c15c[];
extern u32 data_ov007_0227c170[];
extern u32 data_ov007_0227c184[];
extern u32 data_ov007_0227c198[];
extern u32 data_ov007_0227c1ac[];
extern u32 data_ov007_0227c1c0[];
extern u32 data_ov007_0227c1d4[];
extern u32 data_ov007_0227c1e8[];
extern u32 data_ov007_0227c1fc[];
extern u32 data_ov007_0227c210[];
extern u32 data_ov007_0227c224[];
extern u32 data_ov007_0227c238[];
extern u32 data_ov007_0227c24c[];
extern u32 data_ov007_0227c260[];
extern u32 data_ov007_0227c274[];
extern u32 data_ov007_0227c288[];
extern u32 data_ov007_0227c29c[];
extern u32 data_ov007_0227c2b0[];
extern u32 data_ov007_0227c2c4[];
extern u32 data_ov007_0227c2d8[];
extern u32 data_ov007_0227c2ec[];
extern u32 data_ov007_0227c300[];
extern u32 data_ov007_0227c314[];
extern u32 data_ov007_0227c328[];
extern u32 data_ov007_0227c33c[];
extern u32 data_ov007_0227c350[];
extern u32 data_ov007_0227c364[];
extern u32 data_ov007_0227c378[];
extern u32 data_ov007_0227c38c[];
extern u32 data_ov007_0227c3a0[];
extern u32 data_ov007_0227c3b4[];
extern u32 data_ov007_0227c3c8[];
extern u32 data_ov007_0227c3dc[];
u32 *data_ov007_0228acf4[66] = {
    data_ov007_0227c0d0,
    data_ov007_0227c0f8,
    0,
    0,
    data_ov007_0227c170,
    data_ov007_0227c184,
    data_ov007_0227c210,
    data_ov007_0227c1ac,
    data_ov007_0227c1c0,
    data_ov007_0227c1e8,
    data_ov007_0227c1fc,
    data_ov007_0227c238,
    data_ov007_0227c260,
    data_ov007_0227c134,
    data_ov007_0227c29c,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    data_ov007_0227c2ec,
    data_ov007_0227c300,
    data_ov007_0227c328,
    data_ov007_0227c350,
    data_ov007_0227c378,
    data_ov007_0227c38c,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    data_ov007_0227c3c8,
    data_ov007_0227c3a0,
    data_ov007_0227c3b4,
    data_ov007_0227c10c,
    data_ov007_0227c148,
    data_ov007_0227c2c4,
    data_ov007_0227c15c,
    data_ov007_0227c33c,
    data_ov007_0227c198,
    data_ov007_0227c224,
    data_ov007_0227c1d4,
    data_ov007_0227c24c,
    0,
    data_ov007_0227c274,
    0,
    0,
    0,
    data_ov007_0227c2d8,
    0,
    data_ov007_0227c314,
    0,
    data_ov007_0227c364,
    data_ov007_0227c3dc,
    data_ov007_0227c0e4,
    data_ov007_0227c120,
    data_ov007_0227c2b0,
    data_ov007_0227c288,
};

extern "C" s32 FindDictIndexByName(void *dict, const char *name);
extern "C" void MTX_ApplyRotX43(MtxFx43 *m, u16 angle);
extern "C" void MTX_ApplyRotY43(MtxFx43 *m, u16 angle);
extern "C" void MTX_ApplyRotZ43(MtxFx43 *m, u16 angle);
extern "C" void MTX_SetTranslation43(MtxFx43 *m, VecFx32 *v);
extern "C" void MTX_Concat43(MtxFx43 *a, MtxFx43 *b, MtxFx43 *out);
extern "C" void MTX_Inverse43(MtxFx43 *src, MtxFx43 *dst);
extern "C" void MTX_MultVec33(VecFx32 *vec, MtxFx33 *m, VecFx32 *out);
extern "C" s32 FX_AsinIdx(s32 value);
extern "C" s32 FX_AcosIdx(s32 value);
extern "C" void func_020122f4(char *dst, const char *name);


BattleCardModel::BattleCardModel(EffectManager *effects, Camera *camera, u32 a, u32 b, u32 c, u32 d)
    : SceneBase(2, 0) {
    mEffects     = effects;
    mCamera      = camera;
    mUseRealb20      = d;
    mTextBuffer  = 0;
    mTextBuffer2 = 0;
    mTextBufferSize = 0;
    mGlyphCount      = 0;
    mTextWidth   = 0;
    if (a == 4) {
        switch (b) {
            case 4:
            case 5:
            case 6:
                a = 1;
                break;
            default:
                if ((b >= 0x13) & (b <= 0x23)) {
                    a = 1;
                }
                break;
        }
    }
    Init(a, b, *(u8 *) &c); // narrow read of a stack argument (placeholder, see docs/decompiling.md)
}

void BattleCardModel::Init(u32 type, u32 id, u32 c) {
    mAnimRestarted   = 0;
    mBakuganId = id;
    u8 charId = gBakuganNumbers[id];
    mVariant   = c;
    mType     = type;
    mId       = charId;
    BattleCardModelEntry *entry = data_ov007_0228acc0[mType] + charId;
    mEntry                      = entry;
    mCurrentCamKey                     = entry->mCamKeys;
    mParts                      = mEntry->mParts;
    mCount                      = mEntry->mCount;
    mBaku                       = 0;
    mCamFile                     = 0;
    mLights                     = 0;
    mState                      = 0;
    MI_CpuClear8(mHandles, 5);
    mUnk_96    = 0;
    mLastFrame = 0;
}

BattleCardModel::~BattleCardModel() {
    ModelLightGroup *lights = mLights;
    if (lights) {
        delete lights;
    }
    if (mBaku) {
        delete mBaku;
    }
    if (mTextBuffer) {
        OS_Free(mTextBuffer);
    }
    if (mTextBuffer2) {
        OS_Free(mTextBuffer2);
    }
    if (mCamFile) {
        OS_Free(mCamFile);
    }
    FreeEffectGroup(mEffects, 0);
    FreeEffectGroup(mEffects, 1);
}

void BattleCardModel::Update() {
    mPasses = GameSystem::Get().mWirelessWaitFrames + 1;
    mEffects->SetPasses(mPasses);
    mUnk_96 = 0;
    u8 state;
    u8 i = 0;
    do {
        state = mState;
        if (UpdateState()) {
            RequestScene(1);
            mFinished = 1;
            return;
        }
        i++;
    } while (i < mPasses);
    switch (state) {
        case 0:
        case 1:
            break;
        case 2:
        case 3: {
            MtxFx43 mtx = gIdentityMtx43;
            VecFx32 pos = gCardModelScale;
            mBaku->mModel->Draw(&mtx, &pos);
            DrawName();
            UpdateEffects();
            break;
        }
    }
}

BOOL BattleCardModel::UpdateState() {
    switch (mState) {
        case 0:
            SpriteManager::Get();
            SpriteManager::sFade[0]       = 1;
            SpriteManager::sBrightness[0] = -16;
            LoadNameText();
            LoadGraphics();
            mState++;
            break;
        case 1:
            func_ov007_02273be8();
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mState++;
            break;
        case 2:
            if (data_ov007_0227c428[mType] != 0) {
                if (!mBaku->mModel->AdvanceAnim()) {
                    mBaku->mModel->StartAnim(0, 0);
                    mBaku->mModel->SetAnimFrame((s32) FramesToFloat(mEntry->mRestartFrame));
                    mAnimRestarted = 1;
                }
                mCamera->AdvanceFrame(1);
            } else {
                if (!mBaku->mModel->AdvanceAnim()) {
                    mState++;
                }
                if (mBaku->mModel->GetAnimFrame() >= mLastFrame) {
                    SpriteManager::Get();
                    if (SpriteManager::sFade[0] <= 1) {
                        SpriteManager::Get();
                        SpriteManager::sFade[0] = 3;
                    } else {
                        s8 brightness = -(s8) ((mBaku->mModel->GetAnimFrame() - mLastFrame) >> 12);
                        if (brightness < -16) {
                            brightness = -16;
                        }
                        SpriteManager::Get();
                        SpriteManager::sFade[0]       = 1;
                        SpriteManager::sBrightness[0] = brightness;
                        SpriteManager::Get();
                        SpriteManager::sFade[0] = 3;
                    }
                }
                mCamera->AdvanceFrame(0);
            }
            mCamera->ApplyPath();
            SnapshotCamera();
            if (mCurrentCamKey != 0) {
                if (mBaku->mModel->GetAnimFrame() >= (s32) FramesToFloat(mEntry->mCamKeyEndFrame)) {
                    mCurrentCamKey = 0;
                } else if (mBaku->mModel->GetAnimFrame() >= (s32) FramesToFloat(mEntry->mCamKeyStartFrame)) {
                    VecFx32 offset;
                    VecFx32 delta;
                    Camera *camA = mCamera;
                    delta.x      = camA->mPosition.x;
                    delta.y      = camA->mPosition.y;
                    delta.z      = camA->mPosition.z;
                    BattleCardModelKey *key = (BattleCardModelKey *) mCurrentCamKey;
                    s32 kx = key->mX;
                    s32 ky = key->mY;
                    offset.x = kx;
                    offset.y = ky;
                    offset.z = 0;
                    MTX_MultVec43(&offset, CameraManager::Get().mMtxC, &offset);
                    VEC_Subtract(&offset, &delta, &offset);
                    Camera *camB = mCamera;
                    delta.x      = camB->mTarget.x;
                    delta.y      = camB->mTarget.y;
                    delta.z      = camB->mTarget.z;
                    VEC_Add(&delta, &offset, &delta);
                    Camera *camC = mCamera;
                    camC->mTarget.x = delta.x;
                    camC->mTarget.y = delta.y;
                    camC->mTarget.z = delta.z;
                    SnapshotCamera();
                    mCurrentCamKey += 8;
                    key = (BattleCardModelKey *) mCurrentCamKey;
                    if ((key->mX == -1) & (key->mY == -1)) {
                        mCurrentCamKey = mEntry->mCamKeys;
                    }
                }
            }
            StartEffectsAtFrame();
            break;
        case 3:
            SpriteManager::Get();
            if (SpriteManager::sFade[0] > 1) {
                mCamera->AdvanceFrame(1);
                mCamera->ApplyPath();
                SnapshotCamera();
                StartEffectsAtFrame();
            } else {
                mEffects->FreeAllEffects();
                MI_CpuFill8(mHandles, 0, 5);
                mEffects->SetPasses(1);
                return TRUE;
            }
            break;
    }
    return FALSE;
}

// Part `index` of a parts table passed by reference; reading the table through it reloads mParts, as the
// original does for each part read of LoadGraphics.
static inline BattleCardModelPart *PartAt(BattleCardModelPart *&parts, int index) {
    return &parts[index];
}

void BattleCardModel::LoadGraphics() {
    char name[0x80];
    char bone[0x11];
    sprintf(name, "realb%02d", mId);
    LoadEffectGroup(mEffects, 0, name);
    LoadEffectGroup(mEffects, 1, "union");
    ChangeDir("/Game/Realbaku/Mdl");
    sprintf(name, "realb%02d_M_%02d.cam", mId, data_ov007_0227c438[mType]);
    mCamFile = LoadFile(name, 0, 0, 0, 0, 0);
    mCamera->SetPath(mCamFile, 0);
    ChangeDir("/");
    mBaku      = new BattleRealBaku(mBakuganId, data_ov007_0227c418[mType], mVariant, 0, 0, 1, mUseRealb20);
    mLastFrame = mBaku->mAnim->GetFrameCountForSet(0) - 0x10000;
    mCamera->ApplyPath();
    if (mCount) {
        u8 i = 0;
        do {
            // Placeholder: the (u32) conversion keeps this index product apart from the first read's (the original computes
            // mParts + i * 0x28 separately for all three reads).
            if ((mParts[i].mKind == 1) | (PartAt(mParts, (u32) i)->mKind == 3)) {
                MI_CpuFill8(bone, 0, 0x11);
                func_020122f4(bone, PartAt(mParts, i)->mName);
                u8 *model  = *(u8 **) &mBaku->mModel->mRenderObj[4];
                void *dict = model ? model + 0x40 : 0;
                s32 index  = dict ? FindDictIndexByName(dict, bone) : -1;
                mBones[i]  = index;
            }
            i++;
        } while (i < mCount);
    }
    mLights = new ModelLightGroup(data_ov007_0227c3f0);
}

void BattleCardModel::StartEffectsAtFrame() {
    if (mCount) {
        u8 i = 0;
        do {
            if (mAnimRestarted == 0) {
                s32 frame = Part(i)->mFrame;
                if (frame == 0) {
                    frame = 0x1000;
                }
                if (frame == mBaku->mModel->GetAnimFrame()) {
                    if (mHandles[i]) {
                        FreeEffect(mEffects, mHandles[i]);
                    }
                    u32 group = 1;
                    if (!Part(i)->mUnk_17_0) {
                        group = 0;
                    }
                    mHandles[i] = StartEffect(mEffects, group, Part(i)->mScript, 0, 0);
                }
            }
            i++;
        } while (i < mCount);
    }
}

void BattleCardModel::UpdateEffects() {
    MtxFx43 rot;
    VecFx32 dir;
    u16 angles[3];
    if (mCount) {
        u8 i = 0;
        do {
            if (mHandles[i] != 0) {
                BattleCardModelPart *part = &mParts[i];
                switch (part->mKind) {
                    case 0:
                        mPos[i].x = part->mPos.x;
                        mPos[i].y = part->mPos.y;
                        mPos[i].z = part->mPos.z;
                        SetEffectPosition(mEffects, mHandles[i], &mPos[i]);
                        break;
                    case 1:
                        MoveEffectToBone(i);
                        break;
                    case 2:
                        rot = gIdentityMtx43;
                        MTX_ApplyRotX43(&rot, mParts[i].mRot[0]);
                        MTX_ApplyRotY43(&rot, mParts[i].mRot[1]);
                        MTX_ApplyRotZ43(&rot, mParts[i].mRot[2]);
                        MTX_SetTranslation43(&rot, &mParts[i].mPos);
                        MTX_Concat43(&rot, &CameraManager::Get().mMtxC[0], &rot);
                        SetEffectMatrix(mEffects, mHandles[i], &rot);
                        break;
                    case 3:
                        MoveEffectToBone(i);
                        MTX_MultVec43(&mParts[i].mTarget, &CameraManager::Get().mMtxC[0], &dir);
                        VEC_Subtract(&mPos[i], &dir, &dir);
                        VEC_Normalize(&dir, &dir);
                        angles[0] = FX_AsinIdx(dir.y);
                        angles[1] = FX_AcosIdx(dir.x) - 0x4000;
                        angles[2] = 0;
                        SetEffectRotation(mEffects, mHandles[i], angles);
                        break;
                }
            }
            i++;
        } while (i < mCount);
    }
}

void BattleCardModel::MoveEffectToBone(u32 index) {
    MtxFx43 world;
    MtxFx43 rot;
    MtxFx43 *bone = mBaku->mModel->GetCallbackMtx(mBones[index]);
    MTX_Concat43(bone, &CameraManager::Get().mMtxC[0], &world);
    rot = gIdentityMtx43;
    MTX_ApplyRotX43(&rot, mParts[index].mRot[0]);
    MTX_ApplyRotY43(&rot, mParts[index].mRot[1]);
    MTX_ApplyRotZ43(&rot, mParts[index].mRot[2]);
    MTX_Inverse43(&rot, &rot);
    MTX_Concat43(&rot, &world, &world);
    BattleCardModelPart *part = &mParts[index];
    mPos[index].x = part->mPos.x;
    mPos[index].y = part->mPos.y;
    mPos[index].z = part->mPos.z;
    MTX_MultVec33(&mPos[index], (MtxFx33 *) &world, &mPos[index]);
    mPos[index].x += world._[9];
    mPos[index].y += world._[10];
    mPos[index].z += world._[11];
    SetEffectPosition(mEffects, mHandles[index], &mPos[index]);
}

void BattleCardModel::func_ov007_02273be8() {
    if (mId > 0x41) {
        return;
    }
    switch (mType) {
        case 5:
        case 6:
        case 7:
        case 8:
            SoundManager::Get().StartSeqArc(9);
            return;
        case 9:
            SoundManager::Get().StartSeqArc(0x6d);
            return;
        case 10:
            SoundManager::Get().StartSeqArc(0x6e);
            return;
        case 11:
            SoundManager::Get().StartSeqArc(0x6f);
            return;
        case 12:
            SoundManager::Get().StartSeqArc(0x6c);
            return;
    }
    if (mType > 4) {
        return;
    }
    u32 *sounds = data_ov007_0228acf4[mId];
    if (sounds && sounds[mType] != 0x1ee) {
        FrameTask::Get().StartChannel(2, (u16) data_ov007_0228acf4[mId][mType]);
    }
}

void CloseCardModel(BattleCardModel *model) {
    if (model->mState >= 3) {
        return;
    }
    model->mState = 3;
    SpriteManager::Get();
    SpriteManager::sFade[0] = 3;
}

void BattleCardModel::LoadNameText() {
    if (mType != 0) {
        return;
    }
    MessageFile *file = (MessageFile *) LoadFile("/font/mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x20, 2);
    mGlyphCount           = TextManager::Get().GlyphCount(8);
    mTextBuffer       = OS_Alloc(mTextBufferSize);
    mTextBuffer2      = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mTextBuffer2, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    u16 index = mBakuganId;
    if ((index == 5) | (index == 6)) {
        index += 0x24;
    }
    TextManager::Get().mUse4bppFont = 1;
    TextManager &text          = TextManager::Get();
    s16 width;
    s16 height;
    height = 0;
    width  = 0;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffer, 0x20, 2, &width, &height, &mTextWidth, 0, 2,
               0x21, 0x10, 0, 0, 0, 1, 1);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0, mTextBuffer, mTextBufferSize, 0);
    TextManager::Get().mUse4bppFont = 0;
    OS_Free(file);
}

void BattleCardModel::DrawName() {
    if (mUnk_96) {
        return;
    }
    if (mType == 0) {
        SpriteManager::Get().mScreens[0].AddOam(mTextBuffer2, 8, (s16) (0x80 - ((u32) mTextWidth >> 1)), 0xb0, 0, 0, 0xf, 0);
    }
    mUnk_96 = 1;
}
