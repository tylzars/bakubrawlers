#include "Scene/TutorialScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

// Overlay data tables.
// Overlay tables still provided by the original data (not yet moved into this file).
extern const u8 data_ov005_022200d0[];
extern const WindowItemRecord data_ov005_02220190[];
extern const WindowCellRecord data_ov005_02220238[];
extern const WindowItemRecord data_ov005_022200e8[];
extern const WindowCellRecord data_ov005_0222013c[];

TutorialScene::TutorialScene() :
    SceneBase(1000, 0) {
    mUnk_1c        = NULL;
    mWindow        = NULL;
    mMessageWindow = NULL;
    mMessages[0]      = NULL;
    mMessages[1]     = NULL;
    mGlyphs[0].mBuffer   = NULL;
    mGlyphs[0].mCount    = 0;
    mGlyphs[1].mBuffer  = NULL;
    mGlyphs[1].mCount   = 0;
    mUnk_47        = 0;
    mUnk_48        = 0;
    mSelection     = 0;
    mScroll        = 0;
    mUnk_4b        = 0;
    mUnk_4c        = 0;
    mUnk_4d        = 0;
    MI_CpuClear32(mCells, sizeof(mCells));
    for (u8 i = 0; i < 7; i++) {
        if (TestFlagBit(data_020d49ae, i)) {
            mFlags[i] = 1;
        } else {
            mFlags[i] = 0;
        }
    }
    data_020d433c.mUnk_9a   = 0x11;
    data_020d433c.mUnk_9a_0 = data_020d43f0[0].mUnk_28_6;
}

TutorialScene::~TutorialScene() {
    if (mMessageWindow != NULL) {
        delete mMessageWindow;
    }
    if (mWindow != NULL) {
        delete mWindow;
    }
    for (u8 i = 0; i < 4; i++) {
        if (mCells[i] != NULL) {
            delete mCells[i];
        }
    }
    if (mMessages[0] != NULL) {
        OS_Free(mMessages[0]);
    }
    if (mGlyphs[0].mBuffer != NULL) {
        OS_Free(mGlyphs[0].mBuffer);
    }
    if (mMessages[1] != NULL) {
        OS_Free(mMessages[1]);
    }
    if (mGlyphs[1].mBuffer != NULL) {
        OS_Free(mGlyphs[1].mBuffer);
    }
    for (u8 i = 0; i < 7; i++) {
        if (mFlags[i] != 0) {
            SetFlagBit(data_020d49ae, i);
        }
    }
}

void func_ov005_0221e858() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 7);
    func_02001e68(1, 0, 1);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 1);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(2);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x10);
    G2_SetBlendBrightness(0x1f, -16);
    G2S_SetBlendBrightness(0x1f, -16);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

void func_ov005_0221e9c8() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2_SetBG0Priority(2);
    G2S_SetBG0Priority(2);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, -16);
    G2S_SetBlendBrightness(0x1f, -16);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

