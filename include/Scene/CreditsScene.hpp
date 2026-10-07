#pragma once
#include "global.h"
#include "types.h"
#include "Scene/SceneBase.hpp"

struct MessageFile;

// Overlay 0 scene with vtable 0x02227eac: the credits (files in /Game/Credit, font/mes_Credit.mes).
class CreditsScene : public SceneBase {
public:
    /* 0c */ MessageFile *mMessages;
    /* 10 */ u8 mState;
    /* 11 */ u8 mTick;
    /* 12 */ s16 mSubScroll;
    /* 14 */ u32 mBufferSize;
    /* 18 */ void *mBuffers[2];
    /* 20 */ u32 mScroll[2];
    /* 28 */ s16 mMessage[2];
    /* 2c */ u8 mColState[2];
    /* 2e */ u8 mTimer[2];
    /* 30 */ u8 mRow[2];
    /* 32 */

    CreditsScene();
    virtual ~CreditsScene() override;
    virtual void Update() override;

    void InitDisplay();   // 0222520c
    void LoadGraphics();  // 022253c0
    BOOL UpdateText();    // 02225734
    void PrintLine(u32 message, u32 row, u32 column, u32 shadow); // 022259b8
    void LoadText();      // 02225c04
};

void func_ov000_02225f98(void *scene); // frame callback: BG1 scroll
