#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Scene/MyroomGiftScene.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern const u8 data_ov008_02228a64[3][4];
extern const u8 data_ov008_02228a7c[][8];
extern const u8 data_ov008_02228a7d[][8];
extern const u8 data_ov008_02228a94[];
extern const s8 data_ov008_02228aac[];
extern u8 data_ov008_02228a70[];
extern WindowItemRecord data_ov008_02228b28[];
extern WindowCellRecord data_ov008_0222a160[];
extern WindowCellRecord data_ov008_02228ab8[];
extern const u8 data_ov008_02228aa0[];
extern const u8 data_ov008_02228a56[][2];
extern const u32 data_ov008_02228a5c[];

MyroomGiftScene::MyroomGiftScene() :
    SceneBase(1000, 0) {
    mMessages = NULL;
    mMessage  = NULL;
    mWindow   = NULL;
    mResult   = 0;
    mUnk_40   = 0;
    mUnk_42   = 0x100;
    mUnk_44   = 0;
    mEmblem   = -1;
    mUnk_51   = 0;
    mUnk_52   = 0;
    mState    = 0;
    mTimer    = 0;
    mUnk_55   = 0;
    mCellAnim = NULL;
    for (u8 i = 0; i < 2; i++) {
        mTextBuffers[i]    = NULL;
        mGlyphBuffers[i]   = NULL;
        mTextBufferSize[i] = 0;
        mGlyphCount[i]     = 0;
    }
    InitEmblems();
}

MyroomGiftScene::~MyroomGiftScene() {
    delete mWindow;
    delete mMessage;
    CellAnimation *cell = mCellAnim;
    if (cell != NULL) {
        delete cell;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    ClearTextBuffers(0);
    ClearTextBuffers(1);
    for (u8 i = 0; i < 2; i++) {
        OS_Free(mTextBuffers[i]);
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
        }
    }
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().func_0202740c();
}

void MyroomGiftScene::Update() {
    BOOL draw = TRUE;
    switch (mState) {
        case 0:
            draw = FALSE;
            InitDisplay();
            func_ov008_0221de34();
            mTimer = 10;
            mState++;
            break;
        case 1:
            draw = FALSE;
            mTimer--;
            if (mTimer > 0) {
                break;
            }
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 2:
            func_ov008_0221f194();
            break;
        case 3:
            if (!SpriteManager::Get().IsFading()) {
                mState = 4;
            }
            break;
        case 4:
            draw = FALSE;
            RequestScene(1);
            mFinished = TRUE;
            break;
    }
    if (draw) {
        mResult = mWindow->Update(FALSE);
        Draw();
    }
}

