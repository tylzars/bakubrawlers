#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class SelectWindow;
struct MessageFile;

// Overlay 1: a window showing message 11 of mes_BattleArenaMenu.
class ArenaWindowScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ SelectWindow *mWindow;
    /* 10 */ MessageFile *mMessages;
    /* 14 */ void *mTextBuffer;
    /* 18 */ void *mGlyphBuffer;
    /* 1c */ u32 mTextBufferSize;
    /* 20 */ u32 mGlyphCount;
    /* 24 */ u8 mState;
    /* 25 */

    ArenaWindowScene();

    /* 00 */ virtual ~ArenaWindowScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void InitDisplay();
    // Display registers first (main visible planes 0; sub mode 0, BG0 set up, visible planes 0x11, no window or blend),
    // then the union_b window files, LoadMessages, PrintMessage and a new two-item mWindow (disabled, selection 0).
    void SetupWindow();
    void LoadWindowGraphics();
    void LoadMessages();
    void PrintMessage();
    void DrawMessage();
    void ClearTextBuffer();
};
