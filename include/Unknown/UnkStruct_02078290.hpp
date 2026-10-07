#pragma once

#include "Scene/SceneBase.hpp"
#include "global.h"
#include "types.h"

class CellAnimation;

// Antenna (signal strength) indicator scene. Owned through data_020d433c.mUnk_10.
class UnkStruct_02078290 : public SceneBase {
public:
    /* 0c */ CellAnimation *mUnk_0c;
    /* 10 */ CellAnimation *mUnk_10;
    /* 14 */ PAD(0x14, 0x19);
    /* 19 */ u8 mUnk_19; // layout index
    /* 1a */ u8 mUnk_1a; // draw the second animation too
    /* 1b */ PAD(0x1b, 0x1c);
    /* 1c */

    UnkStruct_02078290(u32 unk1);
    virtual ~UnkStruct_02078290();
    virtual void Update();
    void func_020785d8(u32 unk1);
};
