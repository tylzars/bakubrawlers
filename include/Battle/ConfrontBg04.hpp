#pragma once

#include "Battle/BattleBase.hpp"
#include "Battle/BattleBlend.hpp"

// Confrontation screen with background set vs_b_bg_04 (rotating background).
class ConfrontBg04 : public BattleBase {
public:
    /* 68 */ s32 mUnk_68;
    /* 6c */ PAD(0x6c, 0x6e);
    /* 6e */ u16 mAngle;
    /* 70 */ s32 mSpeed;
    /* 74 */ BattleData *mData;
    /* 78 */ BattleBlend *mBlend;
    /* 7c */

    ConfrontBg04();
    virtual ~ConfrontBg04();
    /* 24 */ virtual void Init(BattleData *data, u32 mode);
    /* 28 */ virtual void LoadGraphics();
    /* 2c */ virtual void InitSubScreen();
    /* 30 */ virtual BOOL func_ov007_0221bd84();
    /* 34 */ virtual void vfunc_34();
    /* 38 */ virtual BOOL func_ov007_0221bfa4();
    /* 3c */ virtual void vfunc_3c();
    /* 40 */ virtual u8 UpdateTouch(int a);
    /* 44 */ virtual void vfunc_44(int index, u16 count);

    void func_ov007_02220718();
};

void func_ov007_02220688(ConfrontBg04 *scene); // frame callback: BG3 rotation
