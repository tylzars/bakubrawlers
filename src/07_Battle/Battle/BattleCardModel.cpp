#include "Battle/BattleCardModel.hpp"
#include <nitro/fx.h>

#include "Battle/BattleBase.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/ModelResource.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameTask.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern const u8 data_ov007_0227c428[];

// Copies the active camera into the 3D engine state and the camera manager's snapshot of it.
static inline void SnapshotCamera() {
    CameraManager &manager        = CameraManager::Get();
    Camera *camera                = manager.mCameras[manager.mActive];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mActive] = data_02211374.mProjection;
    manager.mView[manager.mActive]       = data_02211374.mView;
    manager.mMtxA[manager.mActive]       = *func_02032710();
    manager.mMtxB[manager.mActive]       = *func_02032604();
    manager.mMtxC[manager.mActive]       = *func_020323b8();
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

extern const u8 data_020a1074[];
extern BattleCardModelEntry *data_ov007_0228acc0[];
extern const VecFx32 data_ov007_0227c0c4;
extern MtxFx43 data_0220e86c;
extern const u8 data_ov007_0227c418[];
extern const u8 data_ov007_0227c438[];
extern const u8 data_ov007_0227c3f0[];
extern u32 *data_ov007_0228acf4[];

extern "C" s32 func_0203340c(void *dict, const char *name);
extern "C" void func_01ffd088(MtxFx43 *m, u16 angle);
extern "C" void func_01ffd0e0(MtxFx43 *m, u16 angle);
extern "C" void func_01ffd138(MtxFx43 *m, u16 angle);
extern "C" void func_01ffcf64(MtxFx43 *m, VecFx32 *v);
extern "C" void func_01ff89c0(MtxFx43 *a, MtxFx43 *b, MtxFx43 *out);
extern "C" void func_01ff8634(MtxFx43 *src, MtxFx43 *dst);
extern "C" void func_01ff84d0(VecFx32 *vec, MtxFx33 *m, VecFx32 *out);
extern "C" s32 FX_AsinIdx(s32 value);
extern "C" s32 FX_AcosIdx(s32 value);
extern "C" void func_020122f4(char *dst, const char *name);

// Frame clock singleton (instance 0x020c1884, destructor 0x020219c0).
class UnkStruct_020c1884 {
public:
    /* 00 */ PAD(0x00, 0x15);
    /* 15 */ u8 mUnk_15;
    /* 16 */ PAD(0x16, 0x18);
    /* 18 */

    ~UnkStruct_020c1884();

    static UnkStruct_020c1884 &Get() {
        static UnkStruct_020c1884 instance;
        return instance;
    }
};

BattleCardModel::BattleCardModel(EffectManager *effects, Camera *camera, u32 a, u32 b, u32 c, u32 d)
    : SceneBase(2, 0) {
    mEffects     = effects;
    mCamera      = camera;
    mUnk_94      = d;
    mTextBuffer  = 0;
    mTextBuffer2 = 0;
    mTextSize    = 0;
    mUnk_74      = 0;
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
    func_ov007_022725f0(a, b, *(u8 *) &c); // narrow read of a stack argument (placeholder, see docs/decompiling.md)
}

// non-matching: 14 words differ, the table address loads are hoisted and the entry lives in r3.
void BattleCardModel::func_ov007_022725f0(u32 type, u32 id, u32 c) {
    mUnk_8c = id;
    u8 charId = data_020a1074[id];
    mUnk_93   = 0;
    mType     = type;
    mId       = charId;
    mUnk_8d   = c;
    BattleCardModelEntry *entry = data_ov007_0228acc0[mType] + charId;
    mEntry                      = entry;
    mUnk_24                     = entry->mUnk_0c;
    mParts                      = mEntry->mParts;
    mCount                      = mEntry->mCount;
    mBaku                       = 0;
    mUnk_18                     = 0;
    mUnk_28                     = 0;
    mState                      = 0;
    MI_CpuClear8(mHandles, 5);
    mUnk_96    = 0;
    mLastFrame = 0;
}

