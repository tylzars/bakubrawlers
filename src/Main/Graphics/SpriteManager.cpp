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

MtxFx22 data_0220e838;
MtxFx33 data_0220e848;
MtxFx43 data_0220e86c;
MtxFx44 data_0220e89c;

extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y);                         // atan2 (16-bit angle)
extern "C" u16 FX_AsinIdx(fx32 y);                                 // asin (16-bit angle)
extern "C" void func_01ff827c(MtxFx33 *mtx, fx32 sin, fx32 cos);      // MTX_RotY33_
extern "C" void func_01ff8260(MtxFx33 *mtx, fx32 sin, fx32 cos);      // MTX_RotX33_
extern "C" void func_01ff82b0(MtxFx33 *a, MtxFx33 *b, MtxFx33 *out);  // MTX_Concat33
extern "C" void func_01ff84d0(VecFx32 *v, MtxFx33 *m, VecFx32 *out);  // MTX_MultVec33

extern "C" s64 func_01ff8010(fx32 a, fx32 b); // 64-bit fixed-point division (32 fraction bits)
extern "C" fx32 FX_Modf(fx32 value, fx32 *integer); // splits into integer part and fraction

// MTX_Perspective
extern "C" void func_02001688(fx32 sin, fx32 cos, fx32 aspect, fx32 nearPlane, fx32 farPlane, fx32 scale, MtxFx44 *mtx);
// MTX_Ortho
extern "C" void MTX_Frustum(fx32 top, fx32 bottom, fx32 left, fx32 right, fx32 nearPlane, fx32 farPlane, fx32 scale, MtxFx44 *mtx);
extern "C" void func_01ff92a4();
extern MtxFx33 data_02211430; // NNS_G3dGlb base rotation
extern const VecFx32 data_0209094c;
extern const VecFx32 data_02090958;

void func_020256d4();
void func_02026c4c(void *arg);

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
s32 func_02021e00();
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
static u8 sBuffer_0220e900[0x800] __attribute__((aligned(32)));

#define REG_G3_DISP3DCNT (*(vu16 *) 0x4000060)
#define REG_G3_SWAP_BUFFERS (*(vu32 *) 0x4000540)

extern const u8 data_02090968[];
extern const u8 data_02090978[];
extern const u8 data_020909c0[];


#define INT_TO_FX(n) ((fx32) ((n) > 0 ? (float) ((n) << 12) + 0.5f : (float) ((n) << 12) - 0.5f))


static inline fx32 SinIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) * 2];
}
static inline fx32 CosIdx(int idx) {
    return FX_SinCosTable_[(idx >> 4) * 2 + 1];
}

extern "C" fx32 FX_Div(fx32 a, fx32 b); // FX_Div

void func_02025e18() {
    MTX_Identity22_(&data_0220e838);
    func_01ff817c(&data_0220e848);
    func_01ff854c(&data_0220e86c);
    func_02000d88(&data_0220e89c);
}

Camera::Camera() {
    mNear      = 0x1000;
    mFar       = 0x190000;
    mDistance  = 0x1000;
    mUnk_50 = 0;
    mUnk_52      = 0;
    mPath       = NULL;
    mUnk_58     = 0;
    mPitch      = 0;
    mYaw        = 0;
    mRoll       = 0;
    SetPosition(0, 0, 0);
    SetTarget(0, 0, -0x1000);
    SetUp(0, 0x1000, 0);
    func_02026004(0x2aaa);
}

