#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Graphics/BakuganModel.hpp"
#include "System/FileSystem.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Model.hpp"
#include "Game/SaveData.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Text/TextManager.hpp"

// per cell values, five columns
static const u8 data_0209e5bc[41][5] = {
    {3, 1, 1, 0, 2},
    {3, 2, 2, 0, 2},
    {2, 1, 0, 1, 2},
    {3, 2, 0, 1, 2},
    {1, 1, 3, 2, 0},
    {0, 1, 3, 2, 2},
    {0, 1, 3, 2, 2},
    {2, 3, 2, 0, 0},
    {3, 3, 2, 0, 0},
    {1, 0, 2, 3, 1},
    {2, 0, 3, 3, 1},
    {2, 2, 0, 3, 1},
    {3, 2, 1, 2, 1},
    {2, 1, 3, 2, 0},
    {1, 3, 1, 0, 2},
    {0, 1, 3, 3, 1},
    {2, 1, 1, 1, 3},
    {2, 2, 3, 0, 0},
    {3, 0, 0, 3, 1},
    {0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0},
    {2, 0, 0, 0, 1},
    {0, 0, 0, 0, 1},
    {2, 1, 0, 1, 0},
    {3, 1, 0, 1, 0},
    {0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1},
    {0, 2, 0, 0, 1},
    {0, 3, 1, 0, 0},
    {0, 0, 1, 1, 0},
    {0, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {0, 1, 2, 1, 1},
    {1, 0, 0, 2, 0},
    {2, 1, 1, 2, 1},
    {2, 1, 2, 3, 1},
    {1, 3, 0, 3, 1},
    {1, 2, 1, 3, 2},
    {3, 1, 1, 0, 3},
};

static const u16 data_0209e588[6] = {0x4000, 0x8000, 0, 0, 0, 0};

// Message index per command (row) and column (0x0209e68c, 0xf8 bytes with two trailing zero bytes). As a
// function-local static of an inline accessor it gets a .rodata section of its own, after the other tables.
struct CommandMessageTable {
    /* 00 */ u8 mRows[41][6];
    /* f6 */ u8 mUnk_f6[2];
    /* f8 */
};
inline const CommandMessageTable &CommandMessages() {
    static const CommandMessageTable sCommandMessageTable = {
        {
            {0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {0, 0, 0, 2, 0, 0},
            {0, 0, 0, 3, 0, 0},
            {0, 4, 0, 0, 0, 0},
            {0, 5, 0, 0, 0, 0},
            {0, 5, 0, 0, 0, 0},
            {0, 0, 6, 0, 0, 0},
            {0, 0, 7, 0, 0, 0},
            {0, 0, 0, 0, 0, 8},
            {0, 0, 0, 0, 0, 9},
            {0, 0, 0, 0, 10, 0},
            {0, 0, 0, 0, 11, 0},
            {124, 125, 127, 126, 12, 128},
            {0, 0, 13, 0, 0, 0},
            {0, 14, 0, 0, 0, 0},
            {15, 0, 0, 0, 0, 0},
            {0, 16, 0, 16, 0, 0},
            {0, 0, 0, 0, 0, 17},
            {22, 23, 26, 24, 25, 27},
            {28, 29, 32, 30, 31, 33},
            {34, 35, 38, 36, 37, 39},
            {40, 41, 44, 42, 43, 45},
            {46, 47, 50, 48, 49, 51},
            {52, 53, 56, 54, 55, 57},
            {112, 113, 116, 114, 115, 117},
            {58, 59, 62, 60, 61, 63},
            {64, 65, 68, 66, 67, 69},
            {70, 71, 74, 72, 73, 75},
            {76, 77, 80, 78, 79, 81},
            {82, 83, 86, 84, 85, 87},
            {88, 89, 92, 90, 91, 93},
            {94, 95, 98, 96, 97, 99},
            {100, 101, 104, 102, 103, 105},
            {106, 107, 110, 108, 109, 111},
            {118, 119, 122, 120, 121, 123},
            {18, 18, 18, 18, 18, 18},
            {19, 19, 19, 19, 19, 19},
            {0, 0, 0, 0, 20, 0},
            {0, 0, 0, 0, 21, 0},
            {0, 0, 0, 0, 0, 0},
        },
        {0, 0},
    };
    return sCommandMessageTable;
}

// item table of the list window (positions are patched by CreateCommandListWindow)
inline WindowItemRecord *ListItems() {
    static WindowItemRecord sItems[4] = {
        { 0, 0x208, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 1 },
        { 1, 0x408, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 1 },
        { 2, 0x8000, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 0 },
        { 3, 0x8000, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 0 },
    };
    return sItems;
}

#define PAGE "/Game/2D/union_b/union_b_obj_page_nce.bin"
inline WindowCellRecord *ListCells() {
    static WindowCellRecord sCells[4] = {
        { PAGE, {0}, 0, {0}, {0}, 1, {0, 1, 1, 0} },
        { PAGE, {3}, 0, {0}, {0}, 1, {3, 4, 4, 0} },
        { PAGE, {6}, 0, {0}, {0}, 1, {6, 6, 6, 0} },
        { PAGE, {7}, 0, {0}, {0}, 1, {7, 7, 7, 0} },
    };
    return sCells;
}

#include "Graphics/SpriteManager.hpp"
#include "Graphics/Camera.hpp"
#include "System/SoundManager.hpp"

// paths loaded by LoadGraphics
inline const char **ListPaths() {
    static const char *sPaths[14] = {
        "/Game/Card/Graphic/cardspec_u_obj_back_ncl.bin",
        "/Game/Card/Graphic/cardspec_u_obj_back.nce_ncg1D.bin",
        "/Game/Card/Graphic/cardspec_u_obj_00_ncl.bin",
        "/Game/Card/Graphic/cardspec_u_obj_00.nce_ncg1D.bin",
        "/Game/Card/Graphic/cardspec_u_obj_00_nce.bin",
        "/Game/2D/union_b/union_b_obj_gp_ncl.bin",
        "/Game/2D/union_b/union_b_obj_gp.nce_ncg1D.bin",
        "/Game/2D/union_b/union_b_obj_gp_nce.bin",
        "/Game/2D/union_b/union_b_obj_you_ncl.bin",
        "/Game/2D/union_b/union_b_obj_you.nce_ncg1D.bin",
        "/Game/2D/union_b/union_b_obj_you_nce.bin",
        "/Game/Card/Graphic/bakugan_sub_l_ncl.bin",
        "/Game/Card/Graphic/bakugan_sub_l.nce_ncg1D.bin",
        "/Game/Card/Graphic/bakugan_sub_l_nce.bin",
    };
    return sPaths;
}

// light table of the list model
static const u16 sCommandListLights[20] = {1, 0x7fff, 0xf666, 0xf5ec, 0xf829};

// table of func_0205aea4
static const u16 data_0209e57c[6] = {0x2000, 0xc000, 0, 0, 0, 0};

extern u16 data_020c1a4c[];

void StopAsyncLoad();

BattleCommandList::BattleCommandList(u8 a, u8 b, s32 c, u8 d) :
    SceneBase(0x3e8, 0) {
    mBakuganMes  = NULL;
    mBakuganDisplayMes  = NULL;
    mBakuganNameMes  = NULL;
    mResultMes  = NULL;
    mLights  = NULL;
    mWindow  = NULL;
    mBakuganModel  = NULL;
    mCamera  = NULL;
    mUnk_14e = a;
    mUnk_14f = a;
    mUnk_150 = b;
    mUnk_151 = b;
    mPlayer = c;
    mNameWidth = 0;
    mTextPage = 0;
    mLastTextPage = 0;
    mWindowVisible = 0;
    mState = 0;
    mUnk_15a = 0;
    mUnk_15b = 0;
    mUnk_15c = 0;
    mVisible = 0;
    mClosing = 0;
    mUnk_15f = 0;
    mUnk_160 = 0;
    mUnk_161 = d;
    mUnk_162 = 0;
    mFourItems = 0;
    mUnk_164 = 0;
    mCountUpTimer = 0;
    mCountUpRemaining = 0;
    MI_CpuClear32(mTextBuffers, 0xc);
    MI_CpuClear32(mGlyphBuffers, 0xc);
    MI_CpuClear32(mTextBufferSizes, 0xc);
    MI_CpuClear32(mGlyphCounts, 0xc);
    MI_CpuClear32(mFiles, 0x38);
    MI_CpuClear32(mFileSizes, 0x38);
    MI_CpuClear32(mNceData, 0x10);
    MI_CpuClear32(mCellFiles, 0x10);
    MI_CpuClear32(mCells3c, 0x1c);
    MI_CpuClear32(mCells58, 0x20);
    MI_CpuFill8(mUnk_0c, 0, 0xa);
    MI_CpuFill8(mUnk_16, 0, 5);
    mUnk_98 = 0;
    mUnk_9a = 0;
    mUnk_9c = 0;
    ClearValues();
}

BattleCommandList::~BattleCommandList() {
    StopAsyncLoad();
    if (mBakuganMes != NULL) {
        OS_Free(mBakuganMes);
    }
    if (mBakuganDisplayMes != NULL) {
        OS_Free(mBakuganDisplayMes);
    }
    if (mBakuganNameMes != NULL) {
        OS_Free(mBakuganNameMes);
    }
    if (mResultMes != NULL) {
        OS_Free(mResultMes);
    }
    for (u8 i = 0; i < 3; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
        }
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
        }
    }
    for (u8 i = 0; i < 4; i++) {
        if (mNceData[i] != NULL) {
            OS_Free(mNceData[i]);
        }
        CellAnimFile *file = mCellFiles[i];
        if (file != NULL) {
            delete file;
        }
    }
    CellAnimation *cell;
    for (u8 i = 0; i < 7; i++) {
        cell = mCells3c[i];
        if (cell != NULL) {
            delete cell;
        }
    }
    CellAnimation *cell2;
    for (u8 i = 0; i < 8; i++) {
        cell2 = mCells58[i];
        if (cell2 != NULL) {
            delete cell2;
        }
    }
    delete mCamera;
    ModelLightGroup *lights = mLights;
    if (lights != NULL) {
        delete lights;
    }
    if (mWindow != NULL) {
        delete mWindow;
    }
    BakuganModel *task = mBakuganModel;
    if (task != NULL) {
        delete task;
    }
    CameraManager::Get().SetCamerasAndRequestMode(0, NULL, 0);
    SpriteManager::Get().ResetSlots();
    GX_SetVisibleWnd(0);
    G2_SetBlendBrightness(0, 0);
}