BattleCardModel::~BattleCardModel() {
    UnkStruct_02029ca8 *palette = mUnk_28;
    if (palette) {
        delete palette;
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
    if (mUnk_18) {
        OS_Free(mUnk_18);
    }
    func_0202bc58(mEffects, 0);
    func_0202bc58(mEffects, 1);
}

void BattleCardModel::Update() {
    mPasses = UnkStruct_020c1884::Get().mUnk_15 + 1;
    mEffects->func_0202c024(mPasses);
    mUnk_96 = 0;
    u8 state;
    u8 i = 0;
    do {
        state = mState;
        if (func_ov007_0227292c()) {
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
            MtxFx43 mtx = data_0220e86c;
            VecFx32 pos = data_ov007_0227c0c4;
            mBaku->mModel->func_02029174(&mtx, &pos);
            func_ov007_02274238();
            func_ov007_022737e0();
            break;
        }
    }
}

BOOL BattleCardModel::func_ov007_0227292c() {
    switch (mState) {
        case 0:
            SpriteManager::Get();
            SpriteManager::sFade[0]       = 1;
            SpriteManager::sBrightness[0] = -16;
            func_ov007_02273ec0();
            func_ov007_022734d8();
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
                if (!mBaku->mModel->func_02028d80()) {
                    mBaku->mModel->func_02028de8(0, 0);
                    mBaku->mModel->func_02028e70((s32) FramesToFloat(mEntry->mUnk_00));
                    mUnk_93 = 1;
                }
                mCamera->func_020264a4(1);
            } else {
                if (!mBaku->mModel->func_02028d80()) {
                    mState++;
                }
                if (mBaku->mModel->func_02028ea4() >= mLastFrame) {
                    SpriteManager::Get();
                    if (SpriteManager::sFade[0] <= 1) {
                        SpriteManager::Get();
                        SpriteManager::sFade[0] = 3;
                    } else {
                        s8 brightness = -(s8) ((mBaku->mModel->func_02028ea4() - mLastFrame) >> 12);
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
                mCamera->func_020264a4(0);
            }
            mCamera->func_02026180();
            SnapshotCamera();
            if (mUnk_24 != 0) {
                if (mBaku->mModel->func_02028ea4() >= (s32) FramesToFloat(mEntry->mUnk_0a)) {
                    mUnk_24 = 0;
                } else if (mBaku->mModel->func_02028ea4() >= (s32) FramesToFloat(mEntry->mUnk_08)) {
                    VecFx32 offset;
                    VecFx32 delta;
                    Camera *camA = mCamera;
                    delta.x      = camA->mPosition.x;
                    delta.y      = camA->mPosition.y;
                    delta.z      = camA->mPosition.z;
                    BattleCardModelKey *key = (BattleCardModelKey *) mUnk_24;
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
                    mUnk_24 += 8;
                    key = (BattleCardModelKey *) mUnk_24;
                    if ((key->mX == -1) & (key->mY == -1)) {
                        mUnk_24 = mEntry->mUnk_0c;
                    }
                }
            }
            func_ov007_02273718();
            break;
        case 3:
            SpriteManager::Get();
            if (SpriteManager::sFade[0] > 1) {
                mCamera->func_020264a4(1);
                mCamera->func_02026180();
                SnapshotCamera();
                func_ov007_02273718();
            } else {
                mEffects->func_0202be68();
                MI_CpuFill8(mHandles, 0, 5);
                mEffects->func_0202c024(1);
                return TRUE;
            }
            break;
    }
    return FALSE;
}

// non-matching: 0x22c vs 0x240; the original recomputes mParts + i * 0x28 for each of its three reads (separate mla with
// three constant registers) where mwcc shares one product.
void BattleCardModel::func_ov007_022734d8() {
    char name[0x80];
    char bone[0x11];
    sprintf(name, "realb%02d", mId);
    func_0202ba64(mEffects, 0, name);
    func_0202ba64(mEffects, 1, "union");
    ChangeDir("/Game/Realbaku/Mdl");
    sprintf(name, "realb%02d_M_%02d.cam", mId, data_ov007_0227c438[mType]);
    mUnk_18 = func_02023504(name, 0, 0, 0, 0, 0);
    mCamera->func_0202610c(mUnk_18, 0);
    ChangeDir("/");
    mBaku      = new BattleRealBaku(mUnk_8c, data_ov007_0227c418[mType], mUnk_8d, 0, 0, 1, mUnk_94);
    mLastFrame = mBaku->mAnim->func_020297d4(0) - 0x10000;
    mCamera->func_02026180();
    if (mCount) {
        u8 i = 0;
        do {
            if ((mParts[i].mKind == 1) | (mParts[i].mKind == 3)) {
                MI_CpuFill8(bone, 0, 0x11);
                func_020122f4(bone, mParts[i].mName);
                u8 *model  = *(u8 **) &mBaku->mModel->mRenderObj[4];
                void *dict = model ? model + 0x40 : 0;
                s32 index  = dict ? func_0203340c(dict, bone) : -1;
                mBones[i]  = index;
            }
            i++;
        } while (i < mCount);
    }
    mUnk_28 = new UnkStruct_02029ca8(data_ov007_0227c3f0);
}

// non-matching: 0xc4 vs 0xc8; the original adds the base to i * 0x28 once per field read (kind bit, script) where mwcc
// shares one address.
void BattleCardModel::func_ov007_02273718() {
    if (mCount) {
        u8 i = 0;
        do {
            if (mUnk_93 == 0) {
                s32 frame = mParts[i].mFrame;
                if (frame == 0) {
                    frame = 0x1000;
                }
                if (frame == mBaku->mModel->func_02028ea4()) {
                    if (mHandles[i]) {
                        func_0202beb8(mEffects, mHandles[i]);
                    }
    u32 group = 1;
 if (!mParts[i].mUnk_17_0) group = 0;
 mHandles[i] = func_0202bd40(mEffects, group, mParts[i].mScript, 0, 0);
                }
            }
            i++;
        } while (i < mCount);
    }
}

void BattleCardModel::func_ov007_022737e0() {
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
                        func_0202bef4(mEffects, mHandles[i], &mPos[i]);
                        break;
                    case 1:
                        func_ov007_02273a4c(i);
                        break;
                    case 2:
                        rot = data_0220e86c;
                        func_01ffd088(&rot, mParts[i].mRot[0]);
                        func_01ffd0e0(&rot, mParts[i].mRot[1]);
                        func_01ffd138(&rot, mParts[i].mRot[2]);
                        func_01ffcf64(&rot, &mParts[i].mPos);
                        func_01ff89c0(&rot, &CameraManager::Get().mMtxC[0], &rot);
                        func_0202bf60(mEffects, mHandles[i], &rot);
                        break;
                    case 3:
                        func_ov007_02273a4c(i);
                        MTX_MultVec43(&mParts[i].mTarget, &CameraManager::Get().mMtxC[0], &dir);
                        VEC_Subtract(&mPos[i], &dir, &dir);
                        VEC_Normalize(&dir, &dir);
                        angles[0] = FX_AsinIdx(dir.y);
                        angles[1] = FX_AcosIdx(dir.x) - 0x4000;
                        angles[2] = 0;
                        func_0202bf3c(mEffects, mHandles[i], angles);
                        break;
                }
            }
            i++;
        } while (i < mCount);
    }
}

