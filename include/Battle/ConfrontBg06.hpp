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
