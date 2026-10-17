#include "Scene/CharacterEditScene.hpp"
#include "Graphics/CharacterModel.hpp"
#include "System/FrameTask.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include <nitro/gx.h>
#include <nitro/os.h>
#include <nitro/mi.h>
#include "System/FileSystem.hpp"
#include "Game/SaveData.hpp"
#include "Game/Random.hpp"
#include <nitro/math.h>

static const WindowItemRecord sEditMenuItems[22] = {
    { 2, 0x1, 135, 26, 114, 24, {0x403}, {-1, -1, -1, -1}, 0, 1 },
    { 9, 0x8, 8, 170, 28, 20, {0x201}, {-1, -1, -1, -1}, 0, 3 },
    { 10, 0x8, 100, 170, 28, 20, {0x201}, {-1, -1, -1, -1}, 0, 3 },
    { 11, 0x8, 40, 170, 56, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
    { 12, 0x2, 225, 170, 30, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
    { 13, 0x8000, 128, 96, 0, 0, {0}, {-1, -1, -1, -1}, 0, 0 },
};

static const WindowItemRecord data_0209f12c[22] = {
    { 0, 0x1, 68, 56, 112, 32, {0x403}, {1, 1, 0, 0}, 0, 2 },
    { 1, 0x1, 68, 104, 112, 32, {0x304}, {0, 0, 1, 1}, 0, 2 },
};

// Camera position and target (in whole units) of the four views.
static const s16 sCameraPositionTargets[4][6] = {
    {180, 110, 90, 5, 80, 0},
    {60, 130, 30, 5, 130, 0},
    {70, 110, 35, 5, 90, 0},
    {100, 100, 50, 5, 40, 0},
};

static const u16 sCharacterEditLights[20] = {1, 0x7fff, 0xf666, 0xf5ec, 0xf829};

static const WindowItemRecord data_0209f394[22] = {
    { 2, 0x1, 135, 26, 114, 24, {0x403}, {2, 1, 0, 0}, 0, 2 },
    { 3, 0x1, 140, 54, 114, 24, {0x403}, {0, 2, 1, 1}, 0, 2 },
    { 4, 0x1, 140, 82, 114, 24, {0x403}, {1, 0, 2, 2}, 0, 2 },
    { 9, 0x8, 8, 170, 28, 20, {0x201}, {-1, -1, -1, -1}, 0, 3 },
    { 10, 0x8, 100, 170, 28, 20, {0x201}, {-1, -1, -1, -1}, 0, 3 },
    { 11, 0x8, 40, 170, 56, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
    { 8, 0x1, 134, 170, 88, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
    { 12, 0x2, 225, 170, 30, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
};

static const WindowCellRecord data_0209e888[13] = {
    { "union_b_obj_edit_nce.bin", {0}, 0x6000, {0}, {0x1}, 0, {0, 2, 1, 0} },
    { "union_b_obj_edit_nce.bin", {0}, 0x6000, {0}, {0x1}, 0, {0, 2, 1, 0} },
    { "union_b_obj_edit_nce.bin", {0x3}, 0x6000, {0}, {0x1}, 0, {3, 5, 4, 0} },
    { "union_b_obj_edit_nce.bin", {0x3}, 0x6000, {0}, {0x1}, 0, {3, 5, 4, 0} },
    { "union_b_obj_edit_nce.bin", {0x3}, 0x6000, {0}, {0x1}, 0, {3, 5, 4, 0} },
    { "union_b_obj_edit_nce.bin", {0x3}, 0x6000, {0}, {0x1}, 0, {3, 5, 4, 0} },
    { "union_b_obj_edit_nce.bin", {0x3}, 0x6000, {0}, {0x1}, 0, {3, 5, 4, 0} },
    { "union_b_obj_edit_nce.bin", {0x18}, 0x6000, {0}, {0x1}, 0, {24, 26, 25, 0} },
    { "union_b_obj_edit_nce.bin", {0x9}, 0x6000, {0}, {0x1}, 0, {9, 11, 10, 0} },
    { "union_b_obj_edit_nce.bin", {0xc}, 0x6000, {0}, {0x1}, 0, {12, 13, 13, 0} },
    { "union_b_obj_edit_nce.bin", {0xe}, 0x6000, {0}, {0x1}, 0, {14, 15, 15, 0} },
    { "union_b_obj_edit_nce.bin", {0x10}, 0x6000, {0}, {0x1}, 0, {16, 17, 16, 0} },
    { "union_b_obj_cockpit_nce.bin", {0x2}, 0xd000, {0x4}, {0x1}, 0, {2, 3, 3, 0} },
};

inline WindowCellRecord *CellTable_020b3a28() {
    static WindowCellRecord data_020b3a28[1] = {
        { "edit_ch_b_obj_00_nce.bin", {0}, 0xf000, {0x5}, {0x2}, 0, {0, 0, 0, 0} },
    };
    return data_020b3a28;
}

inline WindowCellRecord *CellTable_020b3a44() {
    static WindowCellRecord sGroupButtonCells[7] = {
        { "edit_ch_b_obj_00_nce.bin", {0x1}, 0xf000, {0x5}, {0x2}, 0, {1, 1, 1, 0} },
        { "edit_ch_b_obj_00_nce.bin", {0x2}, 0xf000, {0x5}, {0x1}, 0, {2, 2, 2, 0} },
        { "edit_ch_b_obj_00_nce.bin", {0x3}, 0xf000, {0x5}, {0x1}, 0, {3, 3, 3, 0} },
        { "edit_ch_b_obj_00_nce.bin", {0x4}, 0xf000, {0x5}, {0x1}, 0, {4, 4, 4, 0} },
        { "edit_ch_b_obj_00_nce.bin", {0x5}, 0xf000, {0x5}, {0x1}, 0, {5, 5, 5, 0} },
        { "edit_ch_b_obj_00_nce.bin", {0x6}, 0xf000, {0x5}, {0x1}, 0, {6, 6, 6, 0} },
        { "edit_ch_b_obj_00_nce.bin", {0x7}, 0xf000, {0x5}, {0x1}, 0, {7, 7, 7, 0} },
    };
    return sGroupButtonCells;
}

static const WindowCellRecord data_0209e834[3] = {
    { "union_b_obj_window_nce.bin", {0x3}, 0x19000, {0xd}, {0}, 0, {3, 5, 4, 0} },
    { "union_b_obj_window_nce.bin", {0x6}, 0x19000, {0xd}, {0}, 0, {6, 8, 7, 0} },
    { "union_b_obj_window_nce.bin", {0xa}, 0x19000, {0xd}, {0}, 0, {10, 10, 10, 0} },
};

static const WindowItemRecord sGroupButtonItems[22] = {
    { 0, 0x8000, 128, 96, 0, 0, {0}, {-1, -1, -1, -1}, 0, 0 },
    { 1, 0x8, 56, 34, 24, 24, {0}, {-1, -1, -1, -1}, 0, 1 },
    { 2, 0x8, 34, 63, 21, 47, {0}, {-1, -1, -1, -1}, 0, 1 },
    { 3, 0x8, 81, 63, 21, 47, {0}, {-1, -1, -1, -1}, 0, 1 },
    { 4, 0x8, 60, 63, 16, 38, {0}, {-1, -1, -1, -1}, 0, 1 },
    { 5, 0x8, 49, 102, 17, 60, {0}, {-1, -1, -1, -1}, 0, 1 },
    { 6, 0x8, 70, 102, 17, 60, {0}, {-1, -1, -1, -1}, 0, 1 },
};

static const Vec2s sCharacterEditCellPositions[16] = {
    {145, 52}, {177, 52}, {209, 52}, {145, 84}, {177, 84}, {209, 84}, {145, 116}, {177, 116}, {146, 136}, {146, 152}, {219, 134}, {219, 150}, {145, 52}, {145, 52}, {144, 134}, {144, 150},
};

static const WindowItemRecord sCharacterEditCursorItems[22] = {
    { 0, 0x8, 145, 52, 30, 30, {0}, {14, 3, 2, 1}, 0, 1 },
    { 1, 0x8, 177, 52, 30, 30, {0}, {16, 4, 0, 2}, 0, 1 },
    { 2, 0x8, 209, 52, 30, 30, {0}, {18, 5, 1, 0}, 0, 1 },
    { 3, 0x8, 145, 84, 30, 30, {0}, {0, 6, 5, 4}, 0, 1 },
    { 4, 0x8, 177, 84, 30, 30, {0}, {1, 7, 3, 5}, 0, 1 },
    { 5, 0x8, 209, 84, 30, 30, {0}, {2, 12, 4, 3}, 0, 1 },
    { 6, 0x8, 145, 116, 30, 30, {0}, {3, 8, 7, 7}, 0, 1 },
    { 7, 0x8, 177, 116, 30, 30, {0}, {4, 10, 6, 6}, 0, 1 },
    { 8, 0x8, 146, 136, 12, 11, {0}, {6, 14, 13, 9}, 0, 1 },
    { 9, 0x8, 161, 136, 12, 11, {0}, {6, 15, 8, 10}, 0, 1 },
    { 10, 0x8, 176, 136, 12, 11, {0}, {7, 16, 9, 11}, 0, 1 },
    { 11, 0x8, 191, 136, 12, 11, {0}, {7, 17, 10, 12}, 0, 1 },
    { 12, 0x8, 206, 136, 12, 11, {0}, {5, 18, 11, 13}, 0, 1 },
    { 13, 0x8, 221, 136, 12, 11, {0}, {5, 19, 12, 8}, 0, 1 },
    { 14, 0x8, 146, 152, 12, 11, {0}, {8, 0, 19, 15}, 0, 1 },
    { 15, 0x8, 161, 152, 12, 11, {0}, {9, 0, 14, 16}, 0, 1 },
    { 16, 0x8, 176, 152, 12, 11, {0}, {10, 1, 15, 17}, 0, 1 },
    { 17, 0x8, 191, 152, 12, 11, {0}, {11, 1, 16, 18}, 0, 1 },
    { 18, 0x8, 206, 152, 12, 11, {0}, {12, 2, 17, 19}, 0, 1 },
    { 19, 0x8, 221, 152, 12, 11, {0}, {13, 2, 18, 14}, 0, 1 },
};

static const WindowItemRecord data_0209facc[22] = {
    { 0, 0x1, 45, 152, 78, 20, {0}, {0, 0, 1, 1}, 0, 2 },
    { 1, 0x2, 133, 152, 78, 20, {0}, {1, 1, 0, 0}, 0, 2 },
    { 2, 0x8000, 120, 18, 0, 0, {0}, {-1, -1, -1, -1}, 0, 0 },
};

static const WindowItemRecord sRootMenuItems[22] = {
    { 7, 0x8, 135, 26, 114, 24, {0x403}, {4, 1, 0, 0}, 0, 2 },
    { 3, 0x1, 135, 54, 114, 24, {0x403}, {0, 2, 1, 1}, 0, 2 },
    { 4, 0x1, 135, 82, 114, 24, {0x403}, {1, 3, 2, 2}, 0, 2 },
    { 5, 0x1, 135, 110, 114, 24, {0x403}, {2, 4, 3, 3}, 0, 2 },
    { 6, 0x1, 135, 138, 114, 24, {0x403}, {3, 0, 4, 4}, 0, 2 },
    { 9, 0x8, 8, 170, 28, 20, {0x201}, {-1, -1, -1, -1}, 0, 3 },
    { 10, 0x8, 100, 170, 28, 20, {0x201}, {-1, -1, -1, -1}, 0, 3 },
    { 11, 0x8, 40, 170, 56, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
    { 8, 0x1, 134, 170, 88, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
    { 12, 0x2, 225, 170, 30, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
};

static const u32 sCharacterEditTextVramOffsets[6] = {0, 0x1000, 0x2000, 0x3000, 0x4000, 0x5000};

static const WindowItemRecord data_0209eec4[22] = {
    { 2, 0x1, 135, 26, 114, 24, {0x403}, {4, 1, 0, 0}, 0, 2 },
    { 3, 0x1, 140, 54, 114, 24, {0x403}, {0, 2, 1, 1}, 0, 2 },
    { 4, 0x1, 140, 82, 114, 24, {0x403}, {1, 3, 2, 2}, 0, 2 },
    { 5, 0x1, 140, 110, 114, 24, {0x403}, {2, 4, 3, 3}, 0, 2 },
    { 6, 0x1, 140, 138, 114, 24, {0x403}, {3, 0, 4, 4}, 0, 2 },
    { 9, 0x8, 8, 170, 28, 20, {0x201}, {-1, -1, -1, -1}, 0, 3 },
    { 10, 0x8, 100, 170, 28, 20, {0x201}, {-1, -1, -1, -1}, 0, 3 },
    { 11, 0x8, 40, 170, 56, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
    { 8, 0x1, 134, 170, 88, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
    { 12, 0x2, 225, 170, 30, 20, {0x201}, {-1, -1, -1, -1}, 0, 1 },
};

#include "Graphics/SpriteManager.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
// The look fields of the save data (+0x14..+0x1c of gSaveData) as CharacterLook.
#define SAVE_LOOK ((CharacterLook *) ((u8 *) gSaveData + 0x14))

// Rounds an integer through float, the way the camera set-up code does (the temporary keeps it from being folded).
static inline fx32 FxRound(s32 v) {
    s32 t = v;
    return (fx32) (t > 0 ? t + 0.5f : t - 0.5f);
}



extern MtxFx43 gIdentityMtx43;



CharacterEditScene::CharacterEditScene(u32 unk) :
    SceneBase(2, 0) {
    mModel   = NULL;
    mCamera  = NULL;
    mState   = 0;
    mStep  = 0;
    mStepFrames  = 0;
    mMessages = NULL;
    mMessageWindow = NULL;
    mRotateState = 0;
    mChoicesVisible = 0;
    mCutRoomModel = NULL;
    mMenuDepthMove = 0;
    mFromMyroom = unk;
    mModelAngle = 0x2cec;
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    for (u8 i = 0; i < 6; i++) {
        mTextBuffers[i]    = NULL;
        mTexts[i].mVisible  = 0;
        mTexts[i].mItemIndex  = -1;
    }
    mTextGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mTextGlyphCount = 0;
    mWindow0Result  = 0;
    mWindow1Result  = 0;
    mWindow2Result  = 0;
    mPalette = new ModelLightGroup(sCharacterEditLights);
    mMenuDepth = 0;
    for (u8 i = 0; i < 3; i++) {
        mMenuSelection[i] = 0;
    }
    InitLookLists();
    for (u8 i = 0; i < 16; i++) {
        mCellAnims[i] = NULL;
    }
    mRebuildModel = 0;
    mUnk_1b0 = 0;
    mChoiceApplied = 0;
    mWindowTextVisible = 0;
    mWindowTextBuffer = NULL;
    mWindowGlyphBuffer = NULL;
    mWindowTextBufferSize = 0;
    mWindowGlyphCount = 0;
    mNextScene = 2;
    mFixedItem = -1;
    mNameBuffer = NULL;
    mNameGlyphBuffer = NULL;
    mNameBufferSize = 0;
    mNameGlyphCount = 0;
    mNameplateCellAnim = NULL;
}

CharacterEditScene::~CharacterEditScene() {
    UnkStruct_0205401c *model = mModel;
    if (model != NULL) {
        model->Release();
        delete model;
    }
    ModelInstance *instance = mCutRoomModel;
    if (instance != NULL) {
        instance->mOwnFlags = 0xf;
        delete instance;
        mCutRoomModel = NULL;
    }
    CameraManager::Get().SetCamerasAndRequestMode(0, NULL, 0);
    delete mCamera;
    if (mWindows[0] != NULL) {
        delete mWindows[0];
    }
    if (mWindows[1] != NULL) {
        delete mWindows[1];
    }
    if (mWindows[2] != NULL) {
        delete mWindows[2];
    }
    delete mCursor;
    ResetTexts();
    if (mMessageWindow != NULL) {
        delete mMessageWindow;
    }
    for (u8 i = 0; i < 6; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
            mTextBuffers[i] = NULL;
        }
    }
    if (mWindowTextBuffer != NULL) {
        OS_Free(mWindowTextBuffer);
        mWindowTextBuffer = NULL;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mTextGlyphBuffer != NULL) {
        OS_Free(mTextGlyphBuffer);
        mTextGlyphBuffer = NULL;
    }
    if (mWindowGlyphBuffer != NULL) {
        OS_Free(mWindowGlyphBuffer);
        mWindowGlyphBuffer = NULL;
    }
    ModelLightGroup *palette = mPalette;
    if (palette != NULL) {
        delete palette;
    }
    CellAnimation *cell;
    u8 i = 0;
    do {
        cell = mCellAnims[i];
        if (cell != NULL) {
            delete cell;
        }
        i++;
    } while (i < 16);
    ClearName();
    if (mNameBuffer != NULL) {
        OS_Free(mNameBuffer);
        mNameBuffer = NULL;
    }
    if (mNameGlyphBuffer != NULL) {
        OS_Free(mNameGlyphBuffer);
        mNameGlyphBuffer = NULL;
    }
    cell = mNameplateCellAnim;
    if (cell != NULL) {
        delete cell;
    }
    SpriteManager::Get().ResetSlots();
}


void CharacterEditScene::ResetTexts() {
    u8 i = 0;
    do {
        mTexts[i].mVisible = 0;
        mTexts[i].mItemIndex = -1;
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
        i++;
    } while (i < 6);
    MI_CpuClear16(mWindowTextBuffer, mWindowTextBufferSize);
    mWindowTextVisible = 0;
    mMessageWindow->ClearBuffer();
}

void CharacterEditScene::SetText(u32 slot, u32 message, u32 value) {
    if (slot == 0) {
        mMessageWindow->StartMessage(message, 0, 1);
    } else {
        TextManager &text = TextManager::Get();
        s16 a;
        s16 b;
        b = 0;
        a = 0;
        MessageFile *file = (MessageFile *) mMessages;
        text.Print(file->GetText(message), file->GetLength(message), mTextBuffers[slot], 0x20, 2, &a, &b,
                   &mTexts[slot].mWidth, 0, 2, 0x21, 0xc, 0, 0, 1, 1, 1);
        if (&mTexts[slot].mHeight != NULL) {
            mTexts[slot].mHeight = b;
        }
        QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, sCharacterEditTextVramOffsets[slot], mTextBuffers[slot],
                      mTextBufferSize, 0);
    }
    mTexts[slot].mItemIndex = value;
    mTexts[slot].mVisible = 1;
}

void CharacterEditScene::DrawTexts() {
    if (mState == 0xd) {
        return;
    }
    WindowItem *items = mWindows[0]->mCursor->mItems;
    u8 i = 0;
    do {
        if (mTexts[i].mVisible != 0) {
            if (i == 0) {
                mMessageWindow->Update();
            } else {
                Vec2s pos = mWindows[0]->mCellPos[(u8) items[mTexts[i].mItemIndex].mId];
                s32 y = pos.y;
                SpriteScreen &screen = SpriteManager::Get().Screen(1);
                WindowItem *item = &items[mTexts[i].mItemIndex];
                screen.AddOam(mTextGlyphBuffer, (u8) mTextGlyphCount, (s16) (pos.x + item->mWidth / 2 - mTexts[i].mWidth / 2),
                                     (s16) (y + item->mHeight / 2 - mTexts[i].mHeight / 2), 0,
                                     (u16) (sCharacterEditTextVramOffsets[i] >> 7), 0xf, 0);
            }
        }
        i++;
    } while (i < 6);
    if (mWindowTextVisible != 0) {
        SpriteManager::Get().Screen(1).AddOam(mWindowGlyphBuffer, (u8) mWindowGlyphCount, 0x20, 0x28, 0, 0x20, 0xf, 0);
    }
}

void CharacterEditScene::UpdateInit() {
    switch (mStep) {
    case 0:
        func_02061c3c();
        LoadGraphics();
        mWindows[0]->mEnabled = 0;
        mWindows[1]->mEnabled = 0;
        mWindows[2]->mEnabled = 0;
        mWindows[1]->mButtons[0].mEnabled = 1;
        GX_SetVisiblePlane(0x1f);
        GXS_SetVisiblePlane(0x15);
        PrintName();
        mStep++;
        break;
    case 1:
        mStepFrames++;
        if (mStepFrames >= 3) {
            SpriteManager::Get().StartFade(2);
            mStepFrames = 0;
            mState = 1;
            mStep = 2;
        }
        break;
    }
}

void CharacterEditScene::UpdateRootMenu() {
    switch (mStep) {
    case 2:
        SpriteManager::Get();
        if (SpriteManager::sFade[0] > 1 | SpriteManager::sFade[1] > 1) {
            return;
        }
        mWindows[0]->mEnabled = 1;
        mWindows[1]->mEnabled = 1;
        if (mMenuDepthMove != 2) {
            mMenuSelection[mMenuDepth] = 0;
        }
        mWindows[0]->SetItems(10, (WindowItem *) sRootMenuItems, (s8) mMenuSelection[mMenuDepth]);
        if (mMenuSelection[mMenuDepth] == 0) {
            mWindows[0]->mCellAnims[(u8) mWindows[0]->GetSelectedId()]->SetAnimation(0x19);
        } else {
            mWindows[0]->mCellAnims[(u8) mWindows[0]->GetSelectedId()]->SetAnimation(4);
        }
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(0);
        GXS_SetVisiblePlane(0x15);
        {
            fx32 posZ = FxRound(sCameraPositionTargets[0][2] * 0x1000);
            fx32 posY = FxRound(sCameraPositionTargets[0][1] * 0x1000);
            Camera *cam = mCamera;
            cam->SetPosition(FxRound(sCameraPositionTargets[0][0] * 0x1000), posY, posZ);
            mCamera->SetTarget(FxRound(sCameraPositionTargets[0][3] * 0x1000), FxRound(sCameraPositionTargets[0][4] * 0x1000), FxRound(sCameraPositionTargets[0][5] * 0x1000));
        }
        {
            CameraManager &manager = CameraManager::Get();
            Camera *camera = manager.mCameras[manager.mCameraIndex];
            data_02211374.mCameraPosition = camera->mPosition;
            data_02211374.mCameraUp = camera->mUp;
            data_02211374.mCameraTarget = camera->mTarget;
            MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
            data_02211374.mFlags &= ~0xe8;
            manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
            manager.mView[manager.mCameraIndex] = data_02211374.mView;
            manager.mMtxA[manager.mCameraIndex] = *func_02032710();
            manager.mMtxB[manager.mCameraIndex] = *func_02032604();
            manager.mMtxC[manager.mCameraIndex] = *func_020323b8();
        }
        ResetTexts();
        SetText(0, 2, 0);
        switch (mFixedItem) {
        case 0:
            SetText(1, 7, 0);
            break;
        case 1:
            switch (mFromMyroom) {
            case 0:
                SetText(2, 0x17, 1);
                break;
            case 1:
                SetText(2, 0xf, 1);
                break;
            }
            break;
        case 2:
            switch (mFromMyroom) {
            case 0:
                SetText(3, 0xf, 2);
                break;
            case 1:
                SetText(3, 0x10, 2);
                break;
            }
            break;
        case 3:
            switch (mFromMyroom) {
            case 0:
                SetText(4, 0x10, 3);
                break;
            case 1:
                SetText(4, 0x11, 3);
                break;
            }
            break;
        case 4:
            SetText(5, 0x11, 4);
            break;
        }
        mStep = 3;
        break;
    case 3:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 7, 0);
        }
        switch (mFromMyroom) {
        case 0:
            if (mFixedItem != 1) {
                SetText(2, 0x17, 1);
            }
            if (mFixedItem != 2) {
                SetText(3, 0xf, 2);
            }
            if (mFixedItem != 3) {
                SetText(4, 0x10, 3);
            }
            if (mFixedItem != 4) {
                SetText(5, 0x11, 4);
            }
            break;
        case 1:
            if (mFixedItem != 1) {
                SetText(2, 0xf, 1);
            }
            if (mFixedItem != 2) {
                SetText(3, 0x10, 2);
            }
            if (mFixedItem != 3) {
                SetText(4, 0x11, 3);
            }
            break;
        }
        mStep = 4;
        break;
    case 4:
        if (mWindows[0]->mState == 1) {
            mStep = 5;
        }
        break;
    case 5:
        if (mWindow1Result == 3) {
            if (EnableSelectedGroupButtons()) {
                mWindows[0]->mState = 3;
                mWindows[1]->mState = 3;
                mStep = 10;
                return;
            }
        } else if (mWindow0Result == 3) {
            if (mWindows[0]->GetSelectedId() == 7) {
                SpriteManager::Get();
                SpriteManager::sFade[0] = 3;
                mStep = 6;
                mWindows[0]->mEnabled = 0;
                mWindows[1]->mEnabled = 0;
                return;
            }
        } else if (mWindow0Result == 1) {
            func_02063b10(1);
            SlideToFirstRow();
            mStep = 10;
            return;
        } else if (mWindow0Result == 2) {
            if (mChoiceApplied != 0 && mFromMyroom == 1) {
                mStep = 0xb;
                func_02064cd4(1);
                return;
            }
            if (mFromMyroom == 0) {
                StoreLook(NULL);
            }
            mStep = 9;
            func_02063b10(1);
            return;
        } else if (InputManager::Get().IsHeld(8) && mWindows[0]->mState == 1) {
            mUnk_1b0 = 1;
            func_02063b10(1);
            mWindows[0]->mState = 3;
            mStep = 10;
            SoundManager::Get().StartSeqArc(0);
            return;
        }
        UpdateRotateInput();
        break;
    case 6:
        SpriteManager::Get();
        if (SpriteManager::sFade[0] > 1) {
            return;
        }
        RandomizeLook();
        ApplyCursorItem();
        mStepFrames = 0;
        mStep = 7;
        break;
    case 7:
        mStepFrames++;
        if (mStepFrames != 4) {
            return;
        }
        SpriteManager::Get();
        SpriteManager::sFade[0] = 2;
        mStep = 8;
        break;
    case 8:
        if (SpriteManager::Get().IsFading()) {
            return;
        }
        mWindows[0]->mCellAnims[7]->SetAnimation(0x19);
        mStep = 5;
        mWindows[0]->mEnabled = 1;
        mWindows[1]->mEnabled = 1;
        break;
    case 9:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 0xd;
        mStep = 0x5a;
        break;
    case 10:
        if (mWindows[0]->mState != 4) {
            return;
        }
        if (SwitchToSelectedGroup()) {
            return;
        }
        if (mUnk_1b0 != 0) {
            mState = 0xc;
            mStep = 0x53;
            return;
        }
        if (mWindows[0]->GetSelectedId() == 3) {
            mState = 2;
            mStep = 0x10;
            if (mFromMyroom == 1) {
                mState = 3;
                mStep = 0x16;
            }
            mMenuSelection[mMenuDepth] = mWindows[0]->mCursor->mSelection;
            MoveMenuDepth(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 4) {
            mState = 3;
            mStep = 0x16;
            if (mFromMyroom == 1) {
                mState = 4;
                mStep = 0x38;
            }
            mMenuSelection[mMenuDepth] = mWindows[0]->mCursor->mSelection;
            MoveMenuDepth(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 5) {
            mState = 4;
            mStep = 0x38;
            if (mFromMyroom == 1) {
                mState = 5;
                mStep = 0x3f;
            }
            mMenuSelection[mMenuDepth] = mWindows[0]->mCursor->mSelection;
            MoveMenuDepth(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 6) {
            mState = 5;
            mStep = 0x3f;
            mMenuSelection[mMenuDepth] = mWindows[0]->mCursor->mSelection;
            MoveMenuDepth(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 8) {
            mState = 0xc;
            mStep = 0x53;
        }
        break;
    case 11:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mWindows[0]->mEnabled = 0;
        mWindows[1]->mEnabled = 0;
        mWindows[2]->mEnabled = 1;
        mWindows[2]->mCursor->mSelection = 1;
        mWindows[2]->mCellAnims[1]->SetAnimation(7);
        GXS_SetVisiblePlane(0x1d);
        mWindows[2]->mButtons[0].mEnabled = 1;
        mWindows[2]->mButtons[1].mEnabled = 1;
        mWindows[2]->mButtons[2].mEnabled = 1;
        ResetTexts();
        PrintWindowText();
        mStep = 0xc;
        mRotateState = 0;
        mWindows[0]->mCellAnims[9]->SetAnimation(0xc);
        mWindows[0]->mCellAnims[10]->SetAnimation(0xe);
        break;
    case 12:
        if (mWindow2Result == 1) {
            mStep = 0xd;
        } else if (mWindow2Result == 2) {
            mStep = 0xe;
        }
        break;
    case 13:
        if (mWindows[2]->mState != 4) {
            return;
        }
        func_02064cd4(0);
        mState = 0xd;
        mStep = 0x5a;
        break;
    case 14:
        if (mWindows[2]->mState != 4) {
            return;
        }
        mWindows[2]->Reset(-1);
        mWindows[2]->mEnabled = 0;
        mWindows[0]->Reset(0);
        mWindows[0]->mCellAnims[(u8) mWindows[0]->GetSelectedId()]->SetAnimation(0x19);
        mWindows[0]->mEnabled = 1;
        mWindows[1]->mEnabled = 1;
        ResetTexts();
        mStep = 0xf;
        break;
    case 15:
        if (mWindows[0]->mState != 1) {
            return;
        }
        func_02064cd4(0);
        GXS_SetVisiblePlane(0x15);
        mWindows[2]->mButtons[0].mEnabled = 0;
        mWindows[2]->mButtons[1].mEnabled = 0;
        mWindows[2]->mButtons[2].mEnabled = 0;
        SetText(0, 2, 0);
        SetText(1, 7, 0);
        switch (mFromMyroom) {
        case 0:
            SetText(2, 0x17, 1);
            SetText(3, 0xf, 2);
            SetText(4, 0x10, 3);
            SetText(5, 0x11, 4);
            break;
        case 1:
            SetText(2, 0xf, 1);
            SetText(3, 0x10, 2);
            SetText(4, 0x11, 3);
            break;
        }
        mStep = 5;
        break;
    }
}

void CharacterEditScene::UpdateSkinEdit() {
    switch (mStep) {
    case 0x10:
        mWindows[0]->SetItems(6, (WindowItem *) sEditMenuItems, -1);
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(1);
        SetupChoices();
        LoadChoiceCells("sub_skin.nce_ncg1D.bin", "sub_skin_ncl.bin", "sub_skin_nce.bin", "color_skin_ncl.bin");
        ResetTexts();
        SetText(0, 0x17, 0);
        if (mFixedItem == 0) {
            SetText(1, 0x17, 0);
        }
        mStep = 0x11;
        break;
    case 0x11:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 0x17, 0);
        }
        mStep = 0x12;
        break;
    case 0x12:
        if (mWindows[0]->mState == 1) {
            mStep = 0x13;
        }
        break;
    case 0x13:
        UpdateChoiceInput();
        if (mWindow1Result == 3) {
            if (EnableSelectedGroupButtons()) {
                mWindows[0]->mState = 3;
                mWindows[1]->mState = 3;
                mStep = 0x15;
                return;
            }
        } else if (mWindow0Result == 1) {
            mStep = 0x15;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        } else if (mWindow0Result == 2) {
            mStep = 0x14;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        }
        UpdateRotateInput();
        break;
    case 0x14:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 1;
        mStep = 2;
        MoveMenuDepth(2);
        break;
    case 0x15:
        if (mWindows[0]->mState != 4) {
            return;
        }
        if (SwitchToSelectedGroup()) {
            return;
        }
        mState = 1;
        mStep = 2;
        MoveMenuDepth(2);
        break;
    }
}

void CharacterEditScene::func_0205e958() {
    switch (mStep) {
    case 0x16: {
        if (mMenuDepthMove != 2) {
            mMenuSelection[mMenuDepth] = 1;
        }
        mWindows[0]->SetItems(10, (WindowItem *) data_0209eec4, (s8) mMenuSelection[mMenuDepth]);
        mWindows[0]->mCellAnims[(u8) mWindows[0]->GetSelectedId()]->SetAnimation(4);
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(0);
        fx32 posZ = FxRound(sCameraPositionTargets[1][2] * 0x1000);
        fx32 posY = FxRound(sCameraPositionTargets[1][1] * 0x1000);
        mCamera->SetPosition(FxRound(sCameraPositionTargets[1][0] * 0x1000), posY, posZ);
        fx32 tgtZ = FxRound(sCameraPositionTargets[1][5] * 0x1000);
        fx32 tgtY = FxRound(sCameraPositionTargets[1][4] * 0x1000);
        mCamera->SetTarget(FxRound(sCameraPositionTargets[1][3] * 0x1000), tgtY, tgtZ);
        CameraManager &manager = CameraManager::Get();
        Camera *camera = manager.mCameras[manager.mCameraIndex];
        data_02211374.mCameraPosition = camera->mPosition;
        data_02211374.mCameraUp = camera->mUp;
        data_02211374.mCameraTarget = camera->mTarget;
        MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
        data_02211374.mFlags &= ~0xe8;
        manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
        manager.mView[manager.mCameraIndex] = data_02211374.mView;
        manager.mMtxA[manager.mCameraIndex] = *func_02032710();
        manager.mMtxB[manager.mCameraIndex] = *func_02032604();
        manager.mMtxC[manager.mCameraIndex] = *func_020323b8();
        ResetTexts();
        SetText(0, 0xf, 0);
        switch (mFixedItem) {
        case 0:
            SetText(1, 0xf, 0);
            break;
        case 1:
            switch (mFromMyroom) {
            case 0:
                SetText(2, 0x12, 1);
                break;
            case 1:
                SetText(2, 0x13, 1);
                break;
            }
            break;
        case 2:
            switch (mFromMyroom) {
            case 0:
                SetText(3, 0x13, 2);
                break;
            case 1:
                SetText(3, 0x18, 2);
                break;
            }
            break;
        case 3:
            switch (mFromMyroom) {
            case 0:
                SetText(4, 0x18, 3);
                break;
            case 1:
                SetText(4, 0x14, 3);
                break;
            }
            break;
        case 4:
            SetText(5, 0x14, 4);
            break;
        }
        mStep = 0x17;
        break;
    }
    case 0x17:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 0xf, 0);
        }
        switch (mFromMyroom) {
        case 0:
            if (mFixedItem != 1) {
                SetText(2, 0x12, 1);
            }
            if (mFixedItem != 2) {
                SetText(3, 0x13, 2);
            }
            if (mFixedItem != 3) {
                SetText(4, 0x18, 3);
            }
            if (mFixedItem != 4) {
                SetText(5, 0x14, 4);
            }
            break;
        case 1:
            if (mFixedItem != 1) {
                SetText(2, 0x13, 1);
            }
            if (mFixedItem != 2) {
                SetText(3, 0x18, 2);
            }
            if (mFixedItem != 3) {
                SetText(4, 0x14, 3);
            }
            break;
        }
        mStep = 0x18;
        break;
    case 0x18:
        if (mWindows[0]->mState == 1) {
            mStep = 0x19;
        }
        break;
    case 0x19:
        if (mWindow1Result == 3) {
            if (EnableSelectedGroupButtons()) {
                if (mWindows[1]->GetSelectedId() != 1) {
                    mWindows[0]->mState = 3;
                    mWindows[1]->mState = 3;
                    mStep = 0x1b;
                    return;
                }
            }
        } else if (mWindow0Result == 1) {
            if (mWindows[0]->GetSelectedId() == 2) {
                mStep = 0x1a;
                func_02063b10(1);
                SlideToParent();
                return;
            }
            mStep = 0x1b;
            func_02063b10(1);
            SlideToFirstRow();
            return;
        } else if (mWindow0Result == 2) {
            mStep = 0x1a;
            func_02063b10(1);
            SlideToParent();
            return;
        } else if (InputManager::Get().IsHeld(8) && mWindows[0]->mState == 1) {
            mUnk_1b0 = 1;
            func_02063b10(1);
            mWindows[0]->mState = 3;
            mStep = 0x1b;
            return;
        }
        UpdateRotateInput();
        break;
    case 0x1a:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 1;
        mStep = 2;
        MoveMenuDepth(2);
        break;
    case 0x1b:
        if (mWindows[0]->mState != 4) {
            return;
        }
        if (SwitchToSelectedGroup()) {
            return;
        }
        if (mUnk_1b0 != 0) {
            mState = 0xc;
            mStep = 0x53;
            return;
        }
        if (mWindows[0]->GetSelectedId() == 3) {
            mState = 6;
            mStep = 0x1c;
            if (mFromMyroom == 1) {
                mState = 7;
                mStep = 0x23;
            }
            mMenuSelection[mMenuDepth] = mWindows[0]->mCursor->mSelection;
            MoveMenuDepth(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 4) {
            mState = 7;
            mStep = 0x23;
            if (mFromMyroom == 1) {
                mState = 8;
                mStep = 0x2a;
            }
            mMenuSelection[mMenuDepth] = mWindows[0]->mCursor->mSelection;
            MoveMenuDepth(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 5) {
            mState = 8;
            mStep = 0x2a;
            if (mFromMyroom == 1) {
                mState = 9;
                mStep = 0x31;
            }
            mMenuSelection[mMenuDepth] = mWindows[0]->mCursor->mSelection;
            MoveMenuDepth(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 6) {
            mState = 9;
            mStep = 0x31;
            mMenuSelection[mMenuDepth] = mWindows[0]->mCursor->mSelection;
            MoveMenuDepth(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 8) {
            mState = 0xc;
            mStep = 0x53;
        }
        break;
    }
}

void CharacterEditScene::UpdateFaceEdit() {
    switch (mStep) {
    case 0x1c:
        mWindows[0]->SetItems(6, (WindowItem *) sEditMenuItems, -1);
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(1);
        SetupChoices();
        LoadChoiceCells("sub_face_male.nce_ncg1D.bin", "sub_face_male_ncl.bin", "sub_face_male_nce.bin", "color_face_male_ncl.bin");
        ResetTexts();
        SetText(0, 0x12, 0);
        if (mFixedItem == 0) {
            SetText(1, 0x12, 0);
        }
        mStep = 0x1d;
        break;
    case 0x1d:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 0x12, 0);
        }
        mStep = 0x1e;
        break;
    case 0x1e:
        if (mWindows[0]->mState == 1) {
            mStep = 0x1f;
        }
        break;
    case 0x1f:
        UpdateChoiceInput();
        if (mWindow1Result == 3) {
            if (EnableSelectedGroupButtons()) {
                if (mWindows[1]->GetSelectedId() != 1) {
                    mWindows[0]->mState = 3;
                    mWindows[1]->mState = 3;
                    mStep = 0x22;
                    return;
                }
            }
        } else if (mWindow0Result == 1) {
            mStep = 0x21;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        } else if (mWindow0Result == 2) {
            mStep = 0x21;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        }
        UpdateRotateInput();
        break;
    case 0x20:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 3;
        mStep = 0x16;
        MoveMenuDepth(2);
        break;
    case 0x21:
        mStep = 0x20;
        break;
    case 0x22:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mChoicesVisible = 0;
        SwitchToSelectedGroup();
        break;
    }
}

void CharacterEditScene::UpdateHairEdit() {
    switch (mStep) {
    case 0x23:
        mWindows[0]->SetItems(6, (WindowItem *) sEditMenuItems, -1);
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(1);
        SetupChoices();
        LoadChoiceCells("sub_hair_male.nce_ncg1D.bin", "sub_hair_male_ncl.bin", "sub_hair_male_nce.bin", "color_hair_male_ncl.bin");
        ResetTexts();
        SetText(0, 0x13, 0);
        if (mFixedItem == 0) {
            SetText(1, 0x13, 0);
        }
        mStep = 0x24;
        break;
    case 0x24:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 0x13, 0);
        }
        mStep = 0x25;
        break;
    case 0x25:
        if (mWindows[0]->mState == 1) {
            mStep = 0x26;
        }
        break;
    case 0x26:
        UpdateChoiceInput();
        if (mWindow1Result == 3) {
            if (EnableSelectedGroupButtons()) {
                if (mWindows[1]->GetSelectedId() != 1) {
                    mWindows[0]->mState = 3;
                    mWindows[1]->mState = 3;
                    mStep = 0x29;
                    return;
                }
            }
        } else if (mWindow0Result == 1) {
            mStep = 0x28;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        } else if (mWindow0Result == 2) {
            mStep = 0x27;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        }
        UpdateRotateInput();
        break;
    case 0x27:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 3;
        mStep = 0x16;
        MoveMenuDepth(2);
        break;
    case 0x28:
        mStep = 0x27;
        break;
    case 0x29:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mChoicesVisible = 0;
        SwitchToSelectedGroup();
        break;
    }
}

void CharacterEditScene::UpdateCapEdit() {
    switch (mStep) {
    case 0x2a:
        mWindows[0]->SetItems(6, (WindowItem *) sEditMenuItems, -1);
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(1);
        SetupChoices();
        LoadChoiceCells("sub_cap_male.nce_ncg1D.bin", "sub_cap_male_ncl.bin", "sub_cap_male_nce.bin", "color_cap_male_ncl.bin");
        ResetTexts();
        SetText(0, 0x18, 0);
        if (mFixedItem == 0) {
            SetText(1, 0x18, 0);
        }
        mStep = 0x2b;
        break;
    case 0x2b:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 0x18, 0);
        }
        mStep = 0x2c;
        break;
    case 0x2c:
        if (mWindows[0]->mState == 1) {
            mStep = 0x2d;
        }
        break;
    case 0x2d:
        UpdateChoiceInput();
        if (mWindow1Result == 3) {
            if (EnableSelectedGroupButtons()) {
                if (mWindows[1]->GetSelectedId() != 1) {
                    mWindows[0]->mState = 3;
                    mWindows[1]->mState = 3;
                    mStep = 0x30;
                    return;
                }
            }
        } else if (mWindow0Result == 1) {
            mStep = 0x2f;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        } else if (mWindow0Result == 2) {
            mStep = 0x2e;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        }
        UpdateRotateInput();
        break;
    case 0x2e:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 3;
        mStep = 0x16;
        MoveMenuDepth(2);
        break;
    case 0x2f:
        mStep = 0x2e;
        break;
    case 0x30:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mChoicesVisible = 0;
        SwitchToSelectedGroup();
        break;
    }
}

void CharacterEditScene::UpdateGlassesEdit() {
    switch (mStep) {
    case 0x31:
        mWindows[0]->SetItems(6, (WindowItem *) sEditMenuItems, -1);
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(1);
        SetupChoices();
        LoadChoiceCells("sub_glasses.nce_ncg1D.bin", "sub_glasses_ncl.bin", "sub_glasses_nce.bin", "color_glasses_fr_ncl.bin");
        ResetTexts();
        SetText(0, 0x14, 0);
        if (mFixedItem == 0) {
            SetText(1, 0x14, 0);
        }
        mStep = 0x32;
        break;
    case 0x32:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 0x14, 0);
        }
        mStep = 0x33;
        break;
    case 0x33:
        if (mWindows[0]->mState == 1) {
            mStep = 0x34;
        }
        break;
    case 0x34:
        UpdateChoiceInput();
        if (mWindow1Result == 3) {
            if (EnableSelectedGroupButtons()) {
                if (mWindows[1]->GetSelectedId() != 1) {
                    mWindows[0]->mState = 3;
                    mWindows[1]->mState = 3;
                    mStep = 0x37;
                    return;
                }
            }
        } else if (mWindow0Result == 1) {
            mStep = 0x36;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        } else if (mWindow0Result == 2) {
            mStep = 0x35;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        }
        UpdateRotateInput();
        break;
    case 0x35:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 3;
        mStep = 0x16;
        MoveMenuDepth(2);
        break;
    case 0x36:
        mStep = 0x35;
        break;
    case 0x37:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mChoicesVisible = 0;
        SwitchToSelectedGroup();
        break;
    }
}

void CharacterEditScene::UpdateTopEdit() {
    switch (mStep) {
    case 0x38: {
        mWindows[0]->SetItems(6, (WindowItem *) sEditMenuItems, -1);
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(1);
        SetupChoices();
        LoadChoiceCells("sub_top_male.nce_ncg1D.bin", "sub_top_male_ncl.bin", "sub_top_male_nce.bin",
                      "color_top_male_ncl.bin");
        mCamera->SetPosition(FxRound(sCameraPositionTargets[2][0] * 0x1000), FxRound(sCameraPositionTargets[2][1] * 0x1000), FxRound(sCameraPositionTargets[2][2] * 0x1000));
        mCamera->SetTarget(FxRound(sCameraPositionTargets[2][3] * 0x1000), FxRound(sCameraPositionTargets[2][4] * 0x1000), FxRound(sCameraPositionTargets[2][5] * 0x1000));
        CameraManager &manager = CameraManager::Get();
        Camera *camera = manager.mCameras[manager.mCameraIndex];
        data_02211374.mCameraPosition = camera->mPosition;
        data_02211374.mCameraUp = camera->mUp;
        data_02211374.mCameraTarget = camera->mTarget;
        MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
        data_02211374.mFlags &= ~0xe8;
        manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
        manager.mView[manager.mCameraIndex] = data_02211374.mView;
        manager.mMtxA[manager.mCameraIndex] = *func_02032710();
        manager.mMtxB[manager.mCameraIndex] = *func_02032604();
        manager.mMtxC[manager.mCameraIndex] = *func_020323b8();
        ResetTexts();
        SetText(0, 0x10, 0);
        if (mFixedItem == 0) {
            SetText(1, 0x10, 0);
        }
        mStep = 0x39;
        break;
    }
    case 0x39:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 0x10, 0);
        }
        mStep = 0x3a;
        break;
    case 0x3a:
        if (mWindows[0]->mState == 1) {
            mStep = 0x3b;
        }
        break;
    case 0x3b:
        UpdateChoiceInput();
        if (mWindow1Result == 3) {
            if (EnableSelectedGroupButtons()) {
                if (mWindows[1]->GetSelectedId() != 2 && mWindows[1]->GetSelectedId() != 3 &&
                    mWindows[1]->GetSelectedId() != 4) {
                    mWindows[0]->mState = 3;
                    mWindows[1]->mState = 3;
                    mStep = 0x3e;
                    return;
                }
            }
        } else if (mWindow0Result == 1) {
            mStep = 0x3d;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        } else if (mWindow0Result == 2) {
            mStep = 0x3c;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        }
        UpdateRotateInput();
        break;
    case 0x3c:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 1;
        mStep = 2;
        MoveMenuDepth(2);
        break;
    case 0x3d:
        mStep = 0x3c;
        break;
    case 0x3e:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mChoicesVisible = 0;
        SwitchToSelectedGroup();
        break;
    }
}

