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
void func_01ff817c(MtxFx33 *mtx); // MTX_Identity33_
void func_01ff854c(MtxFx43 *mtx); // MTX_Identity43_
void func_02000d88(MtxFx44 *mtx); // MTX_Identity44_

MtxFx22 data_0220e838;
MtxFx33 data_0220e848;
MtxFx43 data_0220e86c;
MtxFx44 data_0220e89c;

u16 func_0200198c(fx32 x, fx32 y);                         // atan2 (16-bit angle)
u16 func_02001b74(fx32 y);                                 // asin (16-bit angle)
void func_01ff827c(MtxFx33 *mtx, fx32 sin, fx32 cos);      // MTX_RotY33_
void func_01ff8260(MtxFx33 *mtx, fx32 sin, fx32 cos);      // MTX_RotX33_
void func_01ff82b0(MtxFx33 *a, MtxFx33 *b, MtxFx33 *out);  // MTX_Concat33
void func_01ff84d0(VecFx32 *v, MtxFx33 *m, VecFx32 *out);  // MTX_MultVec33

s64 func_01ff8010(fx32 a, fx32 b); // 64-bit fixed-point division (32 fraction bits)
fx32 func_02001b38(fx32 value, fx32 *integer); // splits into integer part and fraction

// MTX_Perspective
void func_02001688(fx32 sin, fx32 cos, fx32 aspect, fx32 nearPlane, fx32 farPlane, fx32 scale, MtxFx44 *mtx);
// MTX_Ortho
void func_0200144c(fx32 top, fx32 bottom, fx32 left, fx32 right, fx32 nearPlane, fx32 farPlane, fx32 scale, MtxFx44 *mtx);
void func_01ff92a4();
extern MtxFx33 data_02211430; // NNS_G3dGlb base rotation
extern const VecFx32 data_0209094c;
extern const VecFx32 data_02090958;

void func_020256d4();
void func_02026c4c(void *arg);

// VRAM bank mapping and engine setup (NitroSDK GX_SetBankFor* and related).
void func_02002070(u32 banks);
void func_020025fc(u32 banks);
void func_02002300(u32 banks);
void func_02002450(u32 banks);
void func_02002550(u32 banks);
void func_020027d4(u32 banks);
void func_020028bc(u32 banks);
void func_020028dc(u32 banks);
void func_02002984(u32 banks);
void func_020029f4(u32 banks);
void func_02002a74(u32 banks);
void func_02002d18();
void func_02002d2c();
void func_02002d40();
void func_02002d64();
void func_02002d88();
void func_02002d9c();
void func_02002db0();
void func_02002dc4();
void func_02002dd8();
void func_02002dec();
void func_02002e00();
void func_02002e14();
void func_02002e3c();
void func_0200754c(u32 dmaNo, void *dest, u32 data, u32 size);
s32 func_02021e00();
void func_020313f8(void *obj, u32 unk2, s32 heap);
void func_02035e58(u32 unk1, u32 unk2);
void func_02036538(u32 unk1, void *buffer, u32 size, u32 unk4);
void func_02035ee0();
void func_020360cc(UnkStruct_SpriteManager_28 *slot);
void func_0203610c(UnkStruct_SpriteManager_28 *slot);
void func_02032d54();
void func_0200356c();
void func_0200367c(const void *unk1);
void func_02003664(const void *unk1);
void func_02003694(int rgb, int alpha, int depth, int polygonId, BOOL fog); // G3X_SetClearColor
void func_0200341c(const void *unk1);

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

fx32 func_01ff8000(fx32 a, fx32 b); // FX_Div

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
    mYaw   = func_0200198c(-dir.x, -dir.z);
    mPitch = func_02001b74(dir.y);
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
    mRoll = func_0200198c(-dir.x, dir.y);
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
    mUnk_44   = func_01ff8000(mNear * 2, mCos);
    mUnk_44   = FX_Mul(mSin, mUnk_44);
    mUnk_40   = FX_Mul(mUnk_44 / 2, 0x1555);
    mUnk_48   = mUnk_44 * 0x69 / 192;
    mUnk_34   = func_01ff8000(mCos << 7, mSin) * 3 / 4;
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

