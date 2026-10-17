#pragma once

#include "global.h"
#include "types.h"

struct Player;

// Movie player object (0x114 bytes, ctor 0203c18c, dtor 0203c1b8): wraps the Mobiclip decoder. Update writes blocks
// from func_0203a79c into the ring mBuffer (and mBuffer2), which StartSound hands to func_02008160, with CountTick as
// func_02008098's callback; DrawFrame passes GetVideoTarget() to func_0203a73c.
class MoviePlayer {
public:
    /* 00 */ volatile s64 mUnk_00; // CountTick calls since Open or the last reset
    /* 08 */ s64 mFrames; // blocks (mUnk_fc elements each) written into mBuffer; the Update reset subtracts mUnk_00
    /* 10 */ Player *mVideo;
    /* 14 */ Player *mVideo2; // second stream; never opened in this build (the only Open call passes b = 0)
    /* 18 */ u8 mFile[0x48];
    /* 60 */ u8 mFile2[0x48];
    /* a8 */ u8 mAlarm[0x2c];
    /* d4 */ u32 mUnk_d4;
    /* d8 */ u32 mUnk_d8;
    /* dc */ u32 mFrameCount;
    /* e0 */ u32 mFrameIndex;
    /* e4 */ u32 mUnk_e4;
    /* e8 */ volatile u32 mFrameDue;
    /* ec */ u32 mVramPage;
    /* f0 */ volatile u32 mFlipDone;
    /* f4 */ u32 mWriteOffset;
    /* f8 */ u32 mUnk_f8;
    /* fc */ u32 mUnk_fc;
    /* 100 */ u16 *mBuffer;
    /* 104 */ u16 *mBuffer2;
    /* 108 */ u8 mUseBuffer2; // always 1 in this build (MovieScene passes c = 1); Update steps the audio channel loop by mUseBuffer2 + 1
    /* 109 */ u8 mSoundStarted;
    /* 10a */ u8 mUnk_10a;
    /* 10b */ u8 mUnk_10b;
    /* 10c */ u8 mUnk_10c;
    /* 10d */ PAD(0x10d, 0x10e);
    /* 10e */ u8 mUseVideo2; // always 0 in this build (MovieScene passes b = 0)
    /* 10f */ u8 mAudioChannelCount;
    /* 110 */ u8 mAudioChannelIndex;
    /* 111 */ PAD(0x111, 0x114);
    /* 114 */

    MoviePlayer();
    ~MoviePlayer();
    void StartSound(u32 rate);
    void StopSound();
    u32 GetVideoTarget();
    u32 GetVideo2Target();
    void DrawFrame();
    void Open(void *a, void *b, u8 c, u32 d); // d: only the low byte is used
    void Close();
    // Sets mFrameIndex = mFrameCount (and mUnk_10b, which nothing reads), so Update returns TRUE on its next frame; the
    // decoder is not moved.
    void SeekToEnd();
    BOOL Update();
};

void SetFrameDue(MoviePlayer *player); // alarm handler: a frame is due
void FlipVramPage(MoviePlayer *player); // vblank task: flips the screen
void CountTick(MoviePlayer *player); // alarm handler: counts a tick
