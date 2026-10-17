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
    /* 1c */ CellAnimation *mDigitCellAnim;
    /* 20 */ CellAnimation *mStateCellAnims[3];
    /* 2c */ MessageWindow *mMessageWindow;
    /* 30 */ BattleCommandPage *mCommandPage;
    /* 34 */ BattleCommandList *mCommandList;
    /* 38 */ SelectWindow *mWindow;
    /* 3c */ CharacterIcons *mCharacterIcons;
    /* 40 */ CellAnimation *mCharacterIconCellAnims[3];
    /* 4c */ u32 mCharOffset;
    /* 50 */ s16 mSlideX;
    /* 52 */ u16 mCardIds[6];
    /* 5e */ u16 mNextScenes[6]; // entry index | 0x100 (card) or 0x200 (record) | player << 10, not a SceneRunner id
    /* 6a */ u8 mKinds[6]; // the slot's mKind; for records BattleRecord::mStatus
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
    /* 93 */ u8 mCollectMode; // 0 only kind-0 slots / free records, 1 every slot, 2 every slot with 0 (0xd6 in mode 7) for kinds other than 2
    /* 94 */ u8 mDisabledBits[4];
    /* 98 */

    BattleCommandPrompt(u32 mode, void *target, u32 flags, u32 charOffset, u8 player);
    virtual ~BattleCommandPrompt();
    virtual void Update();
    void Load();
    void CollectCards(const BattlePromptSlot *slot); // then shows the cursor's card on mCommandPage
    // Then shows the cursor's record on mCommandList; also sets mCommandList->mUnk_161 when a Bakugan id 5 / 6 record
    // has mStatus 1.
    void CollectRecords();
};

void CloseCommandPrompt(BattleCommandPrompt *prompt, u32 flag); // state 7: the prompt ends on its next Update; flag is unused
extern "C" u8 GetLowByte(u32 v);
