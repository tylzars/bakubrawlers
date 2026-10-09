#pragma once
// Mobiclip player internals shared by MobiclipDecoder.cpp and MobiclipFile.cpp.
#include "types.h"
extern "C" {
struct Src;
struct Src { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual int read(u8 *, u32); virtual void adv(); };
struct Ctx {
    u8 *ptr;            // 0x00
    int pad4[2];
    void *a[6];         // 0x0c
    void *b[6];         // 0x24
    u8 pad3c[0x3b4 - 0x3c];
    void *o3b4;
};
struct Chan { u8 d[0x14f8]; };
struct P {
    Src *src;
    int pad4;
    s8 c8;              // 0x08
    s8 c9;              // 0x09
    u8 pada[2];
    u32 frames;         // 0x0c
    u32 o10;
    u32 o14;
    u8 pad18[4];
    u16 o1c;
    u16 o1e;
    u32 o20;
    u8 pad24[0x34 - 0x24];
    Ctx *ctx;           // 0x34
    int (*fn38)(Ctx *);
    void *o3c, *o40, *o44; u32 o48; u32 o4c, o50, o54;
    u8 *o58;
    void **o5c, **o60, **o64;
    void *o68, *o6c;
    u8 *o70[2];
    u32 o78[2];
    u32 o80[2];
    u32 o88[2];
    int cur;
    u32 pad94;
    u32 o98, o9c, oa0, oa4, oa8;
    u32 oac[6];
    u32 oc4, oc8, occ, od0;
};
}
