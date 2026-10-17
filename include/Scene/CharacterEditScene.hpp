#pragma once
#include "global.h"
#include "types.h"
#include "Scene/SceneBase.hpp"
#include "UI/SelectWindow.hpp"
#include "UI/MessageWindow.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Camera.hpp"
#include "System/FileSystem.hpp"
#include "Graphics/Model.hpp"

struct TextSlot {
    /* 00 */ s8 mItemIndex;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u32 mVisible;
    /* 08 */ u16 mWidth;
    /* 0a */ u16 mHeight;
    /* 0c */
};

// Appearance of the player's character: the bit fields stored in the save data at +0x14.
struct CharacterLook {
    /* 00 */ u16 mUnk_00_0 : 1;
    /* 00 */ u16 mUnk_00_1 : 3;
    /* 00 */ u16 mUnk_00_4 : 3;
    /* 00 */ u16 mUnk_00_7 : 3;
    /* 00 */ u16 mUnk_00_10 : 3;
    /* 00 */ u16 mUnk_00_13 : 3;
    /* 02 */ u16 mUnk_02_0 : 3;
    /* 02 */ u16 mUnk_02_3 : 3;
    /* 02 */ u16 mUnk_02_6 : 3;
    /* 02 */ u16 mUnk_02_9 : 3;
    /* 02 */ u16 mUnk_02_12 : 3;
    /* 02 */ u16 mUnk_02_15 : 1;
    /* 04 */ u16 mUnk_04_0 : 3;
    /* 04 */ u16 mUnk_04_3 : 5;
    /* 04 */ u16 mUnk_04_8 : 3;
    /* 04 */ u16 mUnk_04_11 : 3;
    /* 04 */ u16 mUnk_04_14 : 2;
    /* 06 */ u16 mUnk_06_0 : 3;
    /* 06 */ u16 mUnk_06_3 : 5;
    /* 06 */ u16 mUnk_06_8 : 8;
    /* 08 */
};

// A list of choices (hair styles, colours...) and the one that is selected.
struct LookChoices {
    /* 00 */ u8 mIndex;
    /* 01 */ u8 mCount;
    /* 02 */ u8 mValues[8];
    /* 0a */
};

class CharacterEditScene : public SceneBase {
public:
    /* 0c */ SelectWindow *mWindows[3];
    /* 18 */ u32 mWindow0Result;
    /* 1c */ u32 mWindow1Result;
    /* 20 */ u32 mWindow2Result;
    /* 24 */ UnkStruct_0205401c *mModel;
    /* 28 */ Camera *mCamera;
    /* 2c */ ModelLightGroup *mLights;
    /* 30 */ u8 mState;
    /* 31 */ u8 mStep;
    /* 32 */ u16 mStepFrames;
    /* 34 */ LookChoices mLists[16];
    /* d4 */ TextSlot mTexts[6];
    /* 11c */ u32 *mTextBuffers[6];
    /* 134 */ void *mTextGlyphBuffer;
    /* 138 */ u32 mTextBufferSize;
    /* 13c */ u32 mTextGlyphCount;
    /* 140 */ void *mMessages;
    /* 144 */ MessageWindow *mMessageWindow;
    /* 148 */ u8 mRotateState;
    /* 14c */ u32 mChoicesVisible;
    /* 150 */ ModelInstance *mCutRoomModel;
    /* 154 */ u8 mMenuDepth;
    /* 155 */ u8 mMenuSelection[3]; // one entry per mMenuDepth
    /* 158 */ u8 mMenuDepthMove;
    /* 159 */ u8 mCursorItem;
    /* 15a */ u8 mStyleIndex;
    /* 15b */ u8 mColorIndex;
    // colour row 2 (items 14-19): hair's only colour list, the glasses' color_glasses_len one
    /* 15c */ u8 mColor2Index;
    /* 15d */ u8 mStyleCount;
    /* 15e */ u8 mColorCount;
    /* 15f */ u8 mColor2Count; // count of colour row 2 (see mColor2Index)
    /* 160 */ u8 *mStyleAnimIndices;
    /* 164 */ SelectWindowCursor *mCursor;
    /* 168 */ CellAnimation *mCellAnims[16];
    /* 1a8 */ u32 mRebuildModel;
    /* 1ac */ s8 mFixedItem; // item index that does not slide in (SetupSlideIn), -1 = none
    /* 1ad */ PAD(0x1ad, 0x1b0);
    /* 1b0 */ u32 mUnk_1b0;
    // Ctor argument: 1 from MyroomScene, 0 from TitleScene. 1 sets flags 0x8000 on item 4 of the five-item menus
    // (mState 1 / 3) and moves the root entries' states and texts up by one, keeps lists 0-2 in RandomizeLook, skips
    // StoreLook on the direct exit and asks first when mChoiceApplied is set.
    /* 1b4 */ u8 mFromMyroom;
    /* 1b5 */ PAD(0x1b5, 0x1b8);
    /* 1b8 */ u32 mChoiceApplied;
    /* 1bc */ void *mWindowTextBuffer; // the text shown while mWindows[2] is up (steps 0xb-0xe): PrintWindowText
    /* 1c0 */ void *mWindowGlyphBuffer;
    /* 1c4 */ u32 mWindowTextBufferSize; // size of mWindowTextBuffer
    /* 1c8 */ u32 mWindowGlyphCount;
    /* 1cc */ u32 mWindowTextVisible;
    /* 1d0 */ u8 mNextScene;
    /* 1d1 */ PAD(0x1d1, 0x1d2);
    /* 1d2 */ u16 mModelAngle;
    /* 1d4 */ void *mNameBuffer;
    /* 1d8 */ void *mNameGlyphBuffer;
    /* 1dc */ u32 mNameBufferSize;
    /* 1e0 */ u32 mNameGlyphCount;
    /* 1e4 */ CellAnimation *mNameplateCellAnim;
    /* 1e8 */

