#pragma once

#include "Battle/BattleBase.hpp"
#include "Battle/BattleBlend.hpp"

class ConfrontBg01 : public BattleBase {
public:
    /* 68 */ u8 mSoundTimer;
    /* 69 */ u8 mUnk_69;
    /* 6a */ u16 mUnk_6a;
    /* 6c */ u32 mDistanceSum; // stylus distance summed over hit frames; zeroed at the limit and on frames with no hit
    /* 70 */ BattleData *mData;
    /* 74 */ BattleBlend *mBlend;
    /* 78 */

    ConfrontBg01();
    virtual ~ConfrontBg01();
    /* 24 */ virtual void Init(BattleData *data, u32 mode);
    /* 28 */ virtual void LoadGraphics();
    /* 2c */ virtual void InitSubScreen();
    /* 30 */ virtual BOOL func_ov007_0221bd84();
    /* 34 */ virtual void vfunc_34();
    /* 38 */ virtual BOOL func_ov007_0221bfa4();
    /* 3c */ virtual void vfunc_3c();
    /* 40 */ virtual u8 UpdateTouch(int a);
    /* 44 */ virtual void vfunc_44(int index, u16 count);
};
