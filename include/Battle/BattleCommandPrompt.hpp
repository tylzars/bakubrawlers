#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class BattleCommandPage;
class BattleCommandList;
class SelectWindow;
class CharacterIcons;
struct BattleRecord;

struct BattlePromptSlot {
    u16 mId;
    u8 mKind;
    u8 mUnk_03;
};

// 0x98 bytes, ctor 0225d08c: the prompt of the command screen (page/list of cells to pick from).
class BattleCommandPrompt : public SceneBase {
public:
    /* 0c */ CellAnimation *mBakuganSubCellAnims[3];
    /* 18 */ CellAnimation *mCursorAnim;
    /* 1c */ CellAnimation *mDigitAnim;
    /* 20 */ CellAnimation *mStateCellAnims[3];
    /* 2c */ MessageWindow *mMessageWindow;
    /* 30 */ BattleCommandPage *mCommandPage;
    /* 34 */ BattleCommandList *mCommandList;
    /* 38 */ SelectWindow *mWindow;
    /* 3c */ CharacterIcons *mUnk_3c;
    /* 40 */ CellAnimation *mCharacterIconCellAnims[3];
    /* 4c */ u32 mCharOffset;
    /* 50 */ s16 mSlideX;
    /* 52 */ u16 mCardIds[6];
    /* 5e */ u16 mNextScenes[6];
    /* 6a */ u8 mKinds[6];
    /* 70 */ BattleRecord *mRecords[6];
    /* 88 */ u16 mSlideOutDelayFrames;
    /* 8a */ u8 mState;
    /* 8b */ u8 mMode;
    /* 8c */ u8 mPlayer;
    /* 8d */ u8 mUnk_8d;
    /* 8e */ u8 mStep;
    /* 8f */ u8 mCursorPos;
    /* 90 */ u8 mScroll;
    /* 91 */ u8 mUnk_91;
    /* 92 */ u8 mCount;
    /* 93 */ u8 mCollectMode;
    /* 94 */ u8 mDisabledBits[4];
    /* 98 */

    BattleCommandPrompt(u32 mode, void *target, u32 flags, u32 charOffset, u8 player);
    virtual ~BattleCommandPrompt();
    virtual void Update();
    void Load();
    void CollectCards(const BattlePromptSlot *slot);
    void CollectRecords();
};

void CloseCommandPrompt(BattleCommandPrompt *prompt, u32 flag); // releases the prompt
extern "C" u8 func_ov007_0225f0bc(u32 v);
