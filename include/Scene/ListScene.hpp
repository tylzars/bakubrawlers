#pragma once

#include "Scene/SceneBase.hpp"
#include "UI/MessageWindow.hpp"
#include "global.h"
#include "types.h"

class CellAnimation;
class MessageWindow;
class SelectWindow;
struct SelectWindowCursor;
class BattleCommandList;
class BattleCommandPage;
class UnkStruct_02076058;
class UnkStruct_02071d24;

// Object with a virtual destructor in the second slot (vtable +4).
class UnkObject_02078e14 {
public:
    /* 00 */ virtual ~UnkObject_02078e14();
    /* 04 */ u8 mUnk_04;
    /* 05 */ PAD(0x05, 0x64);
    /* 64 */ u8 mUnk_64;
};

// Created by StoreScene (modes 0-2), MyroomScene (modes 3-4) and BattlePlayerList (mode 5).
class ListScene : public SceneBase {
public:
    /* 00c */ void *mStorecommentMessages;
    /* 010 */ void *mBakuganNameShortMessages;
    /* 014 */ void *mBakuganNameMessages;
    /* 018 */ void *mCardNameShortMessages;
    /* 01c */ void *mDeckSelectMessages;
    /* 020 */ void *mResultMessages;
    /* 024 */ u16 mAlphabetChar;
    /* 026 */ PAD(0x026, 0x028);
    /* 028 */ void *mTextBuffers[2];
    /* 030 */ void *mGlyphBuffers[2];
    /* 038 */ u32 mTextBufferSizes[2]; // buffer sizes
    /* 040 */ u32 mGlyphCounts[2]; // glyph counts
    /* 048 */ SelectWindow *mPromptWindow;
    /* 04c */ SelectWindow *mListWindow;
    /* 050 */ SelectWindow *mDeckWindow;
    /* 054 */ SelectWindowCursor *mPageAndRowCursor;
    /* 058 */ SelectWindowCursor *mAreaCursor;
    /* 05c */ CellAnimation *mMainScreenCellAnims[6];
    /* 074 */ CellAnimation *mSubScreenCellAnims[19];
    /* 0c0 */ BattleCommandList *mCommandList;
    /* 0c4 */ BattleCommandPage *mCommandPage;
    /* 0c8 */ u16 mSlotValues[12];
    /* 0e0 */ // (the u8 fields below start here)
    /* 0e0 */ u8 mMode;
    /* 0e1 */ u8 mState;
    /* 0e2 */ u8 mListWindowResult;
    /* 0e3 */ PAD(0x0e3, 0x0e4);
    /* 0e4 */ s16 mPageSlideX;
    /* 0e6 */ u8 mPage;
    /* 0e7 */ s8 mTargetPage;
    /* 0e8 */ u8 mCursorPage;
    /* 0e9 */ u8 mPageCount;
    /* 0ea */ u8 mPageScroll;
    /* 0eb */ PAD(0x0eb, 0x0ec);
    /* 0ec */ u32 mPageSliding;
    /* 0f0 */ u32 mSelectingPage;
    /* 0f4 */ u8 mFirstPage;
    /* 0f5 */ u8 mUnk_f5;
    /* 0f6 */ PAD(0x0f6, 0x0f8);
    /* 0f8 */ s16 mRowSlideY;
    // mCursorItem - mScroll is the visible row (ListScene.cpp:1874); follows mTargetItem, the item the input moved to
    // (:2219-2221)
    /* 0fa */ u8 mCursorItem;
    /* 0fb */ s8 mTargetItem;
    /* 0fc */ u8 mItemCount; // BuildList sets 2, with no ids, for page 0 (mPage == 0) of modes 3 / 5 with mCharacterId 0x15
    /* 0fd */ u8 mScroll;
    /* 0fe */ PAD(0x0fe, 0x100);
    /* 100 */ u32 mListScrolling;
    /* 104 */ u8 mListFilter;
    /* 105 */ PAD(0x105, 0x106);
    /* 106 */ u16 mItemIds[0x40];
    /* 186 */ u8 mSavedCursorItems[13];
    /* 193 */ PAD(0x193, 0x194);
    /* 194 */ u32 mDirtyFlags[3];
    /* 1a0 */ u8 mMainScreenState;
    /* 1a1 */ u8 mTextKind;
    /* 1a2 */ struct {
        u16 mCount;
        u16 mTotal;
    } mGalleryCounts[14];
    /* 1da */ PAD(0x1da, 0x1dc);
    /* 1dc */ u32 mPrice;
    /* 1e0 */ UnkStruct_02076058 *mUnk_1e0;
    /* 1e4 */ UnkStruct_02071d24 *mUnk_1e4;
    /* 1e8 */ UnkObject_02078e14 *mAntenna;
    /* 1ec */ MessageWindow *mMessageWindow;
    /* 1f0 */ u8 mPlayer;
    /* 1f1 */ u8 mCharacterId;
    /* 1f2 */ u16 mPickList[0x100];
    /* 3f2 */ u8 mPickCount;
    /* 3f3 */ PAD(0x3f3, 0x3f4);
    /* 3f4 */ u32 mFirstSlotOnly;
    /* 3f8 */ u8 mAppliedWndMode;
    /* 3f9 */ u8 mWndMode;
    /* 3fa */ u8 mListWindowCell0AnimTimer;
    /* 3fb */ PAD(0x3fb, 0x3fc);
    /* 3fc */ s16 mMoveY;
    /* 3fe */ s8 mMoveYPerFrame;
    /* 3ff */ u8 mItemMoving;
    /* 400 */ u8 mCommandListClosed;
    /* 401 */ u8 mCommandPageClosed;
    /* 402 */ PAD(0x402, 0x404);
    /* 404 */ s32 mPendingPlanes;
    /* 408 */

