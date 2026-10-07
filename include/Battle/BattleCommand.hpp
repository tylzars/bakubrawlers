#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
class MessageWindow;
class BattleCommandMap;
class BattleCommandPage;
class BattleCommandList;
class BattleCommandPrompt;

// Main battle command screen (battle_b_obj_00, mes_Command, union_b cockpit): the command windows, the turn
// counter and the sub objects of the current mode. Vtable 02287ad0, at least 0x13c bytes.
class BattleCommand : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ CellAnimation *mUnk_0c;
    /* 10 */ CellAnimation *mUnk_10;
    /* 14 */ CellAnimation *mUnk_14;
    /* 18 */ CellAnimation *mUnk_18;
    /* 1c */ CellAnimation *mUnk_1c;
    /* 20 */ CellAnimation *mUnk_20;
    /* 24 */ CellAnimation *mTurnDigits;
    /* 28 */ CellAnimation *mUnk_28;
    /* 2c */ CellAnimation *mUnk_2c[4];
    /* 3c */ CellAnimation *mUnk_3c[12];
    /* 6c */ SelectWindow *mUnk_6c;
    /* 70 */ SelectWindow *mUnk_70;
    /* 74 */ SelectWindow *mUnk_74;
    /* 78 */ BattleCommandPage *mUnk_78;
    /* 7c */ BattleCommandList *mUnk_7c;
    /* 80 */ u32 mUnk_80;
    /* 84 */ BattleCommandMap *mUnk_84;
    /* 88 */ MessageWindow *mMessage;
    /* 8c */ BattleCommandPrompt *mUnk_8c; // freed with func_ov007_0225e68c(obj, flag)
    /* 90 */ u8 mUnk_90[14][10];
    /* 11c */ s16 mUnk_11c;
    /* 11e */ s16 mUnk_11e;
    /* 120 */ s16 mUnk_120;
    /* 122 */ s16 mUnk_122;
    /* 124 */ u16 mUnk_124;
    /* 126 */ u16 mUnk_126;
    /* 128 */ u8 mUnk_128;
    /* 129 */ u8 mUnk_129;
    /* 12a */ u8 mMode;
    /* 12b */ u8 mNextMode;
    /* 12c */ u8 mUnk_12c;
    /* 12d */ u8 mUnk_12d;
    /* 12e */ u8 mUnk_12e;
    /* 12f */ u8 mUnk_12f;
    /* 130 */ PAD(0x130, 0x133);
    /* 133 */ u8 mUnk_133;
    /* 134 */ PAD(0x134, 0x135);
    /* 135 */ u8 mUnk_135;
    /* 136 */ s8 mUnk_136;
    /* 137 */ PAD(0x137, 0x138);
    /* 138 */ u8 mUnk_138;
    /* 139 */ u8 mUnk_139;
    /* 13a */ u8 mUnk_13a;
    /* 13b */ u8 mUnk_13b;
    /* 13c */

    BattleCommand(u32 unk);
    virtual ~BattleCommand();
    /* 08 */ virtual void Update();
    BOOL func_ov007_02239e1c(BOOL input);
    BOOL func_ov007_02239fd8();
    void func_ov007_0223ce24();
    void func_ov007_0223ce90();
    void func_ov007_0223c74c(BOOL flag);
    void func_ov007_0223c8ac();
    void func_ov007_0223c8d4(u32 flag);
    void func_ov007_0223b38c();
    void func_ov007_0223c578();
    void func_ov007_0223b874();
    void func_ov007_0223ca80();
    void func_ov007_0223b2f0();
    BOOL func_ov007_0223a1c8();
    void func_ov007_0223bb30(u32 flag); // 0xa48 bytes, not decompiled
};

// Sub objects of the modes (constructed through external constructors).
class BattleCommandMap : public SceneBase {
public:
    /* 0c */ u8 mPad[0x158 - 0xc];
    BattleCommandMap(u8 unk);
    virtual ~BattleCommandMap();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void vfunc_0c();
};
