#include "global.h"
#include "types.h"
#include "Park/ParkMenu.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Unknown/UnkStruct_020758f4.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>


// overlay data tables
static const WindowItemRecord sItems[7] = {
    {0, 0x8000, 88, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8000, 186, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {2, 0x8000, 284, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {3, 0x8000, 382, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {4, 0x8000, 0, 72, 80, 48, 0, {-1, -1, -1, -1}, 0, 1},
    {5, 0x8000, 176, 72, 80, 48, 0, {-1, -1, -1, -1}, 0, 1},
    {6, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
};

static const WindowCellRecord sCells[7] = {
    {"/Game/Park/park_b_obj_nce.bin", 0, 0x0, 0, 2, 0, {2, 2, 2, 0}},
    {"/Game/Park/park_b_obj_nce.bin", 0, 0x0, 0, 2, 0, {2, 2, 2, 0}},
    {"/Game/Park/park_b_obj_nce.bin", 0, 0x0, 0, 2, 0, {2, 2, 2, 0}},
    {"/Game/Park/park_b_obj_nce.bin", 0, 0x0, 0, 2, 0, {2, 2, 2, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 0, 0x5000, 5, 1, 0, {0, 2, 2, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 1, 0x5000, 5, 1, 1, {1, 3, 2, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x6000, 6, 2, 1, {2, 3, 3, 0}},
};
static const u8 sTextY[2] = {40, 144};
static const u32 sTextVramOffsets[2] = {0xc000, 0x12000};
struct ParkDigits {
    s8 d[7];
};
static const ParkDigits sEmptyDigits = {{-1, -1, -1, -1, -1, -1, -1}};

ParkMenu::ParkMenu(u8 current) {
    mCurrent = current;
    mTarget = 0;
    mMode = 0;
    mTimer = 0;
    mWindow = NULL;
    mMessage = NULL;
    for (u8 i = 0; i < 8; i++) {
        mSlots[i]     = NULL;
        mSlots[i + 6] = NULL;
    }
    mSlots[2] = NULL;
    mWindow = new SelectWindow(7, sItems, 0);
    mWindow->SetCells(7, sCells);
    s8 sel = current;
    mWindow->mCursor->mSelection = -1;
    mWindow->func_0205c7c8(sel);
    mWindow->mCursor->mSelection = sel;
    mUnk_5c = new UnkStruct_020758f4();
    mUnk_5c->func_02075a0c();
    mCount = data_020d43f0[0].mUnk_5c1;
    if (mCount >= 4) {
        mCount = 4;
    }
    LoadGraphics();
    Setup();
}

ParkMenu::~ParkMenu() {
    delete mMessage;
    OS_Free(mSlots[2]);
    OS_Free(mSlots[1]);
    OS_Free(mSlots[0]);
    OS_Free(mSlots[7]);
    OS_Free(mSlots[6]);
    for (u8 i = 0; i < 8; i++) {
        CellAnimation *anim = (CellAnimation *) mSlots[8 + i];
        if (anim != NULL) {
            delete anim;
        }
    }
    delete mUnk_5c;
    delete mWindow;
}

// non-matching: register allocation (original keeps screen in r9, charBase/palette/index in r6/r7/r8; 34 words)
void ParkMenu::LoadGraphics() {
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/Park/");
    screen.LoadCharacters("park_b_obj.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("park_b_obj_ncl.bin", 0, 0);
    mCells[4] = new CellAnimation("park_b_obj_nce.bin", 4, 1, 1);
    mCells[4]->mCharBase = screen.func_020276f4(0);
    mCells[4]->mUnk_11   = 0;
    mCells[5] = new CellAnimation("park_b_obj_nce.bin", 3, 1, 1);
    mCells[5]->mCharBase = screen.func_020276f4(0);
    mCells[5]->mUnk_11   = 0;
    u8 i         = 0;
    u32 charBase = 0x8000;
    u8 palette   = 7;
    do {
        char name[64];
        int id = func_02089a64(i);
        sprintf(name, "park_b_ch_obj_%02d.nce_ncg1D.bin", id);
        screen.LoadCharacters(name, charBase, 0);
        sprintf(name, "park_b_ch_obj_%02d_ncl.bin", id);
        screen.LoadPalette(name, palette << 5, 0);
        sprintf(name, "park_b_ch_obj_%02d_nce.bin", id);
        mCells[i]            = new CellAnimation(name, 2, 1, 1);
        mCells[i]->mCharBase = screen.func_020276f4(charBase);
        charBase += 0x1000;
        mCells[i]->mUnk_11 = palette;
        palette            = palette + 2;
        i++;
    } while (i < mCount);
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_arrow.nce_ncg1D.bin", 0x5000, 0);
    screen.LoadPalette("union_b_obj_arrow_ncl.bin", 0xa0, 0);
    screen.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x6000, 0);
    screen.LoadPalette("union_b_obj_cockpit_ncl.bin", 0xc0, 0);
    screen.LoadCharacters("union_b_obj_point.nce_ncg1D.bin", 0x13000, 0);
    screen.LoadPalette("union_b_obj_point_ncl.bin", 0x80, 0);
    mCells[6]            = new CellAnimation("union_b_obj_point_nce.bin", 0, 1, 1);
    mCells[6]->mCharBase = screen.func_020276f4(0x13000);
    mCells[6]->mUnk_11   = 4;
    screen.LoadCharacters("union_b_obj_number.nce_ncg1D.bin", 0x14000, 0);
    mCells[7]            = new CellAnimation("union_b_obj_number_nce.bin", 0, 1, 1);
    mCells[7]->mCharBase = screen.func_020276f4(0x14000);
    mCells[7]->mUnk_11   = 4;
    ChangeDir("/");
    mMessages[0] = (MessageFile *) func_02023504("font/mes_park.mes", 0, 0, 0, 0, 0);
    mMessages[1] = (MessageFile *) func_02023504("font/mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 2);
    mTextBuffers[0] = OS_Alloc(mTextBufferSize);
    mTextBuffers[1] = OS_Alloc(mTextBufferSize);
    ClearBuffers();
    mGlyphCount  = TextManager::Get().GlyphCount(6);
    mGlyphBuffer = OS_Alloc((s16) mGlyphCount * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 2, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    mMessage = new MessageWindow("mes_park", 1, 0x1f000);
    mMessage->func_02065890(3, 0, 1);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
}

u32 ParkMenu::UpdateSelection(u32 mode) {
    u8 result = 0;
    mTarget   = mCurrent;
    if (mode == 3) {
        switch (mWindow->GetSelectedId()) {
            case 4:
                if (mCurrent != 0) {
                    result = 1;
                    mTarget--;
                }
                break;
            case 5:
                if (mCurrent < 3) {
                    result = 2;
                    mTarget++;
                }
                break;
        }
    }
    if (result == 0 && mWindow->mUnk_061 == 1) {
        switch (HandleInput()) {
            case 1:
                mWindow->func_0205c844(4, 3);
                break;
            case 2:
                mWindow->func_0205c844(5, 3);
                break;
        }
    }
    if (result != 0) {
        u8 k = 0;
        WindowItem *items = mWindow->mCursor->mItems;
        do {
            if (mTarget == k) {
                mWindow->mCells[k]->func_02029f6c();
                mWindow->mCells[k]->SetAnimation(1);
                mCells[k]->func_02029f6c();
                mCells[k]->SetAnimation(1);
            } else if (mCurrent == k) {
                mWindow->mCells[k]->func_02029f6c();
                mWindow->mCells[k]->SetAnimation(1);
                mWindow->mCells[k]->mUnk_0f = mWindow->mCells[k]->mFrames[mWindow->mCells[k]->mUnk_0e].mUnk_10;
                mWindow->mCells[k]->func_02029fb8(2);
                mCells[k]->func_02029f6c();
                mCells[k]->SetAnimation(1);
                mCells[k]->mUnk_0f = mCells[k]->mFrames[mCells[k]->mUnk_0e].mUnk_10;
                mCells[k]->func_02029fb8(2);
            }
            items[k].mFlags = 0x8000;
            k++;
        } while (k <= mCount);
        if (mTarget == 0) {
            mWindow->mButtons[4].mEnabled = 0;
            items[4].mFlags               = 0x8000;
        }
        if (mTarget == mCount - 1) {
            mWindow->mButtons[5].mEnabled = 0;
            items[5].mFlags               = 0x8000;
        }
        mTimer = 0x62;
    }
    mMode = result;
    return result;
}

int ParkMenu::HandleInput() {
    int result = 0;
    if (InputManager::Get().IsTrigger(0x20)) {
        if (mCurrent != 0) {
            result = 1;
            mTarget--;
        }
    } else if (InputManager::Get().IsTrigger(0x10)) {
        if (mCurrent < mCount - 1) {
            result = 2;
            mTarget++;
        }
    }
    if (result != 0) {
        SoundManager::Get().func_020246f8(0);
    }
    return result;
}

void ParkMenu::Setup() {
    WindowItem *items = mWindow->mCursor->mItems;
    s16 base          = mCurrent * 0x62;
    u8 i = 0;
    do {
        SelectWindow *w = mWindow;
        Vec2s R = w->mCellPos[i];
        R.x = 0x58 + i * 0x62 - base;
        Vec2s Q = R;
        w->mCellPos[i].x = Q.x;
        w->mCellPos[i].y = Q.y;
        items[i].mX = R.x;
        if (i == mCurrent) {
            CellAnimation *c = mWindow->mCells[i];
            c->SetAnimation(1);
            items[i].mFlags = 5;
            c               = mCells[i];
            c->SetAnimation(1);
        } else {
            CellAnimation *c = mWindow->mCells[i];
            c->SetAnimation(0);
            items[i].mFlags = 0x8000;
            c               = mCells[i];
            c->SetAnimation(0);
        }
        mWindow->mCells[i]->func_02029f6c();
        mCells[i]->func_02029f6c();
        mWindow->mButtons[i].mEnabled = 1;
        i++;
    } while (i < mCount);
    if (mCurrent == 0) {
        mWindow->mButtons[4].mEnabled = 0;
        items[4].mFlags               = 0x8000;
    } else {
        mWindow->mButtons[4].mEnabled = 1;
        items[4].mFlags               = 8;
    }
    if (mCurrent == mCount - 1) {
        mWindow->mButtons[5].mEnabled = 0;
        items[5].mFlags               = 0x8000;
    } else {
        mWindow->mButtons[5].mEnabled = 1;
        items[5].mFlags               = 8;
    }
    mWindow->mCursor->mSelection = mCurrent;
    DrawText();
    func_02089924(mCurrent, mUnk_0c);
}

BOOL ParkMenu::Slide() {
    BOOL result = FALSE;
    s16 step    = 0xe;
    if (mTimer < 0xe) {
        step = mTimer;
    }
    mTimer -= step;
    if (mMode == 2) {
        step = -step;
    }
    WindowItem *items = mWindow->mCursor->mItems;
    u8 i = 0;
    do {
        SelectWindow *w = mWindow;
        Vec2s R = w->mCellPos[i];
        R.x += step;
        Vec2s Q = R;
        w->mCellPos[i].x = Q.x;
        w->mCellPos[i].y = Q.y;
        items[i].mX = step + items[i].mX;
        i++;
    } while (i < mCount);
    if (mTimer == 0) {
        CellAnimation *cell = mWindow->mCells[mCurrent];
        cell->SetAnimation(0);
        items[mTarget].mFlags  = 5;
        items[mCurrent].mFlags = 0x8000;
        mWindow->mCursor->mSelection = mTarget;
        mCurrent = mTarget;
        func_02089924(mCurrent, mUnk_0c);
        if (mCurrent == 0) {
            mWindow->mButtons[4].mEnabled = 0;
            items[4].mFlags               = 0x8000;
        } else {
            mWindow->mButtons[4].mEnabled = 1;
            items[4].mFlags               = 8;
        }
        if (mTarget == mCount - 1) {
            mWindow->mButtons[5].mEnabled = 0;
            items[5].mFlags               = 0x8000;
        } else {
            mWindow->mButtons[5].mEnabled = 1;
            items[5].mFlags               = 8;
        }
        mWindow->mCells[5]->func_02029fc0();
        mWindow->mCells[4]->func_02029fc0();
        if (mMode == 1) {
            CellAnimation *c = mWindow->mCells[4];
            c->SetAnimation(0);
        } else {
            CellAnimation *c = mWindow->mCells[5];
            c->SetAnimation(1);
        }
        DrawText();
        result = TRUE;
    }
    return result;
}

// non-matching: guard load vs mUnk_5b store order (see docs/decompiling.md); size 0x268 vs 0x264
void ParkMenu::DrawText() {
    ClearBuffers();
    for (u8 i = 0; i < 2; i++) {
        u32 entry;
        if (i == 0) {
            entry = 0;
            UnkStruct_020758f4_Record *e = mUnk_5c->func_02075998(func_02089a64(mCurrent));
            TextManager::Get().mUnk_10 = e->mTextOffset;
        } else {
            entry = func_02089a64(mCurrent);
        }
        TextManager::Get().mUnk_5b = 1;
        TextManager &text = TextManager::Get();
        s16 w;
        s16 h;
        h = 0;
        w = 0;
        MessageFile *file = mMessages[i];
        text.Print(file->GetText(entry), file->GetLength(entry), mTextBuffers[i], 0x18, 2, &w, &h, &mTextWidth[i], 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);

        TextManager::Get().mUnk_5b = 0;
        func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, sTextVramOffsets[i], mTextBuffers[i], mTextBufferSize, 0);
    }
}

void ParkMenu::ClearBuffers() {
    for (u8 i = 0; i < 2; i++) {
        if (mTextBuffers[i] != NULL) {
            MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
        }
    }
}

void ParkMenu::Draw() {
    u8 i = 0;
    do {
        Vec2s pt = mWindow->mCellPos[i];
        mCells[i]->Update();
        u8 state = mCells[i]->mUnk_0e;
        if (state != 0) {
            if (state == 2 && mUnk_12 != 0xff) {
                mCells[5]->Update();
                mCells[5]->Draw(1, (s16) (pt.x + 0x23), (s16) (pt.y + 0x32), 0);
            }
            mCells[i]->Draw(1, pt.x, pt.y, 0);
        } else {
            mCells[4]->func_0202a3ec(1, func_02089a64(i), pt.x, pt.y, 0);
        }
        CellAnimation *c = mCells[i];
        if (c->mUnk_0e == 1 && c->mUnk_12 != 0) {
            if (c->mUnk_10 == 2) {
                c->SetAnimation(0);
            } else {
                c->SetAnimation(2);
            }
        }
        i++;
    } while (i < mCount);
    mWindow->Draw();
    for (i = 0; i < 2; i++) {
        SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, 6, (s16) (0x80 - ((u32) mTextWidth[i] >> 1)),
                                                     sTextY[i], 0, (u16) (sTextVramOffsets[i] >> 7), 0xf, 0);
    }
    DrawNumber();
    mMessage->func_020658d0();
}

void ParkMenu::DrawNumber() {
    u32 value = data_020d43f0[0].mMoney;
    u8 i = 0;
    ParkDigits digits = sEmptyDigits;
    s8 pos = 6;
    do {
        digits.d[pos] = value % 10;
        value /= 10;
        pos--;
    } while (pos >= 0 && value != 0);
    for (; i < 7; i++) {
        s32 d = digits.d[i];
        if (d >= 0 && mCells[7] != NULL) {
            mCells[7]->func_0202a3ec(1, (u8) d, (s16) (i * 7 + 0x2f), 0x17, 0);
        }
    }
    mCells[6]->Draw(1, 3, 0x14, 0);
}
