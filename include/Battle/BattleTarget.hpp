#pragma once

#include "global.h"
#include "types.h"

#include <nitro/fx.h>

#include "Graphics/Camera.hpp"
#include "Graphics/UnkStruct_0203d374.hpp"
#include "Scene/SceneBase.hpp"

struct GimmickSetData;
class CellAnimation;
class BattleStatusPanels;
class BattleCommandMap;
struct BattleUnk29c;
struct BattlePlayer;

// A box on the field floor (centre, half sizes and the rotation of its frame).
struct BattleHitBox {
    /* 00 */ VecFx32 mPos;
    union {
        /* 0c */ VecFx32 mAxes[3]; // the frame's axes, refreshed from the rotation by func_ov007_0227238c
        /* 0c */ PAD(0x0c, 0x30);
    };
    /* 30 */ fx32 mHalfX;
    /* 34 */ fx32 mHalfY;
    /* 38 */ fx32 mHalfZ;
    /* 3c */ u16 mRotX;
    /* 3e */ u16 mRotY;
    /* 40 */ u16 mRotZ;
    /* 42 */ PAD(0x42, 0x44);
    /* 44 */
};

// One 0x20-byte obstacle record of a gimmick set file (at set + 0x10 + 0x20 * i): box centre, half sizes, rotation, shape.
struct BattleObstacle {
    /* 00 */ VecFx32 mPos;
    /* 0c */ fx32 mHalfX;
    /* 10 */ fx32 mHalfY;
    /* 14 */ fx32 mHalfZ;
    /* 18 */ u16 mRotX;
    /* 1a */ u16 mRotY;
    /* 1c */ u16 mRotZ;
    /* 1e */ u8 mType; // 0: box, 1: ball, 4: wall, ...
    /* 1f */ u8 mUnk_1f;
};

// A ball: centre and radius.
struct BattleSphere {
    VecFx32 mPos;
    fx32 mRadius;
};

// A plane: normal and distance.
struct BattlePlane {
    VecFx32 mNormal;
    fx32 mDist;
};

