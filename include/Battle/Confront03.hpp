#pragma once

#include "Battle/BattleBase.hpp"
#include "Battle/ConfrontEmblem03.hpp"

// Timing row used by Confront03 (table at 02278c0c).
struct ConfrontRow {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u8 mMinDelay;
    /* 03 */ u8 mMaxDelay;
    /* 04 */ u8 mUnk_04;
};

// Confrontation screen with only emblems (no vs_b_bg file), vtables 02286078 / 022860c8.
class Confront03 : public BattleBase {
public:
    /* 68 */ u32 mPenFrames;
    /* 6c */ u8 mPenEmblemIndex;
    /* 6d */ u8 mLongHold; // UpdateTouch's `low`: emblems then hold at scale 1.0 for 0x50 frames, not 0x14
    /* 6e */ u16 mUsedSlotMask; // bit k = slot k taken; the ctor sets bit 0, so slot 0 is never handed out
    /* 70 */ ConfrontRow *mConfrontRow; // always gConfrontRows (row 0)
    /* 74 */ u16 mDelay;
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

    BOOL CountDownDelay();
    // Frees the bits of idle emblems, then starts the first idle emblem on a random free slot. Without mTouched a pick
    // of slot 1 or 2 only draws two random numbers; with no free slot or idle emblem it draws three.
    void StartIdleEmblem();
};
