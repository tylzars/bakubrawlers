#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

#include <nitro/fx.h>

class CellAnimation;
class ModelInstance;
class ModelLightGroup;
class StorytalkWindow;
class Camera;
class UnkStruct_0205401c;
class BakuganModel;
class TextureFile;

// func_0206b05c's result: an enum, not BOOL (the final compare's moveq/movne order needs it)
enum UnkStruct_02065c40_Kind { UNK_02065C40_KIND_0, UNK_02065C40_KIND_1 };

struct UnkStruct_02065c40_cmd {
    /* 00 */ s16 mValue;
    /* 02 */ u8 mCommand;
    /* 03 */ PAD(0x03, 0x04);
    /* 04 */
};

struct UnkStruct_Vec3s {
    /* 00 */ s16 x;
    /* 02 */ s16 y;
    /* 04 */ s16 z;
    /* 06 */
};

struct UnkStruct_02065c40_18 {
    /* 00 */ fx32 mUnk_00;
    /* 04 */ fx32 mUnk_04;
    /* 08 */ fx32 mUnk_08;
    /* 0c */ s16 mUnk_0c;
    /* 0e */ s16 mUnk_0e;
    /* 10 */ s16 mUnk_10;
    /* 12 */ s16 mUnk_12;
    /* 14 */ s16 mUnk_14;
    /* 16 */ s16 mUnk_16;
    /* 18 */
};

struct UnkStruct_02065c40_74 {
  union {
   UnkStruct_Vec3s mVec[3];
   struct {
    /* 00 */ u16 mUnk_00;
    union {
        /* 02 */ u16 mUnk_02;
        /* 02 */ s16 mUnk_02s;
    };
    /* 04 */ PAD(0x04, 0x08);
    /* 08 */ s16 mUnk_08;
    /* 0a */ PAD(0x0a, 0x14);
   };
  };
    /* 14 */
};

struct UnkStruct_02065c40_34 {
    /* 00 */ s32 mUnk_00;
    /* 04 */ fx32 mUnk_04;
    /* 08 */ fx32 mUnk_08;
    /* 0c */ s16 mUnk_0c[3];
    /* 12 */ s16 mUnk_12[3];
    /* 18 */ s16 mUnk_18[3];
    /* 1e */ s16 mUnk_1e[3];
    /* 24 */ s16 mUnk_24[3];
    /* 2a */ PAD(0x2a, 0x2c);
    /* 2c */ ModelInstance *mUnk_2c;
    /* 30 */ s16 mUnk_30;
    /* 32 */ u8 mUnk_32;
    /* 33 */ PAD(0x33, 0x34);
    /* 34 */
};

struct UnkStruct_02065c40_54 {
    /* 00 */ fx32 mUnk_00;
    /* 04 */ fx32 mUnk_04;
    /* 08 */ fx32 mUnk_08;
    /* 0c */ s16 mUnk_0c[3];
    /* 12 */ s16 mUnk_12[3];
    /* 18 */ s16 mUnk_18[3];
    /* 1e */ s16 mUnk_1e[3];
    /* 24 */ s16 mUnk_24[3];
    /* 2a */ PAD(0x2a, 0x2c);
    /* 2c */ ModelInstance *mUnk_2c;
    /* 30 */ void *mUnk_30[3];
    /* 3c */ void *mUnk_3c[3];
    /* 48 */ s16 mUnk_48;
    /* 4a */ s16 mUnk_4a;
    /* 4c */ u8 mUnk_4c;
    /* 4d */ u8 mUnk_4d;
    /* 4e */ u8 mUnk_4e;
    /* 4f */ u8 mUnk_4f;
    /* 50 */ u8 mUnk_50;
    /* 51 */ u8 mUnk_51;
    /* 52 */ u8 mUnk_52;
    /* 53 */ PAD(0x53, 0x54);
    /* 54 */
};

struct UnkStruct_02065c40_44 {
    /* 00 */ fx32 mUnk_00;
    /* 04 */ fx32 mUnk_04;
    /* 08 */ fx32 mUnk_08;
    /* 0c */ s16 mUnk_0c[3];
    /* 12 */ s16 mUnk_12[3];
    /* 18 */ s16 mUnk_18[3];
    /* 1e */ s16 mUnk_1e[3];
    /* 24 */ s16 mUnk_24[3];
    /* 2a */ PAD(0x2a, 0x2c);
    /* 2c */ BakuganModel *mUnk_2c;
    /* 30 */ BakuganModel *mUnk_30;
    /* 34 */ s8 mUnk_34;
    /* 35 */ s8 mUnk_35;
    /* 36 */ u8 mUnk_36;
    /* 37 */ u8 mUnk_37;
    /* 38 */ u8 mUnk_38;
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ u8 mUnk_3b;
    /* 3c */ u8 mUnk_3c;
    /* 3d */ u8 mUnk_3d;
    /* 3e */ u8 mUnk_3e;
    /* 3f */ u8 mUnk_3f;
    /* 40 */ s16 mUnk_40;
    /* 42 */ u8 mUnk_42;
    /* 43 */ PAD(0x43, 0x44);
    /* 44 */
};

