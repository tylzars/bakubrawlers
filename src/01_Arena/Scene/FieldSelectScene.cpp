#include "Scene/FieldSelectScene.hpp"
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"
#include "Unknown/UnkStruct_020c3030.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// non-matching: same size and same structure, 102 words differ (register allocation and the scheduling of the mUnk_33b loads in the first flag loop)
extern const u8 data_ov001_02227194[];
extern const u8 data_ov001_022270c4[];
FieldSelectScene::FieldSelectScene(u8 unk1) : SceneBase(1000, 0) {
    mWindows[0] = NULL;
    mWindows[1] = NULL;
    mWindows[2] = NULL;
    mWindows[3] = NULL;
    mMessage = NULL;
    mPanel[0] = NULL;
    mPanel[1] = NULL;
    mUnk_2f4 = NULL;
    mUnk_2f8 = NULL;
    mUnk_2fc = NULL;
    mUnk_300 = 0;
    mUnk_304 = 0;
    mUnk_308 = NULL;
    mUnk_31c = NULL;
    mUnk_320 = 0;
    mUnk_324 = 0;
    mState = 0;
    mUnk_329 = 0;
    mUnk_32c = 0;
    mUnk_32d = 0;
    mUnk_32e = 0x10;
    mUnk_32f = 0x10;
    mUnk_330 = 1;
    mUnk_332 = 2;
    mUnk_333 = 0;
    mUnk_334 = 0;
    mUnk_335 = 0;
    mUnk_336 = 0;
    mUnk_337 = 0;
    mUnk_338 = 0;
    mUnk_339 = 0;
    mUnk_33a = unk1;
    mUnk_33b = 0;
    mUnk_33d = 0;
    mUnk_33e = 2;
    mUnk_33f = 0;
    mUnk_340 = 0;
    mUnk_341 = 0;
    mUnk_344 = -1;
    mUnk_348 = 1;
    data_020d433c.mUnk_10 = NULL;
    MI_CpuFill8(mUnk_10c, 0, sizeof(mUnk_10c));
    MI_CpuClearFast(mUnk_14c, sizeof(mUnk_14c));
    MI_CpuFill8(&mMain, 0, sizeof(mMain));
    MI_CpuFill8(mCells, 0, sizeof(mCells));
    MI_CpuFill8(mCells2, 0, sizeof(mCells2));
    MI_CpuClear32(mFiles, sizeof(mFiles));
    MI_CpuClear32(mFileSizes, sizeof(mFileSizes));
    MI_CpuClear32(mUnk_30c, sizeof(mUnk_30c));
    MI_CpuFill8(mUnk_2d2, 0, sizeof(mUnk_2d2));
    MI_CpuFill8(mUnk_2e4, 0, sizeof(mUnk_2e4));
    UnkStruct_020c3030::Get();
    const u8 *mode = &data_020c4258;
    if (IsWirelessMode(mode)) {
        mUnk_32a = mUnk_32b = UnkStruct_020c3030::Get().func_0202f134();
    } else {
        mUnk_32b = 1;
        mUnk_32a = 2;
    }
    mUnk_333 = 1;
    UnkStruct_020c3030::Get();
    if (IsWirelessMode(mode)) {
        mUnk_33b = data_020d433c.mUnk_ae;
        if (mUnk_33b != 0) {
            mUnk_333 = 0;
        }
    } else {
        mUnk_33b = 0;
    }
    for (u8 i = 0; i < 0x12; i++) {
        if (i == 0 || i == 0x11) {
            mUnk_266[mUnk_33b][i] = 3;
            mUnk_2d2[i]           = 0;
        } else {
            u8 f                  = data_ov001_02227194[i];
            mUnk_266[mUnk_33b][i] = 0;
            u32 byte = (u32) f >> 3;
            int bit  = f % 8;
            for (u8 k = 0; k < 3; k++) {
                if (data_020d496a[k * 3 + byte] & (1 << bit)) {
                    mUnk_266[mUnk_33b][i] = k + 1;
                }
            }
            if (mUnk_266[mUnk_33b][i] != 0) {
                if (!(data_020d4973[byte] & (1 << bit))) {
                    mUnk_2d2[i] = 1;
                }
            }
        }
    }
    for (u8 j = 0; j < 9; j++) {
        if (j == 8) {
            mUnk_2ae[mUnk_33b][j] = 1;
        } else {
            u8 f = data_ov001_022270c4[j];
            if (TestFlagBit(data_020d4968, f)) {
                mUnk_2ae[mUnk_33b][j] = 1;
                if (!TestFlagBit(data_020d4969, f)) {
                    mUnk_2e4[j] = 1;
                }
            } else {
                mUnk_2ae[mUnk_33b][j] = 0;
            }
        }
    }
    u8 n = 0;
    for (u8 i = 0; i < 5; i++) {
        for (u8 j = 0; j < 2; j++) {
            mUnk_25c[i * 2 + j] = 0;
        }
        mUnk_25c[i * 2 + 1] = 2;
    }
    mUnk_25c[1] = 3;
    if (mUnk_33a == 0) {
        UnkStruct_020c3030::Get();
        if (IsWirelessMode(mode)) {
            mUnk_2ed[0] = 5;
            n++;
        }
        mUnk_329 = n;
        if (mUnk_32a < 4) {
            mUnk_2ed[n] = 0;
            n++;
        }
        u8 n1 = n + 1;
        u8 n2 = n1 + 1;
        u8 n3 = n2 + 1;
        u8 n4 = n3 + 1;
        mUnk_2ed[n]  = 2;
        mUnk_2ed[n1] = 3;
        mUnk_2ed[n2] = 4;
        mUnk_2ed[n3] = 1;
        mUnk_2ed[n4] = 6;
    }
    data_020d433c.mUnk_99 = 3;
    data_020d433c.mUnk_a7 = 0;
    data_020d433c.mUnk_9a = data_020d43f0[0].mUnk_27;
    mUnk_25c[0]           = data_020d43f0[0].mUnk_27_5;
    mUnk_25c[2] = data_020d433c.mUnk_9a_3 == 0;
    mUnk_25c[4] = data_020d433c.mUnk_9a_2 == 0;
    mUnk_25c[6] = data_020d433c.mUnk_9a_4 == 0;
    mUnk_25c[8] = data_020d433c.mUnk_9a_0 == 0;
    data_020d433c.mUnk_96 = mUnk_25c[0];
}

