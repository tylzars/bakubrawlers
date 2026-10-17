#pragma once

#include "Battle/BattleBase.hpp"
#include "Battle/ConfrontEmblem03.hpp"

// Timing row used by Confront03 (table at 02278c0c).
struct ConfrontRow {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u8 mUnk_02;
    /* 03 */ u8 mUnk_03;
    /* 04 */ u8 mUnk_04;
};

// Confrontation screen with only emblems (no vs_b_bg file), vtables 02286078 / 022860c8.
class Confront03 : public BattleBase {
public:
    /* 68 */ u32 mUnk_68;
    /* 6c */ u8 mUnk_6c;
    /* 6d */ u8 mUnk_6d;
    /* 6e */ u16 mUnk_6e;
    /* 70 */ ConfrontRow *mUnk_70;
    /* 74 */ u16 mUnk_74;
    /* 78 */ BattleData *mData;
    /* 7c */ ConfrontEmblem03 *mEmblems[4];
    /* 8c */

    Confront03();
    virtual ~Confront03();
    /* 24 */ virtual void Init(BattleData *data, u32 mode);
    /* 28 */ virtual void LoadGraphics();
    /* 2c */ virtual void InitSubScreen();
    /* 30 */ virtual BOOL func_ov007_0221bd84();
    /* 34 */ virtual void vfunc_34();
    /* 38 */ virtual BOOL func_ov007_0221bfa4();
    /* 3c */ virtual void vfunc_3c();
    /* 40 */ virtual u8 UpdateTouch(int a);
    /* 44 */ virtual void vfunc_44(int index, u16 count);

    BOOL func_ov007_0221fe4c();
    void func_ov007_0221feb8();
};
