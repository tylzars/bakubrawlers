#include "UI/UnlockPopup.hpp"

#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// Unlock message window (/font/mes_Unlock.mes, Unlock.mes text shown over a union_b window).

static const WindowCellRecord sMenuCells[2] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 0, 0, 2, 1, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 0, 0, 2, 1, {9, 9, 9, 0}},
};

static const WindowItemRecord sMenuItems[2] = {
    {0, 5, 89, 136, 78, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_CURSOR_SKIP, 120, 35, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
};

static const u8 sMessageIds[4] = {0, 1, 4, 0}; // mIndex 0 / 1 / 2 -> entries 0 / 1 / 4 of mMessages

UnlockPopup::UnlockPopup(s32 unk1) :
    SceneBase(0x3e8, 0) {
    mWindow        = NULL;
    mMessages    = NULL;
    mTextBuffer  = NULL;
    mGlyphBuffer = NULL;
    mState       = 0;
    mIndex       = unk1;
}

UnlockPopup::~UnlockPopup() {
    delete mWindow;
    ClearText();
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

void UnlockPopup::Update() {
    BOOL draw = TRUE;
    switch (mState) {
        case 0:
            InitDisplay();
            Load();
            SpriteManager::Get().StartSubFade(FADE_MODE_TO_ZERO);
            draw = FALSE;
            mState++;
            break;
        case 1:
            if (SpriteManager::Get().sFade[1] <= FADE_MODE_HOLD) {
                mWindow->mEnabled = 1;
                mState++;
            }
            break;
        case 2:
            if (mWindow->Update(TRUE) == 1) {
                SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
                mState++;
            }
            break;
        case 3:
            if (SpriteManager::Get().sFade[1] <= FADE_MODE_HOLD) {
                RequestScene(1);
                draw      = FALSE;
                mFinished = 1;
            }
            break;
    }
    if (draw) {
        Draw();
    }
}

void UnlockPopup::InitDisplay() {
    MI_CpuClearFast((void *) 0x06200000, 0x20000);
    MI_CpuClearFast((void *) 0x06600000, 0x20000);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Offset(0, 0);
    GXS_SetVisiblePlane(0x11);
}

void UnlockPopup::Load() {
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "union_b_bg_window_b_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "union_b_bg_window_a_nsc.bin", 0, 0, 0);
    SpriteScreen &bottom = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    bottom.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0, 0);
    bottom.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    mMessages       = (MessageFile *) LoadFile("/font/mes_Unlock.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 0xc);
    mGlyphCount     = TextManager::Get().GlyphCount(9);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    ClearText();
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 0xc, 0x42);
    TextManager::Get().SetColor(1, 0, 0xf);
    PrintMessage();
    ChangeDir("/");
    delete mWindow;
    mWindow = new SelectWindow(2, sMenuItems, -1);
    mWindow->SetCells(2, sMenuCells);
    mWindow->mCursor->mSelection = -1;
    mWindow->mCursor->mSelection = 0;
    mWindow->mEnabled            = 0;
}

void UnlockPopup::Draw() {
    mWindow->Draw();
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, 9, 0x20, 0x38, 2, 0x40, 0xf, 0);
}

void UnlockPopup::PrintMessage() {
    ClearText();
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width              = 0;
    height             = 0;
    u8 id              = sMessageIds[mIndex];
    MessageFile *table = mMessages;
    text.Print(table->GetText(id), table->GetLength(id), mTextBuffer, 0x18, 0xc, &height, &width, NULL, 0, 2, 0x42, 0xe,
               0, 0, 1, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0x2000, mTextBuffer, mTextBufferSize, 0);
}

void UnlockPopup::ClearText() {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
}
