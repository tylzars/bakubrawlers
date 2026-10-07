#include "Scene/UnkScene_ov000.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/InputManager.hpp"
#include "System/TouchManager.hpp"
#include "Unknown/UnkStruct_02071868.hpp"
#include <nitro/gx.h>

UnkScene_02226e4c::UnkScene_02226e4c() :
    SceneBase(1000, 0) {
    mObj0c = NULL;
    mObj10 = NULL;
    mState = 0;
}

UnkScene_02226e4c::~UnkScene_02226e4c() {
    delete mObj10;
    delete mObj0c;
}

void UnkScene_02226e4c::Update() {
    switch (mState) {
        case 0: {
            InitDisplay();
            UnkStruct_020073d8 info;
            func_020073d8(&info);
            if (info.mKind == 1) {
                data_020d433c.mUnk_79 = 0;
                mState = 8;
            } else if (info.mKind == 2) {
                data_020d433c.mUnk_79 = 1;
                mState = 8;
            } else {
                data_020d433c.mUnk_79 = 0;
                mObj0c = new UnkStruct_02071868(2);
                mState = 3;
            }
            break;
        }
        case 3:
            if (InputManager::Get().IsHeld(1) | TouchManager::Get().mUnk_57) {
                delete mObj0c;
                mObj0c = NULL;
                mState = 8;
            }
            break;
        case 8:
            RequestScene(1);
            mFinished = 1;
            break;
    }
}

void UnkScene_02226e4c::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2_SetBG0Priority(0);
    G2S_SetBG0Priority(0);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

UnkScene_02226e60::UnkScene_02226e60() :
    SceneBase(2, 0) {
    mState = 0;
}

UnkScene_02226e60::~UnkScene_02226e60() {
    data_020d433c.mUnk_80 = 0;
}

void UnkScene_02226e60::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 5, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 0, 0, 1, 0);
    G2_SetBG1Control(0, 1, 1, 3, 0);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 1, 3, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
}

BOOL UnkScene_02226e60::WaitFrames(u32 maxFrames, u32 minFrames) {
    BOOL done = FALSE;
    mFrames++;
    if (mFrames >= minFrames) {
        if (TouchManager::Get().mUnk_57 | (InputManager::Get().IsHeld(1) | InputManager::Get().IsHeld(8))) {
            done = TRUE;
        }
    }
    if (mFrames >= maxFrames) {
        done = TRUE;
    }
    return done;
}
