#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>
#include <nitro/mi.h>

#include "System/FileSystem.hpp"

// NNS G3d resource layouts used by the model helper classes (library data, declared by layout only).

// A texture block (NNSG3dResTex, "TEX0"): three info blocks for textures, 4x4 compressed textures and palettes.
struct NNSG3dResTex {
    /* 00 */ u32 mMagic;
    /* 04 */ u32 mSize;
    /* 08 */ u32 mTexHandle;
    /* 0c */ u16 mTexBlocks; // in units of 8 bytes
    /* 0e */ u16 mUnk_0e;
    /* 10 */ u16 mTexState;
    /* 12 */ u16 mUnk_12;
    /* 14 */ u32 mTexData;
    /* 18 */ u32 m4x4Handle;
    /* 1c */ u16 m4x4Blocks;
    /* 1e */ u16 mUnk_1e;
    /* 20 */ u16 m4x4State;
    /* 22 */ u16 mUnk_22;
    /* 24 */ u32 m4x4Data;
    /* 28 */ u32 m4x4IndexData;
    /* 2c */ u32 mPlttHandle;
    /* 30 */ u16 mPlttBlocks;
    /* 32 */ u16 mPlttState;
    /* 34 */ u16 mPlttNames;
    /* 36 */ u16 mUnk_36;
    /* 38 */ u32 mPlttData;
    /* 3c */
};

// Texture VRAM allocator hooks (NNS gfd function pointers, defaults at 02035df0..02035e08).
extern u32 (*data_020a5880)(u32 size, int is4x4, u32 opt);
extern int (*data_020a5884)(u32 key);
extern u32 (*data_020a5888)(u32 size, int is4pltt, u32 opt);
extern int (*data_020a588c)(u32 key);

extern "C" void func_02005e9c(void *buffer, u32 size);
extern "C" void func_01ff9510(const void *src, void *dst, u32 size);

// A resource dictionary (NNSG3dResDict) as embedded in the model set block.
struct NNSG3dResDict {
    /* 00 */ u8 mUnk_00;
    /* 01 */ u8 mCount;
    /* 02 */ u16 mUnk_02;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mTableOfs;
};

// Model set block (NNSG3dResMdlSet, "MDL0").
struct NNSG3dResMdlSet {
    /* 00 */ u32 mMagic;
    /* 04 */ u32 mSize;
    /* 08 */ NNSG3dResDict mDict;
};

extern "C" NNSG3dResMdlSet *func_02033598(void *file);
extern "C" void func_0203209c(NNSG3dResMdlSet *set);
extern "C" void func_02032fb4(void *file);
extern "C" void func_02032d7c(void *file);
extern "C" void func_02031fec(NNSG3dResMdlSet *set, NNSG3dResTex *tex);

// Animation resource set (the dictionary count is at +9) and a running animation object.
struct NNSG3dResAnmSet {
    /* 00 */ u8 mUnk_00[9];
    /* 09 */ u8 mCount;
};
struct NNSG3dResAnm {
    /* 00 */ u32 mUnk_00;
    /* 04 */ u16 mFrames;
};
struct NNSG3dAnmObj {
    /* 00 */ s32 mFrame;
    /* 04 */ u32 mUnk_04;
    /* 08 */ NNSG3dResAnm *mRes;
};

extern "C" NNSG3dResAnmSet *func_02033810(void *file);
extern "C" NNSG3dResAnmSet *func_020336a0(void *file);
extern "C" NNSG3dResAnmSet *func_020337f4(void *file);
extern "C" NNSG3dResAnmSet *func_020337bc(void *file);
extern "C" NNSG3dResAnmSet *func_020337d8(void *file);
extern "C" void func_02031518(NNSG3dAnmObj *obj, u32 id);
extern "C" void *func_020335e8(void *file, u32 index);
extern "C" u32 func_02031418(void *anm, void *model);
extern "C" void func_02031478(void *mem, void *anm, void *model, void *tex);

struct NNSG3dRenderObj {
    /* 00 */ u8 mUnk_00[0x2c];
    /* 2c */ void *mUserData;
};

// Render state passed to render callbacks.
struct NNSG3dRS {
    /* 00 */ u32 mUnk_00;
    /* 04 */ NNSG3dRenderObj *mRenderObj;
    /* 08 */ u32 mFlags;
    /* 0c */ u8 mUnk_0c[0x24 - 0x0c];
    /* 24 */ void (*mCallback)(NNSG3dRS *);
    /* 28 */ u8 mUnk_28[0x92 - 0x28];
    /* 92 */ u8 mUnk_92;
    /* 93 */ u8 mUnk_93[0xae - 0x93];
    /* ae */ u8 mUnk_ae;
};

extern "C" void func_02031554(void *renderObj, void *model);
extern "C" void func_020316c8(void *renderObj, NNSG3dAnmObj *obj);
extern "C" void func_020317bc(void *renderObj, NNSG3dAnmObj *obj);
extern "C" void func_02031810(void *renderObj, void (*callback)(NNSG3dRS *), u32 unk2, u32 unk3, u32 unk4);
extern "C" void func_02031824(void *renderObj);
extern "C" void func_02032cc8(MtxFx43 *mtx, void *unk2);
extern "C" void func_01ff92a4();
extern "C" BOOL G3X_GetBoxTestResult(s32 *result);
extern "C" void func_020322c0(const VecFx32 *trans); // NNS_G3dGlbSetBaseTrans
extern "C" void func_020322f0(const VecFx32 *scale); // NNS_G3dGlbSetBaseScale
extern "C" void func_02032284();
extern "C" void func_01ffb0ec(void *renderObj);

extern "C" void func_01ff90dc(s16 *in, s16 *out);
extern "C" void func_0203235c(u32 index, u32 color);
extern "C" void func_02032320(u32 index, s32 x, s32 y, s32 z);

extern "C" void func_01ffd0e0(MtxFx43 *mtx, u16 angle);
extern "C" void func_01ffd088(MtxFx43 *mtx, u16 angle);
extern "C" void func_01ffd138(MtxFx43 *mtx, u16 angle);
extern "C" void func_01ffcf64(MtxFx43 *mtx, VecFx32 *pos);