void BattleCommandList::Update() {
    switch (mState) {
        case 0:
            InitDisplayAndCamera();
            LoadText();
            ResetLoadStep();
            mState++;
            break;
        case 1:
            if (LoadGraphics()) {
                mVisible = 1;
                mState++;
            }
            break;
        case 2:
            if (SpriteManager::Get().sFade[0] <= 1) {
                mState++;
            }
            break;
        case 3: {
            BakuganModel *task = mBakuganModel;
            if (task != NULL) {
                delete task;
            }
            mBakuganModel = NULL;
            SpriteManager::Get().ResetSlots();
            ResetLoadStep();
            mState++;
            break;
        }
        case 4:
            if (CreateBakuganModel()) {
                mState++;
            }
            break;
        case 5:
            PrintText(0);
            UpdateListItems();
            mState++;
            break;
        case 6: {
            u8 i;
            u8 offset = (mUnk_15b == 0) ? 1 : 0;
            u8 *dst   = (u8 *) this + offset;
            for (i = 0; i < 5; i++) {
                dst[0xc + i * 2] = mUnk_16[i];
            }
            if (mUnk_160 == 0) {
                mUnk_15f = 0;
            }
            if (mClosing == 0) {
                SpriteManager::Get().sFade[0] = 2;
            }
            mState++;
            break;
        }
        case 7: {
            if (mUnk_160 != 0) {
                mUnk_15f = 1;
            }
            u8 next = mUnk_15c;
            if (mUnk_15b != next) {
                mUnk_15b = next;
            }
            if ((mUnk_14f != mUnk_14e) | (mUnk_151 != mUnk_150)) {
                mUnk_14e = mUnk_14f;
                mUnk_150 = mUnk_151;
                mUnk_160 = 0;
                if (mUnk_15b != 0) {
                    mUnk_15b = 1;
                    mUnk_15c = 1;
                }
                mState = 3;
            }
            if (mClosing != 0) {
                mState = 8;
            }
            break;
        }
        case 8:
            SpriteManager::Get().sFade[0] = 3;
            mState++;
            break;
        case 9:
            if (SpriteManager::Get().sFade[0] <= 1) {
                mVisible = 0;
                BakuganModel *task = mBakuganModel;
                if (task != NULL) {
                    delete task;
                }
                mBakuganModel = NULL;
                mState++;
            }
            break;
        case 10:
            RequestScene(1);
            mFinished = 1;
            break;
    }
    if (mVisible == 0) {
        return;
    }
    UpdatePage();
    DrawName();
    if (mUnk_15f == 0 && mState >= 5) {
        DrawBakugan();
    }
    if (mWindowVisible != 0 && mUnk_15f == 0) {
        if (SpriteManager::Get().sBrightness[1] == 0) {
            if (mState == 7) {
                UpdateWindow();
            }
            mWindow->Draw();
        }
    }
    func_0205aea4();
}

