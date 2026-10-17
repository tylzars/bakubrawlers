#include "Graphics/Camera.hpp"
#include "Graphics/ModelInternal.hpp"
#include <nitro/fx.h>
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FrameUpdate.hpp"
#include "Unknown/UnkFunctions.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>

// SDK matrix helpers (ITCM / main).
void MTX_Identity22_(MtxFx22 *mtx);
extern "C" void func_01ff817c(MtxFx33 *mtx); // MTX_Identity33_
extern "C" void func_01ff854c(MtxFx43 *mtx); // MTX_Identity43_
extern "C" void func_02000d88(MtxFx44 *mtx); // MTX_Identity44_

MtxFx22 gIdentityMtx22;
MtxFx33 data_0220e848;
MtxFx43 gIdentityMtx43;
MtxFx44 gIdentityMtx44;

extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y);                         // atan2 (16-bit angle)
extern "C" u16 FX_AsinIdx(fx32 y);                                 // asin (16-bit angle)
extern "C" void func_01ff827c(MtxFx33 *mtx, fx32 sin, fx32 cos);      // MTX_RotY33_
extern "C" void func_01ff8260(MtxFx33 *mtx, fx32 sin, fx32 cos);      // MTX_RotX33_
extern "C" void func_01ff82b0(MtxFx33 *a, MtxFx33 *b, MtxFx33 *out);  // MTX_Concat33
extern "C" void func_01ff84d0(VecFx32 *v, MtxFx33 *m, VecFx32 *out);  // MTX_MultVec33

extern "C" s64 FX_DivFx64c(fx32 a, fx32 b); // 64-bit fixed-point division (32 fraction bits)
extern "C" fx32 FX_Modf(fx32 value, fx32 *integer); // splits into integer part and fraction

// MTX_Perspective
extern "C" void func_02001688(fx32 sin, fx32 cos, fx32 aspect, fx32 nearPlane, fx32 farPlane, fx32 scale, MtxFx44 *mtx);
// MTX_Ortho
extern "C" void MTX_Frustum(fx32 top, fx32 bottom, fx32 left, fx32 right, fx32 nearPlane, fx32 farPlane, fx32 scale, MtxFx44 *mtx);
extern "C" void func_01ff92a4();
extern MtxFx33 data_02211430; // NNS_G3dGlb base rotation
extern const VecFx32 gDefaultBaseScale;
extern const VecFx32 gDefaultBaseTrans;

void UpdateSpriteFade();
void EndFrameCallback(void *arg);

// VRAM bank mapping and engine setup (NitroSDK GX_SetBankFor* and related).
extern "C" void GX_SetBankForBG(u32 banks);
extern "C" void GX_SetBankForTex(u32 banks);
extern "C" void GX_SetBankForOBJ(u32 banks);
extern "C" void GX_SetBankForBGExtPltt(u32 banks);
extern "C" void GX_SetBankForOBJExtPltt(u32 banks);
extern "C" void GX_SetBankForTexPltt(u32 banks);
extern "C" void GX_SetBankForLCDC(u32 banks);
extern "C" void GX_SetBankForSubBG(u32 banks);
extern "C" void GX_SetBankForSubOBJ(u32 banks);
extern "C" void GX_SetBankForSubBGExtPltt(u32 banks);
extern "C" void GX_SetBankForSubOBJExtPltt(u32 banks);
extern "C" void GX_ResetBankForBG();
extern "C" void GX_ResetBankForOBJ();
extern "C" void GX_ResetBankForBGExtPltt();
extern "C" void GX_ResetBankForOBJExtPltt();
extern "C" void GX_ResetBankForTex();
extern "C" void GX_ResetBankForTexPltt();
extern "C" void GX_ResetBankForSubBG();
extern "C" void GX_ResetBankForSubOBJ();
extern "C" void func_02002dd8();
extern "C" void func_02002dec();
extern "C" void func_02002e00();
extern "C" void GX_ResetBankForSubBGExtPltt();
extern "C" void GX_ResetBankForSubOBJExtPltt();
extern "C" void MI_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size);
s32 GetGameHeapHandle();
extern "C" void func_020313f8(void *obj, u32 unk2, s32 heap);
extern "C" void func_02035e58(u32 unk1, u32 unk2);
extern "C" void func_02036538(u32 unk1, void *buffer, u32 size, u32 unk4);
extern "C" void func_02035ee0();
extern "C" void func_020360cc(UnkStruct_SpriteManager_28 *slot);
extern "C" void func_0203610c(UnkStruct_SpriteManager_28 *slot);
extern "C" void func_02032d54();
extern "C" void G3X_InitMtxStack();
extern "C" void G3X_SetToonTable(const void *unk1);
extern "C" void G3X_SetEdgeColorTable(const void *unk1);
extern "C" void G3X_SetClearColor(int rgb, int alpha, int depth, int polygonId, BOOL fog); // G3X_SetClearColor
extern "C" void G3_Shininess(const void *unk1);

