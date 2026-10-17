#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class ParkMenu;
class SelectWindow;
class ScriptPlayer;

// Overlay 4: the park (tournament and battle lobby).
class ParkScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ u8 mEntryIndex;
    /* 0d */ u8 mState;
    /* 0e */ u8 mTimer;
    /* 0f */ u8 mUnk_0f;
    /* 10 */ u8 mPlayerCount;
    /* 11 */ u8 mWindowVisible;
    /* 12 */ u8 mRequestParkScene;
    /* 14 */ ParkMenu *mMenu;
    /* 18 */ ScriptPlayer *mEvent;
    /* 1c */ void *mUnk_1c;
    /* 20 */ void *mGlyphBuffer;
    /* 24 */ u32 mUnk_24[2];
    /* 2c */ void *mUnk_2c;
    /* 30 */ SelectWindow *mWindow;
    /* 34 */

    ParkScene();

    /* 00 */ virtual ~ParkScene() override;
    /* 08 */ virtual void Update() override; // 02219580, not decompiled yet
    /* 0c */

    void Init();                    // 0221b1d4
    void InitDisplay();             // 0221b214
    void LoadWindowGraphics();      // 0221b3f4
    void ReloadBackground();
    void LoadBackground();          // 0221b5ec
    void Cleanup();                 // 0221b74c
    void Draw();                    // 0221b7f4
    u8 func_ov004_0221b8c4(int index);
    int GetUnlockedFieldId(int index);
    void ShowWindow();              // 0221b94c
    void HideWindow();              // 0221b9d0
};
