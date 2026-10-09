// Shared types for the NNS sound heap / archive loaders (0x020381b0-0x0203913c).
#ifndef SND_C_H
#define SND_C_H
#include <nnsys/private/nns_types.h>

typedef struct { u32 archive, file; } SndFileId;
typedef struct { u32 offset, size, cache, b; } SndArcFatEnt;       // cache: loaded copy of the file (0 if none)
typedef struct { u32 pad; u32 pad4; u32 count; SndArcFatEnt ent[1]; } SndArcFat;
typedef struct SndArc {
    u32 hdr0[4];
    u32 symbOff, symbSize, infoOff, infoSize, fatOff, fatSize;
    u32 pad28[2];
    u32 opened;
    u8 file[0x48];
    SndFileId fileId;        // 0x7c
    SndArcFat *fat;          // 0x84
    void *symb;              // 0x88
    u8 *info;                // 0x8c
    u32 chunk;               // 0x90
} SndArc;
extern SndArc *data_022149b0;

// Sound heap (NNSSndHeap): a frame heap plus a list of save-state blocks.
typedef struct SndHeapBlk {          // allocation header, user data follows at +0x20
    NNSFndLink link;                 // 0x00
    u32 size;                        // 0x08
    void (*cb)(void *ptr, u32 size, u32 a, u32 b);   // 0x0c  called when the block is freed
    u32 a;                           // 0x10
    u32 b;                           // 0x14
    u32 pad[2];
} SndHeapBlk;
typedef struct SndHeap {
    void *heap;                      // 0x00 frame heap
    NNSFndList states;               // 0x04 state blocks (each a NNSFndList of SndHeapBlk), link offset 0xc
} SndHeap;

extern void *func_02030a44(void *list, void *obj);   // NNS_FndGetPrevListObject
extern void NNS_FndAppendListObject(void *list, void *obj);    // NNS_FndAppendListObject
extern void NNS_FndRemoveListObject(void *list, void *obj);    // NNS_FndRemoveListObject
extern void NNS_FndInitList(void *list, u16 off);      // NNS_FndInitList
extern void *func_020311a8(void *start, u32 size, u16 opt);   // NNS_FndCreateFrmHeapEx
extern void func_020311e0(void *h);
extern void *func_020311ec(void *h, u32 size, s32 align);
extern void func_02031220(void *h, u32 mode);
extern int func_0203124c(void *h, u32 tag);
extern int func_0203129c(void *h, u32 tag);
extern SndArc *func_02037d50(SndArc *arc);
extern SndArc *func_02037d68(void);
extern void *func_02037d78(int idx);
extern void *func_02037ddc(int idx);
extern void *func_02037e40(int idx);
extern void *func_02037ea4(int idx);
extern void *func_02037f6c(int idx);
extern void *func_02038034(int idx);
extern u32 func_020380c0(u32 idx);
extern int func_020380e8(u32 idx, void *dst, int len, u32 off);
extern void *func_02038390(SndHeap *h, u32 size, void *cb, u32 a, u32 b);
extern int func_0203855c(SndHeap *h);
extern void func_02038598(void);
extern void func_020382d4(SndHeap *h);
extern void func_02038518(void *list);
extern int func_02038528(SndHeap *h, void *frm);
extern u32 func_020381d0(u32 idx);
extern void func_020381f8(u32 idx, u32 v);
#endif
