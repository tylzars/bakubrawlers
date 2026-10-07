#include "Scene/TitleScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "System/SoundManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include <nitro/gx.h>
#include "System/FileSystem.hpp"
#include <stdio.h>
#include "Text/TextManager.hpp"
#include <nitro/fx.h>

extern u8 data_ov000_022266fc[];

void operator delete(void *ptr);

TitleScene::TitleScene() :
    SceneBase(1000, 0) {
    mCursor = NULL;
    mWindow30 = NULL;
    mMessageWindow = NULL;
    mBuf38 = NULL;
    mBuf3c = NULL;
    mBuf40 = NULL;
    mBuf44 = NULL;
    mUnk_48 = 0;
    mUnk_4c = 0;
    mUnk_c8 = 0;
    mUnk_cb = 0;
    mUnk_cc = 0;
    mUnk_cd = 0;
    mUnk_cf = 0xff;
    mUnk_d0 = 0;
    mUnk_d1 = 0x10;
    mUnk_d4 = 0;
    mUnk_d6 = 0;
    mUnk_d7 = 0;
    mUnk_d8 = 0x60;
    data_020d433c.mUnk_7b = 4;
    data_020d433c.mUnk_7d = 0;
    data_020d433c.mUnk_87 = 0;
    data_020d433c.mUnk_88_4 = 0;
    data_020d433c.mUnk_88_0 = 0;
    data_020d433c.mUnk_7e_0 = 0;
    data_020d433c.mUnk_8a_0 = 0;
    MI_CpuClear32(mCells, sizeof(mCells));
    MI_CpuClear32(mBuffers, sizeof(mBuffers));
    MI_CpuClear32(mUnk_8c, sizeof(mUnk_8c));
    mCursor = new SelectWindowCursor(1, data_ov000_022266fc, 0);
    mUnk_ce = data_020d433c.mUnk_7e_4;
    mUnk_d5 = mUnk_ce;
    SoundManager::Get().func_020245a4();
    SoundManager::Get().func_020246d4(4);
}

TitleScene::~TitleScene() {
    for (u8 i = 0; i < 8; i++) {
        if (mCells[i] != NULL) {
            delete mCells[i];
        }
    }
    operator delete(mCursor);
    delete mWindow30;
    delete mMessageWindow;
    if (mBuf38 != NULL) {
        OS_Free(mBuf38);
    }
    if (mBuf3c != NULL) {
        OS_Free(mBuf3c);
    }
    if (mBuf40 != NULL) {
        OS_Free(mBuf40);
    }
    if (mBuf44 != NULL) {
        OS_Free(mBuf44);
    }
    for (u8 i = 0; i < 5; i++) {
        for (u8 j = 0; j < 3; j++) {
            if (mBuffers[i][j] != NULL) {
                OS_Free(mBuffers[i][j]);
            }
        }
    }
}

void TitleScene::func_ov000_02223aa4() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(2, 1, 2, 4, 0);
    G2S_SetBG2Control(2, 1, 4, 6, 0);
    G2S_SetBG3Control(0, 1, 6, 7, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0x140);
    G2S_SetBG2Offset(0, 0x140);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(1);
    G2S_SetBG2Priority(2);
    G2S_SetBG3Priority(0);
    GX_SetVisiblePlane(2);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

void TitleScene::func_ov000_02223cdc() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(0);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x13);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_BlendNone();
    G2S_BlendNone();
}

