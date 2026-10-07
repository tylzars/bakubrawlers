#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

struct BattleData;
struct BattleEntry;
struct BattleParam;
class BattleSideHelper;

// Generic polymorphic object owned by the battle scene (deleted through vtable slot 1).
class BattleObject {
public:
    virtual ~BattleObject() {}
};

// Per-player helper object (0x38 bytes, created by func_ov007_022227e4).
class BattleSlot : public BattleObject {
public:
    /* 04 */ u8 mMode;
    /* 05 */ u8 mCharacter;
    /* 06 */ u8 mUnk_06;
    /* 07 */ u8 mUnk_07;
    /* 08 */ u16 mUnk_08;
    /* 0a */ u16 mUnk_0a;
    /* 0c */ s16 mUnk_0c;
    /* 0e */ u16 mUnk_0e;
    /* 10 */ u16 mUnk_10;
    /* 12 */ u16 mUnk_12;
    union {
        /* 14 */ u8 mUnk_14;
        struct {
            /* 14 */ u8 mUnk_14_0 : 1;
            /* 14 */ u8 mUnk_14_1 : 1;
            /* 14 */ u8 mUnk_14_2 : 1;
        };
    };
    /* 15 */ u8 mUnk_15;
    /* 16 */ u8 mUnk_16;
    /* 17 */ u8 mIndex;
    /* 18 */ int mRole;
    /* 1c */ u16 mUnk_1c;
    /* 1e */ u8 mUnk_1e;
    /* 20 */ BattleData *mData;
    /* 24 */ u8 *mUnk_24;
    /* 28 */ u8 *mUnk_28;
    /* 2c */ u8 *mUnk_2c;
    /* 30 */ BattleParam *mParam;
    /* 34 */ BattleSideHelper *mHelper;
    /* 38 */

    BattleSlot(BattleData *data, u8 index, u8 mode, u8 unk4, u8 unk5, u8 unk6, BattleParam *param, u8 unk8);
    ~BattleSlot();
    int func_ov007_02223260(u16 *a, u16 *b, u16 *c, s8 *choice, u8 n, u8 *d);
    BOOL func_ov007_02222bac();
    int func_ov007_02222c34(u8 n, u16 *a, u16 *b);
    u32 func_ov007_022230b4(u16 value, u8 n);
    void func_ov007_02223484(int sel);
};

u32 func_ov007_022234f0(int player, u8 mode, u8 ch);

// Per-side helper object (0x14 bytes, created by func_ov007_02223534).
class BattleSideHelper : public BattleObject {
public:
    /* 04 */ u8 mUnk_04;
    /* 05 */ u8 mUnk_05;
    /* 06 */ u8 mUnk_06;
    /* 07 */ u8 mUnk_07;
    /* 08 */ u16 mUnk_08;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mUnk_0d;
    /* 0e */ u16 mUnk_0e;
    /* 10 */ u8 mUnk_10[4];
    /* 14 */

    BattleSideHelper(u8 index, u8 unk2, BattleEntry *entries);
    BOOL func_ov007_02223638(int side, u8 n);
};

// Interface base with vtable 02286c5c (slot 0: empty, slot 1: delete this).
class IBattle {
public:
    IBattle();
    virtual ~IBattle();
};

// One 0x14-byte player/slot record inside BattleData (array starts at +0x0c).
struct BattleEntry {
    /* 00 */ PAD(0x00, 0x0c);
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mUnk_0d_0 : 4;
    /* 0d */ u8 mUnk_0d_4 : 4;
    /* 0e */ PAD(0x0e, 0x0f);
    /* 0f */ u8 mUnk_0f_0 : 1;
    /* 0f */ u8 mUnk_0f_1 : 1;
    /* 0f */ u8 mUnk_0f_2 : 1;
    /* 0f */ u8 mUnk_0f_3 : 1;
    /* 0f */ u8 mUnk_0f_4 : 1;
    /* 0f */ u8 mUnk_0f_5 : 1;
    /* 0f */ u8 mUnk_0f_6 : 1;
    /* 0f */ u8 mUnk_0f_7 : 1;
    /* 10 */ u8 mUnk_10;
    /* 11 */ u8 mUnk_11;
    /* 12 */ u16 mUnk_12;
    /* 14 */
};

struct BattleData {
    /* 00 */ PAD(0x00, 0x0c);
    /* 0c */ BattleEntry mEntries[2];
    /* 34 */
};