    ListScene(u32 unk1, u32 unk2);
    virtual ~ListScene();
    virtual void Update();
    void Release(u32 releaseCommands);
    // Only when mWndMode changed; also sets the sub screen's blend brightness (-8 in modes 1 / 2); mode 0 turns the
    // windows off.
    void ApplyWndMode();
    void InitDisplay();
    void LoadMessages(); // also copies a u16 of mes_Alphabet.mes into mAlphabetChar
    void PrintRows();
    void ClearSubScreenText();
    void DrawSubScreenText(); // also steps mMessageWindow; nothing while mTextKind is set
    void UpdateMainScreen();
    void UpdateListWindow();
    void UpdatePages();
    void DrawMainScreen();
    void DrawWindows();
    void DrawRows();
    void DrawItemNumberAndCount();
    void DrawMoneyFilterButtonsAndCursor();
    // GetEntryPrice uses it (5000 when 0), scaled by sPricePercents[entry mUnk_00_0] and reduced by the entry's mGauge
    u32 GetPrice(u32 unk1, u32 unk2);
    u32 GetEntryPrice(u32 unk1, u32 unk2);
    void DrawMoneyAmount(u32 unk1, u32 unk2);
    BOOL IsInSlots(u32 unk1, u32 unk2, u32 unk3);
    void DrawPagesAndMovingItem();
    void DrawBakuganMark(u8 unk1, u32 unk2, u32 unk3, s16 unk4, s16 unk5);
    void DrawSmallCard(u8 unk1, u8 unk2, u32 unk3, s16 unk4, s16 unk5);
    s32 func_0208644c();
    u8 GetRatioFrame(u32 unk1);
    void DrawMainScreenText();
    void UpdateListScroll();
    void LoadBakuganMark(u32 unk1, u32 unk2, u32 unk3);
    void LoadSmallCard(u32 unk1, u32 unk2, u32 unk3);
    void LoadRowGraphics();
    void RefreshCommandList();
    void RefreshCommandPage();
    // unk1 >= 0 starts that entry; < 0: modes 3 / 5 start entry 6, mCursorItem + 7 or mPage + 8, the other modes do
    // nothing
    void SetMessage(s32 unk1);
    void ApplyTargetPage();
    BOOL ShouldApplyTargetPage();
    BOOL IsCommandListOrPageSettingUp();
    void SetItemFlags(u32 unk1, u32 unk2, u32 unk3);
    void LoadSlotGraphics();
    BOOL func_02080b48(u32 unk1, u32 unk2);
    void ReleaseMainScreenGraphics();
    void SetupMainScreenForPage();
    void ClearMainScreenText();
    void StoreSlotsInSaveData();
    void func_020851d0();
    void DrawPromptWindowContents();
    void InitMainScreenText();
    void InitAndLoadMainScreen();
    u32 func_02086548(u32 unk1, u32 unk2);
    u32 func_02086570(u32 unk1, u32 unk2);
    void PrintBakuganRow(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    void PrintCardNameShort(u32 unk1, u32 unk2);
    void SetupPagesAndList();
    void ReadSlots();
    void BuildList(u32 unk1);
    void RefreshCounts();
    void LoadSubScreenGraphics(); // with data_020d433c.mWireless set also creates mAntenna (AntennaIndicator layout 3) or re-lays it
    void UpdateListInput();
};

// Free functions taking the object, defined in ListScene.cpp after the class functions.
void SetupListTextKind(ListScene *self, u32 mode);
void ResetCursorBakuganEntry(ListScene *self);
void func_020843ac(ListScene *self, u32 unk1);
void CopySlotsToFirstPlayer(ListScene *self);
BOOL func_02084a10(ListScene *self);
BOOL func_02084c38(ListScene *self);
BOOL func_02084e64(ListScene *self); // self is passed but unused
BOOL func_02084f40(ListScene *self); // self is passed but unused
BOOL func_0208501c(ListScene *self);
void SetupDeckTextKind(ListScene *self, u32 mode);
void PickSlots(ListScene *self);
