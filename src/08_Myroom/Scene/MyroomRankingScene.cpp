#include "UI/SelectWindow.hpp"
const u8 data_ov008_02228cb0[6][2] = {{0x10, 0x2e}, {0x10, 0x42}, {0x10, 0x56}, {0x10, 0x6a}, {0x10, 0x7e}, {0x10, 0x92}};

const WindowCellRecord data_ov008_02228db0[9] = {
    {"Game/Myroom/myroom_ranking_b_obj_00_nce.bin", 6, 0, 0, 0, 1, {6, 6, 6, 0}},
    {"Game/Myroom/myroom_ranking_b_obj_00_nce.bin", 4, 0, 0, 1, 1, {4, 4, 4, 0}},
    {"Game/Myroom/myroom_ranking_b_obj_00_nce.bin", 4, 0, 0, 1, 1, {4, 4, 4, 0}},
    {"Game/Myroom/myroom_ranking_b_obj_00_nce.bin", 4, 0, 0, 1, 1, {4, 4, 4, 0}},
    {"Game/Myroom/myroom_ranking_b_obj_00_nce.bin", 4, 0, 0, 1, 1, {4, 4, 4, 0}},
    {"Game/Myroom/myroom_ranking_b_obj_00_nce.bin", 4, 0, 0, 1, 1, {4, 4, 4, 0}},
    {"Game/Myroom/myroom_ranking_b_obj_00_nce.bin", 0, 0, 0, 1, 1, {0, 0, 0, 0}},
    {"Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 49152, 12, 0, 1, {2, 3, 3, 0}},
    {"Game/Myroom/myroom_ranking_b_obj_00_nce.bin", 7, 0, 0, 0, 1, {7, 7, 7, 0}},
};

const WindowCellRecord data_ov008_02228cc8[4] = {
    {"union_b_obj_scroll_nce.bin", 4, 24576, 10, 0, 1, {4, 4, 4, 0}},
    {"union_b_obj_scroll_nce.bin", 0, 24576, 10, 0, 1, {0, 1, 1, 0}},
    {"union_b_obj_scroll_nce.bin", 2, 24576, 10, 0, 1, {2, 3, 3, 0}},
    {"union_b_obj_scroll_nce.bin", 7, 24576, 10, 0, 1, {7, 7, 7, 0}},
};

const WindowItemRecord data_ov008_02228eac[13] = {
    {0, 0x8000, 120, 30, 0, 0, 0x0, {-1, -1, -1, -1}, 0, 0},
    {1, 0x8, 16, 46, 208, 16, 0x0, {-1, -1, -1, -1}, 0, 1},
    {2, 0x8, 16, 66, 208, 16, 0x0, {-1, -1, -1, -1}, 0, 1},
    {3, 0x8, 16, 86, 208, 16, 0x0, {-1, -1, -1, -1}, 0, 1},
    {4, 0x8, 16, 106, 208, 16, 0x0, {-1, -1, -1, -1}, 0, 1},
    {5, 0x8, 16, 126, 208, 16, 0x0, {-1, -1, -1, -1}, 0, 1},
    {6, 0x8, 16, 146, 208, 16, 0x0, {-1, -1, -1, -1}, 0, 1},
    {7, 0x2, 225, 170, 30, 20, 0x0, {-1, -1, -1, -1}, 0, 1},
    {8, 0x8000, 16, 36, 0, 0, 0x0, {-1, -1, -1, -1}, 0, 0},
    {9, 0x10, 228, 63, 32, 12, 0x0, {-1, -1, -1, -1}, 0, 0},
    {10, 0x20, 228, 44, 32, 20, 0x0, {-1, -1, -1, -1}, 0, 3},
    {11, 0x40, 228, 144, 32, 20, 0x0, {-1, -1, -1, -1}, 0, 3},
    {12, 0x8000, 238, 64, 0, 0, 0x0, {-1, -1, -1, -1}, 0, 0},
};

// Position (x, y) of each digit cell, per ranking row, as passed to DrawFrame.
struct RankingDigitPos {
    /* 0 */ u8 mX;
    /* 1 */ u8 mY;
};
const RankingDigitPos data_ov008_02228d38[6][10] = {
    {{46, 50}, {38, 50}, {30, 50}, {208, 50}, {202, 50}, {196, 50}, {190, 50}, {184, 50}, {178, 50}, {172, 50}},
    {{46, 70}, {38, 70}, {30, 70}, {208, 70}, {202, 70}, {196, 70}, {190, 70}, {184, 70}, {178, 70}, {172, 70}},
    {{46, 90}, {38, 90}, {30, 90}, {208, 90}, {202, 90}, {196, 90}, {190, 90}, {184, 90}, {178, 90}, {172, 90}},
    {{46, 110}, {38, 110}, {30, 110}, {208, 110}, {202, 110}, {196, 110}, {190, 110}, {184, 110}, {178, 110}, {172, 110}},
    {{46, 130}, {38, 130}, {30, 130}, {208, 130}, {202, 130}, {196, 130}, {190, 130}, {184, 130}, {178, 130}, {172, 130}},
    {{46, 150}, {38, 150}, {30, 150}, {208, 150}, {202, 150}, {196, 150}, {190, 150}, {184, 150}, {178, 150}, {172, 150}},
};

const u8 data_ov008_02228cbc[6][2] = {{0x37, 0x31}, {0x37, 0x45}, {0x37, 0x59}, {0x37, 0x6d}, {0x37, 0x81}, {0x37, 0x95}};

#include "Text/TextManager.hpp"
#include "Scene/MyroomRankingScene.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Game/Ranking.hpp"
#include "Scene/RankingCharacterView.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

// Selects the animation of each visible row's cell from its rank.
inline void MyroomRankingScene::UpdateRowAnimations() {
    for (u8 i = 0; i < 6; i++) {
        switch (mRows[i].mUnk_2a) {
            case 1:
                mWindow->mCells[(u8) (i + 1)]->SetAnimation(1);
                break;
            case 2:
                mWindow->mCells[(u8) (i + 1)]->SetAnimation(2);
                break;
            case 3:
                mWindow->mCells[(u8) (i + 1)]->SetAnimation(3);
                break;
            default:
                if ((s32) mRows[i].mUnk_2a > 10) {
                    mWindow->mCells[(u8) (i + 1)]->SetAnimation(0);
                } else {
                    mWindow->mCells[(u8) (i + 1)]->SetAnimation(4);
                }
                break;
        }
    }
}

MyroomRankingScene::MyroomRankingScene() :
    SceneBase(1000, 0) {
    u8 j = 0;
    u8 i = 0;
    mUnk_0c   = NULL;
    mUnk_10   = NULL;
    mResult   = 0;
    mMessages = NULL;
    mMessage  = NULL;
    mUnk_1b8  = 0;
    mUnk_1b9  = 0;
    mUnk_1ba  = 0;
    mUnk_1bb  = 0;
    mUnk_1bc  = -1;
    mUnk_1bd  = 0;
    mUnk_1bf  = 0;
    mUnk_1c0  = 0;
    for (; i < 6; i++) {
        for (j = 0; j < 10; j++) {
            mRows[i].mCells[j] = NULL;
        }
    }
    mUnk_42  = mRows[0].mUnk_2a;
    mUnk_44  = mRows[0].mUnk_2c;
    mUnk_40  = mRows[0].mUnk_28;
    mUnk_16a = mRows[0].mUnk_2a;
    mUnk_16c = mRows[0].mUnk_2c;
    mUnk_168 = mRows[0].mUnk_28;
    for (i = 0; i < 6; i++) {
        mTextBuffers[i]  = NULL;
        mGlyphBuffers[i] = NULL;
    }
    mTextBufferSize = 0;
    mGlyphCount     = 0;
}

MyroomRankingScene::~MyroomRankingScene() {
    u8 j;
    u8 i;
    CellAnimation *cell;
    delete mUnk_0c;
    delete mWindow;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 10; j++) {
            cell = mRows[i].mCells[j];
            if (cell != NULL) {
                delete cell;
            }
        }
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    ClearGlyphBuffers();
    for (i = 0; i < 6; i++) {
        OS_Free(mTextBuffers[i]);
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
        }
    }
    delete mMessage;
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().func_0202740c();
}