FieldSelectScene::~FieldSelectScene() {
    if (mMain.mAnim != NULL) {
        delete mMain.mAnim;
    }
    for (u8 i = 0; i < 9; i++) {
        if (mCells[i].mAnim != NULL) {
            delete mCells[i].mAnim;
        }
    }
    CellAnimation *anim;
    u8 i;
    for (i = 0; i < 9; i++) {
        anim = mCells2[i].mAnim;
        if (anim != NULL) {
            delete anim;
        }
    }
    if (mUnk_2f4 != NULL) {
        OS_Free(mUnk_2f4);
    }
    if (mUnk_2fc != NULL) {
        OS_Free(mUnk_2fc);
    }
    if (mUnk_2f8 != NULL) {
        OS_Free(mUnk_2f8);
    }
    if (mUnk_308 != NULL) {
        OS_Free(mUnk_308);
    }
    if (mUnk_31c != NULL) {
        OS_Free(mUnk_31c);
    }
    for (u8 i = 0; i < 4; i++) {
        if (mUnk_30c[i] != NULL) {
            OS_Free(mUnk_30c[i]);
        }
    }
    if (mWindows[0] != NULL) {
        delete mWindows[0];
    }
    if (mWindows[1] != NULL) {
        delete mWindows[1];
    }
    if (mWindows[2] != NULL) {
        delete mWindows[2];
    }
    if (mWindows[3] != NULL) {
        delete mWindows[3];
    }
    if (mMessage != NULL) {
        delete mMessage;
    }
    UnkStruct_020651bc *panel0 = mPanel[0];
    if (panel0 != NULL) {
        panel0->~UnkStruct_020651bc();
        operator delete(panel0);
    }
    UnkStruct_020651bc *panel1 = mPanel[1];
    if (panel1 != NULL) {
        panel1->~UnkStruct_020651bc();
        operator delete(panel1);
    }
    for (u8 i = 0; i < 10; i++) {
        for (u8 j = 0; j < 3; j++) {
            if (mFiles[i][j] != NULL) {
                OS_Free(mFiles[i][j]);
            }
        }
    }
    if (data_020d433c.mUnk_a8 != 0) {
        data_020d433c.mUnk_10->mUnk_04 = 1;
    }
}

