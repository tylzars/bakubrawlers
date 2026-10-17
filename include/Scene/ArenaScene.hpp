#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
struct MessageFile;
struct SelectWindowCursor;

// A cell animation placed on screen.
struct CellSlot {
    /* 00 */ CellAnimation *mCellAnim;
    /* 04 */ s16 mX;
    /* 06 */ s16 mY;
    /* 08 */ s16 mTargetX;
    /* 0a */ s16 mTargetY;
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mVisible;
    /* 0e */ s8 mFrame;
    /* 0f */ PAD(0x0f, 0x10);
    /* 10 */
};

// Requests: the values ArenaScene passes its parent ArenaWindowScene through RequestScene (mRequest). On
// FIELD_SELECT the parent creates a FieldSelectScene; on any other non-zero value it requests SCENE_TITLE.
#define ARENA_REQUEST_FIELD_SELECT 1
#define ARENA_REQUEST_TITLE 2

// Overlay 1: battle arena selection (select_arena_*, select_wireless_*, mes_BattleArenaMenu).
class ArenaScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ CellSlot mMain;
    /* 1c */ CellSlot mSlots[3];
    /* 4c */ void *mBuffers[12];
    /* 7c */ void *mGlyphBuffer;
    /* 80 */ u32 mTextBufferSize;
    /* 84 */ u32 mGlyphCount;
    /* 88 */ MessageFile *mMessages;
    /* 8c */ void *mMessageTextBuffer;
    /* 90 */ void *mMessageGlyphBuffer;
    /* 94 */ u32 mMessageTextBufferSize;
    /* 98 */ u32 mMessageGlyphCount;
    /* 9c */ SelectWindowCursor *mCursor;
    /* a0 */ SelectWindow *mMenuWindow;
    /* a4 */ SelectWindow *mSubMenuWindow;
    /* a8 */ MessageWindow *mMessageWindow;
    /* ac */ u8 mPlayerCount;
    /* ad */ u8 mState;
    /* ae */ u8 mTimer;
    /* af */ u8 mVariant;
    /* b0 */ u8 mResult; // UpdateSelection's return (0, 1 or 2), stored in Update case 2
    /* b1 */ u8 mSelection;
    /* b2 */ u8 mSubMenuMode;
    // set by StopWirelessAndReset; while set and the wireless state byte (data_020c4258) is non-zero, Update returns
    // after its send step, drawing only the message (with mStoppingMessageVisible); cleared once that byte is 0
    /* b3 */ u8 mWirelessStopping;
    /* b4 */ u8 mStoppingMessageVisible;
    /* b5 */ u8 mRequest;
    /* b6 */ u8 mUnk_b6;
    /* b7 */

    ArenaScene();

    /* 00 */ virtual ~ArenaScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    // Nothing before state 2. Else Stop, data_020d433c.mWireless = 0, names reset, back to state 2 with mVariant 0,
    // the antenna finished, the sub-menu closed and StartFade(FADE_MODE_TO_ZERO).
    void StopWirelessAndReset();
    // No-op for the current mode. 1 / 2: seq 1 / 2, BG2 priority 0, a new SelectWindow(3 / 2, ...) as mSubMenuWindow,
    // PrintMessage(12 / 11); 0: deletes mSubMenuWindow, BG2 priority 3.
    void SetSubMenuMode(u32 mode);
    void InitDisplay();
    void LoadGraphics(); // first calls data_020d433c.mAntenna->SetupLayout(1) when it is set
    void SetupVariant();
    void RefreshSlotAnimations(); // mSlots[0] and [1] only: mFrame + 1 for the one at mSelection, else mFrame
    // 1 for cursor event 1, event 3 without a selection change, or UpdateMenu 1; 2 for UpdateMenu 2; else 0.
    u32 UpdateSelection();
    u32 UpdateMenu();
    // Runs mMenuWindow's Draw and Update(0) until its mState is <= 2 or 4, then Reset(-1), no selection, disabled and
    // both item cells back to their first animation.
    void ResetMenu();
    void func_ov001_0221bee8();
    // DrawNames for mVariant 2 / 3; eases each visible mSlots entry half way to its target and draws it on screen 1;
    // mMenuWindow->Draw(). mMessageWindow is updated by func_ov001_0221bee8.
    void func_ov001_0221bf40();
    void InitNameText(); // also clears 0x2000 bytes at 0x6600000 and calls PrintNames
    void ClearNameText();
    // Update cases 6, 9 and 10 call it every frame past their menu checks; prints the number i + 1 on all four rows,
    // the name only for i < mPlayerCount.
    void PrintNames();
    void DrawNames();
    void ResetNames();
    u32 SetMessageTypeAndCheckAll(u32 unk1);
    void LoadMessages();
    void PrintMessage(u32 unk1);
    void DrawMessage();
};
