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
    /* 10 */ MessageWindow *mMessage;
    /* 14 */ void *mTextBuffer;
    /* 18 */ void *mGlyphBuffer;
    /* 1c */ u32 mTextBufferSize;
    /* 20 */ u32 mGlyphCount;
    /* 24 */ SelectWindow *mMenus[3];
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
    void UpdateMenu();
    void StartScroll();
    void HandleWindowResult();
    BOOL UpdateScroll();
    void RefreshCellLayout();
    void SetupText();              // 0x0221d030
    void func_ov008_0221d2c0();
    void DrawText();               // 0x0221d370
    void ClearMessageBuffer();
    void PrintStatus();            // 0x0221d468
    void ClearTextBuffer();        // 0x0221d690
};
