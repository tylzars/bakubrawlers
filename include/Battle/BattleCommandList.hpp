#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"
#include "UI/SelectWindow.hpp"

class CellAnimation;
class CellAnimFile;
class ModelLightGroup;
class Camera;
class BakuganModel;
struct BattleCellPair;

// A value at 0x0c of BattleCommandList that func_0205a804 steps towards its target by one per frame.
struct UnkStruct_BattleCommandList_0c {
    /* 0 */ u8 mCur;
    /* 1 */ u8 mTarget;
    /* 2 */
};

// List page of the battle command screen (0x168 bytes, ctor 020575e0, vtable 020b3298).
class BattleCommandList : public SceneBase {
public:
    /* 0c */ UnkStruct_BattleCommandList_0c mUnk_0c[5];
    /* 16 */ u8 mUnk_16[5];
    /* 1b */ PAD(0x1b, 0x1c);
    /* 1c */ void *mUnk_1c[4];
    /* 2c */ CellAnimFile *mUnk_2c[4];
    /* 3c */ CellAnimation *mCells3c[7];
    /* 58 */ CellAnimation *mCells58[8];
    /* 78 */ void *mUnk_78;
    /* 7c */ void *mUnk_7c;
    /* 80 */ void *mUnk_80;
    /* 84 */ void *mUnk_84;
    /* 88 */ ModelLightGroup *mUnk_88;
    /* 8c */ SelectWindow *mUnk_8c;
    /* 90 */ BakuganModel *mUnk_90;
    /* 94 */ Camera *mUnk_94;
    /* 98 */ u16 mUnk_98;
    /* 9a */ u16 mUnk_9a;
    /* 9c */ u16 mUnk_9c;
    /* 9e */ PAD(0x9e, 0xa0);
    /* a0 */ void *mUnk_a0[3];
    /* ac */ void *mUnk_ac[3];
    /* b8 */ u32 mUnk_b8[3];
    /* c4 */ u32 mUnk_c4[3];
    /* d0 */ u32 mUnk_d0[14];
    /* 108 */ u32 mUnk_108[14];
    /* 140 */ u8 mUnk_140;
    /* 141 */ PAD(0x141, 0x142);
    /* 142 */ u16 mUnk_142[6];
    /* 14e */ u8 mUnk_14e;
    /* 14f */ u8 mUnk_14f;
    /* 150 */ u8 mUnk_150;
    /* 151 */ u8 mUnk_151;
    /* 152 */ s8 mUnk_152;
    /* 153 */ PAD(0x153, 0x154);
    /* 154 */ u16 mUnk_154;
    /* 156 */ u8 mUnk_156;
    /* 157 */ u8 mUnk_157;
    /* 158 */ u8 mUnk_158;
    /* 159 */ u8 mUnk_159;
    /* 15a */ u8 mUnk_15a;
    /* 15b */ u8 mUnk_15b;
    /* 15c */ u8 mUnk_15c;
    /* 15d */ u8 mUnk_15d;
    /* 15e */ u8 mUnk_15e;
    /* 15f */ u8 mUnk_15f;
    /* 160 */ u8 mUnk_160;
    /* 161 */ u8 mUnk_161;
    /* 162 */ u8 mUnk_162;
    /* 163 */ u8 mUnk_163;
    /* 164 */ u8 mUnk_164;
    /* 165 */ u8 mUnk_165;
    /* 166 */ u16 mUnk_166;
    /* 168 */

    BattleCommandList(u8 a, u8 b, s32 c, u8 d);
    virtual ~BattleCommandList();
    virtual void Update();
    void func_02058240(u32 value);
    void func_0205839c(u32 flag);
    BOOL func_020583f0();
    void func_02058530(u32 enable);
    void func_02058558();
    void func_020585e0();
    BOOL func_02058774(u32 right);
    void func_020587b4();
    void func_020589e8();
    BOOL func_020589f4();
    BOOL func_02059734();
    void func_020597d0();
    void func_02059c50(u8 index);
    void func_0205a6a8();
    void func_0205a774();
    void func_0205a7ac();
    void func_0205a804();
    void func_0205aea4();
    void func_0205af8c();
    void func_0205b034(u32 index, u32 value);
    void func_0205b048(u32 value);
    BOOL func_0205b0c4();
    void func_0205b0dc(const BattleCellPair *pair);
};

// 4 byte record of a battle command cell: a 4 bit value and five 3 bit values.
struct BattleCellPair {
    union {
        /* 0 */ u16 mUnk_00;
        struct {
            u16 mUnk_00_0 : 4;
            u16 mUnk_00_4 : 12;
        };
    };
    union {
        /* 2 */ u16 mUnk_02;
        struct {
            u16 mUnk_02_0 : 3;
            u16 mUnk_02_3 : 3;
            u16 mUnk_02_6 : 3;
            u16 mUnk_02_9 : 3;
            u16 mUnk_02_12 : 3;
            u16 mUnk_02_15 : 1;
        };
    };
};

void func_0205afb4(BattleCommandList *list, u32 a, u32 b, BattleCellPair *pair);
void func_0205b1c4(u32 index, u32 unk2);

void func_0205821c(BattleCommandList *list);
void func_02058228(BattleCommandList *list, u32 a);
void func_02058248(BattleCommandList *list, u8 b);
void func_02058260(BattleCommandList *list, u8 a, u8 b, s32 c);
void func_02058404(BattleCommandList *list, u32 a, u32 b, s16 c, s16 d, s16 e, s16 f, u32 g);
