#include "Scene/FieldSelectScene.hpp"
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "System/TouchManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"
#include "System/FrameTask.hpp"
#include "Unknown/UnkStruct_020c3030.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// non-matching: same size and same structure, 102 words differ (register allocation and the scheduling of the mUnk_33b loads in the first flag loop)
extern const u8 data_ov001_02227194[];
extern const u8 data_ov001_022270c4[];

struct UnkStruct_020c4230 {
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u16 mUnk_20;
};
extern UnkStruct_020c4230 data_020c4230;
extern u8 data_020d43e3[];
extern "C" void func_01ff96c4(const void *src, void *dest, u32 size);

#define NET_KEYS (*(u16 *) ((u8 *) &data_0220f540 + 4))
#define NET_BYTE(n) (*((u8 *) &data_0220f540 + (n)))

extern const WindowItemRecord data_ov001_022271a8[][2];
extern const WindowCellRecord data_ov001_0222715c[];
extern const WindowItemRecord data_ov001_022270ec[];
extern const WindowCellRecord data_ov001_02227124[];
extern const WindowItemRecord data_ov001_02227108[];
extern const WindowCellRecord data_ov001_02227140[];

extern const u8 data_ov001_022270dd[];
extern const u8 data_ov001_022270de[];
extern const u8 data_ov001_022270df[];

static inline u8 IsZeroU8(u8 v) {
    return v == 0;
}

extern u8 data_020d43d7[];

static inline void G2_SetBlendAlpha(int plane1, int plane2, int ev1, int ev2) {
    G2x_SetBlendAlpha_(&reg_G2_BLDCNT, plane1, plane2, ev1, ev2);
}

extern const u8 data_ov001_022270d0[];

extern const WindowItemRecord data_ov001_02227218[][6];
extern const WindowCellRecord data_ov001_02227560[][6];

extern const u8 data_ov001_022270dc[];
extern const u8 data_ov001_022270c0[];
extern const u8 data_ov001_022270bc[];

extern u8 data_020d43eb[];

// non-matching: register allocation and scheduling of the mUnk_33b loads in the first flag loop.
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
// non-matching: the original does not predicate the single-store branches of `j == 0 || j == 0x11` and `j == 8`.
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