void CharacterEditScene::func_020604e8() {
    switch (mStep) {
    case 0x3f: {
        if (mMenuDepthMove != 2) {
            mMenuSelection[mMenuDepth] = 1;
        }
        mWindows[0]->SetItems(8, (WindowItem *) data_0209f394, (s8) mMenuSelection[mMenuDepth]);
        mWindows[0]->mCellAnims[(u8) mWindows[0]->GetSelectedId()]->SetAnimation(4);
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(0);
        mCamera->SetPosition(FxRound(sCameraPositionTargets[3][0] * 0x1000), FxRound(sCameraPositionTargets[3][1] * 0x1000), FxRound(sCameraPositionTargets[3][2] * 0x1000));
        mCamera->SetTarget(FxRound(sCameraPositionTargets[3][3] * 0x1000), FxRound(sCameraPositionTargets[3][4] * 0x1000), FxRound(sCameraPositionTargets[3][5] * 0x1000));
        CameraManager &manager = CameraManager::Get();
        Camera *camera = manager.mCameras[manager.mCameraIndex];
        data_02211374.mCameraPosition = camera->mPosition;
        data_02211374.mCameraUp = camera->mUp;
        data_02211374.mCameraTarget = camera->mTarget;
        MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
        data_02211374.mFlags &= ~0xe8;
        manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
        manager.mView[manager.mCameraIndex] = data_02211374.mView;
        manager.mMtxA[manager.mCameraIndex] = *func_02032710();
        manager.mMtxB[manager.mCameraIndex] = *func_02032604();
        manager.mMtxC[manager.mCameraIndex] = *func_020323b8();
        ResetTexts();
        SetText(0, 0x11, 0);
        switch (mFixedItem) {
        case 0:
            SetText(1, 0x11, 0);
            break;
        case 1:
            SetText(2, 0x15, 1);
            break;
        case 2:
            SetText(3, 0x16, 2);
            break;
        }
        mStep = 0x40;
        break;
    }
    case 0x40:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 0x11, 0);
        }
        if (mFixedItem != 1) {
            SetText(2, 0x15, 1);
        }
        if (mFixedItem != 2) {
            SetText(3, 0x16, 2);
        }
        mStep = 0x41;
        break;
    case 0x41:
        if (mWindows[0]->mState == 1) {
            mStep = 0x42;
        }
        break;
    case 0x42:
        if (mWindow1Result == 3) {
            if (EnableSelectedGroupButtons()) {
                if (mWindows[1]->GetSelectedId() != 5 && mWindows[1]->GetSelectedId() != 6) {
                    mWindows[0]->mState = 3;
                    mWindows[1]->mState = 3;
                    mStep = 0x44;
                    return;
                }
            }
        } else if (mWindow0Result == 1) {
            if (mWindows[0]->GetSelectedId() == 2) {
                mStep = 0x43;
                func_02063b10(1);
                SlideToParent();
                return;
            }
            mStep = 0x44;
            func_02063b10(1);
            SlideToFirstRow();
            return;
        } else if (mWindow0Result == 2) {
            mStep = 0x43;
            func_02063b10(1);
            SlideToParent();
            return;
        } else if (InputManager::Get().IsHeld(8) && mWindows[0]->mState == 1) {
            mUnk_1b0 = 1;
            func_02063b10(1);
            mWindows[0]->mState = 3;
            mStep = 0x44;
            return;
        }
        UpdateRotateInput();
        break;
    case 0x43:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 1;
        mStep = 2;
        MoveMenuDepth(2);
        break;
    case 0x44:
        if (mWindows[0]->mState != 4) {
            return;
        }
        if (SwitchToSelectedGroup()) {
            return;
        }
        if (mUnk_1b0 != 0) {
            mState = 0xc;
            mStep = 0x53;
            return;
        }
        if (mWindows[0]->GetSelectedId() == 3) {
            mState = 0xa;
            mStep = 0x45;
            mMenuSelection[mMenuDepth] = mWindows[0]->mCursor->mSelection;
            MoveMenuDepth(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 4) {
            mState = 0xb;
            mStep = 0x4c;
            mMenuSelection[mMenuDepth] = mWindows[0]->mCursor->mSelection;
            MoveMenuDepth(1);
            return;
        }
        if (mWindows[0]->GetSelectedId() == 8) {
            mState = 0xc;
            mStep = 0x53;
        }
        break;
    }
}

