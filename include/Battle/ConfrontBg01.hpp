#pragma once

#include "Battle/BattleBase.hpp"
#include "Battle/BattleBlend.hpp"

class ConfrontBg01 : public BattleBase {
public:
    /* 68 */ u8 mUnk_68;
    /* 69 */ u8 mUnk_69;
    /* 6a */ u16 mUnk_6a;
    /* 6c */ u32 mUnk_6c;
    /* 70 */ BattleData *mData;
    /* 74 */ BattleBlend *mBlend;
    /* 78 */

    ConfrontBg01();
    virtual ~ConfrontBg01();
    /* 24 */ virtual void func_ov007_02219888(BattleData *data, u32 mode);
    /* 28 */ virtual void vfunc_28();
    /* 2c */ virtual void func_ov007_0221a624();
    /* 30 */ virtual BOOL func_ov007_0221bd84();
    /* 34 */ virtual void vfunc_34();
    /* 38 */ virtual BOOL func_ov007_0221bfa4();
    /* 3c */ virtual void vfunc_3c();
    /* 40 */ virtual u8 vfunc_40(int a);
    /* 44 */ virtual void vfunc_44(int index, u16 count);
};
