
void LoadActivisionLogo();
#include "Scene/LogoScene.hpp"
#include "Scene/UnkScene_ov000.hpp"
#include "Game/GameData.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/InputManager.hpp"
#include "System/TouchManager.hpp"
#include "UI/MessageScreen.hpp"
#include <nitro/gx.h>
#include "System/FileSystem.hpp"
#include "System/SaveManager.hpp"

LogoScene::LogoScene() :
    SceneBase(2, 0) {
    mState = 0;
}

LogoScene::~LogoScene() {
    gGameData.mSkipTitleMovie = 0;
}

void LogoScene::Update() {
    switch (mState) {
        case 0:
            SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
            if (gGameData.mUnk_81 == 0) {
                gGameData.mUnk_81 = 1;
                new UnkScene_02226e4c();
                mContext->mRequest = 0;
                mState++;
            } else {
                mState = 3;
            }
            break;
        case 1:
            if (mContext->mRequest != 0) {
                mState++;
            }
            break;
        case 2:
            switch (SaveManager::Get().GetSlotStatus(0)) {
                case SAVE_STATUS_ERROR:
                    new MessageScreen(MESSAGE_SCREEN_SYSTEM_5_HALT);
                    break;
                case SAVE_STATUS_NO_MAGIC:
                    gGameData.mSaveSlotContent = 0;
                    mState++;
                    break;
                case SAVE_STATUS_MARKER:
                    gGameData.mSaveSlotContent = 1;
                    mState++;
                    break;
                case SAVE_STATUS_OK:
                    gGameData.mSaveSlotContent = 2;
                    mState++;
                    break;
            }
            break;
        case 3:
            InitDisplay();
            mState++;
            break;
        case 4:
            LoadActivisionLogo();
            LoadNelvanaLogo();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mWaitFrames = 0;
            mState++;
            break;
        case 5:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (!WaitFrames(0x78, 0)) {
                return;
            }
            SpriteManager::Get().StartFade(FADE_MODE_UP);
            mState++;
            break;
        case 6:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            LoadNowLogo();
            LoadCriLogo();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mWaitFrames = 0;
            mState++;
            break;
        case 7:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (!WaitFrames(0x78, 0)) {
                return;
            }
            SpriteManager::Get().StartFade(FADE_MODE_UP);
            mState++;
            break;
        case 8:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            LoadLegaltextLogo();
            LoadMobiclipLogo();
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mWaitFrames = 0;
            mState++;
            break;
        case 9:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (!WaitFrames(0x3c, 0x3c)) {
                return;
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mState++;
            break;
        case 10:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            RequestScene(SCENE_TITLE);
            mFinished = 1;
            break;
    }
}

void LogoScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 1);
    GX_SetGraphicsMode(1, 5, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 0, 0, 1, 0);
    G2_SetBG1Control(0, 1, 1, 3, 0);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 1, 3, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
}

// Singleton at data_020c17a8 (guard data_020c17d8); the destructor is func_02020a44.


BOOL LogoScene::WaitFrames(u32 maxFrames, u32 minFrames) {
    BOOL done = FALSE;
    mWaitFrames++;
    if (mWaitFrames >= minFrames) {
        if (TouchManager::Get().mPressed | (InputManager::Get().IsPressed(1) | InputManager::Get().IsPressed(8))) {
            done = TRUE;
        }
    }
    if (mWaitFrames >= maxFrames) {
        done = TRUE;
    }
    return done;
}

void LoadActivisionLogo() {
    ChangeDir("/logo");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_PLTT, 0, "logo_activision_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "logo_activision_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "logo_activision_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    GX_SetVisiblePlane(0x11);
}

void LogoScene::LoadNowLogo() {
    ChangeDir("/logo");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_PLTT, 0, "logo_now_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_CHAR, 0, "logo_now_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG0_SCR, 0, "logo_now_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    GX_SetVisiblePlane(0x11);
}

void LogoScene::LoadMobiclipLogo() {
    ChangeDir("/logo");
    LoadAndQueueUpload(0, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "logo_mobiclip_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG1_CHAR, 0, "logo_mobiclip_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, UPLOAD_KIND_BG1_SCR, 0, "logo_mobiclip_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    GX_SetVisiblePlane(0x12);
}

void LogoScene::LoadLegaltextLogo() {
    ChangeDir("/logo");
    if (gGameData.mLanguage == 0) {
        LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "logo_legaltext_english_ncl.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "logo_legaltext_english_ncg.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "logo_legaltext_english_nsc.bin", 0, 0, 0);
    } else {
        LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "logo_legaltext_french_ncl.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "logo_legaltext_french_ncg.bin", 0, 0, 0);
        LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "logo_legaltext_french_nsc.bin", 0, 0, 0);
    }
    ChangeDir("/");
    GXS_SetVisiblePlane(0x12);
}

void LogoScene::LoadCriLogo() {
    ChangeDir("/logo");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "logo_cri_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "logo_cri_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "logo_cri_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    GXS_SetVisiblePlane(0x12);
}

void LogoScene::LoadNelvanaLogo() {
    ChangeDir("/logo");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0x2000, "logo_nelvana_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_CHAR, 0, "logo_nelvana_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG1_SCR, 0, "logo_nelvana_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    GXS_SetVisiblePlane(0x12);
}
