#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"
#include "Video/MoviePlayer.hpp"

// Scene that plays one movie (Mobiclip) on the main screen over a wallpaper background; vtable 020ac790, 0x14 bytes.
class MovieScene : public SceneBase {
public:
    /* 00 SceneBase */
    /* 0c */ MoviePlayer *mPlayer;
    /* 10 */ u8 mUnk_10; // 0 playing, 1 finished and waiting for the fade
    /* 11 */ PAD(0x11, 0x14);
    /* 14 */

    MovieScene(void *video, u32 unk2);
    virtual ~MovieScene();
    virtual void Update();
};