// non-matching: 2053 words each side; zero/constant kept in callee-saved registers, the SpriteScreen pointer in r9 in some loops.
void FieldSelectScene::func_ov001_0221fa94() {
    MI_CpuClearFast((void *) 0x6004000, 0xc000);
    MI_CpuClearFast((void *) 0x6204000, 0xc000);
    if (mWindows[0] != NULL) {
        delete mWindows[0];
    }
    u32 variant;
    switch (mUnk_2ed[mUnk_329]) {
        case 0:
        case 1:
        case 3:
        case 4:
            variant = 1;
            break;
        default:
            variant = 0;
            break;
    }
    mWindows[0] = new SelectWindow(mUnk_33e, data_ov001_022271a8[variant], 0);
    mWindows[0]->SetCells(mUnk_33e, data_ov001_0222715c);
    mWindows[0]->mCursor->mSelection = -1;
    mWindows[0]->mUnk_064            = 0;
    if (mUnk_2ed[mUnk_329] == 2) {
        mWindows[0]->mButtons[0].mEnabled = 0;
    }
    mUnk_334 = 0;
    mUnk_335 = 0;
    mUnk_336 = 0;
    mUnk_337 = 0;
    mUnk_338 = 0;
    mUnk_339 = 0;
    if (mWindows[1] != NULL) {
        delete mWindows[1];
    }
    mWindows[1] = NULL;
    if (mWindows[2] != NULL) {
        delete mWindows[2];
    }
    mWindows[2] = NULL;
    if (mWindows[3] != NULL) {
        delete mWindows[3];
    }
    mWindows[3] = NULL;
    mUnk_33d = 0;
    for (u8 i = 0; i < 4; i++) {
        mInput[i].mUnk_0a = 0;
    }
    mUnk_33f = mUnk_33b;
    mUnk_340 = 0;
    mUnk_331 = 0;
    for (u8 i = 0; i < 9; i++) {
        CellAnimation *anim = mCells[i].mAnim;
        if (anim != NULL) {
            delete anim;
        }
        mCells[i].mAnim    = NULL;
        mCells[i].mUnk_0a  = 1;
        mCells[i].mVisible = 0;
        mCells[i].mUnk_09  = 1;
    }
    for (u8 i = 0; i < 9; i++) {
        CellAnimation *anim = mCells2[i].mAnim;
        if (anim != NULL) {
            delete anim;
        }
        mCells2[i].mAnim    = NULL;
        mCells2[i].mUnk_0a  = 1;
        mCells2[i].mVisible = 0;
        mCells2[i].mUnk_09  = 1;
    }
    switch (mUnk_2ed[mUnk_329]) {
        case 0: {
            if (mUnk_333 != 0) {
                mMessage->func_02065890(0x13, 0, 1);
            } else {
                mMessage->func_02065890(4, 1, 1);
            }
            mMain.mAnim->SetAnimation(6);
            if (mUnk_32b > 2) {
                mInput[0].mUnk_02 = 1;
            }
            SpriteScreen &sub = SpriteManager::Get().Screen(1);
            ChangeDir("/Game/Character/Icon/");
            sub.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0x13000, 0);
            sub.LoadPalette("icon_ch_base_ncl.bin", 0x40, 0);
            mCells2[7].mAnim            = new CellAnimation("icon_ch_base_nce.bin", 0xb, 1, 1);
            mCells2[7].mAnim->mCharBase = sub.func_020276f4(0x13000);
            mCells2[7].mAnim->mUnk_11   = 2;
            mCells2[7].mVisible         = 0;
            ChangeDir("/Game/Battle/Obj/");
            sub.LoadCharacters("select_number_b_obj_00.nce_ncg1D.bin", 0x8000, 0);
            sub.LoadPalette("select_number_b_obj_00_ncl.bin", 0x100, 0);
            s16 y = 0x30;
            u8 n  = 0;
            if (mUnk_32b > 2) {
                y -= 0x10;
                n++;
            }
            do {
                mCells2[n].mAnim            = new CellAnimation("select_number_b_obj_00_nce.bin", n & 0xff, 1, 1);
                mCells2[n].mAnim->mCharBase = sub.func_020276f4(0x8000);
                mCells2[n].mAnim->mUnk_11   = 8;
                mCells2[n].mX               = 0x20;
                mCells2[n].mY               = n * 0x24 + y;
                mCells2[n].mUnk_09          = 3;
                mCells2[n].mVisible         = 1;
                n++;
            } while (n < 3);
            mCells2[mInput[0].mUnk_02].mAnim->SetAnimation(mInput[0].mUnk_02 + 3);
            mUnk_339 = 1;
            mUnk_331 = 0x10;
            break;
        }
        case 1: {
            if (mUnk_333 != 0) {
                mMessage->func_02065890(5, 0, 1);
            } else {
                mMessage->func_02065890(4, 1, 1);
            }
            mMain.mAnim->SetAnimation(2);
            ChangeDir("/Game/Battle/Bg/");
            if (mUnk_333 != 0) {
                func_02025b64(1, 1, 0, "select_rule_b_bg_00_ncl.bin", 0, 0, 0);
                func_02025b64(1, 11, 0, "select_rule_b_bg_00_ncg.bin", 0, 0, 0);
                func_02025b64(1, 7, 0, "select_rule_b_bg_00_nsc.bin", 0, 0, 0);
                ChangeDir("/Game/Battle/Obj/");
                SpriteScreen &sub = SpriteManager::Get().Screen(1);
                sub.LoadCharacters("select_rule_b_obj_00.nce_ncg1D.bin", 0x8000, 0);
                sub.LoadPalette("select_rule_b_obj_00_ncl.bin", 0x100, 0);
                u8 k = 0;
                do {
                    mCells2[k].mAnim            = new CellAnimation("select_rule_b_obj_00_nce.bin", 0, 1, 1);
                    mCells2[k].mAnim->mCharBase = sub.func_020276f4(0x8000);
                    mCells2[k].mAnim->mUnk_11   = 8;
                    mCells2[k].mX               = 0x88;
                    mCells2[k].mY               = (k % 5) * 0x18 + 0x26;
                    mCells2[k].mUnk_09          = 2;
                    mCells2[k].mVisible         = 1;
                    k++;
                } while (k < 8);
                mCells2[5].mX = 0x79;
                mCells2[5].mAnim->SetAnimation(0);
                mCells2[6].mX = 0xe4;
                mCells2[6].mAnim->SetAnimation(3);
                mCells2[0].mAnim->SetAnimation(7);
                mCells2[0].mUnk_0a = 0;
                mCells2[1].mAnim->SetAnimation(8);
                mCells2[1].mUnk_0a = 0;
                mCells2[2].mAnim->SetAnimation(9);
                mCells2[2].mUnk_0a = 0;
                mCells2[3].mAnim->SetAnimation(10);
                mCells2[3].mUnk_0a = 0;
                mCells2[4].mAnim->SetAnimation(11);
                mCells2[4].mUnk_0a = 0;
                mCells2[7].mAnim->SetAnimation(6);
                mCells2[7].mUnk_09 = 1;
            } else {
                func_02025b64(0, 1, 0, "select_rule_u_bg_00_ncl.bin", 0, 0, 0);
                func_02025b64(0, 11, 0, "select_rule_u_bg_00_ncg.bin", 0, 0, 0);
                func_02025b64(0, 7, 0, "select_rule_u_bg_00_nsc.bin", 0, 0, 0);
                ChangeDir("/Game/Battle/Obj/");
                SpriteScreen &main = SpriteManager::Get().Screen(0);
                main.LoadCharacters("select_rule_u_obj_00.nce_ncg1D.bin", 0x8000, 0);
                main.LoadPalette("select_rule_u_obj_00_ncl.bin", 0x100, 0);
                u8 k = 0;
                do {
                    mCells[k].mAnim            = new CellAnimation("select_rule_u_obj_00_nce.bin", 0, 1, 1);
                    mCells[k].mAnim->mCharBase = main.func_020276f4(0x8000);
                    mCells[k].mAnim->mUnk_11   = 8;
                    mCells[k].mX               = 0x88;
                    mCells[k].mY               = k * 0x18 + 0x26;
                    mCells[k].mVisible         = 1;
                    mCells[k].mUnk_09          = 1;
                    mCells[k].mUnk_0a          = 0;
                    k++;
                } while (k < 5);
                mCells[0].mAnim->SetAnimation(0);
                mCells[1].mAnim->SetAnimation(1);
                mCells[2].mAnim->SetAnimation(2);
                mCells[3].mAnim->SetAnimation(3);
                mCells[4].mAnim->SetAnimation(4);
            }
            func_ov001_02223d70(0);
            break;
        }
        case 2: {
            mMessage->func_02065890(6, 0, 1);
            mMain.mAnim->SetAnimation(3);
            ChangeDir("/Game/Battle/Bg/");
            func_02025b64(1, 1, 0, "select_ch_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 11, 0, "select_ch_b_bg_00_ncg.bin", 0, 0, 0);
            func_02025b64(1, 7, 0, "select_ch_b_bg_00_nsc.bin", 0, 0, 0);
            u8 j = 0;
            do {
                mInput[j].mUnk_02 = mInput[j].mUnk_08;
                j++;
            } while (j < mUnk_32a);
            ChangeDir("/Game/Battle/Obj/");
            SpriteScreen &sub = SpriteManager::Get().Screen(1);
            sub.LoadCharacters("select_ch_b_obj_00.nce_ncg1D.bin", 0x8000, 0);
            sub.LoadPalette("select_ch_b_obj_00_ncl.bin", 0x80, 0);
            u8 k = 0;
            do {
                mCells2[k].mAnim            = new CellAnimation("select_ch_b_obj_00_nce.bin", k & 0xff, 1, 1);
                mCells2[k].mAnim->mCharBase = sub.func_020276f4(0x8000);
                mCells2[k].mAnim->mUnk_11   = 4;
                mCells2[k].mX               = func_ov001_02225628(mInput[k].mUnk_02);
                mCells2[k].mY               = func_ov001_0222565c(mInput[k].mUnk_02);
                mCells2[k].mUnk_09          = 1;
                mCells2[k].mVisible         = 1;
                k++;
            } while (k < mUnk_32a);
            u8 m = 4;
            do {
                mCells2[m].mAnim            = new CellAnimation("select_ch_b_obj_00_nce.bin", m & 0xff, 1, 1);
                mCells2[m].mAnim->mCharBase = sub.func_020276f4(0x8000);
                mCells2[m].mAnim->mUnk_11   = 4;
                mCells2[m].mX               = 0x80;
                mCells2[m].mY               = 0x60;
                mCells2[m].mUnk_09          = 3;
                mCells2[m].mVisible         = 0;
                m++;
            } while (m < 7);
            mCells2[4].mAnim->SetAnimation(5);
            mCells2[5].mAnim->SetAnimation(4);
            mCells2[6].mAnim->SetAnimation(0x18);
            ChangeDir("/Game/Battle/Obj/");
            SpriteManager::Get();
            SpriteScreen &main = SpriteManager::Get().Screen(0);
            main.LoadCharacters("select_order_u_obj_00.nce_ncg1D.bin", 0x4000, 0);
            main.LoadPalette("select_order_u_obj_00_ncl.bin", 0xa0, 0);
            main.LoadCharacters("select_ch_u_obj_00.nce_ncg1D.bin", 0x8000, 0);
            main.LoadPalette("select_ch_u_obj_00_ncl.bin", 0x80, 0);
            u8 c = 0;
            do {
                u32 idx                           = c & 0xff;
                mCells[idx].mAnim            = new CellAnimation("select_ch_u_obj_00_nce.bin", 0, 1, 1);
                mCells[idx].mAnim->mCharBase = main.func_020276f4(0x8000);
                mCells[idx].mAnim->mUnk_11   = 4;
                mCells[idx].mX               = 0x18;
                mCells[idx].mY               = c * 0x24 + 0x2a;
                mCells[idx].mUnk_09          = 3;
                mCells[idx].mVisible         = 1;
                mCells[idx].mUnk_0a          = 0;
                c++;
            } while (c < mUnk_32a);
            mCells[4].mAnim            = new CellAnimation("select_ch_u_obj_00_nce.bin", 2, 1, 1);
            mCells[4].mAnim->mCharBase = main.func_020276f4(0x8000);
            mCells[4].mAnim->mUnk_11   = 4;
            mCells[4].mVisible         = 0;
            ChangeDir("/Game/Character/Icon/");
            main.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0x10000, 0);
            main.LoadPalette("icon_ch_base_ncl.bin", 0x120, 0);
            mCells[6].mAnim            = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
            mCells[6].mAnim->mCharBase = main.func_020276f4(0x10000);
            mCells[6].mAnim->mUnk_11   = 9;
            mCells[6].mVisible         = 0;
            mCells[7].mAnim            = new CellAnimation("icon_ch_base_nce.bin", 2, 1, 1);
            mCells[7].mAnim->mCharBase = main.func_020276f4(0x10000);
            mCells[7].mAnim->mUnk_11   = 9;
            mCells[7].mVisible         = 0;
            sub.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0x13000, 0);
            sub.LoadPalette("icon_ch_base_ncl.bin", 0x40, 0);
            mCells2[7].mAnim            = new CellAnimation("icon_ch_base_nce.bin", 2, 1, 1);
            mCells2[7].mAnim->mCharBase = sub.func_020276f4(0x13000);
            mCells2[7].mAnim->mUnk_11   = 2;
            mCells2[7].mVisible         = 0;
            ChangeDir("/Game/2D/union_b/");
            sub.LoadCharacters("union_b_obj_new.nce_ncg1D.bin", 0x14400, 0);
            sub.LoadPalette("union_b_obj_new_ncl.bin", 0x20, 0);
            mCells2[8].mAnim            = new CellAnimation("union_b_obj_new_nce.bin", 0, 1, 1);
            mCells2[8].mAnim->mCharBase = sub.func_020276f4(0x14400);
            mCells2[8].mAnim->mUnk_11   = 1;
            mCells2[8].mUnk_09          = 3;
            mCells2[8].mVisible         = 0;
            ChangeDir("/");
            u8 p = 0;
            do {
                mPanel[0]->func_02065204(p, -1, 0, 0);
                mPanel[1]->func_02065204(p, -1, 0, 0);
                func_ov001_02226bf8(p);
                UnkStruct_020c3030::Get();
                if (IsWirelessMode(&data_020c4258) && p < mUnk_32b) {
                    func_ov001_022267d0(p);
                }
                p++;
            } while (p < 4);
            mPanel[1]->func_02065204(0, 0x15, (u32) (mUnk_14c + mUnk_33b * 8), 0);
            s16 px = func_ov001_02225628(0);
            s16 py = func_ov001_0222565c(0);
            mPanel[1]->func_0206547c(0, px + 6, py + 3, 1);
            mUnk_334 = 1;
            mUnk_335 = 1;
            mUnk_336 = 1;
            mUnk_331 = 0x10;
            mWindows[2] = new SelectWindow(1, data_ov001_022270ec, 0);
            mWindows[2]->SetCells(1, data_ov001_02227124);
            mWindows[2]->mCursor->mSelection = -1;
            break;
        }
        case 3: {
            mUnk_32a = data_020d433c.mPlayerCount;
            if (mUnk_333 != 0) {
                mMessage->func_02065890(0x11, 0, 1);
            } else {
                mMessage->func_02065890(4, 1, 1);
            }
            mMain.mAnim->SetAnimation(4);
            ChangeDir("/Game/Battle/Obj/");
            SpriteScreen &sub = SpriteManager::Get().Screen(1);
            sub.LoadCharacters("select_ch_b_obj_00.nce_ncg1D.bin", 0x8000, 0);
            sub.LoadPalette("select_ch_b_obj_00_ncl.bin", 0x80, 0);
            u8 k = 0;
            do {
                if (k == 0) {
                    mCells2[k].mAnim = new CellAnimation("select_ch_b_obj_00_nce.bin", 0x19, 1, 1);
                } else {
                    mCells2[k].mAnim = new CellAnimation("select_ch_b_obj_00_nce.bin", 7, 1, 1);
                }
                mCells2[k].mAnim->mCharBase = sub.func_020276f4(0x8000);
                mCells2[k].mAnim->mUnk_11   = 4;
                mCells2[k].mX               = 0x10;
                mCells2[k].mY               = k * 0x24 + 0x1a;
                mCells2[k].mUnk_09          = 3;
                mCells2[k].mVisible         = 1;
                k++;
            } while (k < 5);
            mCells2[0].mAnim->SetAnimation(0x1a);
            mCells2[4].mAnim->SetAnimation(6);
            mCells2[4].mX      = 0x10;
            mCells2[4].mY      = 0x1a;
            mCells2[4].mUnk_09 = 2;
            ChangeDir("/Game/Character/Icon/");
            SpriteScreen &sub2 = SpriteManager::Get().Screen(1);
            sub2.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0x13000, 0);
            sub2.LoadPalette("icon_ch_base_ncl.bin", 0x40, 0);
            mCells2[5].mAnim            = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
            mCells2[5].mAnim->mCharBase = sub2.func_020276f4(0x13000);
            mCells2[5].mAnim->mUnk_11   = 2;
            mCells2[5].mVisible         = 0;
            mCells2[7].mAnim            = new CellAnimation("icon_ch_base_nce.bin", 2, 1, 1);
            mCells2[7].mAnim->mCharBase = sub2.func_020276f4(0x13000);
            mCells2[7].mAnim->mUnk_11   = 2;
            mCells2[7].mVisible         = 0;
            ChangeDir("/Game/Battle/Obj/");
            SpriteScreen &main = SpriteManager::Get().Screen(0);
            main.LoadCharacters("select_order_u_obj_00.nce_ncg1D.bin", 0x4000, 0);
            main.LoadPalette("select_order_u_obj_00_ncl.bin", 0xa0, 0);
            main.LoadCharacters("select_ch_u_obj_00.nce_ncg1D.bin", 0x8000, 0);
            main.LoadPalette("select_ch_u_obj_00_ncl.bin", 0x80, 0);
            u8 c = 0;
            do {
                u32 idx                           = c & 0xff;
                mCells[idx].mAnim            = new CellAnimation("select_ch_u_obj_00_nce.bin", 0, 1, 1);
                mCells[idx].mAnim->mCharBase = main.func_020276f4(0x8000);
                mCells[idx].mAnim->mUnk_11   = 4;
                mCells[idx].mX               = 0x18;
                mCells[idx].mY               = c * 0x24 + 0x2a;
                mCells[idx].mUnk_09          = 3;
                mCells[idx].mVisible         = 1;
                mCells[idx].mUnk_0a          = 0;
                c++;
            } while (c < mUnk_32a);
            mCells[4].mAnim            = new CellAnimation("select_ch_u_obj_00_nce.bin", 2, 1, 1);
            mCells[4].mAnim->mCharBase = main.func_020276f4(0x8000);
            mCells[4].mAnim->mUnk_11   = 4;
            mCells[4].mVisible         = 0;
            ChangeDir("/Game/Character/Icon/");
            main.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0x10000, 0);
            main.LoadPalette("icon_ch_base_ncl.bin", 0x120, 0);
            mCells[6].mAnim            = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
            mCells[6].mAnim->mCharBase = main.func_020276f4(0x10000);
            mCells[6].mAnim->mUnk_11   = 9;
            mCells[6].mVisible         = 0;
            mCells[7].mAnim            = new CellAnimation("icon_ch_base_nce.bin", 2, 1, 1);
            mCells[7].mAnim->mCharBase = main.func_020276f4(0x10000);
            mCells[7].mAnim->mUnk_11   = 9;
            mCells[7].mVisible         = 0;
            mUnk_334 = 1;
            mUnk_338 = 1;
            func_ov001_02223e5c();
            u8 p = 0;
            do {
                mPanel[0]->func_02065204(p, -1, 0, 0);
                mPanel[1]->func_02065204(p, -1, 0, 0);
                if (p < mUnk_32a) {
                    mPanel[0]->func_02065204(p, (s8) data_ov001_02227194[mInput[p].mUnk_08], (u32) (mUnk_14c + p * 8), 0);
                    mPanel[1]->func_02065204(p, (s8) data_ov001_02227194[mInput[p].mUnk_08], (u32) (mUnk_14c + p * 8), 0);
                }
                func_ov001_022267d0(p);
                p++;
            } while (p < 4);
            mUnk_331 = 0x10;
            break;
        }
        case 4: {
            mUnk_32a = data_020d433c.mPlayerCount;
            if (mUnk_333 != 0) {
                mMessage->func_02065890(8, 0, 1);
            } else {
                mMessage->func_02065890(4, 1, 1);
            }
            mMain.mAnim->SetAnimation(5);
            ChangeDir("/Game/Battle/Bg/");
            func_02025b64(1, 1, 0, "select_field_b_bg_00_ncl.bin", 0, 0, 0);
            func_02025b64(1, 11, 0, "select_field_b_bg_00_ncg.bin", 0, 0, 0);
            func_02025b64(1, 7, 0, "select_field_b_bg_00_nsc.bin", 0, 0, 0);
            ChangeDir("/Game/Battle/Obj/");
            SpriteScreen &sub = SpriteManager::Get().Screen(1);
            sub.LoadCharacters("select_field_b_obj_00.nce_ncg1D.bin", 0x8000, 0);
            sub.LoadPalette("select_field_b_obj_00_ncl.bin", 0x100, 0);
            mInput[0].mUnk_02           = mUnk_341;
            mCells2[0].mAnim            = new CellAnimation("select_field_b_obj_00_nce.bin", 0, 1, 1);
            mCells2[0].mAnim->mCharBase = sub.func_020276f4(0x8000);
            mCells2[0].mAnim->mUnk_11   = 8;
            mCells2[0].mX               = func_ov001_02225698(mInput[0].mUnk_02);
            mCells2[0].mY               = func_ov001_022256c8(mInput[0].mUnk_02);
            mCells2[0].mUnk_09          = 1;
            mCells2[0].mVisible         = 1;
            mCells2[4].mAnim            = new CellAnimation("select_field_b_obj_00_nce.bin", 4, 1, 1);
            mCells2[4].mAnim->mCharBase = sub.func_020276f4(0x8000);
            mCells2[4].mAnim->mUnk_11   = 4;
            mCells2[4].mUnk_09          = 3;
            mCells2[4].mVisible         = 0;
            SpriteScreen &main          = SpriteManager::Get().Screen(0);
            main.LoadCharacters("select_field_u_obj_00.nce_ncg1D.bin", 0x8000, 0);
            main.LoadPalette("select_field_u_obj_00_ncl.bin", 0x80, 0);
            mCells[4].mAnim            = new CellAnimation("select_field_u_obj_00_nce.bin", 0, 1, 1);
            mCells[4].mAnim->mCharBase = main.func_020276f4(0x8000);
            mCells[4].mAnim->mUnk_11   = 4;
            mCells[4].mX               = 0x80;
            mCells[4].mY               = 0x60;
            mCells[4].mUnk_09          = 3;
            mCells[4].mVisible         = 1;
            mCells[4].mUnk_0a          = 0;
            ChangeDir("/Game/2D/union_b/");
            SpriteScreen &sub2 = SpriteManager::Get().Screen(1);
            sub2.LoadCharacters("union_b_obj_new.nce_ncg1D.bin", 0x14400, 0);
            sub2.LoadPalette("union_b_obj_new_ncl.bin", 0x20, 0);
            mCells2[8].mAnim            = new CellAnimation("union_b_obj_new_nce.bin", 0, 1, 1);
            mCells2[8].mAnim->mCharBase = sub2.func_020276f4(0x14400);
            mCells2[8].mAnim->mUnk_11   = 1;
            mCells2[8].mUnk_09          = 1;
            mCells2[8].mVisible         = 0;
            SpriteScreen &sub3 = SpriteManager::Get().Screen(1);
            sub3.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x1b000, 0);
            sub3.LoadPalette("union_b_obj_window_ncl.bin", 0x40, 0);
            mUnk_337 = 1;
            func_ov001_02225150(mInput[0].mUnk_02);
            mUnk_331    = 0x10;
            mWindows[3] = new SelectWindow(1, data_ov001_02227108, 0);
            mWindows[3]->SetCells(1, data_ov001_02227140);
            mWindows[3]->mCursor->mSelection = -1;
            break;
        }
        case 5: {
            if (mUnk_333 != 0) {
                mMessage->func_02065890(0xc, 0, 1);
            } else {
                mMessage->func_02065890(4, 1, 1);
            }
            mMain.mAnim->SetAnimation(1);
            if (mUnk_333 != 0) {
                ChangeDir("/Game/2D/union_b/");
                func_02025b64(1, 1, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
                func_02025b64(1, 11, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
                func_02025b64(1, 7, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
                SpriteScreen &sub = SpriteManager::Get().Screen(1);
                sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x8000, 0);
                sub.LoadPalette("union_b_obj_window_ncl.bin", 0x100, 0);
                func_ov001_022262d0(0xc);
                func_ov001_022253f0(0, 1, 0);
            }
            mUnk_331 = 0x10;
            break;
        }
    }
    ChangeDir("/");
}