void BattleCardModel::func_ov007_02273a4c(u32 index) {
    MtxFx43 world;
    MtxFx43 rot;
    MtxFx43 *bone = mBaku->mModel->func_02029084(mBones[index]);
    func_01ff89c0(bone, &CameraManager::Get().mMtxC[0], &world);
    rot = data_0220e86c;
    func_01ffd088(&rot, mParts[index].mRot[0]);
    func_01ffd0e0(&rot, mParts[index].mRot[1]);
    func_01ffd138(&rot, mParts[index].mRot[2]);
    func_01ff8634(&rot, &rot);
    func_01ff89c0(&rot, &world, &world);
    BattleCardModelPart *part = &mParts[index];
    mPos[index].x = part->mPos.x;
    mPos[index].y = part->mPos.y;
    mPos[index].z = part->mPos.z;
    func_01ff84d0(&mPos[index], (MtxFx33 *) &world, &mPos[index]);
    mPos[index].x += world._[9];
    mPos[index].y += world._[10];
    mPos[index].z += world._[11];
    func_0202bef4(mEffects, mHandles[index], &mPos[index]);
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
            SoundManager::Get().func_020246f8(9);
            return;
        case 9:
            SoundManager::Get().func_020246f8(0x6d);
            return;
        case 10:
            SoundManager::Get().func_020246f8(0x6e);
            return;
        case 11:
            SoundManager::Get().func_020246f8(0x6f);
            return;
        case 12:
            SoundManager::Get().func_020246f8(0x6c);
            return;
    }
    if (mType > 4) {
        return;
    }
    u32 *sounds = data_ov007_0228acf4[mId];
    if (sounds && sounds[mType] != 0x1ee) {
        FrameTask::Get().func_0203dc34(2, (u16) data_ov007_0228acf4[mId][mType]);
    }
}

void func_ov007_02273e3c(BattleCardModel *model) {
    if (model->mState >= 3) {
        return;
    }
    model->mState = 3;
    SpriteManager::Get();
    SpriteManager::sFade[0] = 3;
}

void BattleCardModel::func_ov007_02273ec0() {
    if (mType != 0) {
        return;
    }
    MessageFile *file = (MessageFile *) func_02023504("/font/mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mTextSize         = TextManager::Get().BufferSize(0x20, 2);
    mUnk_74           = TextManager::Get().GlyphCount(8);
    mTextBuffer       = OS_Alloc(mTextSize);
    mTextBuffer2      = OS_Alloc(mUnk_74 * 0xc);
    TextManager::Get().Init(mTextBuffer2, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    MI_CpuClear16(mTextBuffer, mTextSize);
    u16 index = mUnk_8c;
    if ((index == 5) | (index == 6)) {
        index += 0x24;
    }
    TextManager::Get().mUnk_5b = 1;
    TextManager &text          = TextManager::Get();
    s16 width;
    s16 height;
    height = 0;
    width  = 0;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffer, 0x20, 2, &width, &height, &mTextWidth, 0, 2,
               0x21, 0x10, 0, 0, 0, 1, 1);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0, mTextBuffer, mTextSize, 0);
    TextManager::Get().mUnk_5b = 0;
    OS_Free(file);
}

void BattleCardModel::func_ov007_02274238() {
    if (mUnk_96) {
        return;
    }
    if (mType == 0) {
        SpriteManager::Get().mScreens[0].func_02027778(mTextBuffer2, 8, (s16) (0x80 - ((u32) mTextWidth >> 1)), 0xb0, 0, 0, 0xf, 0);
    }
    mUnk_96 = 1;
}
