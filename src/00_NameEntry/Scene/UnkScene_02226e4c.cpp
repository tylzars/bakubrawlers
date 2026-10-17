
void LoadActivisionLogo();
#include "Scene/UnkScene_ov000.hpp"
#include "System/SaveManager.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/TouchManager.hpp"
#include "System/InputManager.hpp"
#include "UI/MessageScreen.hpp"
#include <nitro/gx.h>
#include "System/FileSystem.hpp"

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
            OwnerInfo info;
            OS_GetOwnerInfo(&info);
            if (info.mKind == 1) {
                data_020d433c.mLanguage = 0;
                mState = 8;
            } else if (info.mKind == 2) {
                data_020d433c.mLanguage = 1;
                mState = 8;
            } else {
                data_020d433c.mLanguage = 0;
                mObj0c = new MessageScreen(MESSAGE_SCREEN_TITLE_8);
                mState = 3;
            }
            break;
        }
        case 3:
            if (InputManager::Get().IsPressed(1) | TouchManager::Get().mPressed) {
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
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
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
