#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class SelectWindow;
struct MessageFile;

// A SceneBase task, created with new and left to run itself (the result is not kept). Item 1 skips the save; item 0
// starts SaveWriter, and a result of SAVE_STATUS_ERROR creates MessageScreen(MESSAGE_SCREEN_SYSTEM_4_HALT)
// (SaveWindow.cpp:134-135); otherwise it fades out and ends with RequestScene(1) (:140-150). Every caller passes
// mScreen = 1, so the mScreen == 0 branch of Load never runs.
class SaveWindow : public SceneBase {
public:
    /* 0c */ SelectWindow *mWindow;
    /* 10 */ MessageFile *mMessages;
    /* 14 */ void *mTextBuffer;
    /* 18 */ u32 mUnk_18;
    /* 1c */ u32 mUnk_1c;
    /* 20 */ u32 mUnk_20;
    /* 24 */ u32 mUnk_24;
    /* 28 */ u32 mUnk_28;
    /* 2c */ u16 mUnk_2c;
    /* 2e */ u16 mTextBufferSize;
    /* 30 */ u8 mScreen;
    /* 31 */ u8 mWindowBg;
    /* 32 */ u8 mTextBg;
    /* 33 */ u8 mState;
    /* 34 */

    SaveWindow(u32 unk1, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6, u32 unk7, u32 unk8, u32 unk9);
    virtual ~SaveWindow();
    virtual void Update();
    void Load();
    void PrintMessage(u32 unk1);
};
