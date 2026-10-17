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

extern void *func_02030a44(void *list, void *obj);
extern void NNS_FndAppendListObject(void *list, void *obj);
extern void NNS_FndRemoveListObject(void *list, void *obj);
extern void NNS_FndInitList(void *list, u16 off);
extern void *CreateFrmHeapInRange(void *start, u32 size, u16 opt);
extern void UnlinkFrmHeap(void *h);
extern void *FrmHeapAlloc(void *h, u32 size, s32 align);
extern void ResetFrmHeapByMask(void *h, u32 mode);
extern int PushFrmState(void *h, u32 tag);
extern int RestoreFrmState(void *h, u32 tag);
extern SndArc *SetCurrentSndArc(SndArc *arc);
extern SndArc *GetCurrentSndArc(void);
extern void *GetSeqInfo(int idx);
extern void *func_02037ddc(int idx);
extern void *GetBankInfo(int idx);
extern void *GetWaveArcInfo(int idx);
extern void *GetPlayerInfo(int idx);
extern void *GetGroupInfo(int idx);
extern u32 GetFatEntrySize(u32 idx);
extern int ReadFatEntryData(u32 idx, void *dst, int len, u32 off);
extern void *AllocSndHeapBlock(SndHeap *h, u32 size, void *cb, u32 a, u32 b);
extern int AppendSndHeapState(SndHeap *h);
extern void FlushSndCommandsAndWait(void);
extern void ResetSndHeap(SndHeap *h);
extern void InitSndHeapBlockList(void *list);
extern int InitSndHeap(SndHeap *h, void *frm);
extern u32 GetFatEntryCache(u32 idx);
extern void SetFatEntryCache(u32 idx, u32 v);
#endif