// non-matching: 16 words, the base register for the MATH_Rand32 update of data_020d42e8 differs
void FieldSelectScene::Update() {
    func_ov001_02222040();
    switch (mState) {
        case 0:
            func_ov001_0221f23c();
            func_ov001_0221f4e8();
            func_ov001_022260b8();
            func_ov001_0222659c();
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 1:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            UnkStruct_020c3030::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                mState++;
            } else {
                MI_CpuCopy8(data_020d4404, mUnk_14c, 8);
                mState = 4;
            }
            break;
        case 2:
            if (func_ov001_02221c0c()) {
                mState++;
            }
            break;
        case 3:
            if (func_ov001_02221f1c()) {
                mState++;
            }
            break;
        case 4:
            MATHRandContext32 *ctx = &data_020d42e8;
            ctx->x                 = ctx->mul * ctx->x + ctx->add;
            data_0220f540.mUnk_04  = (u32) (ctx->x >> 32);
            if (func_ov001_02221aa8(2)) {
                u8 *p = UnkStruct_020c3030::Get().func_0202f0cc(0);
                if (p != NULL) {
                    func_020219ec(*(u32 *) (p + 4));
                }
                mState++;
            }
            break;
        case 5: {
            u32 r = func_ov001_02222e44();
            if (r == 0) {
                break;
            }
            if (r == 3) {
                mState++;
            } else {
                if (r == 1) {
                    func_ov001_02226cb0();
                    func_ov001_02226d74();
                }
                mState = 8;
            }
            break;
        }
        case 6:
            UnkStruct_020c3030::Get().func_0202edd0();
            mState++;
            break;
        case 7:
            UnkStruct_020c3030::Get();
            if (data_020c4030.mUnk_228 == 0) {
                mState++;
            }
            break;
        case 8:
            if (mUnk_344 != -1) {
                G2S_SetBG1Priority(0);
                GXS_SetVisiblePlane(mUnk_344);
                mUnk_344 = -1;
            }
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 9:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 10:
            RequestScene(mUnk_332);
            mFinished = 1;
            break;
    }
    func_ov001_022256e4();
}

void FieldSelectScene::func_ov001_0221f23c() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 1);
    G2_SetBG2Control(0, 1, 4, 7, 0);
    G2_SetBG3Control(0, 1, 2, 4, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 1);
    G2S_SetBG2Control(0, 1, 4, 7, 0);
    G2S_SetBG3Control(0, 1, 2, 4, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(0);
    G2S_SetBG3Priority(3);
    GX_SetVisiblePlane(0x1a);
    GXS_SetVisiblePlane(0x1a);
    G2_SetBlendBrightness(0x1b, 0);
    G2S_SetBlendBrightness(0x1b, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
    G2_SetWndOutsidePlane(0x3f, FALSE);
    GXS_SetVisibleWnd(1);
    G2S_SetWndOutsidePlane(0x3f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0xff, 0x12);
}

// non-matching: 4 words, zero kept in a callee-saved register across func_02025b64 calls (see docs/decompiling.md)
void FieldSelectScene::func_ov001_0221f4e8() {
    ChangeDir("/Game/Battle/Bg/");
    func_02025b64(0, 1, 0x6000, "select_arena_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 12, 0, "select_arena_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 8, 0, "select_arena_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x6000, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 1, 0x4000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 13, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 9, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.LoadCharacters("select_arena_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("select_arena_u_obj_00_ncl.bin", 0, 0);
    if (mUnk_2ed[mUnk_329] == 0) {
        mMain.mAnim = new CellAnimation("select_arena_u_obj_00_nce.bin", 6, 1, 1);
    } else {
        mMain.mAnim = new CellAnimation("select_arena_u_obj_00_nce.bin", 3, 1, 1);
    }
    mMain.mAnim->mCharBase = main.func_020276f4(0);
    mMain.mAnim->mUnk_11   = 0;
    mMain.mX               = 0x80;
    mMain.mY               = 0x60;
    mMain.mUnk_09          = 0;
    mMain.mVisible         = 1;
    ChangeDir("/Game/2D/union_b/");
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.func_02027624(3);
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("union_b_obj_cockpit_ncl.bin", 0, 0);
    ChangeDir("/Game/Battle/Bg/");
    char name[0x80];
    for (u8 i = 0; i < 10; i++) {
        sprintf(name, "select_field_u_sub_%02d_ncl.bin", i);
        mFiles[i][1] = func_02023504(name, 0, 0, 0, (u32) &mFileSizes[i][1], 0);
        sprintf(name, "select_field_u_sub_%02d_ncg.bin", i);
        mFiles[i][0] = func_02023504(name, 0, 0, 0, (u32) &mFileSizes[i][0], 0);
        sprintf(name, "select_field_u_sub_%02d_nsc.bin", i);
        mFiles[i][2] = func_02023504(name, 0, 0, 0, (u32) &mFileSizes[i][2], 0);
    }
    ChangeDir("/");
    mMessage = new MessageWindow("mes_BattleArenaMenu", 1, 0x7000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mUnk_33e = 2;
    if (mUnk_333 == 0) {
        mUnk_33e--;
    }
    mPanel[0] = new UnkStruct_020651bc(0x12000, 0xb, 0);
    mPanel[1] = new UnkStruct_020651bc(0x1f000, 0xb, 1);
    if (data_020d433c.mUnk_a8 == 0) {
        return;
    }
    if (data_020d433c.mUnk_10 != NULL) {
        data_020d433c.mUnk_10->func_020785d8(2);
    } else {
        data_020d433c.mUnk_10 = new UnkStruct_02078290(2);
    }
}