void CharacterEditScene::UpdateBottomEdit() {
    switch (mStep) {
    case 0x45:
        mWindows[0]->SetItems(6, (WindowItem *) sEditMenuItems, -1);
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(1);
        SetupChoices();
        LoadChoiceCells("sub_bottom_male.nce_ncg1D.bin", "sub_bottom_male_ncl.bin", "sub_bottom_male_nce.bin", "color_bottom_male_ncl.bin");
        ResetTexts();
        SetText(0, 0x15, 0);
        if (mFixedItem == 0) {
            SetText(1, 0x15, 0);
        }
        mStep = 0x46;
        break;
    case 0x46:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 0x15, 0);
        }
        mStep = 0x47;
        break;
    case 0x47:
        if (mWindows[0]->mState == 1) {
            mStep = 0x48;
        }
        break;
    case 0x48:
        UpdateChoiceInput();
        if (mWindow1Result == 3 && EnableSelectedGroupButtons()) {
            if (mWindows[1]->GetSelectedId() != 5 && mWindows[1]->GetSelectedId() != 6) {
                mWindows[0]->mState = 3;
                mWindows[1]->mState = 3;
                mStep = 0x4b;
                return;
            }
        }
        if (mWindow0Result == 1) {
            mStep = 0x4a;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        } else if (mWindow0Result == 2) {
            mStep = 0x49;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        }
        UpdateRotateInput();
        break;
    case 0x49:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 5;
        mStep = 0x3f;
        MoveMenuDepth(2);
        break;
    case 0x4a:
        mStep = 0x49;
        break;
    case 0x4b:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mChoicesVisible = 0;
        SwitchToSelectedGroup();
        break;
    }
}

