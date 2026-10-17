#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class SelectWindow;

// Popup scene with a message from mes_command and two or three buttons (vtable 0228b008, 0x1c bytes). The window
// slides in over the BG, waits for a button (or START) and requests the scene stored in mNextScene.
class BattleUnk0227532c : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ SelectWindow *mWindow;
    /* 10 */ void *mGlyphBuffer;
    /* 14 */ u16 mGlyphCount;
    /* 16 */ s16 mOffset; // slide position, -0x100 off screen
    /* 18 */ u8 mState;
    /* 19 */ u8 mNextScene;
    /* 1a */ u8 mPlanes; // visible planes of the sub screen engine kept while the popup is open
    /* 1b */ PAD(0x1b, 0x1c);
    /* 1c */

    BattleUnk0227532c(u8 planes);
    virtual ~BattleUnk0227532c();
    /* 08 */ virtual void Update();
    void LoadWindow();
    BOOL SlideTo(int target);
};

void func_ov007_022758a4(BattleUnk0227532c *popup); // frame callback: slide offset and visible planes
