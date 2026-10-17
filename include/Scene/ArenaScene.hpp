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
    /* 00 */ CellAnimation *mAnim;
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
    /* a0 */ SelectWindow *mMenu;
    /* a4 */ SelectWindow *mSubMenu;
    /* a8 */ MessageWindow *mMessage;
    /* ac */ u8 mPlayerCount;
    /* ad */ u8 mState;
    /* ae */ u8 mTimer;
    /* af */ u8 mVariant;
    /* b0 */ u8 mResult;
    /* b1 */ u8 mSelection;
    /* b2 */ u8 mSubMenuMode;
    /* b3 */ u8 mUnk_b3;
    /* b4 */ u8 mUnk_b4;
    /* b5 */ u8 mRequest;
    /* b6 */ u8 mUnk_b6;
    /* b7 */

    ArenaScene();

    /* 00 */ virtual ~ArenaScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void func_ov001_022197ec();
    void SetSubMenuMode(u32 mode);
    void InitDisplay();
    void LoadGraphics();
    void SetupVariant();
    void RefreshSlotAnimations();
    u32 UpdateSelection();
    u32 UpdateMenu();
    void ResetMenu();
    void func_ov001_0221bee8();
    void func_ov001_0221bf40();
    void InitNameText();
    void ClearNameText();
    void PrintNames();
    void DrawNames();
    void ResetNames();
    u32 func_ov001_0221c614(u32 unk1);
    void LoadMessages();
    void PrintMessage(u32 unk1);
    void DrawMessage();
};
