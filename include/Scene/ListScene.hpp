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
    /* 04 */ u8 mFinished;
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
    /* 024 */ u16 mAlphabetChar; // the u16 at byte 0xb4 of entry 0's text in mes_Alphabet.mes
    /* 026 */ PAD(0x026, 0x028);
    /* 028 */ void *mTextBuffers[2];
    /* 030 */ void *mGlyphBuffers[2];
    /* 038 */ u32 mTextBufferSizes[2]; // buffer sizes
    /* 040 */ u32 mGlyphCounts[2]; // glyph counts
    /* 048 */ SelectWindow *mPromptWindow;
    /* 04c */ SelectWindow *mListWindow;
    /* 050 */ SelectWindow *mDeckWindow; // mode 5 only; named after its /Game/Deck/ cells
    /* 054 */ SelectWindowCursor *mPageAndRowCursor;
    /* 058 */ SelectWindowCursor *mAreaCursor;
    /* 05c */ CellAnimation *mMainScreenCellAnims[6];
    /* 074 */ CellAnimation *mSubScreenCellAnims[19];
    /* 0c0 */ BattleCommandList *mCommandList;
    /* 0c4 */ BattleCommandPage *mCommandPage;
    // [0..2] the slot bakugan ids, [9..11] their attributes, [3..5] / [6..8] the card ids of mSlots54 / mSlots6c
    // (BattlePlayerSlot::mValue). Page p writes entry p - 1, and below page 4 the attribute p + 8
    /* 0c8 */ u16 mSlotValues[12];
    /* 0e0 */ // (the u8 fields below start here)
    /* 0e0 */ u8 mMode;
    /* 0e1 */ u8 mState;
    /* 0e2 */ u8 mListWindowResult;
    /* 0e3 */ PAD(0x0e3, 0x0e4);
    /* 0e4 */ s16 mPageSlideX;
    /* 0e6 */ u8 mPage;
    /* 0e7 */ s8 mTargetPage;
    /* 0e8 */ u8 mCursorPage; // equal to mPageScroll at every write; the page line draws from mPageScroll
    /* 0e9 */ u8 mPageCount;
    /* 0ea */ u8 mPageScroll;
    /* 0eb */ PAD(0x0eb, 0x0ec);
    /* 0ec */ u32 mPageSliding;
    /* 0f0 */ u32 mSelectingPage;
    /* 0f4 */ u8 mFirstPage;
    /* 0f5 */ u8 mListInputState;
    /* 0f6 */ PAD(0x0f6, 0x0f8);
    /* 0f8 */ s16 mRowSlideY;
    // mCursorItem - mScroll is the visible row (UpdateListInput); UpdateListScroll moves it to mTargetItem, the item
    // the input moved to
    /* 0fa */ u8 mCursorItem;
    /* 0fb */ s8 mTargetItem;
    /* 0fc */ u8 mItemCount; // BuildList sets 2, with no ids, for page 0 (mPage == 0) of modes 3 / 5 with mCharacterId 0x15
    /* 0fd */ u8 mScroll;
    /* 0fe */ PAD(0x0fe, 0x100);
    /* 100 */ u32 mListScrolling;
    // BuildList's filter: the attribute on bakugan pages, GetCardInfo(k, 1) on card pages. Also the attribute passed to
    // the price and entry calls, the one stored in a slot (mSlotValues[mPage + 8]) and the DrawBakuganMark /
    // DrawSmallCard frame
    /* 104 */ u8 mListFilter;
    /* 105 */ PAD(0x105, 0x106);
    /* 106 */ u16 mItemIds[0x40];
    /* 186 */ u8 mSavedCursorItems[13]; // per page; BuildList restores mCursorItem from it outside modes 3 / 5 only
    /* 193 */ PAD(0x193, 0x194);
    // set when the page, list or slots change, cleared when served: [0] UpdateListWindow's button flags, [1]
    // LoadSlotGraphics, [2] LoadRowGraphics (and PrintRows while set)
    /* 194 */ u32 mDirtyFlags[3];
    /* 1a0 */ u8 mMainScreenState;
    /* 1a1 */ u8 mTextKind;
    /* 1a2 */ struct {
        u16 mCount;
        u16 mTotal;
    } mGalleryCounts[14]; // filled by RefreshCounts (mode 4); the ctor does not set it
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
    /* 400 */ u8 mCommandListClosed; // set when UpdateMainScreen closes the list, cleared when a new list is created
    /* 401 */ u8 mCommandPageClosed; // the same for CloseCommandPage and mCommandPage
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
    u32 GetEntryPrice(u32 unk1, u32 unk2); // 10000000 when the entry's mUnk_00_0 >= 9
    void DrawMoneyAmount(u32 unk1, u32 unk2);
    BOOL IsInSlots(u32 unk1, u32 unk2, u32 unk3); // unk1 != 0: id and attribute in slots 0-2, else id in 3-8
    void DrawPagesAndMovingItem();
    void DrawBakuganMark(u8 unk1, u32 unk2, u32 unk3, s16 unk4, s16 unk5);
    void DrawSmallCard(u8 unk1, u8 unk2, u32 unk3, s16 unk4, s16 unk5);
    // Sum over the bakugan slots (only the first while mFirstSlotOnly) of the player's mRecords[i].mUnk_04, or with
    // mCharacterId 0x15 of func_020231f4 of the slot's id and attribute
    s32 func_0208644c();
    u8 GetRatioFrame(u32 unk1);
    void DrawMainScreenText();
    void UpdateListScroll();
    // nothing in mode 2, in modes 3 / 5 from page 4, in mode 4 on page 0 and from page 7; unk3 0-3 picks the VRAM
    // area (4: offset 0)
    void LoadBakuganMark(u32 unk1, u32 unk2, u32 unk3);
    // nothing in modes 0 / 1, in modes 3 / 5 on pages 1-3 unless unk3 is 1, in mode 4 below page 7
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
    void SetItemFlags(u32 unk1, u32 unk2, u32 unk3); // windows 0 / 1 / 3 also: mButtons[unk2].mEnabled = unk3 != 0x8000
    void LoadSlotGraphics(); // modes 3 / 5: the nine slot images with area 1; SetupMainScreenForPage loads area 0
    BOOL func_02080b48(u32 unk1, u32 unk2);
    void ReleaseMainScreenGraphics();
    void SetupMainScreenForPage();
    void ClearMainScreenText();
    void StoreSlotsInSaveData(); // only with mCharacterId 0x15
    void UpdateMainScreenAndDraw();
    void DrawPromptWindowContents();
    void InitMainScreenText();
    void InitAndLoadMainScreen(); // main engine registers, clears main BG / OBJ VRAM, then the files, cells and text
    u32 func_02086548(u32 unk1, u32 unk2); // sBakuganPriceEntries[GetBakuganTableIndex(unk1, unk2)].mUnk_02 * 10
    u32 func_02086570(u32 unk1, u32 unk2);
    void PrintBakuganRow(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    void PrintCardNameShort(u32 unk1, u32 unk2); // the whole text of a card row; unk2 = visible row (32 px down)
    void SetupPagesAndList();
    void ReadSlots(); // from the player's battle records and slots, or the save with mCharacterId 0x15
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
BOOL ExchangeRecords(ListScene *self);
BOOL ExchangeSlots(ListScene *self);
BOOL ExchangeRandomSeed(ListScene *self); // self is passed but unused
BOOL ExchangeCharacterLooks(ListScene *self); // self is passed but unused
BOOL ExchangeRecordHeads(ListScene *self);
void SetupDeckTextKind(ListScene *self, u32 mode);
void PickSlots(ListScene *self);
