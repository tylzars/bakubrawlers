#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class MessageWindow;
class BattleCommandPage;
class BattleCommandList;
class SelectWindow;
class CharacterIcons;
struct BattleRecord;

struct BattlePromptSlot {
    u16 mId;
    u8 mKind;
    u8 mUnk_03;
};

// 0x98 bytes, ctor 0225d08c: the prompt of the command screen (page/list of cells to pick from).
class BattleCommandPrompt : public SceneBase {
public:
    /* 0c */ CellAnimation *mUnk_0c[3];
    /* 18 */ CellAnimation *mUnk_18;
    /* 1c */ CellAnimation *mUnk_1c;
    /* 20 */ CellAnimation *mUnk_20[3];
    /* 2c */ MessageWindow *mUnk_2c;
    /* 30 */ BattleCommandPage *mUnk_30;
    /* 34 */ BattleCommandList *mUnk_34;
    /* 38 */ SelectWindow *mUnk_38;
    /* 3c */ CharacterIcons *mUnk_3c;
    /* 40 */ CellAnimation *mUnk_40[3];
    /* 4c */ u32 mUnk_4c;
    /* 50 */ s16 mUnk_50;
    /* 52 */ u16 mUnk_52[6];
    /* 5e */ u16 mUnk_5e[6];
    /* 6a */ u8 mUnk_6a[6];
    /* 70 */ BattleRecord *mUnk_70[6];
    /* 88 */ u16 mUnk_88;
    /* 8a */ u8 mUnk_8a;
    /* 8b */ u8 mUnk_8b;
    /* 8c */ u8 mUnk_8c;
    /* 8d */ u8 mUnk_8d;
    /* 8e */ u8 mUnk_8e;
    /* 8f */ u8 mUnk_8f;
    /* 90 */ u8 mUnk_90;
    /* 91 */ u8 mUnk_91;
    /* 92 */ u8 mUnk_92;
    /* 93 */ u8 mUnk_93;
    /* 94 */ u8 mUnk_94[4];
    /* 98 */

    BattleCommandPrompt(u32 mode, void *target, u32 flags, u32 charOffset, u8 player);
    virtual ~BattleCommandPrompt();
    virtual void Update();
    void func_ov007_0225e6d4();
    void func_ov007_0225f0c4(const BattlePromptSlot *slot);
    void func_ov007_0225f200();
};

void func_ov007_0225e68c(BattleCommandPrompt *prompt, u32 flag); // releases the prompt
extern "C" u8 func_ov007_0225f0bc(u32 v);