struct UnkStruct_02065c40_48 {
    /* 00 */ fx32 mUnk_00;
    /* 04 */ fx32 mUnk_04;
    /* 08 */ fx32 mUnk_08;
    /* 0c */ s16 mUnk_0c[3];
    /* 12 */ s16 mUnk_12[3];
    /* 18 */ s16 mUnk_18[3];
    /* 1e */ s16 mUnk_1e[3];
    /* 24 */ s16 mUnk_24[3];
    /* 2a */ PAD(0x2a, 0x2c);
    /* 2c */ UnkStruct_0205401c *mUnk_2c;
    /* 30 */ UnkStruct_0205401c *mUnk_30;
    /* 34 */ s16 mUnk_34;
    /* 36 */ s8 mUnk_36;
    /* 37 */ s8 mUnk_37;
    /* 38 */ u8 mUnk_38;
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ u8 mUnk_3b;
    /* 3c */ u8 mUnk_3c;
    /* 3d */ u8 mUnk_3d;
    /* 3e */ u8 mUnk_3e;
    /* 3f */ u8 mUnk_3f;
    /* 40 */ u8 mUnk_40;
    /* 41 */ u8 mUnk_41;
    /* 42 */ u8 mUnk_42;
    /* 43 */ u8 mUnk_43;
    /* 44 */ PAD(0x44, 0x48);
    /* 48 */
};

// Script/event player.
class UnkStruct_02065c40 : public SceneBase {
public:
    /* 000c */ UnkStruct_02065c40_cmd mUnk_0c[0x400];
    /* 100c */ CellAnimation *mUnk_100c;
    /* 1010 */ ModelInstance *mUnk_1010;
    /* 1014 */ ModelLightGroup *mUnk_1014;
    /* 1018 */ StorytalkWindow *mUnk_1018;
    /* 101c */ UnkStruct_02065c40_34 mUnk_101c[2];
    /* 1084 */ UnkStruct_02065c40_54 mUnk_1084[2];
    /* 112c */ UnkStruct_02065c40_44 mUnk_112c[2];
    /* 11b4 */ UnkStruct_02065c40_48 mUnk_11b4[2];
    /* 1244 */ UnkStruct_02065c40_18 mUnk_1244[2];
    /* 1274 */ UnkStruct_02065c40_74 mUnk_1274;
    /* 1288 */ Camera *mUnk_1288;
    /* 128c */ u16 mUnk_128c[3];
    /* 1292 */ u8 mUnk_1292;
    /* 1293 */ PAD(0x1293, 0x1298);
    /* 1298 */ UnkStruct_Vec3s mUnk_1298;
    /* 129e */ UnkStruct_Vec3s mUnk_129e;
    /* 12a4 */ UnkStruct_Vec3s mUnk_12a4;
    /* 12aa */ PAD(0x12aa, 0x12ac);
    /* 12ac */ u16 mUnk_12ac;
    /* 12ae */ PAD(0x12ae, 0x12b0);
    /* 12b0 */ u32 mUnk_12b0;
    /* 12b4 */ u32 mUnk_12b4;
    /* 12b8 */ u8 mUnk_12b8;
    /* 12b9 */ u8 mUnk_12b9;
    /* 12ba */ u8 mUnk_12ba;
    /* 12bb */ PAD(0x12bb, 0x12bc);
    /* 12bc */ u16 mUnk_12bc;
    /* 12be */ u16 mUnk_12be;
    /* 12c0 */ u16 mUnk_12c0;
    /* 12c2 */ u16 mUnk_12c2;
    /* 12c4 */ u8 mUnk_12c4;
    /* 12c5 */ u8 mUnk_12c5;
    /* 12c6 */ u8 mUnk_12c6;
    /* 12c7 */ PAD(0x12c7, 0x12c8);
    /* 12c8 */ s16 mUnk_12c8;
    /* 12ca */ u8 mUnk_12ca;
    /* 12cb */ u8 mUnk_12cb;
    /* 12cc */ u16 mUnk_12cc;
    /* 12ce */ u16 mUnk_12ce;
    /* 12d0 */ u16 mUnk_12d0;
    /* 12d2 */ s16 mUnk_12d2[0x80];
    /* 13d2 */ s16 mUnk_13d2[0x80];
    /* 14d2 */ s8 mUnk_14d2;
    /* 14d3 */ u8 mUnk_14d3;
    /* 14d4 */ s8 mUnk_14d4;
    /* 14d5 */ s8 mUnk_14d5;
    /* 14d6 */ s8 mUnk_14d6;
    /* 14d7 */ s8 mUnk_14d7;
    /* 14d8 */ u8 mUnk_14d8;
    /* 14d9 */ u8 mUnk_14d9;
    /* 14da */ u16 mUnk_14da;
    /* 14dc */ u16 mUnk_14dc;
    /* 14de */ u16 mUnk_14de;
    /* 14e0 */ u8 mUnk_14e0;
    /* 14e1 */ u8 mUnk_14e1;
    /* 14e2 */ u8 mUnk_14e2;
    /* 14e3 */ PAD(0x14e3, 0x14e4);
    /* 14e4 */ u8 mUnk_14e4;
    /* 14e5 */ u8 mUnk_14e5;
    /* 14e6 */ u8 mUnk_14e6;
    /* 14e7 */ u8 mUnk_14e7;
    /* 14e8 */ u8 mUnk_14e8;
    /* 14e9 */ u8 mUnk_14e9;
    /* 14ea */ u8 mUnk_14ea;
    /* 14eb */ u8 mUnk_14eb;
    /* 14ec */ u8 mUnk_14ec;
    /* 14ed */ u8 mUnk_14ed;
    /* 14ee */ u8 mUnk_14ee;
    /* 14ef */ u8 mUnk_14ef;
    /* 14f0 */ u8 mUnk_14f0;
    /* 14f1 */ u8 mUnk_14f1;
    /* 14f2 */ u8 mUnk_14f2;
    /* 14f3 */ u8 mUnk_14f3;
    /* 14f4 */

