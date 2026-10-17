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
    /* 10 */ MessageWindow *mMessage;
    /* 14 */ void *mTextBuffer;
    /* 18 */ void *mGlyphBuffer;
    /* 1c */ u32 mTextBufferSize;
    /* 20 */ u32 mGlyphCount;
    /* 24 */ SelectWindow *mMenu;
    /* 28 */ ScriptPlayer *mEvent;
    /* 2c */ s16 mUnk_2c;
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
    void func_ov003_0221ab38();
    void func_ov003_0221ac5c();
    BOOL ScrollStep();
    void UpdateSlotLayout();
    void SetupText();
    void DrawText();
    void PrintStatus();
    void ClearTextBuffer();
    void DrawMoney();
};
