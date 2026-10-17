#pragma once

#include "global.h"
#include "types.h"

#include "Battle/BattleBase.hpp"
#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;
class BattleCommandPage;
class BattleCardCamera;
class BattleCardSpec;

class BattleMapObj;
class BattleMapModel;
class BakuganModel;
class ModelLightGroup;

// Camera of the card screen (vtable 02288700, 0x30 bytes): the map objects, two cameras and a model; built from
// the card ids of both sides.
class BattleCardCamera : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ BattleMapObj *mUnk_0c;
    /* 10 */ BakuganModel *mUnk_10[2];
    /* 18 */ BattleMapModel *mUnk_18;
    /* 1c */ ModelLightGroup *mUnk_1c;
    /* 20 */ void *mUnk_20;
    /* 24 */ u16 mUnk_24;
    /* 26 */ u8 mState;
    /* 27 */ u8 mUnk_27;
    /* 28 */ PAD(0x28, 0x29);
    /* 29 */ u8 mUnk_29;
    /* 2a */ u8 mUnk_2a;
    /* 2b */ u8 mUnk_2b;
    /* 2c */ u8 mUnk_2c;
    /* 2d */ u8 mUnk_2d;
    /* 2e */ u8 mUnk_2e;
    /* 2f */ PAD(0x2f, 0x30);
    /* 30 */

    BattleCardCamera(u16 a, u16 b);
    virtual ~BattleCardCamera();
    /* 08 */ virtual void Update();
    void func_ov007_02244bb4();
    void func_ov007_02244e98();
};

// Confrontation card screen (vtable 02288378): shows the cards of both sides before a battle (battle data,
// card graphics, power values, mes_confrontation text). Created with the id of a card layout (-1: from data).
class BattleConfrontCard : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ u8 mState;
    /* 0d */ u8 mUnk_0d;
    /* 0e */ u8 mUnk_0e;
    /* 0f */ u8 mUnk_0f;
    /* 10 */ u16 mUnk_10;
    /* 12 */ u16 mUnk_12;
    /* 14 */ s16 mUnk_14[3];
    /* 1a */ s16 mUnk_1a[3];
    /* 20 */ u8 mUnk_20;
    /* 21 */ s8 mUnk_21;
    /* 22 */ u8 mUnk_22;
    /* 23 */ u8 mUnk_23;
    /* 24 */ CellAnimation *mUnk_24[5];
    /* 38 */ BattleData *mData;
    /* 3c */ BattleCardCamera *mUnk_3c;
    /* 40 */ BattleCommandPage *mUnk_40;
    /* 44 */ BattleCardSpec *mUnk_44;
    /* 48 */ void *mUnk_48;
    /* 4c */ void *mUnk_4c;
    /* 50 */ u8 mUnk_50;
    /* 51 */ u8 mUnk_51;
    /* 52 */ u8 mUnk_52;
    /* 53 */ u8 mUnk_53;
    /* 54 */ u16 mUnk_54;
    /* 56 */ u16 mUnk_56;
    /* 58 */ PAD(0x58, 0x5c);
    /* 5c */ u8 mUnk_5c;
    /* 5d */ u8 mUnk_5d;
    /* 5e */ u8 mUnk_5e;
    /* 5f */ u8 mUnk_5f;
    /* 60 */ u8 mUnk_60;
    /* 61 */ u8 mUnk_61;
    /* 62 */ u8 mUnk_62;
    /* 63 */ u8 mUnk_63;
    /* 64 */ u8 mUnk_64;
    /* 65 */ u8 mUnk_65;
    /* 66 */ u16 mUnk_66;
    /* 68 */ u8 mUnk_68;
    /* 69 */ u8 mUnk_69;
    /* 6a */ PAD(0x6a, 0x6c);
    /* 6c */ u32 mUnk_6c;
    /* 70 */ u32 mUnk_70;
    /* 74 */ void *mUnk_74;
    /* 78 */ void *mUnk_78;
    /* 7c */ void *mUnk_7c;
    /* 80 */ SelectWindow *mUnk_80;
    /* 84 */ struct MessageFile *mUnk_84;
    /* 88 */

    BattleConfrontCard(u32 unk);
    virtual ~BattleConfrontCard();
    /* 08 */ virtual void Update();
    int func_ov007_0224251c();
    u8 func_ov007_022428d4(u16 card);
    u8 func_ov007_02244440();
    BOOL func_ov007_022433c8(u8 side, u16 id);
    void func_ov007_02243f04(const char *path, u32 mode);
    void func_ov007_022442d4(u32 index);
    void func_ov007_0223f044();
    void func_ov007_0223f0c0();
    void func_ov007_0223f168();
    void func_ov007_0223f398();
    void func_ov007_0223f780(u16 card);
    void func_ov007_0223f90c(u16 value);
    BOOL func_ov007_0223f918();
    void func_ov007_022430bc(int brightness);
    void func_ov007_022430d8(int blend);
    void func_ov007_02243100();
    void func_ov007_022432dc(u8 kind, u32 mode, u32 flag);
    u8 func_ov007_022433ac();
    u16 func_ov007_02243b24(u8 side, u32 id);
    void func_ov007_02243d40();
    void func_ov007_02243dec();
    void func_ov007_02243e7c();
};
