#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"
#include "UI/SelectWindow.hpp"

class CellAnimation;
class CellAnimFile;
class ModelLightGroup;
class Camera;
class BakuganModel;
struct BattleCellPair;

// A value at 0x0c of BattleCommandList that UpdatePage steps towards its target by one per frame.
struct UnkStruct_BattleCommandList_0c {
    /* 0 */ u8 mCur;
    /* 1 */ u8 mTarget;
    /* 2 */
};

// List page of the battle command screen (0x168 bytes, ctor 020575e0, vtable 020b3298).
class BattleCommandList : public SceneBase {
public:
    /* 0c */ UnkStruct_BattleCommandList_0c mUnk_0c[5];
    /* 16 */ u8 mUnk_16[5];
    /* 1b */ PAD(0x1b, 0x1c);
    /* 1c */ void *mNceData[4];
    /* 2c */ CellAnimFile *mCellFiles[4];
    /* 3c */ CellAnimation *mCells3c[7];
    /* 58 */ CellAnimation *mCells58[8];
    /* 78 */ void *mBakuganMessages;
    /* 7c */ void *mBakuganDisplayMessages;
    /* 80 */ void *mBakuganNameMessages;
    /* 84 */ void *mResultMessages;
    /* 88 */ ModelLightGroup *mLights;
    /* 8c */ SelectWindow *mWindow;
    /* 90 */ BakuganModel *mBakuganModel;
    /* 94 */ Camera *mCamera;
    /* 98 */ u16 mUnk_98;
    /* 9a */ u16 mUnk_9a;
    /* 9c */ u16 mUnk_9c;
    /* 9e */ PAD(0x9e, 0xa0);
    /* a0 */ void *mTextBuffers[3];
    /* ac */ void *mGlyphBuffers[3];
    /* b8 */ u32 mTextBufferSizes[3];
    /* c4 */ u32 mGlyphCounts[3];
    /* d0 */ u32 mFiles[14];
    /* 108 */ u32 mFileSizes[14];
    /* 140 */ u8 mLoadStep;
    /* 141 */ PAD(0x141, 0x142);
    /* 142 */ u16 mValues[6];
    /* 14e */ u8 mBakuganId;
    /* 14f */ u8 mNextBakuganId;
    /* 150 */ u8 mAttribute;
    /* 151 */ u8 mNextAttribute;
    /* 152 */ s8 mPlayer;
    /* 153 */ PAD(0x153, 0x154);
    /* 154 */ u16 mNameWidth;
    /* 156 */ u8 mTextPage;
    /* 157 */ u8 mLastTextPage;
    /* 158 */ u8 mWindowVisible;
    /* 159 */ u8 mState;
    /* 15a */ u8 mUnk_15a;
    /* 15b */ u8 mPage;
    /* 15c */ u8 mNextPage;
    /* 15d */ u8 mVisible; // draw gate; UpdatePage's count-up and the window update are skipped with it
    /* 15e */ u8 mCloseRequested;
    /* 15f */ u8 mBakuganHidden;
    /* 160 */ u8 mHideBakuganRequested;
    /* 161 */ u8 mUnk_161;
    /* 162 */ u8 mUnk_162;
    /* 163 */ u8 mFourItems; // always 0 in this build: every CreateCommandListWindow call passes g = 0
    /* 164 */ u8 mUnk_164;
    /* 165 */ u8 mCountUpTimer; // frames left before the count-up starts (0x3c), counted down by UpdatePage
    /* 166 */ u16 mCountUpRemaining;
    /* 168 */

    BattleCommandList(u8 a, u8 b, s32 c, u8 d);
    virtual ~BattleCommandList();
    virtual void Update();
    void func_02058240(u32 value);
    void CyclePage(u32 flag);
    BOOL IsSettingUp();
    void SetWindowVisible(u32 enable); // the gate also stops UpdateWindow; enabling does nothing while mWindow is NULL
    void UpdateWindow(); // a chosen item steps the page (CyclePage)
    // Item 1 steps forward (enabled while mPage <= mLastTextPage), item 0 back; a disabled item gets flag 0x8000,
    // which the cursor skips.
    void UpdateListItems();
    BOOL IsListItemEnabled(u32 right);
    void InitDisplayAndCamera();
    void ResetLoadStep();
    BOOL LoadGraphics(); // staged: one file per call
    BOOL CreateBakuganModel();
    void LoadText();
    void PrintText(u8 index);
    void DrawName();
    void ClearText();
    void DrawBakugan();
    void UpdatePage(); // also runs the count-up StartCountUp began; with mBakuganHidden set only mCells3c[2] is drawn
    void DrawCardspecBack();
    void ClearValues();
    void SetValue(u32 index, u32 value);
    void StartCountUp(u32 value); // ignored while a count-up runs; UpdatePage advances it
    BOOL IsCountingUp();
    void func_0205b0dc(const BattleCellPair *pair);
};

// 4 byte record of a battle command cell: a 4 bit value and five 3 bit values.
struct BattleCellPair {
    union {
        /* 0 */ u16 mUnk_00;
        struct {
            u16 mUnk_00_0 : 4;
            u16 mUnk_00_4 : 12;
        };
    };
    union {
        /* 2 */ u16 mUnk_02;
        struct {
            u16 mUnk_02_0 : 3;
            u16 mUnk_02_3 : 3;
            u16 mUnk_02_6 : 3;
            u16 mUnk_02_9 : 3;
            u16 mUnk_02_12 : 3;
            u16 mUnk_02_15 : 1;
        };
    };
};

void SetCommandListValues(BattleCommandList *list, u32 a, u32 b, BattleCellPair *pair);
void SetInitialEntryFields(u32 index, u32 unk2);

void CloseCommandList(BattleCommandList *list); // asks the list to end (mCloseRequested)
void SetCommandListBakuganHidden(BattleCommandList *list, u32 a);
void func_02058248(BattleCommandList *list, u8 b);
// Also sets mPlayer = c % 4 (-1, with mUnk_16 cleared, when c < 0) and clears mHideBakuganRequested (and mBakuganHidden in state 1 or
// when the ids are unchanged).
void SetCommandListBakugan(BattleCommandList *list, u8 a, u8 b, s32 c);
void CreateCommandListWindow(BattleCommandList *list, u32 a, u32 b, s16 c, s16 d, s16 e, s16 f, u32 g);
