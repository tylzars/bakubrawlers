#include "Scene/MyroomCharaScene.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Game/UnkStruct_020d48f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Scene/MyroomStatusScene.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkFunctions.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern const WindowItemRecord data_ov008_022293e8[];
extern const WindowCellRecord data_ov008_02229378[];
extern const WindowCellRecord data_ov008_022292d4[];
extern const WindowCellRecord data_ov008_0222929c[];

extern const u32 data_ov008_022292b8[7];
extern const u8 data_ov008_0222934b[];
extern const u8 data_ov008_0222934d[];
extern u16 data_020d444e[];

extern const u8 data_ov008_0222930c[][21];

MyroomCharaScene::MyroomCharaScene() :
    SceneBase(2, 0) {
    mState      = 0;
    mTab        = 0;
    mUnk_12     = 0;
    mResult     = 0;
    mPage       = 0;
    mListCount  = 0;
    mMessage    = NULL;
    for (u8 i = 0; i < 2; i++) {
        mCells[i]   = NULL;
        mUnk_14[i]  = 0;
    }
    mCell = NULL;
    mUnk_1c = 0;
    for (u8 i = 0; i < 7; i++) {
        mTextBuffers[i] = NULL;
        mUnk_64[i]      = 0;
    }
    mNames       = NULL;
    mMessages    = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount  = 0;
}

MyroomCharaScene::~MyroomCharaScene() {
    Cleanup();
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().func_0202740c();
}

void MyroomCharaScene::Cleanup() {
    delete mWindow;
    mWindow = NULL;
    CellAnimation *cell = mCell;
    if (cell != NULL) {
        delete cell;
    }
    mCell = NULL;
    FreeCells();
    FreeText();
    delete mMessage;
    mMessage = NULL;
}

void MyroomCharaScene::FreeCells() {
    for (u8 i = 0; i < 2; i++) {
        CellAnimation *cell = mCells[i];
        if (cell != NULL) {
            delete cell;
        }
        mCells[i] = NULL;
    }
}

void MyroomCharaScene::Update() {
    switch (mState) {
        case 0:
            func_ov008_02225814();
            MI_CpuClearFast((void *) 0x6200000, 0x20000);
            InitDisplay();
            func_ov008_022242c0();
            mStatus                  = new MyroomStatusScene(0x15);
            mContext->mUnk_12        = 0;
            mWindow->mCursor->mSelection = 0;
            mWindow->mCells[0]->SetAnimation(2);
            mTab    = 0;
            mUnk_12 = 0;
            func_ov008_02225910();
            mWindow->mUnk_064 = 0;
            mState            = 1;
            return;
        case 1:
            SpriteManager::Get().StartFade(2);
            mState = 2;
            break;
        case 2:
            if (!SpriteManager::Get().IsFading()) {
                mWindow->mUnk_064 = 1;
                if (mWindow->mUnk_061 == 1) {
                    mState = 3;
                }
            }
            break;
        case 3: {
            s8 id  = mWindow->GetSelectedId();
            u8 tab = id;
            if (tab < 4 && tab != mTab) {
                mTab = tab;
                SpriteManager::Get().sFade[0] = 3;
                mState = 4;
                break;
            }
            if (mResult == 3) {
                if (tab == 4) {
                    mPage = (mPage + mPageCount - 1) % mPageCount;
                    func_ov008_022256f0();
                    func_ov008_02225910();
                } else if (tab == 5) {
                    mPage = (mPage + 1) % mPageCount;
                    func_ov008_022256f0();
                    func_ov008_02225910();
                }
            } else if (mResult == 2) {
                SpriteManager::Get().StartFade(3);
                mState = 0x11;
            }
            break;
        }
        case 4:
            mWindow->mUnk_064 = 0;
            if (SpriteManager::Get().sFade[0] <= 1) {
                mState = 5;
                if (mUnk_12 == 0) {
                    SetReady();
                }
            }
            break;
        case 5:
            if (mUnk_12 == 0) {
                if (mContext->mUnk_12 != 0) {
                    mStatus = NULL;
                    InitDisplay();
                    func_ov008_0222460c();
                    func_ov008_02224880();
                    mPage = 0;
                    func_ov008_02225910();
                    func_ov008_02224a5c();
                    func_ov008_022256f0();
                    SpriteManager::Get().sFade[0] = 2;
                    mState                     = 7;
                    mUnk_12                    = mTab;
                }
            } else {
                if (mTab == 0) {
                    FreeCells();
                    FreeText();
                    mStatus           = new MyroomStatusScene(0x15);
                    mContext->mUnk_12 = 0;
                    func_ov008_02225910();
                } else {
                    func_ov008_02224880();
                    mPage = 0;
                    func_ov008_02225910();
                    func_ov008_022256f0();
                }
                mState++;
                mUnk_12 = mTab;
            }
            break;
        case 6:
            SpriteManager::Get().sFade[0] = 2;
            mState++;
            break;
        case 7:
            if (SpriteManager::Get().sFade[0] <= 1) {
                mState = 8;
            }
            break;
        case 8:
            switch (mTab) {
                case 0:
                    mState = 0xc;
                    break;
                case 1:
                    mState = 0xd;
                    break;
                case 3:
                    mState = 0xe;
                    break;
                case 2:
                    mState = 0xf;
                    break;
            }
            break;
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            mWindow->mUnk_064 = 1;
            mState            = 3;
            break;
        case 17:
            if (mWindow->mUnk_061 == 4) {
                if (!SpriteManager::Get().IsFading()) {
                    SetReady();
                    mState = 0x12;
                }
            }
            break;
        case 18:
            if (mContext->mUnk_12 != 0) {
                RequestScene(1);
                mFinished = TRUE;
            }
            break;
    }
    mResult = mWindow->Update(TRUE);
    Draw();
}