void CharacterEditScene::UpdateShoesEdit() {
    switch (mStep) {
    case 0x4c:
        mWindows[0]->SetItems(6, (WindowItem *) sEditMenuItems, -1);
        mWindows[1]->Reset(-1);
        func_02063b10(0);
        SetupSlideIn();
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(1);
        SetupChoices();
        LoadChoiceCells("sub_shoes_male.nce_ncg1D.bin", "sub_shoes_male_ncl.bin", "sub_shoes_male_nce.bin", "color_shoes_male_ncl.bin");
        ResetTexts();
        SetText(0, 0x16, 0);
        if (mFixedItem == 0) {
            SetText(1, 0x16, 0);
        }
        mStep = 0x4d;
        break;
    case 0x4d:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        if (mFixedItem != 0) {
            SetText(1, 0x16, 0);
        }
        mStep = 0x4e;
        break;
    case 0x4e:
        if (mWindows[0]->mState == 1) {
            mStep = 0x4f;
        }
        break;
    case 0x4f:
        UpdateChoiceInput();
        if (mWindow1Result == 3 && EnableSelectedGroupButtons()) {
            if (mWindows[1]->GetSelectedId() != 5 && mWindows[1]->GetSelectedId() != 6) {
                mWindows[0]->mState = 3;
                mWindows[1]->mState = 3;
                mStep = 0x52;
                return;
            }
        }
        if (mWindow0Result == 1) {
            mStep = 0x51;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        } else if (mWindow0Result == 2) {
            mStep = 0x50;
            func_02063b10(1);
            SlideToParent();
            func_020638a8(0);
            return;
        }
        UpdateRotateInput();
        break;
    case 0x50:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 5;
        mStep = 0x3f;
        MoveMenuDepth(2);
        break;
    case 0x51:
        mStep = 0x50;
        break;
    case 0x52:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mChoicesVisible = 0;
        SwitchToSelectedGroup();
        break;
    }
}

