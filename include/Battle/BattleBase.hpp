#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Scene/SceneBase.hpp"
#include "UI/MessageWindow.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"

class CellAnimation;
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
    int func_ov007_02223260(u16 *a, u16 *b, u16 *c, s8 (*choice)[2], u8 n, u8 *d);
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
    virtual ~BattleSideHelper();
    BOOL func_ov007_02223638(int side, u8 n);
};

// Interface base with vtable 02286c5c (slot 0: empty, slot 1: delete this).
class IBattle {
public:
    IBattle();
    virtual ~IBattle();
    /* 08 */ virtual void func_ov007_0221cebc(int a) = 0;
    /* 0c */ virtual void func_ov007_0221ced0(u16 *a, u16 *b, u16 *c) = 0;
    /* 10 */ virtual void func_ov007_0221cfac(u16 *a, u16 *b) = 0;
    /* 14 */ virtual int func_ov007_0221d494(u16 *a, u16 *b, u16 *c, u8 *d) = 0;
    /* 18 */ virtual void func_ov007_0221d488(u16 *p) = 0;
    /* 1c */ virtual void func_ov007_0221d088(u8 a, u16 *counts) = 0;
};

// One 0x14-byte player/slot record inside BattleData (array starts at +0x0c).
struct BattleEntry {
    /* 00 */ u16 mUnk_00; // power value compared by the gauge
    /* 02 */ u16 mUnk_02;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u16 mUnk_08;
    /* 0a */ u16 mUnk_0a;
    /* 0c */ u8 mUnk_0c;
    /* 0d */ u8 mUnk_0d_0 : 4;
    /* 0d */ u8 mUnk_0d_4 : 4;
    /* 0e */ u8 mUnk_0e;
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

// Data of a pairing of two bakugan about to battle (vtable 022881e8): per-side entries, the animations of both
// sides and the windows that show them.
struct BattleData {
    /* 00 (vtable) */
    /* 04 */ u16 mUnk_04;
    /* 06 */ u8 mUnk_06;
    /* 07 */ u8 mUnk_07;
    /* 08 */ u8 mUnk_08[2];
    /* 0a */ u8 mUnk_0a;
    /* 0b */ u8 mUnk_0b;
    /* 0c */ BattleEntry mEntries[2];
    /* 34 */ CellAnimation *mUnk_34[14];
    /* 6c */ MessageWindow *mUnk_6c;
    /* 70 */ UnkStruct_020651bc *mUnk_70;
    /* 74 */

    BattleData();
    virtual ~BattleData();
    BattleEntry *Entries() { return mEntries; }
    void func_ov007_0223d504();
    void func_ov007_0223db20(u8 side, s16 x, s16 y);
    BOOL func_ov007_0223dd6c();
    BOOL func_ov007_0223ddac(u8 side);
    void func_ov007_0223de88(u8 side, s16 x, s16 y, u32 flag, u32 value);
    void func_ov007_0223dfcc(u8 side, u8 kind, s32 x, s16 y, u32 flag, u32 mode, u16 a, u16 b);
    void func_ov007_0223e180();
    void func_ov007_0223e1a4(u32 unk, u8 side);
    void func_ov007_0223e1cc(u8 side, u8 kind);
};

// 12-byte per-mode parameter record (table at 0x02278b20).
struct BattleParam {
    /* 00 */ u8 mUnk_00;
    /* 01 */ u8 mUnk_01;
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
class BattleObj01;

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

class Camera;
class ModelInstance;
class ModelAnim;
class UnkStruct_02029ca8;
class EffectManager;

class BattleMapObjPart;
class UnkStruct_02028308;
struct BattleMapObjTable;

// Helper of the battle map scene (0x24 bytes): ctor 0x0224916c, destructor 0x02249198 (non-virtual).
// Loads the 3D model parts of the map field of the given kind (9..13: throw fields, real pop, command field; other
// kinds use the tables at 0227a2bc / 0227a2fc).
class BattleMapObj {
public:
    /* 00 */ BattleMapObjPart *mParts[8];
    /* 20 */ u8 mUnk_20; // kind
    /* 21 */ u8 mCount;  // used entries of mParts
    /* 22 */ PAD(0x22, 0x24);
    /* 24 */