// 12-byte per-mode parameter record (table at 0x02278b20).
struct BattleParam {
    /* 00 */ u8 mUnk_00;
    /* 01 */ PAD(0x01, 0x02);
    /* 02 */ u16 mUnk_02;
    /* 04 */ u32 mUnk_04;
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */
};
extern BattleParam data_ov007_02278b20[];

class CellAnimation;

// Confrontation screen overlay (0x80 bytes): the two players' emblems and gauges on the sub screen.
class BattleSub48 : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ u8 mUnk_0c; // state: 0 idle, 1 slide in (update), 2 slide out, 3 hold, 4 finished
    /* 0d */ u8 mUnk_0d; // sub-state of the animation started by func_ov007_02224a70
    union {
        /* 0e */ u8 mUnk_0e;
        struct {
            /* 0e */ u8 mUnk_0e_0 : 1;
            /* 0e */ u8 mUnk_0e_1 : 1;
            /* 0e */ u8 mUnk_0e_2 : 1;
            /* 0e */ u8 mUnk_0e_3 : 1;
            /* 0e */ u8 mUnk_0e_4 : 1;
        };
    };
    /* 0f */ u8 mUnk_0f;
    /* 10 */ PAD(0x10, 0x12);
    /* 12 */ u8 mUnk_12[2];
    /* 14 */ u16 mUnk_14;
    /* 16 */ u16 mUnk_16;
    /* 18 */ float mPos[2][2];
    /* 28 */ float mVel[2][2];
    /* 38 */ s16 mUnk_38;
    /* 3a */ s16 mUnk_3a;
    /* 3c */ int mUnk_3c;
    /* 40 */ BattleData *mUnk_40;
    /* 44 */ CellAnimation *mAnims[15];
    /* 80 */

    BattleSub48(BattleData *data, u8 mode);
    virtual ~BattleSub48();
    virtual void Update();
    void func_ov007_02224270();
    void func_ov007_02224444();
    u8 func_ov007_02224a70();
    void func_ov007_02224c90();
    void func_ov007_02224ef8();
    void func_ov007_02224fdc();
};

// 0x40 object (0x54 bytes), deleted through its vtable.
class BattleSub40 : public BattleObject {
public:
    /* 04 */ PAD(0x04, 0x54);

    BattleSub40(BattleData *data, int unk2);
};

struct BattleSub44 {
    /* 000 */ PAD(0x000, 0x13);
    /* 013 */ u8 mUnk_13;
    /* 014 */ PAD(0x014, 0x22);
    /* 022 */ u8 mUnk_22;
    /* 023 */ u8 mUnk_23;
    /* 024 */ PAD(0x024, 0x117);
    /* 117 */ u8 mUnk_117;
    /* 118 */ PAD(0x118, 0x12c);
    /* 12c */ u16 mUnk_12c;
    /* 12e */ u16 mUnk_12e;
    /* 130 */

    BattleSub44(BattleData *data, BattleParam *param, u8 mode, s8 *choice);
    void func_ov007_02227490(int mode, s32 value, s32 count, bool boost, bool useLevel);
    void func_ov007_022275c0(int mode, s16 x, s16 y, u8 flag);
    u8 func_ov007_022275a0(u16 value, u8 a, u8 b);
};

