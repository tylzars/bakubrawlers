#pragma once

#include "global.h"
#include "types.h"
#include "Graphics/CellAnimation.hpp"
#include "Scene/SceneBase.hpp"
#include "UI/SelectWindow.hpp"

class UnkStruct_02089f90 : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ CellAnimation *mCell;
    /* 10 */ CellAnimation *mCells[2];
    /* 18 */ SelectWindow *mMenu;
    /* 1c */ s16 mOffset;
    /* 1e */ u8 mUnk_1e;
    /* 1f */ u8 mState;
    /* 20 */ s8 mIndex;
    /* 21 */ PAD(0x21, 0x24);
    /* 24 */

    UnkStruct_02089f90(s32 unk1);
    /* 00 */ virtual ~UnkStruct_02089f90() override;
    /* 08 */ virtual void Update() override;

    void func_0208a454();
    void func_0208a598();
    void func_0208aa5c();
};
