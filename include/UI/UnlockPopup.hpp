#pragma once

#include "global.h"
#include "types.h"
#include "Graphics/CellAnimation.hpp"
#include "Scene/SceneBase.hpp"
#include "Text/TextManager.hpp"
#include "UI/SelectWindow.hpp"

class UnlockPopup : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ SelectWindow *mWindow;
    /* 10 */ MessageFile *mMessages;
    /* 14 */ void *mTextBuffer;
    /* 18 */ void *mGlyphBuffer;
    /* 1c */ u32 mTextBufferSize;
    /* 20 */ u32 mGlyphCount;
    /* 24 */ u8 mState;
    /* 25 */ u8 mIndex;
    /* 26 */ PAD(0x26, 0x28);
    /* 28 */

    UnlockPopup(s32 unk1);
    /* 00 */ virtual ~UnlockPopup() override;
    /* 08 */ virtual void Update() override;

    void InitDisplay();
    void Load();
    void Draw();
    void PrintMessage();
    void ClearText();
};
