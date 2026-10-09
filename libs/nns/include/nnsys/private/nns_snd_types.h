// Shared NitroSystem sound library structs (players, sound slots, streams, capture, sound archive).
#ifndef SND_TYPES_H
#define SND_TYPES_H
#include <nnsys/private/nns_types.h>

typedef struct SndHandle SndHandle;
typedef struct { s32 value; s32 target; s32 step; s32 frames; } SndRamp;   // volume ramp

typedef struct SndPlayer {
    NNSFndList sounds;       // 0x00 active sounds, ordered by priority (link offset 0xc)
    NNSFndList heaps;        // 0x0c player heaps (link offset 0)
    u32 maxSounds;           // 0x18
    u32 heapSize;            // 0x1c
    u8 volume;               // 0x20
    u8 pad[3];
} SndPlayer;                 // stride 0x24

typedef struct SndPlayerHeapBlk SndPlayerHeapBlk;
typedef struct SndSound {
    SndHandle *handle;          // 0x00
    SndPlayer *player;          // 0x04
    SndPlayerHeapBlk *heapBlk;  // 0x08
    u32 padc;                   // 0x0c
    u32 pad10;                  // 0x10
    NNSFndLink link;            // 0x14
    SndRamp ramp;               // 0x1c
    u8 status;                  // 0x2c  0 free, 1 playing, 2 fading out
    u8 started;                 // 0x2d
    u8 pad2e;
    u8 pending;                 // 0x2f
    u32 tick;                   // 0x30
    u16 kind;                   // 0x34
    u16 pad36;
    u16 idA;                    // 0x38
    u16 idB;                    // 0x3a
    u8 channel;                 // 0x3c
    u8 priority;                // 0x3d
    s16 volume;                 // 0x3e
    u8 volA;                    // 0x40
    u8 volB;                    // 0x41
    u16 pad42;
} SndSound;                     // stride 0x44

struct SndHandle { struct SndSound * volatile sound; };
struct SndPlayerHeapBlk { u32 pad0; u32 pad4; void *heap; SndSound *owner; u32 player; };

// A {callback, argument, link} record registered with the SND command layer (12 bytes).
typedef struct { void (*fn)(void *); void *arg; void *next; } SndCallbackRec;

// PCM stream (NNSSndStrm). Registered in data_022142e8.
typedef void (*SndStrmCallback)(int phase, int chCount, void **bufs, u32 size, int fmt, void *arg);
typedef struct SndStrm {
    u32 pad0[2];
    SndCallbackRec cbA;      // 0x08 stop-timer callback
    SndCallbackRec cbB;      // 0x14 drain callback
    u32 fmt;                 // 0x20
    s32 active:1;            // 0x24 bit 0
    s32 registered:1;        // 0x24 bit 1
    u32 bufSize;             // 0x28 bytes per channel
    s32 frames;              // 0x2c buffer divisions per loop
    SndStrmCallback cb;      // 0x30
    void *cbArg;             // 0x34
    s32 pos;                 // 0x38 division being filled
    u32 offset;              // 0x3c
    s32 slot;                // 0x40 timer slot
    u32 chMask;              // 0x44
    s32 chCount;             // 0x48
    u8 chan[16];             // 0x4c
} SndStrm;
typedef struct { void *addr; u32 off; } SndStrmChan;   // data_02214334, per hardware channel

// Reverb/capture state (data_022143fc)
typedef struct {
    s32 active;              // 0x00
    s32 mode;                // 0x04
    u32 pad8;
    void *buf0;              // 0x0c
    void *buf1;              // 0x10
    u32 size;                // 0x14
    u32 pad18;
    u32 pos;                 // 0x1c
    u32 capMask;             // 0x20 locked channels
    u32 chMask;              // 0x24
    u32 capSlots;            // 0x28
    s32 alarm;               // 0x2c
    u32 pad30[3];
    SndRamp ramp;            // 0x3c
    s32 rampOn;              // 0x4c
    s32 lastVol;             // 0x50
} SndCapture;

// Sound archive (SDAT) state
typedef struct { u32 count; u32 off[1]; } SndArcTable;
typedef struct { u32 offset, size, a, b; } SndArcFatEnt;
typedef struct { u32 pad; u32 pad4; u32 count; SndArcFatEnt ent[1]; } SndArcFat;
typedef struct { u32 archive, file; } SndFileId;
typedef struct SndArc {
    u32 hdr0[4];             // 0x00 file header
    u32 symbOff, symbSize;   // 0x10
    u32 infoOff, infoSize;   // 0x18
    u32 fatOff, fatSize;     // 0x20
    u32 pad28[2];
    u32 opened;              // 0x30
    u8 file[0x48];           // 0x34 FSFile
    SndFileId fileId;        // 0x7c FSFileID
    SndArcFat *fat;          // 0x84
    void *symb;              // 0x88
    u8 *info;                // 0x8c
    u32 chunk;               // 0x90
} SndArc;
#endif