// Battle scene base: IBattle at +0, SceneBase at +4. Derived classes are the per-field battle screens.
class BattleBase : public IBattle, public SceneBase {
public:
    /* 00 IBattle */
    /* 04 SceneBase */
    /* 10 */ u8 mUnk_10;
    /* 11 */ u8 mUnk_11;
    /* 12 */ u8 mUnk_12;
    /* 13 */ u8 mUnk_13;
    /* 14 */ s8 mUnk_14;
    /* 15 */ u8 mUnk_15;
    /* 16 */ u16 mUnk_16;
    /* 18 */ u8 mUnk_18_0 : 1;
    /* 18 */ u8 mUnk_18_1 : 1;
    /* 18 */ u8 mUnk_18_2 : 1;
    /* 19 */ u8 mUnk_19;
    /* 1a */ u8 mUnk_1a;
    /* 1b */ u8 mUnk_1b;
    /* 1c */ s8 mUnk_1c;
    /* 1d */ s8 mUnk_1d;
    /* 1e */ s8 mUnk_1e;
    /* 1f */ s8 mUnk_1f;
    /* 20 */ BattleParam *mUnk_20;
    /* 24 */ u8 mUnk_24;
    /* 25 */ u8 mUnk_25;
    /* 26 */ s16 mUnk_26;
    /* 28 */ u8 mUnk_28;
    /* 29 */ u8 mUnk_29;
    /* 2a */ u16 mUnk_2a;
    /* 2c */ BattleData *mUnk_2c;
    /* 30 */ BattleSlot *mSlots[4];
    /* 40 */ BattleSub40 *mUnk_40;
    /* 44 */ BattleSub44 *mUnk_44;
    /* 48 */ BattleSub48 *mUnk_48;
    /* 4c */ u8 mUnk_4c;
    /* 4d */ u8 mUnk_4d;
    /* 4e */ u8 mUnk_4e;
    /* 4f */ u8 mUnk_4f;
    /* 50 */ u8 mUnk_50;
    /* 51 */ u8 mUnk_51;
    /* 52 */ u8 mUnk_52[2];
    /* 54 */ u8 mUnk_54_0 : 1;
    /* 54 */ u8 mUnk_54_1 : 1;
    /* 54 */ u8 mUnk_54_2 : 1;
    /* 54 */ u8 mUnk_54_3 : 1;
    /* 54 */ u8 mUnk_54_4 : 1;
    /* 54 */ u8 mUnk_54_5 : 1;
    /* 55 */ u8 mUnk_55;
    /* 56 */ u16 mUnk_56;
    /* 58 */ u32 mUnk_58;
    /* 5c */ u32 mUnk_5c;
    /* 60 */ BattleSideHelper *mHelpers[2];
    /* 68 */

    // Players 2 and 3 only watch (no touch input). Kept as BOOL: callers test it as a byte.
    BOOL IsSpectator() const {
        return mUnk_12 >= 2;
    }

    BattleBase();
    virtual ~BattleBase();
    /* 08 */ virtual int func_ov007_0221cebc(int a);
    /* 0c */ virtual void func_ov007_0221ced0(u16 *a, u16 *b, u16 *c);
    /* 10 */ virtual void func_ov007_0221cfac(u16 *a, u16 *b);
    /* 14 */ virtual int func_ov007_0221d494(u16 *a, u16 *b, u16 *c, u8 *d);
    /* 18 */ virtual void func_ov007_0221d488(u16 *p);
    /* 1c */ virtual void func_ov007_0221d088(int a, u16 *counts);
    /* 20 */ virtual void Update();
    /* 24 */ virtual void func_ov007_02219888(BattleData *data, u32 mode);
    /* 28 */ virtual void vfunc_28() = 0;
    /* 2c */ virtual void func_ov007_0221a624();
    /* 30 */ virtual BOOL func_ov007_0221bd84();
    /* 34 */ virtual void vfunc_34() = 0;
    /* 38 */ virtual BOOL func_ov007_0221bfa4();
    /* 3c */ virtual void vfunc_3c() = 0;
    /* 40 */ virtual u8 vfunc_40(int fast) = 0;
    /* 44 */ virtual void vfunc_44(int index, u16 count);

    BOOL func_ov007_0221a664();
    void func_ov007_0221bb28(s16 a, s16 b);
    void func_ov007_0221bb3c();
    void func_ov007_0221bb68();
    void func_ov007_0221c8f8(u16 *p);
    int func_ov007_0221c850(u16 *a, u16 *b, u16 *c, u8 *d);
    void func_ov007_0221a55c();
    void func_ov007_0221c938();
    int func_ov007_0221c178(u16 *a, u16 *b, u16 *c, u8 *flag);
    BOOL func_ov007_0221d398();
    void func_ov007_0221a6b4();
    void func_ov007_0221b8e8();
    BOOL func_ov007_0221bb84();
};

BOOL func_ov007_0221b998(int unk1, int mode, BattleEntry *entry);
BOOL func_ov007_0221b9d0(int unk1, int mode, BattleEntry *entry);
BOOL func_ov007_0221ba0c(int unk1, int mode, BattleEntry *entry);
BOOL func_ov007_0221ba38(int unk1, u8 side, int value, BattleEntry *entries);
int func_ov007_0221ba88(int unk1, int index, BattleEntry *entries);
u32 func_ov007_0221d348(u32 max);
int func_ov007_02222764(int side, BattleEntry *entry);