// non-matching: size 0x1c8 vs 0x1d8: the original indexes the path without a post-indexed load.
void Camera::func_020262cc(VecFx32 *out, u32 track, fx32 t) {
    u32 n               = mPath[0];
    const VecFx32 *keys = (const VecFx32 *) (mPath + 1) + n * track;
    u32 index           = (u32) (t << 4) >> 16;
    if (n <= index) {
        *out = keys[n - 1];
        return;
    }
    u16 next = index + 1;
    if (n <= next) {
        *out = keys[n - 1];
        return;
    }
    fx32 whole;
    fx32 frac = func_02001b38(t, &whole);
    if (frac == 0) {
        *out = keys[index];
        return;
    }
    fx32 w        = 0x1000 - frac;
    VecFx32 a     = keys[index];
    VecFx32 b     = keys[next];
    out->z = func_01ff8000(FX_Mul(a.z, w) + FX_Mul(b.z, frac), 0x1000);
    out->y = func_01ff8000(FX_Mul(a.y, w) + FX_Mul(b.y, frac), 0x1000);
    out->x = func_01ff8000(FX_Mul(a.x, w) + FX_Mul(b.x, frac), 0x1000);
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
                func_0200144c(cam0->mUnk_44 / 2, -cam0->mUnk_44 / 2, -cam0->mUnk_40, cam0->mUnk_40, cam0->mNear, cam0->mFar,
                              0x19a, &data_02211374.mProjection);
                data_02211374.mFlags &= ~0x50;
            } else {
                func_0200144c(-cam1->mUnk_44 / 2 - cam1->mUnk_48, -cam1->mUnk_44 - cam1->mUnk_44 / 2 - cam1->mUnk_48,
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
// non-matching: 3 words: scheduling in the OAM-fill loop.
void CameraManager::func_02026890() {
    if (mDirty != 0) {
        mMode  = mRequestedMode;
        mDirty = 0;
        SpriteManager::Get().func_02026d90();
        switch (mMode) {
            case 0:
                break;
            case 1:
                func_02001e68(1, 5, 1);
                break;
            case 2:
            case 3:
                func_02001e68(1, 5, 1);
                func_02001ed0(5);
                G2S_SetBG2Control(1, 1, 0, 1, 0);
                reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & ~0x60) | 0x20;
                break;
            case 4:
                func_02001e68(1, 5, 1);
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
                func_020028bc(8);
                func_020028dc(4);
                reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & ~0x1f00) | 0x400;
                *(vu32 *) 0x4000064 = 0x80330010;
                mActive = 1;
            } else {
                reg_GX_POWCNT = reg_GX_POWCNT & ~0x8000;
                func_02002dec();
                func_020028bc(4);
                func_02002984(8);
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
                    e->mAttr = ((row << 6) & 0xff) | 0xc0000c00 | (((col << 6) & 0x1ff) << 16);
                    e->mPos  = (col << 3) + (row << 8) | 0xf000;
                }
            }
            return;
        case 4:
            if (mActive == 0) {
                func_02002d18();
                func_020028bc(8);
                func_02002070(2);
                reg_G2_BG0CNT = (reg_G2_BG0CNT & ~3) | 2;
                reg_G2_BG2CNT = (reg_G2_BG2CNT & ~3) | 3;
                *(vu32 *) 0x4000064 = 0x81330010;
                mActive = 1;
                return;
            } else {
                func_02002d18();
                func_020028bc(2);
                func_02002070(8);
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
    func_02005e9c(&mScreens[0], 0x400);
    func_02005e9c(&mScreens[1], 0x400);
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
            func_020028dc(4);
            func_02002984(8);
            break;
        case 9:
            func_02002070(2);
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
            func_02002070(1);
            func_02002300(2);
            func_02002450(0x10);
            func_02002550(0x20);
            func_020028dc(4);
            func_02002984(8);
            func_020029f4(0x80);
            func_02002a74(0x100);
            mScreens[0].func_02027624(3);
            mScreens[1].func_02027624(3);
            SET_POWCNT(2);
            break;
        case 2:
            func_02002070(3);
            func_02002300(0x10);
            func_02002450(0x40);
            func_02002550(0x20);
            func_020028dc(4);
            func_02002984(8);
            func_020029f4(0x80);
            func_02002a74(0x100);
            mScreens[0].func_02027624(3);
            mScreens[1].func_02027624(3);
            SET_POWCNT(2);
            break;
        case 3:
            func_020025fc(1);
            func_020027d4(0x10);
            func_02002070(2);
            func_02002300(0x20);
            func_02002450(0x40);
            func_020028dc(4);
            func_02002984(8);
            func_020029f4(0x80);
            func_02002a74(0x100);
            mScreens[0].func_02027624(3);
            mScreens[1].func_02027624(8);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(1, 0x10000);
            break;
        case 4:
            func_020025fc(1);
            func_020027d4(0x10);
            func_02002300(2);
            func_02002550(0x20);
            func_020028dc(4);
            func_02002984(8);
            func_020029f4(0x80);
            func_02002a74(0x100);
            mScreens[0].func_02027624(1);
            mScreens[1].func_02027624(8);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(1, 0x10000);
            break;
        case 5:
            func_020025fc(3);
            func_020027d4(0x10);
            func_02002300(0x20);
            func_02002550(0x40);
            func_020028dc(4);
            func_02002984(8);
            func_020029f4(0x80);
            func_02002a74(0x100);
            mScreens[0].func_02027624(1);
            mScreens[1].func_02027624(8);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(2, 0x10000);
            break;
        case 6:
            func_020025fc(3);
            func_020027d4(0x60);
            func_02002300(0x10);
            func_020028dc(4);
            func_02002984(8);
            func_020029f4(0x80);
            func_02002a74(0x100);
            mScreens[0].func_02027624(2);
            mScreens[1].func_02027624(8);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(2, 0x8000);
            break;
        case 7:
            func_020025fc(3);
            func_020027d4(0x10);
            func_02002300(0x60);
            func_020028dc(4);
            func_02002984(8);
            func_020029f4(0x80);
            func_02002a74(0x100);
            mScreens[0].func_02027624(1);
            mScreens[1].func_02027624(3);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(2, 0x10000);
            break;
        case 8:
            func_020025fc(3);
            func_020027d4(0x40);
            func_02002070(8);
            func_02002450(0x20);
            func_02002300(0x10);
            func_020028dc(4);
            func_020029f4(0x80);
            mScreens[0].func_02027624(3);
            mScreens[1].func_02027624(3);
            SET_POWCNT(0xe);
            func_0202743c();
            func_020273dc(2, 0x4000);
            break;
        case 9:
            func_020025fc(1);
            func_020027d4(0x10);
            func_02002070(2);
            func_02002300(0x20);
            func_02002450(0x40);
            func_020028dc(4);
            func_02002984(0x100);
            func_020029f4(0x80);
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
    func_02002d18();
    func_02002d2c();
    func_02002d40();
    func_02002d64();
    func_02002d88();
    func_02002d9c();
    func_02002db0();
    func_02002dec();
    func_02002e00();
    func_02002e14();
    func_02002e3c();
    func_02002dc4();
    func_020028bc(0x1ff);
    func_0200754c(3, (void *) 0x6800000, 0, 0xa4000);
    MIi_CpuClear16(0xc0, (void *) 0x7000000, 0x800);
    func_0200754c(3, (void *) 0x5000000, 0, 0x800);
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
    func_0200356c();
    REG_G3_DISP3DCNT = REG_G3_DISP3DCNT & 0xffffcffd;
    func_0200367c(data_02090978);
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x10;
    REG_G3_DISP3DCNT = REG_G3_DISP3DCNT & 0xcffb;
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x8;
    *(vu32 *) 0x4000580 = 0xbfff0000;
    *(vu32 *) 0x4000540 = 3;
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x20;
    func_02003664(data_02090968);
    func_02003694(0x7fff, 0, 0x7fff, 0x3f, 0);
    func_0200341c(data_020909c0);
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