BOOL FieldSelectScene::func_ov001_02221aa8(u32 unk1) {
    UnkStruct_020c3030::Get();
    if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
        return TRUE;
    }
    u32 count = UnkStruct_020c3030::Get().func_0202f134();
    u8 matches = 0;
    u8 i       = 0;
    do {
        u8 *p = UnkStruct_020c3030::Get().func_0202f0cc(data_020d433c.mUnk_af[i]);
        if (p != NULL && *p == unk1) {
            matches++;
        }
        i++;
    } while (i < count);
    data_0220f540.mUnk_00 = unk1;
    return matches == count;
}

BOOL FieldSelectScene::func_ov001_02221c0c() {
    func_01ff96c4(data_020d496a, (u8 *) &data_0220f540 + 4, 9);
    func_01ff96c4(data_020d4968, (u8 *) &data_0220f540 + 0xd, 1);
    data_0220f540.mUnk_0e = data_020d43f0[0].mUnk_27;
    data_0220f540.mUnk_0f = data_020d43f0[0].mUnk_5bf_0;
    if (func_ov001_02221aa8(0x17)) {
        u32 count = UnkStruct_020c3030::Get().func_0202f134();
        u8 i      = 0;
        do {
            u8 *p = UnkStruct_020c3030::Get().func_0202f0cc(data_020d433c.mUnk_af[i]);
            if (p != NULL) {
                for (u8 j = 0; j < 0x12; j++) {
                    if (j == 0 || j == 0x11) {
                        mUnk_266[i][j] = 3;
                    } else {
                        u8 f           = data_ov001_02227194[j];
                        mUnk_266[i][j] = 0;
                        u32 byte       = (u32) f >> 3;
                        int bit        = f % 8;
                        for (u8 k = 0; k < 3; k++) {
                            if (p[k * 3 + byte + 4] & (1 << bit)) {
                                mUnk_266[i][j] = k + 1;
                            }
                        }
                    }
                }
                for (u8 j = 0; j < 9; j++) {
                    if (j == 8) {
                        mUnk_2ae[i][j] = 1;
                    } else {
                        u8 f = data_ov001_022270c4[j];
                        if (p[0xd + ((u32) f >> 3)] & (1 << (f % 8))) {
                            mUnk_2ae[i][j] = 1;
                        } else {
                            mUnk_2ae[i][j] = 0;
                        }
                    }
                }
                if (i == 0) {
                    u8 b                  = p[0xe];
                    data_020d433c.mUnk_9a = b;
                    mUnk_25c[0]           = ((u32) b << 25) >> 30;
                    mUnk_25c[2]           = data_020d433c.mUnk_9a_3 == 0;
                    mUnk_25c[4]           = data_020d433c.mUnk_9a_2 == 0;
                    mUnk_25c[6]           = data_020d433c.mUnk_9a_4 == 0;
                    mUnk_25c[8]           = data_020d433c.mUnk_9a_0 == 0;
                    data_020d433c.mUnk_96 = mUnk_25c[0];
                }
                if (p[0xf] != 0) {
                    SetFlagBit(data_020d43e3, i);
                }
            }
            i++;
        } while (i < count);
        return TRUE;
    }
    return FALSE;
}

BOOL FieldSelectScene::func_ov001_02221f1c() {
    MI_CpuCopy8(data_020d4404, (u8 *) &data_0220f540 + 4, 8);
    if (func_ov001_02221aa8(6)) {
        u32 count = UnkStruct_020c3030::Get().func_0202f134();
        u8 i      = 0;
        do {
            u8 *p = UnkStruct_020c3030::Get().func_0202f0cc(data_020d433c.mUnk_af[i]);
            if (p != NULL) {
                MI_CpuCopy8(p + 4, mUnk_14c + i * 8, 8);
            }
            i++;
        } while (i < count);
        return TRUE;
    }
    return FALSE;
}

