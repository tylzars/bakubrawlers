#include "UI/SelectWindow.hpp"

// Placeholder for an unidentified object. The original's data layout needs one more participant in this file's
// size sort, ahead of the tables: a small weak object that no instruction addresses (nothing in the unit's ranges is
// left over for it, and the Random and wireless-packet globals as inline statics change other units' code). This
// accessor's static joins the sort here, its call leaves no code, and the link strips it.
inline u32 *DataLayoutSlot() {
    static u32 sSlot;
    return &sSlot;
}
static const u8 sFieldSelectPairColumnX[4] = {0x1a, 0x44, 0x94, 0xbe};
// The mFiles set UploadFieldFiles uploads for a field item; also mCells[4]'s frame.
static const u8 sFieldSelectSubFileIndices[10] = {0x6, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x8, 0x7, 0x9};
static const WindowItemRecord sFieldSelectWindow3Items[1] = {
    {0, 0x1, 9, 170, 86, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};
static const WindowItemRecord sFieldSelectWindow2Items[1] = {
    {0, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};
static const WindowItemRecord sFieldSelectWindow1Items[5][6] = {
    {
        {0, 0x1, 56, 86, 144, 18, 0, {0, 0, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {1, 0x1, 56, 110, 144, 18, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {2, 0x1, 56, 134, 144, 18, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {3, WINDOW_ITEM_CURSOR_SKIP, 128, 96, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
        {4, 0x1, 169, 200, 0, 0, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {5, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    },
    {
        {0, 0x1, 56, 86, 144, 18, 0, {1, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {1, 0x1, 56, 110, 144, 18, 0, {0, 0, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {2, 0x1, 56, 134, 144, 18, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {3, WINDOW_ITEM_CURSOR_SKIP, 128, 96, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
        {4, 0x1, 169, 200, 0, 0, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {5, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    },
    {
        {0, 0x1, 56, 86, 144, 18, 0, {2, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {1, 0x1, 56, 110, 144, 18, 0, {0, 2, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {2, 0x1, 56, 134, 144, 18, 0, {1, 0, 2, 2}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {3, WINDOW_ITEM_CURSOR_SKIP, 128, 96, 0, 0, 0, {-1, -1, -1, -1}, 0, 0},
        {4, 0x1, 169, 200, 0, 0, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        {5, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    },
    {
        {0, 0x1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {1, 0x2, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {2, WINDOW_ITEM_CURSOR_SKIP, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
    },
    {
        {0, 0x1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {1, 0x1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, WINDOW_TOUCH_RELEASE_IF_SELECTED},
        {2, WINDOW_ITEM_CURSOR_SKIP, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
    },
};
static const WindowCellRecord sFieldSelectWindow0Cells[2] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0x0, 0, 1, 1, {0, 1, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x0, 0, 1, 1, {2, 3, 3, 0}},
};
static const WindowCellRecord sFieldSelectWindow3Cells[1] = {
    {"/Game/Battle/Obj/select_field_b_obj_00_nce.bin", 5, 0x8000, 8, 1, 1, {5, 6, 6, 0}},
};
static const WindowCellRecord sFieldSelectWindow2Cells[1] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x0, 0, 1, 1, {2, 3, 3, 0}},
};
static const struct {
    u8 ids[17];
    WindowItemRecord items[2][2];
} sIdItems = {
    {0x15, 0x0, 0x1, 0x2, 0x4, 0x3, 0xe, 0xd, 0x10, 0xf, 0x8, 0xc, 0xb, 0xa, 0x9, 0x6, 0x7},
    {
        {
            {0, 0x1, 169, 170, 52, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
            {1, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        },
        {
            {0, 0x1, 169, 200, 0, 0, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
            {1, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
        },
    },
};
static const WindowCellRecord sFieldSelectWindow1Cells[5][6] = {
    {
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 12, 0x8000, 4, 0, 1, {11, 13, 12, 0}},
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 18, 0x8000, 4, 0, 1, {18, 18, 18, 0}},
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 22, 0x8000, 4, 0, 1, {22, 22, 22, 0}},
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 23, 0x8000, 4, 0, 1, {23, 23, 23, 0}},
        {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0x0, 0, 1, 1, {0, 1, 1, 0}},
        {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x0, 0, 1, 1, {2, 3, 3, 0}},
    },
    {
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 11, 0x8000, 4, 0, 1, {11, 13, 12, 0}},
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 16, 0x8000, 4, 0, 1, {15, 17, 16, 0}},
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 22, 0x8000, 4, 0, 1, {22, 22, 22, 0}},
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 23, 0x8000, 4, 0, 1, {23, 23, 23, 0}},
        {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0x0, 0, 1, 1, {0, 1, 1, 0}},
        {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x0, 0, 1, 1, {2, 3, 3, 0}},
    },
    {
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 11, 0x8000, 4, 0, 1, {11, 13, 12, 0}},
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 16, 0x8000, 4, 0, 1, {15, 17, 16, 0}},
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 19, 0x8000, 4, 0, 1, {19, 21, 20, 0}},
        {"/Game/Battle/Obj/select_ch_b_obj_00_nce.bin", 23, 0x8000, 4, 0, 1, {23, 23, 23, 0}},
        {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0x0, 0, 1, 1, {0, 1, 1, 0}},
        {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x0, 0, 1, 1, {2, 3, 3, 0}},
    },
    {
        {"/Game/2D/union_b/union_b_obj_window_nce.bin", 4, 0x1b000, 2, 0, 1, {3, 5, 4, 0}},
        {"/Game/2D/union_b/union_b_obj_window_nce.bin", 6, 0x1b000, 2, 0, 1, {6, 8, 7, 0}},
        {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x1b000, 2, 0, 1, {10, 10, 10, 0}},
    },
    {
        {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x8000, 8, 0, 1, {3, 5, 4, 0}},
        {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0x8000, 8, 0, 1, {6, 8, 7, 0}},
        {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x8000, 8, 0, 1, {10, 10, 10, 0}},
    },
};
static const u8 sFieldSelectFirstRowColumnX[4] = {0x1a, 0x52, 0x88, 0xbe};
static const u8 sFieldSelectPairSlots[4][4] = {{0x0, 0x1, 0x2, 0x3}, {0x0, 0x1, 0x2, 0x3}, {0x0, 0x2, 0x1, 0x3}, {0x0, 0x3, 0x1, 0x2}};
// [8] is the always-unlocked random entry: TakeCursorItem replaces it with an unlocked 0-7 before mFieldId is read, so
// its 0 is not used there.
static const u8 sFieldSelectFieldIds[9] = {0x0, 0x1, 0x2, 0x4, 0x5, 0x6, 0x3, 0x7, 0x0};

#include "Text/TextManager.hpp"
#include "System/WirelessManager.hpp"
#include "System/SoundManager.hpp"
#include "Scene/FieldSelectScene.hpp"
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "System/TouchManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include "Graphics/CharacterIcons.hpp"
#include "System/FrameTask.hpp"
#include "System/WirelessManager.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>


struct UnkStruct_020c4230 {
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u16 mUnk_20;
};
extern UnkStruct_020c4230 data_020c4230;
extern u8 data_020d43e3[];
extern "C" void MI_CpuCopy8(const void *src, void *dest, u32 size);

#define NET_KEYS (*(u16 *) ((u8 *) &data_0220f540 + 4))
#define NET_BYTE(n) (*((u8 *) &data_0220f540 + (n)))


static inline u8 IsZeroU8(u8 v) {
    return v == 0;
}

extern u8 data_020d43d7[];


extern u8 data_020d43eb[];

FieldSelectScene::FieldSelectScene(u8 unk1) : SceneBase(1000, 0) {
    (void) DataLayoutSlot();
    mWindows[0] = NULL;
    mWindows[1] = NULL;
    mWindows[2] = NULL;
    mWindows[3] = NULL;
    mMessageWindow = NULL;
    mPanel[0] = NULL;
    mPanel[1] = NULL;
    mMessages = NULL;
    mTextBuffer = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mStoryCharaNameMessages = NULL;
    mNameGlyphBuffer = NULL;
    mNameBufferSize = 0;
    mNameGlyphCount = 0;
    mState = 0;
    mPage = 0;
    mHasPartners = 0;
    mShowRulePage = 0;
    mBlend = 0x10;
    mBlendTarget = 0x10;
    mBlendStep = 1;
    mNextScene = 2;
    mLocalIsFirstSlot = 0;
    mPlayerRowsVisible = 0;
    mUnk_335 = 0;
    mCharaMarksVisible = 0;
    mFieldMarksVisible = 0;
    mPairGridVisible = 0;
    mNumberMarksVisible = 0;
    mMode = unk1;
    mLocalSlot = 0;
    mLocalTaken = 0;
    mWindow0ItemCount = 2;
    mLocalCursorSlot = 0;
    mUnk_340 = 0;
    mSavedFieldItem = 0;
    mPendingSubPlanes = -1;
    mMessageEnabled = 1;
    data_020d433c.mAntenna = NULL;
    MI_CpuFill8(mInputBytes, 0, sizeof(mInputBytes));
    MI_CpuClearFast(mCharacterLooks, sizeof(mCharacterLooks));
    MI_CpuFill8(&mMain, 0, sizeof(mMain));
    MI_CpuFill8(mCells, 0, sizeof(mCells));
    MI_CpuFill8(mCells2, 0, sizeof(mCells2));
    MI_CpuClear32(mFiles, sizeof(mFiles));
    MI_CpuClear32(mFileSizes, sizeof(mFileSizes));
    MI_CpuClear32(mNameBuffers, sizeof(mNameBuffers));
    MI_CpuFill8(mCharaNew, 0, sizeof(mCharaNew));
    MI_CpuFill8(mFieldNew, 0, sizeof(mFieldNew));
    u8 j;
    u8 i;
    WirelessManager::Get();
    const u8 *mode = &data_020c4258;
    if (IsWirelessMode(mode)) {
        mPlayerCount = mInputCount = WirelessManager::Get().GetRecvBlockCount();
    } else {
        mInputCount = 1;
        mPlayerCount = 2;
    }
    mLocalIsFirstSlot = 1;
    WirelessManager::Get();
    if (IsWirelessMode(mode)) {
        mLocalSlot = data_020d433c.mLocalSlot;
        if (mLocalSlot != 0) {
            mLocalIsFirstSlot = 0;
        }
    } else {
        mLocalSlot = 0;
    }
    for (i = 0; i < 0x12; i++) {
        if (i == 0 || i == 0x11) {
            mCharaUnlocked[mLocalSlot][i] = 3;
            mCharaNew[i]           = 0;
        } else {
            mCharaUnlocked[mLocalSlot][i] = 0;
            u8 f                  = sIdItems.ids[i];
            int byte = f / 8;
            for (u8 k = 0; k < 3; k++) {
                const u8 *flags = gUnlockedCharacterLevel0Bits;
                flags += k * 3;
                if (flags[byte] & (1 << (f % 8))) {
                    mCharaUnlocked[mLocalSlot][i] = k + 1;
                }
            }
            if (mCharaUnlocked[mLocalSlot][i] != 0) {
                if (!(gCharacterNotNewBits[byte] & (1 << (f % 8)))) {
                    mCharaNew[i] = 1;
                }
            }
        }
    }
    for (j = 0; j < 9; j++) {
        if (j == 8) {
            mFieldUnlocked[mLocalSlot][j] = 1;
        } else {
            u8 f = sFieldSelectFieldIds[j];
            if (TestFlagBit(gUnlockedFieldBits, f)) {
                mFieldUnlocked[mLocalSlot][j] = 1;
                if (!TestFlagBit(gFieldNotNewBits, f)) {
                    mFieldNew[j] = 1;
                }
            } else {
                mFieldUnlocked[mLocalSlot][j] = 0;
            }
        }
    }
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            mOptionBytes[i][j] = 0;
        }
        mOptionBytes[i][1] = 2;
    }
    mUnk_25c[1] = 3;
    u8 n = 0;
    if (mMode == 0) {
        WirelessManager::Get();
        if (IsWirelessMode(mode)) {
            n++;
            mPageKinds[0] = FIELD_SELECT_PAGE_WINDOW;
        }
        mPage = n;
        if (mPlayerCount < 4) {
            mPageKinds[n++] = FIELD_SELECT_PAGE_NUMBER;
        }
        u8 n1 = n + 1;
        u8 n2 = n1 + 1;
        u8 n3 = n2 + 1;
        u8 n4 = n3 + 1;
        mPageKinds[n]  = FIELD_SELECT_PAGE_CHARA;
        mPageKinds[n1] = FIELD_SELECT_PAGE_PAIR;
        mPageKinds[n2] = FIELD_SELECT_PAGE_FIELD;
        mPageKinds[n3] = FIELD_SELECT_PAGE_RULE;
        mPageKinds[n4] = FIELD_SELECT_PAGE_END;
    }
    data_020d433c.mSlotsPerPlayer = 3;
    data_020d433c.mUnk_a7 = 0;
    data_020d433c.mRules = gSaveData[0].mRules;
    mUnk_25c[0]           = gSaveData[0].mRuleFieldVariant;
    mUnk_25c[2] = data_020d433c.mUnk_9a_3 == 0;
    mUnk_25c[4] = data_020d433c.mUnk_9a_2 == 0;
    mUnk_25c[6] = data_020d433c.mUnk_9a_4 == 0;
    mUnk_25c[8] = data_020d433c.mMicEnabled == 0;
    data_020d433c.mFieldVariant = mUnk_25c[0];
}

FieldSelectScene::~FieldSelectScene() {
    if (mMain.mCellAnim != NULL) {
        delete mMain.mCellAnim;
    }
    for (u8 i = 0; i < 9; i++) {
        if (mCells[i].mCellAnim != NULL) {
            delete mCells[i].mCellAnim;
        }
    }
    CellAnimation *anim;
    u8 i;
    for (i = 0; i < 9; i++) {
        anim = mCells2[i].mCellAnim;
        if (anim != NULL) {
            delete anim;
        }
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mStoryCharaNameMessages != NULL) {
        OS_Free(mStoryCharaNameMessages);
    }
    if (mNameGlyphBuffer != NULL) {
        OS_Free(mNameGlyphBuffer);
    }
    for (u8 i = 0; i < 4; i++) {
        if (mNameBuffers[i] != NULL) {
            OS_Free(mNameBuffers[i]);
        }
    }
    if (mWindows[0] != NULL) {
        delete mWindows[0];
    }
    if (mWindows[1] != NULL) {
        delete mWindows[1];
    }
    if (mWindows[2] != NULL) {
        delete mWindows[2];
    }
    if (mWindows[3] != NULL) {
        delete mWindows[3];
    }
    if (mMessageWindow != NULL) {
        delete mMessageWindow;
    }
    CharacterIcons *panel0 = mPanel[0];
    if (panel0 != NULL) {
        panel0->~CharacterIcons();
        operator delete(panel0);
    }
    CharacterIcons *panel1 = mPanel[1];
    if (panel1 != NULL) {
        panel1->~CharacterIcons();
        operator delete(panel1);
    }
    for (u8 i = 0; i < 10; i++) {
        for (u8 j = 0; j < 3; j++) {
            if (mFiles[i][j] != NULL) {
                OS_Free(mFiles[i][j]);
            }
        }
    }
    if (data_020d433c.mWireless != 0) {
        data_020d433c.mAntenna->mFinished = 1;
    }
}

void FieldSelectScene::Update() {
    func_ov001_02222040();
    switch (mState) {
        case 0:
            InitDisplay();
            LoadGraphics();
            LoadMessages();
            LoadNameText();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 1:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                mState++;
            } else {
                MIi_CpuCopyFast(gCharacterLook, mCharacterLooks, 8);
                mState = 4;
            }
            break;
        case 2:
            if (func_ov001_02221c0c()) {
                mState++;
            }
            break;
        case 3:
            if (ExchangeCharacterLooks()) {
                mState++;
            }
            break;
        case 4:
            MATHRandContext32 *ctx = &gRandom;
            ctx->x                 = ctx->mul * ctx->x + ctx->add;
            data_0220f540.mUnk_04  = (u32) (ctx->x >> 32);
            if (SetMessageTypeAndCheckAll(2)) {
                u8 *p = WirelessManager::Get().GetRecvBlock(0);
                if (p != NULL) {
                    SeedRandom(*(u32 *) (p + 4));
                }
                mState++;
            }
            break;
        case 5: {
            u32 r = UpdatePage();
            if (r == 0) {
                break;
            }
            if (r == 3) {
                mState++;
            } else {
                if (r == 1) {
                    MarkCharasAndFieldNotNew();
                    func_ov001_02226d74();
                }
                mState = 8;
            }
            break;
        }
        case 6:
            WirelessManager::Get().Stop();
            mState++;
            break;
        case 7:
            WirelessManager::Get();
            if (data_020c4030.mUnk_228 == 0) {
                mState++;
            }
            break;
        case 8:
            if (mPendingSubPlanes != -1) {
                G2S_SetBG1Priority(0);
                GXS_SetVisiblePlane(mPendingSubPlanes);
                mPendingSubPlanes = -1;
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 9:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 10:
            RequestScene(mNextScene);
            mFinished = 1;
            break;
    }
    Draw();
}

void FieldSelectScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 1);
    G2_SetBG2Control(0, 1, 4, 7, 0);
    G2_SetBG3Control(0, 1, 2, 4, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 1);
    G2S_SetBG2Control(0, 1, 4, 7, 0);
    G2S_SetBG3Control(0, 1, 2, 4, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(0);
    G2S_SetBG3Priority(3);
    GX_SetVisiblePlane(0x1a);
    GXS_SetVisiblePlane(0x1a);
    G2_SetBlendBrightness(0x1b, 0);
    G2S_SetBlendBrightness(0x1b, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
    G2_SetWndOutsidePlane(0x3f, FALSE);
    GXS_SetVisibleWnd(1);
    G2S_SetWndOutsidePlane(0x3f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0xff, 0x12);
}

void FieldSelectScene::LoadGraphics() {
    ChangeDir("/Game/Battle/Bg/");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0x6000, "select_arena_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG1_CHAR, 0, "select_arena_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG1_SCR, 0, "select_arena_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x6000, "select_arena_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "select_arena_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "select_arena_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG2_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG2_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.LoadCharacters("select_arena_u_obj_00.nce_ncg1D.bin", 0, 0);
    main.LoadPalette("select_arena_u_obj_00_ncl.bin", 0, 0);
    if (mPageKinds[mPage] == FIELD_SELECT_PAGE_NUMBER) {
        mMain.mCellAnim = new CellAnimation("select_arena_u_obj_00_nce.bin", 6, 1, 1);
    } else {
        mMain.mCellAnim = new CellAnimation("select_arena_u_obj_00_nce.bin", 3, 1, 1);
    }
    mMain.mCellAnim->mCharBase = main.GetCharBase(0);
    mMain.mCellAnim->mPaletteBase   = 0;
    mMain.mX               = 0x80;
    mMain.mY               = 0x60;
    mMain.mUnk_09          = 0;
    mMain.mVisible         = 1;
    ChangeDir("/Game/2D/union_b/");
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("union_b_obj_cockpit_ncl.bin", 0, 0);
    ChangeDir("/Game/Battle/Bg/");
    char name[0x80];
    for (u8 i = 0; i < 10; i++) {
        sprintf(name, "select_field_u_sub_%02d_ncl.bin", i);
        mFiles[i][1] = LoadFile(name, 0, 0, 0, &mFileSizes[i][1], 0);
        sprintf(name, "select_field_u_sub_%02d_ncg.bin", i);
        mFiles[i][0] = LoadFile(name, 0, 0, 0, &mFileSizes[i][0], 0);
        sprintf(name, "select_field_u_sub_%02d_nsc.bin", i);
        mFiles[i][2] = LoadFile(name, 0, 0, 0, &mFileSizes[i][2], 0);
    }
    ChangeDir("/");
    mMessageWindow = new MessageWindow("mes_BattleArenaMenu", 1, 0x7000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mWindow0ItemCount = 2;
    if (mLocalIsFirstSlot == 0) {
        mWindow0ItemCount--;
    }
    mPanel[0] = new CharacterIcons(0x12000, 0xb, 0);
    mPanel[1] = new CharacterIcons(0x1f000, 0xb, 1);
    if (data_020d433c.mWireless == 0) {
        return;
    }
    if (data_020d433c.mAntenna != NULL) {
        data_020d433c.mAntenna->SetupLayout(2);
    } else {
        data_020d433c.mAntenna = new AntennaIndicator(2);
    }
}

void FieldSelectScene::SetupPage() {
    MI_CpuClearFast((void *) 0x6004000, 0xc000);
    MI_CpuClearFast((void *) 0x6204000, 0xc000);
    if (mWindows[0] != NULL) {
        delete mWindows[0];
    }
    u32 variant;
    switch (mPageKinds[mPage]) {
        case FIELD_SELECT_PAGE_NUMBER:
        case FIELD_SELECT_PAGE_RULE:
        case FIELD_SELECT_PAGE_PAIR:
        case FIELD_SELECT_PAGE_FIELD:
            variant = 1;
            break;
        default:
            variant = 0;
            break;
    }
    mWindows[0] = new SelectWindow(mWindow0ItemCount, sIdItems.items[variant], 0);
    mWindows[0]->SetCells(mWindow0ItemCount, sFieldSelectWindow0Cells);
    mWindows[0]->mCursor->mSelection = -1;
    mWindows[0]->mEnabled            = 0;
    if (mPageKinds[mPage] == FIELD_SELECT_PAGE_CHARA) {
        mWindows[0]->mButtons[0].mEnabled = 0;
    }
    mPlayerRowsVisible = 0;
    mUnk_335 = 0;
    mCharaMarksVisible = 0;
    mFieldMarksVisible = 0;
    mPairGridVisible = 0;
    mNumberMarksVisible = 0;
    if (mWindows[1] != NULL) {
        delete mWindows[1];
    }
    mWindows[1] = NULL;
    if (mWindows[2] != NULL) {
        delete mWindows[2];
    }
    mWindows[2] = NULL;
    if (mWindows[3] != NULL) {
        delete mWindows[3];
    }
    mWindows[3] = NULL;
    mLocalTaken = 0;
    for (u8 i = 0; i < 4; i++) {
        mInput[i].mWindowOpen = 0;
    }
    mLocalCursorSlot = mLocalSlot;
    mUnk_340 = 0;
    mBlendTimer = 0;
    u8 i;
    for (i = 0; i < 9; i++) {
        CellAnimation *anim = mCells[i].mCellAnim;
        if (anim != NULL) {
            delete anim;
        }
        mCells[i].mCellAnim    = NULL;
        mCells[i].mUpdateAnim  = 1;
        mCells[i].mVisible = 0;
        mCells[i].mUnk_09  = 1;
    }
    for (i = 0; i < 9; i++) {
        CellAnimation *anim = mCells2[i].mCellAnim;
        if (anim != NULL) {
            delete anim;
        }
        mCells2[i].mCellAnim    = NULL;
        mCells2[i].mUpdateAnim  = 1;
        mCells2[i].mVisible = 0;
        mCells2[i].mUnk_09  = 1;
    }
    switch (mPageKinds[mPage]) {
        case FIELD_SELECT_PAGE_NUMBER: {
            if (mLocalIsFirstSlot != 0) {
                mMessageWindow->StartMessage(0x13, 0, 1);
            } else {
                mMessageWindow->StartMessage(4, 1, 1);
            }
            mMain.mCellAnim->SetAnimation(6);
            if (mInputCount > 2) {
                mInput[0].mCursorItem = 1;
            }
            SpriteScreen &sub = SpriteManager::Get().Screen(1);
            ChangeDir("/Game/Character/Icon/");
            sub.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0x13000, 0);
            sub.LoadPalette("icon_ch_base_ncl.bin", 0x40, 0);
            mCells2[7].mCellAnim            = new CellAnimation("icon_ch_base_nce.bin", 0xb, 1, 1);
            mCells2[7].mCellAnim->mCharBase = sub.GetCharBase(0x13000);
            mCells2[7].mCellAnim->mPaletteBase   = 2;
            mCells2[7].mVisible         = 0;
            ChangeDir("/Game/Battle/Obj/");
            sub.LoadCharacters("select_number_b_obj_00.nce_ncg1D.bin", 0x8000, 0);
            sub.LoadPalette("select_number_b_obj_00_ncl.bin", 0x100, 0);
            s16 y = 0x30;
            u8 n  = 0;
            if (mInputCount > 2) {
                y -= 0x10;
                n++;
            }
            do {
                mCells2[n].mCellAnim            = new CellAnimation("select_number_b_obj_00_nce.bin", n & 0xff, 1, 1);
                mCells2[n].mCellAnim->mCharBase = sub.GetCharBase(0x8000);
                mCells2[n].mCellAnim->mPaletteBase   = 8;
                mCells2[n].mX               = 0x20;
                mCells2[n].mY               = n * 0x24 + y;
                mCells2[n].mUnk_09          = 3;
                mCells2[n].mVisible         = 1;
                n++;
            } while (n < 3);
            mCells2[mInput[0].mCursorItem].mCellAnim->SetAnimation(mInput[0].mCursorItem + 3);
            mNumberMarksVisible = 1;
            mBlendTimer = 0x10;
            break;
        }
        case FIELD_SELECT_PAGE_RULE: {
            if (mLocalIsFirstSlot != 0) {
                mMessageWindow->StartMessage(5, 0, 1);
            } else {
                mMessageWindow->StartMessage(4, 1, 1);
            }
            mMain.mCellAnim->SetAnimation(2);
            ChangeDir("/Game/Battle/Bg/");
            if (mLocalIsFirstSlot != 0) {
                LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_rule_b_bg_00_ncl.bin", 0, 0, 0);
                LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "select_rule_b_bg_00_ncg.bin", 0, 0, 0);
                LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "select_rule_b_bg_00_nsc.bin", 0, 0, 0);
                ChangeDir("/Game/Battle/Obj/");
                SpriteScreen &sub = SpriteManager::Get().Screen(1);
                sub.LoadCharacters("select_rule_b_obj_00.nce_ncg1D.bin", 0x8000, 0);
                sub.LoadPalette("select_rule_b_obj_00_ncl.bin", 0x100, 0);
                u8 k = 0;
                do {
                    mCells2[k].mCellAnim            = new CellAnimation("select_rule_b_obj_00_nce.bin", 0, 1, 1);
                    mCells2[k].mCellAnim->mCharBase = sub.GetCharBase(0x8000);
                    mCells2[k].mCellAnim->mPaletteBase   = 8;
                    mCells2[k].mX               = 0x88;
                    mCells2[k].mY               = (k % 5) * 0x18 + 0x26;
                    mCells2[k].mUnk_09          = 2;
                    mCells2[k].mVisible         = 1;
                    k++;
                } while (k < 8);
                mCells2[5].mX = 0x79;
                mCells2[5].mCellAnim->SetAnimation(0);
                mCells2[6].mX = 0xe4;
                mCells2[6].mCellAnim->SetAnimation(3);
                mCells2[0].mCellAnim->SetAnimation(7);
                mCells2[0].mUpdateAnim = 0;
                mCells2[1].mCellAnim->SetAnimation(8);
                mCells2[1].mUpdateAnim = 0;
                mCells2[2].mCellAnim->SetAnimation(9);
                mCells2[2].mUpdateAnim = 0;
                mCells2[3].mCellAnim->SetAnimation(10);
                mCells2[3].mUpdateAnim = 0;
                mCells2[4].mCellAnim->SetAnimation(11);
                mCells2[4].mUpdateAnim = 0;
                mCells2[7].mCellAnim->SetAnimation(6);
                mCells2[7].mUnk_09 = 1;
            } else {
                LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_rule_u_bg_00_ncl.bin", 0, 0, 0);
                LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "select_rule_u_bg_00_ncg.bin", 0, 0, 0);
                LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "select_rule_u_bg_00_nsc.bin", 0, 0, 0);
                ChangeDir("/Game/Battle/Obj/");
                SpriteScreen &main = SpriteManager::Get().Screen(0);
                main.LoadCharacters("select_rule_u_obj_00.nce_ncg1D.bin", 0x8000, 0);
                main.LoadPalette("select_rule_u_obj_00_ncl.bin", 0x100, 0);
                u8 k = 0;
                do {
                    mCells[k].mCellAnim            = new CellAnimation("select_rule_u_obj_00_nce.bin", 0, 1, 1);
                    mCells[k].mCellAnim->mCharBase = main.GetCharBase(0x8000);
                    mCells[k].mCellAnim->mPaletteBase   = 8;
                    mCells[k].mX               = 0x88;
                    mCells[k].mY               = k * 0x18 + 0x26;
                    mCells[k].mUnk_09          = 1;
                    mCells[k].mVisible         = 1;
                    mCells[k].mUpdateAnim          = 0;
                    k++;
                } while (k < 5);
                mCells[0].mCellAnim->SetAnimation(0);
                mCells[1].mCellAnim->SetAnimation(1);
                mCells[2].mCellAnim->SetAnimation(2);
                mCells[3].mCellAnim->SetAnimation(3);
                mCells[4].mCellAnim->SetAnimation(4);
            }
            RefreshRuleOptions(0);
            break;
        }
        case FIELD_SELECT_PAGE_CHARA: {
            mMessageWindow->StartMessage(6, 0, 1);
            mMain.mCellAnim->SetAnimation(3);
            ChangeDir("/Game/Battle/Bg/");
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_ch_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "select_ch_b_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "select_ch_b_bg_00_nsc.bin", 0, 0, 0);
            u8 j = 0;
            do {
                mInput[j].mCursorItem = mInput[j].mTakenItem;
                j++;
            } while (j < mPlayerCount);
            ChangeDir("/Game/Battle/Obj/");
            SpriteScreen &sub = SpriteManager::Get().Screen(1);
            sub.LoadCharacters("select_ch_b_obj_00.nce_ncg1D.bin", 0x8000, 0);
            sub.LoadPalette("select_ch_b_obj_00_ncl.bin", 0x80, 0);
            u8 k = 0;
            do {
                mCells2[k].mCellAnim            = new CellAnimation("select_ch_b_obj_00_nce.bin", k & 0xff, 1, 1);
                mCells2[k].mCellAnim->mCharBase = sub.GetCharBase(0x8000);
                mCells2[k].mCellAnim->mPaletteBase   = 4;
                mCells2[k].mX               = GetCharaItemX(mInput[k].mCursorItem);
                mCells2[k].mY               = GetCharaItemY(mInput[k].mCursorItem);
                mCells2[k].mUnk_09          = 1;
                mCells2[k].mVisible         = 1;
                k++;
            } while (k < mPlayerCount);
            u8 m = 4;
            do {
                mCells2[m].mCellAnim            = new CellAnimation("select_ch_b_obj_00_nce.bin", m & 0xff, 1, 1);
                mCells2[m].mCellAnim->mCharBase = sub.GetCharBase(0x8000);
                mCells2[m].mCellAnim->mPaletteBase   = 4;
                mCells2[m].mX               = 0x80;
                mCells2[m].mY               = 0x60;
                mCells2[m].mUnk_09          = 3;
                mCells2[m].mVisible         = 0;
                m++;
            } while (m < 7);
            mCells2[4].mCellAnim->SetAnimation(5);
            mCells2[5].mCellAnim->SetAnimation(4);
            mCells2[6].mCellAnim->SetAnimation(0x18);
            ChangeDir("/Game/Battle/Obj/");
            SpriteScreen &main = SpriteManager::Get().Screen(0);
            SpriteScreen &sub2 = SpriteManager::Get().Screen(1);
            main.LoadCharacters("select_order_u_obj_00.nce_ncg1D.bin", 0x4000, 0);
            main.LoadPalette("select_order_u_obj_00_ncl.bin", 0xa0, 0);
            main.LoadCharacters("select_ch_u_obj_00.nce_ncg1D.bin", 0x8000, 0);
            main.LoadPalette("select_ch_u_obj_00_ncl.bin", 0x80, 0);
            u8 c = 0;
            do {
                u32 idx                           = c & 0xff;
                mCells[idx].mCellAnim            = new CellAnimation("select_ch_u_obj_00_nce.bin", 0, 1, 1);
                mCells[idx].mCellAnim->mCharBase = main.GetCharBase(0x8000);
                mCells[idx].mCellAnim->mPaletteBase   = 4;
                mCells[idx].mX               = 0x18;
                mCells[idx].mY               = c * 0x24 + 0x2a;
                mCells[idx].mUnk_09          = 3;
                mCells[idx].mVisible         = 1;
                mCells[idx].mUpdateAnim          = 0;
                c++;
            } while (c < mPlayerCount);
            mCells[4].mCellAnim            = new CellAnimation("select_ch_u_obj_00_nce.bin", 2, 1, 1);
            mCells[4].mCellAnim->mCharBase = main.GetCharBase(0x8000);
            mCells[4].mCellAnim->mPaletteBase   = 4;
            mCells[4].mVisible         = 0;
            ChangeDir("/Game/Character/Icon/");
            main.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0x10000, 0);
            main.LoadPalette("icon_ch_base_ncl.bin", 0x120, 0);
            mCells[6].mCellAnim            = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
            mCells[6].mCellAnim->mCharBase = main.GetCharBase(0x10000);
            mCells[6].mCellAnim->mPaletteBase   = 9;
            mCells[6].mVisible         = 0;
            mCells[7].mCellAnim            = new CellAnimation("icon_ch_base_nce.bin", 2, 1, 1);
            mCells[7].mCellAnim->mCharBase = main.GetCharBase(0x10000);
            mCells[7].mCellAnim->mPaletteBase   = 9;
            mCells[7].mVisible         = 0;
            sub2.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0x13000, 0);
            sub2.LoadPalette("icon_ch_base_ncl.bin", 0x40, 0);
            mCells2[7].mCellAnim            = new CellAnimation("icon_ch_base_nce.bin", 2, 1, 1);
            mCells2[7].mCellAnim->mCharBase = sub2.GetCharBase(0x13000);
            mCells2[7].mCellAnim->mPaletteBase   = 2;
            mCells2[7].mVisible         = 0;
            ChangeDir("/Game/2D/union_b/");
            sub2.LoadCharacters("union_b_obj_new.nce_ncg1D.bin", 0x14400, 0);
            sub2.LoadPalette("union_b_obj_new_ncl.bin", 0x20, 0);
            mCells2[8].mCellAnim            = new CellAnimation("union_b_obj_new_nce.bin", 0, 1, 1);
            mCells2[8].mCellAnim->mCharBase = sub2.GetCharBase(0x14400);
            mCells2[8].mCellAnim->mPaletteBase   = 1;
            mCells2[8].mUnk_09          = 3;
            mCells2[8].mVisible         = 0;
            ChangeDir("/");
            u8 p = 0;
            do {
                mPanel[0]->SetCharacter(p, -1, 0, 0);
                mPanel[1]->SetCharacter(p, -1, 0, 0);
                ClearName(p);
                if (IsWirelessMode((const u8 *) &WirelessManager::Get().mState) && p < mInputCount) {
                    PrintSlotName(p);
                }
                p++;
            } while (p < 4);
            mPanel[1]->SetCharacter(0, 0x15, (u32) (mCharacterLooks + mLocalSlot * 8), 0);
            s16 px = GetCharaItemX(0);
            s16 py = GetCharaItemY(0);
            mPanel[1]->SetDrawArgs(0, px + 6, py + 3, 1);
            mPlayerRowsVisible = 1;
            mUnk_335 = 1;
            mCharaMarksVisible = 1;
            mBlendTimer = 0x10;
            mWindows[2] = new SelectWindow(1, sFieldSelectWindow2Items, 0);
            mWindows[2]->SetCells(1, sFieldSelectWindow2Cells);
            mWindows[2]->mCursor->mSelection = -1;
            break;
        }
        case FIELD_SELECT_PAGE_PAIR: {
            mPlayerCount = data_020d433c.mPlayerCount;
            if (mLocalIsFirstSlot != 0) {
                mMessageWindow->StartMessage(0x11, 0, 1);
            } else {
                mMessageWindow->StartMessage(4, 1, 1);
            }
            mMain.mCellAnim->SetAnimation(4);
            ChangeDir("/Game/Battle/Obj/");
            SpriteScreen &sub = SpriteManager::Get().Screen(1);
            sub.LoadCharacters("select_ch_b_obj_00.nce_ncg1D.bin", 0x8000, 0);
            sub.LoadPalette("select_ch_b_obj_00_ncl.bin", 0x80, 0);
            u8 k = 0;
            do {
                if (k == 0) {
                    mCells2[k].mCellAnim = new CellAnimation("select_ch_b_obj_00_nce.bin", 0x19, 1, 1);
                } else {
                    mCells2[k].mCellAnim = new CellAnimation("select_ch_b_obj_00_nce.bin", 7, 1, 1);
                }
                mCells2[k].mCellAnim->mCharBase = sub.GetCharBase(0x8000);
                mCells2[k].mCellAnim->mPaletteBase   = 4;
                mCells2[k].mX               = 0x10;
                mCells2[k].mY               = k * 0x24 + 0x1a;
                mCells2[k].mUnk_09          = 3;
                mCells2[k].mVisible         = 1;
                k++;
            } while (k < 5);
            mCells2[0].mCellAnim->SetAnimation(0x1a);
            mCells2[4].mCellAnim->SetAnimation(6);
            mCells2[4].mX      = 0x10;
            mCells2[4].mY      = 0x1a;
            mCells2[4].mUnk_09 = 2;
            ChangeDir("/Game/Character/Icon/");
            SpriteScreen &sub2 = SpriteManager::Get().Screen(1);
            sub2.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0x13000, 0);
            sub2.LoadPalette("icon_ch_base_ncl.bin", 0x40, 0);
            mCells2[5].mCellAnim            = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
            mCells2[5].mCellAnim->mCharBase = sub2.GetCharBase(0x13000);
            mCells2[5].mCellAnim->mPaletteBase   = 2;
            mCells2[5].mVisible         = 0;
            mCells2[7].mCellAnim            = new CellAnimation("icon_ch_base_nce.bin", 2, 1, 1);
            mCells2[7].mCellAnim->mCharBase = sub2.GetCharBase(0x13000);
            mCells2[7].mCellAnim->mPaletteBase   = 2;
            mCells2[7].mVisible         = 0;
            ChangeDir("/Game/Battle/Obj/");
            SpriteScreen &main = SpriteManager::Get().Screen(0);
            main.LoadCharacters("select_order_u_obj_00.nce_ncg1D.bin", 0x4000, 0);
            main.LoadPalette("select_order_u_obj_00_ncl.bin", 0xa0, 0);
            main.LoadCharacters("select_ch_u_obj_00.nce_ncg1D.bin", 0x8000, 0);
            main.LoadPalette("select_ch_u_obj_00_ncl.bin", 0x80, 0);
            u8 c = 0;
            do {
                u32 idx                           = c & 0xff;
                mCells[idx].mCellAnim            = new CellAnimation("select_ch_u_obj_00_nce.bin", 0, 1, 1);
                mCells[idx].mCellAnim->mCharBase = main.GetCharBase(0x8000);
                mCells[idx].mCellAnim->mPaletteBase   = 4;
                mCells[idx].mX               = 0x18;
                mCells[idx].mY               = c * 0x24 + 0x2a;
                mCells[idx].mUnk_09          = 3;
                mCells[idx].mVisible         = 1;
                mCells[idx].mUpdateAnim          = 0;
                c++;
            } while (c < mPlayerCount);
            mCells[4].mCellAnim            = new CellAnimation("select_ch_u_obj_00_nce.bin", 2, 1, 1);
            mCells[4].mCellAnim->mCharBase = main.GetCharBase(0x8000);
            mCells[4].mCellAnim->mPaletteBase   = 4;
            mCells[4].mVisible         = 0;
            ChangeDir("/Game/Character/Icon/");
            main.LoadCharacters("icon_ch_base.nce_ncg1D.bin", 0x10000, 0);
            main.LoadPalette("icon_ch_base_ncl.bin", 0x120, 0);
            mCells[6].mCellAnim            = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
            mCells[6].mCellAnim->mCharBase = main.GetCharBase(0x10000);
            mCells[6].mCellAnim->mPaletteBase   = 9;
            mCells[6].mVisible         = 0;
            mCells[7].mCellAnim            = new CellAnimation("icon_ch_base_nce.bin", 2, 1, 1);
            mCells[7].mCellAnim->mCharBase = main.GetCharBase(0x10000);
            mCells[7].mCellAnim->mPaletteBase   = 9;
            mCells[7].mVisible         = 0;
            mPlayerRowsVisible = 1;
            mPairGridVisible = 1;
            UpdatePairCells();
            u8 p = 0;
            do {
                mPanel[0]->SetCharacter(p, -1, 0, 0);
                mPanel[1]->SetCharacter(p, -1, 0, 0);
                if (p < mPlayerCount) {
                    mPanel[0]->SetCharacter(p, (s8) sIdItems.ids[mInput[p].mTakenItem], (u32) (mCharacterLooks + p * 8), 0);
                    mPanel[1]->SetCharacter(p, (s8) sIdItems.ids[mInput[p].mTakenItem], (u32) (mCharacterLooks + p * 8), 0);
                }
                PrintSlotName(p);
                p++;
            } while (p < 4);
            mBlendTimer = 0x10;
            break;
        }
        case FIELD_SELECT_PAGE_FIELD: {
            mPlayerCount = data_020d433c.mPlayerCount;
            if (mLocalIsFirstSlot != 0) {
                mMessageWindow->StartMessage(8, 0, 1);
            } else {
                mMessageWindow->StartMessage(4, 1, 1);
            }
            mMain.mCellAnim->SetAnimation(5);
            ChangeDir("/Game/Battle/Bg/");
            LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "select_field_b_bg_00_ncl.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "select_field_b_bg_00_ncg.bin", 0, 0, 0);
            LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "select_field_b_bg_00_nsc.bin", 0, 0, 0);
            ChangeDir("/Game/Battle/Obj/");
            SpriteScreen &sub = SpriteManager::Get().Screen(1);
            sub.LoadCharacters("select_field_b_obj_00.nce_ncg1D.bin", 0x8000, 0);
            sub.LoadPalette("select_field_b_obj_00_ncl.bin", 0x100, 0);
            mInput[0].mCursorItem           = mSavedFieldItem;
            mCells2[0].mCellAnim            = new CellAnimation("select_field_b_obj_00_nce.bin", 0, 1, 1);
            mCells2[0].mCellAnim->mCharBase = sub.GetCharBase(0x8000);
            mCells2[0].mCellAnim->mPaletteBase   = 8;
            mCells2[0].mX               = GetFieldItemX(mInput[0].mCursorItem);
            mCells2[0].mY               = GetFieldItemY(mInput[0].mCursorItem);
            mCells2[0].mUnk_09          = 1;
            mCells2[0].mVisible         = 1;
            mCells2[4].mCellAnim            = new CellAnimation("select_field_b_obj_00_nce.bin", 4, 1, 1);
            mCells2[4].mCellAnim->mCharBase = sub.GetCharBase(0x8000);
            mCells2[4].mCellAnim->mPaletteBase   = 4;
            mCells2[4].mUnk_09          = 3;
            mCells2[4].mVisible         = 0;
            SpriteScreen &main          = SpriteManager::Get().Screen(0);
            main.LoadCharacters("select_field_u_obj_00.nce_ncg1D.bin", 0x8000, 0);
            main.LoadPalette("select_field_u_obj_00_ncl.bin", 0x80, 0);
            mCells[4].mCellAnim            = new CellAnimation("select_field_u_obj_00_nce.bin", 0, 1, 1);
            mCells[4].mCellAnim->mCharBase = main.GetCharBase(0x8000);
            mCells[4].mCellAnim->mPaletteBase   = 4;
            mCells[4].mX               = 0x80;
            mCells[4].mY               = 0x60;
            mCells[4].mUnk_09          = 3;
            mCells[4].mVisible         = 1;
            mCells[4].mUpdateAnim          = 0;
            ChangeDir("/Game/2D/union_b/");
            SpriteScreen &sub2 = SpriteManager::Get().Screen(1);
            sub2.LoadCharacters("union_b_obj_new.nce_ncg1D.bin", 0x14400, 0);
            sub2.LoadPalette("union_b_obj_new_ncl.bin", 0x20, 0);
            mCells2[8].mCellAnim            = new CellAnimation("union_b_obj_new_nce.bin", 0, 1, 1);
            mCells2[8].mCellAnim->mCharBase = sub2.GetCharBase(0x14400);
            mCells2[8].mCellAnim->mPaletteBase   = 1;
            mCells2[8].mUnk_09          = 1;
            mCells2[8].mVisible         = 0;
            SpriteScreen &sub3 = SpriteManager::Get().Screen(1);
            sub3.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x1b000, 0);
            sub3.LoadPalette("union_b_obj_window_ncl.bin", 0x40, 0);
            mFieldMarksVisible = 1;
            UploadFieldFiles(mInput[0].mCursorItem);
            mBlendTimer    = 0x10;
            mWindows[3] = new SelectWindow(1, sFieldSelectWindow3Items, 0);
            mWindows[3]->SetCells(1, sFieldSelectWindow3Cells);
            mWindows[3]->mCursor->mSelection = -1;
            break;
        }
        case FIELD_SELECT_PAGE_WINDOW: {
            if (mLocalIsFirstSlot != 0) {
                mMessageWindow->StartMessage(0xc, 0, 1);
            } else {
                mMessageWindow->StartMessage(4, 1, 1);
            }
            mMain.mCellAnim->SetAnimation(1);
            if (mLocalIsFirstSlot != 0) {
                ChangeDir("/Game/2D/union_b/");
                LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
                LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
                LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
                SpriteScreen &sub = SpriteManager::Get().Screen(1);
                sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x8000, 0);
                sub.LoadPalette("union_b_obj_window_ncl.bin", 0x100, 0);
                PrintWindowText(0xc);
                SetWindowOpen(0, 1, 0);
            }
            mBlendTimer = 0x10;
            break;
        }
    }
    ChangeDir("/");
}

BOOL FieldSelectScene::SetMessageTypeAndCheckAll(u32 unk1) {
    WirelessManager::Get();
    if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
        return TRUE;
    }
    u32 count = WirelessManager::Get().GetRecvBlockCount();
    u8 matches = 0;
    u8 i       = 0;
    do {
        u8 *p = WirelessManager::Get().GetRecvBlock(data_020d433c.mRecvBlockIndices[i]);
        if (p != NULL && *p == unk1) {
            matches++;
        }
        i++;
    } while (i < count);
    data_0220f540.mUnk_00 = unk1;
    return matches == count;
}

BOOL FieldSelectScene::func_ov001_02221c0c() {
    MI_CpuCopy8(gUnlockedCharacterLevel0Bits, (u8 *) &data_0220f540 + 4, 9);
    MI_CpuCopy8(gUnlockedFieldBits, (u8 *) &data_0220f540 + 0xd, 1);
    data_0220f540.mUnk_0e = gSaveData[0].mRules;
    data_0220f540.mUnk_0f = gSaveData[0].mCreditsDone;
    if (SetMessageTypeAndCheckAll(0x17)) {
        u32 count = WirelessManager::Get().GetRecvBlockCount();
        u8 i      = 0;
        do {
            u8 *p = WirelessManager::Get().GetRecvBlock(data_020d433c.mRecvBlockIndices[i]);
            if (p != NULL) {
                u8 j;
                u8 k;
                u8 *dst;
                // Byte offset of player i's row, so that row[j + 0x266] is mCharaUnlocked[i][j]: indexing mCharaUnlocked
                // directly folds the member offset into the row pointer, which the original doesn't.
                u8 *row = (u8 *) this + i * 0x12;
                for (j = 0; j < 0x12; j++) {
                    switch (j) {
                    case 0:
                    case 0x11:
                        row[j + 0x266] = 3;
                        break;
                    default: {
                        u8 f       = sIdItems.ids[j];
                        dst        = row + j;
                        dst[0x266] = 0;
                        u32 byte   = (u32) f >> 3;
                        int bit    = f % 8;
                        for (k = 0; k < 3; k++) {
                            const u8 *flags = p;
                            flags += k * 3;
                            if (flags[byte + 4] & (1 << bit)) {
                                dst[0x266] = k + 1;
                            }
                        }
                        break;
                    }
                    }
                }
                for (u8 j = 0; j < 9; j++) {
                    switch (j) {
                    case 8:
                        mFieldUnlocked[i][j] = 1;
                        break;
                    default: {
                        u8 f = sFieldSelectFieldIds[j];
                        if (p[0xd + ((u32) f >> 3)] & (1 << (f % 8))) {
                            mFieldUnlocked[i][j] = 1;
                        } else {
                            mFieldUnlocked[i][j] = 0;
                        }
                        break;
                    }
                    }
                }
                if (i == 0) {
                    u8 b                  = p[0xe];
                    data_020d433c.mRules = b;
                    mUnk_25c[0]           = ((u32) b << 25) >> 30;
                    mUnk_25c[2]           = data_020d433c.mUnk_9a_3 == 0;
                    mUnk_25c[4]           = data_020d433c.mUnk_9a_2 == 0;
                    mUnk_25c[6]           = data_020d433c.mUnk_9a_4 == 0;
                    mUnk_25c[8]           = data_020d433c.mMicEnabled == 0;
                    data_020d433c.mFieldVariant = mUnk_25c[0];
                }
                if (p[0xf] != 0) {
                    SetFlagBit(data_020d43e3, i);
                }
            }
            i++;
        } while (i < count);
        return TRUE;
    }
    return FALSE;
}

BOOL FieldSelectScene::ExchangeCharacterLooks() {
    MIi_CpuCopyFast(gCharacterLook, (u8 *) &data_0220f540 + 4, 8);
    if (SetMessageTypeAndCheckAll(6)) {
        u32 count = WirelessManager::Get().GetRecvBlockCount();
        u8 i      = 0;
        do {
            u8 *p = WirelessManager::Get().GetRecvBlock(data_020d433c.mRecvBlockIndices[i]);
            if (p != NULL) {
                MIi_CpuCopyFast(p + 4, mCharacterLooks + i * 8, 8);
            }
            i++;
        } while (i < count);
        return TRUE;
    }
    return FALSE;
}

void FieldSelectScene::func_ov001_02222040() {
    if (data_020d433c.mWireless != 0) {
        WirelessManager::Get();
        if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
            if (!((mState != 0) & (mState < 6))) {
                return;
            }
            mState                = 10;
            data_020d433c.mSlotsPerPlayer = 3;
            data_020d433c.mPlayerCount = 2;
            u8 i                  = 0;
            data_020d433c.mHasPartners = 0;
            data_020d433c.mFieldId = 0;
            data_020d433c.mFieldVariant = 0;
            do {
                data_020d433c.mCharacterIds[i] = i;
                data_020d433c.mPartners[i] = i;
                i++;
            } while (i < mPlayerCount);
            mNextScene = 3;
            return;
        }
    }
    u8 i;
    WirelessManager::Get();
    const u8 *mode = &data_020c4258;
    if (IsWirelessMode(mode)) {
        WirelessManager::Get();
        data_0220f540.mUnk_01 = data_020c4230.mUnk_20;
        WirelessManager::Get().QueueSend(&data_0220f540);
        WirelessManager::Get().SetSendBlock(&data_0220f540);
    }
    WirelessManager::Get();
    if (IsWirelessMode(mode)) {
        i = 0;
        do {
            u8 *p = WirelessManager::Get().GetRecvBlock(data_020d433c.mRecvBlockIndices[i]);
            if (p != NULL) {
                if (p[0] == 0xd) {
                    mInput[i].mReleased = p[6];
                    mInput[i].mTouchX   = p[7];
                    mInput[i].mTouchY   = p[8];
                    mInput[i].mKeys     = *(u16 *) (p + 4);
                    mInput[i].mUnk_07   = p[9];
                    mInput[i].mWindowSelection   = p[0xb];
                    mInput[i].mUnk_0e   = p[0xc];
                    mInput[i].mRulePageRequested   = p[0xd];
                } else {
                    mInput[i].mReleased = 0;
                    mInput[i].mTouchX   = 0;
                    mInput[i].mTouchY   = 0;
                    mInput[i].mKeys     = 0;
                    mInput[i].mUnk_07   = 0;
                    mInput[i].mWindowSelection   = 0;
                    mInput[i].mUnk_0e   = 0;
                    mInput[i].mRulePageRequested   = 0;
                }
            }
            i++;
        } while (i < 4);
        data_0220f540.mUnk_00 = 0xd;
        NET_BYTE(6) = TouchManager::Get().mReleased;
        NET_BYTE(7) = TouchManager::Get().mX;
        NET_BYTE(8) = TouchManager::Get().mY;
        NET_KEYS   = InputManager::Get().IsTrigger(KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT) | InputManager::Get().IsPressed(3);
        NET_BYTE(9) = 0;
        NET_BYTE(12) = 0;
        NET_BYTE(13) = 0;
    } else {
        mInput[0].mReleased = TouchManager::Get().mReleased;
        mInput[0].mTouchX   = TouchManager::Get().mX;
        mInput[0].mTouchY   = TouchManager::Get().mY;
        mInput[0].mKeys     = InputManager::Get().IsTrigger(KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT) | InputManager::Get().IsPressed(3);
        mInput[0].mUnk_07   = 0;
        mInput[0].mUnk_0e   = 0;
        mInput[0].mRulePageRequested   = 0;
    }
    if ((mState == 5) & (mBlendStep == 0)) {
        if (mLocalTaken != 0 && mInput[mLocalSlot].mWindowOpen == 0) {
            u8 r = mWindows[2]->Update(0);
            if (r != 0) {
                switch (r) {
                    case 1:
                    case 2:
                        WirelessManager::Get();
                        if (IsWirelessMode(mode)) {
                            NET_KEYS |= 2;
                        } else {
                            mInput[0].mKeys |= 2;
                        }
                        break;
                }
                ResetWindow(mWindows[2]);
            }
        }
        if (mLocalIsFirstSlot != 0) {
            if (mInput[0].mWindowOpen != 0) {
                if (mInput[0].mPressTimer != 0) {
                    goto tail;
                }
                BOOL r = mWindows[1]->Update(1);
                switch (r) {
                    case 1:
                        WirelessManager::Get();
                        if (IsWirelessMode(mode)) {
                            NET_KEYS |= 1;
                        } else {
                            mInput[0].mKeys |= 1;
                        }
                        break;
                    case 2:
                        WirelessManager::Get();
                        if (IsWirelessMode(mode)) {
                            NET_KEYS |= 2;
                        } else {
                            mInput[0].mKeys |= 2;
                        }
                        break;
                    default:
                        WirelessManager::Get();
                        if (IsWirelessMode(mode)) {
                            if (NET_KEYS & 1) {
                                NET_KEYS -= 1;
                            }
                            if (NET_KEYS & 2) {
                                NET_KEYS -= 2;
                            }
                        } else {
                            if (mInput[0].mKeys & 1) {
                                mInput[0].mKeys -= 1;
                            }
                            if (mInput[0].mKeys & 2) {
                                mInput[0].mKeys -= 2;
                            }
                        }
                        break;
                }
                u8 sel = mWindows[1]->mCursor->mSelection;
                if (sel < 3) {
                    WirelessManager::Get();
                    if (IsWirelessMode(mode)) {
                        NET_BYTE(11) = sel;
                    } else {
                        mInput[0].mWindowSelection = sel;
                    }
                }
            } else {
                u8 r = mWindows[0]->Update(0);
                if (r != 0) {
                    switch (r) {
                        case 1:
                            WirelessManager::Get();
                            if (!IsWirelessMode(mode)) {
                                mInput[0].mUnk_0e = 1;
                            } else {
                                NET_BYTE(12) = 1;
                            }
                            break;
                        case 2:
                            WirelessManager::Get();
                            if (IsWirelessMode(mode)) {
                                if (mLocalTaken != 0) {
                                    if (!(NET_KEYS & 2)) {
                                        NET_BYTE(9) = 1;
                                    }
                                } else {
                                    NET_BYTE(9) = 1;
                                }
                            } else {
                                if (mLocalTaken != 0) {
                                    if (!(mInput[0].mKeys & 2)) {
                                        mInput[0].mUnk_07 = 1;
                                    }
                                } else {
                                    mInput[0].mUnk_07 = 1;
                                }
                            }
                        }
                        ResetWindow(mWindows[0]);
                    }
                    if (mWindows[3] != NULL) {
                        u8 r2 = mWindows[3]->Update(0);
                        if (r2 != 0) {
                            if (r2 == 1) {
                                WirelessManager::Get();
                                if (IsWirelessMode(mode)) {
                                    NET_BYTE(13) = 1;
                                } else {
                                    mInput[0].mRulePageRequested = 1;
                                }
                            }
                            ResetWindow(mWindows[3]);
                        }
                    }
                }
            } else {
                if (mPageKinds[mPage] == FIELD_SELECT_PAGE_CHARA) {
                    WirelessManager::Get();
                    if (IsWirelessMode(mode)) {
                                                if (mInput[mLocalSlot].mWindowOpen != 0) {
                            if (mInput[mLocalSlot].mPressTimer == 0) {
                                BOOL r = mWindows[1]->Update(1);
                                switch (r) {
                                    case 1:
                                        NET_KEYS |= 1;
                                        break;
                                    case 2:
                                        NET_KEYS |= 2;
                                        break;
                                    default:
                                        if (NET_KEYS & 1) {
                                            NET_KEYS -= 1;
                                        }
                                        if (NET_KEYS & 2) {
                                            NET_KEYS -= 2;
                                        }
                                        break;
                                }
                                u8 sel = mWindows[1]->mCursor->mSelection;
                                if (sel < 3) {
                                    NET_BYTE(11) = sel;
                                }
                            }
                        } else {
                            u8 r = mWindows[0]->Update(0);
                            if (r != 0) {
                                switch (r) {
                                    case 1:
                                    case 2:
                                        NET_KEYS |= 1;
                                        break;
                                }
                                ResetWindow(mWindows[0]);
                            }
                        }
                    }
                }
            }
        }
tail:
    i = 0;
    do {
        if (mInput[i].mPressTimer != 0) {
            mInput[i].mPressTimer--;
        }
        if (mInput[i].mUnk_07 != 0) {
            if (mInput[i].mPressTimer != 0) {
                mInput[i].mUnk_07 = 0;
            } else {
                mInput[i].mPressTimer = 8;
            }
        }
        if (mInput[i].mKeys & 2) {
            if (mInput[i].mPressTimer != 0) {
                mInput[i].mKeys -= 2;
            } else {
                mInput[i].mPressTimer = 8;
            }
        }
        if (mInput[i].mKeys & 1) {
            if (mInput[i].mPressTimer != 0) {
                mInput[i].mKeys -= 1;
            } else {
                mInput[i].mPressTimer = 8;
            }
        }
        i++;
    } while (i < mInputCount);
}

void FieldSelectScene::ResetPage() {
    u8 i = 0;
    do {
        mInput[i].mCursorItem = 0;
        mInput[i].mTaken = 0;
        mInput[i].mUnk_07 = 0;
        i++;
    } while (i < 4);
    SetupPage();
}

u32 FieldSelectScene::UpdatePage() {
    if (UpdatePageBlend()) {
        return 0;
    }
    if (mLocalIsFirstSlot != 0) {
        if (mInput[0].mWindowOpen != 0) {
            switch (mPageKinds[mPage]) {
                case FIELD_SELECT_PAGE_CHARA:
                    mPanel[1]->DrawIcons();
                    mCells2[6].mCellAnim->Draw(1, 0x70, 0x2d, 0);
                    break;
                case FIELD_SELECT_PAGE_FIELD:
                case FIELD_SELECT_PAGE_WINDOW:
                    DrawWindowText();
                    break;
            }
            mWindows[1]->Draw();
        } else {
            mWindows[0]->Draw();
            if (mLocalTaken != 0) {
                mWindows[2]->Draw();
            }
            if (mWindows[3] != NULL) {
                mWindows[3]->Draw();
            }
        }
    } else {
        if (mPageKinds[mPage] == FIELD_SELECT_PAGE_CHARA) {
            if (mInput[mLocalSlot].mWindowOpen != 0) {
                mPanel[1]->DrawIcons();
                mCells2[6].mCellAnim->Draw(1, 0x70, 0x2d, 0);
                mWindows[1]->Draw();
            } else if (mLocalTaken != 0) {
                mWindows[2]->Draw();
            } else {
                mWindows[0]->Draw();
            }
        }
    }
    if (mPendingSubPlanes != -1) {
        G2S_SetBG1Priority(0);
        GXS_SetVisiblePlane(mPendingSubPlanes);
        mPendingSubPlanes = -1;
    }
    u32 result;
    switch (mPageKinds[mPage]) {
        case FIELD_SELECT_PAGE_WINDOW:
            result = UpdateWindowPage();
            break;
        case FIELD_SELECT_PAGE_RULE:
            result = UpdateRulePage();
            break;
        case FIELD_SELECT_PAGE_CHARA:
            result = UpdateCharaPage();
            break;
        case FIELD_SELECT_PAGE_PAIR:
            result = UpdatePairPage();
            break;
        case FIELD_SELECT_PAGE_FIELD:
            result = UpdateFieldPage();
            break;
        case FIELD_SELECT_PAGE_NUMBER:
            result = UpdateNumberPage();
            break;
    }
    if (result != 0) {
    switch (result) {
        case 1:
            mPage++;
            if ((data_020d433c.mPlayerCount < 4) & (mPageKinds[mPage] == FIELD_SELECT_PAGE_PAIR)) {
                mPage++;
            }
            if ((mShowRulePage == 0) & (mPageKinds[mPage] == FIELD_SELECT_PAGE_RULE)) {
                mPage++;
            }
            if (mPageKinds[mPage] >= FIELD_SELECT_PAGE_END) {
                mNextScene = 1;
                return 1;
            }
            break;
        case 2:
            if (mPageKinds[mPage] == FIELD_SELECT_PAGE_CHARA) {
                mPanel[1]->SetCharacter(3, -1, 0, 0);
            }
            for (;;) {
                if (mPage != 0) {
                    mPage--;
                } else {
                    return 2;
                }
                if (mPageKinds[mPage] != FIELD_SELECT_PAGE_PAIR) {
                    if (mPageKinds[mPage] == FIELD_SELECT_PAGE_CHARA) {
                        u8 i = 0;
                        do {
                            mInput[i].mTaken = 1;
                            mInput[i].mCursorItem = FindCharaItem(data_020d433c.mCharacterIds[i]);
                            i++;
                        } while (i < mPlayerCount);
                    }
                    break;
                }
                if (mPlayerCount == 4) {
                    break;
                }
            }
            break;
        case 3:
            return 3;
    }
    mBlendStep = 1;
    }
    return 0;
}

void FieldSelectScene::SetCursorItem(u32 slot, u32 value, u32 playSound) {
    if (value != mInput[slot].mCursorItem) {
        mInput[slot].mCursorItem = value;
        switch (mPageKinds[mPage]) {
            case FIELD_SELECT_PAGE_CHARA:
                mCells2[slot].mX = GetCharaItemX(mInput[slot].mCursorItem);
                mCells2[slot].mY = GetCharaItemY(mInput[slot].mCursorItem);
                if ((slot == mLocalSlot) | ((slot >= mInputCount) && (mLocalIsFirstSlot != 0))) {
                    playSound = 1;
                } else {
                    playSound = 0;
                }
                break;
            case FIELD_SELECT_PAGE_NUMBER: {
                for (u8 i = 0; i < 3; i++) {
                    if (mCells2[i].mCellAnim != NULL) {
                        mCells2[i].mCellAnim->SetAnimation(i);
                    }
                }
                u8 s = mInput[0].mCursorItem;
                mCells2[s].mCellAnim->SetAnimation(s + 3);
                break;
            }
            case FIELD_SELECT_PAGE_PAIR: {
                for (u8 i = 0; i < 4; i++) {
                    if (i == 0) {
                        mCells2[i].mCellAnim->SetAnimation(0x19);
                    } else {
                        mCells2[i].mCellAnim->SetAnimation(7);
                    }
                }
                u8 s = mInput[0].mCursorItem;
                if (s == 0) {
                    mCells2[s].mCellAnim->SetAnimation(0x1a);
                } else {
                    mCells2[s].mCellAnim->SetAnimation(8);
                }
                mCells2[4].mY = s * 0x24 + 0x1a;
                break;
            }
            case FIELD_SELECT_PAGE_FIELD:
                mCells2[slot].mX = GetFieldItemX(mInput[slot].mCursorItem);
                mCells2[slot].mY = GetFieldItemY(mInput[slot].mCursorItem);
                UploadFieldFiles(mInput[slot].mCursorItem);
                break;
        }
    }
    if (playSound) {
        SoundManager::Get().StartSeqArc(5);
    }
}

BOOL FieldSelectScene::TakeCursorItem(u8 slot) {
    switch (mPageKinds[mPage]) {
        case FIELD_SELECT_PAGE_CHARA:
            if (mInput[slot].mCursorItem == 0x11) {
                do {
                    do {
                        mInput[slot].mCursorItem = MATH_Rand32(&gRandom, 0x12);
                    } while (mInput[slot].mCursorItem == 0x11);
                } while (IsCursorItemBlocked(slot) != 0);
                mCells2[slot].mX = GetCharaItemX(mInput[slot].mCursorItem);
                mCells2[slot].mY = GetCharaItemY(mInput[slot].mCursorItem);
            } else {
                if (IsCursorItemBlocked(slot) != 0) {
                    if (slot < mInputCount) {
                        if (slot == mLocalSlot) {
                            SoundManager::Get().StartSeqArc(2);
                        }
                    } else if (mLocalSlot == 0) {
                        SoundManager::Get().StartSeqArc(2);
                    }
                    return FALSE;
                }
            }
            mInput[slot].mTakenItem  = mInput[slot].mCursorItem;
            mCells2[slot].mUnk_09 = 2;
            mPanel[0]->SetCharacter(slot, ((const s8 *) sIdItems.ids)[mInput[slot].mCursorItem], (u32) (mCharacterLooks + slot * 8), 0);
            PrintSlotName(slot);
            break;
        case FIELD_SELECT_PAGE_PAIR: {
            u8 s = mInput[0].mCursorItem;
            if (s == 0) {
                mCells2[s].mCellAnim->SetAnimation(0x1b);
            } else {
                mCells2[s].mCellAnim->SetAnimation(9);
            }
            break;
        }
        case FIELD_SELECT_PAGE_NUMBER:
            mCells2[mInput[0].mCursorItem].mCellAnim->SetAnimation(mInput[0].mCursorItem + 6);
            break;
        case FIELD_SELECT_PAGE_FIELD:
            if (mInput[0].mCursorItem == 8) {
                u8 r;
                do {
                    r = MATH_Rand32(&gRandom, 8);
                } while (mFieldUnlocked[0][r] == 0);
                SetCursorItem(0, r, 0);
            }
            if (mFieldUnlocked[0][mInput[0].mCursorItem] == 0) {
                if (mLocalSlot == 0) {
                    SoundManager::Get().StartSeqArc(2);
                }
                return FALSE;
            }
            break;
    }
    mInput[slot].mTaken = 1;
    SoundManager::Get().StartSeqArc(0);
    return TRUE;
}

BOOL FieldSelectScene::IsCursorItemBlocked(u32 slot) {
    u8 idx;
    if (slot < mInputCount) {
        idx = mInput[slot].mCursorItem;
        if (mCharaUnlocked[slot][idx] == 0) {
            return TRUE;
        }
    } else {
        idx = mInput[slot].mCursorItem;
        if (mCharaUnlocked[0][idx] == 0) {
            return TRUE;
        }
    }
    if (idx == 0) {
        WirelessManager::Get();
        if (IsWirelessMode(&data_020c4030.mUnk_228)) {
            return slot >= mInputCount;
        }
        return slot != 0;
    }
    BOOL result = FALSE;
    u8 i        = 0;
    do {
        if (i != slot && mInput[i].mTaken != 0) {
            u8 v = mInput[i].mCursorItem;
            if (v == idx) {
                result = TRUE;
                break;
            }
        }
        i++;
    } while (i < mPlayerCount);
    return result;
}

BOOL FieldSelectScene::func_ov001_02223914() {
    if (mPageKinds[mPage] == FIELD_SELECT_PAGE_CHARA) {
        u8 flag        = 0;
        const u8 *mode = &data_020c4258;
        u8 i           = 0;
        do {
            if (mInput[i].mKeys & 2) {
                u8 slot = i;
                if (i == 0) {
                    u8 j = mInputCount;
                    u8 n = mPlayerCount;
                    if (n != j) {
                        do {
                            if (mInput[j].mTaken != 0) {
                                slot = j;
                            }
                            j++;
                        } while (j < n);
                    }
                    if (mInput[slot].mTaken != 0) {
                        mPanel[0]->SetCharacter(slot, -1, 0, 0);
                        WirelessManager::Get();
                        if (IsWirelessMode(mode)) {
                            if (slot >= mInputCount) {
                                ClearName(slot);
                            }
                        } else {
                            ClearName(slot);
                        }
                        if (slot == mLocalSlot) {
                            mPanel[1]->SetCharacter(3, -1, 0, 0);
                        }
                        mCells2[slot].mUnk_09 = 1;
                        mInput[slot].mTaken  = 0;
                        if (mInput[mLocalSlot].mWindowOpen == 0) {
                            SoundManager::Get().StartSeqArc(1);
                        }
                        flag = 1;
                    }
                } else {
                    if (mInput[i].mTaken != 0) {
                        mPanel[0]->SetCharacter(i, -1, 0, 0);
                        WirelessManager::Get();
                        if (IsWirelessMode(mode)) {
                            if (i >= mInputCount) {
                                ClearName(i);
                            }
                        } else {
                            ClearName(i);
                        }
                        if (i == mLocalSlot) {
                            mPanel[1]->SetCharacter(3, -1, 0, 0);
                        }
                        mCells2[i].mUnk_09 = 1;
                        mInput[i].mTaken  = 0;
                        if (mInput[mLocalSlot].mWindowOpen == 0) {
                            SoundManager::Get().StartSeqArc(1);
                        }
                    }
                }
                if (slot == mLocalSlot) {
                    mPanel[1]->SetCharacter(0, -1, 0, 0);
                    mPanel[1]->SetCharacter(0, 0x15, (u32) (mCharacterLooks + mLocalSlot * 8), 0);
                    mUnk_340 = 0;
                }
            }
            i++;
        } while (i < mInputCount);
        if (flag != 0) {
            return FALSE;
        }
    }
    if (mInput[0].mWindowOpen != 0) {
        return FALSE;
    }
    if (mInput[0].mUnk_07 != 0) {
        SoundManager::Get().StartSeqArc(1);
        return TRUE;
    }
    if (mInput[0].mKeys & 2) {
        SoundManager::Get().StartSeqArc(1);
        return TRUE;
    }
    return FALSE;
}

void FieldSelectScene::RefreshRuleOptions(u32 unk1) {
    u8 v    = mInput[0].mCursorItem;
    u8 mode = mUnk_25c[0];
    if (v == 0 && unk1 != 0) {
        switch (mode) {
            case 0:
                mUnk_25c[2] = 0;
                mUnk_25c[4] = 0;
                mUnk_25c[6] = 0;
                break;
            case 1:
                mUnk_25c[2] = 1;
                mUnk_25c[4] = 0;
                mUnk_25c[6] = 0;
                break;
            case 2:
                mUnk_25c[2] = 1;
                mUnk_25c[4] = 1;
                mUnk_25c[6] = 1;
                break;
        }
    }
    if (mLocalIsFirstSlot != 0) {
        u8 i = 0;
        do {
            mCells2[i].mCellAnim->mFrameIndex = mUnk_25c[i * 2];
            i++;
        } while (i < 5);
        mCells2[7].mY = mInput[0].mCursorItem * 0x18 + 0x26;
    } else {
        u8 i = 0;
        do {
            mCells[i].mCellAnim->mFrameIndex = mUnk_25c[i * 2];
            i++;
        } while (i < 5);
    }
}

void FieldSelectScene::UpdatePairCells() {
    u8 r = mInput[0].mCursorItem % 4;
    if (r == 0) {
        mCells[0].mCellAnim->SetAnimation(0);
        mCells[1].mCellAnim->SetAnimation(0);
        mCells[2].mCellAnim->SetAnimation(0);
        mCells[3].mCellAnim->SetAnimation(0);
    } else {
        mCells[0].mCellAnim->SetAnimation(1);
        mCells[1].mCellAnim->SetAnimation(1);
        mCells[2].mCellAnim->SetAnimation(1);
        mCells[3].mCellAnim->SetAnimation(1);
        u8 a = sFieldSelectPairSlots[r][1];
        u8 b = sFieldSelectPairSlots[r][2];
        u8 c = sFieldSelectPairSlots[r][3];
        mCells[0].mCellAnim->mFrameIndex = 0;
        mCells[a].mCellAnim->mFrameIndex = 0;
        mCells[b].mCellAnim->mFrameIndex = 1;
        mCells[c].mCellAnim->mFrameIndex = 1;
    }
}

u32 FieldSelectScene::UpdateNumberPage() {
    u32 result = 0;
    if (func_ov001_02223914()) {
        result = 2;
    } else {
    if (mInput[0].mKeys & 1) {
        TakeCursorItem(0);
    } else if (mInput[0].mReleased != 0) {
        u8 cur = mInput[0].mCursorItem;
        int dx = (s16) (mInput[0].mTouchX - 0x20);
        if ((dx >= 0) & (dx < 0xc0)) {
            u8 i = 0;
            if (mInputCount > 2) {
                i++;
            }
            do {
                s16 base = 0x30;
                if (mInputCount > 2) {
                    base -= 0x10;
                }
                int dy = (s16) (mInput[0].mTouchY - base - i * 0x24);
                if ((dy >= 0) & (dy < 0x20)) {
                    if (cur == i) {
                        TakeCursorItem(0);
                    } else {
                        SetCursorItem(0, i, 1);
                    }
                    break;
                }
                i++;
            } while (i < 3);
        }
    } else if (mInput[0].mKeys != 0) {
        u8 v   = mInput[0].mCursorItem % 3;
        do {
            if (mInput[0].mKeys & KEY_UP) {
                v = (v + 2) % 3;
            } else if (mInput[0].mKeys & KEY_DOWN) {
                v = (v + 1) % 3;
            }
        } while (mInputCount > 2 && v == 0);
        if (v != mInput[0].mCursorItem) {
            SetCursorItem(0, v, 1);
        }
    }
    if (mInput[0].mTaken != 0) {
        u8 m     = mInput[0].mCursorItem % 3;
        mPlayerCount = m + 2;
        result   = 1;
    }
    }
    return result;
}

u32 FieldSelectScene::UpdateRulePage() {
    u32 result = 0;
    if (func_ov001_02223914()) {
        data_020d433c.mRuleFieldVariant = mUnk_25c[0];
        data_020d433c.mUnk_9a_3 = (u8) ((mUnk_25c[2] == 0) ? 1 : 0);
        data_020d433c.mUnk_9a_2 = (u8) ((mUnk_25c[4] == 0) ? 1 : 0);
        data_020d433c.mUnk_9a_4 = (u8) ((mUnk_25c[6] == 0) ? 1 : 0);
        data_020d433c.mMicEnabled = (u8) ((mUnk_25c[8] == 0) ? 1 : 0);
        data_020d433c.mFieldVariant   = mUnk_25c[0];
        if (mLocalIsFirstSlot != 0) {
            gSaveData[0].mRules   = data_020d433c.mRules;
            gSaveData[0].mMicEnabled = data_020d433c.mMicEnabled;
        }
        result = 2;
    } else {
        u32 prev = 0;
        u32 next = 0;
        u32 changed = 0;
        u32 keys = mInput[0].mKeys;
        if (!(keys & 1)) {
            if (mInput[0].mReleased != 0) {
                int y = mInput[0].mTouchY;
                int t = y - 0x26;
                int x = mInput[0].mTouchX;
                if (((t % 0x18) < 0x14) & ((y < 0x9e) & ((y > 0x26) & ((x >= 0x79) & (x <= 0xfc))))) {
                    int sel = t / 0x18;
                    if (sel > 4) {
                        sel = 4;
                    }
                    u8 ns = sel;
                    if (mInput[0].mCursorItem != ns) {
                        mInput[0].mCursorItem = ns;
                        changed           = 1;
                    }
                    if (x <= 0x91) {
                        prev = 1;
                    }
                    if (x >= 0xe4) {
                        next = 1;
                    }
                }
            } else if (keys != 0) {
                u8 cur = mInput[0].mCursorItem;
                u8 v   = cur % 5;
                if (keys & KEY_UP) {
                    v = (v + 4) % 5;
                } else if (keys & KEY_DOWN) {
                    v = (v + 1) % 5;
                }
                if (v != cur) {
                    mInput[0].mCursorItem = v;
                    changed           = 1;
                } else if (keys & KEY_LEFT) {
                    prev = 1;
                } else if (keys & KEY_RIGHT) {
                    next = 1;
                }
            }
        }
        u8 idx = mInput[0].mCursorItem % 5;
        if (prev) {
            mOptions[idx].mValue = (mOptions[idx].mValue + mOptions[idx].mCount - 1) % mOptions[idx].mCount;
            if (mLocalIsFirstSlot != 0) {
                mCells2[5].mCellAnim->SetAnimation(1);
                mCells2[5].mY = idx * 0x18 + 0x26;
            }
            changed = 1;
        }
        if (next) {
            mOptions[idx].mValue = (mOptions[idx].mValue + 1) % mOptions[idx].mCount;
            if (mLocalIsFirstSlot != 0) {
                mCells2[6].mCellAnim->SetAnimation(4);
                mCells2[6].mY = idx * 0x18 + 0x26;
            }
            changed = 1;
        }
        if (changed) {
            SetCursorItem(0, idx, 1);
            RefreshRuleOptions(prev || next);
        }
    }
    return result;
}

u32 FieldSelectScene::UpdateCharaPage() {
    u8 result = 0;
    u8 numDone = 0;
    if (func_ov001_02223914()) {
        result = 2;
    } else {
        u8 numOpen = 0;
        BOOL flagB   = 0;
        mLocalCursorSlot   = 4;
        u8 i       = 0;
        do {
            u8 owner;
            u8 event = 0;
            if (i >= mInputCount) {
                if (flagB != 0) {
                    break;
                }
                owner = 0;
                if (mInput[0].mWindowOpen != 0) {
                    numOpen++;
                    if (i == mUnk_33c) {
                        u16 keys = mInput[0].mKeys;
                        if (keys & 1) {
                            event = 1;
                            mInput[mUnk_33c].mTakenWindowSelection = mInput[0].mWindowSelection;
                        } else if (keys & 2) {
                            mInput[mUnk_33c].mTaken = 0;
                            event                    = 1;
                        }
                        if (event) {
                            if (mLocalSlot == 0) {
                                mPanel[1]->SetCharacter(3, -1, 0, 0);
                            }
                            SetWindowOpen(0, 0, 0);
                            break;
                        }
                    }
                    continue;
                }
            } else {
                owner = i;
                if (mInput[i].mWindowOpen != 0) {
                    numOpen++;
                    if (i == 0 && mUnk_33c != 0) {
                        continue;
                    }
                    u16 keys = mInput[i].mKeys;
                    u8 fired = 0;
                    if (keys & 1) {
                        fired                = 1;
                        mInput[i].mTakenWindowSelection = mInput[i].mWindowSelection;
                    } else if (keys & 2) {
                        mInput[i].mTaken = 0;
                        fired             = 1;
                    }
                    if (fired == 0) {
                        continue;
                    }
                    if (mUnk_33c == 0) {
                        flagB = 1;
                    }
                    if (i == mLocalSlot) {
                        mPanel[1]->SetCharacter(3, -1, 0, 0);
                    }
                    SetWindowOpen(i, 0, 0);
                    continue;
                }
            }
            if (mInput[i].mTaken != 0) {
                numDone++;
                continue;
            }
            {
                u16 keys = mInput[owner].mKeys;
                if (keys & 1) {
                    event = 1;
                } else if (mInput[owner].mReleased != 0) {
                    u8 cur = mInput[i].mCursorItem;
                    u32 k  = 0;
                    do {
                        int dx = (s16) (mInput[owner].mTouchX - GetCharaItemX(k));
                        int dy = (s16) (mInput[owner].mTouchY - GetCharaItemY(k));
                        BOOL hit = ((dy >= 0) & ((dx >= 0) & (dx <= 0x28)));
                        if (hit & (dy <= 0x20)) {
                            if (cur == k) {
                                event = 1;
                            } else {
                                SetCursorItem(i, k, 1);
                            }
                            break;
                        }
                        k = (u8) (k + 1);
                    } while (k < 0x12);
                } else if (keys & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT)) {
                    u8 cur = mInput[i].mCursorItem;
                    u8 col = cur % 6;
                    u8 row = cur / 6;
                    if (keys & KEY_UP) {
                        row = (row + 2) % 3;
                    } else if (keys & KEY_DOWN) {
                        row = (row + 1) % 3;
                    } else if (keys & KEY_LEFT) {
                        col = (col + 5) % 6;
                    } else if (keys & KEY_RIGHT) {
                        col = (col + 1) % 6;
                    }
                    int n = row * 6 + col;
                    if (n > 0x11) {
                        n = 0x11;
                    }
                    SetCursorItem(i, (u8) n, 1);
                }
            }
            if (owner == 0) {
                flagB = 1;
            }
            if (owner == mLocalSlot) {
                mLocalCursorSlot = i;
            }
            if (event != 0) {
                if (TakeCursorItem(i)) {
                    u8 show = 0;
                    if (mInput[i].mCursorItem != 0) {
                        if (owner == 0) {
                            mUnk_33c = i;
                        }
                        SetWindowOpen(owner, 1, mInput[i].mCursorItem);
                        if (owner == mLocalSlot) {
                            mPanel[1]->SetCharacter(3, ((const s8 *) sIdItems.ids)[mInput[i].mCursorItem], (u32) (mCharacterLooks + i * 8), 0);
                            mPanel[1]->SetDrawArgs(3, 0x72, 0x2f, 0);
                        }
                    }
                    if (mInput[i].mCursorItem == 0) {
                        if (i == mLocalSlot) {
                            show = 1;
                        }
                    } else if (mLocalSlot == 0) {
                        if (mPlayerCount != mInputCount) {
                            if (i == mLocalSlot) {
                                show = 1;
                            }
                        }
                    }
                    if (show) {
                        mPanel[1]->SetCharacter(0, -1, 0, 0);
                        mPanel[1]->SetCharacter(0, 0x15, (u32) (mCharacterLooks + mLocalSlot * 8), 1);
                        mUnk_340 = 1;
                    }
                }
            }
        } while (++i < mPlayerCount);
        if (((numOpen == 0) & (numDone >= mPlayerCount)) || mInput[0].mUnk_0e != 0) {
            if (mInput[0].mUnk_0e != 0) {
                SoundManager::Get().StartSeqArc(0);
            }
            data_020d433c.mPlayerCount = numDone;
            if (numDone < 4) {
                mHasPartners              = 0;
                data_020d433c.mHasPartners = 0;
            }
            u8 j = 0;
            do {
                if ((data_020d433c.mCharacterIds[j] = sIdItems.ids[mInput[j].mCursorItem]) == 0x15) {
                    data_020d433c.mCharacterLevels[j] = 0;
                } else {
                    data_020d433c.mCharacterLevels[j] = mInput[j].mTakenWindowSelection;
                }
                j++;
            } while (j < numDone);
            mSavedFieldItem = 0;
            result   = 1;
        }
    }
    u8 old   = mLocalTaken;
    mLocalTaken = mInput[mLocalSlot].mTaken;
    if (mLocalTaken == 0) {
        mWindows[0]->mButtons[0].mEnabled = 0;
        if (old != 0) {
            mWindows[2]->mState              = 1;
            mWindows[2]->mInputLocked              = 0;
            mWindows[2]->mCursor->mSelection   = -1;
            mWindows[2]->mCellAnims[0]->SetAnimation(2);
        }
    }
    return result;
}

u32 FieldSelectScene::UpdatePairPage() {
    u32 result = 0;
    if (func_ov001_02223914()) {
        result = 2;
    } else {
        u16 keys = mInput[0].mKeys;
        if (keys & 1) {
            TakeCursorItem(0);
        } else if (mInput[0].mReleased != 0) {
            u8 cur = mInput[0].mCursorItem;
            int dx = (s16) (mInput[0].mTouchX - 0x10);
            if ((dx >= 0) & (dx < 0xe0)) {
                int y = mInput[0].mTouchY - 0x1a;
                u8 k  = 0;
                do {
                    int d = (s16) (y - k * 0x24);
                    if ((d >= 0) & (d < 0x24)) {
                        if (cur == k) {
                            TakeCursorItem(0);
                        } else {
                            SetCursorItem(0, k, 1);
                        }
                        break;
                    }
                    k++;
                } while (k < 4);
            }
        } else if (keys != 0) {
            u8 cur = mInput[0].mCursorItem;
            u8 v   = cur % 4;
            if (keys & KEY_UP) {
                v = (v + 3) % 4;
            } else if (keys & KEY_DOWN) {
                v = (v + 1) % 4;
            }
            if (v != cur) {
                SetCursorItem(0, v, 1);
            }
        }
        UpdatePairCells();
        if (mInput[0].mTaken != 0) {
            u8 idx = mInput[0].mCursorItem % 4;
            if (idx == 0) {
                mHasPartners = 0;
            } else {
                mHasPartners                 = 1;
                u8 a                     = sFieldSelectPairSlots[idx][1];
                data_020d433c.mPartners[0] = a;
                data_020d43d7[a]         = 0;
                u8 b = sFieldSelectPairSlots[idx][2];
                u8 c = sFieldSelectPairSlots[idx][3];
                data_020d43d7[b]         = c;
                data_020d43d7[c]         = b;
            }
            data_020d433c.mHasPartners = mHasPartners;
            mSavedFieldItem              = 0;
            result                = 1;
        }
    }
    return result;
}

u32 FieldSelectScene::UpdateFieldPage() {
    u32 result = 0;
    if (func_ov001_02223914()) {
        result = 2;
    } else {
        if (mInput[0].mWindowOpen != 0) {
            u16 keys = mInput[0].mKeys;
            if (keys & 1) {
                SetWindowOpen(0, 0, 0);
                mMessageEnabled = 1;
                if (mInput[0].mWindowSelection == 1) {
                    mInput[0].mTaken = 0;
                }
            } else if (keys & 2) {
                SetWindowOpen(0, 0, 0);
                mMessageEnabled          = 1;
                mInput[0].mTaken = 0;
            }
        } else {
            u16 keys = mInput[0].mKeys;
            u8 ev    = 0;
            if (keys & 1) {
                ev = 1;
            } else if (mInput[0].mReleased != 0) {
                u8 cur = mInput[0].mCursorItem;
                u32 k  = 0;
                do {
                    int dx = (s16) (mInput[0].mTouchX - GetFieldItemX(k));
                    int dy = (s16) (mInput[0].mTouchY - GetFieldItemY(k));
                    BOOL hit = ((dy >= 0) & ((dx >= 0) & (dx <= 0x38)));
                    if (hit & (dy <= 0x20)) {
                        if (cur == k) {
                            ev = 1;
                        } else {
                            SetCursorItem(0, k, 1);
                        }
                        break;
                    }
                    k = (u8) (k + 1);
                } while (k < 9);
            } else if (keys != 0) {
                u8 col = mInput[0].mCursorItem % 4;
                u8 row = mInput[0].mCursorItem / 4;
                if (keys & KEY_UP) {
                    row = (row + 2) % 3;
                } else if (keys & KEY_DOWN) {
                    row = (row + 1) % 3;
                } else if (keys & KEY_LEFT) {
                    col = (col + 3) % 4;
                } else if (keys & KEY_RIGHT) {
                    col = (col + 1) % 4;
                }
                int n = col + row * 4;
                if (n > 8) {
                    n = 8;
                }
                if ((u8) n != mInput[0].mCursorItem) {
                    SetCursorItem(0, (u8) n, 1);
                }
            }
            if (ev) {
                if (TakeCursorItem(0)) {
                    SetWindowOpen(0, 1, 0);
                    if (mLocalIsFirstSlot != 0) {
                        mMessageEnabled = 0;
                    }
                }
            }
        }
        if ((mInput[0].mTaken == 1) & (mInput[0].mWindowOpen == 0)) {
            data_020d433c.mFieldId = sFieldSelectFieldIds[mInput[0].mCursorItem];
            result                = 1;
            mShowRulePage              = 0;
        }
        if (mInput[0].mRulePageRequested != 0) {
            mSavedFieldItem = mInput[0].mCursorItem;
            result   = 1;
            mShowRulePage = 1;
            SoundManager::Get().StartSeqArc(0);
        }
    }
    return result;
}

void FieldSelectScene::UploadFieldFiles(u32 idx) {
    u8 sel;
    if (mFieldUnlocked[mLocalSlot][idx] == 0 && mFieldUnlocked[0][idx] == 0) {
        sel = 9;
    } else {
        sel = sFieldSelectSubFileIndices[idx];
    }
    QueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, mFiles[sel][1], mFileSizes[sel][1], 0);
    QueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, mFiles[sel][0], mFileSizes[sel][0], 0);
    QueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, mFiles[sel][2], mFileSizes[sel][2], 0);
    mCells[4].mCellAnim->mFrameIndex = sel;
}

u32 FieldSelectScene::UpdateWindowPage() {
    u32 result = 0;
    if (mInput[0].mKeys & 1) {
        if (mInput[0].mWindowSelection == 1) {
            result = 1;
        } else {
            result = 3;
        }
    }
    return result;
}

BOOL FieldSelectScene::UpdatePageBlend() {
    if (mBlendStep != 0) {
    if (mBlendTarget == mBlend) {
        switch (mBlendStep) {
            case 1:
                mBlendTarget = 0x10;
                if (mWindows[0] != NULL) {
                    mWindows[0]->mEnabled = 0;
                }
                mBlendStep++;
                break;
            case 2:
                ResetPage();
                reg_G2_BLDCNT = 0;
                reg_G2S_DB_BLDCNT = 0;
                mBlendStep++;
                break;
            case 3:
                if (mBlendTimer != 0) {
                    mBlendTimer--;
                    return TRUE;
                }
                mBlendTarget = 0;
                mBlendStep++;
                break;
            case 4:
                mWindows[0]->mEnabled = 1;
                mBlendStep = 0;
                break;
        }
    }
    MoveBlendToTarget();
    return TRUE;
    }
    return FALSE;
}

void FieldSelectScene::MoveBlendToTarget() {
    u8 target = mBlendTarget;
    u8 cur    = mBlend;
    if (cur == target) {
        return;
    }
    if (mPageKinds[mPage] == FIELD_SELECT_PAGE_WINDOW) {
        mBlend = target;
    } else if (cur < target) {
        mBlend = cur + 1;
    } else {
        mBlend = cur - 1;
    }
    GX_SetVisiblePlane(0x1b);
    GXS_SetVisiblePlane(0x1b);
    G2_SetBlendAlpha(2, 0x39, mBlend, 0x10 - mBlend);
    G2S_SetBlendAlpha(2, 0x3d, mBlend, 0x10 - mBlend);
}

void FieldSelectScene::SetWindowOpen(u32 slot, u32 value, u32 idx) {
    if (value == mInput[slot].mWindowOpen) {
        return;
    }
    mInput[slot].mWindowOpen = value;
    if (slot != mLocalSlot) {
        return;
    }
    if (mInput[slot].mWindowOpen != 0) {
        if (mWindows[1] != NULL) {
            delete mWindows[1];
        }
        u32 set;
        u32 count;
        u8 sel;
        switch (mPageKinds[mPage]) {
            case FIELD_SELECT_PAGE_FIELD:
                PrintWindowText(0x12);
                set   = 3;
                count = 3;
                G2S_SetBG1Priority(1);
                GXS_SetVisiblePlane(0x1f);
                sel = 0;
                break;
            case FIELD_SELECT_PAGE_WINDOW:
                set   = 4;
                count = 3;
                sel   = 1;
                break;
            default:
                if (mLocalIsFirstSlot != 0 && mUnk_33c != 0) {
                    mMessageWindow->StartMessage(0xe, 0, 1);
                } else {
                    mMessageWindow->StartMessage(0xf, 0, 1);
                }
                set     = 2;
                count = 6;
                sel   = 1;
                u32 row = slot;
                if (slot >= mInputCount) {
                    row = 0;
                }
                switch (mCharaUnlocked[row][idx]) {
                    case 1:
                        set = 0;
                        sel = 0;
                        break;
                    case 2:
                        set = 1;
                        break;
                }
                break;
        }
        mWindows[1] = new SelectWindow(count, sFieldSelectWindow1Items[set], -1);
        mWindows[1]->SetCells(count, sFieldSelectWindow1Cells[set]);
        mWindows[1]->mCursor->mSelection = -1;
        mWindows[1]->mCursor->mSelection = sel;
        mInput[slot].mWindowSelection             = sel;
        if (slot == mLocalSlot) {
            NET_BYTE(0xb) = sel;
        }
    } else {
        if (mWindows[1] != NULL) {
            delete mWindows[1];
        }
        mWindows[1] = NULL;
        if (mPageKinds[mPage] == FIELD_SELECT_PAGE_CHARA) {
            mMessageWindow->StartMessage(6, 0, 1);
        }
        if (mPageKinds[mPage] == FIELD_SELECT_PAGE_FIELD) {
            ClearTextBuffer();
            mPendingSubPlanes = 0x1b;
        }
    }
}

s16 FieldSelectScene::GetCharaItemX(u32 idx) {
    int i = idx;
    return (i % 6) * 0x28 + 8;
}

s16 FieldSelectScene::GetCharaItemY(u32 idx) {
    int i = idx;
    return ((i / 6) % 6) * 0x28 + 0x2c;
}

s16 FieldSelectScene::GetFieldItemX(u32 idx) {
    if (idx >= 8) {
        return 0x64;
    }
    return ((int) idx % 4) * 0x3c + 10;
}

s16 FieldSelectScene::GetFieldItemY(u32 idx) {
    return (idx >> 2) * 0x26 + 0x2c;
}

void FieldSelectScene::Draw() {
    if (mMessageEnabled != 0) {
        mMessageWindow->Update();
    }
    if (mMain.mVisible != 0) {
        mMain.mCellAnim->Update();
        mMain.mCellAnim->Draw(0, mMain.mX, mMain.mY, mMain.mUnk_09);
    }
    if (mCells2[8].mCellAnim != NULL) {
        mCells2[8].mCellAnim->Update();
    }
    if (mPlayerRowsVisible != 0) {
        DrawNames();
        u8 i = 0;
        do {
            s16 y = i * 0x24 + 0x27;
            mPanel[0]->SetDrawArgs(i, 0x1b, (s16) (y + 4), 1);
            mCells[6].mCellAnim->Draw(0, 0x19, (s16) (y + 2), 3);
            if (mInputCount <= i) {
                mCells[7].mCellAnim->SetAnimation(0xb);
                mCells[7].mCellAnim->DrawFrame(0, i, 0x15, y, 1);
            }
            if (mLocalSlot == i) {
                mCells[7].mCellAnim->SetAnimation(i + 2);
                mCells[7].mCellAnim->Draw(0, 0x19, (s16) (y + 0x16), 1);
            }
            mCells[7].mCellAnim->SetAnimation(i + 7);
            mCells[7].mCellAnim->Draw(0, 0x15, y, 1);
            int sel = -1;
            u8 kind = mPageKinds[mPage];
            if (kind == FIELD_SELECT_PAGE_CHARA) {
                if (mInputCount <= i) {
                    if (mUnk_33c > i) {
                        if (mInput[i].mTaken != 0) {
                            sel = (s8) mInput[i].mTakenWindowSelection;
                        }
                    } else if (mUnk_33c == i && mInput[0].mWindowOpen == 0) {
                        if (mInput[i].mTaken != 0) {
                            sel = (s8) mInput[i].mTakenWindowSelection;
                        }
                    }
                } else {
                    if (mInput[i].mWindowOpen == 0) {
                        if (mInput[i].mTaken != 0) {
                            sel = (s8) mInput[i].mTakenWindowSelection;
                        }
                    }
                    if (i == 0) {
                        if (mUnk_33c != 0 && mInput[i].mTaken != 0) {
                            sel = (s8) mInput[i].mTakenWindowSelection;
                        }
                    }
                }
                if (sIdItems.ids[mInput[i].mCursorItem] == 0x15) {
                    sel = -1;
                }
            } else if (kind != FIELD_SELECT_PAGE_NUMBER) {
                if (i < data_020d433c.mPlayerCount) {
                    if (data_020d433c.mCharacterIds[i] != 0x15) {
                        sel = (s8) data_020d433c.mCharacterLevels[i];
                    }
                }
            }
            if (sel >= 0) {
                mCells[4].mCellAnim->DrawFrame(0, sel, 0x40, (s16) (i * 0x24 + 0x38), 1);
            }
            i++;
        } while (i < mPlayerCount);
        mPanel[0]->DrawIcons();
    }
    if (mCharaMarksVisible != 0) {
        u8 slot = mLocalCursorSlot;
        if (slot < 4) {
            if (mInputCount <= slot) {
                mCells2[7].mCellAnim->SetAnimation(0xb);
                mCells2[7].mCellAnim->DrawFrame(1, slot, mCells2[slot].mX, mCells2[slot].mY, mCells2[slot].mUnk_09);
            }
            if (mLocalSlot == slot) {
                mCells2[7].mCellAnim->SetAnimation(slot + 2);
                mCells2[7].mCellAnim->Draw(1, (s16) (mCells2[slot].mX + 4), (s16) (mCells2[slot].mY + 0x16),
                                       mCells2[slot].mUnk_09);
            }
            mCells2[7].mCellAnim->SetAnimation(slot + 7);
            mCells2[7].mCellAnim->Draw(1, mCells2[slot].mX, mCells2[slot].mY, mCells2[slot].mUnk_09);
        }
    }
    if (mUnk_335 != 0) {
        mPanel[1]->DrawIcons();
    }
    if (mPairGridVisible != 0) {
        u8 r = 0;
        do {
            u8 col = 0;
            do {
                s16 v = sFieldSelectPairSlots[r][col];
                s16 x = sFieldSelectPairColumnX[col];
                if (r == 0) {
                    x = sFieldSelectFirstRowColumnX[col];
                }
                s16 y = r * 0x24 + 0x18;
                mPanel[1]->SetDrawArgs(v, x + 6, y + 4, 1);
                mCells2[5].mCellAnim->Draw(1, x + 4, y + 2, 2);
                if (mInputCount <= v) {
                    mCells2[7].mCellAnim->SetAnimation(0xb);
                    mCells2[7].mCellAnim->DrawFrame(1, v, x, y, 1);
                }
                if (mLocalSlot == v) {
                    mCells2[7].mCellAnim->SetAnimation(v + 2);
                    mCells2[7].mCellAnim->Draw(1, x + 4, y + 0x16, 1);
                }
                mCells2[7].mCellAnim->SetAnimation(v + 7);
                mCells2[7].mCellAnim->Draw(1, x, y, 1);
                col++;
            } while (col < 4);
            mPanel[1]->DrawIcons();
            r++;
        } while (r < 4);
    }
    if (mNumberMarksVisible != 0) {
        u8 j = 0;
        if (mInputCount > 2) {
            j++;
        }
        do {
            u8 n    = mInputCount;
            if (n < j + 2) {
                do {
                    mCells2[7].mCellAnim->DrawFrame(1, n, (s16) (mCells2[j].mX + 0x14 + n * 32), mCells2[j].mY,
                                                    mCells2[j].mUnk_09);
                    n++;
                } while (n < j + 2);
            }
            j++;
        } while (j < 3);
    }
    if (mFieldMarksVisible != 0) {
        u8 k = 0;
        do {
            if (mFieldNew[k] != 0) {
                s16 x = GetFieldItemX(k);
                s16 y = GetFieldItemY(k);
                mCells2[8].mCellAnim->Draw(1, (s16) (x + 0xb), (s16) (y + 0x1b), mCells2[8].mUnk_09);
            }
            k++;
        } while (k < 9);
    }
    u8 n = 0;
    do {
        if (mCells[n].mVisible != 0) {
            if (mCells[n].mUpdateAnim != 0) {
                mCells[n].mCellAnim->Update();
            }
            mCells[n].mCellAnim->Draw(0, mCells[n].mX, mCells[n].mY, mCells[n].mUnk_09);
        }
        n++;
    } while (n < 9);
    if (mCharaMarksVisible != 0) {
        u8 s = mLocalCursorSlot;
        if (s < 4) {
            if (mCells2[s].mVisible != 0) {
                if (mCells2[s].mUpdateAnim != 0) {
                    mCells2[s].mCellAnim->Update();
                }
                mCells2[s].mCellAnim->Draw(1, mCells2[s].mX, mCells2[s].mY, mCells2[s].mUnk_09);
            }
        }
    } else {
        u8 m = 0;
        do {
            if (mCells2[m].mVisible != 0) {
                if (mCells2[m].mUpdateAnim != 0) {
                    mCells2[m].mCellAnim->Update();
                }
                mCells2[m].mCellAnim->Draw(1, mCells2[m].mX, mCells2[m].mY, mCells2[m].mUnk_09);
            }
            m++;
        } while (m < 9);
    }
    if (mFieldMarksVisible != 0) {
        u8 k = 0;
        do {
            if (mFieldUnlocked[mLocalSlot][k] == 0) {
                s16 x = GetFieldItemX(k);
                s16 y = GetFieldItemY(k);
                mCells2[4].mCellAnim->Draw(1, x, (s16) (y + 1), mCells2[4].mUnk_09);
            }
            k++;
        } while (k < 9);
    }
    if (mCharaMarksVisible == 0) {
        return;
    }
    u8 f = 0;
    do {
        if (mCharaUnlocked[mLocalSlot][f] == 0) {
            s16 x = GetCharaItemX(f);
            s16 y = GetCharaItemY(f);
            mCells2[4].mCellAnim->Draw(1, x, (s16) (y - 4), mCells2[4].mUnk_09);
        } else if (mCharaNew[f] != 0) {
            u32 show = 1;
            if (mLocalCursorSlot < 4) {
                if (f == mInput[mLocalCursorSlot].mCursorItem) {
                    show = 0;
                }
            }
            if (show != 0) {
                s16 x = GetCharaItemX(f);
                s16 y = GetCharaItemY(f);
                mCells2[8].mCellAnim->Draw(1, (s16) (x + 3), (s16) (y + 0x1b), mCells2[8].mUnk_09);
            }
        }
        u8 p = 0;
        do {
            if (mInput[p].mTaken != 0) {
                u8 q = mInput[p].mTakenItem;
                if ((q != 0) & (f == q)) {
                    mCells2[5].mCellAnim->DrawFrame(1, f, mCells2[4].mX, mCells2[4].mY, mCells2[4].mUnk_09);
                    break;
                }
            }
            p++;
        } while (p < mPlayerCount);
        f++;
    } while (f < 0x12);
    if (mUnk_340 != 0) {
        mCells2[5].mCellAnim->DrawFrame(1, 0, mCells2[4].mX, mCells2[4].mY, mCells2[4].mUnk_09);
    }
}

void FieldSelectScene::LoadMessages() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    ChangeDir("/");
    mMessages = (MessageFile *) LoadFile("font/mes_BattleArenaMenu.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 0x10);
    mTextBuffer = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    mGlyphCount = TextManager::Get().GlyphCount(6);
    mGlyphBuffer = OS_Alloc(mGlyphCount * 12);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 0x10, 0x44);
    TextManager::Get().SetColor(1, 0, 0xf);
}