void TitleScene::LoadGraphics() {
    char path[0x80];
    ChangeDir("/Game/Title/");
    func_02025b64(0, 1, 0, "title_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "title_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "title_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(0, 1, 0x2000, "title_u_bg_01_ncl.bin", 0, 0, 0);
    func_02025b64(0, 12, 0, "title_u_bg_01_ncg.bin", 0, 0, 0);
    func_02025b64(0, 8, 0, "title_u_bg_01_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "title_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "title_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "title_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "title_b_bg_02_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "title_b_bg_02_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "title_b_bg_02_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 1, 0x6000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 14, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 10, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Title/");
    if (mBuffers[0][1] == NULL) {
        for (u8 i = 0; i < 5; i++) {
            sprintf(path, "title_b_bg_sub_%02d_ncl.bin", i);
            mBuffers[i][1] = func_02023504(path, 0, 0, 0, &mUnk_8c[i][1], 0);
            sprintf(path, "title_b_bg_sub_%02d_ncg.bin", i);
            mBuffers[i][0] = func_02023504(path, 0, 0, 0, &mUnk_8c[i][0], 0);
            sprintf(path, "title_b_bg_sub_%02d_nsc.bin", i);
            mBuffers[i][2] = func_02023504(path, 0, 0, 0, &mUnk_8c[i][2], 0);
        }
    }
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.LoadCharacters("title_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen.func_02027fec("title_b_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i < 8; i++) {
        if (mCells[i] != NULL) {
            delete mCells[i];
        }
        mCells[i] = new CellAnimation("title_b_obj_00_nce.bin", 0, 1, 1);
        mCells[i]->mCharBase = screen.func_020276f4(0);
        mCells[i]->mUnk_11 = 0;
    }
    mCells[3]->SetAnimation(0);
    mCells[4]->SetAnimation(3);
    mCells[5]->SetAnimation(6);
    mCells[6]->SetAnimation(9);
    mCells[7]->SetAnimation(12);
    screen.LoadCharacters("title_b_obj_03.nce_ncg1D.bin", 0xb000, 0);
    screen.LoadPalette("title_b_obj_03_ncl.bin", 0x40, 0);
    if (mCells[0] != NULL) {
        delete mCells[0];
    }
    mCells[0] = new CellAnimation("title_b_obj_03_nce.bin", 0, 1, 1);
    mCells[0]->mCharBase = screen.func_020276f4(0xb000);
    mCells[0]->mUnk_11 = 2;
    screen.LoadCharacters("title_b_obj_02.nce_ncg1D.bin", 0xc000, 0);
    screen.func_02027fec("title_b_obj_02_ncl.bin", 0x400, 0);
    if (mCells[1] != NULL) {
        delete mCells[1];
    }
    mCells[1] = new CellAnimation("title_b_obj_02_nce.bin", 1, 1, 1);
    mCells[1]->mCharBase = screen.func_020276f4(0xc000);
    mCells[1]->mUnk_11 = 2;
    screen.LoadCharacters("title_b_obj_04.nce_ncg1D.bin", 0x1d000, 0);
    screen.LoadPalette("title_b_obj_04_ncl.bin", 0xe0, 0);
    if (mCells[2] != NULL) {
        delete mCells[2];
    }
    mCells[2] = new CellAnimation("title_b_obj_04_nce.bin", 0, 1, 1);
    mCells[2]->mCharBase = screen.func_020276f4(0x1d000);
    mCells[2]->mUnk_11 = 7;
    screen.LoadCharacters("title_b_obj_01.nce_ncg1D.bin", 0x13000, 0);
    screen.func_02027fec("title_b_obj_01_ncl.bin", 0x200, 0);
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x1b000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
    delete mMessageWindow;
    mMessageWindow = new MessageWindow("mes_title", 1, 0x1f000);
    mMessageWindow->func_02065bd4(0x14, 4, 1);
    mMessageWindow->func_02065be4(0xd8);
}

void TitleScene::func_ov000_022245e8() {
    mCells[0]->Update();
    mCells[0]->Draw(1, 0x80, 0x60, 0);
}

void TitleScene::func_ov000_02224620() {
    for (u8 i = 0; i < 5; i++) {
        mCells[3 + i]->Update();
        mCells[3 + i]->Draw(1, 0x80, 0x60, 3);
    }
}

void TitleScene::func_ov000_02224678() {
    for (u8 i = 0; i < 5; i++) {
        u8 anim = i * 3;
        if (i == mUnk_ce) {
            anim = anim + 1;
        }
        mCells[3 + i]->SetAnimation(anim);
    }
}

void TitleScene::LoadSystemText() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    if (mBuf38 == NULL) {
        ChangeDir("/");
        mBuf38 = func_02023504("font/mes_system.mes", 0, 0, 0, 0, 0);
        mBuf3c = func_02023504("font/mes_title.mes", 0, 0, 0, 0, 0);
        mUnk_48 = TextManager::Get().BufferSize(0x18, 0xa);
        mBuf40 = OS_Alloc(mUnk_48);
        mUnk_4c = TextManager::Get().GlyphCount(0x3c);
        mBuf44 = OS_Alloc(mUnk_4c * 0xc);
        TextManager::Get().Init(mBuf44, 1, 0, 0x18, 0xa, 0x11);
    }
    MI_CpuClear16(mBuf40, mUnk_48);
    TextManager::Get().SetColor(1, 0, 0xf);
}