struct UnkStruct_020c2acc {
    u32 mAttr;
    u16 mPos;
};

#define REG_G3_DISP3DCNT (*(vu16 *) 0x4000060)
#define REG_G3_SWAP_BUFFERS (*(vu32 *) 0x4000540)

extern const u8 gEdgeColorTable[];
extern const u8 gToonTable[];
extern const u8 gShininessTable[];


#define INT_TO_FX(n) ((fx32) ((n) > 0 ? (float) ((n) << 12) + 0.5f : (float) ((n) << 12) - 0.5f))


static inline fx32 SinIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) * 2];
}
static inline fx32 CosIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) * 2 + 1];
}

extern "C" fx32 FX_Div(fx32 a, fx32 b); // FX_Div

void InitIdentityMatrices() {
    MTX_Identity22_(&gIdentityMtx22);
    func_01ff817c(&data_0220e848);
    func_01ff854c(&gIdentityMtx43);
    func_02000d88(&gIdentityMtx44);
}

Camera::Camera() {
    mNear      = 0x1000;
    mFar       = 0x190000;
    mDistance  = 0x1000;
    mFrameCount = 0;
    mFrameIndex      = 0;
    mPath       = NULL;
    mPathFov     = 0;
    mPitch      = 0;
    mYaw        = 0;
    mRoll       = 0;
    SetPosition(0, 0, 0);
    SetTarget(0, 0, -0x1000);
    SetUp(0, 0x1000, 0);
    SetFov(0x2aaa);
}

// Derives yaw, pitch, roll and distance from position, target and up.
void Camera::ComputeDistanceAndAngles() {
    MtxFx33 mtx;
    MtxFx33 rot;
    VecFx32 dir;
    VEC_Subtract(&mTarget, &mPosition, &dir);
    mDistance = VEC_Mag(&dir);
    VEC_Normalize(&dir, &dir);
    mYaw   = FX_Atan2Idx(-dir.x, -dir.z);
    mPitch = FX_AsinIdx(dir.y);
    mtx = data_0220e848;
    if (mYaw != 0) {
        func_01ff827c(&mtx, -SinIdx(mYaw), CosIdx(mYaw));
    }
    if (mPitch != 0) {
        func_01ff8260(&rot, -SinIdx(mPitch), CosIdx(mPitch));
        func_01ff82b0(&mtx, &rot, &mtx);
    }
    func_01ff84d0(&mUp, &mtx, &dir);
    VEC_Normalize(&dir, &dir);
    mRoll = FX_Atan2Idx(-dir.x, dir.y);
}

void Camera::SetFov(u32 fov) {
    fov = (u16) fov;
    u16 angle = fov;
    u16 scaled = angle * 3 / 4;
    u16 h = scaled >> 1;
    int half  = h;
    mFov      = angle;
    mSin      = SinIdx(half);
    mCos      = CosIdx(half);
    mNearHeight   = FX_Div(mNear * 2, mCos);
    mNearHeight   = FX_Mul(mSin, mNearHeight);
    mNearHalfWidth   = FX_Mul(mNearHeight / 2, 0x1555);
    mUnk_48   = mNearHeight * 0x69 / 192;
    mUnk_34   = FX_Div(mCos << 7, mSin) * 3 / 4;
}

void Camera::SetPath(void *file, u32 frameCount) {
    const u32 *path = (const u32 *) file;
    mFrameIndex  = 0;
    mPathFov = 0;
    if (path[0] == 0) {
        if (path[1] == 0) {
            mPath   = path + 3;
            mPathFov = path[2];
        }
    } else {
        mPath = path;
    }
    if (frameCount == 0) {
        mFrameCount = mPath[0];
    } else {
        mFrameCount = frameCount;
    }
}

void Camera::SetFrameIndex(u16 frame) {
    if (frame >= mFrameCount) {
        frame = mFrameCount - 1;
    }
    mFrameIndex = frame;
}