void MyroomRankingScene::Update() {
    BOOL draw = TRUE;
    switch (mUnk_1b8) {
        case 0:
            draw = FALSE;
            InitDisplay();
            func_ov008_0221fe9c();
            mUnk_1bf = 10;
            mUnk_1b8++;
            break;
        case 1:
            draw = FALSE;
            mUnk_1bf--;
            if (mUnk_1bf > 0) {
                break;
            }
            SpriteManager::Get().StartFade(2);
            mUnk_1b8++;
            break;
        case 2:
            if (mUnk_1c0 != 0) {
                SpriteManager::Get().StartFade(3);
                mUnk_10->mFinishRequested = 1;
                mUnk_1b8         = 4;
            } else {
                if (func_ov008_02220848() != -1) {
                    s32 index = mUnk_1b9 + mUnk_1ba;
                    if (index > 10) {
                        mUnk_1bb = 10;
                    } else {
                        mUnk_1bb = index;
                    }
                    RankingEntry *entry;
                    if (mUnk_1bb >= 10) {
                        entry = (RankingEntry *) ((u8 *) mUnk_0c + 0x8c);
                    } else {
                        entry = mUnk_0c->GetEntry(mUnk_1bb);
                    }
                    mUnk_168 = entry->mCharacterId;
                    mUnk_16a = entry->mRank;
                    mUnk_16c = entry->mScore;
                    if (mUnk_168 != mUnk_10->mCharacterId) {
                        mUnk_10->ChangeCharacter(mUnk_168, mUnk_16a, mUnk_16c);
                        mUnk_1b8 = 3;
                    }
                }
            }
            func_ov008_02220b88();
            break;
        case 3:
            if (mUnk_10->mChanging == 0) {
                mUnk_1b8 = 2;
            }
            break;
        case 4:
            if (mWindow->mState == 4) {
                if (!SpriteManager::Get().IsFading()) {
                    mUnk_1b8 = 5;
                }
            }
            break;
        case 5:
            draw = FALSE;
            RequestScene(1);
            mFinished = TRUE;
            break;
    }
    if (draw) {
        u8 scroll = mWindow->mCursor->mScroll;
        mResult   = mWindow->Update(FALSE);
        if (scroll != mWindow->mCursor->mScroll) {
            switch (mWindow->mCursor->mUnk_05) {
                case 10:
                case 11:
                    SoundManager::Get().StartSeqArc(5);
                    break;
            }
        }
        if (mResult != 2) {
            UpdateRowAnimations();
        } else {
            SoundManager::Get().StartSeqArc(1);
            mUnk_1c0 = 1;
        }
        func_ov008_022205ac();
    }
}

