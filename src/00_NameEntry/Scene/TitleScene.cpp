#include "System/SaveManager.hpp"
#include "Scene/TitleScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "System/SoundManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include <nitro/gx.h>
#include "System/FileSystem.hpp"
#include <stdio.h>
#include "System/FrameTask.hpp"
#include "Text/TextManager.hpp"
#include <nitro/fx.h>
#include "System/FrameUpdate.hpp"
#include "System/SaveManager.hpp"
#include "UI/SaveWindow.hpp"
#include "Scene/NameEntryScene.hpp"
#include "Scene/AttrSelScene.hpp"
#include "UI/MessageScreen.hpp"
#include "Game/SaveData.hpp"


void operator delete(void *ptr);

static const WindowCellRecord sTitleTwoButtonCells[2] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 1, 0x1b000, 0, 0, 1, {0, 2, 1, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 9, 0x1b000, 0, 0, 1, {9, 9, 9, 0}},
};
static const WindowCellRecord sTitleThreeButtonCells[3] = {
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 3, 0x1b000, 0, 0, 1, {3, 5, 4, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 7, 0x1b000, 0, 0, 1, {6, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_window_nce.bin", 10, 0x1b000, 0, 0, 1, {10, 10, 10, 0}},
};
static const WindowItemRecord sTitleObj01Items[2][2] = {
    {
        {0, 0x1, 24, 84, 208, 24, 0, {0, 0, 0, 0}, 0, 1},
        {0, 0x0, 0, 0, 0, 0, 0, {0, 0, 0, 0}, 0, 0},
    },
    {
        {0, 0x1, 24, 104, 208, 24, 0, {1, 1, 0, 0}, 0, 2},
        {1, 0x1, 24, 64, 208, 24, 0, {0, 0, 1, 1}, 0, 2},
    },
};
static const WindowItemRecord sTitleFullScreenItems[1] = {
    {0, 0x1, 0, 0, 255, 255, 0, {0, 0, 0, 0}, 0, 1},
};
// Record 1 (flag 0x8000) is skipped by the cursor and by touch: one item is selectable.
static const WindowItemRecord sTitleTwoButtonItems[2] = {
    {0, 0x1, 89, 152, 78, 20, 0, {0, 0, 0, 0}, 0, 1},
    {1, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};
static const WindowCellRecord sTitleObj01Cells[2][2] = {
    {
        {"/Game/Title/title_b_obj_01_nce.bin", 7, 0x13000, 1, 0, 1, {6, 8, 7, 0}},
        {NULL, 0, 0x0, 0, 0, 0, {0, 0, 0, 0}},
    },
    {
        {"/Game/Title/title_b_obj_01_nce.bin", 0, 0x13000, 1, 0, 1, {0, 2, 1, 0}},
        {"/Game/Title/title_b_obj_01_nce.bin", 3, 0x13000, 1, 0, 1, {3, 5, 4, 0}},
    },
};
static const WindowItemRecord sTitleMenuItems[5] = {
    {0, 0x1, 2, 35, 148, 25, 0, {4, 1, 0, 0}, 0, 2},
    {1, 0x1, 14, 63, 121, 25, 0, {0, 2, 1, 1}, 0, 2},
    {2, 0x1, 4, 90, 127, 25, 0, {1, 3, 2, 2}, 0, 2},
    {3, 0x1, 14, 118, 127, 25, 0, {2, 4, 3, 3}, 0, 2},
    {4, 0x1, 23, 147, 145, 25, 0, {3, 0, 4, 4}, 0, 2},
};
// Record 2 (flag 0x8000) is skipped by the cursor and by touch: two items are selectable.
static const WindowItemRecord sTitleThreeButtonItems[3] = {
    {0, 0x1, 45, 152, 78, 20, 0, {0, 0, 1, 1}, 0, 2},
    {1, 0x1, 133, 152, 78, 20, 0, {1, 1, 0, 0}, 0, 2},
    {2, 0x8000, 120, 18, 78, 20, 0, {-1, -1, -1, -1}, 0, 0},
};


s32 GetSlotBakuganId(SaveData *base, u32 unk1, u32 unk2);
void SetSlotBakugan(SaveData *base, u32 a, u32 b, u32 c, u32 d, u32 e);
void SetBakuganEntry(SaveData *base, const u16 *src, u32 b, u32 c);
void ApplyVolumeOptions(SaveData *base);
void ResetSaveData();
void func_02022b0c();
s32 UnlockEmblem();
void func_02086d48();
void FillParkSlots();

class MovieScene {
public:
    u8 mPad[0x14];
    MovieScene(void *data, u32 value);
};
class CharacterEditScene {
public:
    u8 mPad[0x1e8];
    CharacterEditScene(u32 unk);
};
class OptionScene {
public:
    u8 mPad[0x50];
    OptionScene();
};
class CreditsScene {
public:
    u8 mPad[0x34];
    CreditsScene();
};
class GiftUnlockScene {
public:
    u8 mPad[0x24];
    GiftUnlockScene(s32 unk);
};

// Loads the save data (vtable 0x020a5494).
class SaveReader : public SceneBase {
public:
    SaveReader() :
        SceneBase(0xffdc, 0) {
        ((u8 *) mContext)[0x23] = 0;
    }
    virtual ~SaveReader();
    virtual void Update();
};

TitleScene::TitleScene() :
    SceneBase(1000, 0) {
    mCursor = NULL;
    mWindow30 = NULL;
    mMessageWindow = NULL;
    mBuf38 = NULL;
    mBuf3c = NULL;
    mBuf40 = NULL;
    mBuf44 = NULL;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mUnk_c8 = 0;
    mState = 0;
    mTimer = 0;
    mMenuVisible = 0;
    mMessageIndex = 0xff;
    mUnk_d0 = 0;
    mBlendAmplitude = 0x10;
    mCursorStep = 0;
    mUnk_d6 = 0;
    mLogoVisible = 0;
    mLogoY = 0x60;
    data_020d433c.mNextScene = SCENE_TITLE;
    data_020d433c.mUnk_7d = 0;
    data_020d433c.mUnk_87 = 0;
    data_020d433c.mUnk_88_4 = 0;
    data_020d433c.mUnk_88_0 = 0;
    data_020d433c.mMapTab = 0;
    data_020d433c.mLastBattle = 0;
    MI_CpuClear32(mCellAnims, sizeof(mCellAnims));
    MI_CpuClear32(mBuffers, sizeof(mBuffers));
    MI_CpuClear32(mBufferSizes, sizeof(mBufferSizes));
    mCursor = new SelectWindowCursor(1, sTitleFullScreenItems, 0);
    mMenuSelection = data_020d433c.mTitleMenuSelection;
    mSubBgIndex = mMenuSelection;
    SoundManager::Get().RestoreHeapState();
    SoundManager::Get().LoadSoundGroup(4);
}

TitleScene::~TitleScene() {
    for (u8 i = 0; i < 8; i++) {
        if (mCellAnims[i] != NULL) {
            delete mCellAnims[i];
        }
    }
    operator delete(mCursor);
    delete mWindow30;
    delete mMessageWindow;
    if (mBuf38 != NULL) {
        OS_Free(mBuf38);
    }
    if (mBuf3c != NULL) {
        OS_Free(mBuf3c);
    }
    if (mBuf40 != NULL) {
        OS_Free(mBuf40);
    }
    if (mBuf44 != NULL) {
        OS_Free(mBuf44);
    }
    for (u8 i = 0; i < 5; i++) {
        for (u8 j = 0; j < 3; j++) {
            if (mBuffers[i][j] != NULL) {
                OS_Free(mBuffers[i][j]);
            }
        }
    }
}

void TitleScene::Update() {
    switch (mState) {
        case 0:
            if (data_020d433c.mCreditsPending) {
                mPlaneBlendEnabled = 0;
                mState = 0x2a;
                break;
            }
            if (data_020d433c.mUnk_80) {
                mState = 3;
                break;
            }
            FrameTask::Get().CloseHandles();
            mState++;
            break;
        case 1:
            new MovieScene((void *) "/movie/bknd_op.mods", data_020d433c.mLanguage);
            mState++;
            break;
        case 2:
            if (mContext->mRequest == 1) {
                FrameTask::Get().OpenHandles();
                mState++;
            }
            break;
        case 3:
            InitDisplay();
            LoadGraphics();
            LoadSystemText();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            if (data_020d433c.mUnk_76) {
                FrameTask::Get().StartChannel(0, 0x10);
                mLogoVisible = 1;
            } else {
                FrameTask::Get().StartChannel(0, 0x17);
            }
            mState++;
            break;
        case 4:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            SetPlaneBlend(1);
            mState++;
            break;
        case 5:
            if (IsFadeDone(0)) {
                mUnk_c8 = 0;
                if (data_020d433c.mUnk_76) {
                    mState = 0x20;
                    break;
                }
                mState++;
            }
            break;
        case 6:
            func_ov000_022245e8();
            mUnk_c8++;
            if (mUnk_c8 > 0xe10) {
                mUnk_c8 = 0;
                mState = 7;
                break;
            }
            if (mCursor->Update(1) == 1) {
                mTimer = 0;
                mState = 9;
            }
            break;
        case 7:
            func_ov000_022245e8();
            if (IsFadeDone(0)) {
                SetPlaneBlend(0);
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState++;
            }
            break;
        case 8:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            FrameTask::Get().CloseHandles();
            mState = 1;
            return;
        case 9: {
            ResetSaveData();
            u8 a = data_020d433c.mUnk_81;
            u8 b = data_020d433c.mLanguage;
            u8 c = data_020d433c.mSaveSlotContent;
            func_02022b0c();
            data_020d433c.mUnk_81 = a;
            data_020d433c.mLanguage = b;
            data_020d433c.mSaveSlotContent = c;
            GXS_SetVisiblePlane(0x11);
            u32 idx = 1;
            u8 count = 2;
            switch (data_020d433c.mSaveSlotContent) {
                case 0:
                    data_020d433c.mSaveSlotContent = idx;
                    if (SaveManager::Get().WriteSlotMarker(0) == SAVE_STATUS_ERROR) {
                        new MessageScreen(MESSAGE_SCREEN_SYSTEM_4_HALT);
                    }
                case 1:
                    count--;
                    idx = 0;
                    break;
            }
            delete mWindow30;
            mWindow30 = new SelectWindow(count, sTitleObj01Items[idx], -1);
            mWindow30->SetCells(count, sTitleObj01Cells[idx]);
            mWindow30->mCursor->mSelection = -1;
            if (idx == 0) {
                mWindow30->mCursor->mSelection = 0;
                SetMessage(0);
            } else if (mUnk_d0) {
                mWindow30->mCursor->mSelection = 0;
                mWindow30->mCellAnims[0]->SetAnimation(1);
            } else {
                mWindow30->mCursor->mSelection = 1;
                mWindow30->mCellAnims[1]->SetAnimation(4);
            }
            mState++;
            break;
        }
        case 10: {
            u8 result = mWindow30->Update(1);
            u8 sel = mWindow30->mCursor->mSelection;
            if (result == 1) {
                if (sel == 0) {
                    switch (data_020d433c.mSaveSlotContent) {
                        case 0:
                        case 1:
                            mState = 0x13;
                            break;
                        default:
                            mState = 0xd;
                            break;
                    }
                } else {
                    FrameTask::Get().StopAllChannels();
                    mState++;
                }
            }
            mWindow30->Draw();
            break;
        }
        case 11:
            if (func_0203dd0c(&FrameTask::Get(), 0) != 0) {
                break;
            }
            mContext->mRequest = 0;
            SaveManager::Get();
            new SaveReader();
            mState++;
            break;
        case 12:
            switch (mContext->mRequest) {
                case SAVE_STATUS_ERROR:
                case SAVE_STATUS_NO_MAGIC:
                case SAVE_STATUS_MARKER:
                    new MessageScreen(MESSAGE_SCREEN_SYSTEM_5_HALT);
                    break;
                case SAVE_STATUS_CORRUPT:
                    mState = 0xf;
                    break;
                case SAVE_STATUS_OK: {
                    u8 buf[3] = {0, 1, 4};
                    u8 changed = 0;
                    u8 i;
                    for (i = 0; i < 3; i++) {
                        if (GetSlotBakuganId(gSaveData, 0, i) == 0x28) {
                            u8 v = buf[i];
                            SetSlotBakugan(gSaveData, 0, i, 0, 0x22, v);
                            changed = 1;
                        }
                    }
                    for (i = 0; i < 6; i++) {
                        if (func_020231f4(gSaveData, 0, 0x28, i)) {
                            int r = GetBakuganEntryIndex(gSaveData, 0, 0x28, i);
                            func_020231c0(gSaveData, r, 0);
                            changed = 1;
                        }
                    }
                    Vec2s tmp = {0, 0};
                    if (changed) {
                        for (i = 0; i < 3; i++) {
                            u8 v = buf[i];
                            int r = GetBakuganEntryIndex(gSaveData, 0, 0x22, v);
                            func_020231c0(gSaveData, r, 0xc8);
                            SetBakuganEntry(gSaveData, (const u16 *) &tmp, 0x22, v);
                        }
                    }
                    ApplyVolumeOptions(gSaveData);
                    FrameTask::Get().StartChannel(0, 0x10);
                    data_020d433c.mUnk_76 = 1;
                    mLogoVisible = 1;
                    mLogoY = 0x48;
                    mState = 0x20;
                    break;
                }
            }
            break;
        case 13:
            delete mWindow30;
            mWindow30 = new SelectWindow(3, sTitleThreeButtonItems, -1);
            mWindow30->SetCells(3, sTitleThreeButtonCells);
            mWindow30->mCursor->mSelection = -1;
            mWindow30->mCursor->mSelection = 1;
            PrintText((MessageFile *) mBuf38, 8);
            GXS_SetVisiblePlane(0x19);
            mUnk_d0 = 1;
            mState++;
            break;
        case 14:
            if ((u8) mWindow30->Update(1) == 1) {
                if ((u8) mWindow30->mCursor->mSelection == 0) {
                    mState = 0x13;
                } else {
                    mState = 9;
                }
            }
            DrawText();
            mWindow30->Draw();
            break;
        case 15:
            delete mWindow30;
            mWindow30 = new SelectWindow(3, sTitleThreeButtonItems, -1);
            mWindow30->SetCells(3, sTitleThreeButtonCells);
            mWindow30->mCursor->mSelection = -1;
            mWindow30->mCursor->mSelection = 1;
            PrintText((MessageFile *) mBuf38, 7);
            GXS_SetVisiblePlane(0x19);
            mState++;
            break;
        case 16:
            if ((u8) mWindow30->Update(1) == 1) {
                if ((u8) mWindow30->mCursor->mSelection == 0) {
                    data_020d433c.mSaveSlotContent = 1;
                    if (SaveManager::Get().WriteSlotMarker(0) == SAVE_STATUS_ERROR) {
                        new MessageScreen(MESSAGE_SCREEN_SYSTEM_4_HALT);
                    }
                    mState++;
                } else {
                    mState = 9;
                }
            }
            DrawText();
            mWindow30->Draw();
            break;
        case 17:
            delete mWindow30;
            mWindow30 = new SelectWindow(2, sTitleTwoButtonItems, -1);
            mWindow30->SetCells(2, sTitleTwoButtonCells);
            mWindow30->mCursor->mSelection = -1;
            mWindow30->mCursor->mSelection = 0;
            PrintText((MessageFile *) mBuf38, 3);
            GXS_SetVisiblePlane(0x19);
            mState++;
            break;
        case 18:
            if ((u8) mWindow30->Update(1) == 1) {
                mState = 9;
            }
            DrawText();
            mWindow30->Draw();
            break;
        case 19:
            if (IsFadeDone(1)) {
                SetPlaneBlend(0);
                data_020d433c.mUnk_76 = 1;
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                FrameTask::Get().CloseHandles();
                mState++;
            }
            break;
        case 20:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            new MovieScene((void *) "/movie/bknd_op2.mods", data_020d433c.mLanguage);
            mState++;
            break;
        case 21:
            if (mContext->mRequest == 1) {
                FrameTask::Get().OpenHandles();
                FrameTask::Get().StartChannel(0, 0x17);
                mState++;
            }
            break;
        case 22:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            new NameEntryScene(0);
            mContext->mRequest = 0;
            mState++;
            break;
        case 23:
            if (mContext->mRequest != 0) {
                gSaveData[0].mAttribute = 0;
                mState++;
            }
            break;
        case 24:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            new AttrSelScene();
            mContext->mRequest = 0;
            mState++;
            break;
        case 25:
            switch (mContext->mRequest) {
                case ATTR_SEL_REQUEST_NAME_ENTRY:
                    mState = 0x16;
                    break;
                case ATTR_SEL_REQUEST_CHARACTER_EDIT:
                    mState++;
                    break;
            }
            break;
        case 26:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            new CharacterEditScene(0);
            mContext->mRequest = 0;
            mState++;
            break;
        case 27:
            switch (mContext->mRequest) {
                case 1:
                    mState++;
                    break;
                case 2:
                    mState = 0x18;
                    break;
            }
            break;
        case 28:
            InitSaveWindowDisplay();
            new SaveWindow(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mRequest = 0;
            data_020d433c.mSaveDirty = 0;
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 29:
            if (mContext->mRequest != 0) {
                InitDisplay();
                LoadGraphics();
                G2_BlendNone();
                SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
                mLogoVisible = 1;
                FrameTask::Get().StartChannel(0, 0x10);
                mState++;
            }
            break;
        case 30:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            SetPlaneBlend(1);
            mState++;
            break;
        case 31:
            if (IsFadeDone(0)) {
                mUnk_d6 = 1;
                mState++;
            }
            break;
        case 32:
            data_020d433c.mUnk_80 = 1;
            SelectWindowCursor *cursor = mCursor;
            cursor->mItems = (WindowItem *) sTitleMenuItems;
            cursor->mCount = 5;
            mCursor->mSelection = mMenuSelection;
            UploadSubBg(mMenuSelection);
            GXS_SetVisiblePlane(0x17);
            QueueFrameCallback(TitleScene::ApplyScroll, this);
            mScrollY = -0xc0;
            mTimer = 0x13;
            mState++;
            break;
        case 33:
            mTimer--;
            u8 cc = mTimer;
            if (cc != 0) {
                QueueFrameCallback(TitleScene::ApplyScroll, this);
                s8 d = cc - 3;
                mScrollY = -((s16) ((d * d - 9) * 0xc0 / 0x100));
                break;
            }
            QueueFrameCallback(TitleScene::ApplyScroll, this);
            mScrollY = 0;
            mMenuVisible = 1;
            RefreshMenuAnimations();
            SetMessage((u8) (mMenuSelection + 2));
            mTimer = 0;
            if (mUnk_d6) {
                mUnk_d6 = 0;
                delete mWindow30;
                mWindow30 = new SelectWindow(2, sTitleTwoButtonItems, -1);
                mWindow30->SetCells(2, sTitleTwoButtonCells);
                mWindow30->mCursor->mSelection = -1;
                mWindow30->mCursor->mSelection = 0;
                PrintText((MessageFile *) mBuf3c, 7);
                GXS_SetVisiblePlane(0x1f);
                mState++;
            } else {
                mState = 0x24;
            }
            break;
        case 34:
            if (mWindow30->Update(1)) {
                mState++;
            }
            DrawText();
            mWindow30->Draw();
            break;
        case 35:
            GXS_SetVisiblePlane(0x17);
            mState++;
            break;
        case 36: {
            u8 changed = 0;
            u8 prev = mMenuSelection;
            u8 event = mCursor->Update(1);
            if (prev != (u8) mCursor->mSelection) {
                mMenuSelection = mCursor->mSelection;
                RefreshMenuAnimations();
                SetMessage((u8) (mMenuSelection + 2));
            }
            if (event == 1 || event == 3) {
                changed = 1;
            }
            if (changed) {
                mCellAnims[mMenuSelection + 3]->SetAnimation(mMenuSelection * 3 + 2);
                mState++;
            }
            UpdateCursorCell();
            break;
        }
        case 37:
            UpdateCursorCell();
            if (IsFadeDone(0)) {
                SetPlaneBlend(0);
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mState++;
            }
            break;
        case 38:
            if (UpdateCursorCell()) {
                mState++;
            }
            break;
        case 39:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mMenuVisible = 0;
            mLogoVisible = 0;
            mState = 0x28;
            break;
        case 40: {
            data_020d433c.mTitleMenuSelection = mMenuSelection;
            u32 scene;
            switch (mMenuSelection) {
                case TITLE_MENU_MAP:
                    scene = SCENE_MAP;
                    if (gSaveData[0].mUnk_5bf_0) {
                        FillParkSlots();
                    }
                    break;
                case TITLE_MENU_ARENA_WINDOW:
                    data_020d433c.mUnk_89 = 0;
                    scene = SCENE_ARENA_WINDOW;
                    break;
                case TITLE_MENU_TUTORIAL:
                    data_020d433c.mTutorialNumber = 0;
                    data_020d433c.mTutorialPart = 0;
                    data_020d433c.mUnk_82 = 0;
                    scene = SCENE_TUTORIAL;
                    break;
                case TITLE_MENU_MYROOM:
                    data_020d433c.mUnk_7f = SCENE_TITLE;
                    scene = SCENE_MYROOM;
                    break;
                case TITLE_MENU_OPTION:
                    mContext->mRequest = 0;
                    new OptionScene();
                    mState = 0x29;
                    return;
            }
            RequestScene(scene);
            mFinished = 1;
            break;
        }
        case 41:
            if (mContext->mRequest != 0) {
                RequestScene(SCENE_TITLE);
                mFinished = 1;
                return;
            }
            break;
        case 42:
            new CreditsScene();
            mContext->mRequest = 0;
            mState++;
            break;
        case 43:
            if (mContext->mRequest != 0) {
                mState++;
            }
            break;
        case 44: {
            gSaveData[0].mUnk_5bf_0 = 1;
            s32 id = UnlockEmblem();
            if (id >= 0) {
                new GiftUnlockScene(id);
                mContext->mRequest = 0;
                mState--;
            } else {
                func_02086d48();
                mState++;
            }
            break;
        }
        case 45:
            InitSaveWindowDisplay();
            new SaveWindow(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mRequest = 0;
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
            break;
        case 46:
            if (mContext->mRequest != 0) {
                mState++;
            }
            break;
        case 47:
            data_020d433c.mCreditsPending = 0;
            RequestScene(SCENE_LOGO);
            mFinished = 1;
            return;
    }
    UpdateFade();
    if (mMenuVisible) {
        DrawMenuCells();
    }
    DrawLogo();
}

void TitleScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(2, 1, 2, 4, 0);
    G2S_SetBG2Control(2, 1, 4, 6, 0);
    G2S_SetBG3Control(0, 1, 6, 7, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0x140);
    G2S_SetBG2Offset(0, 0x140);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(1);
    G2S_SetBG2Priority(2);
    G2S_SetBG3Priority(0);
    GX_SetVisiblePlane(2);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

void TitleScene::InitSaveWindowDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(0);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x13);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_BlendNone();
    G2S_BlendNone();
}

void TitleScene::LoadGraphics() {
    char path[0x80];
    ChangeDir("/Game/Title/");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0, "title_u_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "title_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "title_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "title_u_bg_01_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG1_CHAR, 0, "title_u_bg_01_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG1_SCR, 0, "title_u_bg_01_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "title_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "title_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "title_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "title_b_bg_02_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "title_b_bg_02_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "title_b_bg_02_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x6000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG3_CHAR, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG3_SCR, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Title/");
    if (mBuffers[0][1] == NULL) {
        for (u8 i = 0; i < 5; i++) {
            sprintf(path, "title_b_bg_sub_%02d_ncl.bin", i);
            mBuffers[i][1] = LoadFile(path, 0, 0, 0, &mBufferSizes[i][1], 0);
            sprintf(path, "title_b_bg_sub_%02d_ncg.bin", i);
            mBuffers[i][0] = LoadFile(path, 0, 0, 0, &mBufferSizes[i][0], 0);
            sprintf(path, "title_b_bg_sub_%02d_nsc.bin", i);
            mBuffers[i][2] = LoadFile(path, 0, 0, 0, &mBufferSizes[i][2], 0);
        }
    }
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.LoadCharacters("title_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadExtPalette("title_b_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i < 8; i++) {
        if (mCellAnims[i] != NULL) {
            delete mCellAnims[i];
        }
        mCellAnims[i] = new CellAnimation("title_b_obj_00_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = screen.GetCharBase(0);
        mCellAnims[i]->mPaletteBase = 0;
    }
    mCellAnims[3]->SetAnimation(0);
    mCellAnims[4]->SetAnimation(3);
    mCellAnims[5]->SetAnimation(6);
    mCellAnims[6]->SetAnimation(9);
    mCellAnims[7]->SetAnimation(12);
    screen.LoadCharacters("title_b_obj_03.nce_ncg1D.bin", 0xb000, 0);
    screen.LoadPalette("title_b_obj_03_ncl.bin", 0x40, 0);
    if (mCellAnims[0] != NULL) {
        delete mCellAnims[0];
    }
    mCellAnims[0] = new CellAnimation("title_b_obj_03_nce.bin", 0, 1, 1);
    mCellAnims[0]->mCharBase = screen.GetCharBase(0xb000);
    mCellAnims[0]->mPaletteBase = 2;
    screen.LoadCharacters("title_b_obj_02.nce_ncg1D.bin", 0xc000, 0);
    screen.LoadExtPalette("title_b_obj_02_ncl.bin", 0x400, 0);
    if (mCellAnims[1] != NULL) {
        delete mCellAnims[1];
    }
    mCellAnims[1] = new CellAnimation("title_b_obj_02_nce.bin", 1, 1, 1);
    mCellAnims[1]->mCharBase = screen.GetCharBase(0xc000);
    mCellAnims[1]->mPaletteBase = 2;
    screen.LoadCharacters("title_b_obj_04.nce_ncg1D.bin", 0x1d000, 0);
    screen.LoadPalette("title_b_obj_04_ncl.bin", 0xe0, 0);
    if (mCellAnims[2] != NULL) {
        delete mCellAnims[2];
    }
    mCellAnims[2] = new CellAnimation("title_b_obj_04_nce.bin", 0, 1, 1);
    mCellAnims[2]->mCharBase = screen.GetCharBase(0x1d000);
    mCellAnims[2]->mPaletteBase = 7;
    screen.LoadCharacters("title_b_obj_01.nce_ncg1D.bin", 0x13000, 0);
    screen.LoadExtPalette("title_b_obj_01_ncl.bin", 0x200, 0);
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x1b000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
    delete mMessageWindow;
    mMessageWindow = new MessageWindow("mes_title", 1, 0x1f000);
    mMessageWindow->func_02065bd4(0x14, 4, 1);
    mMessageWindow->SetWidth(0xd8);
}

void TitleScene::func_ov000_022245e8() {
    mCellAnims[0]->Update();
    mCellAnims[0]->Draw(1, 0x80, 0x60, 0);
}

void TitleScene::DrawMenuCells() {
    for (u8 i = 0; i < 5; i++) {
        mCellAnims[3 + i]->Update();
        mCellAnims[3 + i]->Draw(1, 0x80, 0x60, 3);
    }
}

void TitleScene::RefreshMenuAnimations() {
    for (u8 i = 0; i < 5; i++) {
        u8 anim = i * 3;
        if (i == mMenuSelection) {
            anim = anim + 1;
        }
        mCellAnims[3 + i]->SetAnimation(anim);
    }
}

void TitleScene::LoadSystemText() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    if (mBuf38 == NULL) {
        ChangeDir("/");
        mBuf38 = LoadFile("font/mes_system.mes", 0, 0, 0, 0, 0);
        mBuf3c = LoadFile("font/mes_title.mes", 0, 0, 0, 0, 0);
        mTextBufferSize = TextManager::Get().BufferSize(0x18, 0xa);
        mBuf40 = OS_Alloc(mTextBufferSize);
        mGlyphCount = TextManager::Get().GlyphCount(0x3c);
        mBuf44 = OS_Alloc(mGlyphCount * 0xc);
        TextManager::Get().Init(mBuf44, 1, 0, 0x18, 0xa, 0x11);
    }
    MI_CpuClear16(mBuf40, mTextBufferSize);
    TextManager::Get().SetColor(1, 0, 0xf);
}

void TitleScene::PrintText(MessageFile *file, u32 index) {
    ClearText();
    TextManager &text = TextManager::Get();
    s16 outWidth;
    s16 outHeight;
    outHeight = 0;
    outWidth = 0;
    text.Print(file->GetText(index), file->GetLength(index), mBuf40, 0x18, 0xa, &outWidth, &outHeight, NULL, 0, 2,
               0x11, 0xe, 0, 0, 1, 1, 1);
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0x19000, mBuf40, mTextBufferSize, 0);
}

void TitleScene::DrawText() {
    SpriteManager::Get().Screen(1).AddOam(mBuf44, (u8) mGlyphCount, 0x20, 0x28, 0, 0x320, 0xf, 0);
}

void TitleScene::ClearText() {
    if (mBuf40 != NULL) {
        MI_CpuClear16(mBuf40, mTextBufferSize);
    }
}

void TitleScene::SetMessage(u32 index) {
    if ((index <= 6) & (mMessageIndex != index)) {
        mMessageIndex = index;
        mMessageWindow->StartMessage(mMessageIndex, 0, 1);
    }
}

void TitleScene::SetPlaneBlend(u8 enabled) {
    mPlaneBlendEnabled = enabled;
    if (enabled) {
        GX_SetVisiblePlane(3);
        G2_SetWndOutsidePlane(0x1f, TRUE);
        mBlendAngle = 0x2d;
        mBlendAmplitude = 0x10;
    } else {
        GX_SetVisiblePlane(1);
        G2_BlendNone();
        mBlendAngle = 0;
    }
}

// Same helper as FieldSelectScene.cpp defines locally; belongs in nitro/gx.h next to G2S_SetBlendAlpha.
void TitleScene::UpdateFade() {
    if (!mPlaneBlendEnabled) {
        return;
    }
    u16 angle = (mBlendAngle << 16) / 180;
    s16 sin = FX_SinCosTable_[(angle >> 4) * 2];
    s8 level = (mBlendAmplitude * sin) / 4096;
    if (level < 0) {
        level = 0;
    }
    int ev2 = 0x10 - level;
    G2_SetBlendAlpha(2, 1, level, ev2);
    if (mBlendAngle != 0) {
        mBlendAngle--;
        return;
    }
    mBlendAmplitude = 6;
    mBlendAngle = 0xb4;
}

BOOL TitleScene::IsFadeDone(BOOL tick) {
    if (!mPlaneBlendEnabled) {
        return TRUE;
    }
    if (mBlendAngle < 0x5a) {
        if (tick != 0 && mBlendAngle != 0) {
            mBlendAngle--;
        }
        return FALSE;
    }
    return TRUE;
}

void TitleScene::UploadSubBg(u32 index) {
    QueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x4000, mBuffers[index][1], mBufferSizes[index][1], 0);
    QueueUpload(1, UPLOAD_KIND_BG2_CHAR, 0, mBuffers[index][0], mBufferSizes[index][0], 0);
    QueueUpload(1, UPLOAD_KIND_BG2_SCR, 0, mBuffers[index][2], mBufferSizes[index][2], 0);
}

BOOL TitleScene::UpdateCursorCell() {
    mCellAnims[1]->Update();
    mCellAnims[1]->Draw(1, 0x80, 0x58, 2);
    if (mCursorStep == 0) {
        if (mSubBgIndex == mMenuSelection) {
            return TRUE;
        }
        mCursorStep = 1;
        mCellAnims[1]->SetAnimation(2);
        SoundManager::Get().StartSeqArc(0xd);
    }
    switch (mCursorStep) {
        case 1:
            if (mCellAnims[1]->mFinished != 0) {
                mCursorStep++;
            }
            break;
        case 2: {
            u8 cur = mMenuSelection;
            mSubBgIndex = cur;
            UploadSubBg(cur);
            mCellAnims[1]->SetAnimation(3);
            SoundManager::Get().StartSeqArc(0xc);
            mCursorStep++;
            break;
        }
        case 3:
            if (mCellAnims[1]->mFinished != 0) {
                mCursorStep = 0;
            }
            break;
    }
    return FALSE;
}

void TitleScene::ApplyScroll(void *arg) {
    TitleScene *self = (TitleScene *) arg;
    G2S_SetBG1Offset(0, self->mScrollY);
    G2S_SetBG2Offset(0, self->mScrollY);
}

void TitleScene::DrawLogo() {
    if (!mLogoVisible) {
        return;
    }
    if (mLogoY < 0x60) {
        mLogoY++;
    } else {
        mMessageWindow->Update();
    }
    mCellAnims[2]->Draw(1, 0x80, mLogoY, 1);
}