    BattleMapObj(int unk1);
    ~BattleMapObj();
    void func_ov007_02249200(const char *nsbtx, const char *nsbmd, const char *data, u32 flag);
    void func_ov007_0224927c(UnkStruct_02028308 *texture, const char *nsbmd, const char *data, u32 flag);
    void func_ov007_022492f8(const BattleMapObjTable *table);
    void func_ov007_022493fc();
    void func_ov007_0224954c();
};

// A Real Bakugan model (0x10 bytes, ctor 0x0227430c), deleted through its vtable.
class BattleRealBaku : public BattleObject {
public:
    /* 04 */ u8 mId;
    /* 05 */ u8 mUnk_05;
    union {
        /* 06 */ u8 mUnk_06;
        struct {
            /* 06 */ u8 mUnk_06_0 : 3; // variant number in the file names
            /* 06 */ u8 mUnk_06_3 : 1;
            /* 06 */ u8 mUnk_06_4 : 1;
            /* 06 */ u8 mUnk_06_5 : 1;
            /* 06 */ u8 mUnk_06_6 : 1;
        };
    };
    /* 07 */ PAD(0x07, 0x08);
    /* 08 */ ModelInstance *mModel;
    /* 0c */ ModelAnim *mAnim;
    /* 10 */

    BattleRealBaku(u8 id, int unk2, u8 unk3, int unk4, int unk5, int unk6, int unk7);
    virtual ~BattleRealBaku();
    void func_ov007_02274454();
};

// Camera rows of the 3D battle field (12 bytes, table at 0x022791ec).
struct BattleCamRow {
    /* 00 */ s32 mUnk_00;
    /* 04 */ s32 mUnk_04;
    /* 08 */ s32 mUnk_08;
    /* 0c */
};

// 3D side of the battle (0x54 bytes, formerly BattleField3D): the camera animation of the map (commandfiled0.cam),
// the two Real Bakugan models and the two "aura%02d" effects. Deleted through its vtable.
class BattleField3D : public BattleObject {
public:
    union {
        /* 04 */ u8 mUnk_04;
        struct {
            /* 04 */ u8 mUnk_04_0 : 1;
            /* 04 */ u8 mUnk_04_1 : 1;
            /* 04 */ u8 mUnk_04_2 : 1;
        };
    };
    /* 05 */ PAD(0x05, 0x08);
    /* 08 */ s16 mAura[2]; // effect handles, -1 when none
    /* 0c */ s32 mUnk_0c;
    /* 10 */ s32 mUnk_10;
    /* 14 */ s32 mUnk_14;
    /* 18 */ s32 mUnk_18;
    /* 1c */ VecFx32 mPos[2];
    /* 34 */ float mUnk_34;
    /* 38 */ BattleMapObj *mUnk_38;
    /* 3c */ UnkStruct_02029ca8 *mUnk_3c;
    /* 40 */ void *mCamFile;
    /* 44 */ EffectManager *mEffects;
    /* 48 */ BattleRealBaku *mUnk_48;
    /* 4c */ BattleRealBaku *mUnk_4c;
    /* 50 */ BattleData *mData;
    /* 54 */