void FieldSelectScene::PrintWindowText(u32 index) {
    ClearTextBuffer();
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width             = 0;
    height            = 0;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffer, 0x18, 0x10, &height, &width, 0, 0, 2, 0x44, 0xe, 0, 0,
               1, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0x15000, mTextBuffer, mTextBufferSize, 0);
}

void FieldSelectScene::DrawWindowText() {
    if (mPageKinds[mPage] == FIELD_SELECT_PAGE_FIELD) {
        SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, (u8) mGlyphCount, 0x20, 0x28, 0, 0x2a0, 0xf, 0);
    } else {
        SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, (u8) mGlyphCount, 0x20, 0x38, 0, 0x2a0, 0xf, 0);
    }
}

void FieldSelectScene::ClearTextBuffer() {
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}

void FieldSelectScene::LoadNameText() {
    SpriteManager::Get().Screen(0).SetObjVramMode(3);
    ChangeDir("/");
    mStoryCharaNameMessages = (MessageFile *) LoadFile("/font/mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
    mNameBufferSize = TextManager::Get().BufferSize(0x20, 2);
    for (u8 i = 0; i < 4; i++) {
        mNameBuffers[i] = OS_Alloc(mNameBufferSize);
        ClearName(i);
    }
    mNameGlyphCount = TextManager::Get().GlyphCount(8);
    mNameGlyphBuffer = OS_Alloc(mNameGlyphCount * 12);
    TextManager::Get().Init(mNameGlyphBuffer, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
}

void FieldSelectScene::PrintSlotName(u8 slot) {
    ClearName(slot);
    u32 useName = 0;
    u8 id       = sIdItems.ids[mInput[slot].mTakenItem];
    if (id == 0x15) {
        useName = 1;
    } else {
        WirelessManager::Get();
        if (IsWirelessMode(&data_020c4030.mUnk_228)) {
            if (slot < mInputCount) {
                useName = 1;
            }
        }
    }
    TextManager::Get().mUse4bppFont = 1;
    if (useName) {
        s16 height;
        s16 width;
        height = 0;
        width  = 0;
        TextManager::Get().Print(&data_020d433c.mNames[data_020d43eb[slot]], 0x18, mNameBuffers[slot], 0x20, 2, &height, &width, 0, 0, 2,
                                 0x21, 0x10, 0, 0, 0, 1, 1);
    } else {
        TextManager &text = TextManager::Get();
        s16 height;
        s16 width;
        width             = 0;
        height            = 0;
        MessageFile *file = mStoryCharaNameMessages;
        text.Print(file->GetText(id), file->GetLength(id), mNameBuffers[slot], 0x20, 2, &height, &width, 0, 0, 2, 0x21, 0xe, 0, 0, 0, 1,
                   1);
    }
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, UPLOAD_KIND_OBJ, (slot << 12) + 0xc000, mNameBuffers[slot], mNameBufferSize, 0);
}

void FieldSelectScene::DrawNames() {
    u8 i = 0;
    do {
        SpriteManager::Get().Screen(0).AddOam(mNameGlyphBuffer, (u8) mNameGlyphCount, 0x40, (s16) (i * 0x24 + 0x2c), 2,
                                                     ((((u32) i << 12) + 0xc000) << 9) >> 16, 0xf, 0);
        i++;
    } while (i < mPlayerCount);
}

void FieldSelectScene::ClearName(u32 index) {
    if (mNameBuffers[index] != NULL) {
        MI_CpuClear16(mNameBuffers[index], mNameBufferSize);
        MI_CpuClearFast((void *) (0x6400000 + 0xc000 + (index << 12)), 0x1000);
    }
}

void FieldSelectScene::ResetWindow(SelectWindow *window) {
    u8 state;
    do {
        window->Draw();
        window->Update(0);
        state = window->mState;
    } while (!((state <= 2) | (state == 4)));
    window->Reset(-1);
    window->mCursor->mSelection = -1;
    window->mEnabled            = 1;
}

void FieldSelectScene::MarkCharasAndFieldNotNew() {
    u8 i = 0;
    do {
        MarkCharaNotNew(data_020d433c.mCharacterIds[i]);
        i++;
    } while (i < mPlayerCount);
    SetFlagBit(gFieldNotNewBits, data_020d433c.mFieldId);
}

void FieldSelectScene::MarkCharaNotNew(u32 index) {
    if (index < 0x15) {
        SetFlagBit(gCharacterNotNewBits, index);
    }
}

u32 FieldSelectScene::FindCharaItem(u32 id) {
    u8 i = 0;
    do {
        if (sIdItems.ids[i] == id) {
            return i;
        }
        i++;
    } while (i < 0x12);
    return i;
}

void FieldSelectScene::func_ov001_02226d74() {
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228)) {
        u8 *flags;
        u8 i = mInputCount;
        if (mPlayerCount > i) {
            flags = data_020d43e3;
            do {
                if (data_020d433c.mUnk_a7 & 1) {
                    SetFlagBit(flags, i);
                }
                i++;
            } while (i < mPlayerCount);
        }
    }
}
