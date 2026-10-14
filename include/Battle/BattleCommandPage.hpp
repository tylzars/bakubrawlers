#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"
#include "UI/SelectWindow.hpp"

class CellAnimation;
class CellAnimFile;
class SelectWindow;

// Page of the battle command screen (0x10c bytes, ctor 02054df0, vtable 020b2dc8): shows the player/character
// icons and a SelectWindow of the pages.
class BattleCommandPage : public SceneBase {
public:
    /* 0c */ void *mUnk_0c[3];
    /* 18 */ CellAnimFile *mUnk_18[3];
    /* 24 */ CellAnimation *mCells[11];
    /* 50 */ SelectWindow *mUnk_50;
    /* 54 */ void *mUnk_54;
    /* 58 */ void *mUnk_58;
    /* 5c */ void *mUnk_5c;
    /* 60 */ void *mUnk_60[3];
    /* 6c */ void *mUnk_6c[3];
    /* 78 */ u32 mUnk_78[3];
    /* 84 */ u32 mUnk_84[3];
    /* 90 */ void *mUnk_90[12];
    /* c0 */ u32 mUnk_c0[12];
    /* f0 */ u8 mUnk_f0;
    /* f1 */ u8 mUnk_f1;
    /* f2 */ u8 mUnk_f2;
    /* f3 */ PAD(0xf3, 0xf4);
    /* f4 */ u16 mUnk_f4;
    /* f6 */ u16 mUnk_f6;
    /* f8 */ u16 mUnk_f8;
    /* fa */ s8 mUnk_fa;
    /* fb */ u8 mUnk_fb;
    /* fc */ u8 mUnk_fc;
    /* fd */ u8 mUnk_fd;
    /* fe */ u8 mUnk_fe;
    /* ff */ PAD(0xff, 0x100);
    /* 100 */ u16 mUnk_100;
    /* 102 */ s16 mUnk_102;
    /* 104 */ u8 mUnk_104;
    /* 105 */ u8 mUnk_105;
    /* 106 */ u8 mUnk_106;
    /* 107 */ u8 mUnk_107;
    /* 108 */ u8 mUnk_108;
    /* 109 */ u8 mUnk_109;
    /* 10a */ u8 mUnk_10a;
    /* 10b */ PAD(0x10b, 0x10c);
    /* 10c */

    BattleCommandPage(u16 a, s32 b);
    virtual ~BattleCommandPage();
    virtual void Update();
    u16 func_02055644(u16 id);
    void func_020556d0(u32 flag);
    BOOL func_02055724();
    void func_02055890(u32 enable);
    void func_020558b8();
    void func_02055940();
    BOOL func_02055ab4(u32 right);
    void func_02055af4();
    void func_02055c1c();
    BOOL func_02055c28();
    BOOL func_020561c4();
    void func_02056414();
    void func_02056814(u32 index);
    void func_02056d38();
    void func_02056f38();
    void func_02056f70();
    void func_020571a8();
};

void func_02055624(BattleCommandPage *page, u32 a);
void func_0205562c(BattleCommandPage *page, u32 a);
void func_02055668(BattleCommandPage *page, u16 a, s32 b);
void func_02055738(BattleCommandPage *page, u32 a, u32 b, u32 c);
void func_02055764(BattleCommandPage *page, u32 a, u32 b, s16 c, s16 d, s16 e, s16 f, u32 g);