void FieldSelectScene::func_ov001_02222040() {
    if (data_020d433c.mUnk_a8 != 0) {
        UnkStruct_020c3030::Get();
        if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
            if (!((mState != 0) & (mState < 6))) {
                return;
            }
            mState                = 10;
            data_020d433c.mUnk_99 = 3;
            data_020d433c.mPlayerCount = 2;
            u8 i                  = 0;
            data_020d433c.mUnk_98 = 0;
            data_020d433c.mUnk_95 = 0;
            data_020d433c.mUnk_96 = 0;
            do {
                data_020d433c.mUnk_9f[i] = i;
                data_020d433c.mUnk_9b[i] = i;
                i++;
            } while (i < mUnk_32a);
            mUnk_332 = 3;
            return;
        }
    }
    u8 i;
    UnkStruct_020c3030::Get();
    const u8 *mode = &data_020c4258;
    if (IsWirelessMode(mode)) {
        UnkStruct_020c3030::Get();
        data_0220f540.mUnk_01 = data_020c4230.mUnk_20;
        UnkStruct_020c3030::Get().func_0202f024(&data_0220f540);
        UnkStruct_020c3030::Get().func_0202f078(&data_0220f540);
    }
    UnkStruct_020c3030::Get();
    if (IsWirelessMode(mode)) {
        i = 0;
        do {
            u8 *p = UnkStruct_020c3030::Get().func_0202f0cc(data_020d433c.mUnk_af[i]);
            if (p != NULL) {
                if (p[0] == 0xd) {
                    mInput[i].mTouching = p[6];
                    mInput[i].mTouchX   = p[7];
                    mInput[i].mTouchY   = p[8];
                    mInput[i].mKeys     = *(u16 *) (p + 4);
                    mInput[i].mUnk_07   = p[9];
                    mInput[i].mUnk_0b   = p[0xb];
                    mInput[i].mUnk_0e   = p[0xc];
                    mInput[i].mUnk_0f   = p[0xd];
                } else {
                    mInput[i].mTouching = 0;
                    mInput[i].mTouchX   = 0;
                    mInput[i].mTouchY   = 0;
                    mInput[i].mKeys     = 0;
                    mInput[i].mUnk_07   = 0;
                    mInput[i].mUnk_0b   = 0;
                    mInput[i].mUnk_0e   = 0;
                    mInput[i].mUnk_0f   = 0;
                }
            }
            i++;
        } while (i < 4);
        data_0220f540.mUnk_00 = 0xd;
        NET_BYTE(6) = TouchManager::Get().mTouching;
        NET_BYTE(7) = TouchManager::Get().mX;
        NET_BYTE(8) = TouchManager::Get().mY;
        NET_KEYS   = InputManager::Get().IsTrigger(0xf0) | InputManager::Get().IsHeld(3);
        NET_BYTE(9) = 0;
        NET_BYTE(12) = 0;
        NET_BYTE(13) = 0;
    } else {
        mInput[0].mTouching = TouchManager::Get().mTouching;
        mInput[0].mTouchX   = TouchManager::Get().mX;
        mInput[0].mTouchY   = TouchManager::Get().mY;
        mInput[0].mKeys     = InputManager::Get().IsTrigger(0xf0) | InputManager::Get().IsHeld(3);
        mInput[0].mUnk_07   = 0;
        mInput[0].mUnk_0e   = 0;
        mInput[0].mUnk_0f   = 0;
    }
    if ((mState == 5) & (mUnk_330 == 0)) {
        if (mUnk_33d != 0 && mInput[mUnk_33b].mUnk_0a == 0) {
            u8 r = mWindows[2]->Update(0);
            if (r != 0) {
                switch (r) {
                    case 1:
                    case 2:
                        UnkStruct_020c3030::Get();
                        if (IsWirelessMode(mode)) {
                            NET_KEYS |= 2;
                        } else {
                            mInput[0].mKeys |= 2;
                        }
                        break;
                }
                func_ov001_02226c38(mWindows[2]);
            }
        }
        if (mUnk_333 != 0) {
            if (mInput[0].mUnk_0a != 0) {
                if (mInput[0].mUnk_0d != 0) {
                    goto tail;
                }
                BOOL r = mWindows[1]->Update(1);
                switch (r) {
                    case 1:
                        UnkStruct_020c3030::Get();
                        if (IsWirelessMode(mode)) {
                            NET_KEYS |= 1;
                        } else {
                            mInput[0].mKeys |= 1;
                        }
                        break;
                    case 2:
                        UnkStruct_020c3030::Get();
                        if (IsWirelessMode(mode)) {
                            NET_KEYS |= 2;
                        } else {
                            mInput[0].mKeys |= 2;
                        }
                        break;
                    default:
                        UnkStruct_020c3030::Get();
                        if (IsWirelessMode(mode)) {
                            if (NET_KEYS & 1) {
                                NET_KEYS -= 1;
                            }
                            if (NET_KEYS & 2) {
                                NET_KEYS -= 2;
                            }
                        } else {
                            if (mInput[0].mKeys & 1) {
                                mInput[0].mKeys -= 1;
                            }
                            if (mInput[0].mKeys & 2) {
                                mInput[0].mKeys -= 2;
                            }
                        }
                        break;
                }
                u8 sel = mWindows[1]->mCursor->mSelection;
                if (sel < 3) {
                    UnkStruct_020c3030::Get();
                    if (IsWirelessMode(mode)) {
                        NET_BYTE(11) = sel;
                    } else {
                        mInput[0].mUnk_0b = sel;
                    }
                }
            } else {
                u8 r = mWindows[0]->Update(0);
                if (r != 0) {
                    switch (r) {
                        case 1:
                            UnkStruct_020c3030::Get();
                            if (!IsWirelessMode(mode)) {
                                mInput[0].mUnk_0e = 1;
                            } else {
                                NET_BYTE(12) = 1;
                            }
                            break;
                        case 2:
                            UnkStruct_020c3030::Get();
                            if (IsWirelessMode(mode)) {
                                if (mUnk_33d != 0) {
                                    if (!(NET_KEYS & 2)) {
                                        NET_BYTE(9) = 1;
                                    }
                                } else {
                                    NET_BYTE(9) = 1;
                                }
                            } else {
                                if (mUnk_33d != 0) {
                                    if (!(mInput[0].mKeys & 2)) {
                                        mInput[0].mUnk_07 = 1;
                                    }
                                } else {
                                    mInput[0].mUnk_07 = 1;
                                }
                            }
                        }
                        func_ov001_02226c38(mWindows[0]);
                    }
                    if (mWindows[3] != NULL) {
                        u8 r2 = mWindows[3]->Update(0);
                        if (r2 != 0) {
                            if (r2 == 1) {
                                UnkStruct_020c3030::Get();
                                if (IsWirelessMode(mode)) {
                                    NET_BYTE(13) = 1;
                                } else {
                                    mInput[0].mUnk_0f = 1;
                                }
                            }
                            func_ov001_02226c38(mWindows[3]);
                        }
                    }
                }
            } else {
                if (mUnk_2ed[mUnk_329] == 2) {
                    UnkStruct_020c3030::Get();
                    if (IsWirelessMode(mode)) {
                                                if (mInput[mUnk_33b].mUnk_0a != 0) {
                            if (mInput[mUnk_33b].mUnk_0d == 0) {
                                BOOL r = mWindows[1]->Update(1);
                                switch (r) {
                                    case 1:
                                        NET_KEYS |= 1;
                                        break;
                                    case 2:
                                        NET_KEYS |= 2;
                                        break;
                                    default:
                                        if (NET_KEYS & 1) {
                                            NET_KEYS -= 1;
                                        }
                                        if (NET_KEYS & 2) {
                                            NET_KEYS -= 2;
                                        }
                                        break;
                                }
                                u8 sel = mWindows[1]->mCursor->mSelection;
                                if (sel < 3) {
                                    NET_BYTE(11) = sel;
                                }
                            }
                        } else {
                            u8 r = mWindows[0]->Update(0);
                            if (r != 0) {
                                switch (r) {
                                    case 1:
                                    case 2:
                                        NET_KEYS |= 1;
                                        break;
                                }
                                func_ov001_02226c38(mWindows[0]);
                            }
                        }
                    }
                }
            }
        }
tail:
    i = 0;
    do {
        if (mInput[i].mUnk_0d != 0) {
            mInput[i].mUnk_0d--;
        }
        if (mInput[i].mUnk_07 != 0) {
            if (mInput[i].mUnk_0d != 0) {
                mInput[i].mUnk_07 = 0;
            } else {
                mInput[i].mUnk_0d = 8;
            }
        }
        if (mInput[i].mKeys & 2) {
            if (mInput[i].mUnk_0d != 0) {
                mInput[i].mKeys -= 2;
            } else {
                mInput[i].mUnk_0d = 8;
            }
        }
        if (mInput[i].mKeys & 1) {
            if (mInput[i].mUnk_0d != 0) {
                mInput[i].mKeys -= 1;
            } else {
                mInput[i].mUnk_0d = 8;
            }
        }
        i++;
    } while (i < mUnk_32b);
}

void FieldSelectScene::func_ov001_02222e10() {
    u8 i = 0;
    do {
        mInput[i].mUnk_02 = 0;
        mInput[i].mUnk_06 = 0;
        mInput[i].mUnk_07 = 0;
        i++;
    } while (i < 4);
    func_ov001_0221fa94();
}

// non-matching: block layout in the result == 2 loop (the v == 3 block comes after v == 2 in the original).
u32 FieldSelectScene::func_ov001_02222e44() {
    if (func_ov001_02225240()) {
        return 0;
    }
    if (mUnk_333 != 0) {
        if (mInput[0].mUnk_0a != 0) {
            switch (mUnk_2ed[mUnk_329]) {
                case 2:
                    mPanel[1]->func_020654a0();
                    mCells2[6].mAnim->Draw(1, 0x70, 0x2d, 0);
                    break;
                case 4:
                case 5:
                    func_ov001_02226430();
                    break;
            }
            mWindows[1]->Draw();
        } else {
            mWindows[0]->Draw();
            if (mUnk_33d != 0) {
                mWindows[2]->Draw();
            }
            if (mWindows[3] != NULL) {
                mWindows[3]->Draw();
            }
        }
    } else {
        if (mUnk_2ed[mUnk_329] == 2) {
            if (mInput[mUnk_33b].mUnk_0a != 0) {
                mPanel[1]->func_020654a0();
                mCells2[6].mAnim->Draw(1, 0x70, 0x2d, 0);
                mWindows[1]->Draw();
            } else if (mUnk_33d != 0) {
                mWindows[2]->Draw();
            } else {
                mWindows[0]->Draw();
            }
        }
    }
    if (mUnk_344 != -1) {
        G2S_SetBG1Priority(0);
        GXS_SetVisiblePlane(mUnk_344);
        mUnk_344 = -1;
    }
    u32 result;
    switch (mUnk_2ed[mUnk_329]) {
        case 5:
            result = func_ov001_02225214();
            break;
        case 1:
            result = func_ov001_02224178();
            break;
        case 2:
            result = func_ov001_02224538();
            break;
        case 3:
            result = func_ov001_02224be0();
            break;
        case 4:
            result = func_ov001_02224df0();
            break;
        case 0:
            result = func_ov001_02223f5c();
            break;
    }
    if (result != 0) {
    switch (result) {
        case 1:
            mUnk_329++;
            if ((data_020d433c.mPlayerCount < 4) & (mUnk_2ed[mUnk_329] == 3)) {
                mUnk_329++;
            }
            if ((mUnk_32d == 0) & (mUnk_2ed[mUnk_329] == 1)) {
                mUnk_329++;
            }
            if (mUnk_2ed[mUnk_329] >= 6) {
                mUnk_332 = 1;
                return 1;
            }
            break;
        case 2:
            if (mUnk_2ed[mUnk_329] == 2) {
                mPanel[1]->func_02065204(3, -1, 0, 0);
            }
            for (;;) {
                if (mUnk_329 != 0) {
                    mUnk_329--;
                } else {
                    return 2;
                }
                if (mUnk_2ed[mUnk_329] == 3) {
                    if (mUnk_32a == 4) {
                        break;
                    }
                    continue;
                }
                if (mUnk_2ed[mUnk_329] == 2) {
                    u8 i = 0;
                    do {
                        mInput[i].mUnk_06 = 1;
                        mInput[i].mUnk_02 = func_ov001_02226d48(data_020d433c.mUnk_9f[i]);
                        i++;
                    } while (i < mUnk_32a);
                }
                break;
            }
            break;
        case 3:
            return 3;
    }
    mUnk_330 = 1;
    }
    return 0;
}

void FieldSelectScene::func_ov001_022231ec(u32 slot, u32 value, u32 playSound) {
    if (value != mInput[slot].mUnk_02) {
        mInput[slot].mUnk_02 = value;
        switch (mUnk_2ed[mUnk_329]) {
            case 2:
                mCells2[slot].mX = func_ov001_02225628(mInput[slot].mUnk_02);
                mCells2[slot].mY = func_ov001_0222565c(mInput[slot].mUnk_02);
                if ((slot == mUnk_33b) | ((slot >= mUnk_32b) && (mUnk_333 != 0))) {
                    playSound = 1;
                } else {
                    playSound = 0;
                }
                break;
            case 0: {
                for (u8 i = 0; i < 3; i++) {
                    if (mCells2[i].mAnim != NULL) {
                        mCells2[i].mAnim->SetAnimation(i);
                    }
                }
                u8 s = mInput[0].mUnk_02;
                mCells2[s].mAnim->SetAnimation(s + 3);
                break;
            }
            case 3: {
                for (u8 i = 0; i < 4; i++) {
                    if (i == 0) {
                        mCells2[i].mAnim->SetAnimation(0x19);
                    } else {
                        mCells2[i].mAnim->SetAnimation(7);
                    }
                }
                u8 s = mInput[0].mUnk_02;
                if (s == 0) {
                    mCells2[s].mAnim->SetAnimation(0x1a);
                } else {
                    mCells2[s].mAnim->SetAnimation(8);
                }
                mCells2[4].mY = s * 0x24 + 0x1a;
                break;
            }
            case 4:
                mCells2[slot].mX = func_ov001_02225698(mInput[slot].mUnk_02);
                mCells2[slot].mY = func_ov001_022256c8(mInput[slot].mUnk_02);
                func_ov001_02225150(mInput[slot].mUnk_02);
                break;
        }
    }
    if (playSound) {
        SoundManager::Get().func_020246f8(5);
    }
}

