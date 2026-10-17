#pragma once

#include "Scene/SceneBase.hpp"
#include "global.h"
#include "types.h"

class CellAnimation;

// Window that shows one message of a .mes file at a time (0x4c bytes, ctor 02087898, vtable 020baa50): loaded with
// LoadMessageFile, given a message with SetMessage (-1 clears it) and polled with func_02087cac. The battle scene
// keeps one at BattlePlayerList::mUnk_2a4, the script player one at ScriptPlayer::mStorytalkWindow.
class StorytalkWindow : public SceneBase {
public:
    /* 0c */ CellAnimation *mCellAnims[3];
    /* 18 */ void *mMessages;
    /* 1c */ void *mStoryCharaNames;
    /* 20 */ void *mTextBuffers[2];
    /* 28 */ u32 mTextBufferSizes[2];
    /* 30 */ u8 mState; // state
    /* 31 */ u8 mUnk_31;
    /* 32 */ u8 mSlideTimer;
    /* 33 */ PAD(0x33, 0x34);
    /* 34 */ s16 mMessageIndex;
    /* 36 */ u8 mUnk_36;
    /* 37 */ u8 mTextPage;
    // last page found so far: printing a page sets it to mTextPage + 1 when the text runs past that page
    // (StorytalkWindow.cpp:266-273); equal to mTextPage on the final page
    /* 38 */ u8 mLastTextPage;
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mTextOnlyX;
    /* 3b */ u8 mTextOnlyY;
    /* 3c */ u16 mPageTextTimer;
    /* 3e */ u8 mUnk_3e;
    /* 3f */ u8 mCell2Frame;
    /* 40 */ u16 mCell2X;
    /* 42 */ s16 mCharacterId;
    // set when a message index >= 0 is set (:235), cleared for -1 (:232) and when its last page is done (:121)
    /* 44 */ u8 mHasMessage;
    /* 45 */ u8 mLastPageDone;
    /* 46 */ u8 mCell1Visible;
    /* 47 */ u8 mCloseRequested;
    /* 48 */ u8 mBlink;
    /* 49 */ u8 mBlinkHidden;
    /* 4a */ u8 mKeepLastPage;
    /* 4b */ u8 mTextOnly;
    /* 4c */

    StorytalkWindow();
    virtual ~StorytalkWindow();
    virtual void Update();
    BOOL func_02087cac();
    void Close();
    void LoadMessageFile(const char *path);
    void LoadGraphics();
    void SetCell2FrameAndX(u32 unk1, u32 unk2);
    BOOL NextPage(u32 unk1);
    void SetMessage(s32 unk1, s32 unk2, u32 unk3, u32 unk4);
    void SetCell1Visible(u32 unk1);
    u32 GetPageTextTimer();
    BOOL HasMorePages();
    BOOL IsTextShown();
};