    BattleField3D(BattleData *data, int unk2);
    virtual ~BattleField3D();
    void func_ov007_022298bc(float a, float b);
    void func_ov007_02229900();
    void func_ov007_022299f4(u32 frame);
    void func_ov007_02229b2c(u32 frame);
};

// Battle HUD scene on the sub screen (0x130 bytes, formerly BattleHud): the power gauge that tugs between the two
// players, the countdown, the small card images and the cut-in animations.
class BattleHud : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ int mUnk_0c; // viewer index: 0/1 players, 2/3 spectators
    /* 10 */ u16 mUnk_10;
    /* 12 */ u8 mUnk_12;
    /* 13 */ u8 mUnk_13; // gauge position in percent
    /* 14 */ s32 mUnk_14; // same, 20.12 fixed point
    /* 18 */ s16 mUnk_18;
    /* 1a */ s16 mUnk_1a;
    /* 1c */ u16 mUnk_1c[2];
    /* 20 */ u16 mUnk_20;
    union {
        /* 22 */ u8 mUnk_22;
        struct {
            /* 22 */ u8 mUnk_22_0 : 1;
            /* 22 */ u8 mUnk_22_1 : 1;
            /* 22 */ u8 mUnk_22_2 : 1;
            /* 22 */ u8 mUnk_22_3 : 1;
            /* 22 */ u8 mUnk_22_4 : 1;
            /* 22 */ u8 mUnk_22_5 : 1;
            /* 22 */ u8 mUnk_22_6 : 1;
            /* 22 */ u8 mUnk_22_7 : 1;
        };
    };
    union {
        /* 23 */ u8 mUnk_23;
        struct {
            /* 23 */ u8 mUnk_23_0 : 1;
            /* 23 */ u8 mUnk_23_1 : 1;
            /* 23 */ u8 mUnk_23_2 : 1;
        };
    };
    /* 24 */ u16 mUnk_24;
    /* 26 */ u8 mUnk_26;
    /* 27 */ u8 mUnk_27;
    /* 28 */ s32 mUnk_28[2];
    /* 30 */ CellAnimation *mAnims[23];
    /* 8c */ BattleData *mData;
    /* 90 */ BattleParam *mParam;
    /* 94 */ s8 (*mChoice)[2]; // per side: the two chosen entries (-1: none)
    /* 98 */ u32 mUnk_98;
    /* 9c */ u16 mUnk_9c;
    /* 9e */ PAD(0x9e, 0x9f);
    /* 9f */ u8 mUnk_9f;
    /* a0 */ float mUnk_a0[2];
    /* a8 */ float mUnk_a8[2];
    /* b0 */ float mUnk_b0[2];
    /* b8 */ float mUnk_b8[2];
    /* c0 */ u16 mUnk_c0;
    /* c2 */ PAD(0xc2, 0xc3);
    /* c3 */ u8 mUnk_c3;
    /* c4 */ BattleObj01 *mObjs[15];
    /* 100 */ u8 mUnk_100[2];
    /* 102 */ u8 mUnk_102[2];
    /* 104 */ u8 mUnk_104[2];
    /* 106 */ PAD(0x106, 0x108);
    /* 108 */ float mUnk_108;
    /* 10c */ float mUnk_10c;
    /* 110 */ float mUnk_110;
    /* 114 */ u8 mUnk_114;
    /* 115 */ u8 mUnk_115;
    /* 116 */ u8 mUnk_116;
    union {
        /* 117 */ u8 mUnk_117;
        struct {
            /* 117 */ u8 mUnk_117_0 : 1;
            /* 117 */ u8 mUnk_117_1 : 1;
            /* 117 */ u8 mUnk_117_2 : 1;
            /* 117 */ u8 mUnk_117_3 : 1;
        };
    };
    /* 118 */ u8 mUnk_118[2];
    /* 11a */ u8 mUnk_11a;
    /* 11b */ u8 mUnk_11b;
    /* 11c */ s16 mUnk_11c[2];
    /* 120 */ s16 mUnk_120;
    /* 122 */ u8 mUnk_122[2];
    /* 124 */ s16 mUnk_124;
    /* 126 */ s16 mUnk_126;
    /* 128 */ u8 mUnk_128;
    /* 129 */ u8 mUnk_129;
    /* 12a */ PAD(0x12a, 0x12b);
    /* 12b */ u8 mUnk_12b; // state of Update
    /* 12c */ s16 mUnk_12c;
    /* 12e */ s16 mUnk_12e;
    /* 130 */