BOOL FieldSelectScene::func_ov001_02223430(u32 slot) {
    switch (mUnk_2ed[mUnk_329]) {
        case 2:
            if (mInput[slot].mUnk_02 == 0x11) {
                do {
                    do {
                        mInput[slot].mUnk_02 = MATH_Rand32(&data_020d42e8, 0x12);
                    } while (mInput[slot].mUnk_02 == 0x11);
                } while (func_ov001_022237d0(slot) != 0);
                mCells2[slot].mX = func_ov001_02225628(mInput[slot].mUnk_02);
                mCells2[slot].mY = func_ov001_0222565c(mInput[slot].mUnk_02);
            } else {
// non-matching: register allocation only.
                if (func_ov001_022237d0(slot) != 0) {
                    if (slot < mUnk_32b) {
                        if (slot == mUnk_33b) {
                            SoundManager::Get().func_020246f8(2);
                        }
                    } else if (mUnk_33b == 0) {
                        SoundManager::Get().func_020246f8(2);
                    }
                    return FALSE;
                }
            }
            mInput[slot].mUnk_08  = mInput[slot].mUnk_02;
            mCells2[slot].mUnk_09 = 2;
            mPanel[0]->func_02065204(slot, ((const s8 *) data_ov001_02227194)[mInput[slot].mUnk_02], (u32) (mUnk_14c + slot * 8), 0);
            func_ov001_022267d0(slot);
            break;
        case 3: {
            u8 s = mInput[0].mUnk_02;
            if (s == 0) {
                mCells2[s].mAnim->SetAnimation(0x1b);
            } else {
                mCells2[s].mAnim->SetAnimation(9);
            }
            break;
        }
        case 0:
            mCells2[mInput[0].mUnk_02].mAnim->SetAnimation(mInput[0].mUnk_02 + 6);
            break;
        case 4:
            if (mInput[0].mUnk_02 == 8) {
                u8 r;
                do {
                    r = MATH_Rand32(&data_020d42e8, 8);
                } while (mUnk_2ae[0][r] == 0);
                func_ov001_022231ec(0, r, 0);
            }
            if (mUnk_2ae[0][mInput[0].mUnk_02] == 0) {
                if (mUnk_33b == 0) {
                    SoundManager::Get().func_020246f8(2);
                }
                return FALSE;
            }
            break;
    }
    mInput[slot].mUnk_06 = 1;
    SoundManager::Get().func_020246f8(0);
    return TRUE;
}

BOOL FieldSelectScene::func_ov001_022237d0(u32 slot) {
    u8 idx;
    if (slot < mUnk_32b) {
        idx = mInput[slot].mUnk_02;
        if (mUnk_266[slot][idx] == 0) {
            return TRUE;
        }
    } else {
        idx = mInput[slot].mUnk_02;
        if (mUnk_266[0][idx] == 0) {
            return TRUE;
        }
    }
    if (idx == 0) {
        UnkStruct_020c3030::Get();
        if (IsWirelessMode(&data_020c4030.mUnk_228)) {
            return slot >= mUnk_32b;
        }
        return slot != 0;
    }
    BOOL result = FALSE;
    u8 i        = 0;
    do {
        if (i != slot && mInput[i].mUnk_06 != 0) {
            if (mInput[i].mUnk_02 == idx) {
                return TRUE;
            }
        }
        i++;
    } while (i < mUnk_32a);
    return result;
}

BOOL FieldSelectScene::func_ov001_02223914() {
    if (mUnk_2ed[mUnk_329] == 2) {
        u8 flag        = 0;
        const u8 *mode = &data_020c4258;
        u8 i           = 0;
        do {
            if (mInput[i].mKeys & 2) {
                u8 slot = i;
                if (i == 0) {
                    u8 j = mUnk_32b;
                    u8 n = mUnk_32a;
                    if (n != j) {
                        do {
                            if (mInput[j].mUnk_06 != 0) {
                                slot = j;
                            }
                            j++;
                        } while (j < n);
                    }
                    if (mInput[slot].mUnk_06 != 0) {
                        mPanel[0]->func_02065204(slot, -1, 0, 0);
                        UnkStruct_020c3030::Get();
                        if (IsWirelessMode(mode)) {
                            if (slot >= mUnk_32b) {
                                func_ov001_02226bf8(slot);
                            }
                        } else {
                            func_ov001_02226bf8(slot);
                        }
                        if (slot == mUnk_33b) {
                            mPanel[1]->func_02065204(3, -1, 0, 0);
                        }
                        mCells2[slot].mUnk_09 = 1;
                        mInput[slot].mUnk_06  = 0;
                        if (mInput[mUnk_33b].mUnk_0a == 0) {
                            SoundManager::Get().func_020246f8(1);
                        }
                        flag = 1;
                    }
                } else {
                    if (mInput[i].mUnk_06 != 0) {
                        mPanel[0]->func_02065204(i, -1, 0, 0);
                        UnkStruct_020c3030::Get();
                        if (IsWirelessMode(mode)) {
                            if (i >= mUnk_32b) {
                                func_ov001_02226bf8(i);
                            }
                        } else {
                            func_ov001_02226bf8(i);
                        }
                        if (i == mUnk_33b) {
                            mPanel[1]->func_02065204(3, -1, 0, 0);
                        }
                        mCells2[i].mUnk_09 = 1;
                        mInput[i].mUnk_06  = 0;
                        if (mInput[mUnk_33b].mUnk_0a == 0) {
                            SoundManager::Get().func_020246f8(1);
                        }
                    }
                }
                if (slot == mUnk_33b) {
                    mPanel[1]->func_02065204(0, -1, 0, 0);
                    mPanel[1]->func_02065204(0, 0x15, (u32) (mUnk_14c + mUnk_33b * 8), 0);
                    mUnk_340 = 0;
                }
            }
            i++;
        } while (i < mUnk_32b);
        if (flag != 0) {
            return FALSE;
        }
    }
    if (mInput[0].mUnk_0a != 0) {
        return FALSE;
    }
    if (mInput[0].mUnk_07 != 0) {
        SoundManager::Get().func_020246f8(1);
        return TRUE;
    }
    if (mInput[0].mKeys & 2) {
        SoundManager::Get().func_020246f8(1);
        return TRUE;
    }
    return FALSE;
}

void FieldSelectScene::func_ov001_02223d70(u32 unk1) {
    u8 v    = mInput[0].mUnk_02;
    u8 mode = mUnk_25c[0];
    if (v == 0 && unk1 != 0) {
        switch (mode) {
            case 0:
                mUnk_25c[2] = 0;
                mUnk_25c[4] = 0;
                mUnk_25c[6] = 0;
                break;
            case 1:
                mUnk_25c[2] = 1;
                mUnk_25c[4] = 0;
                mUnk_25c[6] = 0;
                break;
            case 2:
                mUnk_25c[2] = 1;
                mUnk_25c[4] = 1;
                mUnk_25c[6] = 1;
                break;
        }
    }
    if (mUnk_333 != 0) {
        u8 i = 0;
        do {
            mCells2[i].mAnim->mUnk_0f = mUnk_25c[i * 2];
            i++;
        } while (i < 5);
        mCells2[7].mY = mInput[0].mUnk_02 * 0x18 + 0x26;
    } else {
        u8 i = 0;
        do {
            mCells[i].mAnim->mUnk_0f = mUnk_25c[i * 2];
            i++;
        } while (i < 5);
    }
}

void FieldSelectScene::func_ov001_02223e5c() {
    u8 r = mInput[0].mUnk_02 % 4;
    if (r == 0) {
        mCells[0].mAnim->SetAnimation(0);
        mCells[1].mAnim->SetAnimation(0);
        mCells[2].mAnim->SetAnimation(0);
        mCells[3].mAnim->SetAnimation(0);
    } else {
        mCells[0].mAnim->SetAnimation(1);
        mCells[1].mAnim->SetAnimation(1);
        mCells[2].mAnim->SetAnimation(1);
        mCells[3].mAnim->SetAnimation(1);
        u8 a = data_ov001_022270dd[r * 4];
        u8 b = data_ov001_022270de[r * 4];
        u8 c = data_ov001_022270df[r * 4];
        mCells[0].mAnim->mUnk_0f = 0;
        mCells[a].mAnim->mUnk_0f = 0;
        mCells[b].mAnim->mUnk_0f = 1;
        mCells[c].mAnim->mUnk_0f = 1;
    }
}

// non-matching: the original computes (s16) (0x30 - 0x10) at run time; it folds here.
u32 FieldSelectScene::func_ov001_02223f5c() {
    u32 result = 0;
    if (func_ov001_02223914()) {
        result = 2;
    } else {
    if (mInput[0].mKeys & 1) {
        func_ov001_02223430(0);
    } else if (mInput[0].mTouching != 0) {
        u8 cur = mInput[0].mUnk_02;
        int dx = (s16) (mInput[0].mTouchX - 0x20);
        if ((dx >= 0) & (dx < 0xc0)) {
            u8 n = mUnk_32b;
            u8 i = 0;
            if (n > 2) {
                i++;
            }
            do {
                int base = 0x30;
                if (n > 2) {
                    base = 0x20;
                }
                int dy = (s16) (mInput[0].mTouchY - base - i * 0x24);
                if ((dy >= 0) & (dy < 0x20)) {
                    if (cur == i) {
                        func_ov001_02223430(0);
                    } else {
                        func_ov001_022231ec(0, i, 1);
                    }
                    break;
                }
                i++;
            } while (i < 3);
        }
    } else if (mInput[0].mKeys != 0) {
        u8 cur = mInput[0].mUnk_02;
        u8 v   = cur % 3;
        u8 n   = mUnk_32b;
        do {
            if (mInput[0].mKeys & 0x40) {
                v = (v + 2) % 3;
            } else if (mInput[0].mKeys & 0x80) {
                v = (v + 1) % 3;
            }
        } while (n > 2 && v == 0);
        if (v != cur) {
            func_ov001_022231ec(0, v, 1);
        }
    }
    if (mInput[0].mUnk_06 != 0) {
        mUnk_32a = mInput[0].mUnk_02 % 3 + 2;
        result   = 1;
    }
    }
    return result;
}