// The shoot minigame object (shoot, battle_shoot_b_*): the camera, the fired bakugan (a posed 3D object), its status
// panels and the sprites. Vtable 02289810 (0x1f0 bytes, created by func_ov007_0226bd3c).
class BattleTarget : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ CellAnimation *mAnims[7];
    /* 28 */ BattleStatusPanels *mUnk_28;
    /* 2c */ void *mUnk_2c;
    /* 30 */ u32 mUnk_30;
    /* 34 */ s32 mUnk_34;
    /* 38 */ u32 mUnk_38;
    /* 3c */ u32 mUnk_3c;
    /* 40 */ u32 mUnk_40;
    /* 44 */ u32 mUnk_44;
    /* 48 */ u32 mUnk_48;
    /* 4c */ u32 mUnk_4c;
    /* 50 */ u32 mUnk_50;
    /* 54 */ u32 mUnk_54;
    /* 58 */ VecFx32 mUnk_58;
    /* 64 */ VecFx32 mUnk_64;
    /* 70 */ VecFx32 mUnk_70;
    /* 7c */ VecFx32 mUnk_7c;
    /* 88 */ u32 mUnk_88;
    /* 8c */ fx32 mUnk_8c;
    /* 90 */ u32 mUnk_90;
    /* 94 */ VecFx32 mUnk_94;
    /* a0 */ VecFx32 mUnk_a0;
    /* ac */ VecFx32 mUnk_ac;
    /* b8 */ VecFx32 mUnk_b8;
    /* c4 */ u32 mUnk_c4;
    /* c8 */ VecFx32 mUnk_c8;
    /* d4 */ VecFx32 mUnk_d4;
    /* e0 */ u8 *mUnk_e0;
    /* e4 */ GimmickSetData *mUnk_e4[12];
    /* 114 */ s32 mUnk_114;
    /* 118 */ s32 mUnk_118;
    /* 11c */ VecFx32 mUnk_11c;
    /* 128 */ VecFx32 mUnk_128;
    /* 134 */ VecFx32 mUnk_134;
    /* 140 */ u32 mUnk_140;
    /* 144 */ VecFx32 *mUnk_144;
    /* 148 */ u32 mUnk_148;
    /* 14c */ u32 mUnk_14c;
    /* 150 */ PAD(0x150, 0x15c);
    /* 15c */ VecFx32 mUnk_15c;
    /* 168 */ VecFx32 mUnk_168;
    /* 174 */ Camera *mUnk_174;
    /* 178 */ BattleCommandMap *mUnk_178;
    /* 17c */ u32 mUnk_17c;
    /* 180 */ s32 mUnk_180;
    /* 184 */ s32 mUnk_184;
    /* 188 */ VecFx32 mUnk_188;
    /* 194 */ VecFx32 mUnk_194;
    /* 1a0 */ u32 mUnk_1a0;
    /* 1a4 */ UnkStruct_0203d374 *mUnk_1a4;
    /* 1a8 */ GimmickSetData *mUnk_1a8;
    /* 1ac */ GimmickSetData *mUnk_1ac;
    /* 1b0 */ u16 mUnk_1b0;
    /* 1b2 */ u16 mUnk_1b2;
    /* 1b4 */ u16 mUnk_1b4;
    /* 1b6 */ u16 mUnk_1b6;
    /* 1b8 */ u16 mUnk_1b8;
    /* 1ba */ u16 mUnk_1ba;
    /* 1bc */ u16 mUnk_1bc;
    /* 1be */ u16 mUnk_1be;
    /* 1c0 */ u16 mUnk_1c0;
    /* 1c2 */ u16 mUnk_1c2;
    /* 1c4 */ u16 mUnk_1c4;
    /* 1c6 */ u16 mUnk_1c6;
    /* 1c8 */ u16 mUnk_1c8;
    /* 1ca */ u8 mUnk_1ca;
    /* 1cb */ u8 mUnk_1cb;
    /* 1cc */ u8 mUnk_1cc;
    /* 1cd */ u8 mUnk_1cd;
    /* 1ce */ u8 mUnk_1ce;
    /* 1cf */ u8 mUnk_1cf;
    /* 1d0 */ u8 mUnk_1d0;
    /* 1d1 */ u8 mUnk_1d1;
    /* 1d2 */ u8 mUnk_1d2;
    /* 1d3 */ u8 mUnk_1d3;
    /* 1d4 */ u8 mUnk_1d4;
    /* 1d5 */ u8 mUnk_1d5;
    /* 1d6 */ u8 mUnk_1d6;
    /* 1d7 */ u8 mUnk_1d7;
    /* 1d8 */ u8 mUnk_1d8;
    /* 1d9 */ u8 mUnk_1d9;
    /* 1da */ u8 mUnk_1da;
    /* 1db */ u8 mUnk_1db;
    /* 1dc */ u8 mUnk_1dc;
    /* 1dd */ u8 mUnk_1dd;
    /* 1de */ u8 mUnk_1de;
    /* 1df */ u8 mUnk_1df;
    /* 1e0 */ u8 mUnk_1e0;
    /* 1e1 */ u8 mUnk_1e1;
    /* 1e2 */ u8 mUnk_1e2;
    /* 1e3 */ u8 mUnk_1e3;
    /* 1e4 */ u8 mUnk_1e4;
    /* 1e5 */ u8 mUnk_1e5;
    /* 1e6 */ u8 mUnk_1e6;
    /* 1e7 */ u8 mUnk_1e7_0 : 1;
    /* 1e7 */ u8 mUnk_1e7_1 : 1;
    /* 1e7 */ u8 mUnk_1e7_2 : 6;
    /* 1e8 */ u8 mUnk_1e8;
    /* 1e9 */ u8 mUnk_1e9;
    /* 1ea */ u8 mUnk_1ea;
    /* 1eb */ u8 mUnk_1eb;
    /* 1ec */ u8 mUnk_1ec;
    /* 1ed */ PAD(0x1ed, 0x1f0);
    /* 1f0 */

    BattleTarget(Camera *camera, u8 unk2, u8 unk3);
    virtual ~BattleTarget();
    /* 08 */ virtual void Update();
    void func_ov007_02253fbc();
    void func_ov007_02254020();
    void func_ov007_02254790();
    void func_ov007_022547e4();
    void func_ov007_02254898();
    void func_ov007_02254928(u32 unk);
    BOOL func_ov007_02254938(VecFx32 *a, VecFx32 *b, VecFx32 *vel, u8 index);
    void func_ov007_02254a14(GimmickSetData *set);
    void func_ov007_02255640();
    void func_ov007_0225897c();
    void func_ov007_022593b4();
    void func_ov007_022567ac();
    void func_ov007_02256c4c(VecFx32 *target);
    void func_ov007_02255cb4(VecFx32 *target);
    void func_ov007_022571d4();
    void func_ov007_022573e4();
    void func_ov007_0225ca48(u32 player, u32 record);
    void func_ov007_0225cd6c();
    BOOL func_ov007_0225d078();
    void func_ov007_0225b6d0();
    void func_ov007_02257f3c(GimmickSetData *set, u32 index, VecFx32 *vel, VecFx32 *out);
    BOOL func_ov007_02257c80(BattleSphere *sphere, VecFx32 *dir, BattleObstacle *obj, VecFx32 *out, u8 flag, u8 *outFlag);
    void func_ov007_0225c4dc();
    void func_ov007_0225b298(s32 *pair, u8 mode, VecFx32 *out);
    BOOL func_ov007_0225b4a0(VecFx32 *pos, u16 *angle, VecFx32 *vel, u8 *outA, u8 *outB);
    void func_ov007_0225c360(u16 *in, s32 a, u16 *out);
    void func_ov007_0225ce08();
    void func_ov007_0225cb7c();
    BOOL func_ov007_022583b8();
    void func_ov007_02258448();
    void func_ov007_02259494(VecFx32 *target);
    void func_ov007_02258edc();
    void func_ov007_02258660();
    BOOL func_ov007_02258cec(VecFx32 *a, VecFx32 *b, VecFx32 *c, u8 d);
    void func_ov007_02258a34();
    void func_ov007_022599fc();
    BOOL func_ov007_02259af0();
    BOOL func_ov007_0225a278();
    void func_ov007_02254ae8(VecFx32 *a, VecFx32 *b, u32 *c, u32 *d);
    void func_ov007_0225745c(VecFx32 *pos, u16 *rot, VecFx32 *vel, VecFx32 *a, VecFx32 *b, u32 *counter, u8 flag, u32 mode);
    void func_ov007_0225bf70(VecFx32 *a, VecFx32 *b);
    void func_ov007_02255a9c();
    void func_ov007_022585e0();
    BOOL func_ov007_0225ac28(BattleHitBox *box, BattleSphere *sphere, VecFx32 *dir, VecFx32 *out, u8 flag, u8 *outFlag);
    BOOL func_ov007_0225aa80(BattleHitBox *box, VecFx32 *point);
    BOOL func_ov007_0225aba0(VecFx32 *a, VecFx32 *b, VecFx32 *tri, fx32 *t, VecFx32 *out);
    void func_ov007_0225b69c(u8 a, u8 b);
    void func_ov007_0225bf38();
    void func_ov007_0225c098(s32 *pair);
    void func_ov007_0225c2e4(VecFx32 *v);
    void func_ov007_02259a40(u8 a, u8 b, VecFx32 *out, VecFx32 *ref);
    BOOL func_ov007_0225a8d4(VecFx32 *a, VecFx32 *b, BattlePlane *plane, fx32 *t, VecFx32 *hit);
    BOOL func_ov007_0225a9c8(VecFx32 *a, VecFx32 *b, VecFx32 *c, VecFx32 *d, VecFx32 *e, fx32 *t, VecFx32 *hit);
    void func_ov007_02255b0c();
    void func_ov007_02255b78(VecFx32 *b, VecFx32 *c, VecFx32 *in, VecFx32 *out);
    void func_ov007_02255c04(VecFx32 *vec, s32 *out);
};
