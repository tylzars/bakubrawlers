#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class CellAnimation;
class SelectWindow;

// Overlay 3: the shop. Skeleton from the constructor and cleanup; extend as functions are decompiled.
class StoreScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ void *mUnk_0c;
    /* 10 */ SelectWindow *mUnk_10;
    /* 14 */ void *mUnk_14;
    /* 18 */ void *mUnk_18;
    /* 1c */ u32 mUnk_1c;
    /* 20 */ u32 mUnk_20;
    /* 24 */ SelectWindow *mUnk_24;
    /* 28 */ PAD(0x28, 0x2c);
    /* 2c */ u16 mUnk_2c;
    /* 2e */ PAD(0x2e, 0x30);
    /* 30 */ CellAnimation *mCellAnims[2];
    /* 38 */

    StoreScene();

    /* 00 */ virtual ~StoreScene() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void Cleanup();
};