void TitleScene::PrintText(MessageFile *file, u32 index) {
    ClearText();
    TextManager &text = TextManager::Get();
    s16 outWidth;
    s16 outHeight;
    outHeight = 0;
    outWidth = 0;
    text.Print(file->GetText(index), file->GetLength(index), mBuf40, 0x18, 0xa, &outWidth, &outHeight, NULL, 0, 2,
               0x11, 0xe, 0, 0, 1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x19000, mBuf40, mUnk_48, 0);
}

void TitleScene::DrawText() {
    SpriteManager::Get().Screen(1).func_02027778(mBuf44, (u8) mUnk_4c, 0x20, 0x28, 0, 0x320, 0xf, 0);
}

void TitleScene::ClearText() {
    if (mBuf40 != NULL) {
        MI_CpuClear16(mBuf40, mUnk_48);
    }
}

void TitleScene::SetMessage(u32 index) {
    if ((index <= 6) & (mUnk_cf != index)) {
        mUnk_cf = index;
        mMessageWindow->func_02065890(mUnk_cf, 0, 1);
    }
}

void TitleScene::SetWindowMode(u8 mode) {
    mUnk_d3 = mode;
    if (mode) {
        GX_SetVisiblePlane(3);
        G2_SetWndOutsidePlane(0x1f, TRUE);
        mUnk_d2 = 0x2d;
        mUnk_d1 = 0x10;
    } else {
        GX_SetVisiblePlane(1);
        G2_BlendNone();
        mUnk_d2 = 0;
    }
}

// Same helper as FieldSelectScene.cpp defines locally; belongs in nitro/gx.h next to G2S_SetBlendAlpha.
static inline void G2_SetBlendAlpha(int plane1, int plane2, int ev1, int ev2) {
    G2x_SetBlendAlpha_(&reg_G2_BLDCNT, plane1, plane2, ev1, ev2);
}

void TitleScene::UpdateFade() {
    if (!mUnk_d3) {
        return;
    }
    u16 angle = (mUnk_d2 << 16) / 180;
    s16 sin = FX_SinCosTable_[(angle >> 4) * 2];
    s8 level = (mUnk_d1 * sin) / 4096;
    if (level < 0) {
        level = 0;
    }
    int ev2 = 0x10 - level;
    G2_SetBlendAlpha(2, 1, level, ev2);
    if (mUnk_d2 != 0) {
        mUnk_d2--;
        return;
    }
    mUnk_d1 = 6;
    mUnk_d2 = 0xb4;
}

BOOL TitleScene::IsFadeDone(BOOL tick) {
    if (!mUnk_d3) {
        return TRUE;
    }
    if (mUnk_d2 < 0x5a) {
        if (tick != 0 && mUnk_d2 != 0) {
            mUnk_d2--;
        }
        return FALSE;
    }
    return TRUE;
}

void TitleScene::UploadSubBg(u32 index) {
    func_02025c1c(1, 1, 0x4000, mBuffers[index][1], mUnk_8c[index][1], 0);
    func_02025c1c(1, 13, 0, mBuffers[index][0], mUnk_8c[index][0], 0);
    func_02025c1c(1, 9, 0, mBuffers[index][2], mUnk_8c[index][2], 0);
}

BOOL TitleScene::UpdateCursorCell() {
    mCells[1]->Update();
    mCells[1]->Draw(1, 0x80, 0x58, 2);
    if (mUnk_d4 == 0) {
        if (mUnk_d5 == mUnk_ce) {
            return TRUE;
        }
        mUnk_d4 = 1;
        mCells[1]->SetAnimation(2);
        SoundManager::Get().func_020246f8(0xd);
    }
    switch (mUnk_d4) {
        case 1:
            if (mCells[1]->mUnk_12 != 0) {
                mUnk_d4++;
            }
            break;
        case 2: {
            u8 cur = mUnk_ce;
            mUnk_d5 = cur;
            UploadSubBg(cur);
            mCells[1]->SetAnimation(3);
            SoundManager::Get().func_020246f8(0xc);
            mUnk_d4++;
            break;
        }
        case 3:
            if (mCells[1]->mUnk_12 != 0) {
                mUnk_d4 = 0;
            }
            break;
    }
    return FALSE;
}

void TitleScene::ApplyScroll() {
    G2S_SetBG1Offset(0, mUnk_da);
    G2S_SetBG2Offset(0, mUnk_da);
}

void TitleScene::DrawLogo() {
    if (!mUnk_d7) {
        return;
    }
    if (mUnk_d8 < 0x60) {
        mUnk_d8++;
    } else {
        mMessageWindow->func_020658d0();
    }
    mCells[2]->Draw(1, 0x80, mUnk_d8, 1);
}
