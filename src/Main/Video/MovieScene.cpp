#include "Video/MovieScene.hpp"

#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include <nitro/gx.h>

void RewindUploadQueue();
extern "C" void GX_SetBankForSubBG(u32 a);
extern "C" void GX_SetBankForSubBGExtPltt(u32 a);

MovieScene::MovieScene(void *video, u32 unk2)
    : SceneBase(2, 0) {
    mPlayer = new MoviePlayer();
    mPlayer->Open(video, 0, 1, unk2);
    RequestScene(0);
    G2x_SetMasterBrightness_((vu16 *) 0x400006c, -16);
    G2x_SetMasterBrightness_((vu16 *) 0x400106c, -16);
    SpriteManager::Get().SetFade(FADE_MODE_HOLD, -16);
    GX_SetBankForSubBG(4);
    GX_SetBankForSubBGExtPltt(0x80);
    GXS_SetGraphicsMode(5);
    vu16 *bg0cnt = (vu16 *) 0x4001008;
    *bg0cnt      = (*bg0cnt & 0x43) | 0x84;
    ChangeDir("/Game/2D/union_b");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "union_b_bg_wallpaper_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "union_b_bg_wallpaper_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "union_b_bg_wallpaper_ncl.bin", 0, 0, 0);
    ChangeDir("/");
    vu32 *dispcnt = (vu32 *) 0x4001000;
    *dispcnt      = (*dispcnt & ~0x1f00) | 0x100;
    vu16 *bg1cnt  = (vu16 *) 0x4001008;
    *bg1cnt       = *bg1cnt & ~3;
    *(vu32 *) 0x4001010 = 0;
    RewindUploadQueue();
    SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
    mPlaybackDone = 0;
}

MovieScene::~MovieScene() {
    delete mPlayer;
}

void MovieScene::Update() {
    if (mPlaybackDone) {
        SpriteManager &manager = SpriteManager::Get();
        if (manager.IsFading()) {
            return;
        }
        RequestScene(1);
        mFinished = 1;
        return;
    }
    if (mPlayer->Update()) {
        mPlayer->Close();
        SpriteManager::Get().StartFade(FADE_MODE_DOWN);
        mPlaybackDone = 1;
        return;
    }
    if (InputManager::Get().IsPressed(8)) {
        mPlayer->SeekToEnd();
    }
}