void TutorialScene::LoadGraphics() {
    ChangeDir("/Game/Tutorial/bg/");
    func_02025b64(0, 1, 0, "tutorial_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "tutorial_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "tutorial_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "tutorial_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "tutorial_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "tutorial_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Tutorial/obj/");
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.func_02027624(3);
    main.LoadCharacters("tutorial_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("tutorial_u_obj_00_ncl.bin", 0, 0);
    if (mCells[3] != NULL) {
        delete mCells[3];
    }
    mCells[3]            = new CellAnimation("tutorial_u_obj_00_nce.bin", 0, 1, 1);
    mCells[3]->mCharBase = main.func_020276f4(0);
    mCells[3]->mUnk_11   = 0;
    SpriteScreen &sub    = SpriteManager::Get().Screen(1);
    sub.func_02027624(3);
    sub.LoadCharacters("tutorial_b_obj_00.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("tutorial_b_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i < 3; i++) {
        if (mCells[i] != NULL) {
            delete mCells[i];
        }
        mCells[i]            = new CellAnimation("tutorial_b_obj_00_nce.bin", 0, 1, 1);
        mCells[i]->mCharBase = sub.func_020276f4(0);
        mCells[i]->mUnk_11   = 0;
    }
    CellAnimation *cell = mCells[1];
    cell->mUnk_0e       = 2;
    cell->func_02029f6c();
    cell          = mCells[0];
    cell->mUnk_0e = 1;
    cell->func_02029f6c();
    cell          = mCells[2];
    cell->mUnk_0e = 0;
    cell->func_02029f6c();
    ChangeDir("/Game/2D/union_b/");
    SpriteScreen &window = SpriteManager::Get().Screen(1);
    window.LoadCharacters("union_b_obj_scroll.nce_ncg1D.bin", 0x5000, 0);
    window.LoadPalette("union_b_obj_scroll_ncl.bin", 0x60, 0);
    window.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x6000, 0);
    delete mWindow;
    mWindow = new SelectWindow(6, data_ov005_02220190, -1);
    mWindow->SetCells(6, data_ov005_02220238);
    mWindow->mCursor->mSelection = -1;
    mWindow->mCursor->mSelection = 0;
    mWindow->mCursor->func_02030474(0x30, 0x90, 4, 6);
    mWindow->mUnk_064 = 0;
    if (data_020d433c.mUnk_83 != 0) {
        mSelection = data_020d433c.mUnk_83 - 1;
    } else {
        mSelection = 0;
        for (u8 i = 1; i < 7; i++) {
            if (mFlags[i] == 0) {
                mSelection = i - 1;
                break;
            }
        }
    }
    int scroll = mSelection - 3;
    if (scroll < 0) {
        scroll = 0;
    }
    mScroll = scroll;
    UpdateScroll();
    ChangeDir("/");
    delete mMessageWindow;
    mMessageWindow = new MessageWindow("mes_Tutorial", 1, 0x4000);
    mMessageWindow->func_02065bd4(0x14, 4, 2);
    mMessageWindow->func_02065be4(0xd8);
    mMessageWindow->func_02065890(2, 0, 1);
}

void TutorialScene::LoadListWindow() {
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG0Priority(2);
    GXS_SetVisiblePlane(0x11);
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 1, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
    if (mMessages[1] != NULL) {
        OS_Free(mMessages[1]);
    }
    if (mGlyphs[1].mBuffer != NULL) {
        OS_Free(mGlyphs[1].mBuffer);
    }
    mMessages[1]     = (MessageFile *) func_02023504("font/mes_Tutorial.mes", 0, 0, 0, 0, 0);
    mGlyphs[1].mCount   = TextManager::Get().GlyphCount(12);
    mGlyphs[1].mBuffer  = OS_Alloc((s16) mGlyphs[1].mCount * (s16) 12);
    TextManager::Get().Init(mGlyphs[1].mBuffer, 1, 0, 0x18, 4, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    delete mWindow;
    mWindow = new SelectWindow(3, data_ov005_022200e8, -1);
    mWindow->SetCells(3, data_ov005_0222013c);
    mWindow->mCursor->mSelection = -1;
    mWindow->mUnk_064             = 0;
    mWindow->mCursor->mSelection = 1;
}

void TutorialScene::ShowText(int index) {
    u32 size     = TextManager::Get().BufferSize(0x18, 4);
    void *buffer = OS_Alloc(size);
    MI_CpuClear16(buffer, size);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mMessages[1];
    text.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 4, &height,
                             &width, 0, 0, 2, 0x21, 0x10, 0, 0, 1, 1, 1);
    func_02025c1c(1, 4, 0x1000, buffer, size, 1);
}

void TutorialScene::LoadListText() {
    u8 i;
    ChangeDir("/");
    mMessages[0]     = (MessageFile *) func_02023504("font/mes_Tutorial.mes", 0, 0, 0, 0, 0);
    mGlyphs[0].mCount   = TextManager::Get().GlyphCount(8);
    mGlyphs[0].mBuffer  = OS_Alloc((s16) mGlyphs[0].mCount * (s16) 12);
    TextManager::Get().Init(mGlyphs[0].mBuffer, 1, 0, 0x20, 2, 0x21);
    for (i = 0; i < 6; i++) {
        u32 size     = TextManager::Get().BufferSize(0x20, 2);
        void *buffer = OS_Alloc(size);
        MI_CpuClear16(buffer, size);
        TextManager &text = TextManager::Get();
        s16 height;
        s16 width;
        width  = 0;
        height = 0;
        MessageFile *file = mMessages[0];
        text.Print(file->GetText((u16) (i + 5)), file->GetLength((u16) (i + 5)), buffer, 0x20, 2, &height,
                   &width, 0, 0, 2, 0x21, 0, 0, 0, 1, 1, 1);
        func_02025c1c(1, 4, 0x8000 + i * 0x800, buffer, size, 1);
    }
    TextManager::Get().SetColor(1, 0, 0xf);
}

u8 TutorialScene::UpdateList() {
    u8 result = mWindow->Update(FALSE);
    u8 i;
    if (result != 0) {
        if (result == 1) {
            CellAnimation *cell = mCells[1];
            cell->mUnk_0e       = 3;
            cell->func_02029f6c();
        }
        return result;
    }
    u8 prevScroll = mScroll;
    u8 scroll     = mWindow->mCursor->mScroll;
    mScroll       = scroll;
    u8 prevSel    = mSelection;
    int bottom    = scroll + 3;
    if (prevSel <= bottom) {
        if (prevSel >= scroll) {
            scroll = prevSel;
        }
        bottom = scroll;
    }
    mSelection = bottom;
    if (InputManager::Get().IsTrigger(0x40)) {
        if (mSelection != 0) {
            mSelection--;
        }
        if (mScroll > mSelection) {
            mScroll = mSelection;
        }
    } else if (InputManager::Get().IsTrigger(0x80)) {
        if (mSelection < 5) {
            mSelection++;
        }
        if (mScroll + 4 <= mSelection) {
            mScroll = mSelection - 3;
        }
    }
    if (TouchManager::Get().mUnk_5a != 0) {
        for (i = 0; i < 4; i++) {
            if (TouchManager::Get().func_020228c0(0x20, (s16) (i * 32 + 0x20), 0xc0, 0x1c)) {
                u8 index = mScroll + i;
                if (mSelection == index) {
                    result = 1;
                } else {
                    mSelection = index;
                }
                break;
            }
        }
    }
    if ((prevSel != mSelection) | (prevScroll != mScroll)) {
        UpdateScroll();
        SoundManager::Get().func_020246f8(5);
    }
    return result;
}

void TutorialScene::UpdateScroll() {
    mWindow->mCursor->SetScroll(mScroll);
    SelectWindow *window = mWindow;
    u8 y                 = window->mCursor->mUnk_0b;
    window->mCellPos[2].x = 0xe4;
    window->mCellPos[2].y = y;
    CellAnimation *cell = mCells[1];
    cell->mUnk_0e       = 2;
    cell->func_02029f6c();
}

void TutorialScene::DrawList() {
    mWindow->Draw();
    mMessageWindow->func_020658d0();
    u8 base = mWindow->mCursor->mScroll;
    for (u8 i = 0; i < 4; i++) {
        u8 index = base + i;
        SpriteManager::Get().Screen(1).func_02027778(mGlyphs[0].mBuffer, mGlyphs[0].mCount, 0x28, (s16) (i * 32 + 0x29), 2,
                                                     (u16) ((index * 0x800 + 0x8000) >> 7), 0xf, 0);
        if (mFlags[index + 1] != 0) {
            mCells[2]->Draw(1, 8, (s16) (i * 32 + 0x24), 2);
        }
        u8 selected;
        if (mSelection != index) {
            selected = 0;
        } else {
            selected = 1;
            mCells[1]->Update();
        }
        mCells[selected]->Draw(1, 0x20, (s16) (i * 32 + 0x20), 2);
    }
    mCells[3]->Draw(0, 0x80, 0x60, 0);
}

u8 func_ov005_0221fd0c() {
    s8 a = data_020d433c.mUnk_84 - 1;
    u8 b = data_020d433c.mUnk_83;
    if (b == 2) {
        a = a * 2;
        if (data_020d433c.mUnk_86 != 0) {
            a = a + 1;
        }
    }
    return data_ov005_022200d0[b] + a;
}

u8 func_ov005_0221fd0c();
BOOL func_ov005_0221fd64() {
    switch (func_ov005_0221fd0c()) {
        case 3:
        case 7:
        case 8:
        case 9:
        case 10:
        case 12:
        case 14:
            return TRUE;
        default:
            return FALSE;
    }
}

void func_ov005_0221fdc4() {
    data_020d433c.mUnk_99 = 3;
    data_020d433c.mPlayerCount = 4;
    u8 i;
    for (i = 0; i < 4; i++) {
        data_020d433c.mUnk_9f[i] = i;
        data_020d433c.mUnk_a3[i] = 0;
        data_020d433c.mUnk_9b[i] = i;
    }
    data_020d433c.mUnk_7b = 10;
    switch (data_020d433c.mUnk_83) {
        case 5:
            data_020d433c.mUnk_98 = 1;
            data_020d433c.mUnk_9b[0] = 1;
            data_020d433c.mUnk_9b[1] = 0;
            data_020d433c.mUnk_9b[2] = 3;
            data_020d433c.mUnk_9b[3] = 2;
            data_020d433c.mUnk_9f[0] = 0x15;
            data_020d433c.mUnk_9f[1] = 0;
            data_020d433c.mUnk_9f[2] = 0xe;
            data_020d433c.mUnk_9f[3] = 0xd;
            break;
        case 6:
            data_020d433c.mUnk_98 = 0;
            data_020d433c.mUnk_9f[0] = 0x15;
            data_020d433c.mUnk_9f[1] = 0xd;
            data_020d433c.mUnk_9f[2] = 0xf;
            data_020d433c.mUnk_9f[3] = 0x10;
            break;
    }
}