// Derives yaw, pitch, roll and distance from position, target and up.
void Camera::func_02025ec8() {
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

void Camera::func_02026004(u32 fov) {
    fov = (u16) fov;
    u16 angle = fov;
    u16 scaled = angle * 3 / 4;
    u16 h = scaled >> 1;
    int half  = h;
    mFov      = angle;
    mSin      = SinIdx(half);
    mCos      = CosIdx(half);
    mUnk_44   = FX_Div(mNear * 2, mCos);
    mUnk_44   = FX_Mul(mSin, mUnk_44);
    mUnk_40   = FX_Mul(mUnk_44 / 2, 0x1555);
    mUnk_48   = mUnk_44 * 0x69 / 192;
    mUnk_34   = FX_Div(mCos << 7, mSin) * 3 / 4;
}

void Camera::func_0202610c(void *file, u32 frameCount) {
    const u32 *path = (const u32 *) file;
    mUnk_52  = 0;
    mUnk_58 = 0;
    if (path[0] == 0) {
        if (path[1] == 0) {
            mPath   = path + 3;
            mUnk_58 = path[2];
        }
    } else {
        mPath = path;
    }
    if (frameCount == 0) {
        mUnk_50 = mPath[0];
    } else {
        mUnk_50 = frameCount;
    }
}

void Camera::func_02026164(u16 frame) {
    if (frame >= mUnk_50) {
        frame = mUnk_50 - 1;
    }
    mUnk_52 = frame;
}

void Camera::func_02026180() {
    if (mUnk_58 != 0) {
        func_02026004(mUnk_58);
    }
    fx32 keys   = INT_TO_FX(mPath[0]);
    fx32 frames = INT_TO_FX(mUnk_50);
    s64 ratio   = func_01ff8010(keys, frames);
    fx32 t      = (fx32) ((ratio * INT_TO_FX(mUnk_52) + 0x80000000LL) >> 32);
    VecFx32 v;
    func_020262cc(&v, 0, t);
    mPosition = v;
    func_020262cc(&v, 1, t);
    mTarget = v;
}

void Camera::func_020262cc(VecFx32 *out, u32 track, fx32 t) {
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

void Camera::func_020264a4(u32 loop) {
    mUnk_52++;
    if (mUnk_52 < mUnk_50) {
        return;
    }
    if (loop) {
        mUnk_52 = 0;
    } else {
        mUnk_52--;
    }
}

CameraManager::CameraManager() {
}

void CameraManager::func_020264e0() {
    mUnk_01 = 1;
    mCameras[0] = NULL;
    mCameras[1] = NULL;
    mActive = 0;
    mMode = 0;
    mDirty = 0;
}

void CameraManager::func_02026504(u32 mode, Camera *camera, u32 unk3) {
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

void CameraManager::func_0202655c() {
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
                MTX_Frustum(cam0->mUnk_44 / 2, -cam0->mUnk_44 / 2, -cam0->mUnk_40, cam0->mUnk_40, cam0->mNear, cam0->mFar,
                              0x19a, &data_02211374.mProjection);
                data_02211374.mFlags &= ~0x50;
            } else {
                MTX_Frustum(-cam1->mUnk_44 / 2 - cam1->mUnk_48, -cam1->mUnk_44 - cam1->mUnk_44 / 2 - cam1->mUnk_48,
                              -cam1->mUnk_40, cam1->mUnk_40, cam1->mNear, cam1->mFar, 0x19a, &data_02211374.mProjection);
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

void CameraManager::func_020267f0() {
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

void func_02026848() {
    MI_Copy36B(&data_0220e848, &data_02211430);
    data_02211374.mFlags &= ~0xa4;
    func_020322c0(&data_02090958);
    func_020322f0(&data_0209094c);
}
void CameraManager::func_02026890() {
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

void func_02026c4c(void *arg) {
    SpriteManager *self = (SpriteManager *) arg;
    func_020256d4();
    CameraManager::Get().func_02026890();
    self->mScreens[0].func_020275f0();
    self->mScreens[1].func_020275f0();
}

void SpriteManager::func_02026cc0() {
    DC_FlushRange(&mScreens[0], 0x400);
    DC_FlushRange(&mScreens[1], 0x400);
    CameraManager::Get().func_020267f0();
    func_020254f0(func_02026c4c, this);
}

void SpriteManager::func_02026d48() {
    mScreens[0].func_020275e8(0);
    mScreens[1].func_020275e8(1);
    for (u8 i = 0; i < 8; i++) {
        mUnk_968[i] = 0;
    }
}

void SpriteManager::func_02026d90() {
    switch (mMode) {
        case 3:
        case 5:
        case 6:
        case 7:
            GX_SetBankForSubBG(4);
            GX_SetBankForSubOBJ(8);
            break;
        case 9:
            GX_SetBankForBG(2);
            break;
    }
}

#define SET_POWCNT(extra) (reg_GX_POWCNT = (reg_GX_POWCNT & 0xfffffdf1) | (extra) | 0x200)

void SpriteManager::func_02026dec(u32 unk1, u32 mode) {
    func_020313f8((u8 *) this + 0x814, 0, func_02021e00());
    func_02027354(unk1 & 0xff);
    mMode = mode;
    switch (mode) {
        case 1:
            GX_SetBankForBG(1);
            GX_SetBankForOBJ(2);
            GX_SetBankForBGExtPltt(0x10);
            GX_SetBankForOBJExtPltt(0x20);
            GX_SetBankForSubBG(4);
            GX_SetBankForSubOBJ(8);
            GX_SetBankForSubBGExtPltt(0x80);
            GX_SetBankForSubOBJExtPltt(0x100);
            mScreens[0].func_02027624(3);
            mScreens[1].func_02027624(3);
            SET_POWCNT(2);
            break;
        case 2:
            GX_SetBankForBG(3);
            GX_SetBankForOBJ(0x10);
            GX_SetBankForBGExtPltt(0x40);
            GX_SetBankForOBJExtPltt(0x20);
            GX_SetBankForSubBG(4);
            GX_SetBankForSubOBJ(8);
            GX_SetBankForSubBGExtPltt(0x80);
            GX_SetBankForSubOBJExtPltt(0x100);
            mScreens[0].func_02027624(3);
            mScreens[1].func_02027624(3);
            SET_POWCNT(2);
            break;
        case 3:
            GX_SetBankForTex(1);
            GX_SetBankForTexPltt(0x10);
            GX_SetBankForBG(2);
            GX_SetBankForOBJ(0x20);
            GX_SetBankForBGExtPltt(0x40);
            GX_SetBankForSubBG(4);
            GX_SetBankForSubOBJ(8);
            GX_SetBankForSubBGExtPltt(0x80);
            GX_SetBankForSubOBJExtPltt(0x100);
            mScreens[0].func_02027624(3);
            mScreens[1].func_02027624(8);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(1, 0x10000);
            break;
        case 4:
            GX_SetBankForTex(1);
            GX_SetBankForTexPltt(0x10);
            GX_SetBankForOBJ(2);
            GX_SetBankForOBJExtPltt(0x20);
            GX_SetBankForSubBG(4);
            GX_SetBankForSubOBJ(8);
            GX_SetBankForSubBGExtPltt(0x80);
            GX_SetBankForSubOBJExtPltt(0x100);
            mScreens[0].func_02027624(1);
            mScreens[1].func_02027624(8);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(1, 0x10000);
            break;
        case 5:
            GX_SetBankForTex(3);
            GX_SetBankForTexPltt(0x10);
            GX_SetBankForOBJ(0x20);
            GX_SetBankForOBJExtPltt(0x40);
            GX_SetBankForSubBG(4);
            GX_SetBankForSubOBJ(8);
            GX_SetBankForSubBGExtPltt(0x80);
            GX_SetBankForSubOBJExtPltt(0x100);
            mScreens[0].func_02027624(1);
            mScreens[1].func_02027624(8);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(2, 0x10000);
            break;
        case 6:
            GX_SetBankForTex(3);
            GX_SetBankForTexPltt(0x60);
            GX_SetBankForOBJ(0x10);
            GX_SetBankForSubBG(4);
            GX_SetBankForSubOBJ(8);
            GX_SetBankForSubBGExtPltt(0x80);
            GX_SetBankForSubOBJExtPltt(0x100);
            mScreens[0].func_02027624(2);
            mScreens[1].func_02027624(8);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(2, 0x8000);
            break;
        case 7:
            GX_SetBankForTex(3);
            GX_SetBankForTexPltt(0x10);
            GX_SetBankForOBJ(0x60);
            GX_SetBankForSubBG(4);
            GX_SetBankForSubOBJ(8);
            GX_SetBankForSubBGExtPltt(0x80);
            GX_SetBankForSubOBJExtPltt(0x100);
            mScreens[0].func_02027624(1);
            mScreens[1].func_02027624(3);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(2, 0x10000);
            break;
        case 8:
            GX_SetBankForTex(3);
            GX_SetBankForTexPltt(0x40);
            GX_SetBankForBG(8);
            GX_SetBankForBGExtPltt(0x20);
            GX_SetBankForOBJ(0x10);
            GX_SetBankForSubBG(4);
            GX_SetBankForSubBGExtPltt(0x80);
            mScreens[0].func_02027624(3);
            mScreens[1].func_02027624(3);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(2, 0x4000);
            break;
        case 9:
            GX_SetBankForTex(1);
            GX_SetBankForTexPltt(0x10);
            GX_SetBankForBG(2);
            GX_SetBankForOBJ(0x20);
            GX_SetBankForBGExtPltt(0x40);
            GX_SetBankForSubBG(4);
            GX_SetBankForSubOBJ(0x100);
            GX_SetBankForSubBGExtPltt(0x80);
            mScreens[0].func_02027624(1);
            mScreens[1].func_02027624(3);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(1, 0x10000);
            break;
    }
}

void SpriteManager::func_02027354(u32 unk1) {
    if (unk1 != 1) {
        return;
    }
    GX_ResetBankForBG();
    GX_ResetBankForOBJ();
    GX_ResetBankForBGExtPltt();
    GX_ResetBankForOBJExtPltt();
    GX_ResetBankForTex();
    GX_ResetBankForTexPltt();
    GX_ResetBankForSubBG();
    func_02002dec();
    func_02002e00();
    GX_ResetBankForSubBGExtPltt();
    GX_ResetBankForSubOBJExtPltt();
    GX_ResetBankForSubOBJ();
    GX_SetBankForLCDC(0x1ff);
    MI_DmaFill32(3, (void *) 0x6800000, 0, 0xa4000);
    MIi_CpuClear16(0xc0, (void *) 0x7000000, 0x800);
    MI_DmaFill32(3, (void *) 0x5000000, 0, 0x800);
    func_02002dd8();
}

void SpriteManager::func_020273dc(u32 unk1, u32 unk2) {
    func_02035e58(unk1, 1);
    func_02036538(unk2, sBuffer_0220e900, 0x800, 1);
}

void SpriteManager::func_0202740c() {
    func_02035ee0();
    for (u8 i = 0; i < 8; i++) {
        mUnk_968[i] = 0;
    }
}

void SpriteManager::func_0202743c() {
    func_02032d54();
    G3X_InitMtxStack();
    REG_G3_DISP3DCNT = REG_G3_DISP3DCNT & 0xffffcffd;
    G3X_SetToonTable(data_02090978);
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x10;
    REG_G3_DISP3DCNT = REG_G3_DISP3DCNT & 0xcffb;
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x8;
    *(vu32 *) 0x4000580 = 0xbfff0000;
    *(vu32 *) 0x4000540 = 3;
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x20;
    G3X_SetEdgeColorTable(data_02090968);
    G3X_SetClearColor(0x7fff, 0, 0x7fff, 0x3f, 0);
    G3_Shininess(data_020909c0);
}

void SpriteManager::func_02027504(s32 index) {
    if (index >= 8) {
        return;
    }
    func_020360cc(&mUnk_828[index]);
    mUnk_968[index] = 1;
}

void SpriteManager::func_0202753c(s32 index) {
    if (mUnk_968[index] == 0) {
        return;
    }
    func_0203610c(&mUnk_828[index]);
}

void SpriteManager::func_02027568(s32 index) {
    mUnk_968[index] = 0;
}

void SpriteManager::func_02027578(s32 index, const UnkStruct_SpriteManager_28 *src) {
    if (src == NULL) {
        return;
    }
    mUnk_968[index] = 1;
    mUnk_828[index] = *src;
}
