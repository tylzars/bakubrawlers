#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class ScriptPlayer;
struct MessageFile;

// Overlay 8: the player's room menu (three pages of icons, each with a description line).
class MyroomScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ MessageFile *mMessages;
    /* 10 */ MessageWindow *mMessageWindow;
    /* 14 */ void *mTextBuffer;
    /* 18 */ void *mGlyphBuffer;
    /* 1c */ u32 mTextBufferSize;
    /* 20 */ u32 mGlyphCount;
    /* 24 */ SelectWindow *mWindows[3];
    /* 30 */ ScriptPlayer *mEvent;
    /* 34 */ CellAnimation *mCellAnim;
    /* 38 */

    MyroomScene(); // 0x0221ac64

    /* 00 */ virtual ~MyroomScene() override; // 0x0221ad44
    /* 08 */ virtual void Update() override;
    /* 0c */

    void Cleanup();                // 0x0221ae44
    void InitDisplay();            // 0x0221bb6c
    void LoadGraphics();           // 0x0221bd84
    void SwitchPage(u8 page);      // 0x0221c0d8
    // Reads input (window Update, LEFT / RIGHT, touch, page scroll) only in mInfo state 4; in the other states it only
    // draws the window (and mCellAnim on pages 1 / 2).
    void UpdateMenu();
    // mInfo state 5 with 0x62 left to scroll, counted in mInfo.mScrollDir (the direction is in mInfo.mPage); on page 0
    // also sets the item cells and flags and buttons 0 / 4 / 6 / 7 for the target position mInfo.mUnk_0a.
    void StartScroll();
    // mInfo.mPrevSelection 2: page 0 -> state 0xf, pages 1 / 2 -> back to page 0; 1 or 3: by the selection, a page
    // switch, state 0xc / 0xf, or a MYROOM_MENU_* value (or the literal 6 / 7) in mInfo.mScroll with state 6.
    void HandleWindowResult();
    // Page 0 only (else FALSE): moves the five item cells by up to 0xe per call out of mInfo.mScrollDir; when that
    // reaches 0, moves the selection to mInfo.mUnk_0a, re-enables arrows 6 / 7, PrintStatus and TRUE.
    BOOL UpdateScroll();
    void RefreshCellLayout();
    void SetupText();              // 0x0221d030
    void func_ov008_0221d2c0();
    void DrawText();               // 0x0221d370
    void ClearMessageBuffer();
    void PrintStatus();            // 0x0221d468
    void ClearTextBuffer();        // 0x0221d690
};
