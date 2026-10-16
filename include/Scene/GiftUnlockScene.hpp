#pragma once

#include "global.h"
#include "types.h"
#include "Graphics/CellAnimation.hpp"
#include "Scene/SceneBase.hpp"
#include "UI/SelectWindow.hpp"

class GiftUnlockScene : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ CellAnimation *mCell;
    /* 10 */ CellAnimation *mCellAnims[2];
    /* 18 */ SelectWindow *mWindow;
    /* 1c */ s16 mOffset;
    /* 1e */ u8 mUnk_1e;
    /* 1f */ u8 mState;
    /* 20 */ s8 mIndex;
    /* 21 */ PAD(0x21, 0x24);
    /* 24 */

    GiftUnlockScene(s32 unk1);
    /* 00 */ virtual ~GiftUnlockScene() override;
    /* 08 */ virtual void Update() override;

    void InitDisplay();
    void LoadGraphics();
    void Draw();
};