// non-matching: the original keeps this in r7 without spilling.
u32 FieldSelectScene::func_ov001_02224178() {
    u32 result = 0;
    if (func_ov001_02223914()) {
        data_020d433c.mUnk_9a_5 = mUnk_25c[0];
        data_020d433c.mUnk_9a_3 = (u8) ((mUnk_25c[2] == 0) ? 1 : 0);
        data_020d433c.mUnk_9a_2 = (u8) ((mUnk_25c[4] == 0) ? 1 : 0);
        data_020d433c.mUnk_9a_4 = (u8) ((mUnk_25c[6] == 0) ? 1 : 0);
        data_020d433c.mUnk_9a_0 = (u8) ((mUnk_25c[8] == 0) ? 1 : 0);
        data_020d433c.mUnk_96   = mUnk_25c[0];
        if (mUnk_333 != 0) {
            data_020d43f0[0].mUnk_27   = data_020d433c.mUnk_9a;
            data_020d43f0[0].mUnk_28_6 = data_020d433c.mUnk_9a_0;
        }
        result = 2;
    } else {
        u32 prev = 0;
        u32 next = 0;
        u32 changed = 0;
        if (!(mInput[0].mKeys & 1)) {
            if (mInput[0].mTouching != 0) {
                int y = mInput[0].mTouchY;
                int x = mInput[0].mTouchX;
                int t = y - 0x26;
                if (((t % 0x18) < 0x14) & ((y < 0x9e) & ((y > 0x26) & ((x >= 0x79) & (x <= 0xfc))))) {
                    int sel = t / 0x18;
                    if (sel > 4) {
                        sel = 4;
                    }
                    if (mInput[0].mUnk_02 != (u8) sel) {
                        mInput[0].mUnk_02 = sel;
                        changed           = 1;
                    }
                    if (x <= 0x91) {
                        prev = 1;
                    }
                    if (x >= 0xe4) {
                        next = 1;
                    }
                }
            } else if (mInput[0].mKeys != 0) {
                u8 cur = mInput[0].mUnk_02;
                u8 v   = cur % 5;
                if (mInput[0].mKeys & 0x40) {
                    v = (v + 4) % 5;
                } else if (mInput[0].mKeys & 0x80) {
                    v = (v + 1) % 5;
                }
                if (v != cur) {
                    mInput[0].mUnk_02 = v;
                    changed           = 1;
                } else if (mInput[0].mKeys & 0x20) {
                    prev = 1;
                } else if (mInput[0].mKeys & 0x10) {
                    next = 1;
                }
            }
        }
        u8 idx = mInput[0].mUnk_02 % 5;
        if (prev) {
            mUnk_25c[idx * 2] = (mUnk_25c[idx * 2] + mUnk_25c[idx * 2 + 1] - 1) % mUnk_25c[idx * 2 + 1];
            if (mUnk_333 != 0) {
                mCells2[5].mAnim->SetAnimation(1);
                mCells2[5].mY = idx * 0x18 + 0x26;
            }
            changed = 1;
        }
        if (next) {
            mUnk_25c[idx * 2] = (mUnk_25c[idx * 2] + 1) % mUnk_25c[idx * 2 + 1];
            if (mUnk_333 != 0) {
                mCells2[6].mAnim->SetAnimation(4);
                mCells2[6].mY = idx * 0x18 + 0x26;
            }
            changed = 1;
        }
        if (changed) {
            func_ov001_022231ec(0, idx, 1);
            func_ov001_02223d70(prev || next);
        }
    }
    return result;
}

// non-matching: register allocation and prologue.
u32 FieldSelectScene::func_ov001_02224538() {
    u32 result = 0;
    u8 numDone = 0;
    if (func_ov001_02223914()) {
        result = 2;
    } else {
        u8 numOpen = 0;
        u8 flagB   = 0;
        mUnk_33f   = 4;
        u8 i       = 0;
        do {
            u8 owner;
            u8 event = 0;
            if (i >= mUnk_32b) {
                if (flagB != 0) {
                    break;
                }
                owner = 0;
                if (mInput[0].mUnk_0a != 0) {
                    numOpen++;
                    if (i == mUnk_33c) {
                        u16 keys = mInput[0].mKeys;
                        if (keys & 1) {
                            event = 1;
                            mInput[mUnk_33c].mUnk_0c = mInput[0].mUnk_0b;
                        } else if (keys & 2) {
                            mInput[mUnk_33c].mUnk_06 = 0;
                            event                    = 1;
                        }
                        if (event) {
                            if (mUnk_33b == 0) {
                                mPanel[1]->func_02065204(3, -1, 0, 0);
                            }
                            func_ov001_022253f0(0, 0, 0);
                            break;
                        }
                    }
                    continue;
                }
            } else {
                owner = i;
                if (mInput[i].mUnk_0a != 0) {
                    numOpen++;
                    if (i == 0 && mUnk_33c != 0) {
                        continue;
                    }
                    u16 keys = mInput[i].mKeys;
                    u8 fired = 0;
                    if (keys & 1) {
                        fired                = 1;
                        mInput[i].mUnk_0c = mInput[i].mUnk_0b;
                    } else if (keys & 2) {
                        mInput[i].mUnk_06 = 0;
                        fired             = 1;
                    }
                    if (fired == 0) {
                        continue;
                    }
                    if (mUnk_33c == 0) {
                        flagB = 1;
                    }
                    if (i == mUnk_33b) {
                        mPanel[1]->func_02065204(3, -1, 0, 0);
                    }
                    func_ov001_022253f0(i, 0, 0);
                    continue;
                }
            }
            if (mInput[i].mUnk_06 != 0) {
                numDone++;
                continue;
            }
            {
                u16 keys = mInput[owner].mKeys;
                if (keys & 1) {
                    event = 1;
                } else if (mInput[owner].mTouching != 0) {
                    u8 cur = mInput[i].mUnk_02;
                    u8 k   = 0;
                    do {
                        int dx = (s16) (mInput[owner].mTouchX - func_ov001_02225628(k));
                        int dy = (s16) (mInput[owner].mTouchY - func_ov001_0222565c(k));
                        if (((dy >= 0) & ((dx >= 0) & (dx <= 0x28))) & (dy <= 0x20)) {
                            if (cur == k) {
                                event = 1;
                            } else {
                                func_ov001_022231ec(i, k, 1);
                            }
                            break;
                        }
                        k++;
                    } while (k < 0x12);
                } else if (keys & 0xf0) {
                    u8 cur = mInput[i].mUnk_02;
                    u8 col = cur % 6;
                    u8 row = cur / 6;
                    if (keys & 0x40) {
                        row = (row + 2) % 3;
                    } else if (keys & 0x80) {
                        row = (row + 1) % 3;
                    } else if (keys & 0x20) {
                        col = (col + 5) % 6;
                    } else if (keys & 0x10) {
                        col = (col + 1) % 6;
                    }
                    int n = row * 6 + col;
                    if (n > 0x11) {
                        n = 0x11;
                    }
                    func_ov001_022231ec(i, n, 1);
                }
            }
            if (owner == 0) {
                flagB = 1;
            }
            if (owner == mUnk_33b) {
                mUnk_33f = i;
            }
            if (event != 0) {
                if (func_ov001_02223430(i)) {
                    u8 show = 0;
                    if (mInput[i].mUnk_02 != 0) {
                        if (owner == 0) {
                            mUnk_33c = i;
                        }
                        func_ov001_022253f0(owner, 1, mInput[i].mUnk_02);
                        if (owner == mUnk_33b) {
                            mPanel[1]->func_02065204(3, ((const s8 *) data_ov001_02227194)[mInput[i].mUnk_02], (u32) (mUnk_14c + i * 8), 0);
                            mPanel[1]->func_0206547c(3, 0x72, 0x2f, 0);
                        }
                    }
                    if (mInput[i].mUnk_02 == 0) {
                        if (i == mUnk_33b) {
                            show = 1;
                        }
                    } else if (mUnk_33b == 0) {
                        if (mUnk_32a != mUnk_32b) {
                            if (i == mUnk_33b) {
                                show = 1;
                            }
                        }
                    }
                    if (show) {
                        mPanel[1]->func_02065204(0, -1, 0, 0);
                        mPanel[1]->func_02065204(0, 0x15, (u32) (mUnk_14c + mUnk_33b * 8), 1);
                        mUnk_340 = 1;
                    }
                }
            }
        } while (++i < mUnk_32a);
        if (((numOpen == 0) & (numDone >= mUnk_32a)) || mInput[0].mUnk_0e != 0) {
            if (mInput[0].mUnk_0e != 0) {
                SoundManager::Get().func_020246f8(0);
            }
            data_020d433c.mPlayerCount = numDone;
            if (numDone < 4) {
                mUnk_32c              = 0;
                data_020d433c.mUnk_98 = 0;
            }
            u8 j = 0;
            do {
                u8 v                     = data_ov001_02227194[mInput[j].mUnk_02];
                data_020d433c.mUnk_9f[j] = v;
                if (v == 0x15) {
                    data_020d433c.mUnk_a3[j] = 0;
                } else {
                    data_020d433c.mUnk_a3[j] = mInput[j].mUnk_0c;
                }
                j++;
            } while (j < numDone);
            mUnk_341 = 0;
            result   = 1;
        }
    }
    u8 old   = mUnk_33d;
    mUnk_33d = mInput[mUnk_33b].mUnk_06;
    if (mUnk_33d == 0) {
        mWindows[0]->mButtons[0].mEnabled = 0;
        if (old != 0) {
            mWindows[2]->mUnk_061              = 1;
            mWindows[2]->mUnk_065              = 0;
            mWindows[2]->mCursor->mSelection   = -1;
            mWindows[2]->mCells[0]->SetAnimation(2);
        }
    }
    return result;
}

// non-matching: register order in the table-store tail.
u32 FieldSelectScene::func_ov001_02224be0() {
    u32 result = 0;
    if (func_ov001_02223914()) {
        result = 2;
    } else {
        u16 keys = mInput[0].mKeys;
        if (keys & 1) {
            func_ov001_02223430(0);
        } else if (mInput[0].mTouching != 0) {
            u8 cur = mInput[0].mUnk_02;
            int dx = (s16) (mInput[0].mTouchX - 0x10);
            if ((dx >= 0) & (dx < 0xe0)) {
                int y = mInput[0].mTouchY - 0x1a;
                u8 k  = 0;
                do {
                    int d = (s16) (y - k * 0x24);
                    if ((d >= 0) & (d < 0x24)) {
                        if (cur == k) {
                            func_ov001_02223430(0);
                        } else {
                            func_ov001_022231ec(0, k, 1);
                        }
                        break;
                    }
                    k++;
                } while (k < 4);
            }
        } else if (keys != 0) {
            u8 cur = mInput[0].mUnk_02;
            u8 v   = cur % 4;
            if (keys & 0x40) {
                v = (v + 3) % 4;
            } else if (keys & 0x80) {
                v = (v + 1) % 4;
            }
            if (v != cur) {
                func_ov001_022231ec(0, v, 1);
            }
        }
        func_ov001_02223e5c();
        if (mInput[0].mUnk_06 != 0) {
            u8 idx = mInput[0].mUnk_02 % 4;
            if (idx == 0) {
                mUnk_32c = 0;
            } else {
                mUnk_32c                 = 1;
                u8 a                     = data_ov001_022270dd[idx * 4];
                u8 b                     = data_ov001_022270de[idx * 4];
                u8 c                     = data_ov001_022270df[idx * 4];
                data_020d433c.mUnk_9b[0] = a;
                data_020d43d7[a]         = 0;
                data_020d43d7[b]         = c;
                data_020d43d7[c]         = b;
            }
            data_020d433c.mUnk_98 = mUnk_32c;
            mUnk_341              = 0;
            result                = 1;
        }
    }
    return result;
}

