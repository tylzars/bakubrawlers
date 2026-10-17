#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class CharacterIcons;
struct MessageFile;

// A cell animation entry (one per button or marker).
struct CellEntry {
    /* 00 */ CellAnimation *mCellAnim;
    /* 04 */ s16 mX;
    /* 06 */ s16 mY;
    /* 08 */ u8 mVisible;
    /* 09 */ u8 mUnk_09;
    /* 0a */ u8 mUpdateAnim;
    /* 0b */ PAD(0x0b, 0x0c);
    /* 0c */
};

// Input state of one player (local or received), 0x10 bytes each.
struct FieldInput {
    /* 00 */ u16 mKeys;
    /* 02 */ u8 mCursorItem;
    /* 03 */ u8 mReleased;
    /* 04 */ u8 mTouchX;
    /* 05 */ u8 mTouchY;
    /* 06 */ u8 mTaken;
    /* 07 */ u8 mUnk_07;
    /* 08 */ u8 mTakenItem;
    /* 09 */ PAD(0x09, 0x0a);
    /* 0a */ u8 mWindowOpen;
    /* 0b */ u8 mWindowSelection;
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mPressTimer;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */
};

// Page kinds: the values the constructor writes into mPageKinds[] in page order and that SetupPage, UpdatePage, Draw
// and the cursor and window helpers test through mPageKinds[mPage]. NUMBER, RULE, CHARA, FIELD and WINDOW are named
// after the files SetupPage loads for the page (select_number_*, select_rule_*, select_ch_* with the slot names from
// mes_StoryCharaName, select_field_*, union_b_bg_window_*); PAIR's update (UpdatePairPage) pairs the slots from
// sFieldSelectPairSlots. END closes the list: UpdatePage stops when the next page is END or later.
#define FIELD_SELECT_PAGE_NUMBER 0
#define FIELD_SELECT_PAGE_RULE 1
#define FIELD_SELECT_PAGE_CHARA 2
#define FIELD_SELECT_PAGE_PAIR 3
#define FIELD_SELECT_PAGE_FIELD 4
#define FIELD_SELECT_PAGE_WINDOW 5
#define FIELD_SELECT_PAGE_END 6

// Overlay 1: battle field selection (select_field_*); a state machine on mState.
class FieldSelectScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ CellEntry mMain;
    /* 18 */ CellEntry mCells[9];
    /* 84 */ CellEntry mCells2[9];
    /* f0 */ SelectWindow *mWindows[4];
    /* 100 */ MessageWindow *mMessageWindow;
    /* 104 */ CharacterIcons *mPanel[2];
    union {
        /* 10c */ u8 mUnk_10c[0x40];
        /* 10c */ FieldInput mInput[4];
    };
    /* 14c */ u8 mCharacterLooks[0x20];
    /* 16c */ void *mFiles[10][3];
    /* 1e4 */ u32 mFileSizes[10][3];
    union {
        /* 25c */ u8 mUnk_25c[10];
        /* 25c */ struct { u8 mValue; u8 mCount; } mOptions[5];
        /* 25c */ u8 mOptionBytes[5][2];
    };
    /* 266 */ u8 mCharaUnlocked[4][0x12];
    /* 2ae */ u8 mFieldUnlocked[4][9];
    /* 2d2 */ u8 mCharaNew[0x12];
    /* 2e4 */ u8 mFieldNew[9];
    /* 2ed */ u8 mPageKinds[7];
    /* 2f4 */ MessageFile *mMessages;
    /* 2f8 */ void *mTextBuffer;
    /* 2fc */ void *mGlyphBuffer;
    /* 300 */ u32 mTextBufferSize;
    /* 304 */ u32 mGlyphCount;
    /* 308 */ MessageFile *mStoryCharaNameMessages;
    /* 30c */ void *mNameBuffers[4];
    /* 31c */ void *mNameGlyphBuffer;
    /* 320 */ u32 mNameBufferSize;
    /* 324 */ u32 mNameGlyphCount;
    /* 328 */ u8 mState;
    /* 329 */ u8 mPage;
    /* 32a */ u8 mPlayerCount;
    /* 32b */ u8 mInputCount;
    /* 32c */ u8 mHasPartners;
    /* 32d */ u8 mShowRulePage;
    /* 32e */ u8 mBlend;
    /* 32f */ u8 mBlendTarget;
    /* 330 */ u8 mBlendStep;
    /* 331 */ u8 mBlendTimer;
    /* 332 */ u8 mNextScene;
    /* 333 */ u8 mLocalIsFirstSlot;
    /* 334 */ u8 mPlayerRowsVisible;
    /* 335 */ u8 mUnk_335;
    /* 336 */ u8 mUnk_336;
    /* 337 */ u8 mFieldMarksVisible;
    /* 338 */ u8 mPairGridVisible;
    /* 339 */ u8 mNumberMarksVisible;
    /* 33a */ u8 mUnk_33a;
    /* 33b */ u8 mLocalSlot;
    /* 33c */ u8 mUnk_33c;
    /* 33d */ u8 mLocalTaken;
    /* 33e */ u8 mWindow0ItemCount;
    /* 33f */ u8 mUnk_33f;
    /* 340 */ u8 mUnk_340;
    /* 341 */ u8 mSavedFieldItem;
    /* 342 */ PAD(0x342, 0x344);
    /* 344 */ s32 mPendingSubPlanes;
    /* 348 */ u8 mMessageEnabled;
    /* 349 */

    FieldSelectScene(u8 unk1);

    /* 00 */ virtual ~FieldSelectScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void func_ov001_02222040();
    BOOL func_ov001_02221c0c();
    BOOL func_ov001_02221f1c();
    BOOL func_ov001_02221aa8(u32 unk1);
    u32 UpdatePage();
    void func_ov001_02226cb0();
    void func_ov001_02226d74();
    void ResetWindow(SelectWindow *window);
    BOOL UpdatePageBlend();
    void DrawWindowText();
    u32 UpdateWindowPage();
    u32 UpdateRulePage();
    u32 UpdateCharaPage();
    u32 UpdatePairPage();
    u32 UpdateFieldPage();
    u32 UpdateNumberPage();
    u32 FindCharaItem(u32 unk1);
    // provisional declarations (signatures guessed from callers; fix when decompiling the function)
    void ResetPage();
    void SetupPage();
    void SetCursorItem(u32 slot, u32 value, u32 playSound);
    BOOL TakeCursorItem(u8 slot);
    BOOL IsCursorItemBlocked(u32 slot);
    BOOL func_ov001_02223914();
    void func_ov001_02223d70(u32 unk1);
    void UpdatePairCells();
    void UploadFieldFiles(u32 unk1);
    void MoveBlendToTarget();
    void SetWindowOpen(u32 unk1, u32 unk2, u32 unk3);
    s16 GetCharaItemX(u32 unk1);
    s16 GetCharaItemY(u32 unk1);
    s16 GetFieldItemX(u32 unk1);
    s16 GetFieldItemY(u32 unk1);
    void PrintWindowText(u32 unk1);
    void ClearTextBuffer();
    void PrintSlotName(u8 unk1);
    void DrawNames();
    void ClearName(u32 unk1);
    void func_ov001_02226d18(u32 unk1);
    void Draw();
    void LoadMessages();
    void LoadNameText();
    void InitDisplay();
    void LoadGraphics();
};