    UnkStruct_02065c40(u32 unk1, u32 unk2, u32 unk3);
    virtual ~UnkStruct_02065c40();
    virtual void Update();
    s32 func_02066814(u32 unk1);
    void func_0206791c(u32 unk1);
    void func_020679d8();
    void func_02067a30();
    void func_02067e2c(u32 index, u8 unk2);
    void func_02067e74();
    void func_02068074(u32 index);
    void func_0206852c(u32 index);
    void func_02069a24(u32 index);
    void func_02069340(u32 index);
    BOOL func_020680f8(u32 index, u32 flag);
    void func_020682a0(u32 index, u32 kind);
    void func_020683cc(u32 index);
    s8 func_020687a8(s32 kind, u32 index, u32 column);
    void func_0206867c(u32 index, u32 value);
    void func_02068714();
    void func_02068884(u32 index);
    BOOL func_020688e0(s32 value);
    BOOL func_02068954();
    BOOL func_02068c54(u32 index, s32 value);
    void func_02068d0c(TextureFile *texture, u32 id);
    void func_02069120(u32 index, u32 value);
    void func_020695e8(u32 index);
    BOOL func_02069680(u32 index);
    void func_020696e8(u32 index);
    u8 func_02069780(u32 index);
    BOOL func_02068e14(u32 index);
    BOOL func_02069194(u32 index);
    BOOL func_020697c8(u32 index, u32 flag);
    void func_020699a4(u32 index);
    void func_02069b80(u32 index);
    void func_02069d0c();
    void func_02069d54();
    void func_02069eb8(u32 mode);
    void func_0206a01c();
    void func_0206a1c8(u32 value, u32 force);
    void func_02070390();
    void func_0206a208();
    void func_0206a850();
    void func_0206aa34();
    void func_0206abf4(u32 index);
    void func_0206ada4();
    void func_0206aecc();
    UnkStruct_02065c40_Kind func_0206b05c(u32 id);
    void func_0206b09c();
    void func_0206b150(u32 command, s32 value);
    s32 func_02070554(s32 kind, s32 index);
    void func_0206e9d0(u32 flag);
    BOOL func_0206ef08();
    BOOL func_02067f30(u32 index, s32 value);
    u32 func_02066f60();
    void func_02067b08();
    BOOL func_0206ed40();
    void func_0206d3b8();
    void func_020666d8();
    void func_02067708(u32 unk1);
    void func_02069bdc();
    void func_02069c6c();
    u32 func_020667ec();
    BOOL func_02066c7c();
    void func_02066c9c(u32 unk1, u32 unk2);
    u32 func_020670b4(u32 id);
    u8 func_02067240(u32 id);
    u8 func_02067120(u32 unk1, u32 unk2, u32 unk3, u32 unk4);
    BOOL func_020678e8();
    u32 func_02067900();
    BOOL func_02067938();
    void func_02067944();
    u32 func_02067954();
    void func_0206796c();
    u32 func_0206797c();
    void func_02067988();
    BOOL func_0206eb10(const char *path);
    void func_020676f8();
    BOOL func_0206ef4c(const char *name, u32 unk2, u32 unk3, u32 unk4, s32 unk5);
    void func_02067998(u8 unk1);
    void func_0206f564(s32 unk1, u32 unk2, s32 unk3);
    u32 func_0206f9b4(const char *name, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7);
    u8 func_0206fa20(const char *name, const u16 *unk2, const u8 *unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7,
                     u32 unk8, u32 unk9);
    BOOL func_020702e0(s32 unk1, s32 unk2, s32 unk3, s32 unk4, s32 unk5, s32 unk6);
};