void CharacterEditScene::func_020612cc() {
    switch (mStep) {
    case 0x53: {
        mWindows[0]->SetItems(2, (WindowItem *) data_0209f12c, 0);
        mWindows[0]->mCellAnims[0]->SetAnimation(1);
        func_020637a8();
        EnableGroupButtons();
        func_020638a8(0);
        GXS_SetVisiblePlane(0x14);
        fx32 posZ = FxRound(sCameraPositionTargets[0][2] * 0x1000);
        fx32 posY = FxRound(sCameraPositionTargets[0][1] * 0x1000);
        Camera *cam = mCamera;
        cam->SetPosition(FxRound(sCameraPositionTargets[0][0] * 0x1000), posY, posZ);
        mCamera->SetTarget(FxRound(sCameraPositionTargets[0][3] * 0x1000), FxRound(sCameraPositionTargets[0][4] * 0x1000), FxRound(sCameraPositionTargets[0][5] * 0x1000));
        CameraManager &manager = CameraManager::Get();
        Camera *camera = manager.mCameras[manager.mCameraIndex];
        data_02211374.mCameraPosition = camera->mPosition;
        data_02211374.mCameraUp = camera->mUp;
        data_02211374.mCameraTarget = camera->mTarget;
        MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
        data_02211374.mFlags &= ~0xe8;
        manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
        manager.mView[manager.mCameraIndex] = data_02211374.mView;
        manager.mMtxA[manager.mCameraIndex] = *func_02032710();
        manager.mMtxB[manager.mCameraIndex] = *func_02032604();
        manager.mMtxC[manager.mCameraIndex] = *func_020323b8();
        ResetTexts();
        SetText(0, 3, 0);
        mStep = 0x54;
        mUnk_1b0 = 0;
        mRotateState = 3;
        break;
    }
    case 0x54:
        if (mWindows[0]->mSlideStep != 1) {
            return;
        }
        SetText(1, 8, 0);
        SetText(2, 9, 1);
        mStep = 0x55;
        break;
    case 0x55:
        if (mWindows[0]->mState == 1) {
            mStep = 0x56;
        }
        break;
    case 0x56:
        if (mWindow0Result != 1) {
            return;
        }
        if (mWindows[0]->GetSelectedId() == 0) {
            StoreLook(NULL);
            mModel->func_0205461c(0x17, 0);
            data_020d433c.mUnk_14_1 = 1;
            mStep = 0x59;
            mStepFrames = 0;
            return;
        }
        if (mWindows[0]->GetSelectedId() == 1) {
            mStep = 0x57;
        }
        break;
    case 0x57:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mState = 1;
        mStep = 2;
        mMenuDepth = 0;
        MoveMenuDepth(0);
        break;
    case 0x59:
        if (mWindows[0]->mState != 4) {
            return;
        }
        mStepFrames++;
        if (mStepFrames == 100) {
            FrameTask::Get().StartChannel(1, 0x183);
        }
        if (mModel->IsAnimEndReached()) {
            mNextScene = 1;
            mState = 0xd;
            mStep = 0x5a;
            mStepFrames = 0;
        }
        break;
    }
}

void CharacterEditScene::UpdateFinish() {
    switch (mStep) {
    case 0x5a:
        SpriteManager::Get().StartFade(3);
        mStep = 0x5b;
        break;
    case 0x5b:
        if (!SpriteManager::Get().IsFading()) {
            mStep = 0x5c;
        }
        break;
    case 0x5c:
        RequestScene(mNextScene);
        mFinished = TRUE;
        break;
    }
}

void CharacterEditScene::Update() {
    if (mRebuildModel != 0) {
        u32 value = ((CharacterModel *) mModel->mCharacter)->GetHeadFrame();
        mRebuildModel = 0;
        UnkStruct_0205401c *model = mModel;
        if (model != NULL) {
            model->Release();
            delete model;
        }
        CharacterLook look;
        StoreLook(&look);
        mModel = new UnkStruct_0205401c();
        mModel->Create(0x15, 1, (u32) &look, 0);
        mModel->LoadShadow();
        mModel->mCharacter->mRotY = mModelAngle;
        mModel->func_0205461c(0, 1);
        ((CharacterModel *) mModel->mCharacter)->SetHeadFrame(value);
    }
    switch (mState) {
    case 0:
        UpdateInit();
        break;
    case 1:
        UpdateRootMenu();
        break;
    case 2:
        UpdateSkinEdit();
        break;
    case 3:
        func_0205e958();
        break;
    case 6:
        UpdateFaceEdit();
        break;
    case 7:
        UpdateHairEdit();
        break;
    case 8:
        UpdateCapEdit();
        break;
    case 9:
        UpdateGlassesEdit();
        break;
    case 4:
        UpdateTopEdit();
        break;
    case 5:
        func_020604e8();
        break;
    case 10:
        UpdateBottomEdit();
        break;
    case 11:
        UpdateShoesEdit();
        break;
    case 12:
        func_020612cc();
        break;
    case 13:
        UpdateFinish();
        break;
    }
    mWindow0Result = mWindows[0]->Update(1);
    mWindow1Result = mWindows[1]->Update(1);
    mWindow2Result = mWindows[2]->Update(1);
    Draw();
    UnkStruct_0205401c_04 *data = mModel->mCharacter;
    u16 angle = data->mRotY;
    u16 delta = angle - 0x2cec;
    switch (mRotateState) {
    case 1:
        data->mRotY = angle + 0xcd;
        break;
    case 2:
        data->mRotY = angle - 0xcd;
        break;
    case 3:
        if (delta > 0x8000) {
            data->mRotY = angle + 0x1000;
            if (delta > (u16) (data->mRotY - 0x2cec)) {
                data->mRotY = 0x2cec;
                mRotateState = 0;
            }
        } else {
            data->mRotY = angle - 0x1000;
            if (delta < (u16) (data->mRotY - 0x2cec)) {
                data->mRotY = 0x2cec;
                mRotateState = 0;
            }
        }
        break;
    }
    mModelAngle = data->mRotY;
}

