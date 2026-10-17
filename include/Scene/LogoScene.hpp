#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

// Overlay 0 scene with vtable 0x02226e60. States 0-2 run only while data_020d433c.mUnk_81 is 0 (state 0 sets it):
// create UnkScene_02226e4c, wait for its request, then store GetSlotStatus(0) in data_020d433c.mSaveSlotContent
// (NO_MAGIC 0, MARKER 1, OK 2; ERROR opens MessageScreen(MESSAGE_SCREEN_SYSTEM_5_HALT)). States 3-10 show three
// pairs of /logo sets between fades, then request SCENE_TITLE.
class LogoScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ u16 mWaitFrames;
    /* 0e */ u8 mState;
    /* 0f */

    LogoScene();

    /* 00 */ virtual ~LogoScene() override;
    /* 08 */ virtual void Update() override; // 0221c71c
    /* 0c */

    void InitDisplay(); // 0221cf30
    void LoadNowLogo();
    void LoadMobiclipLogo();
    void LoadLegaltextLogo(); // english set when data_020d433c.mLanguage is 0, else french
    void LoadCriLogo();
    void LoadNelvanaLogo();
    BOOL WaitFrames(u32 maxFrames, u32 minFrames); // 0221d034
};