    BattleHud(BattleData *data, BattleParam *param, u8 mode, s8 *choice);
    virtual ~BattleHud();
    virtual void Update();
    void func_ov007_022257e0();
    void func_ov007_0222644c(u8 side, int card);
    void func_ov007_02226614();
    void func_ov007_02226ab8();
    BOOL func_ov007_02226c70();
    void func_ov007_02226e7c();
    BOOL func_ov007_02226f1c();
    void func_ov007_02226fc8();
    BOOL func_ov007_02227068();
    void func_ov007_02227128();
    BOOL func_ov007_02227158();
    BOOL func_ov007_022273b4();
    void func_ov007_02227460(int side);
    void func_ov007_02227490(int mode, s32 value, s32 count, u8 boost, bool useLevel);
    u8 func_ov007_022275a0(u32 value, u8 a, u8 b);
    BOOL func_ov007_022275c0(u8 side, s16 x, s16 y, u8 flag);
    void func_ov007_02227730();
    void func_ov007_02227bbc(u8 index);
    void func_ov007_02227c54();
    void func_ov007_022284a0(int side);
    void func_ov007_022284c0(int side);
    void func_ov007_022284e0(int side);
    BOOL func_ov007_02228514();
    void func_ov007_02228530(int card);
    void func_ov007_022286c8();
    void func_ov007_022286e0();
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
    /* 40 */ BattleField3D *mUnk_40;
    /* 44 */ BattleHud *mUnk_44;
    /* 48 */ BattleSub48 *mUnk_48;
    /* 4c */ u8 mUnk_4c;
    /* 4d */ u8 mUnk_4d;
    /* 4e */ u8 mUnk_4e;
    /* 4f */ u8 mUnk_4f;
    /* 50 */ u8 mUnk_50;
    /* 51 */ u8 mUnk_51;
    /* 52 */ u8 mUnk_52[2];
    union {
        /* 54 */ u8 mUnk_54;
        struct {
            /* 54 */ u8 mUnk_54_0 : 1;
            /* 54 */ u8 mUnk_54_1 : 1;
            /* 54 */ u8 mUnk_54_2 : 1;
            /* 54 */ u8 mUnk_54_3 : 1;
            /* 54 */ u8 mUnk_54_4 : 1;
            /* 54 */ u8 mUnk_54_5 : 1;
        };
    };
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
    /* 08 */ virtual void func_ov007_0221cebc(int a);
    /* 0c */ virtual void func_ov007_0221ced0(u16 *a, u16 *b, u16 *c);
    /* 10 */ virtual void func_ov007_0221cfac(u16 *a, u16 *b);
    /* 14 */ virtual int func_ov007_0221d494(u16 *a, u16 *b, u16 *c, u8 *d);
    /* 18 */ virtual void func_ov007_0221d488(u16 *p);
    /* 1c */ virtual void func_ov007_0221d088(u8 a, u16 *counts);
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
    BOOL func_ov007_0221a7d0();
};

BOOL func_ov007_0221b998(int unk1, int mode, BattleEntry *entry);
BOOL func_ov007_0221b9d0(int unk1, int mode, BattleEntry *entry);
BOOL func_ov007_0221ba0c(int unk1, int mode, BattleEntry *entry);
BOOL func_ov007_0221ba38(int unk1, u8 side, int value, BattleEntry *entries);
int func_ov007_0221ba88(int unk1, int index, BattleEntry *entries);
void func_ov007_0223e214(BattleData *data, int side, u8 bit); // sets mEntries[side].mUnk_0f_3
u32 func_ov007_0221d348(u32 max);
int func_ov007_02222764(int side, BattleEntry *entry);
