#include "Battle/BattleUnk0227532c.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include "System/InputManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/fx.h>
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern MtxFx22 data_0220e838;
extern "C" void func_01ffcef4(MtxFx22 *mtx, fx32 scaleX, fx32 scaleY, u16 angle);

static const WindowItemRecord sTwoButtonItems[2] = {
    {0, 0x1, 89, 136, 78, 20, 0, {-1, -1, -1, -1}, 0, 2},
    {1, 0x8000, 120, 35, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};

static const WindowCellRecord sThreeButtonCells[3] = {
    {"union_b_obj_window_nce.bin", 3, 0x10000, 12, 0, 1, {3, 5, 4, 0}},
    {"union_b_obj_window_nce.bin", 7, 0x10000, 12, 0, 1, {6, 8, 7, 0}},
    {"union_b_obj_window_nce.bin", 10, 0x10000, 12, 0, 1, {10, 10, 10, 0}},
};

static const WindowCellRecord sTwoButtonCells[2] = {
    {"union_b_obj_window_nce.bin", 1, 0x10000, 12, 0, 1, {0, 2, 1, 0}},
    {"union_b_obj_window_nce.bin", 10, 0x10000, 12, 0, 1, {10, 10, 10, 0}},
};

static const Vec2s sTwoButtonPos[2] = {
    {0x159, 0x88},
    {0x178, 0x23},
};

static const Vec2s sThreeButtonPos[3] = {
    {0x12d, 0x88},
    {0x185, 0x88},
    {0x178, 0x23},
};

static const WindowItemRecord sThreeButtonItems[3] = {
    {0, 0x1, 45, 136, 78, 20, 0, {-1, -1, 1, 1}, 0, 2},
    {1, 0x1, 133, 136, 78, 20, 0, {-1, -1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 35, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};

BattleUnk0227532c::BattleUnk0227532c(u8 planes)
    : SceneBase(2, 0) {
    mState  = 0;
    mWindow = 0;
    mGlyphBuffer = 0;
    mPlanes = planes;
    mOffset = -0x100;
    MtxFx22 mtx;
    func_01ffcef4(&mtx, 0x1000, 0x1000, 0);
    G2x_SetBGyAffine_((vu16 *) 0x4001030, &mtx, 0, 0, -mOffset, 0);
}

BattleUnk0227532c::~BattleUnk0227532c() {
    delete mWindow;
    if (mGlyphBuffer) {
        OS_Free(mGlyphBuffer);
    }
}

void BattleUnk0227532c::Update() {
    switch (mState) {
        case 0:
            func_ov007_02275900();
            QueueFrameCallback((FrameCallback) func_ov007_022758a4, this);
            mState++;
            break;
        case 1:
            if (func_ov007_022756f0(0)) {
                mState++;
            }
            break;
        case 2:
            SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, (u8) mGlyphCount, (s16) (0x20 - mOffset), 0x40, 0, 0x220, 0xf, 0);
            if (mWindow->Update(1) == 1) {
                s8 id = mWindow->GetSelectedId();
                switch (id) {
                    case 0:
                        if (data_020d433c.mNextScene == 8) {
                            mResult = 2;
                        } else if (data_020d433c.mUnk_8a_4 != 0) {
                            mResult = 1;
                        } else {
                            mResult = 2;
                        }
                        break;
                    case 1:
                        mResult = 1;
                        break;
                }
                mState++;
            }
            if (InputManager::Get().IsHeld(8)) {
                mResult = 1;
                mState++;
            }
            break;
        case 3:
            if (func_ov007_022756f0(0x100)) {
                mState++;
            }
            break;
        case 4:
            GXS_SetVisiblePlane(mPlanes | 0x10);
            RequestScene(mResult);
            mFinished = 1;
            break;
    }
    if (mWindow) {
        mWindow->Draw();
    }
}

BOOL BattleUnk0227532c::func_ov007_022756f0(int target) {
    BOOL done = FALSE;
    mOffset += 0x18;
    u8 i = 0;
    do {
        Vec2s pos = mWindow->GetCellPos(i);
        pos.x -= 0x18;
        mWindow->SetCellPos(i, pos);
        i++;
    } while (i < mWindow->mCursor->mCount);
    if (mOffset >= target) {
        u8 j = 0;
        do {
            Vec2s pos = mWindow->GetCellPos(j);
            pos.x -= target - mOffset;
            mWindow->SetCellPos(j, pos);
            j++;
        } while (j < mWindow->mCursor->mCount);
        mOffset = target;
        done    = TRUE;
    }
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, (u8) mGlyphCount, (s16) (0x20 - mOffset), 0x40, 0, 0x220, 0xf, 0);
    QueueFrameCallback((FrameCallback) func_ov007_022758a4, this);
    return done;
}

void func_ov007_022758a4(BattleUnk0227532c *popup) {
    G2x_SetBGyAffine_((vu16 *) 0x4001030, &data_0220e838, 0, 0, popup->mOffset, 0);
    GXS_SetVisiblePlane(popup->mPlanes | 0x18);
}

void BattleUnk0227532c::func_ov007_02275900() {
    G2S_SetBG0Priority(3);
    G2S_SetBG3Priority(0);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, 1, 0x6000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 14, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 10, 0, "union_b_bg_window_a_nsc.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x10000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0x180, 0);
    TextManager::Get().SetColor(1, 0, 15);
    MessageFile *file = (MessageFile *) LoadFile("/font/mes_command.mes", 0, 0, 0, 0, 0);
    void *buffer      = OS_Alloc(TextManager::Get().BufferSize(0x18, 8));
    MI_CpuClearFast(buffer, 0x1800);
    int index;
    if (data_020d433c.mNextScene == 8) {
        index = 0x3c;
    } else if (data_020d433c.mUnk_8a_4 != 0) {
        index = 0x3d;
    } else {
        index = 0x3b;
    }
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width  = 0;
    height = 0;
    text.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 8, &height, &width, 0, 0, 2, 0x44, 0x10, 0, 0, 1, 1, 1);
    OS_Free(file);
    QueueUpload(1, 4, 0x11000, buffer, 0x1800, 1);
    mGlyphCount  = TextManager::Get().GlyphCount(3);
    mGlyphBuffer = OS_Alloc((s16) mGlyphCount * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 8, 0x44);
    if (index == 0x3d) {
        mWindow = new SelectWindow(2, sTwoButtonItems, 0);
        mWindow->SetCells(2, sTwoButtonCells);
        for (u8 i = 0; i < 2; i++) {
            SelectWindow *w = mWindow;
            w->mCellPos[i].x = sTwoButtonPos[i].x;
            w->mCellPos[i].y = sTwoButtonPos[i].y;
        }
    } else {
        mWindow = new SelectWindow(3, sThreeButtonItems, 1);
        mWindow->SetCells(3, sThreeButtonCells);
        for (u8 i = 0; i < 3; i++) {
            SelectWindow *w = mWindow;
            w->mCellPos[i].x = sThreeButtonPos[i].x;
            w->mCellPos[i].y = sThreeButtonPos[i].y;
        }
    }
    ChangeDir("/");
}
