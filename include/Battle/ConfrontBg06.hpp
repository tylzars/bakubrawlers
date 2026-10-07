#pragma once

#include "Battle/BattleBase.hpp"
#include "Battle/ConfrontEmblem06.hpp"

// Confrontation screen with one bouncing emblem (ConfrontEmblem06).
class ConfrontBg06 : public BattleBase {
public:
    /* 68 */ BattleData *mData;
    /* 6c */ ConfrontEmblem06 *mEmblem;
    /* 70 */

    ConfrontBg06();
    virtual ~ConfrontBg06();
    /* 24 */ virtual void func_ov007_02219888(BattleData *data, u32 mode);
    /* 28 */ virtual void vfunc_28();
    /* 2c */ virtual void func_ov007_0221a624();
    /* 30 */ virtual BOOL func_ov007_0221bd84();
    /* 34 */ virtual void vfunc_34();
    /* 38 */ virtual BOOL func_ov007_0221bfa4();
    /* 3c */ virtual void vfunc_3c();
    /* 40 */ virtual u8 vfunc_40(int a);
    /* 44 */ virtual void vfunc_44(int index, int count);
};