void Camera::ApplyPath() {
    if (mPathFov != 0) {
        SetFov(mPathFov);
    }
    fx32 keys   = INT_TO_FX(mPath[0]);
    fx32 frames = INT_TO_FX(mFrameCount);
    s64 ratio   = FX_DivFx64c(keys, frames);
    fx32 t      = (fx32) ((ratio * INT_TO_FX(mFrameIndex) + 0x80000000LL) >> 32);
    VecFx32 v;
    SampleTrack(&v, 0, t);
    mPosition = v;
    SampleTrack(&v, 1, t);
    mTarget = v;
}

void Camera::SampleTrack(VecFx32 *out, u32 track, fx32 t) {
    const VecFx32 *keys;
    u32 n      = mPath[0];
    keys       = (const VecFx32 *) (mPath + 1) + track * n;
    u32 index  = (u32) (t << 4) >> 16;
    if (n <= index) {
        *out = keys[n - 1];
        return;
    }
    u16 next = index + 1;
    if (n <= next) {
        *out = keys[n - 1];
        return;
    }
    fx32 w;
    fx32 frac = FX_Modf(t, &w);
    if (frac == 0) {
        *out = keys[index];
        return;
    }
    w         = 0x1000 - frac;
    VecFx32 a = keys[index];
    VecFx32 b = keys[next];
    VecFx32 r;
    r.z  = FX_Div(FX_Mul(a.z, w) + FX_Mul(b.z, frac), 0x1000);
    r.y  = FX_Div(FX_Mul(a.y, w) + FX_Mul(b.y, frac), 0x1000);
    r.x  = FX_Div(FX_Mul(a.x, w) + FX_Mul(b.x, frac), 0x1000);
    *out = r;
}

void Camera::AdvanceFrame(u32 loop) {
    mFrameIndex++;
    if (mFrameIndex < mFrameCount) {
        return;
    }
    if (loop) {
        mFrameIndex = 0;
    } else {
        mFrameIndex--;
    }
}

CameraManager::CameraManager() {
}

void CameraManager::Reset() {
    mModelsVisible = 1;
    mCameras[0] = NULL;
    mCameras[1] = NULL;
    mActive = 0;
    mMode = 0;
    mDirty = 0;
}

void CameraManager::SetCamerasAndRequestMode(u32 mode, Camera *camera, u32 unk3) {
    Camera *second = (Camera *) unk3;
    if (camera == NULL) {
        camera = second;
    }
    if (second == NULL) {
        second = camera;
    }
    if ((camera != NULL) | (second != NULL)) {
        mCameras[0] = camera;
        mCameras[1] = second;
    }
    if (mMode == mode) {
        mDirty = 0;
    } else {
        mDirty = 1;
        mRequestedMode = mode;
    }
}

void CameraManager::UpdateProjection() {
    if (mDirty != 0) {
        if (mRequestedMode == 0) {
            return;
        }
    }
    if (mMode == 0) {
        return;
    }
    Camera *cam0 = mCameras[0];
    Camera *cam1 = mCameras[1];
    if ((cam0 == NULL) & (cam1 == NULL)) {
        return;
    }
    switch (mMode) {
        case 1: {
            Camera *camera = mCameras[mActive];
            func_02001688(camera->mSin, camera->mCos, 0x1555, camera->mNear, camera->mFar, 0x19a, &data_02211374.mProjection);
            data_02211374.mFlags &= ~0x50;
            break;
        }
        case 2:
            if (mActive == 0) {
                MTX_Frustum(cam0->mNearHeight / 2, -cam0->mNearHeight / 2, -cam0->mNearHalfWidth, cam0->mNearHalfWidth, cam0->mNear, cam0->mFar,
                              0x19a, &data_02211374.mProjection);
                data_02211374.mFlags &= ~0x50;
            } else {
                MTX_Frustum(-cam1->mNearHeight / 2 - cam1->mUnk_48, -cam1->mNearHeight - cam1->mNearHeight / 2 - cam1->mUnk_48,
                              -cam1->mNearHalfWidth, cam1->mNearHalfWidth, cam1->mNear, cam1->mFar, 0x19a, &data_02211374.mProjection);
                data_02211374.mFlags &= ~0x50;
            }
            break;
        case 3: {
            Camera *camera = mCameras[mActive];
            func_02001688(camera->mSin, camera->mCos, 0x1555, camera->mNear, camera->mFar, 0x19a, &data_02211374.mProjection);
            data_02211374.mFlags &= ~0x50;
            break;
        }
        case 4:
            if (mActive == 0) {
                func_02001688(cam0->mSin, cam0->mCos, 0x1555, cam0->mNear, 0x18000, 0x1000, &data_02211374.mProjection);
                data_02211374.mFlags &= ~0x50;
            } else {
                func_02001688(cam1->mSin, cam1->mCos, 0x1555, 0x14000, cam1->mFar, 0x1000, &data_02211374.mProjection);
                data_02211374.mFlags &= ~0x50;
            }
            break;
    }
}

