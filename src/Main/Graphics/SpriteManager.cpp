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
extern "C" void MTX_Identity33_(MtxFx33 *mtx); // MTX_Identity33_
extern "C" void MTX_Identity43_(MtxFx43 *mtx); // MTX_Identity43_
extern "C" void MTX_Identity44_(MtxFx44 *mtx); // MTX_Identity44_


extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y);                         // atan2 (16-bit angle)
extern "C" u16 FX_AsinIdx(fx32 y);                                 // asin (16-bit angle)
extern "C" void MTX_RotY33_(MtxFx33 *mtx, fx32 sin, fx32 cos);      // MTX_RotY33_
extern "C" void MTX_RotX33_(MtxFx33 *mtx, fx32 sin, fx32 cos);      // MTX_RotX33_
extern "C" void MTX_Concat33(MtxFx33 *a, MtxFx33 *b, MtxFx33 *out);  // MTX_Concat33
extern "C" void MTX_MultVec33(VecFx32 *v, MtxFx33 *m, VecFx32 *out);  // MTX_MultVec33

extern "C" s64 FX_DivFx64c(fx32 a, fx32 b); // 64-bit fixed-point division (32 fraction bits)
extern "C" fx32 FX_Modf(fx32 value, fx32 *integer); // splits into integer part and fraction

// MTX_Perspective
extern "C" void MTX_Perspective(fx32 sin, fx32 cos, fx32 aspect, fx32 nearPlane, fx32 farPlane, fx32 scale, MtxFx44 *mtx);
// builds a perspective (frustum) matrix
extern "C" void MTX_Frustum(fx32 top, fx32 bottom, fx32 left, fx32 right, fx32 nearPlane, fx32 farPlane, fx32 scale, MtxFx44 *mtx);
extern "C" void func_01ff92a4();
extern MtxFx33 data_02211430; // G3dGlb base rotation
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
extern "C" void GX_ResetBankForLCDC();
extern "C" void func_02002dec();
extern "C" void func_02002e00();
extern "C" void GX_ResetBankForSubBGExtPltt();
extern "C" void GX_ResetBankForSubOBJExtPltt();
extern "C" void MI_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size);
s32 GetGameHeapHandle();
extern "C" void InitOSHeapAllocator(void *obj, u32 unk2, s32 heap);
extern "C" void SetupTexSlots(u32 unk1, u32 unk2);
extern "C" void SetupPlttLnkMan(u32 unk1, void *buffer, u32 size, u32 unk4);
extern "C" void ResetTexSlots();
extern "C" void SaveTexSlotRanges(UnkStruct_SpriteManager_28 *slot);
extern "C" void RestoreTexSlotRanges(UnkStruct_SpriteManager_28 *slot);
extern "C" void InitG3d();
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

void EndFrameCallback(void *arg) {
    SpriteManager *self = (SpriteManager *) arg;
    UpdateSpriteFade();
    CameraManager::Get().UpdateMode();
    self->mScreens[0].FlushOam();
    self->mScreens[1].FlushOam();
}

void SpriteManager::EndFrame() {
    DC_FlushRange(&mScreens[0], 0x400);
    DC_FlushRange(&mScreens[1], 0x400);
    CameraManager::Get().SwapBuffers();
    QueueFrameCallback(EndFrameCallback, this);
}

void SpriteManager::Init() {
    mScreens[0].SetBgIndex(0);
    mScreens[1].SetBgIndex(1);
    for (u8 i = 0; i < 8; i++) {
        mSlotUsed[i] = 0;
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

void SpriteManager::SetupMode(u32 unk1, u32 mode) {
    InitOSHeapAllocator((u8 *) this + 0x814, 0, GetGameHeapHandle());
    ResetVram(unk1 & 0xff);
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
            mScreens[0].SetObjVramMode(3);
            mScreens[1].SetObjVramMode(3);
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
            mScreens[0].SetObjVramMode(3);
            mScreens[1].SetObjVramMode(3);
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
            mScreens[0].SetObjVramMode(3);
            mScreens[1].SetObjVramMode(8);
            SET_POWCNT(0xe);
            Init3D();
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
            mScreens[0].SetObjVramMode(1);
            mScreens[1].SetObjVramMode(8);
            SET_POWCNT(0xe);
            Init3D();
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
            mScreens[0].SetObjVramMode(1);
            mScreens[1].SetObjVramMode(8);
            SET_POWCNT(0xe);
            Init3D();
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
            mScreens[0].SetObjVramMode(2);
            mScreens[1].SetObjVramMode(8);
            SET_POWCNT(0xe);
            Init3D();
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
            mScreens[0].SetObjVramMode(1);
            mScreens[1].SetObjVramMode(3);
            SET_POWCNT(0xe);
            Init3D();
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
            mScreens[0].SetObjVramMode(3);
            mScreens[1].SetObjVramMode(3);
            SET_POWCNT(0xe);
            Init3D();
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
            mScreens[0].SetObjVramMode(1);
            mScreens[1].SetObjVramMode(3);
            SET_POWCNT(0xe);
            Init3D();
            func_020273dc(1, 0x10000);
            break;
    }
}

void SpriteManager::ResetVram(u32 unk1) {
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
    GX_ResetBankForLCDC();
}

void SpriteManager::func_020273dc(u32 unk1, u32 unk2) {
    SetupTexSlots(unk1, 1);
    SetupPlttLnkMan(unk2, sBuffer_0220e900, 0x800, 1);
}

void SpriteManager::func_0202740c() {
    ResetTexSlots();
    for (u8 i = 0; i < 8; i++) {
        mSlotUsed[i] = 0;
    }
}

void SpriteManager::Init3D() {
    InitG3d();
    G3X_InitMtxStack();
    REG_G3_DISP3DCNT = REG_G3_DISP3DCNT & 0xffffcffd;
    G3X_SetToonTable(gToonTable);
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x10;
    REG_G3_DISP3DCNT = REG_G3_DISP3DCNT & 0xcffb;
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x8;
    *(vu32 *) 0x4000580 = 0xbfff0000;
    *(vu32 *) 0x4000540 = 3;
    REG_G3_DISP3DCNT = (REG_G3_DISP3DCNT & ~0x3000) | 0x20;
    G3X_SetEdgeColorTable(gEdgeColorTable);
    G3X_SetClearColor(0x7fff, 0, 0x7fff, 0x3f, 0);
    G3_Shininess(gShininessTable);
}

void SpriteManager::func_02027504(s32 index) {
    if (index >= 8) {
        return;
    }
    SaveTexSlotRanges(&mUnk_828[index]);
    mSlotUsed[index] = 1;
}

void SpriteManager::func_0202753c(s32 index) {
    if (mSlotUsed[index] == 0) {
        return;
    }
    RestoreTexSlotRanges(&mUnk_828[index]);
}

void SpriteManager::ClearSlotUsed(s32 index) {
    mSlotUsed[index] = 0;
}

void SpriteManager::SetSlot(s32 index, const UnkStruct_SpriteManager_28 *src) {
    if (src == NULL) {
        return;
    }
    mSlotUsed[index] = 1;
    mUnk_828[index] = *src;
}