void CloseCommandList(BattleCommandList *list) {
    list->mClosing = 1;
}

void func_02058228(BattleCommandList *list, u32 a) {
    u8 value        = a;
    list->mUnk_160 = value;
    if (list->mState < 3) {
        list->mUnk_15f = value;
    }
}

void BattleCommandList::func_02058240(u32 value) {
    mUnk_162 = value;
}

void func_02058248(BattleCommandList *list, u8 b) {
    SetCommandListBakugan(list, 0x2b, b, -1);
}

void SetCommandListBakugan(BattleCommandList *list, u8 a, u8 b, s32 c) {
    if (a >= 0x2b) {
        if ((list->mUnk_15b == 0) & (list->mUnk_14e < 0x2b)) {
            MI_CpuClear16((void *) 0x6400000, 0x2000);
        }
        list->mUnk_15c = 1;
        list->mUnk_15b = 1;
    } else {
        if (list->mUnk_14e == 0x2b) {
            list->mUnk_15c = 0;
            list->mUnk_15b = 0;
        }
    }
    if (c < 0) {
        list->mPlayer = -1;
        for (u8 i = 0; i < 5; i++) {
            list->mUnk_16[i] = 0;
        }
    } else {
        list->mPlayer = c % 4;
    }
    if (list->mState == 1) {
        list->mUnk_14e = a;
        list->mUnk_14f = a;
        list->mUnk_150 = b;
        list->mUnk_151 = b;
        list->mUnk_15f = 0;
        list->mUnk_160 = 0;
        return;
    }
    list->mUnk_14f = a;
    list->mUnk_151 = b;
    list->mUnk_162 = 0;
    if ((list->mUnk_14f == list->mUnk_14e) & (list->mUnk_151 == list->mUnk_150)) {
        if (list->mUnk_15f != 0) {
            list->mUnk_15f = 0;
        }
    }
    list->mUnk_160 = 0;
}

void BattleCommandList::CyclePage(u32 flag) {
    if (flag == 0) {
        mUnk_15c = (mUnk_15c + 2) % 3;
    } else {
        mUnk_15c = (mUnk_15c + 1) % 3;
    }
    if (mUnk_15c == 0) {
        return;
    }
    PrintText(mUnk_15c - 1);
}

BOOL BattleCommandList::IsSettingUp() {
    return mState < 7;
}