void CameraManager::SwapBuffers() {
    if (mDirty == 0) {
        if (mMode == 0) {
            return;
        }
    }
    if ((mCameras[0] == NULL) & (mCameras[1] == NULL)) {
        return;
    }
    func_01ff92a4();
    *(vu32 *) 0x4000540 = 3;
}

void CameraManager::ResetBaseTransform() {
    MI_Copy36B(&data_0220e848, &data_02211430);
    data_02211374.mFlags &= ~0xa4;
    func_020322c0(&gDefaultBaseTrans);
    func_020322f0(&gDefaultBaseScale);
}
void CameraManager::UpdateMode() {
    if (mDirty != 0) {
        mMode  = mRequestedMode;
        mDirty = 0;
        SpriteManager::Get().func_02026d90();
        switch (mMode) {
            case 0:
                break;
            case 1:
                GX_SetGraphicsMode(1, 5, 1);
                break;
            case 2:
            case 3:
                GX_SetGraphicsMode(1, 5, 1);
                GXS_SetGraphicsMode(5);
                G2S_SetBG2Control(1, 1, 0, 1, 0);
                reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & ~0x60) | 0x20;
                break;
            case 4:
                GX_SetGraphicsMode(1, 5, 1);
                G2_SetBG2Control(1, 1, 0, 1, 0);
                break;
        }
    }
    switch (mMode) {
        case 0:
            return;
        case 1:
            if (mActive == 0) {
                reg_GX_POWCNT = reg_GX_POWCNT | 0x8000;
                return;
            } else {
                reg_GX_POWCNT = reg_GX_POWCNT & ~0x8000;
                return;
            }
        case 2:
        case 3:
            if (mActive == 0) {
                reg_GX_POWCNT = reg_GX_POWCNT | 0x8000;
                func_02002e00();
                GX_SetBankForLCDC(8);
                GX_SetBankForSubBG(4);
                reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & ~0x1f00) | 0x400;
                *(vu32 *) 0x4000064 = 0x80330010;
                mActive = 1;
            } else {
                reg_GX_POWCNT = reg_GX_POWCNT & ~0x8000;
                func_02002dec();
                GX_SetBankForLCDC(4);
                GX_SetBankForSubOBJ(8);
                reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xffbfff9f) | 0x20;
                reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & ~0x1f00) | 0x1000;
                *(vu32 *) 0x4000064 = 0x80320010;
                mActive = 0;
            }
            {
                UnkStruct_020c2acc *p = (UnkStruct_020c2acc *) &SpriteManager::Get().mScreens[1];
                for (s32 i = 0; i < 12; i++) {
                    s32 row = i / 4;
                    s32 col = i % 4;
                    UnkStruct_020c2acc *e = p++;
                    e->mAttr = (((col << 6) & 0x1ff) << 16) | (((row << 6) & 0xff) | 0xc0000c00);
                    e->mPos  = (col << 3) + (row << 8) | 0xf000;
                }
            }
            return;
        case 4:
            if (mActive == 0) {
                GX_ResetBankForBG();
                GX_SetBankForLCDC(8);
                GX_SetBankForBG(2);
                reg_G2_BG0CNT = (reg_G2_BG0CNT & ~3) | 2;
                reg_G2_BG2CNT = (reg_G2_BG2CNT & ~3) | 3;
                *(vu32 *) 0x4000064 = 0x81330010;
                mActive = 1;
                return;
            } else {
                GX_ResetBankForBG();
                GX_SetBankForLCDC(2);
                GX_SetBankForBG(8);
                reg_G2_BG2CNT = (reg_G2_BG2CNT & ~3) | 2;
                reg_G2_BG0CNT = (reg_G2_BG0CNT & ~3) | 3;
                *(vu32 *) 0x4000064 = 0x81310010;
                mActive = 0;
                return;
            }
    }
}