void MyroomCharaScene::SetReady() {
    if (mStatus != NULL) {
        mStatus->SetReady();
    }
}

void MyroomCharaScene::InitDisplay() {
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(0, 1);
    GX_SetGraphicsMode(1, 0, 0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 0);
    G2_SetBG2Control(0, 1, 4, 8, 0);
    G2_SetBG3Control(0, 1, 6, 0xc, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 2, 0);
    G2S_SetBG2Control(0, 1, 4, 4, 0);
    G2S_SetBG3Control(0, 1, 6, 6, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
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

void MyroomCharaScene::func_ov008_022242c0() {
    ChangeDir("Game/Myroom/");
    func_02025b64(1, 7, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    mWindow = NULL;
    mWindow = new SelectWindow(7, data_ov008_022293e8, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.func_02027624(3);
    sub.LoadCharacters("myroom_status_b_obj_00.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("myroom_status_b_obj_00_ncl.bin", 0, 0);
    mWindow->SetCells(4, data_ov008_02229378);
    ChangeDir("/Game/2d/union_b/");
    sub.LoadCharacters("union_b_obj_page.nce_ncg1D.bin", 0x12000, 0);
    sub.LoadPalette("union_b_obj_page_ncl.bin", 0xa0, 0);
    mWindow->SetCells(2, data_ov008_022292d4);
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x10000, 0);
    sub.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x80, 0);
    mWindow->SetCells(1, data_ov008_0222929c);
    ChangeDir("/Game/Myroom/");
    mCell            = new CellAnimation("myroom_status_b_obj_00_nce.bin", 0, 1, 1);
    mCell->mCharBase = sub.func_020276f4(0);
    mCell->mUnk_11   = 0;
    mCell->SetAnimation(0);
    mUnk_1c = 1;
    ChangeDir("/");
    ChangeDir("/font/");
    mMessage = new MessageWindow("mes_myroom", 1, 0x13000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(0x27, 0, 1);
    ChangeDir("/");
}

void MyroomCharaScene::func_ov008_0222460c() {
    ChangeDir("/Game/Myroom/");
    func_02025b64(0, 7, 0, "story_bg_01_nsc.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "story_bg_01_ncg.bin", 0, 0, 0);
    func_02025b64(0, 1, 0, "story_bg_01_ncl.bin", 0, 0, 0);
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.func_02027624(3);
    main.func_02027750();
    main.LoadCharacters("myroom_status_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("myroom_status_u_obj_00_ncl.bin", 0, 0);
    main.LoadCharacters("myroom_status_u_obj_01.nce_ncg1D.bin", 0x9000, 0);
    main.LoadPalette("myroom_status_u_obj_01_ncl.bin", 0x80, 0);
    u8 i;
    for (i = 0; i == 0; i++) {
        mCells[i]            = new CellAnimation("myroom_status_u_obj_00_nce.bin", 0, 1, 1);
        mCells[i]->mCharBase = main.func_020276f4(0);
        mCells[i]->mUnk_11   = 0;
    }
    do {
        mCells[i]            = new CellAnimation("myroom_status_u_obj_01_nce.bin", 0, 1, 1);
        mCells[i]->mCharBase = main.func_020276f4(0x9000);
        mCells[i]->mUnk_11   = 4;
        i++;
    } while (i < 2);
    ChangeDir("/");
}

void MyroomCharaScene::func_ov008_02224880() {
    mUnk_14[0] = 1;
    mUnk_14[1] = 0;
    switch (mTab) {
        case 0:
            mUnk_14[0] = 0;
            break;
        case 1:
            mCells[0]->SetAnimation(0);
            break;
        case 2:
            mCells[0]->SetAnimation(1);
            mCells[1]->SetAnimation(func_ov008_02224a4c());
            mUnk_14[1] = 1;
            break;
        case 3:
            mCells[0]->SetAnimation(2);
            break;
    }
}

void MyroomCharaScene::Draw() {
    DrawWindow();
    DrawCells();
    DrawCell();
    mMessage->func_020658d0();
    DrawText();
}

void MyroomCharaScene::DrawWindow() {
    mWindow->Draw();
}

void MyroomCharaScene::DrawCells() {
    for (u8 i = 0; i < 2; i++) {
        if (mCells[i] != NULL && mUnk_14[i] != 0) {
            mCells[i]->Update();
            if (i == 1) {
                mCells[i]->Draw(0, 0x80, 0x60, 0);
            } else {
                mCells[i]->Draw(0, 0x80, 0x60, 1);
            }
        }
    }
}

void MyroomCharaScene::DrawCell() {
    if (mUnk_1c != 0 && mCell != NULL) {
        mCell->Update();
        mCell->Draw(1, 0x28, (s16) (mTab * 0x1c + 0x2c), 0);
    }
}

u8 MyroomCharaScene::func_ov008_02224a4c() {
    return data_020d43f0[0].mUnk_5b8;
}

void MyroomCharaScene::func_ov008_02224a5c() {
    ChangeDir("/font/");
    mNames    = (MessageFile *) func_02023504("mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
    mMessages = (MessageFile *) func_02023504("mes_myroom_status.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x20, 2);
    for (u8 i = 0; i < 7; i++) {
        mTextBuffers[i] = OS_Alloc(mTextBufferSize);
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
    }
    mGlyphCount  = TextManager::Get().GlyphCount(0x20, 2, 0x21);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    ClearTextBuffers();
}

void MyroomCharaScene::ClearTextBuffers() {
    for (u8 i = 0; i < 7; i++) {
        mUnk_64[i] = 0;
        if (mTextBuffers[i] != NULL) {
            MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
        }
    }
}

// non-matching: 11 words, entry block only (+0x8..+0x30). The original loads both stack arguments at the top
// (ldrsh r5, x; ldr r4, suffix); ours loads x into r4 and the scheduler sinks the suffix load to just before the
// first branch (+0x30), so x takes r4. In a small test function only a consumer of suffix inside the entry block
// (e.g. `unused = suffix;`) keeps the load at the top; parameter types, volatile or address-taken reads, an enum
// copy, double assignment, suffix/type mixes, declaration orders, array/struct forms of pos/unused, uses in later
// blocks and a store through a pointer in the entry block all leave it sunk. So the original's entry block must
// use suffix in an instruction that does not survive to the final code (a copy merged away, or similar).
void MyroomCharaScene::func_ov008_02224ca4(u32 line, u32 value, u8 digits, s16 x, s32 suffix) {
    s16 pos = x;
    BOOL special = FALSE;
    s16 unused;
    s32 type = suffix;
    unused       = 0;
    switch (mTab) {
        case 1:
            if (data_020d43f0[0].mUnk_48 + data_020d43f0[0].mUnk_4a == 0) {
                special = TRUE;
            }
            break;
        case 2:
            if (value == 0) {
                special = TRUE;
            }
            break;
    }
    if (special) {
        pos += (digits - 1) * 6;
        TextManager &text = TextManager::Get();
        s16 width;
        s16 height;
        height            = 0;
        width             = pos;
        MessageFile *file = mMessages;
        text.Print(file->GetText(7), file->GetLength(7), mTextBuffers[line], 0x20, 2, &width, &height, 0, 0, 2, 0x21, 0, 0, 0, 0,
                   1, 1);
        pos += 6;
        if (type != 4 && type != 5) {
            pos += 6;
        }
    } else {
        TextManager::Get().PrintNumber(value, digits, 0, mTextBuffers[line], 0x20, 2, &pos, &unused, 0, 2, 0x21, 0, 0, 1, 1);
        if (type != 4 && type != 5) {
            pos += 6;
        }
    }
    if (type == -1) {
        return;
    }
    TextManager &text2 = TextManager::Get();
    s16 width2;
    s16 height2;
    height2            = 0;
    width2             = pos;
    MessageFile *file2 = mMessages;
    u16 index          = type;
    text2.Print(file2->GetText(index), file2->GetLength(index), mTextBuffers[line], 0x20, 2, &width2, &height2, 0, 0, 2, 0x21, 0,
                0, 0, 0, 1, 1);
}

void MyroomCharaScene::func_ov008_02224f80(u32 line, u32 kind) {
    s32 value;
    s16 x     = 0;
    s32 ratio = 0;
    switch (kind) {
        case 0: {
            func_ov008_02224ca4(line, data_020d43f0[0].mUnk_48, 4, x, 2);
            x += 0x30;
            func_ov008_02224ca4(line, data_020d43f0[0].mUnk_4a, 4, x, 3);
            x += 0x2a;
            value = 0;
            if (data_020d43f0[0].mUnk_48 + data_020d43f0[0].mUnk_4a > 0) {
                value = data_020d43f0[0].mUnk_48 * 1000 / (data_020d43f0[0].mUnk_48 + data_020d43f0[0].mUnk_4a);
            }
            s32 rest = value % 10;
            if (rest == 0) {
                x += 0xc;
                func_ov008_02224ca4(line, value / 10, 3, x, 4);
            } else {
                TextManager::Get().mUnk_10 = rest;
                func_ov008_02224ca4(line, value / 10, 3, x, 5);
            }
            break;
        }
        case 1:
            func_ov008_02224ca4(line, data_020d43f0[0].mUnk_5a, 2, 0, -1);
            break;
        case 2:
            func_ov008_02224ca4(line, data_020d43f0[0].mUnk_5b, 2, 0, -1);
            break;
        case 3: {
            if (data_020d43f0[0].mUnk_4e != 0) {
                ratio = data_020d43f0[0].mUnk_50 * 1000 / data_020d43f0[0].mUnk_4e;
            }
            s32 rest = ratio % 10;
            if (rest == 0) {
                func_ov008_02224ca4(line, ratio / 10, 3, 0xc, 4);
            } else {
                TextManager::Get().mUnk_10 = rest;
                func_ov008_02224ca4(line, ratio / 10, 3, 0, 5);
            }
            break;
        }
        case 4:
            func_ov008_02224ca4(line, data_020d48f0.mUnk_d4, 4, 0, -1);
            break;
        case 5:
            func_ov008_02224ca4(line, data_020d43f0[0].mUnk_5c, 4, 0, -1);
            break;
        case 6:
            func_ov008_02224ca4(line, data_020d43f0[0].mUnk_52, 4, 0, -1);
            break;
        case 7:
            func_ov008_02224ca4(line, data_020d43f0[0].mUnk_80, 3, 0, 6);
            break;
        case 8:
            func_ov008_02224ca4(line, data_020d43f0[0].mUnk_84, 3, 0, 6);
            break;
        case 9:
            func_ov008_02224ca4(line, data_020d43f0[0].mUnk_82, 3, 0, 6);
            break;
        case 10: {
            TextManager::Get().mUnk_5b = 1;
            TextManager &text          = TextManager::Get();
            s16 width;
            s16 height;
            height    = 0;
            width     = 0;
            u8 index  = mList[mPage * 7 + line];
            MessageFile *file = mNames;
            text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[line], 0x20, 2, &width, &height, 0, 0, 2, 0x21, 0, 0,
                       0, 0, 1, 1);
            TextManager::Get().mUnk_5b = 0;
            func_ov008_02224ca4(line, data_020d444e[mList[mPage * 7 + line]], 3,
                                (s16) (data_ov008_0222934d[line * 3] - data_ov008_0222934b[line * 3]), 6);
            break;
        }
    }
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, data_ov008_022292b8[line], mTextBuffers[line], mTextBufferSize, 0);
    mUnk_64[line] = 1;
}

void MyroomCharaScene::DrawText() {
    for (u8 i = 0; i < 7; i++) {
        if (mUnk_64[i] != 0) {
            SpriteManager::Get().Screen(0).func_02027778(mGlyphBuffer, mGlyphCount, data_ov008_0222930c[mUnk_12][i * 3],
                                                         data_ov008_0222930c[mUnk_12][i * 3 + 1], 0,
                                                         (u16) (data_ov008_022292b8[i] >> 7), 0xf, 0);
        }
    }
}

void MyroomCharaScene::FreeText() {
    u8 i = 0;
    ClearTextBuffers();
    for (; i < 7; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
    }
    if (mNames != NULL) {
        OS_Free(mNames);
        mNames = NULL;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
        mMessages = NULL;
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
}

void MyroomCharaScene::func_ov008_022256f0() {
    ClearTextBuffers();
    switch (mTab) {
        case 0:
            break;
        case 1:
            func_ov008_02224f80(0, 0);
            func_ov008_02224f80(1, 1);
            func_ov008_02224f80(2, 2);
            func_ov008_02224f80(3, 3);
            func_ov008_02224f80(4, 4);
            func_ov008_02224f80(5, 5);
            func_ov008_02224f80(6, 6);
            break;
        case 2:
            func_ov008_02224f80(0, 7);
            func_ov008_02224f80(1, 8);
            func_ov008_02224f80(2, 9);
            break;
        case 3: {
            if (mListCount == 0) {
                return;
            }
            u8 i = 0;
            do {
                func_ov008_02224f80(i, 10);
                i++;
            } while (i < 7 && i + mPage * 7 < mListCount);
            break;
        }
    }
}

void MyroomCharaScene::func_ov008_02225814() {
    s16 values[17];
    u8 ids[17];
    u8 best;
    u8 k;
    u8 i;
    u8 j;
    s16 max;
    mListCount = 0;
    for (i = 0; i < 0x11; i++) {
        if (i != 5 && data_020d43f0[0].mUnk_5e[i] != 0) {
            ids[mListCount]    = i;
            values[mListCount] = data_020d43f0[0].mUnk_5e[i];
            mListCount++;
        }
    }
    j    = 0;
    best = 0;
    do {
        k   = 0;
        max = -1;
        do {
            if (values[k] > max) {
                max  = values[k];
                best = k;
            }
            k++;
        } while (k < mListCount);
        mList[j]     = ids[best];
        values[best] = -1;
        j++;
    } while (j < mListCount);
    mPageCount = (mListCount - 1) / 7 + 1;
}

void MyroomCharaScene::func_ov008_02225910() {
    WindowItem *items = mWindow->mCursor->mItems;
    if (mTab == 3) {
        if (mPageCount == 1) {
            mWindow->mCells[4]->SetAnimation(2);
            mWindow->mCells[5]->SetAnimation(5);
            mWindow->mButtons[4].mEnabled = 0;
            mWindow->mButtons[5].mEnabled = 0;
            items[4].mFlags               = 0x8000;
            items[5].mFlags               = 0x8000;
            return;
        }
        if (mPage != 0) {
            mWindow->mCells[4]->SetAnimation(0);
            mWindow->mButtons[4].mEnabled = 1;
            items[4].mFlags               = 0x208;
        } else {
            mWindow->mCells[4]->SetAnimation(2);
            mWindow->mButtons[4].mEnabled = 0;
            items[4].mFlags               = 0x8000;
        }
        if (mPage < mPageCount - 1) {
            mWindow->mCells[5]->SetAnimation(3);
            mWindow->mButtons[5].mEnabled = 1;
            items[5].mFlags               = 0x408;
        } else {
            mWindow->mCells[5]->SetAnimation(5);
            mWindow->mButtons[5].mEnabled = 0;
            items[5].mFlags               = 0x8000;
        }
    } else {
        mWindow->mCells[4]->SetAnimation(2);
        mWindow->mCells[5]->SetAnimation(5);
        mWindow->mButtons[4].mEnabled = 0;
        mWindow->mButtons[5].mEnabled = 0;
        items[4].mFlags               = 0x8000;
        items[5].mFlags               = 0x8000;
    }
}
