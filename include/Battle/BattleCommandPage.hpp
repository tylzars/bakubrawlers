#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"
#include "UI/SelectWindow.hpp"

class CellAnimation;
class CellAnimFile;
class SelectWindow;

// Page of the battle command screen (0x10c bytes, ctor 02054df0, vtable 020b2dc8): shows the player/character
// icons and a SelectWindow of the pages.
class BattleCommandPage : public SceneBase {
public:
    /* 0c */ void *mNceData[3];
    /* 18 */ CellAnimFile *mCellFiles[3];
    /* 24 */ CellAnimation *mCellAnims[11];
    /* 50 */ SelectWindow *mWindow;
    /* 54 */ void *mCardMessages;
    /* 58 */ void *mCardTextMessages;
    /* 5c */ void *mCardNameMessages;
    /* 60 */ void *mTextBuffers[3];
    /* 6c */ void *mGlyphBuffers[3];
    /* 78 */ u32 mTextBufferSizes[3];
    /* 84 */ u32 mGlyphCounts[3];
    /* 90 */ void *mFiles[12];
    /* c0 */ u32 mFileSizes[12];
    /* f0 */ u8 mLoadStep;
    /* f1 */ u8 mState;
    /* f2 */ u8 mUnk_f2;
    /* f3 */ PAD(0xf3, 0xf4);
    /* f4 */ u16 mCardId;
    /* f6 */ u16 mNextCardId;
    /* f8 */ u16 mDrawnCardId;
    /* fa */ s8 mPlayer;
    /* fb */ u8 mTextPage;
    /* fc */ u8 mLastTextPage;
    /* fd */ u8 mWindowVisible;
    /* fe */ u8 mFourItems;
    /* ff */ PAD(0xff, 0x100);
    /* 100 */ u16 mNameWidth;
    /* 102 */ s16 mNameX;
    /* 104 */ u8 mVisible;
    /* 105 */ u8 mCloseFadeKind;
    /* 106 */ u8 mCardHidden;
    /* 107 */ u8 mHideCardRequested;
    /* 108 */ u8 mGlyphBufferIndex;
    /* 109 */ u8 mUnk_109;
    /* 10a */ u8 mUnk_10a;
    /* 10b */ PAD(0x10b, 0x10c);
    /* 10c */

    BattleCommandPage(u16 a, s32 b);
    virtual ~BattleCommandPage();
    virtual void Update();
    u16 func_02055644(u16 id);
    void CycleTextPage(u32 flag);
    BOOL IsSettingUp();
    void SetWindowVisible(u32 enable); // the gate also stops UpdateWindow; enabling does nothing while mWindow is NULL
    void UpdateWindow();
    void UpdatePageItems();
    BOOL IsPageItemEnabled(u32 right);
    void InitDisplay();
    void ResetLoadStep();
    BOOL LoadGraphics();
    BOOL LoadLargeCard();
    void LoadText();
    void PrintCardText(u32 index); // sets mTextPage = index, picks mUnk_108 and prints the card's name and text
    void DrawText();
    void ClearText();
    void DrawLargeCard();
    void DrawCard(); // player cells mCellAnims[7..10] only for mDrawnCardId < 0xd5, mCardHidden == 0, mPlayer >= 0
};

void CloseCommandPage(BattleCommandPage *page, u32 a);
void SetCommandPageCardHidden(BattleCommandPage *page, u32 a);
void SetCommandPageCard(BattleCommandPage *page, u16 a, s32 b); // also sets the page's mPlayer = b
void func_02055738(BattleCommandPage *page, u32 a, u32 b, u32 c);
void CreateCommandPageWindow(BattleCommandPage *page, u32 a, u32 b, s16 c, s16 d, s16 e, s16 f, u32 g);
