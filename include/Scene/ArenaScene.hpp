#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class SelectWindow;
struct MessageFile;
class UnkStruct_0202f420;

// A cell animation placed on screen.
struct CellSlot {
    /* 00 */ CellAnimation *mAnim;
    /* 04 */ s16 mX;
    /* 06 */ s16 mY;
    /* 08 */ s16 mUnk_08;
    /* 0a */ s16 mUnk_0a;
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
    /* 8c */ void *mUnk_8c;
    /* 90 */ void *mUnk_90;
    /* 94 */ u32 mUnk_94;
    /* 98 */ u32 mUnk_98;
    /* 9c */ UnkStruct_0202f420 *mUnk_9c;
    /* a0 */ SelectWindow *mMenu;
    /* a4 */ SelectWindow *mSubMenu;
    /* a8 */ MessageWindow *mMessage;
    /* ac */ u8 mPlayerCount;
    /* ad */ u8 mMode;
    /* ae */ u8 mUnk_ae;
    /* af */ u8 mVariant;
    /* b0 */ u8 mUnk_b0;
    /* b1 */ u8 mSelection;
    /* b2 */ u8 mSubMenuMode;
    /* b3 */ u8 mUnk_b3;
    /* b4 */ u8 mUnk_b4;
    /* b5 */ u8 mUnk_b5;
    /* b6 */

    ArenaScene();

    /* 00 */ virtual ~ArenaScene() override;
    /* 08 */ virtual void Update() override; // 02219b54, not decompiled yet
    /* 0c */

    void func_ov001_022197ec();
    void func_ov001_0221993c(u32 mode);
    void func_ov001_0221b06c();
    void func_ov001_0221b24c();
    void func_ov001_0221b994();
    void func_ov001_0221bc44();
    u32 func_ov001_0221bca0();
    u32 func_ov001_0221bdf0();
    void func_ov001_0221be1c();
    void func_ov001_0221bee8();
    void func_ov001_0221bf40();
    void func_ov001_0221c008();
    void func_ov001_0221c1a0();
    void func_ov001_0221c1d8();
    void func_ov001_0221c484();
    void func_ov001_0221c5b0();
    u32 func_ov001_0221c614(u32 unk1);
    void func_ov001_0221c710();
    void func_ov001_0221c934(u32 unk1);
    void func_ov001_0221caa0();
};
