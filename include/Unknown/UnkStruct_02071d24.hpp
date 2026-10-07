#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class BattleCommandList;
class CellAnimation;
class MessageWindow;
class SelectWindow;
struct BattleCellPair;
struct MessageFile;

// Scene that shows the result screen of a battle (vtable 0x020b9038, 0x6c bytes): cockpit cells, a BattleCommandList and
// a SelectWindow with five buttons.
class UnkStruct_02071d24 : public SceneBase {
public:
    /* 0c */ MessageFile *mUnk_0c;
    /* 10 */ MessageFile *mUnk_10;
    /* 14 */ MessageWindow *mUnk_14;
    /* 18 */ BattleCommandList *mUnk_18;
    /* 1c */ CellAnimation *mUnk_1c;
    /* 20 */ SelectWindow *mUnk_20;
    /* 24 */ void *mUnk_24;
    /* 28 */ void *mUnk_28;
    /* 2c */ u32 mUnk_2c;
    /* 30 */ u32 mUnk_30;
    /* 34 */ BattleCellPair *mUnk_34[6];
    /* 4c */ u8 mUnk_4c;
    /* 4d */ u8 mUnk_4d;
    /* 4e */ u8 mUnk_4e;
    /* 4f */ u8 mUnk_4f;
    /* 50 */ u8 mUnk_50;
    /* 51 */ u8 mUnk_51;
    /* 52 */ u8 mUnk_52;
    /* 53 */ u8 mUnk_53;
    /* 54 */ s32 mUnk_54;
    /* 58 */ u8 mUnk_58[6];
    /* 5e */ u8 mUnk_5e[6];
    /* 64 */ u8 mUnk_64[6];
    /* 6a */ PAD(0x6a, 0x6c);
    /* 6c */

    UnkStruct_02071d24(u32 unk1);
    virtual ~UnkStruct_02071d24();
    virtual void Update();
    void func_02072734();
    void func_02072b14();
    void func_02072b60();
    void func_02072c40();
    BOOL func_02072d20();
    BOOL func_02072d84();
    void func_02072dfc();
    void func_02072e7c();
    void func_02073040();
    void func_020733d4();
    void func_02073750();
    void func_020739e4();
    void func_02073fb0();
    void func_02074128();
    void func_02074148(u32 index, u32 unk2, u32 unk3, u8 unk4);
};