void MyroomRankingScene::InitDisplay() {
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(0, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
    GX_SetVisiblePlane(0x1f);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
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

void MyroomRankingScene::func_ov008_0221fe9c() {
    Ranking *list = new Ranking();
    mUnk_0c                  = list;
    mUnk_40                  = list->mPlayerCharacterId;
    mUnk_42                  = list->mPlayerRank;
    mUnk_44                  = list->mPlayerScore;
    for (u8 i = 0; i < 6; i++) {
        RankingEntry *entry = mUnk_0c->GetEntry(i);
        mRows[i].mUnk_28                = entry->mCharacterId;
        mRows[i].mUnk_2a                = entry->mRank;
        mRows[i].mUnk_2c                = entry->mScore;
    }
    mUnk_168 = mUnk_40;
    mUnk_16a = mUnk_42;
    mUnk_16c = mUnk_44;
    mUnk_10  = new RankingCharacterView(mUnk_168, mUnk_16a, mUnk_16c, 0);
    ChangeDir("/Game/Myroom/");
    LoadAndQueueUpload(1, 7, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    mWindow = NULL;
    mWindow = new SelectWindow(0xd, data_ov008_02228eac, -1);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    ChangeDir("/");
    sub.LoadCharacters("Game/Myroom/myroom_ranking_b_obj_00.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("Game/Myroom/myroom_ranking_b_obj_00_ncl.bin", 0, 0);
    sub.LoadCharacters("Game/2D/union_b/union_b_obj_cockpit.nce_ncg1D.bin", 0xc000, 0);
    sub.LoadPalette("Game/2D/union_b/union_b_obj_cockpit_ncl.bin", 0x180, 0);
    mWindow->SetCells(9, data_ov008_02228db0);
    ChangeDir("/Game/2d/union_b/");
    sub.LoadCharacters("union_b_obj_scroll.nce_ncg1D.bin", 0x6000, 0);
    sub.LoadPalette("union_b_obj_scroll_ncl.bin", 0x140, 0);
    mWindow->SetCells(4, data_ov008_02228cc8);
    mUnk_1bd = 10;
    if (mUnk_42 > 10) {
        mUnk_1bd++;
    }
    mWindow->mCursor->SetupScroll(0x40, 0x90, 6, mUnk_1bd);
    sub.LoadCharacters("union_b_obj_number.nce_ncg1D.bin", 0x7000, 0);
    sub.LoadPalette("union_b_obj_number_ncl.bin", 0x160, 0);
    for (u8 i = 0; i < 6; i++) {
        for (u8 j = 0; j < 10; j++) {
            mRows[i].mCells[j]            = new CellAnimation("union_b_obj_number_nce.bin", 0, 1, 1);
            mRows[i].mCells[j]->mCharBase = sub.GetCharBase(0x7000);
            mRows[i].mCells[j]->mPaletteBase   = 11;
            mRows[i].mCells[j]->SetAnimation(0);
            mRows[i].mCells[j]->SetMode(5);
        }
    }
    ChangeDir("/font/");
    mMessages       = (MessageFile *) LoadFile("mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x10, 4);
    mGlyphCount     = TextManager::Get().GlyphCount(2);
    for (u8 i = 0; i < 6; i++) {
        mTextBuffers[i] = OS_Alloc(mTextBufferSize);
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
        mGlyphBuffers[i] = OS_Alloc(mGlyphCount * 0xc);
        TextManager::Get().Init(mGlyphBuffers[i], 1, 0, 0x10, 4, 0x42);
    }
    TextManager::Get().SetColor(1, 0, 0xf);
    mMessage = new MessageWindow("mes_myroom", 1, 0xb000);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->SetWidth(0xd8);
    mMessage->StartMessage(0x28, 0, 1);
    if ((s32) mUnk_42 > 10) {
        mUnk_1b9 = mUnk_1bd - 6;
        mUnk_1ba = 5;
    } else if ((s32) mUnk_42 <= 4) {
        mUnk_1b9 = 0;
        mUnk_1ba = mUnk_42 - 1;
    } else if ((s32) mUnk_42 >= 8) {
        mUnk_1b9 = mUnk_1bd - 6;
        mUnk_1ba = mUnk_42 - 5;
    } else {
        mUnk_1b9 = mUnk_42 - 4;
        mUnk_1ba = 3;
    }
    Vec2s pos = {data_ov008_02228cb0[mUnk_1ba][0], data_ov008_02228cb0[mUnk_1ba][1]};
    mWindow->mCellPos[8] = pos;
    mWindow->mCursor->SetScroll(mUnk_1b9);
    if ((s32) mUnk_42 > 10) {
        mUnk_1bb = 10;
    } else {
        mUnk_1bb = mUnk_1b9 + mUnk_1ba;
    }
    ChangeDir("/");
}

void MyroomRankingScene::func_ov008_022205ac() {
    func_ov008_022205b8();
}

void MyroomRankingScene::func_ov008_022205b8() {
    for (u8 i = 0; i < 6; i++) {
        u8 y        = data_ov008_02228cbc[i][1];
        u8 x        = data_ov008_02228cbc[i][0];
        u32 base    = (i << 11) + 0x8000;
        for (u8 j = 0; j < 2; j++) {
            SpriteManager::Get().Screen(1).AddOam(mGlyphBuffers[i], 1,
                                                         (s16) (x + (j % 2) * 64), y, 0, (u16) (j * 8 + (base >> 7)), 0xf, 0);
        }
    }
    for (u8 i = 0; i < 6; i++) {
        u16 a;
        u32 b;
        u8 digit;
        a             = mRows[i].mUnk_2a;
        b             = mRows[i].mUnk_2c;
        for (u8 j = 0; j < 10; j++) {
            digit = 0;
            if (mRows[i].mCells[j] == NULL) {
                continue;
            }
            if (j < 3) {
                if (a != 0) {
                    digit = a % 10;
                    a     = a / 10;
                } else if (j != 0) {
                    continue;
                }
            } else {
                if (b != 0) {
                    digit = b % 10;
                    b     = b / 10;
                } else if (j != 3) {
                    continue;
                }
            }
            mRows[i].mCells[j]->Update();
            mRows[i].mCells[j]->DrawFrame(1, digit, data_ov008_02228d38[i][j].mX, data_ov008_02228d38[i][j].mY, 0);
        }
    }
    SelectWindow *window = mWindow;
    u8 cy = window->mCursor->mScrollItemY;
    window->mCellPos[9].x = 0xe4;
    window->mCellPos[9].y = cy;
    mWindow->Draw();
    mMessage->Update();
}

s32 MyroomRankingScene::func_ov008_02220848() {
    s32 result = -1;
    if (mResult == 3) {
        s8 id = mWindow->GetSelectedId();
        switch (id) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6: {
                s32 row = id - 1;
                if (mUnk_1ba != row) {
                    SoundManager::Get().StartSeqArc(0);
                }
                mUnk_1ba = row;
                result   = (s8) (mUnk_1b9 + mUnk_1ba);
                break;
            }
            case 10:
            case 11:
                SoundManager::Get().StartSeqArc(5);
                break;
        }
    }
    if (result == -1) {
        result = func_ov008_022209d4();
        if (result == -1) {
            s32 current = mUnk_1b9 + mUnk_1ba;
            if (current > 10) {
                if (mUnk_1bb != 10) {
                    result = 10;
                }
            } else if (mUnk_1bb != current) {
                result = (s8) current;
            }
        }
    }
    return result;
}

s32 MyroomRankingScene::func_ov008_022209d4() {
    s32 result;
    BOOL scrolled;
    BOOL moved;
    u8 scroll;
    scroll   = mWindow->mCursor->mScroll;
    result   = -1;
    scrolled = FALSE;
    moved    = TRUE;
    if (InputManager::Get().IsTrigger(KEY_UP)) {
        if (mUnk_1ba == 0) {
            if (scroll == 0) {
                moved = FALSE;
            } else {
                scroll--;
                scrolled = TRUE;
            }
        } else {
            mUnk_1ba--;
        }
    } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
        if (mUnk_1ba == 5) {
            if (scroll >= mUnk_1bd - 6) {
                moved = FALSE;
            } else {
                scroll++;
                scrolled = TRUE;
            }
        } else {
            mUnk_1ba++;
        }
    } else {
        moved = FALSE;
    }
    if (scrolled) {
        mWindow->mCursor->SetScroll(scroll);
    }
    if (moved) {
        result = (s8) (scroll + mUnk_1ba);
        SoundManager::Get().StartSeqArc(0);
    }
    return result;
}

void MyroomRankingScene::func_ov008_02220b88() {
    mUnk_1b9 = mWindow->mCursor->mScroll;
    for (u8 i = 0; i < 6; i++) {
        RankingEntry *entry;
        s32 index = mUnk_1b9 + i;
        if (index >= 10) {
            entry = (RankingEntry *) ((u8 *) mUnk_0c + 0x8c);
        } else {
            entry = mUnk_0c->GetEntry((u8) index);
        }
        mRows[i].mUnk_28 = entry->mCharacterId;
        mRows[i].mUnk_2a = entry->mRank;
        mRows[i].mUnk_2c = entry->mScore;
    }
    UpdateRowAnimations();
    Vec2s pos = {data_ov008_02228cb0[mUnk_1ba][0], data_ov008_02228cb0[mUnk_1ba][1]};
    mWindow->mCellPos[8] = pos;
    func_ov008_02220d14();
}

void MyroomRankingScene::func_ov008_02220d14() {
    u8 i = 0;
    if (mUnk_1bc == mUnk_1b9) {
        return;
    }
    mUnk_1bc = mUnk_1b9;
    do {
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
        TextManager::Get().mUse4bppFont = 1;
        if (mRows[i].mUnk_28 == 0x15) {
            s16 width;
            s16 height;
            width  = 0;
            height = 0;
            TextManager::Get().Print(gSaveData[0].mName, 0x18, mTextBuffers[i], 0x10, 4, &width, &height, 0, 0, 2, 0x42, 0,
                                     0, 0, 0, 1, 1);
        } else {
            TextManager &text = TextManager::Get();
            s16 height;
            s16 width;
            width             = 0;
            height            = 0;
            MessageFile *file = mMessages;
            u8 index          = mRows[i].mUnk_28;
            text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[i], 0x10, 4, &height, &width, 0, 0, 2, 0x42,
                       0, 0, 0, 0, 1, 1);
        }
        TextManager::Get().mUse4bppFont = 0;
        QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x8000 + i * 0x800, mTextBuffers[i], mTextBufferSize, 0);
        i++;
    } while (i < 6);
}

void MyroomRankingScene::ClearGlyphBuffers() {
    for (u8 i = 0; i < 6; i++) {
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
    }
}
