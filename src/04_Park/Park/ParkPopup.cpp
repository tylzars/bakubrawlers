#include "global.h"
#include "types.h"
#include "Park/ParkPopup.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>

// overlay data tables
static const WindowItemRecord sTwoButtonItems[2] = {
    {0, 0x1, 89, 152, 78, 20, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};

static const WindowCellRecord sTwoButtonCells[2] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 0x10000, 0, 0, 1, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 0x10000, 0, 0, 1, {9, 9, 9, 0}},
};

static const WindowCellRecord sThreeButtonCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x10000, 0, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0x10000, 0, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x10000, 0, 0, 1, {10, 10, 10, 0}},
};

static const WindowItemRecord sThreeButtonItems[3] = {
    {0, 0x1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, 2},
    {1, 0x1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};

void ParkPopup::Open(const char *mesPath, u32 index, u32 mode) {
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 8, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x10000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    mMessages       = (MessageFile *) func_02023504(mesPath, 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 0xc);
    mGlyphCount     = TextManager::Get().GlyphCount(9);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    mGlyphBuffer    = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 0xc, 0x42);
    PrintMessage(index);
    delete mWindow;
    switch (mode) {
        case 0:
            mWindow = new SelectWindow(3, sThreeButtonItems, 1);
            mWindow->SetCells(3, sThreeButtonCells);
            break;
        case 1:
            mWindow = new SelectWindow(2, sTwoButtonItems, 0);
            mWindow->SetCells(2, sTwoButtonCells);
            break;
    }
    mWindow->mCursor->mSelection = -1;
    mWindow->mUnk_064            = 0;
    ChangeDir("/");
}

void ParkPopup::PrintMessage(u32 index) {
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffer, 0x18, 0xc, &height, &width, 0, 0, 2, 0x42,
               0x10, 0, 0, 0, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x1b000, mTextBuffer, mTextBufferSize, 0);
}

struct ParkRulesBuf {
    u8 pad[6];
    u8 mId;
    u8 pad2[5];
};

BOOL ParkRules::func_ov004_0221bf5c() {
    u8 lo = data_020d433c.mUnk_8b_0;
    u8 hi = data_020d433c.mUnk_8b_4;
    u8 saved = data_020d43f0[0].mUnk_5b8;
    ParkRulesBuf buf;
    data_020d43f0[0].mUnk_5b8 = hi;
    func_02089924(lo, &buf);
    u8 id = buf.mId;
    data_020d43f0[0].mUnk_5b8 = saved;
    BOOL ok = 1;
    if (id == 0xff) {
        return 0;
    }
    switch (id) {
        case 11: if (CountSlotsWith(0) != 3) ok = 0; break;
        case 12: if (CountSlotsWith(1) != 3) ok = 0; break;
        case 13: if (CountSlotsWith(2) != 3) ok = 0; break;
        case 14: if (CountSlotsWith(3) != 3) ok = 0; break;
        case 15: if (CountSlotsWith(4) != 3) ok = 0; break;
        case 16: if (CountSlotsWith(5) != 3) ok = 0; break;
        case 17: if (CountSlotsWith(0) != 0) ok = 0; break;
        case 18: if (CountSlotsWith(3) != 0) ok = 0; break;
        case 19: if (CountSlotsWith(1) != 0) ok = 0; break;
        case 20: if (CountSlotsWith(2) != 0) ok = 0; break;
        case 21: if (CountSlotsWith(4) != 0) ok = 0; break;
        case 22: if (CountSlotsWith(5) != 0) ok = 0; break;
        case 24: if (CountSlotsWith(4) != 0) ok = 0; break;
        case 25: if (data_020d433c.mUnk_8c_0 == 0) ok = 0; break;
    }
    if (ok) {
        func_020876f0(buf.mId);
        if (buf.mId == 0x19) {
            func_020876f0(0x1a);
        }
    }
    return ok;
}

u8 ParkRules::CountSlotsWith(u32 value) {
    u8 count = 0;
    for (u8 i = 0; i < 3; i++) {
        if (value == func_020230a8(&data_020d43f0[0], 0, i)) {
            count++;
        }
    }
    return count;
}
