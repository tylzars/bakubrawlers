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
    /* 0c */ SelectWindow *mMenu;
    /* 10 */ MessageFile *mMessages;
    /* 14 */ void *mTextBuffer;
    /* 18 */ void *mGlyphBuffer;
    /* 1c */ u32 mTextBufferSize;
    /* 20 */ u32 mGlyphCount;
    /* 24 */ u8 mUnk_24;
    /* 25 */

    ArenaWindowScene();

    /* 00 */ virtual ~ArenaWindowScene() override;
    /* 08 */ virtual void Update() override; // 0221cca0, not decompiled yet
    /* 0c */

    void func_ov001_0221d6dc();
    void func_ov001_0221d858();
    void func_ov001_0221db10();
    void func_ov001_0221dd40();
    void func_ov001_0221df64();
    void func_ov001_0221e0b4();
    void func_ov001_0221e15c();
};