    CharacterEditScene(u32 unk);
    virtual ~CharacterEditScene();
    virtual void Update();

    void func_020638a8(u32 v);
    void UpdateRootMenu();
    void UpdateSkinEdit();
    void func_0205e958();
    void UpdateFaceEdit();
    void UpdateHairEdit();
    void UpdateCapEdit();
    void UpdateGlassesEdit();
    void UpdateTopEdit();
    void func_020604e8();
    void UpdateBottomEdit();
    void UpdateShoesEdit();
    void func_020612cc();
    void UpdateFinish();
    void SetupChoices();
    // Also loads palette2, color_hair_male (state 7) or color_glasses_len (state 9), and sets mChoicesVisible.
    void LoadChoiceCells(const char *chars, const char *palette, const char *cells, const char *palette2);
    void Draw();
    BOOL EnableSelectedGroupButtons(); // TRUE for mWindows[1] ids 1-6: enables that id's group (1 / 2-4 / 5-6) only
    void StoreLook(CharacterLook *look);
    void DrawChoices();
    void UpdateChoiceInput();
    // Hair (state 7) style items also reset list 5. Step 6 calls it after RandomizeLook in mState 1, where no case
    // matches: only the mCursorItem index copy and the tail (ApplyLookToModel, mRebuildModel, mChoiceApplied) run.
    void ApplyCursorItem();
    void ApplyLookToModel();
    void RandomizeLook();
    void func_02064cd4(u32 flag);
    void PrintWindowText(); // message 0x1c into mWindowTextBuffer, shown with mWindows[2]
    void DrawName();
    void SetText(u32 slot, u32 message, u32 value);
    void DrawTexts();
    void UpdateInit();
    void func_02061c3c();
    void LoadGraphics(); // also loads mes_chara_edit.mes into mMessages and creates mMessageWindow
    void PrintName();
    void ResetTexts(); // the six mTexts and buffers, mWindowTextBuffer (mWindowTextVisible 0), mMessageWindow's buffer
    void InitLookLists();
    void ClearName();
    void SetItemFlags(u32 i, u32 v);
    void SetItemSlide(u32 i, u32 a, u32 b);
    void SetChoiceItemNeighbors(u32 i, u32 a, u32 b, u32 c, u32 d);
    void MoveMenuDepth(u32 v); // 1: mMenuDepth + 1, 2: - 1, 0: unchanged; v is kept in mMenuDepthMove
    void UpdateRotateInput();
    void func_02063b10(u32 arg);
    void SetupSlideIn(); // one item skips the slide-in (mFixedItem): 0 going deeper, the depth's selection going back
    void SlideToParent();
    void SlideToFirstRow();
    BOOL SwitchToSelectedGroup();
    void func_020637a8();
    void EnableGroupButtons(); // for the current mState: button 0 and that state's group of buttons 1-6 (12: none)
};
