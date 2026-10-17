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
    /* 04 */ u32 mActive;
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
    /* 18 */ u32 mUnk_18;
    /* 1c */ u32 mUnk_1c;
    /* 20 */ u32 mUnk_20;
    /* 24 */ UnkStruct_0205401c *mModel;
    /* 28 */ Camera *mCamera;
    /* 2c */ ModelLightGroup *mPalette;
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
    /* 144 */ MessageWindow *mMessage;
    /* 148 */ u8 mRotateState;
    /* 14c */ u32 mUnk_14c;
    /* 150 */ ModelInstance *mCutRoomModel;
    /* 154 */ u8 mMenuDepth;
    /* 155 */ u8 mMenuSelection[3];
    /* 158 */ u8 mMenuDepthMove;
    /* 159 */ u8 mCursorItem;
    /* 15a */ u8 mStyleIndex;
    /* 15b */ u8 mColorIndex;
    /* 15c */ u8 mColor2Index;
    /* 15d */ u8 mStyleCount;
    /* 15e */ u8 mColorCount;
    /* 15f */ u8 mColor2Count;
    /* 160 */ u8 *mStyleAnimIndices;
    /* 164 */ SelectWindowCursor *mCursor;
    /* 168 */ CellAnimation *mCells[16];
    /* 1a8 */ u32 mRebuildModel;
    /* 1ac */ s8 mFixedItem;
    /* 1ad */ PAD(0x1ad, 0x1b0);
    /* 1b0 */ u32 mUnk_1b0;
    /* 1b4 */ u8 mFromMyroom;
    /* 1b5 */ PAD(0x1b5, 0x1b8);
    /* 1b8 */ u32 mChoiceApplied;
    /* 1bc */ void *mWindowTextBuffer;
    /* 1c0 */ void *mWindowGlyphBuffer;
    /* 1c4 */ u32 mWindowTextBufferSize; // size of mUnk_1bc
    /* 1c8 */ u32 mWindowGlyphCount;
    /* 1cc */ u32 mWindowTextVisible;
    /* 1d0 */ u8 mNextScene;
    /* 1d1 */ PAD(0x1d1, 0x1d2);
    /* 1d2 */ u16 mModelAngle;
    /* 1d4 */ void *mNameBuffer;
    /* 1d8 */ void *mNameGlyphBuffer;
    /* 1dc */ u32 mNameBufferSize;
    /* 1e0 */ u32 mNameGlyphCount;
    /* 1e4 */ CellAnimation *mNameplateCell;
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
    void LoadChoiceCells(const char *chars, const char *palette, const char *cells, const char *palette2);
    void Draw();
    BOOL EnableSelectedGroupButtons();
    void StoreLook(CharacterLook *look);
    void DrawChoices();
    void UpdateChoiceInput();
    void ApplyCursorItem();
    void ApplyLookToModel();
    void RandomizeLook();
    void func_02064cd4(u32 flag);
    void PrintWindowText();
    void DrawName();
    void SetText(u32 slot, u32 message, u32 value);
    void DrawTexts();
    void UpdateInit();
    void func_02061c3c();
    void LoadGraphics();
    void PrintName();
    void ResetTexts();
    void InitLookLists();
    void ClearName();
    void SetItemFlags(u32 i, u32 v);
    void SetItemSlide(u32 i, u32 a, u32 b);
    void SetChoiceItemNeighbors(u32 i, u32 a, u32 b, u32 c, u32 d);
    void MoveMenuDepth(u32 v);
    void UpdateRotateInput();
    void func_02063b10(u32 arg);
    void SetupSlideIn();
    void SlideToParent();
    void SlideToFirstRow();
    BOOL SwitchToSelectedGroup();
    void func_020637a8();
    void EnableGroupButtons();
};