void CreateCommandListWindow(BattleCommandList *list, u32 a, u32 b, s16 c, s16 d, s16 e, s16 f, u32 g) {
    if (list->mWindow != NULL) {
        delete list->mWindow;
    }
    ListItems()[0].mUnk_04 = 0x208;
    ListItems()[0].mUnk_18 = 1;
    ListItems()[1].mUnk_04 = 0x408;
    ListItems()[1].mUnk_18 = 1;
    ListItems()[2].mUnk_04 = 0x8000;
    ListItems()[2].mUnk_18 = 0;
    ListItems()[3].mUnk_04 = 0x8000;
    ListItems()[3].mUnk_18 = 0;
    ListItems()[0].mX      = c;
    ListItems()[0].mY      = d;
    ListItems()[1].mX      = e;
    ListItems()[1].mY      = f;
    ListItems()[2].mX      = c;
    ListItems()[2].mY      = d;
    ListItems()[3].mX      = e;
    ListItems()[3].mY      = f;
    ListCells()[0].mCharOffset = a;
    ListCells()[0].mPaletteBase = b;
    ListCells()[1].mCharOffset = a;
    ListCells()[1].mPaletteBase = b;
    ListCells()[2].mCharOffset = a;
    ListCells()[2].mPaletteBase = b;
    ListCells()[3].mCharOffset = a;
    ListCells()[3].mPaletteBase = b;
    u32 count;
    list->mFourItems = g;
    if (list->mFourItems != 0) {
        count = 4;
    } else {
        count = 2;
    }
    list->mWindow  = new SelectWindow(count, ListItems(), -1);
    list->mWindow->SetCells(count, ListCells());
    list->mWindow->mCursor->mSelection = -1;
    list->mWindowVisible                     = 1;
    list->UpdateListItems();
}

void BattleCommandList::SetWindowVisible(u32 enable) {
    if (enable != 0) {
        if (mWindow != NULL) {
            mWindowVisible = 1;
        }
    } else {
        mWindowVisible = 0;
    }
}

void BattleCommandList::UpdateWindow() {
    if (mWindow->Update(FALSE)) {
        SoundManager::Get().StartSeqArc(0);
        CyclePage(*(u8 *) &mWindow->mCursor->mSelection);
    }
    UpdateListItems();
}

void BattleCommandList::UpdateListItems() {
    if (mWindow == NULL) {
        return;
    }
    u8 selected       = mUnk_15b;
    u8 count          = mLastTextPage;
    WindowItem *items = mWindow->mCursor->mItems;
    if (selected <= count) {
        if (IsListItemEnabled(1) == 0) {
            mWindow->mCells[1]->SetAnimation(3);
            items[1].mFlags = 0x408;
            if (mFourItems != 0) {
                mWindow->mCells[3]->SetAnimation(7);
            }
        }
    } else {
        if (IsListItemEnabled(1) != 0) {
            mWindow->mCells[1]->SetAnimation(5);
            items[1].mFlags = 0x8000;
            if (mFourItems != 0) {
                mWindow->mCells[3]->SetAnimation(5);
            }
        }
    }
    BOOL hasPage = FALSE;
    if (mUnk_14e == 0x2b) {
        if (mTextPage != 0) {
            hasPage = TRUE;
        }
    } else {
        if (mUnk_15b != 0) {
            hasPage = TRUE;
        }
    }
    if (hasPage) {
        if (IsListItemEnabled(0) != 0) {
            return;
        }
        mWindow->mCells[0]->SetAnimation(0);
        items[0].mFlags = 0x208;
        if (mFourItems == 0) {
            return;
        }
        mWindow->mCells[2]->SetAnimation(6);
    } else {
        if (IsListItemEnabled(0) == 0) {
            return;
        }
        mWindow->mCells[0]->SetAnimation(2);
        items[0].mFlags = 0x8000;
        if (mFourItems == 0) {
            return;
        }
        mWindow->mCells[2]->SetAnimation(2);
    }
}

BOOL BattleCommandList::IsListItemEnabled(u32 right) {
    if (right == 0) {
        if (mWindow->mCells[0]->mAnimIndex == 2) {
            return FALSE;
        }
    } else {
        if (mWindow->mCells[1]->mAnimIndex == 5) {
            return FALSE;
        }
    }
    return TRUE;
}

void BattleCommandList::InitDisplayAndCamera() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(0, 4);
    GX_SetGraphicsMode(1, 0, 1);
    GX_SetVisiblePlane(0x11);
    G2_SetBG0Priority(2);
    WriteBG0Offset(0);
    G2_SetBlendBrightness(1, -8);
    GX_SetVisibleWnd(1);
    G2_SetWndOutsidePlane(0x11, FALSE);
    G2_SetWnd0InsidePlane(0x11, TRUE);
    G2_SetWnd0Position(0x7c, 0, 0xff, 0xc0);
    mCamera = new Camera();
    mCamera->SetUp(0, 0x1000, 0);
    mCamera->SetTarget(0, 0, 0);
    mCamera->SetClip(0x1000, 0x1388000);
    mCamera->SetFov(0x2aaa);
    mCamera->SetPosition(0, 0, mCamera->mUnk_34);
    CameraManager::Get().mActive = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, mCamera, 0);
}

void BattleCommandList::ResetLoadStep() {
    mLoadStep = 0;
}