void MyroomGiftScene::InitDisplay() {
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(0, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(3);
    G2S_SetBG0Priority(3);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_SetWnd0InsidePlane(0x1f, FALSE);
    G2_SetWnd1InsidePlane(0x1f, FALSE);
    G2_SetWndOBJInsidePlane(0x1f, FALSE);
    G2_SetWnd0Position(0, 0, 0, 0);
    G2_SetWnd1Position(0, 0, 0, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    G2_BlendNone();
    G2S_BlendNone();
}

// non-matching: size 0x6d8 vs 0x6d4: one extra instruction, and the SpriteManager/CellAnimation setup is ordered differently near +0xe4
void MyroomGiftScene::func_ov008_0221de34() {
    ChangeDir("/Game/Card/Graphic/");
    func_02025b64(0, 7, 0, "cardspec_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "cardspec_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 1, 0, "cardspec_u_bg_00_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/Myroom/");
    func_02025b64(1, 7, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.func_02027624(3);
    LoadEmblem(6);
    ChangeDir("/Game/Myroom/");
    main.LoadCharacters("myroom_gift_u_obj_00.nce_ncg1D.bin", 0x3000, 0);
    main.LoadPalette("myroom_gift_u_obj_00_ncl.bin", 0, 0);
    mCellAnim            = new CellAnimation("myroom_gift_u_obj_00_nce.bin", 0, 1, 1);
    mCellAnim->mCharBase = main.func_020276f4(0x3000);
    mCellAnim->mUnk_11   = 0;
    mCellAnim->SetAnimation(0);
    ChangeDir("/");
    mWindow = NULL;
    mWindow = new SelectWindow(0xe, data_ov008_02228b28, -1);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.func_02027624(3);
    ChangeDir("/");
    sub.LoadCharacters("Game/Myroom/myroom_gift_b_obj.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("Game/Myroom/myroom_gift_b_obj_ncl.bin", 0, 0);
    sub.LoadCharacters("Game/2D/union_b/union_b_obj_cockpit.nce_ncg1D.bin", 0x4000, 0);
    sub.LoadPalette("Game/2D/union_b/union_b_obj_cockpit_ncl.bin", 0x1a0, 0);
    sub.LoadCharacters("Game/2D/union_b/union_b_obj_page.nce_ncg1D.bin", 0x7000, 0);
    sub.LoadPalette("Game/2D/union_b/union_b_obj_page_ncl.bin", 0x1c0, 0);
    for (u8 i = 0; i < 10; i++) {
        if (mHasEmblem[i] != 0) {
            u8 value = data_ov008_02228aa0[i];
            *(u8 *) &data_ov008_0222a160[i].mUnk_04 = value;
            data_ov008_0222a160[i].mUnk_18[0] = value;
            data_ov008_0222a160[i].mUnk_18[1] = value;
            data_ov008_0222a160[i].mUnk_18[2] = value;
        } else {
            *(u8 *) &data_ov008_0222a160[i].mUnk_04 = 0xb;
            data_ov008_0222a160[i].mUnk_18[0] = 0xb;
            data_ov008_0222a160[i].mUnk_18[1] = 0xb;
            data_ov008_0222a160[i].mUnk_18[2] = 0xb;
        }
    }
    mWindow->SetCells(10, data_ov008_0222a160);
    mWindow->SetCells(4, data_ov008_02228ab8);
    ChangeDir("/font/");
    if (mMessages == NULL) {
        mMessages = (MessageFile *) func_02023504("mes_myroom.mes", 0, 0, 0, 0, 0);
    }
    for (u8 i = 0; i < 2; i++) {
        mTextBufferSize[i] = (u16) TextManager::Get().BufferSize((s16) data_ov008_02228a56[i][0], (s16) data_ov008_02228a56[i][1]);
        mGlyphCount[i]     = TextManager::Get().GlyphCount(data_ov008_02228a56[i][0], data_ov008_02228a56[i][1], data_ov008_02228a5c[i]);
        mTextBuffers[i]    = OS_Alloc(mTextBufferSize[i]);
        ClearTextBuffers(i);
        mGlyphBuffers[i] = OS_Alloc(mGlyphCount[i] * 0xc);
        TextManager::Get().Init(mGlyphBuffers[i], 0, 0, data_ov008_02228a56[i][0], data_ov008_02228a56[i][1], data_ov008_02228a5c[i]);
    }
    TextManager::Get().SetColor(0, 0, 0xf);
    mMessage = new MessageWindow("mes_myroom", 1, 0x6000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(0x22, 0, 1);
    func_ov008_0221e9a0();
    ChangeDir("/");
}

void MyroomGiftScene::InitEmblems() {
    MI_CpuFill8(mHasEmblem, 0, sizeof(mHasEmblem));
    for (u8 i = 0; i < 10; i++) {
        if (TestFlagBit(data_020d49a6, i)) {
            mHasEmblem[i] = 1;
        }
    }
}

void MyroomGiftScene::LoadEmblem(u8 index) {
    char name[64];
    if (mEmblem == index) {
        return;
    }
    mEmblem = index;
    if (mHasEmblem[mEmblem] == 0) {
        return;
    }
    ChangeDir("/Game/Myroom/");
    sprintf(name, "myroom_gift_u_obj_emblem_%02d.nce_ncg1D.bin", index + 1);
    func_02025b64(0, 4, 0, name, 0, 0, 0);
    sprintf(name, "myroom_gift_u_obj_emblem_%02d_ncl.bin", index + 1);
    func_02025b64(0, 0, 0, name, 0, 0, 0);
    ChangeDir("/");
    mUnk_55 = 0;
    mUnk_42 = 0x100;
}

void MyroomGiftScene::Draw() {
    func_ov008_0221e670();
    DrawWindows();
}

// non-matching: register allocation of the loop locals (sl/r6/r7 swapped, 27 words)
void MyroomGiftScene::func_ov008_0221e670() {
    if (mCellAnim != NULL) {
        mCellAnim->Update();
        mCellAnim->Draw(0, 0x80, 0x60, 0);
    }
    if (mHasEmblem[mEmblem] == 0) {
        return;
    }
    u8 row = 0;
    do {
        u32 base = row * 0x18;
        s16 y = row * 32 + 0x24;
        u8 col = 0;
        do {
            SpriteManager::Get().Screen(0).func_02027778(data_ov008_02228a70, 1, (s16) (col * 32 + 0xe), y, 0, (u16) (base + col * 8), 0, 0);
            col++;
        } while (col < 3);
        row++;
    } while (row < 4);
    SpriteManager::Get().Screen(0).func_02027778(mGlyphBuffers[0], 4, 0x70, 0x1e, 0, 0xa0, 0xf, 0);
    s16 x = 0x80 - ((u32) mUnk_40 >> 1);
    if (mUnk_40 > 0xf0) {
        x = mUnk_42;
        mUnk_42 = x - 1;
        if (mUnk_42 <= -mUnk_40) {
            mUnk_42 = 0x100;
        }
    }
    u8 first = 0;
    if (x < 0) {
        first = (-x) / 32;
    }
    u8 last = first + 9 - x / 32;
    if (last > 0x10) {
        last = 0x10;
    }
    u8 k = first;
    do {
        SpriteManager::Get().Screen(0).func_02027778((u8 *) mGlyphBuffers[1] + k * 0xc, 1, x, 8, 0, 0x100, 0xf, 0);
        k++;
    } while (k < last);
}

void MyroomGiftScene::DrawWindows() {
    mMessage->func_020658d0();
    mWindow->Draw();
}

void MyroomGiftScene::ClearTextBuffers(u8 index) {
    MI_CpuClear16(mTextBuffers[index], mTextBufferSize[index]);
}

// non-matching: mUnk_5b store vs guard-flag load scheduling, as in MyroomStatusScene::RenderLine (10 words, +0x198)
void MyroomGiftScene::func_ov008_0221e9a0() {
    ClearTextBuffers(0);
    TextManager &text = TextManager::Get();
    s16 width;
    s16 height;
    height            = 2 - mUnk_55 * 0x7e;
    width             = 0x10;
    u16 index         = mEmblem;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[0], 0x10, 0x12, &width, &height, 0, 0, 2, 0x44,
               0xe, 0, 0, 0, 1, 1);
    if (&mUnk_44 != NULL) {
        mUnk_44 = height;
    }
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x5000, mTextBuffers[0], mTextBufferSize[0], 0);
    ClearTextBuffers(1);
    TextManager::Get().mUnk_5b = 1;
    TextManager &text2         = TextManager::Get();
    s16 height2;
    s16 width2;
    width2     = 0;
    height2    = 0;
    u16 index2 = mEmblem + 11;
    file       = mMessages;
    text2.Print(file->GetText(index2), file->GetLength(index2), mTextBuffers[1], 0x40, 2, &height2, &width2, &mUnk_40, 0, 2,
                0x21, 0xe, 0, 0, 0, 1, 1);
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x8000, mTextBuffers[1], mTextBufferSize[1], 0);
    if (mHasEmblem[mEmblem] == 0) {
        mUnk_44 = 0;
    }
}

s32 MyroomGiftScene::func_ov008_0221ece8() {
    s32 result = -1;
    s8 previous = mUnk_55;
    if (mResult == 3) {
        s8 id = mWindow->GetSelectedId();
        if (id >= 0 && id <= 9) {
            result  = data_ov008_02228aac[id];
            mUnk_51 = result % 4;
            mUnk_52 = result / 4;
            if (data_ov008_02228a94[result] != mEmblem) {
                SoundManager::Get().func_020246f8(0);
            }
        } else {
            switch (id) {
                case 0xc:
                    if (previous > 0) {
                        mUnk_55--;
                        SoundManager::Get().func_020246f8(0);
                    }
                    break;
                case 0xd:
                    if (mUnk_44 > 0x80) {
                        mUnk_55++;
                        SoundManager::Get().func_020246f8(0);
                    }
                    break;
            }
        }
    }
    if (previous != mUnk_55) {
        func_ov008_0221e9a0();
    } else if (result == -1) {
        result = func_ov008_0221eec4();
    }
    return result;
}

s8 MyroomGiftScene::func_ov008_0221eec4() {
    s8 result  = -1;
    BOOL moved = FALSE;
    if (InputManager::Get().IsTrigger(0x40)) {
        moved = TRUE;
        if (mUnk_52 != 0) {
            mUnk_52--;
        } else {
            mUnk_52 = 2;
            if (data_ov008_02228a64[mUnk_52][mUnk_51] == 0) {
                mUnk_52--;
            }
        }
    } else if (InputManager::Get().IsTrigger(0x80)) {
        moved = TRUE;
        mUnk_52++;
        if (mUnk_52 >= 3) {
            mUnk_52 = 0;
        } else if (data_ov008_02228a64[mUnk_52][mUnk_51] == 0) {
            mUnk_52 = 0;
        }
    }
    if (InputManager::Get().IsTrigger(0x20)) {
        moved = TRUE;
        if (mUnk_51 != 0) {
            mUnk_51--;
        } else {
            mUnk_51 = 3;
            if (data_ov008_02228a64[mUnk_52][mUnk_51] == 0) {
                do {
                    mUnk_51--;
                } while (mUnk_51 != 0 && data_ov008_02228a64[mUnk_52][mUnk_51] == 0);
            }
        }
    } else if (InputManager::Get().IsTrigger(0x10)) {
        moved = TRUE;
        mUnk_51++;
        if (mUnk_51 >= 4) {
            mUnk_51 = 0;
        } else if (data_ov008_02228a64[mUnk_52][mUnk_51] == 0) {
            mUnk_51 = 0;
        }
    }
    if (moved) {
        result = mUnk_51 + mUnk_52 * 4;
        SoundManager::Get().func_020246f8(0);
    }
    return result;
}

void MyroomGiftScene::func_ov008_0221f194() {
    if (mResult == 2) {
        SpriteManager::Get().StartFade(3);
        SoundManager::Get().func_020246f8(1);
        mState = 3;
    } else {
        s32 selection = func_ov008_0221ece8();
        if (selection >= 0) {
            LoadEmblem(data_ov008_02228a94[selection]);
            func_ov008_0221e9a0();
            if (mWindow->mUnk_061 != 1) {
                mWindow->mUnk_061 = 4;
                mWindow->func_0205c7c8(-1);
            }
        }
    }
    WindowItem *items = mWindow->mCursor->mItems;
    if (mUnk_55 != 0) {
        if (mWindow->mCells[12]->mUnk_0e == 2) {
            mWindow->mCells[12]->SetAnimation(0);
            items[12].mFlags = 0x208;
        }
    } else {
        if (mWindow->mCells[12]->mUnk_0e != 2) {
            mWindow->mCells[12]->SetAnimation(2);
            items[12].mFlags = 0x8000;
        }
    }
    if (mUnk_44 > 0x80) {
        if (mWindow->mCells[13]->mUnk_0e == 5) {
            mWindow->mCells[13]->SetAnimation(3);
            items[13].mFlags = 0x408;
        }
    } else {
        if (mWindow->mCells[13]->mUnk_0e != 5) {
            mWindow->mCells[13]->SetAnimation(5);
            items[13].mFlags = 0x8000;
        }
    }
    Vec2s pos = {data_ov008_02228a7c[mUnk_52][mUnk_51 * 2], data_ov008_02228a7d[mUnk_52][mUnk_51 * 2]};
    mWindow->mCellPos[10] = pos;
}
