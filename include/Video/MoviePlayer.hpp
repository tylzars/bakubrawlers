#pragma once

#include "global.h"
#include "types.h"

struct MobiclipPlayer;

// Movie player object (0x114 bytes, ctor 0203c18c, dtor 0203c1b8): wraps the Mobiclip decoder, double buffers the
// decoded frame lines in RGB555 and shows them on the main screen while the sound is streamed with a timer callback.
class MoviePlayer {
public:
    /* 00 */ volatile s64 mUnk_00; // frames shown at the last reset
    /* 08 */ s64 mFrames; // frames decoded
    /* 10 */ MobiclipPlayer *mVideo;
    /* 14 */ MobiclipPlayer *mVideo2; // second video stream (for the lower screen)
    /* 18 */ u8 mDecoder[0x48];
    /* 60 */ u8 mDecoder2[0x48];
    /* a8 */ u8 mAlarm[0x2c];
    /* d4 */ u32 mUnk_d4;
    /* d8 */ u32 mUnk_d8;
    /* dc */ u32 mUnk_dc;
    /* e0 */ u32 mUnk_e0;
    /* e4 */ u32 mUnk_e4;
    /* e8 */ volatile u32 mUnk_e8;
    /* ec */ u32 mUnk_ec;
    /* f0 */ volatile u32 mUnk_f0;
    /* f4 */ u32 mUnk_f4;
    /* f8 */ u32 mUnk_f8;
    /* fc */ u32 mUnk_fc;
    /* 100 */ u16 *mBuffer;
    /* 104 */ u16 *mBuffer2;
    /* 108 */ u8 mUnk_108;
    /* 109 */ u8 mUnk_109;
    /* 10a */ u8 mUnk_10a;
    /* 10b */ u8 mUnk_10b;
    /* 10c */ u8 mUnk_10c;
    /* 10d */ PAD(0x10d, 0x10e);
    /* 10e */ u8 mUnk_10e;
    /* 10f */ u8 mUnk_10f;
    /* 110 */ u8 mUnk_110;
    /* 111 */ PAD(0x111, 0x114);
    /* 114 */

    MoviePlayer();
    ~MoviePlayer();
    void func_0203c1d8(u32 rate);
    void func_0203c354();
    u32 func_0203c384();
    u32 func_0203c39c();
    void func_0203c3d0();
    void func_0203c5b4(void *a, void *b, u8 c, u32 d); // d: only the low byte is used
    void func_0203c9c4();
    void func_0203ca5c();
    BOOL func_0203ca70();
};

void func_0203c448(MoviePlayer *player); // alarm handler: a frame is due
void func_0203c454(MoviePlayer *player); // vblank task: flips the screen
void func_0203c1bc(MoviePlayer *player); // alarm handler: counts a tick