BOOL BattleCommandList::LoadGraphics() {
    if (mLoadStep != 0) {
        if (IsAsyncLoadActive() != 0) {
            return FALSE;
        }
        mFiles[mLoadStep - 1] = (u32) GetAsyncLoadBuffer();
    }
    u32 n = mLoadStep;
    if (n < 14) {
        u32 *out = &mFileSizes[n];
        LoadFileAsync(ListPaths()[n], 0, 0, 0, out, 0);
        mLoadStep++;
        return FALSE;
    }
    SpriteManager::Get().ResetSlots();
    SpriteManager::Get().mScreens[0].SetObjVramMode(3);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 0, 0, (void *) mFiles[0], mFileSizes[0], 1);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x14000, (void *) mFiles[1], mFileSizes[1], 1);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 5, 0x40, (void *) mFiles[2], mFileSizes[2], 1);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x7000, (void *) mFiles[3], mFileSizes[3], 1);
    if (mNceData[0] != NULL) {
        OS_Free(mNceData[0]);
    }
    mNceData[0] = (void *) mFiles[4];
    if (mNceData[0] != NULL) {
        CellAnimFile *file = mCellFiles[0];
        if (file != NULL) {
            delete file;
        }
        mCellFiles[0] = new CellAnimFileData(mNceData[0]);
        if (mCellFiles[0] != NULL) {
            for (u8 i = 0; i < 7; i++) {
                CellAnimation *cell = mCells3c[i];
                if (cell != NULL) {
                    delete cell;
                }
                mCells3c[i]            = new CellAnimation(mCellFiles[0], 0);
                mCells3c[i]->mCharBase = SpriteManager::Get().Screen(0).GetCharBase(0x7000);
                mCells3c[i]->mPaletteBase   = 2;
            }
            mCells3c[0]->SetAnimation(2);
            mCells3c[1]->SetAnimation(3);
            mCells3c[2]->SetAnimation(6);
            mCells3c[4]->SetAnimation(0xa);
            mCells3c[5]->SetAnimation(5);
            mCells3c[3]->SetAnimation(0);
            mCells3c[3]->mFrameIndex = 2;
            mCells3c[6]->SetAnimation(4);
        }
    }
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 5, 0, (void *) mFiles[5], mFileSizes[5], 1);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x4000, (void *) mFiles[6], mFileSizes[6], 1);
    if (mNceData[1] != NULL) {
        OS_Free(mNceData[1]);
    }
    mNceData[1] = (void *) mFiles[7];
    if (mNceData[1] != NULL) {
        CellAnimFile *file = mCellFiles[1];
        if (file != NULL) {
            delete file;
        }
        mCellFiles[1] = new CellAnimFileData(mNceData[1]);
        if (mCellFiles[1] != NULL) {
            for (u8 i = 0; i <= 2; i++) {
                CellAnimation *cell = mCells58[i];
                if (cell != NULL) {
                    delete cell;
                }
                mCells58[i]            = new CellAnimation(mCellFiles[1], 0);
                mCells58[i]->mCharBase = SpriteManager::Get().Screen(0).GetCharBase(0x4000);
                mCells58[i]->mPaletteBase   = 0;
            }
            mCells58[0]->SetAnimation(2);
            mCells58[1]->SetAnimation(1);
            mCells58[2]->SetAnimation(3);
        }
    }
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 5, 0x20, (void *) mFiles[8], mFileSizes[8], 1);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x5000, (void *) mFiles[9], mFileSizes[9], 1);
    if (mNceData[2] != NULL) {
        OS_Free(mNceData[2]);
    }
    mNceData[2] = (void *) mFiles[10];
    if (mNceData[2] != NULL) {
        CellAnimFile *file = mCellFiles[2];
        if (file != NULL) {
            delete file;
        }
        mCellFiles[2] = new CellAnimFileData(mNceData[2]);
        if (mCellFiles[2] != NULL) {
            for (u8 i = 3; i < 7; i++) {
                CellAnimation *cell = mCells58[i];
                if (cell != NULL) {
                    delete cell;
                }
                mCells58[i]            = new CellAnimation(mCellFiles[2], 0);
                mCells58[i]->mCharBase = SpriteManager::Get().Screen(0).GetCharBase(0x5000);
                mCells58[i]->mPaletteBase   = 1;
            }
            mCells58[3]->SetAnimation(1);
            mCells58[4]->SetAnimation(3);
            mCells58[5]->SetAnimation(2);
            mCells58[6]->SetAnimation(4);
        }
    }
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 0, 0x200, (void *) mFiles[11], mFileSizes[11], 1);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0xc000, (void *) mFiles[12], mFileSizes[12], 1);
    if (mNceData[3] != NULL) {
        OS_Free(mNceData[3]);
    }
    mNceData[3] = (void *) mFiles[13];
    if (mNceData[3] != NULL) {
        CellAnimFile *file = mCellFiles[3];
        if (file != NULL) {
            delete file;
        }
        mCellFiles[3] = new CellAnimFileData(mNceData[3]);
        if (mCellFiles[3] != NULL) {
            CellAnimation *cell = mCells58[7];
            if (cell != NULL) {
                delete cell;
            }
            mCells58[7]            = new CellAnimation(mCellFiles[3], 0);
            mCells58[7]->mCharBase = SpriteManager::Get().Screen(0).GetCharBase(0xc000);
            mCells58[7]->mPaletteBase   = 1;
        }
    }
    CameraManager &manager = CameraManager::Get();
    Camera *camera         = manager.mCameras[manager.mActive];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mActive] = data_02211374.mProjection;
    manager.mView[manager.mActive]       = data_02211374.mView;
    manager.mMtxA[manager.mActive]       = *func_02032710();
    manager.mMtxB[manager.mActive]       = *func_02032604();
    manager.mMtxC[manager.mActive]       = *func_020323b8();
    mLights = new ModelLightGroup(sCommandListLights);
    return TRUE;
}