void CharacterEditScene::func_02061c3c() {
    SpriteManager::Get().SetupMode(0, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(0);
    G2S_SetBlendBrightness(0x1f, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG0Priority(2);
    G2S_SetBG1Control(0, 1, 2, 2, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG1Priority(1);
    G2S_SetBG2Control(0, 1, 4, 4, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Control(0, 1, 6, 7, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBG3Priority(0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    mCamera = new Camera();
    mCamera->SetUp(0, 0x1000, 0);
    mCamera->SetClip(0x1000, 0x1388000);
    mCamera->SetFov(0x2aaa);
    fx32 posZ = FxRound(sCameraPositionTargets[0][2] * 0x1000);
    fx32 posY = FxRound(sCameraPositionTargets[0][1] * 0x1000);
    Camera *cam = mCamera;
    cam->SetPosition(FxRound(sCameraPositionTargets[0][0] * 0x1000), posY, posZ);
    mCamera->SetTarget(FxRound(sCameraPositionTargets[0][3] * 0x1000), FxRound(sCameraPositionTargets[0][4] * 0x1000), FxRound(sCameraPositionTargets[0][5] * 0x1000));
    CameraManager::Get().mCameraIndex = 0;
    CameraManager::Get().SetCamerasAndRequestMode(1, mCamera, 0);
    CameraManager &manager = CameraManager::Get();
    Camera *camera = manager.mCameras[manager.mCameraIndex];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp = camera->mUp;
    data_02211374.mCameraTarget = camera->mTarget;
    MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
    manager.mView[manager.mCameraIndex] = data_02211374.mView;
    manager.mMtxA[manager.mCameraIndex] = *func_02032710();
    manager.mMtxB[manager.mCameraIndex] = *func_02032604();
    manager.mMtxC[manager.mCameraIndex] = *func_020323b8();
}

void CharacterEditScene::LoadGraphics() {
    u32 size;
    mWindows[0] = NULL;
    mWindows[0] = new SelectWindow(10, (WindowItemRecord *) sRootMenuItems, -1);
    mWindows[1] = NULL;
    mWindows[1] = new SelectWindow(7, sGroupButtonItems, -1);
    mWindows[1]->mUnk_07e = 1;
    mWindows[2] = NULL;
    mWindows[2] = new SelectWindow(3, data_0209facc, -1);
    mCursor = new SelectWindowCursor(0x14, sCharacterEditCursorItems, 0);
    func_020637a8();
    ChangeDir("/Game/2d/CharacterEditor/Bg");
    void *data;
    data = LoadFile("edit_ch_b_bg_00_ncl.bin", 0, 0, 0, &size, 0);
    QueueUpload(1, 1, 0, data, size, 1);
    data = LoadFile("edit_ch_b_bg_01_ncg.bin", 0, 0, 0, &size, 0);
    QueueUpload(1, 0xb, 0, data, size, 1);
    data = LoadFile("edit_ch_b_bg_01_nsc.bin", 0, 0, 0, &size, 0);
    QueueUpload(1, 7, 0, data, size, 1);
    data = LoadFile("edit_ch_b_bg_00_ncl.bin", 0, 0, 0, &size, 0);
    QueueUpload(1, 1, 0x4000, data, size, 1);
    data = LoadFile("edit_ch_b_bg_00_ncg.bin", 0, 0, 0, &size, 0);
    QueueUpload(1, 0xd, 0, data, size, 1);
    data = LoadFile("edit_ch_b_bg_00_nsc.bin", 0, 0, 0, &size, 0);
    QueueUpload(1, 9, 0, data, size, 1);
    ChangeDir("/Game/2D/union_b");
    data = LoadFile("union_b_bg_window_ncl.bin", 0, 0, 0, &size, 0);
    QueueUpload(1, 1, 0x6000, data, size, 1);
    data = LoadFile("union_b_bg_window_ncg.bin", 0, 0, 0, &size, 0);
    QueueUpload(1, 0xe, 0, data, size, 1);
    data = LoadFile("union_b_bg_window_b_nsc.bin", 0, 0, 0, &size, 0);
    QueueUpload(1, 0xa, 0, data, size, 1);
    SpriteScreen &screen1 = SpriteManager::Get().Screen(1);
    screen1.SetObjVramMode(3);
    ChangeDir("/Game/2d/union_b");
    screen1.LoadCharacters("union_b_obj_edit.nce_ncg1D.bin", 0x6000, 0);
    screen1.LoadPalette("union_b_obj_edit_ncl.bin", 0, 0);
    screen1.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0xd000, 0);
    screen1.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x80, 0);
    screen1.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x19000, 0);
    screen1.LoadPalette("union_b_obj_window_ncl.bin", 0x1a0, 0);
    mWindows[2]->SetCells(3, data_0209e834);
    mWindows[0]->SetCells(0xd, data_0209e888);
    ChangeDir("/Game/2d/CharacterEditor/Obj");
    screen1.LoadCharacters("edit_ch_b_obj_00.nce_ncg1D.bin", 0xf000, 0);
    const char *pal = "edit_ch_b_obj_00_ncl.bin";
    screen1.LoadPalette(pal, 0xa0, 0);
    mWindows[0]->SetCells(1, CellTable_020b3a28());
    mWindows[1]->SetCells(7, CellTable_020b3a44());
    ChangeDir("/");
    ChangeDir("/Game/2d/CharacterEditor/Obj");
    screen1.LoadCharacters("color_base.nce_ncg1D.bin", 0x14000, 0);
    screen1.LoadPalette("color_base_ncl.bin", 0x120, 0);
    mCellAnims[8] = new CellAnimation("color_base_nce.bin", 0, 1, 1);
    mCellAnims[8]->mCharBase = screen1.GetCharBase(0x14000);
    mCellAnims[8]->mPaletteBase = 9;
    mCellAnims[8]->SetAnimation(0);
    screen1.LoadPalette("color_base_ncl.bin", 0x140, 0);
    mCellAnims[9] = new CellAnimation("color_base_nce.bin", 0, 1, 1);
    mCellAnims[9]->mCharBase = screen1.GetCharBase(0x14000);
    mCellAnims[9]->mPaletteBase = 10;
    mCellAnims[9]->SetAnimation(0);
    ChangeDir("/Game/2d/union_b");
    u8 i = 10;
    do {
        mCellAnims[i] = new CellAnimation("union_b_obj_edit_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = screen1.GetCharBase(0x6000);
        mCellAnims[i]->mPaletteBase = 0;
        i++;
    } while (i < 16);
    mCellAnims[10]->SetAnimation(0x16);
    mCellAnims[11]->SetAnimation(0x17);
    mCellAnims[12]->SetAnimation(0x12);
    mCellAnims[13]->SetAnimation(0x14);
    mCellAnims[14]->SetAnimation(0x15);
    mCellAnims[15]->SetAnimation(0x15);
    ChangeDir("/");
    ChangeDir("/font/");
    mMessages = LoadFile("mes_chara_edit.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x20, 2);
    mWindowTextBufferSize = TextManager::Get().BufferSize(0x18, 8);
    u8 j = 0;
    do {
        mTextBuffers[j] = (u32 *) OS_Alloc(mTextBufferSize);
        MI_CpuClear16(mTextBuffers[j], mTextBufferSize);
        j++;
    } while (j < 6);
    mWindowTextBuffer = OS_Alloc(mWindowTextBufferSize);
    MI_CpuClear16(mWindowTextBuffer, mWindowTextBufferSize);
    mTextGlyphCount = TextManager::Get().GlyphCount(8);
    mWindowGlyphCount = TextManager::Get().GlyphCount(0x18);
    mTextGlyphBuffer = OS_Alloc(mTextGlyphCount * 0xc);
    mWindowGlyphBuffer = OS_Alloc(mWindowGlyphCount * 0xc);
    TextManager::Get().Init(mTextGlyphBuffer, 1, 0, 0x20, 2, 0x21);
    TextManager::Get().Init(mWindowGlyphBuffer, 1, 0, 0x18, 8, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    mMessageWindow = new MessageWindow("mes_chara_edit", 1, 0);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(0, 0, 1);
    ResetTexts();
    ChangeDir("/Story/Map/");
    NitroTexture *texture = new NitroTexture("cut_room.nsbtx");
    NitroModel *model     = new NitroModel();
    model->Load("cut_room.nsbmd", texture);
    texture->FreeFile();
    ModelAnimData *animData = new ModelAnimData("cut_room_data.bin");
    ModelAnim *anim         = new ModelAnim(animData, model);
    mCutRoomModel                = model->CreateInstance(anim);
    mCutRoomModel->StartAnimWithIncrement(0, 1, 0x1000);
    ChangeDir("/");
    mModel = new UnkStruct_0205401c();
    mModel->Create(0x15, 1, 0, 0);
    mModel->LoadShadow();
    mModel->mCharacter->mRotY = mModelAngle;
    mModel->func_0205461c(0, 1);
    ChangeDir("/");
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.SetObjVramMode(1);
    ChangeDir("/Game/2d/CharacterEditor/Obj");
    screen.LoadCharacters("nameplate_u_obj_32k.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("nameplate_u_obj_32k_ncl.bin", 0, 0);
    mNameplateCellAnim = new CellAnimation("nameplate_u_obj_32k_nce.bin", 0, 1, 1);
    mNameplateCellAnim->mCharBase = screen.GetCharBase(0);
    mNameplateCellAnim->mPaletteBase = 0;
    mNameplateCellAnim->SetAnimation(0);
    ChangeDir("/");
    mNameBufferSize = TextManager::Get().BufferSize(0x20, 2);
    mNameBuffer = OS_Alloc(mNameBufferSize);
    ClearName();
    mNameGlyphCount = TextManager::Get().GlyphCount(0x20, 2, 0x21);
    mNameGlyphBuffer = OS_Alloc(mNameGlyphCount * 0xc);
    TextManager::Get().Init(mNameGlyphBuffer, 0, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
}

void CharacterEditScene::LoadChoiceCells(const char *chars, const char *palette, const char *cells, const char *palette2) {
    u8 i = 0;
    do {
        CellAnimation *cell = mCellAnims[i];
        if (cell != NULL) {
            delete cell;
        }
        mCellAnims[i] = NULL;
        i++;
    } while (i <= 7);
    ChangeDir("/Game/2d/CharacterEditor/Obj");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.LoadCharacters(chars, 0x15000, 0);
    screen.LoadPalette(palette, 0x160, 0);
    u8 j = 0;
    do {
        if (mStyleAnimIndices == NULL) {
            mCellAnims[j] = new CellAnimation(cells, 0, 1, 1);
        } else {
            mCellAnims[j] = new CellAnimation(cells, mStyleAnimIndices[j], 1, 1);
        }
        mCellAnims[j]->mCharBase = screen.GetCharBase(0x15000);
        mCellAnims[j]->mPaletteBase = 0xb;
        j++;
    } while (j < mStyleCount);
    screen.LoadPalette(palette2, 0x120, 0);
    if (mState == 7) {
        screen.LoadPalette("color_hair_male_ncl.bin", 0x140, 0);
    } else if (mState == 9) {
        screen.LoadPalette("color_glasses_len_ncl.bin", 0x140, 0);
    }
    ChangeDir("/");
    mChoicesVisible = 1;
}

void CharacterEditScene::Draw() {
    DrawTexts();
    mWindows[0]->Draw();
    mWindows[1]->Draw();
    mWindows[2]->Draw();
    DrawChoices();
    if (mCutRoomModel != NULL) {
        MtxFx43 mtx = gIdentityMtx43;
        VecFx32 b;
        VecFx32 a;
        b.x = 0x1000;
        b.y = 0x1000;
        b.z = 0x1000;
        a.x = 0;
        a.y = -0x6000;
        a.z = 0;
        MTX_SetTranslation43(&mtx, &a);
        mCutRoomModel->AdvanceAnim();
        mCutRoomModel->Draw(&mtx, &b);
    }
    if (mModel != NULL) {
        mModel->Draw();
    }
    DrawName();
    if (mNameplateCellAnim != NULL) {
        mNameplateCellAnim->Update();
        mNameplateCellAnim->DrawFrame(0, gSaveData[0].mAttribute, 0x80, 0x60, 0);
    }
}

BOOL CharacterEditScene::EnableSelectedGroupButtons() {
    BOOL result = FALSE;
    if (mWindows[1]->GetSelectedId() == 1) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 1;
        mWindows[1]->mButtons[2].mEnabled = 0;
        mWindows[1]->mButtons[3].mEnabled = 0;
        mWindows[1]->mButtons[4].mEnabled = 0;
        mWindows[1]->mButtons[5].mEnabled = 0;
        mWindows[1]->mButtons[6].mEnabled = 0;
    } else if (mWindows[1]->GetSelectedId() == 3) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 0;
        mWindows[1]->mButtons[2].mEnabled = 1;
        mWindows[1]->mButtons[3].mEnabled = 1;
        mWindows[1]->mButtons[4].mEnabled = 1;
        mWindows[1]->mButtons[5].mEnabled = 0;
        mWindows[1]->mButtons[6].mEnabled = 0;
    } else if (mWindows[1]->GetSelectedId() == 2) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 0;
        mWindows[1]->mButtons[2].mEnabled = 1;
        mWindows[1]->mButtons[3].mEnabled = 1;
        mWindows[1]->mButtons[4].mEnabled = 1;
        mWindows[1]->mButtons[5].mEnabled = 0;
        mWindows[1]->mButtons[6].mEnabled = 0;
    } else if (mWindows[1]->GetSelectedId() == 4) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 0;
        mWindows[1]->mButtons[2].mEnabled = 1;
        mWindows[1]->mButtons[3].mEnabled = 1;
        mWindows[1]->mButtons[4].mEnabled = 1;
        mWindows[1]->mButtons[5].mEnabled = 0;
        mWindows[1]->mButtons[6].mEnabled = 0;
    } else if (mWindows[1]->GetSelectedId() == 6) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 0;
        mWindows[1]->mButtons[2].mEnabled = 0;
        mWindows[1]->mButtons[3].mEnabled = 0;
        mWindows[1]->mButtons[4].mEnabled = 0;
        mWindows[1]->mButtons[5].mEnabled = 1;
        mWindows[1]->mButtons[6].mEnabled = 1;
    } else if (mWindows[1]->GetSelectedId() == 5) {
        result = TRUE;
        mWindows[1]->mButtons[1].mEnabled = 0;
        mWindows[1]->mButtons[2].mEnabled = 0;
        mWindows[1]->mButtons[3].mEnabled = 0;
        mWindows[1]->mButtons[4].mEnabled = 0;
        mWindows[1]->mButtons[5].mEnabled = 1;
        mWindows[1]->mButtons[6].mEnabled = 1;
    }
    return result;
}

void CharacterEditScene::StoreLook(CharacterLook *look) {
    if (look == NULL) {
        look = (CharacterLook *) gCharacterLook;
    }
    look->mUnk_06_0 = mLists[0].mValues[mLists[0].mIndex];
    look->mUnk_00_1 = mLists[1].mValues[mLists[1].mIndex];
    look->mUnk_00_4 = mLists[2].mValues[mLists[2].mIndex];
    look->mUnk_00_7 = mLists[3].mValues[mLists[3].mIndex];
    look->mUnk_00_10 = mLists[4].mValues[mLists[4].mIndex];
    look->mUnk_00_13 = mLists[5].mValues[mLists[5].mIndex];
    look->mUnk_02_0 = mLists[6].mValues[mLists[6].mIndex];
    look->mUnk_02_3 = mLists[7].mValues[mLists[7].mIndex];
    look->mUnk_02_6 = mLists[8].mValues[mLists[8].mIndex];
    look->mUnk_02_9 = mLists[9].mValues[mLists[9].mIndex];
    look->mUnk_02_12 = mLists[10].mValues[mLists[10].mIndex];
    look->mUnk_04_0 = mLists[11].mValues[mLists[11].mIndex];
    look->mUnk_04_3 = mLists[12].mValues[mLists[12].mIndex];
    look->mUnk_04_8 = mLists[13].mValues[mLists[13].mIndex];
    look->mUnk_04_14 = mLists[14].mValues[mLists[14].mIndex];
    look->mUnk_04_11 = mLists[15].mValues[mLists[15].mIndex];
    look->mUnk_06_8 = gSaveData[0].mAttribute;
}

void CharacterEditScene::func_020637a8() {
    u8 i = 0;
    do {
        SelectWindow *w = mWindows[1];
        w->mCellPos[i].x = 0x80;
        w->mCellPos[i].y = 0x60;
        i++;
    } while (i < 7);
}

void CharacterEditScene::EnableGroupButtons() {
    mWindows[1]->mButtons[0].mEnabled = 1;
    u8 i = 1;
    do {
        mWindows[1]->mButtons[i].mEnabled = 0;
        i++;
    } while (i < 7);
    switch (mState) {
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
        mWindows[1]->mButtons[1].mEnabled = 1;
        break;
    case 4:
        mWindows[1]->mButtons[2].mEnabled = 1;
        mWindows[1]->mButtons[3].mEnabled = 1;
        mWindows[1]->mButtons[4].mEnabled = 1;
        break;
    case 5:
    case 10:
    case 11:
        mWindows[1]->mButtons[5].mEnabled = 1;
        mWindows[1]->mButtons[6].mEnabled = 1;
        break;
    case 12:
        mWindows[1]->mButtons[0].mEnabled = 0;
        break;
    }
}

void CharacterEditScene::func_020638a8(u32 v) {
    mWindows[0]->mButtons[13].mEnabled = v;
    mChoicesVisible = v;
}

void CharacterEditScene::UpdateRotateInput() {
    if (mRotateState != 3) {
        mRotateState = 0;
    }
    SelectWindow *w = mWindows[0];
    SelectWindowCursor *cur = w->mCursor;
    s8 id = -1;
    if (cur->mUnk_05 != -1) {
        id = cur->mItems[cur->mUnk_05].mId;
    }
    if (id != -1) {
        switch (id) {
        case 9:
            mRotateState = 1;
            break;
        case 10:
            mRotateState = 2;
            break;
        }
        return;
    }
    w->mCellAnims[9]->SetAnimation(0xc);
    mWindows[0]->mCellAnims[10]->SetAnimation(0xe);
    if (InputManager::Get().IsDown(0x200)) {
        mRotateState = 1;
        mWindows[0]->mCellAnims[9]->SetAnimation(0xd);
        return;
    }
    if (InputManager::Get().IsDown(0x100)) {
        mRotateState = 2;
        mWindows[0]->mCellAnims[10]->SetAnimation(0xf);
        return;
    }
    if (mWindow0Result == 3) {
        SelectWindowCursor *c = mWindows[0]->mCursor;
        s8 id2 = -1;
        if (c->mSelection != -1) {
            id2 = c->mItems[c->mSelection].mId;
        }
        if (id2 == 0xb) {
            mRotateState = 3;
        }
    }
}

void CharacterEditScene::SetItemFlags(u32 i, u32 v) {
    mWindows[0]->mCursor->mItems[i].mFlags = v;
}

void CharacterEditScene::SetItemSlide(u32 i, u32 a, u32 b) {
    WindowItem *item = &mWindows[0]->mCursor->mItems[i];
    item->mSlideIn = a;
    item->mSlideOut = b;
}

void CharacterEditScene::SetChoiceItemNeighbors(u32 i, u32 a, u32 b, u32 c, u32 d) {
    WindowItem *item = &mCursor->mItems[i];
    item->mNeighbors[0] = a;
    item->mNeighbors[1] = b;
    item->mNeighbors[2] = *(u8 *) &c;
    item->mNeighbors[3] = *(u8 *) &d;
}

void CharacterEditScene::MoveMenuDepth(u32 v) {
    mMenuDepthMove = v;
    switch (v) {
    case 0:
        break;
    case 1:
        mMenuDepth++;
        break;
    case 2:
        mMenuDepth--;
        break;
    }
}

void CharacterEditScene::func_02063b10(u32 arg) {
    u8 count;
    switch (mState) {
    case 1:
        count = 5;
        break;
    case 3:
        count = 5;
        break;
    case 5:
        count = 3;
        break;
    case 2:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        count = 1;
        break;
    default:
        return;
    }
    u8 i = 0;
    do {
        if (i == 4) {
            switch (mFromMyroom) {
            case 0:
                SetItemFlags(i, 1);
                SetItemSlide(i, 3, 4);
                break;
            case 1:
                SetItemFlags(i, 0x8000);
                SetItemSlide(i, 0, 0);
                break;
            }
        } else {
            SetItemSlide(i, 3, 4);
        }
        i++;
    } while (i < count);
    if (arg == 0) {
        u8 j = 2;
        do {
            mWindows[0]->mButtons[j].mEnabled = 0;
            j++;
        } while (j <= 7);
    }
}

void CharacterEditScene::SetupSlideIn() {
    mFixedItem = -1;
    WindowItem *items = mWindows[0]->mCursor->mItems;
    if (mMenuDepthMove == 1) {
        items[0].mSlideIn = 0;
        mWindows[0]->mButtons[items[0].mId].mEnabled = 1;
        mFixedItem = 0;
    } else if (mMenuDepthMove == 2) {
        items[mMenuSelection[mMenuDepth]].mSlideIn = 0;
        mWindows[0]->mButtons[items[mMenuSelection[mMenuDepth]].mId].mEnabled = 1;
        mFixedItem = mMenuSelection[mMenuDepth];
    }
}

void CharacterEditScene::SlideToParent() {
    WindowItem *items = mWindows[0]->mCursor->mItems;
    items[0].mSlideOut = 2;
    if (mMenuDepth == 2) {
        u8 id = items[0].mId;
        SelectWindow *w = mWindows[0];
        w->mCellPos[id].x = 0x8c;
        w->mCellPos[id].y = 0x1a;
    }
    mWindows[0]->SlideCellTo(items[0].mId, 0x87, mMenuSelection[mMenuDepth - 1] * 0x1c + 0x1a);
    mWindows[0]->mCellAnims[(u8) items[0].mId]->SetAnimation(3);
}

void CharacterEditScene::SlideToFirstRow() {
    SelectWindowCursor *cur = mWindows[0]->mCursor;
    s8 sel = cur->mSelection;
    WindowItem *items = cur->mItems;
    if (items[sel].mId == 8) {
        return;
    }
    items[sel].mSlideOut = 1;
    mWindows[0]->SlideCellTo(items[sel].mId, 0x87, 0x1a);
    mWindows[0]->mCellAnims[(u8) items[sel].mId]->SetAnimation(3);
}

BOOL CharacterEditScene::SwitchToSelectedGroup() {
    if (mWindows[1]->GetSelectedId() == 1) {
        mState = 3;
        mStep = 0x16;
        mMenuDepth = 0;
        mMenuSelection[mMenuDepth] = 2;
        if (mFromMyroom == 1) {
            mMenuSelection[mMenuDepth] = 1;
        }
        mMenuDepth++;
        MoveMenuDepth(0);
        return TRUE;
    }
    if (mWindows[1]->GetSelectedId() == 2 || mWindows[1]->GetSelectedId() == 3 || mWindows[1]->GetSelectedId() == 4) {
        mState = 4;
        mStep = 0x38;
        mMenuDepth = 0;
        mMenuSelection[mMenuDepth] = 3;
        if (mFromMyroom == 1) {
            mMenuSelection[mMenuDepth] = 2;
        }
        mMenuDepth++;
        MoveMenuDepth(0);
        return TRUE;
    }
    if (mWindows[1]->GetSelectedId() == 5 || mWindows[1]->GetSelectedId() == 6) {
        mState = 5;
        mStep = 0x3f;
        mMenuDepth = 0;
        mMenuSelection[mMenuDepth] = 4;
        if (mFromMyroom == 1) {
            mMenuSelection[mMenuDepth] = 3;
        }
        mMenuDepth++;
        MoveMenuDepth(0);
        return TRUE;
    }
    return FALSE;
}


void CharacterEditScene::InitLookLists() {
    u8 count = 0;
    u8 value = 0;
    u8 i = 0;
    do {
        switch (i) {
        case 0:
            count = 5;
            value = SAVE_LOOK->mUnk_06_0;
            break;
        case 1:
            count = 4;
            value = SAVE_LOOK->mUnk_00_1;
            break;
        case 2:
            count = 6;
            value = SAVE_LOOK->mUnk_00_4;
            break;
        case 3:
            count = 8;
            value = SAVE_LOOK->mUnk_00_7;
            break;
        case 4:
            count = 6;
            value = SAVE_LOOK->mUnk_00_10;
            break;
        case 5:
            count = 6;
            value = SAVE_LOOK->mUnk_00_13;
            break;
        case 6:
            count = 6;
            value = SAVE_LOOK->mUnk_02_0;
            break;
        case 7:
            count = 5;
            value = SAVE_LOOK->mUnk_02_3;
            break;
        case 8:
            count = 4;
            value = SAVE_LOOK->mUnk_02_6;
            break;
        case 9:
            count = 5;
            value = SAVE_LOOK->mUnk_02_9;
            break;
        case 10:
            count = 6;
            value = SAVE_LOOK->mUnk_02_12;
            break;
        case 11:
            count = 6;
            value = SAVE_LOOK->mUnk_04_0;
            break;
        case 12:
            count = 6;
            value = SAVE_LOOK->mUnk_04_3;
            break;
        case 13:
            count = 6;
            value = SAVE_LOOK->mUnk_04_8;
            break;
        case 14:
            count = 3;
            value = SAVE_LOOK->mUnk_04_14;
            break;
        case 15:
            count = 6;
            value = SAVE_LOOK->mUnk_04_11;
            break;
        }
        mLists[i].mCount = 0;
        u8 j = 0;
        s32 mask = 0xff;
        do {
            if ((mask >> j) & 1) {
                mLists[i].mValues[mLists[i].mCount] = j;
                mLists[i].mCount++;
                if (j == value) {
                    mLists[i].mIndex = j;
                }
            }
            j++;
        } while (j < count && mLists[i].mCount < 8);
        i++;
    } while (i < 16);
}

void CharacterEditScene::SetupChoices() {
    mColorCount = 0;
    mColor2Count = 0;
    SetChoiceItemNeighbors(10, 7, 0x10, 9, 0xb);
    SetChoiceItemNeighbors(11, 7, 0x11, 10, 0xc);
    SetChoiceItemNeighbors(12, 5, 0x12, 11, 0xd);
    switch (mState) {
    case 2:
        mStyleIndex = 0;
        mStyleCount = 1;
        mStyleAnimIndices = NULL;
        mColorIndex = mLists[0].mIndex;
        mColorCount = mLists[0].mCount;
        SetChoiceItemNeighbors(10, 0, 0, 9, 0xb);
        SetChoiceItemNeighbors(11, 0, 0, 10, 0xc);
        SetChoiceItemNeighbors(12, 0, 0, 11, 0xd);
        break;
    case 6:
        mStyleIndex = mLists[1].mIndex;
        mStyleCount = mLists[1].mCount;
        mStyleAnimIndices = mLists[1].mValues;
        mColorIndex = mLists[2].mIndex;
        mColorCount = mLists[2].mCount;
        break;
    case 7:
        mStyleIndex = mLists[3].mIndex;
        mStyleCount = mLists[3].mCount;
        mStyleAnimIndices = mLists[3].mValues;
        mColor2Index = mLists[4].mIndex;
        mColor2Count = mLists[4].mCount;
        break;
    case 8:
        mStyleIndex = mLists[5].mIndex;
        mStyleCount = mLists[5].mCount;
        mStyleAnimIndices = mLists[5].mValues;
        mColorIndex = mLists[6].mIndex;
        mColorCount = mLists[6].mCount;
        break;
    case 9:
        mStyleIndex = mLists[7].mIndex;
        mStyleCount = mLists[7].mCount;
        mStyleAnimIndices = mLists[7].mValues;
        mColorIndex = mLists[8].mIndex;
        mColorCount = mLists[8].mCount;
        mColor2Index = mLists[9].mIndex;
        mColor2Count = mLists[9].mCount;
        break;
    case 4:
        mStyleIndex = mLists[10].mIndex;
        mStyleCount = mLists[10].mCount;
        mStyleAnimIndices = mLists[10].mValues;
        mColorIndex = mLists[11].mIndex;
        mColorCount = mLists[11].mCount;
        break;
    case 10:
        mStyleIndex = mLists[12].mIndex;
        mStyleCount = mLists[12].mCount;
        mStyleAnimIndices = mLists[12].mValues;
        mColorIndex = mLists[13].mIndex;
        mColorCount = mLists[13].mCount;
        break;
    case 11:
        mStyleIndex = mLists[14].mIndex;
        mStyleCount = mLists[14].mCount;
        mStyleAnimIndices = mLists[14].mValues;
        mColorIndex = mLists[15].mIndex;
        mColorCount = mLists[15].mCount;
        break;
    }
    u8 selected = mStyleIndex;
    mCursorItem = selected;
    mCursor->mSelection = selected;
    mCellAnims[12]->SetAnimation(0x12);
    WindowItem *items = mCursor->mItems;
    u8 i = 0;
    do {
        if (i < mStyleCount) {
            items[i].mFlags = 8;
        } else {
            items[i].mFlags = 0x8000;
        }
        i++;
    } while (i < 8);
    i = 0;
    do {
        if (i < mColorCount) {
            items[i + 8].mFlags = 8;
        } else {
            items[i + 8].mFlags = 0x8000;
        }
        if (i < mColor2Count) {
            items[i + 14].mFlags = 8;
        } else {
            items[i + 14].mFlags = 0x8000;
        }
        i++;
    } while (i < 6);
}

void CharacterEditScene::DrawChoices() {
    if (mChoicesVisible == 0) {
        return;
    }
    u8 i = 0;
    do {
        if (mCellAnims[i] != NULL) {
            mCellAnims[i]->Update();
            mCellAnims[i]->Draw(1, sCharacterEditCellPositions[i].x, sCharacterEditCellPositions[i].y, 1);
        }
        i++;
    } while (i < mStyleCount);
    u8 j = 8;
    do {
        if (mCellAnims[j] != NULL) {
            mCellAnims[j]->Update();
            switch (j - 8) {
            case 0:
                if (mColorCount != 0) {
                    mCellAnims[j]->DrawFrame(1, mColorCount - 1, sCharacterEditCellPositions[j].x, sCharacterEditCellPositions[j].y, 1);
                }
                break;
            case 1:
                if (mColor2Count != 0) {
                    mCellAnims[j]->DrawFrame(1, mColor2Count - 1, sCharacterEditCellPositions[j].x, sCharacterEditCellPositions[j].y, 1);
                }
                break;
            case 2:
            case 3:
                if (mState == 9) {
                    mCellAnims[j]->Draw(1, sCharacterEditCellPositions[j].x, sCharacterEditCellPositions[j].y, 1);
                }
                break;
            case 4: {
                u8 v = mCursorItem;
                if (v < 8) {
                    mCellAnims[j]->Draw(1, (s16) (v % 3 * 32 + 0x91), (s16) (v / 3 * 32 + 0x34), 0);
                } else if (v < 0xe) {
                    mCellAnims[j]->Draw(1, (s16) ((v - 8) * 15 + 0x90), 0x86, 0);
                } else {
                    mCellAnims[j]->Draw(1, (s16) ((v - 0xe) * 15 + 0x90), 0x96, 0);
                }
                break;
            }
            case 5: {
                u8 v = mStyleIndex;
                mCellAnims[j]->Draw(1, (s16) (sCharacterEditCellPositions[j].x + v % 3 * 32), (s16) (sCharacterEditCellPositions[j].y + v / 3 * 32), 0);
                break;
            }
            case 6:
                if (mColorCount != 0) {
                    mCellAnims[j]->Draw(1, (s16) (sCharacterEditCellPositions[j].x + mColorIndex * 15), sCharacterEditCellPositions[j].y, 0);
                }
                break;
            case 7:
                if (mColor2Count != 0) {
                    mCellAnims[j]->Draw(1, (s16) (sCharacterEditCellPositions[j].x + mColor2Index * 15), sCharacterEditCellPositions[j].y, 0);
                }
                break;
            }
        }
        j++;
    } while (j < 16);
}

void CharacterEditScene::UpdateChoiceInput() {
    if (mWindows[0]->mState != 1) {
        return;
    }
    u32 result = mCursor->Update(1);
    s8 sel = mCursor->mSelection;
    if (mCursorItem != sel) {
        mCursorItem = sel;
        if (mCursorItem < 8) {
            mCellAnims[12]->SetAnimation(0x12);
        } else {
            mCellAnims[12]->SetAnimation(0x13);
        }
        return;
    }
    if (result == 3) {
        ApplyCursorItem();
    }
}

void CharacterEditScene::ApplyCursorItem() {
    u8 v = mCursorItem;
    if (v < 8) {
        mStyleIndex = v;
    } else if (v < 0xe) {
        mColorIndex = v - 8;
    } else {
        mColor2Index = v - 0xe;
    }
    switch (mState) {
    case 2:
        if (mCursorItem >= 8) {
            mLists[0].mIndex = mColorIndex;
        }
        break;
    case 6:
        if (mCursorItem < 8) {
            mLists[1].mIndex = mStyleIndex;
        } else {
            mLists[2].mIndex = mColorIndex;
        }
        break;
    case 7:
        if (mCursorItem >= 8) {
            mLists[4].mIndex = mColor2Index;
        } else {
            mLists[3].mIndex = mStyleIndex;
            mLists[5].mIndex = 0;
        }
        break;
    case 8:
        if (mCursorItem < 8) {
            mLists[5].mIndex = mStyleIndex;
        } else {
            mLists[6].mIndex = mColorIndex;
        }
        break;
    case 9:
        if (mCursorItem < 8) {
            mLists[7].mIndex = mStyleIndex;
        } else if (mCursorItem < 0xe) {
            mLists[8].mIndex = mColorIndex;
        } else {
            mLists[9].mIndex = mColor2Index;
        }
        break;
    case 4:
        if (mCursorItem < 8) {
            mLists[10].mIndex = mStyleIndex;
        } else {
            mLists[11].mIndex = mColorIndex;
        }
        break;
    case 10:
        if (mCursorItem < 8) {
            mLists[12].mIndex = mStyleIndex;
        } else {
            mLists[13].mIndex = mColorIndex;
        }
        break;
    case 11:
        if (mCursorItem < 8) {
            mLists[14].mIndex = mStyleIndex;
        } else {
            mLists[15].mIndex = mColorIndex;
        }
        break;
    }
    ApplyLookToModel();
    mRebuildModel = 1;
    mChoiceApplied = 1;
}

void CharacterEditScene::ApplyLookToModel() {
    u8 i = 0;
    do {
        mModel->mCharacter->mUnk_65[i] = mLists[i].mValues[mLists[i].mIndex];
        i++;
    } while (i < 16);
    mModel->mCharacter->mUnk_75 = gSaveData[0].mAttribute;
}

void CharacterEditScene::RandomizeLook() {
    u8 i = 0;
    if (mFromMyroom == 1) {
        i = 3;
    }
    do {
        mLists[i].mIndex = MATH_Rand32(&gRandom, mLists[i].mCount);
        i++;
    } while (i < 16);
    if (MATH_Rand32(&gRandom, 2) != 0) {
        mLists[7].mIndex = 0;
    }
    if (MATH_Rand32(&gRandom, 2) != 0) {
        mLists[5].mIndex = 0;
    }
}

void CharacterEditScene::func_02064cd4(u32 flag) {
    u8 i = 0;
    do {
        if (flag != 0) {
            SetItemSlide(i, 0, 0);
        } else if (i <= 4) {
            SetItemSlide(i, 3, 4);
            if (mFromMyroom == 1 && i == 4) {
                SetItemSlide(i, 0, 0);
            }
        } else {
            SetItemSlide(i, 1, 2);
        }
        i++;
    } while (i < 10);
}

void CharacterEditScene::PrintWindowText() {
    TextManager &text = TextManager::Get();
    s16 a;
    s16 b;
    b = 0;
    a = 0;
    MessageFile *file = (MessageFile *) mMessages;
    text.Print(file->GetText(0x1c), file->GetLength(0x1c), mWindowTextBuffer, 0x18, 8, &a, &b, NULL, 0, 2, 0x21, 0x10, 0, 0,
               1, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x1000, mWindowTextBuffer, mWindowTextBufferSize, 0);
    mWindowTextVisible = 1;
}

void CharacterEditScene::DrawName() {
    SpriteManager::Get().Screen(0).AddOam(mNameGlyphBuffer, (u8) mNameGlyphCount, 0x16, 0xaa, 0, 0x100, 0xf, 0);
}

void CharacterEditScene::ClearName() {
    MI_CpuClear16(mNameBuffer, mNameBufferSize);
}

void CharacterEditScene::PrintName() {
    s16 a = 0;
    s16 b = 0;
    TextManager &text = TextManager::Get();
    text.mUse4bppFont = 1;
    TextManager::Get().Print(gSaveData[0].mName, 0x18, mNameBuffer, 0x20, 2, &a, &b, NULL, 0, 2, 0x21, 0, 0, 0, 0, 1,
                             1);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x2000, mNameBuffer, mNameBufferSize, 0);
}
