#pragma once

#include "global.h"
#include "types.h"
#include "Graphics/CellAnimation.hpp"
#include "Scene/SceneBase.hpp"
#include "Text/TextManager.hpp"
#include "UI/SelectWindow.hpp"

class UnkStruct_0208ab40 : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ SelectWindow *mMenu;
    /* 10 */ MessageFile *mMessages;
    /* 14 */ void *mTextBuffer;
    /* 18 */ void *mGlyphBuffer;
    /* 1c */ u32 mTextBufferSize;
    /* 20 */ u32 mGlyphCount;
    /* 24 */ u8 mState;
    /* 25 */ u8 mIndex;
    /* 26 */ PAD(0x26, 0x28);
    /* 28 */

    UnkStruct_0208ab40(s32 unk1);
    /* 00 */ virtual ~UnkStruct_0208ab40() override;
    /* 08 */ virtual void Update() override;

    void func_0208aec4();
    void func_0208af20();
    void func_0208b284();
    void func_0208b338();
    void func_0208b4a4();
};
