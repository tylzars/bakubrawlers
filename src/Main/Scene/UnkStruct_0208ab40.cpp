#include "Unknown/UnkStruct_0208ab40.hpp"

#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// Unlock message window (/font/mes_Unlock.mes, Unlock.mes text shown over a union_b window).

static const u8 sMessageIds[4] = {0, 1, 4, 0};

static const WindowItemRecord sMenuItems[2] = {
    {0, 5, 89, 136, 78, 20, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8000, 120, 35, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
};

static const WindowCellRecord sMenuCells[2] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 0, 0, 2, 1, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 0, 0, 2, 1, {9, 9, 9, 0}},
};

UnkStruct_0208ab40::UnkStruct_0208ab40(s32 unk1) :
    SceneBase(0x3e8, 0) {
    mMenu        = NULL;
    mMessages    = NULL;
    mTextBuffer  = NULL;
    mGlyphBuffer = NULL;
    mState       = 0;
    mIndex       = unk1;
}

UnkStruct_0208ab40::~UnkStruct_0208ab40() {
    delete mMenu;
    func_0208b4a4();
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
}

void UnkStruct_0208ab40::Update() {
    BOOL draw = TRUE;
    switch (mState) {
        case 0:
            func_0208aec4();
            func_0208af20();
            SpriteManager::Get().StartSubFade(2);
            draw = FALSE;
            mState++;
            break;
        case 1:
            if (SpriteManager::Get().sFade.mSub <= 1) {
                mMenu->mUnk_064 = 1;
                mState++;
            }
            break;
        case 2:
            if (mMenu->Update(TRUE) == 1) {
                SpriteManager::Get().StartSubFade(3);
                mState++;
            }
            break;
        case 3:
            if (SpriteManager::Get().sFade.mSub <= 1) {
                RequestScene(1);
                draw      = FALSE;
                mFinished = 1;
            }
            break;
    }
    if (draw) {
        func_0208b284();
    }
}

void UnkStruct_0208ab40::func_0208aec4() {
    MI_CpuClearFast((void *) 0x06200000, 0x20000);
    MI_CpuClearFast((void *) 0x06600000, 0x20000);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Offset(0, 0);
    GXS_SetVisiblePlane(0x11);
}

void UnkStruct_0208ab40::func_0208af20() {
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 1, 0, "union_b_bg_window_b_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "union_b_bg_window_a_nsc.bin", 0, 0, 0);
    SpriteScreen &bottom = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    bottom.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0, 0);
    bottom.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    mMessages       = (MessageFile *) func_02023504("/font/mes_Unlock.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 0xc);
    mGlyphCount     = TextManager::Get().GlyphCount(9);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    func_0208b4a4();
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 0xc, 0x42);
    TextManager::Get().SetColor(1, 0, 0xf);
    func_0208b338();
    ChangeDir("/");
    delete mMenu;
    mMenu = new SelectWindow(2, sMenuItems, -1);
    mMenu->SetCells(2, sMenuCells);
    mMenu->mCursor->mSelection = -1;
    mMenu->mCursor->mSelection = 0;
    mMenu->mUnk_064            = 0;
}

void UnkStruct_0208ab40::func_0208b284() {
    mMenu->Draw();
    SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, 9, 0x20, 0x38, 2, 0x40, 0xf, 0);
}

void UnkStruct_0208ab40::func_0208b338() {
    func_0208b4a4();
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width              = 0;
    height             = 0;
    u8 id              = sMessageIds[mIndex];
    MessageFile *table = mMessages;
    text.Print(table->GetText(id), table->GetLength(id), mTextBuffer, 0x18, 0xc, &height, &width, NULL, 0, 2, 0x42, 0xe,
               0, 0, 1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x2000, mTextBuffer, mTextBufferSize, 0);
}

void UnkStruct_0208ab40::func_0208b4a4() {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
}