// non-matching: loop counter and dx swap registers.
u32 FieldSelectScene::func_ov001_02224df0() {
    u32 result = 0;
    if (func_ov001_02223914()) {
        result = 2;
    } else {
        if (mInput[0].mUnk_0a != 0) {
            u16 keys = mInput[0].mKeys;
            if (keys & 1) {
                func_ov001_022253f0(0, 0, 0);
                mUnk_348 = 1;
                if (mInput[0].mUnk_0b == 1) {
                    mInput[0].mUnk_06 = 0;
                }
            } else if (keys & 2) {
                func_ov001_022253f0(0, 0, 0);
                mUnk_348          = 1;
                mInput[0].mUnk_06 = 0;
            }
        } else {
            u16 keys = mInput[0].mKeys;
            u8 ev    = 0;
            if (keys & 1) {
                ev = 1;
            } else if (mInput[0].mTouching != 0) {
                u8 cur = mInput[0].mUnk_02;
                u8 k   = 0;
                do {
                    int dx = (s16) (mInput[0].mTouchX - func_ov001_02225698(k));
                    int dy = (s16) (mInput[0].mTouchY - func_ov001_022256c8(k));
                    if (((dy >= 0) & ((dx >= 0) & (dx <= 0x38))) & (dy <= 0x20)) {
                        if (cur == k) {
                            ev = 1;
                        } else {
                            func_ov001_022231ec(0, k, 1);
                        }
                        break;
                    }
                    k++;
                } while (k < 9);
            } else if (keys != 0) {
                u8 cur = mInput[0].mUnk_02;
                u8 col = cur % 4;
                u8 row = cur / 4;
                if (keys & 0x40) {
                    row = (row + 2) % 3;
                } else if (keys & 0x80) {
                    row = (row + 1) % 3;
                } else if (keys & 0x20) {
                    col = (col + 3) % 4;
                } else if (keys & 0x10) {
                    col = (col + 1) % 4;
                }
                int n = col + row * 4;
                if (n > 8) {
                    n = 8;
                }
                if ((u8) n != cur) {
                    func_ov001_022231ec(0, n, 1);
                }
            }
            if (ev) {
                if (func_ov001_02223430(0)) {
                    func_ov001_022253f0(0, 1, 0);
                    if (mUnk_333 != 0) {
                        mUnk_348 = 0;
                    }
                }
            }
        }
        if ((mInput[0].mUnk_06 == 1) & (mInput[0].mUnk_0a == 0)) {
            data_020d433c.mUnk_95 = data_ov001_022270c4[mInput[0].mUnk_02];
            result                = 1;
            mUnk_32d              = 0;
        }
        if (mInput[0].mUnk_0f != 0) {
            mUnk_341 = mInput[0].mUnk_02;
            result   = 1;
            mUnk_32d = 1;
            SoundManager::Get().func_020246f8(0);
        }
    }
    return result;
}

void FieldSelectScene::func_ov001_02225150(u32 idx) {
    u8 sel;
    if (mUnk_2ae[mUnk_33b][idx] == 0 && mUnk_2ae[0][idx] == 0) {
        sel = 9;
    } else {
        sel = data_ov001_022270d0[idx];
    }
    func_02025c1c(0, 1, 0, mFiles[sel][1], mFileSizes[sel][1], 0);
    func_02025c1c(0, 0xb, 0, mFiles[sel][0], mFileSizes[sel][0], 0);
    func_02025c1c(0, 7, 0, mFiles[sel][2], mFileSizes[sel][2], 0);
    mCells[4].mAnim->mUnk_0f = sel;
}

u32 FieldSelectScene::func_ov001_02225214() {
    u32 result = 0;
    if (mInput[0].mKeys & 1) {
        if (mInput[0].mUnk_0b == 1) {
            result = 1;
        } else {
            result = 3;
        }
    }
    return result;
}

BOOL FieldSelectScene::func_ov001_02225240() {
    if (mUnk_330 != 0) {
    if (mUnk_32f == mUnk_32e) {
        switch (mUnk_330) {
            case 1:
                mUnk_32f = 0x10;
                if (mWindows[0] != NULL) {
                    mWindows[0]->mUnk_064 = 0;
                }
                mUnk_330++;
                break;
            case 2:
                func_ov001_02222e10();
                reg_G2_BLDCNT = 0;
                reg_G2S_DB_BLDCNT = 0;
                mUnk_330++;
                break;
            case 3:
                if (mUnk_331 != 0) {
                    mUnk_331--;
                    return TRUE;
                }
                mUnk_32f = 0;
                mUnk_330++;
                break;
            case 4:
                mWindows[0]->mUnk_064 = 1;
                mUnk_330 = 0;
                break;
        }
    }
    func_ov001_02225338();
    return TRUE;
    }
    return FALSE;
}

void FieldSelectScene::func_ov001_02225338() {
    u8 target = mUnk_32f;
    u8 cur    = mUnk_32e;
    if (cur == target) {
        return;
    }
    if (mUnk_2ed[mUnk_329] == 5) {
        mUnk_32e = target;
    } else if (cur < target) {
        mUnk_32e = cur + 1;
    } else {
        mUnk_32e = cur - 1;
    }
    GX_SetVisiblePlane(0x1b);
    GXS_SetVisiblePlane(0x1b);
    G2_SetBlendAlpha(2, 0x39, mUnk_32e, 0x10 - mUnk_32e);
    G2S_SetBlendAlpha(2, 0x3d, mUnk_32e, 0x10 - mUnk_32e);
}

// non-matching: this, slot and idx in different registers.
void FieldSelectScene::func_ov001_022253f0(u32 slot, u32 value, u32 idx) {
    if (value == mInput[slot].mUnk_0a) {
        return;
    }
    mInput[slot].mUnk_0a = value;
    if (slot != mUnk_33b) {
        return;
    }
    if (mInput[slot].mUnk_0a != 0) {
        if (mWindows[1] != NULL) {
            delete mWindows[1];
        }
        u32 set;
        u32 count;
        u32 sel;
        switch (mUnk_2ed[mUnk_329]) {
            case 4:
                func_ov001_022262d0(0x12);
                set   = 3;
                count = 3;
                G2S_SetBG1Priority(1);
                GXS_SetVisiblePlane(0x1f);
                sel = 0;
                break;
            case 5:
                set   = 4;
                count = 3;
                sel   = 1;
                break;
            default:
                if (mUnk_333 != 0 && mUnk_33c != 0) {
                    mMessage->func_02065890(0xe, 0, 1);
                } else {
                    mMessage->func_02065890(0xf, 0, 1);
                }
                u32 row = slot;
                set     = 2;
                if (slot >= mUnk_32b) {
                    row = 0;
                }
                count = 6;
                sel   = 1;
                if (mUnk_266[row][idx] == 1) {
                    set = 0;
                    sel = 0;
                } else if (mUnk_266[row][idx] == 2) {
                    set = 1;
                }
                break;
        }
        mWindows[1] = new SelectWindow(count, data_ov001_02227218[set], -1);
        mWindows[1]->SetCells(count, data_ov001_02227560[set]);
        mWindows[1]->mCursor->mSelection = -1;
        mWindows[1]->mCursor->mSelection = sel;
        mInput[slot].mUnk_0b             = sel;
        if (slot == mUnk_33b) {
            NET_BYTE(0xb) = sel;
        }
    } else {
        if (mWindows[1] != NULL) {
            delete mWindows[1];
        }
        mWindows[1] = NULL;
        if (mUnk_2ed[mUnk_329] == 2) {
            mMessage->func_02065890(6, 0, 1);
        }
        if (mUnk_2ed[mUnk_329] == 4) {
            func_ov001_0222657c();
            mUnk_344 = 0x1b;
        }
    }
}

s16 FieldSelectScene::func_ov001_02225628(u32 idx) {
    int i = idx;
    return (i % 6) * 0x28 + 8;
}

s16 FieldSelectScene::func_ov001_0222565c(u32 idx) {
    int i = idx;
    return ((i / 6) % 6) * 0x28 + 0x2c;
}

s16 FieldSelectScene::func_ov001_02225698(u32 idx) {
    if (idx >= 8) {
        return 0x64;
    }
    return ((int) idx % 4) * 0x3c + 10;
}

s16 FieldSelectScene::func_ov001_022256c8(u32 idx) {
    return (idx >> 2) * 0x26 + 0x2c;
}

