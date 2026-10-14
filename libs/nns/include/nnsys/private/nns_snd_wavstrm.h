// Types for the NNS sound file-stream player (streams ADPCM/PCM wave data from a file, 0x0203913c-0x0203a52c).
#ifndef NNS_SND_WAVSTRM_H
#define NNS_SND_WAVSTRM_H
#include <nnsys/private/nns_types.h>

typedef struct { s32 cur, target, pos, len; } SndLerp;     // linear ramp: cur..target over len steps
typedef struct { s16 pred; u16 index; } SndAdpcmState;   // index is used as a byte

typedef void (*WavStrmFillCb)(int phase, int channels, u8 **bufs, u32 size, int is16bit, void *arg);
typedef struct WavStrm WavStrm;
typedef struct { u32 slot, id; } WavStrmNextIn;
typedef struct { u32 id, extra; } WavStrmNextOut;
typedef int (*WavStrmNextFn)(int zero, WavStrmNextIn *in, WavStrmNextOut *out, u32 arg);
typedef int (*WavStrmOpenFn)(WavStrm *s, u32 id);
typedef int (*WavStrmCloseFn)(WavStrm *s);
typedef int (*WavStrmReadFn)(WavStrm *s, void *dst, u32 len, u32 off);
typedef int (*WavStrmDoneFn)(WavStrm *s);

struct WavStrm {                 // stride 0x174, 4 of them at data_0221508c
    u8 pad00[0x5c];
    u8 file[0x48];               // 0x5c FSFile
    u32 dataBase;                // 0xa4 file offset of the stream data
    u8 hdrPad[0x18];             // 0xa8 stream header copy (0x40 bytes to 0xe8)
    u8 format;                   // 0xc0 0 PCM8, 1 PCM16, 2 ADPCM
    u8 loop;                     // 0xc1
    u8 channels;                 // 0xc2
    u8 pad_c3;
    u16 sampleRate;              // 0xc4
    u16 pad_c6;
    u32 loopStart;               // 0xc8
    u32 pad_cc;
    u32 dataOff;                 // 0xd0
    u32 blocks;                  // 0xd4
    u32 blockSize;               // 0xd8
    u32 blockSamples;            // 0xdc
    u32 lastBlockSize;           // 0xe0
    u32 lastBlockSamples;        // 0xe4
    SndLerp pan;                 // 0xe8
    SndAdpcmState adpcm[2];      // 0xf8
    u8 pad100[0x10];
    s32 active:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1;   // 0x110 flags
    volatile s32 delay;          // 0x114
    s32 f118;                    // 0x118
    s32 queued;                  // 0x11c
    s32 cnt120;                  // 0x120
    u8 chCount;                  // 0x124
    u8 pad125;
    u8 chan[6];                  // 0x126
    void *buf;                   // 0x12c
    u32 bufSize;                 // 0x130
    WavStrmFillCb fillCb;        // 0x134 called after each buffer is produced
    void *fillArg;               // 0x138
    WavStrmNextFn next;   // 0x13c
    u32 f140;                    // 0x140
    u32 f144;                    // 0x144 file id being played
    u32 slot;                    // 0x148
    u32 *ptr14c;                 // 0x14c
    u32 f150;                    // 0x150
    u32 f154;                    // 0x154
    u32 f158;                    // 0x158
    s32 f15c;                    // 0x15c
    u32 pos;                     // 0x160
    WavStrmOpenFn open;          // 0x164
    WavStrmCloseFn close;        // 0x168
    WavStrmReadFn read;          // 0x16c
    WavStrmDoneFn done;          // 0x170
};

// Command node queued to the stream thread (0x30 bytes, 8 of them at data_02214a20).
typedef struct {
    NNSFndLink link;             // 0x00
    WavStrm *strm;               // 0x08
    s32 phase;                   // 0x0c
    s32 channels;                // 0x10
    u8 *buf[6];                  // 0x14 per-channel output buffers
    u32 size;                    // 0x2c bytes to produce
} WavStrmCmd;

// Stream thread manager (data_02214ba0).
typedef struct {
    u8 thread[0xc0];             // 0x000 OSThread
    u8 stack[0x400];             // 0x0c0
    u8 sleepQueue[8];            // 0x4c0 OSThreadQueue
    u8 mutex[0x18];              // 0x4c8
    NNSFndList cmds;             // 0x4e0
} WavStrmMgr;

typedef struct { int init; WavStrmMgr *mgr; u16 *adpcmBuf; } WavStrmGlobals;   // data_022149f0

extern WavStrm data_0221508c[4];
extern WavStrmGlobals data_022149f0;
extern u8 data_02215068[], data_02214a08[];
extern NNSFndList data_02215080, data_022149fc;
extern WavStrmCmd data_02214a20[8];
extern WavStrmMgr data_02214ba0;
extern u16 data_02215660[];
#endif
