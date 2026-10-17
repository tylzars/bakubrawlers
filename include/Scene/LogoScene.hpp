#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

// Overlay 0 scene with vtable 0x02226e60.
class LogoScene : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ u16 mWaitFrames;
    /* 0e */ u8 mState;
    /* 0f */

    LogoScene();

    /* 00 */ virtual ~LogoScene() override;
    /* 08 */ virtual void Update() override; // 0221c71c, not decompiled yet
    /* 0c */

    void InitDisplay(); // 0221cf30
    void LoadNowLogo();
    void LoadMobiclipLogo();
    void LoadLegaltextLogo();
    void LoadCriLogo();
    void LoadNelvanaLogo();
    BOOL WaitFrames(u32 maxFrames, u32 minFrames); // 0221d034
};