// non-matching: y/v/x registers and the spill order of the (s16) (y + k) temporaries.
void FieldSelectScene::func_ov001_022256e4() {
    if (mUnk_348 != 0) {
        mMessage->func_020658d0();
    }
    if (mMain.mVisible != 0) {
        mMain.mAnim->Update();
        mMain.mAnim->Draw(0, mMain.mX, mMain.mY, mMain.mUnk_09);
    }
    if (mCells2[8].mAnim != NULL) {
        mCells2[8].mAnim->Update();
    }
    if (mUnk_334 != 0) {
        func_ov001_02226b18();
        u8 i = 0;
        do {
            s16 y = i * 0x24 + 0x27;
            mPanel[0]->func_0206547c(i, 0x1b, (s16) (y + 4), 1);
            mCells[6].mAnim->Draw(0, 0x19, (s16) (y + 2), 3);
            if (mUnk_32b <= i) {
                mCells[7].mAnim->SetAnimation(0xb);
                mCells[7].mAnim->func_0202a3ec(0, i, 0x15, y, 1);
            }
            if (mUnk_33b == i) {
                mCells[7].mAnim->SetAnimation(i + 2);
                mCells[7].mAnim->Draw(0, 0x19, (s16) (y + 0x16), 1);
            }
            mCells[7].mAnim->SetAnimation(i + 7);
            mCells[7].mAnim->Draw(0, 0x15, y, 1);
            int sel = -1;
            u8 kind = mUnk_2ed[mUnk_329];
            if (kind == 2) {
                if (mUnk_32b <= i) {
                    if (mUnk_33c > i) {
                        if (mInput[i].mUnk_06 != 0) {
                            sel = mInput[i].mUnk_0c;
                        }
                    } else if (mUnk_33c == i && mInput[0].mUnk_0a == 0) {
                        if (mInput[i].mUnk_06 != 0) {
                            sel = mInput[i].mUnk_0c;
                        }
                    }
                } else {
                    if (mInput[i].mUnk_0a == 0) {
                        if (mInput[i].mUnk_06 != 0) {
                            sel = mInput[i].mUnk_0c;
                        }
                    }
                    if (i == 0) {
                        if (mUnk_33c != 0 && mInput[i].mUnk_06 != 0) {
                            sel = mInput[i].mUnk_0c;
                        }
                    }
                }
                if (data_ov001_02227194[mInput[i].mUnk_02] == 0x15) {
                    sel = -1;
                }
            } else if (kind != 0) {
                if (i < data_020d433c.mPlayerCount) {
                    if (data_020d433c.mUnk_9f[i] != 0x15) {
                        sel = (s8) data_020d433c.mUnk_a3[i];
                    }
                }
            }
            if (sel >= 0) {
                mCells[4].mAnim->func_0202a3ec(0, sel, 0x40, (s16) (i * 0x24 + 0x38), 1);
            }
            i++;
        } while (i < mUnk_32a);
        mPanel[0]->func_020654a0();
    }
    if (mUnk_336 != 0) {
        u8 slot = mUnk_33f;
        if (slot < 4) {
            if (mUnk_32b <= slot) {
                mCells2[7].mAnim->SetAnimation(0xb);
                mCells2[7].mAnim->func_0202a3ec(1, slot, mCells2[slot].mX, mCells2[slot].mY, mCells2[slot].mUnk_09);
            }
            if (mUnk_33b == slot) {
                mCells2[7].mAnim->SetAnimation(slot + 2);
                mCells2[7].mAnim->Draw(1, (s16) (mCells2[slot].mX + 4), (s16) (mCells2[slot].mY + 0x16),
                                       mCells2[slot].mUnk_09);
            }
            mCells2[7].mAnim->SetAnimation(slot + 7);
            mCells2[7].mAnim->Draw(1, mCells2[slot].mX, mCells2[slot].mY, mCells2[slot].mUnk_09);
        }
    }
    if (mUnk_335 != 0) {
        mPanel[1]->func_020654a0();
    }
    if (mUnk_338 != 0) {
        u8 r = 0;
        do {
            s16 y = r * 0x24 + 0x18;
            u8 col = 0;
            do {
                int v = data_ov001_022270dc[r * 4 + col];
                int x = data_ov001_022270c0[col];
                if (r == 0) {
                    x = data_ov001_022270bc[col];
                }
                mPanel[1]->func_0206547c(v, (s16) (x + 6), (s16) (y + 4), 1);
                mCells2[5].mAnim->Draw(1, (s16) (x + 4), (s16) (y + 2), 2);
                if (mUnk_32b <= v) {
                    mCells2[7].mAnim->SetAnimation(0xb);
                    mCells2[7].mAnim->func_0202a3ec(1, v, x, y, 1);
                }
                if (mUnk_33b == v) {
                    mCells2[7].mAnim->SetAnimation(v + 2);
                    mCells2[7].mAnim->Draw(1, (s16) (x + 4), (s16) (y + 0x16), 1);
                }
                mCells2[7].mAnim->SetAnimation(v + 7);
                mCells2[7].mAnim->Draw(1, x, y, 1);
                col++;
            } while (col < 4);
            mPanel[1]->func_020654a0();
            r++;
        } while (r < 4);
    }
    if (mUnk_339 != 0) {
        u8 j = 0;
        if (mUnk_32b > 2) {
            j++;
        }
        do {
            u8 n    = mUnk_32b;
            int lim = j + 2;
            if (n < lim) {
                do {
                    mCells2[7].mAnim->func_0202a3ec(1, n, (s16) (mCells2[j].mX + 0x14 + n * 32), mCells2[j].mY,
                                                    mCells2[j].mUnk_09);
                    n++;
                } while (n < lim);
            }
            j++;
        } while (j < 3);
    }
    if (mUnk_337 != 0) {
        u8 k = 0;
        do {
            if (mUnk_2e4[k] != 0) {
                s16 x = func_ov001_02225698(k);
                s16 y = func_ov001_022256c8(k);
                mCells2[8].mAnim->Draw(1, (s16) (x + 0xb), (s16) (y + 0x1b), mCells2[8].mUnk_09);
            }
            k++;
        } while (k < 9);
    }
    u8 n = 0;
    do {
        if (mCells[n].mVisible != 0) {
            if (mCells[n].mUnk_0a != 0) {
                mCells[n].mAnim->Update();
            }
            mCells[n].mAnim->Draw(0, mCells[n].mX, mCells[n].mY, mCells[n].mUnk_09);
        }
        n++;
    } while (n < 9);
    if (mUnk_336 != 0) {
        u8 s = mUnk_33f;
        if (s < 4) {
            if (mCells2[s].mVisible != 0) {
                if (mCells2[s].mUnk_0a != 0) {
                    mCells2[s].mAnim->Update();
                }
                mCells2[s].mAnim->Draw(1, mCells2[s].mX, mCells2[s].mY, mCells2[s].mUnk_09);
            }
        }
    } else {
        u8 m = 0;
        do {
            if (mCells2[m].mVisible != 0) {
                if (mCells2[m].mUnk_0a != 0) {
                    mCells2[m].mAnim->Update();
                }
                mCells2[m].mAnim->Draw(1, mCells2[m].mX, mCells2[m].mY, mCells2[m].mUnk_09);
            }
            m++;
        } while (m < 9);
    }
    if (mUnk_337 != 0) {
        u8 k = 0;
        do {
            if (mUnk_2ae[mUnk_33b][k] == 0) {
                s16 x = func_ov001_02225698(k);
                s16 y = func_ov001_022256c8(k);
                mCells2[4].mAnim->Draw(1, x, (s16) (y + 1), mCells2[4].mUnk_09);
            }
            k++;
        } while (k < 9);
    }
    if (mUnk_336 == 0) {
        return;
    }
    u8 f = 0;
    do {
        if (mUnk_266[mUnk_33b][f] == 0) {
            s16 x = func_ov001_02225628(f);
            s16 y = func_ov001_0222565c(f);
            mCells2[4].mAnim->Draw(1, x, (s16) (y - 4), mCells2[4].mUnk_09);
        } else if (mUnk_2d2[f] != 0) {
            u32 show = 1;
            if (mUnk_33f < 4) {
                if (f == mInput[mUnk_33f].mUnk_02) {
                    show = 0;
                }
            }
            if (show != 0) {
                s16 x = func_ov001_02225628(f);
                s16 y = func_ov001_0222565c(f);
                mCells2[8].mAnim->Draw(1, (s16) (x + 3), (s16) (y + 0x1b), mCells2[8].mUnk_09);
            }
        }
        u8 p = 0;
        do {
            if (mInput[p].mUnk_06 != 0) {
                u8 q = mInput[p].mUnk_08;
                if ((q != 0) & (f == q)) {
                    mCells2[5].mAnim->func_0202a3ec(1, f, mCells2[4].mX, mCells2[4].mY, mCells2[4].mUnk_09);
                    break;
                }
            }
            p++;
        } while (p < mUnk_32a);
        f++;
    } while (f < 0x12);
    if (mUnk_340 != 0) {
        mCells2[5].mAnim->func_0202a3ec(1, 0, mCells2[4].mX, mCells2[4].mY, mCells2[4].mUnk_09);
    }
}

void FieldSelectScene::func_ov001_022260b8() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    ChangeDir("/");
    mUnk_2f4 = (MessageFile *) func_02023504("font/mes_BattleArenaMenu.mes", 0, 0, 0, 0, 0);
    mUnk_300 = TextManager::Get().BufferSize(0x18, 0x10);
    mUnk_2f8 = OS_Alloc(mUnk_300);
    MI_CpuClear16(mUnk_2f8, mUnk_300);
    mUnk_304 = TextManager::Get().GlyphCount(6);
    mUnk_2fc = OS_Alloc(mUnk_304 * 12);
    TextManager::Get().Init(mUnk_2fc, 1, 0, 0x18, 0x10, 0x44);
    TextManager::Get().SetColor(1, 0, 0xf);
}

void FieldSelectScene::func_ov001_022262d0(u32 index) {
    func_ov001_0222657c();
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mUnk_2f4;
    text.Print(file->GetText(index), file->GetLength(index), mUnk_2f8, 0x18, 0x10, &height, &width, 0, 0, 2, 0x44, 0xe, 0, 0,
               1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x15000, mUnk_2f8, mUnk_300, 0);
}

void FieldSelectScene::func_ov001_02226430() {
    if (mUnk_2ed[mUnk_329] == 4) {
        SpriteManager::Get().Screen(1).func_02027778(mUnk_2fc, (u8) mUnk_304, 0x20, 0x28, 0, 0x2a0, 0xf, 0);
    } else {
        SpriteManager::Get().Screen(1).func_02027778(mUnk_2fc, (u8) mUnk_304, 0x20, 0x38, 0, 0x2a0, 0xf, 0);
    }
}

void FieldSelectScene::func_ov001_0222657c() {
    if (mUnk_2f8 != NULL) {
        MI_CpuClear16(mUnk_2f8, mUnk_300);
    }
}

void FieldSelectScene::func_ov001_0222659c() {
    SpriteManager::Get().Screen(0).func_02027624(3);
    ChangeDir("/");
    mUnk_308 = (MessageFile *) func_02023504("/font/mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
    mUnk_320 = TextManager::Get().BufferSize(0x20, 2);
    for (u8 i = 0; i < 4; i++) {
        mUnk_30c[i] = OS_Alloc(mUnk_320);
        func_ov001_02226bf8(i);
    }
    mUnk_324 = TextManager::Get().GlyphCount(8);
    mUnk_31c = OS_Alloc(mUnk_324 * 12);
    TextManager::Get().Init(mUnk_31c, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
}

// non-matching: guard load vs mUnk_5b store order (see docs/decompiling.md).
void FieldSelectScene::func_ov001_022267d0(u32 slot) {
    func_ov001_02226bf8(slot);
    u32 useName = 0;
    u8 id       = data_ov001_02227194[mInput[slot].mUnk_08];
    if (id == 0x15) {
        useName = 1;
    } else {
        UnkStruct_020c3030::Get();
        if (IsWirelessMode(&data_020c4030.mUnk_228)) {
            if (slot < mUnk_32b) {
                useName = 1;
            }
        }
    }
    TextManager::Get().mUnk_5b = 1;
    if (useName) {
        s16 height;
        s16 width;
        height = 0;
        width  = 0;
        TextManager::Get().Print(&data_020d433c.mNames[data_020d43eb[slot]], 0x18, mUnk_30c[slot], 0x20, 2, &height, &width, 0, 0, 2,
                                 0x21, 0x10, 0, 0, 0, 1, 1);
    } else {
        TextManager &text = TextManager::Get();
        s16 height;
        s16 width;
        width             = 0;
        height            = 0;
        MessageFile *file = mUnk_308;
        text.Print(file->GetText(id), file->GetLength(id), mUnk_30c[slot], 0x20, 2, &height, &width, 0, 0, 2, 0x21, 0xe, 0, 0, 0, 1,
                   1);
    }
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, (slot << 12) + 0xc000, mUnk_30c[slot], mUnk_320, 0);
}

void FieldSelectScene::func_ov001_02226b18() {
    u8 i = 0;
    do {
        SpriteManager::Get().Screen(0).func_02027778(mUnk_31c, (u8) mUnk_324, 0x40, (s16) (i * 0x24 + 0x2c), 2,
                                                     ((((u32) i << 12) + 0xc000) << 9) >> 16, 0xf, 0);
        i++;
    } while (i < mUnk_32a);
}

void FieldSelectScene::func_ov001_02226bf8(u32 index) {
    if (mUnk_30c[index] != NULL) {
        MI_CpuClear16(mUnk_30c[index], mUnk_320);
        MI_CpuClearFast((void *) (0x6400000 + 0xc000 + (index << 12)), 0x1000);
    }
}

void FieldSelectScene::func_ov001_02226c38(SelectWindow *window) {
    u8 state;
    do {
        window->Draw();
        window->Update(0);
        state = window->mUnk_061;
    } while (!((state <= 2) | (state == 4)));
    window->func_0205c7c8(-1);
    window->mCursor->mSelection = -1;
    window->mUnk_064            = 1;
}

void FieldSelectScene::func_ov001_02226cb0() {
    u8 i = 0;
    do {
        func_ov001_02226d18(data_020d433c.mUnk_9f[i]);
        i++;
    } while (i < mUnk_32a);
    SetFlagBit(data_020d4969, data_020d433c.mUnk_95);
}

void FieldSelectScene::func_ov001_02226d18(u32 index) {
    if (index < 0x15) {
        SetFlagBit(data_020d4973, index);
    }
}

u32 FieldSelectScene::func_ov001_02226d48(u32 id) {
    u8 i = 0;
    do {
        if (data_ov001_02227194[i] == id) {
            return i;
        }
        i++;
    } while (i < 0x12);
    return i;
}

// non-matching: the original keeps i in r5 and tests the loaded value directly.
void FieldSelectScene::func_ov001_02226d74() {
    UnkStruct_020c3030::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228)) {
        u8 i = mUnk_32b;
        if (i < mUnk_32a) {
            do {
                if (data_020d433c.mUnk_a7 & 1) {
                    SetFlagBit(data_020d43e3, i);
                }
                i++;
            } while (i < mUnk_32a);
        }
    }
}
