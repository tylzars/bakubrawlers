#include "Scene/UnkScene_ov000.hpp"
#include "Unknown/UnkStruct_020c17a8.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/InputManager.hpp"
#include "System/TouchManager.hpp"
#include "Unknown/UnkStruct_02071868.hpp"
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

// Singleton at data_020c17a8 (guard data_020c17d8); the destructor is func_02020a44.
void func_ov000_0221d150();

void UnkScene_02226e60::Update() {
    switch (mState) {
        case 0:
            SpriteManager::Get().SetFade(1, -16);
            if (data_020d433c.mUnk_81 == 0) {
                data_020d433c.mUnk_81 = 1;
                new UnkScene_02226e4c();
                mContext->mUnk_12 = 0;
                mState++;
            } else {
                mState = 3;
            }
            break;
        case 1:
            if (mContext->mUnk_12 != 0) {
                mState++;
            }
            break;
        case 2:
            switch (UnkStruct_020c17a8::Get().func_02023c38(0)) {
                case 1:
                    new UnkStruct_02071868(0);
                    break;
                case 2:
                    data_020d433c.mUnk_77 = 0;
                    mState++;
                    break;
                case 5:
                    data_020d433c.mUnk_77 = 1;
                    mState++;
                    break;
                case 4:
                    data_020d433c.mUnk_77 = 2;
                    mState++;
                    break;
            }
            break;
        case 3:
            InitDisplay();
            mState++;
            break;
        case 4:
            func_ov000_0221d150();
            func_ov000_0221d5d4();
            SpriteManager::Get().StartFade(2);
            mFrames = 0;
            mState++;
            break;
        case 5:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (!WaitFrames(0x78, 0)) {
                return;
            }
            SpriteManager::Get().StartFade(4);
            mState++;
            break;
        case 6:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            func_ov000_0221d21c();
            func_ov000_0221d500();
            SpriteManager::Get().StartFade(2);
            mFrames = 0;
            mState++;
            break;
        case 7:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (!WaitFrames(0x78, 0)) {
                return;
            }
            SpriteManager::Get().StartFade(4);
            mState++;
            break;
        case 8:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            func_ov000_0221d3b4();
            func_ov000_0221d2e8();
            SpriteManager::Get().StartFade(2);
            mFrames = 0;
            mState++;
            break;
        case 9:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (!WaitFrames(0x3c, 0x3c)) {
                return;
            }
            SpriteManager::Get().StartFade(3);
            mState++;
            break;
        case 10:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            RequestScene(4);
            mFinished = 1;
            break;
    }
}

void func_ov000_0221d150() {
    ChangeDir("/logo");
    func_02025b64(0, 6, 0, "logo_activision_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "logo_activision_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "logo_activision_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    GX_SetVisiblePlane(0x11);
}

void UnkScene_02226e60::func_ov000_0221d21c() {
    ChangeDir("/logo");
    func_02025b64(0, 6, 0, "logo_now_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "logo_now_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "logo_now_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    GX_SetVisiblePlane(0x11);
}

void UnkScene_02226e60::func_ov000_0221d2e8() {
    ChangeDir("/logo");
    func_02025b64(0, 1, 0x2000, "logo_mobiclip_ncl.bin", 0, 0, 0);
    func_02025b64(0, 12, 0, "logo_mobiclip_ncg.bin", 0, 0, 0);
    func_02025b64(0, 8, 0, "logo_mobiclip_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    GX_SetVisiblePlane(0x12);
}

void UnkScene_02226e60::func_ov000_0221d3b4() {
    ChangeDir("/logo");
    if (data_020d433c.mUnk_79 == 0) {
        func_02025b64(1, 1, 0x2000, "logo_legaltext_english_ncl.bin", 0, 0, 0);
        func_02025b64(1, 12, 0, "logo_legaltext_english_ncg.bin", 0, 0, 0);
        func_02025b64(1, 8, 0, "logo_legaltext_english_nsc.bin", 0, 0, 0);
    } else {
        func_02025b64(1, 1, 0x2000, "logo_legaltext_french_ncl.bin", 0, 0, 0);
        func_02025b64(1, 12, 0, "logo_legaltext_french_ncg.bin", 0, 0, 0);
        func_02025b64(1, 8, 0, "logo_legaltext_french_nsc.bin", 0, 0, 0);
    }
    ChangeDir("/");
    GXS_SetVisiblePlane(0x12);
}

void UnkScene_02226e60::func_ov000_0221d500() {
    ChangeDir("/logo");
    func_02025b64(1, 1, 0x2000, "logo_cri_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "logo_cri_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "logo_cri_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    GXS_SetVisiblePlane(0x12);
}

void UnkScene_02226e60::func_ov000_0221d5d4() {
    ChangeDir("/logo");
    func_02025b64(1, 1, 0x2000, "logo_nelvana_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "logo_nelvana_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "logo_nelvana_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    GXS_SetVisiblePlane(0x12);
}
