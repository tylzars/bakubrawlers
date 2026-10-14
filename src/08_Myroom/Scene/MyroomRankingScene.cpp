#include "Text/TextManager.hpp"
#include "Scene/MyroomRankingScene.hpp"
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
#include "Unknown/UnkStruct_020758f4.hpp"
#include "Unknown/UnkStruct_02074938.hpp"

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

extern const u8 data_ov008_02228cb0[];
extern const u8 data_ov008_02228cb1[];
extern const u8 data_ov008_02228cbc[][2];
extern const u8 data_ov008_02228d38[][20];
extern const WindowItemRecord data_ov008_02228eac[];
extern const WindowCellRecord data_ov008_02228db0[];
extern const WindowCellRecord data_ov008_02228cc8[];

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
                mUnk_10->mUnk_84 = 1;
                mUnk_1b8         = 4;
            } else {
                if (func_ov008_02220848() != -1) {
                    s32 index = mUnk_1b9 + mUnk_1ba;
                    if (index > 10) {
                        mUnk_1bb = 10;
                    } else {
                        mUnk_1bb = index;
                    }
                    UnkStruct_020758f4_Entry *entry;
                    if (mUnk_1bb >= 10) {
                        entry = (UnkStruct_020758f4_Entry *) ((u8 *) mUnk_0c + 0x8c);
                    } else {
                        entry = mUnk_0c->func_02075a00(mUnk_1bb);
                    }
                    mUnk_168 = entry->mUnk_0;
                    mUnk_16a = entry->mUnk_2;
                    mUnk_16c = entry->mUnk_4;
                    if (mUnk_168 != mUnk_10->mUnk_58) {
                        mUnk_10->func_02074938(mUnk_168, mUnk_16a, mUnk_16c);
                        mUnk_1b8 = 3;
                    }
                }
            }
            func_ov008_02220b88();
            break;
        case 3:
            if (mUnk_10->mUnk_80 == 0) {
                mUnk_1b8 = 2;
            }
            break;
        case 4:
            if (mWindow->mUnk_061 == 4) {
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
                    SoundManager::Get().func_020246f8(5);
                    break;
            }
        }
        if (mResult != 2) {
            UpdateRowAnimations();
        } else {
            SoundManager::Get().func_020246f8(1);
            mUnk_1c0 = 1;
        }
        func_ov008_022205ac();
    }
}

void MyroomRankingScene::InitDisplay() {
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(0, 7);
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
    UnkStruct_020758f4 *list = new UnkStruct_020758f4();
    mUnk_0c                  = list;
    mUnk_40                  = list->mUnk_8c;
    mUnk_42                  = list->mUnk_8e;
    mUnk_44                  = list->mUnk_90;
    for (u8 i = 0; i < 6; i++) {
        UnkStruct_020758f4_Entry *entry = mUnk_0c->func_02075a00(i);
        mRows[i].mUnk_28                = entry->mUnk_0;
        mRows[i].mUnk_2a                = entry->mUnk_2;
        mRows[i].mUnk_2c                = entry->mUnk_4;
    }
    mUnk_168 = mUnk_40;
    mUnk_16a = mUnk_42;
    mUnk_16c = mUnk_44;
    mUnk_10  = new UnkStruct_02074938(mUnk_168, mUnk_16a, mUnk_16c, 0);
    ChangeDir("/Game/Myroom/");
    func_02025b64(1, 7, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    mWindow = NULL;
    mWindow = new SelectWindow(0xd, data_ov008_02228eac, -1);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.func_02027624(3);
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
    mWindow->mCursor->func_02030474(0x40, 0x90, 6, mUnk_1bd);
    sub.LoadCharacters("union_b_obj_number.nce_ncg1D.bin", 0x7000, 0);
    sub.LoadPalette("union_b_obj_number_ncl.bin", 0x160, 0);
    for (u8 i = 0; i < 6; i++) {
        for (u8 j = 0; j < 10; j++) {
            mRows[i].mCells[j]            = new CellAnimation("union_b_obj_number_nce.bin", 0, 1, 1);
            mRows[i].mCells[j]->mCharBase = sub.func_020276f4(0x7000);
            mRows[i].mCells[j]->mUnk_11   = 11;
            mRows[i].mCells[j]->SetAnimation(0);
            mRows[i].mCells[j]->func_02029fb8(5);
        }
    }
    ChangeDir("/font/");
    mMessages       = (MessageFile *) func_02023504("mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
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
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(0x28, 0, 1);
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
    Vec2s pos = {data_ov008_02228cb0[mUnk_1ba * 2], data_ov008_02228cb1[mUnk_1ba * 2]};
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
            SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffers[i], 1,
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
            mRows[i].mCells[j]->func_0202a3ec(1, digit, data_ov008_02228d38[i][j * 2], data_ov008_02228d38[i][j * 2 + 1], 0);
        }
    }
    SelectWindow *window = mWindow;
    u8 cy = window->mCursor->mUnk_0b;
    window->mCellPos[9].x = 0xe4;
    window->mCellPos[9].y = cy;
    mWindow->Draw();
    mMessage->func_020658d0();
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
                    SoundManager::Get().func_020246f8(0);
                }
                mUnk_1ba = row;
                result   = (s8) (mUnk_1b9 + mUnk_1ba);
                break;
            }
            case 10:
            case 11:
                SoundManager::Get().func_020246f8(5);
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
    if (InputManager::Get().IsTrigger(0x40)) {
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
    } else if (InputManager::Get().IsTrigger(0x80)) {
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
        SoundManager::Get().func_020246f8(0);
    }
    return result;
}

void MyroomRankingScene::func_ov008_02220b88() {
    mUnk_1b9 = mWindow->mCursor->mScroll;
    for (u8 i = 0; i < 6; i++) {
        UnkStruct_020758f4_Entry *entry;
        s32 index = mUnk_1b9 + i;
        if (index >= 10) {
            entry = (UnkStruct_020758f4_Entry *) ((u8 *) mUnk_0c + 0x8c);
        } else {
            entry = mUnk_0c->func_02075a00((u8) index);
        }
        mRows[i].mUnk_28 = entry->mUnk_0;
        mRows[i].mUnk_2a = entry->mUnk_2;
        mRows[i].mUnk_2c = entry->mUnk_4;
    }
    UpdateRowAnimations();
    Vec2s pos = {data_ov008_02228cb0[mUnk_1ba * 2], data_ov008_02228cb1[mUnk_1ba * 2]};
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
        TextManager::Get().mUnk_5b = 1;
        if (mRows[i].mUnk_28 == 0x15) {
            s16 width;
            s16 height;
            width  = 0;
            height = 0;
            TextManager::Get().Print(data_020d43f0[0].mName, 0x18, mTextBuffers[i], 0x10, 4, &width, &height, 0, 0, 2, 0x42, 0,
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
        TextManager::Get().mUnk_5b = 0;
        func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x8000 + i * 0x800, mTextBuffers[i], mTextBufferSize, 0);
        i++;
    } while (i < 6);
}

void MyroomRankingScene::ClearGlyphBuffers() {
    for (u8 i = 0; i < 6; i++) {
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
    }
}
