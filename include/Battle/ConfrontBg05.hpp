#pragma once

#include "Battle/BattleBase.hpp"
#include "Battle/BattleBlend.hpp"
#include "Battle/ConfrontEmblem05.hpp"

// Confrontation screen with background vs_b_bg_05_NN and one emblem (vs_emblem_NN).
class ConfrontBg05 : public BattleBase {
public:
    /* 68 */ u8 mSoundTimer;
    /* 69 */ PAD(0x69, 0x6a);
    /* 6a */ u16 mUnk_6a;
    /* 6c */ BattleData *mData;
    /* 70 */ ConfrontEmblem05 *mEmblem;
    /* 74 */ BattleBlend *mBlend;
    /* 78 */

    ConfrontBg05();
    virtual ~ConfrontBg05();
    /* 24 */ virtual void Init(BattleData *data, u32 mode);
    /* 28 */ virtual void LoadGraphics();
    /* 2c */ virtual void InitSubScreen();
    /* 30 */ virtual BOOL func_ov007_0221bd84();
    /* 34 */ virtual void UpdateEmblems();
    /* 38 */ virtual BOOL func_ov007_0221bfa4();
    /* 3c */ virtual void Draw();
    /* 40 */ virtual u8 UpdateTouch(int a);
    /* 44 */ virtual void vfunc_44(int index, u16 count);
};