BOOL BattleCommandList::CreateBakuganModel() {
    mUnk_14e = mUnk_14f;
    mUnk_150 = mUnk_151;
    if (mUnk_14e >= 0x2b) {
        mBakuganModel = NULL;
        return TRUE;
    }
    mBakuganModel = new BakuganModel();
    mBakuganModel->Setup(mUnk_14e, mUnk_150, 6);
    VecFx32 pos;
    pos.x = -0xd000;
    pos.y = pos.x + 0xb000;
    pos.z = 0xb8500;
    mBakuganModel->func_0203db70(&pos);
    return TRUE;
}

void BattleCommandList::LoadText() {
    SpriteManager::Get().mScreens[0].SetObjVramMode(3);
    mBakuganDisplayMes    = LoadFile("/font/mes_BakuganDisplay.mes", 0, 0, 0, 0, 0);
    mBakuganMes    = LoadFile("/font/mes_Bakugan.mes", 0, 0, 0, 0, 0);
    mTextBufferSizes[0] = TextManager::Get().BufferSize(0x10, 0x12);
    mTextBuffers[0] = OS_Alloc(mTextBufferSizes[0]);
    MI_CpuClear16(mTextBuffers[0], mTextBufferSizes[0]);
    mGlyphCounts[0] = TextManager::Get().GlyphCount(0x24);
    mGlyphBuffers[0] = OS_Alloc((s16) mGlyphCounts[0] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[0], 0, 0, 0x10, 0x12, 0x21);
    mBakuganNameMes    = LoadFile("/font/mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mTextBufferSizes[1] = TextManager::Get().BufferSize(0x10, 8);
    mTextBuffers[1] = OS_Alloc(mTextBufferSizes[1]);
    MI_CpuClear16(mTextBuffers[1], mTextBufferSizes[1]);
    mGlyphCounts[1] = TextManager::Get().GlyphCount(0x10);
    mGlyphBuffers[1] = OS_Alloc((s16) mGlyphCounts[1] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[1], 0, 0, 0x40, 2, 0x21);
    mResultMes    = LoadFile("/font/mes_Result.mes", 0, 0, 0, 0, 0);
    mTextBufferSizes[2] = TextManager::Get().BufferSize(0x10, 2);
    mTextBuffers[2] = OS_Alloc(mTextBufferSizes[2]);
    MI_CpuClear16(mTextBuffers[2], mTextBufferSizes[2]);
    mGlyphCounts[2] = TextManager::Get().GlyphCount(4);
    mGlyphBuffers[2] = OS_Alloc((s16) mGlyphCounts[2] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[2], 0, 0, 0x10, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    MI_CpuClearFast((void *) 0x6400000, 0x3800);
    ClearText();
}

void BattleCommandList::PrintText(u8 index) {
    mTextPage = index;
    ClearText();
    u16 width;
    s16 numY;
    s16 numX;
    u8 cmd = mUnk_14e;
    if (cmd >= 0x2b) {
        if (mTextPage == 0) {
            TextManager &text = TextManager::Get();
            s16 y;
            u16 x;
            x = 2 - (mTextPage << 7);
            y = 0x10;
            u8 sel            = mUnk_150;
            MessageFile *file;
            void *buffer;
            buffer = mTextBuffers[0];
            file   = (MessageFile *) mBakuganMes;
            MI_CpuClear16(data_020c1a4c, 0x20);
            text.mLineStartCount      = 1;
            text.Print(file->GetText(sel), file->GetLength(sel), buffer, 0x10, 0x12, &y, (s16 *) &x, 0, 0, 2, 0x21,
                       0xe, 0, 1, 0, 1, 1);
            u16 w = x;
            for (u8 i = 1; i < 0x10; i++) {
                if (text.mLineStarts[i] == 0) {
                    text.mLineStarts[i] = text.mUnk_36[i];
                }
            }
            mLastTextPage = (w - 0xe) / 0x80;
        }
        {
            TextManager &text = TextManager::Get();
            s16 y;
            u16 x;
            x = 2 - (mTextPage << 7);
            y = 0x10;
            u8 sel            = mUnk_150;
            MessageFile *file = (MessageFile *) mBakuganMes;
            text.Print(file->GetText(sel), file->GetLength(sel), mTextBuffers[0], 0x10, 0x12, &y, (s16 *) &x,
                                     0, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
        }
        mNameWidth = 0;
        TextManager::Get().mUse4bppFont = 1;
        {
            TextManager &text = TextManager::Get();
            s16 y;
            s16 x;
            x = 0;
            y = 0;
            MessageFile *file = (MessageFile *) mBakuganMes;
            text.Print(file->GetText((u16) (mUnk_150 + 6)), file->GetLength((u16) (mUnk_150 + 6)), mTextBuffers[1], 0x40, 2, &y, &x,
                                     &mNameWidth, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
        }
        TextManager::Get().mUse4bppFont = 0;
    } else {
        if (mTextPage == 0) {
            TextManager &text = TextManager::Get();
            s16 y;
            u16 x;
            x = 2 - (mTextPage << 7);
            y = 0x10;
            u8 row = CommandMessages().mRows[cmd][mUnk_150];
            MessageFile *file;
            void *buffer;
            buffer = mTextBuffers[0];
            file   = (MessageFile *) mBakuganDisplayMes;
            MI_CpuClear16(data_020c1a4c, 0x20);
            text.mLineStartCount      = 1;
            text.Print(file->GetText(row), file->GetLength(row), buffer, 0x10, 0x12, &y, (s16 *) &x, 0, 0, 2, 0x21,
                       0xe, 0, 1, 0, 1, 1);
            u16 w = x;
            for (u8 i = 1; i < 0x10; i++) {
                if (text.mLineStarts[i] == 0) {
                    text.mLineStarts[i] = text.mUnk_36[i];
                }
            }
            mLastTextPage = (w - 0xe) / 0x80;
        }
        {
            TextManager &text = TextManager::Get();
            s16 y;
            u16 x;
            x = 2 - (mTextPage << 7);
            y = 0x10;
            u8 row = CommandMessages().mRows[cmd][mUnk_150];
            MessageFile *file = (MessageFile *) mBakuganDisplayMes;
            text.Print(file->GetText(row), file->GetLength(row), mTextBuffers[0], 0x10, 0x12, &y, (s16 *) &x,
                                     0, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
        }
        if (mUnk_161 != 0) {
            if ((cmd == 5) | (cmd == 6)) {
                cmd = cmd + 0x24;
            }
        }
        mNameWidth = 0;
        TextManager::Get().mUse4bppFont = 1;
        {
            TextManager &text = TextManager::Get();
            s16 y;
            s16 x;
            x = 0;
            y = 0;
            MessageFile *file = (MessageFile *) mBakuganNameMes;
            text.Print(file->GetText(cmd), file->GetLength(cmd), mTextBuffers[1], 0x40, 2, &y, &x,
                       &mNameWidth, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
        }
        width = 0;
        {
            TextManager &text = TextManager::Get();
            s16 y;
            s16 x;
            x = 0;
            y = 0;
            MessageFile *file = (MessageFile *) mResultMes;
            text.Print(file->GetText(5), file->GetLength(5), mTextBuffers[2], 0x10, 2, &y, &x, &width, 0, 2,
                       0x21, 0xe, 0, 0, 0, 1, 1);
        }
        if (mUnk_164 >= 9) {
            TextManager &text = TextManager::Get();
            s16 y;
            s16 x;
            x = 0;
            y = width + 6;
            MessageFile *file = (MessageFile *) mResultMes;
            text.Print(file->GetText(6), file->GetLength(6), mTextBuffers[2], 0x10, 2, &y, &x, 0, 0, 2, 0x21,
                       0xe, 0, 0, 0, 1, 1);
        } else {
            numY = width + 10;
            numX = 0;
            TextManager::Get().PrintNumber(mUnk_164 + 1, 1, 0, mTextBuffers[2], 0x10, 2, &numY, &numX, 0, 2, 0x21, 0xe, 0, 1, 1);
        }
        TextManager::Get().mUse4bppFont = 0;
    }
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0, mTextBuffers[0], 0x2000, 0);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x2000, mTextBuffers[1], mTextBufferSizes[1], 0);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x3000, mTextBuffers[2], mTextBufferSizes[2], 0);
}

void BattleCommandList::DrawName() {
    if (mUnk_15f != 0) {
        return;
    }
    SpriteManager::Get().mScreens[0].AddOam(mGlyphBuffers[1], mGlyphCounts[1], (s16) (0x80 - ((u32) mNameWidth >> 1)), 8, 0,
                                                   0x40, 0xf, 0);
}

void BattleCommandList::ClearText() {
    for (u8 i = 0; i < 3; i++) {
        MI_CpuClear16(mTextBuffers[i], mTextBufferSizes[i]);
    }
}

void BattleCommandList::DrawBakugan() {
    if (mUnk_14e < 0x2b) {
        mBakuganModel->Update();
        mBakuganModel->DrawModel();
        return;
    }
    mCells58[7]->DrawFrame(0, mUnk_150, 0x22, 0x4c, 0);
}

extern u8 data_020d43d7[];

void BattleCommandList::UpdatePage() {
    bool late = mUnk_14e >= 0x2b;
    if (mUnk_15f != 0) {
        mCells3c[2]->Draw(0, 0x80, 0x60, 1);
        return;
    }
    if (!late) {
        s16 x;
        u32 count;
        u32 max;
        count = 0;
        max = 0;
        u8 list[6];
        for (u8 i = 0; i < 6; i++) {
            u32 v = mValues[i];
            if (v != 0) {
                list[count] = i;
                count = (u8) (count + 1);
                if (max < v) {
                    max = v;
                }
            }
        }
        if (mCountUpTimer != 0) {
            mCountUpTimer--;
        } else if (mCountUpRemaining != 0) {
            mCountUpRemaining--;
            mValues[list[0]]++;
            if (mCountUpRemaining % 4 == 0) {
                SoundManager::Get().StartSeqArc(8);
            }
        }
        if (count != 0) {
            if (max != 0) {
                x = 0x38;
                if ((s32) max < 10) {
                    x -= 7;
                }
                mCells58[1]->Draw(0, (s16) (x + 0x5a), 0xae, 0);
                do {
                    u8 digit = (s32) max % 10;
                    x -= 10;
                    mCells58[0]->DrawFrame(0, digit, x + 0x58, 0xae, 0);
                    max /= 10;
                } while (max != 0);
            }
            u8 i = 0;
            do {
                mCells3c[0]->DrawFrame(0, list[i], 0x58 - (count - i - 1) * 0x18, 0xa8, 0);
                i++;
            } while (i < count);
        }
        s16 x3;
        u32 val = mCountUpRemaining;
        if (val != 0) {
            x3 = 0xe6;
            for (u8 i = 0; i < 3; i++) {
                u8 digit = (s32) val % 10;
                x3 -= 10;
                mCells58[0]->DrawFrame(0, digit, x3, 0xae, 0);
                val /= 10;
                if (val == 0) {
                    break;
                }
            }
            mCells58[2]->Draw(0, 0xc0, 0xae, 0);
        }
    }
    if (mPlayer > -1) {
        u8 cur = data_020d433c.mBattlePlayers->mUnk_2a9;
        BOOL has;
        if (mPlayer == cur) {
            mCells58[4]->DrawFrame(0, mPlayer, 0x24, 0x18, 0);
        } else {
            has = 0;
            if (data_020d433c.mBattlePlayers->mPlayers[(u8) mPlayer]->mCpu != NULL) {
                has = 1;
            }
            if (has == 1) {
                mCells58[5]->DrawFrame(0, mPlayer, 0x15, 0x12, 0);
            }
            if ((data_020d433c.mHasPartners == 1) & (mPlayer == data_020d43d7[cur])) {
                mCells58[6]->DrawFrame(0, mPlayer, 0x24, 0x18, 0);
            }
        }
        mCells58[3]->DrawFrame(0, mPlayer, 0, 0x16, 0);
    }
    mCells3c[3]->Draw(0, 0x80, 0x60, 1);
    if (!late && mUnk_15b == 0) {
        for (u8 i = 0; i < 5; i++) {
            u8 target = mUnk_0c[i].mTarget;
            u8 cur    = mUnk_0c[i].mCur;
            if (cur != target) {
                if (cur < target) {
                    mUnk_0c[i].mCur = cur + 1;
                } else {
                    mUnk_0c[i].mCur = cur - 1;
                }
            }
            mCells3c[5]->DrawFrame(0, mUnk_0c[i].mCur + 1, 0x84, i * 0x1a + 0x2a, 1);
            mCells3c[6]->DrawFrame(0, mUnk_0c[i].mCur + 1, 0xe1, i * 0x1a + 0x1e, 1);
        }
        mCells3c[2]->Draw(0, 0x80, 0x60, 1);
    } else {
        for (u8 i = 0; i < 0x20; i++) {
            SpriteManager::Get().Screen(0).AddOam((void *) data_0209e588, 1, (s16) ((i % 4) * 0x20 + 0x73),
                                                         (s16) ((i / 4) * 0x10 + 0x20), 3, (u16) (i << 1), 0xf, 0);
        }
    }
    if (late) {
        return;
    }
    SpriteManager::Get().Screen(0).AddOam(mGlyphBuffers[2], mGlyphCounts[2], 0x2a, 0x8e, 0, 0x60, 0xf, 0);
    if (mUnk_162 != 0) {
        mCells3c[4]->Draw(0, 0x80, 0x60, 1);
    }
}

void BattleCommandList::func_0205aea4() {
    for (u8 i = 0; i < 12; i++) {
        SpriteManager::Get().Screen(0).AddOam((void *) data_0209e57c, 1, (s16) ((i % 4) << 6), (s16) ((i / 4) << 6), 3,
                                                     (u16) (0x280 + i * 0x20), 0, 0);
    }
}

void BattleCommandList::ClearValues() {
    for (u8 i = 0; i < 6; i++) {
        mValues[i] = 0;
    }
}

void func_0205afb4(BattleCommandList *list, u32 a, u32 b, BattleCellPair *pair) {
    list->ClearValues();
    list->SetValue(a, b);
    if (list->mUnk_161 != 0) {
        switch (list->mUnk_14f) {
            case 5:
                list->SetValue(3, b);
                break;
            case 6:
                list->SetValue(0, b);
                break;
        }
    }
    list->func_0205b0dc(pair);
}

void BattleCommandList::SetValue(u32 index, u32 value) {
    if (index < 6) {
        mValues[index] = value;
    }
}

void BattleCommandList::StartCountUp(u32 value) {
    if (mCountUpRemaining != 0) {
        return;
    }
    SoundManager::Get().StartSeqArc(8);
    mCountUpTimer = 0x3c;
    mCountUpRemaining = value;
}

BOOL BattleCommandList::IsCountingUp() {
    return mCountUpRemaining != 0 ? 1 : 0;
}

void BattleCommandList::func_0205b0dc(const BattleCellPair *pair) {
    if (pair == NULL) {
        for (u8 i = 0; i < 5; i++) {
            mUnk_16[i] = data_0209e5bc[mUnk_14f][i];
        }
        mUnk_164 = 0;
    } else {
        mUnk_16[0] = pair->mUnk_02_0;
        mUnk_16[2] = pair->mUnk_02_3;
        mUnk_16[3] = pair->mUnk_02_6;
        mUnk_16[4] = pair->mUnk_02_9;
        mUnk_16[1] = pair->mUnk_02_12;
        mUnk_164   = pair->mUnk_00_0;
    }
    for (u8 i = 0; i < 5; i++) {
        mUnk_0c[i].mTarget = mUnk_16[i];
    }
    if (mBakuganDisplayMes != NULL) {
        PrintText(mTextPage);
    }
}

   // 0x20 bytes cleared before the first text of a page

void func_0205b1c4(u32 index, u32 unk2) {
    UnkStruct_02023248 *entry = GetBakuganEntry(gSaveData, index, unk2);
    entry->mUnk_0  = data_0209e5bc[index][0];
    entry->mUnk_3  = data_0209e5bc[index][2];
    entry->mUnk_6  = data_0209e5bc[index][3];
    entry->mUnk_9  = data_0209e5bc[index][4];
    entry->mUnk_12 = data_0209e5bc[index][1];
}

