#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

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
    /* 11c */ u16 mUnk_11c;
    /* 11e */ u16 mUnk_11e;
    /* 120 */ s16 mUnk_120;
    /* 122 */ s16 mUnk_122;
    /* 124 */ s16 mUnk_124;
    /* 126 */ s16 mUnk_126;
    /* 128 */ u8 mUnk_128;
    /* 129 */ u8 mUnk_129;
    /* 12a */ u8 mMode;
    /* 12b */ u8 mNextMode;
    /* 12c */ u8 mUnk_12c;
    /* 12d */ u8 mUnk_12d;
    /* 12e */ u8 mUnk_12e;
    /* 12f */ u8 mUnk_12f;
    /* 130 */ u8 mUnk_130;
    /* 131 */ PAD(0x131, 0x132);
    /* 132 */ u8 mUnk_132;
    /* 133 */ u8 mUnk_133;
    /* 134 */ u8 mUnk_134;
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
    void func_ov007_0223a734();
    void func_ov007_0223c578();
    void func_ov007_0223b874();
    void func_ov007_0223ca80();
    void func_ov007_0223b2f0();
    BOOL func_ov007_0223a1c8();
    void func_ov007_0223bb30(u32 flag); // 0xa48 bytes, not decompiled
};

class BattleMapObj;
class BattleMapModel;
class BakuganModel;
class ModelLightGroup;
class BattleStatusPanels;
struct BattleUnk29c;
class BattleMapPiece;

// Battle map of the command screen (vtable 022887e0, 0x158 bytes): the map object, cameras, models of the field,
// the status panels and the objects of the battle.
class BattleCommandMap : public SceneBase {
public:
    /* 0c */ void *mUnk_0c;
    /* 10 */ BattleMapObj *mUnk_10;
    /* 14 */ BakuganModel *mUnk_14;
    /* 18 */ void *mUnk_18[12];
    /* 48 */ BattleMapModel *mUnk_48;
    /* 4c */ BattleMapModel *mUnk_4c;
    /* 50 */ BattleMapModel *mUnk_50[4];
    /* 60 */ ModelLightGroup *mUnk_60;
    /* 64 */ BattleMapPiece *mUnk_64;
    /* 68 */ BattleMapPiece *mUnk_68;
    /* 6c */ BattleMapPiece *mUnk_6c;
    /* 70 */ BattleStatusPanels *mUnk_70;
    /* 74 */ BattleUnk29c *mUnk_74;
    /* 78 */ u8 mUnk_78[0x60];
    /* d8 */ u32 mUnk_d8[3];
    /* e4 */ u32 mUnk_e4[3];
    /* f0 */ u32 mUnk_f0[3];
    /* fc */ u32 mUnk_fc[3];
    /* 108 */ u32 mUnk_108[3];
    /* 114 */ u32 mUnk_114[3];
    /* 120 */ u32 mUnk_120[3];
    /* 12c */ u32 mUnk_12c;
    /* 130 */ s32 mUnk_130;
    /* 134 */ float mUnk_134;
    /* 138 */ s32 mUnk_138;
    /* 13c */ u16 mUnk_13c;
    /* 13e */ u16 mUnk_13e;
    /* 140 */ u16 mUnk_140;
    /* 142 */ u16 mUnk_142;
    /* 144 */ u16 mUnk_144;
    /* 146 */ u16 mUnk_146;
    /* 148 */ u8 mUnk_148;
    /* 149 */ u8 mUnk_149;
    /* 14a */ u8 mState;
    /* 14b */ u8 mUnk_14b;
    /* 14c */ u8 mUnk_14c;
    /* 14d */ u8 mUnk_14d;
    /* 14e */ PAD(0x14e, 0x14f);
    /* 14f */ u8 mUnk_14f;
    /* 150 */ u8 mUnk_150;
    /* 151 */ u8 mUnk_151;
    /* 152 */ u8 mUnk_152;
    /* 153 */ u8 mUnk_153;
    /* 154 */ u8 mUnk_154;
    /* 155 */ u8 mUnk_155;
    /* 156 */ PAD(0x156, 0x158);
    /* 158 */

    BattleCommandMap(u8 unk);
    virtual ~BattleCommandMap();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void vfunc_0c();
    void func_ov007_02245594();
    void func_ov007_022455c0();
    void func_ov007_0224560c(u32 index);
    void func_ov007_02245894();
    void func_ov007_022458cc();
    void func_ov007_022458f4();
    void func_ov007_02245900();
    void func_ov007_02245b30();
    void func_ov007_02245e58();
    void func_ov007_02245f08();
    void func_ov007_02246450();
};

// Helpers of the map object implemented in BattleCommandMap.cpp (linkage as defined there).
void func_ov007_0224587c(BattleCommandMap *map);
extern "C" void func_ov007_022479f4(BattleCommandMap *map, u32 a);
void func_ov007_02247a3c(BattleCommandMap *map);
extern "C" void func_ov007_02248620(BattleCommandMap *map, u32 flag);
extern "C" void func_ov007_02248748(BattleCommandMap *map, VecFx32 *out);
void func_ov007_02248944(VecFx32 *out, u8 a, u8 b);
extern "C" u32 func_ov007_02248998(VecFx32 *v, u8 *outA, u8 *outB);

void func_ov007_0223ad70(u32 variant); // loads the battle background
