#pragma once

#include "Scene/SceneBase.hpp"
#include "global.h"
#include "types.h"

class CellAnimation;

// Antenna indicator scene: draws two animations of antenna_nce.bin. Owned through data_020d433c.mUnk_10 and
// ListScene::mAntenna.
class AntennaIndicator : public SceneBase {
public:
    /* 0c */ CellAnimation *mFirstAnim;
    /* 10 */ CellAnimation *mSecondAnim;
    /* 14 */ PAD(0x14, 0x19);
    /* 19 */ u8 mLayoutIndex; // layout index
    /* 1a */ u8 mSecondVisible; // draw the second animation too
    /* 1b */ PAD(0x1b, 0x1c);
    /* 1c */

    AntennaIndicator(u32 unk1);
    virtual ~AntennaIndicator();
    virtual void Update();
    void SetupLayout(u32 unk1);
};
