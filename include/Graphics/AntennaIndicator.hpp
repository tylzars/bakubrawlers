#pragma once

#include "Scene/SceneBase.hpp"
#include "global.h"
#include "types.h"

class CellAnimation;

// Antenna indicator scene: draws two animations of antenna_nce.bin. Owned through gGameData.mUnk_10 and
// ListScene::mAntenna.
class AntennaIndicator : public SceneBase {
public:
    /* 0c */ CellAnimation *mFirstCellAnim; // drawn at frame data_020c4030.mUnk_24d (WirelessManager::mUnk_124d), not animated
    /* 10 */ CellAnimation *mSecondCellAnim;
    /* 14 */ PAD(0x14, 0x19);
    /* 19 */ u8 mLayoutIndex; // layout index
    /* 1a */ u8 mSecondVisible; // draw the second animation too
    /* 1b */ PAD(0x1b, 0x1c);
    /* 1c */

    AntennaIndicator(u32 unk1);
    virtual ~AntennaIndicator();
    virtual void Update();
    void SetupLayout(u32 unk1); // camera mode 2 or 3: screen 0, and the cells get 0x20 instead of 0x80 as third argument
};
