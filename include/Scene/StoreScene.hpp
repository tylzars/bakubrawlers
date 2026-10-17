#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
class ScriptPlayer;
struct MessageFile;

// Overlay 3: the shop.
class StoreScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ MessageFile *mMessages;
    /* 10 */ MessageWindow *mMessageWindow;
    /* 14 */ void *mTextBuffer;
    /* 18 */ void *mGlyphBuffer;
    /* 1c */ u32 mTextBufferSize;
    /* 20 */ u32 mGlyphCount;
    /* 24 */ SelectWindow *mWindow;
    /* 28 */ ScriptPlayer *mEvent;
    /* 2c */ s16 mNextScene;
    /* 2e */ PAD(0x2e, 0x30);
    /* 30 */ CellAnimation *mCellAnims[2];
    /* 38 */

    StoreScene();

    /* 00 */ virtual ~StoreScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void Cleanup();
    void InitDisplay();
    void LoadGraphics();
    void CreateMenu();
    void UpdateMenu();
    // mInfo state 11, mScroll 0x62; the mPage item cell restarts on animation sSelectedFrames[mPage], the mSelection
    // one plays backward once; items 0-2 get flags 0x8000; button 0 on for mPage < 2, 2 for mPage >= 1; button and item
    // 4 / 5 off (flags 0x8000) on page 0 / 2.
    void StartScroll();
    void HandleMenuResult();
    BOOL ScrollStep();
    void UpdateSlotLayout();
    void SetupText();
    void DrawText();
    void PrintStatus();
    void ClearTextBuffer();
    void DrawMoney();
};
