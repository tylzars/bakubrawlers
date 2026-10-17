#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"

#include <nitro/os.h>

extern "C" int G2x_GetMasterBrightness_(vu16 *reg);
extern "C" BOOL func_0200d924();

#define UPLOAD_QUEUE_SIZE 32
#define UPLOAD_KIND_COUNT 15
#define UPLOAD_FREE_COUNT 32

typedef void (*UploadFunc)(void *data, u32 offset, u32 size);
typedef void (*BankFunc)();

struct UploadEntry {
    /* 00 */ void *mData;
    /* 04 */ u32 mOffset;
    /* 08 */ u32 mSize;
    /* 0c */ u8 mKind;
    /* 0d */ u8 mFree;
};


// VRAM/BG upload helpers (GX/G2 region of main).
extern "C" void GXS_LoadBGPltt(void *data, u32 offset, u32 size);
extern "C" void GX_LoadOBJ(void *data, u32 offset, u32 size);
extern "C" void GX_LoadBGPltt(void *data, u32 offset, u32 size);
extern "C" void GX_LoadBG0Scr(void *data, u32 offset, u32 size);
extern "C" void GX_LoadBG1Scr(void *data, u32 offset, u32 size);
extern "C" void GX_LoadBG2Scr(void *data, u32 offset, u32 size);
extern "C" void GX_LoadBG3Scr(void *data, u32 offset, u32 size);
extern "C" void GX_LoadBG0Char(void *data, u32 offset, u32 size);
extern "C" void GX_LoadBG1Char(void *data, u32 offset, u32 size);
extern "C" void GX_LoadBG2Char(void *data, u32 offset, u32 size);
extern "C" void GX_LoadBG3Char(void *data, u32 offset, u32 size);
extern "C" void GX_LoadSubOBJExtPltt(void *data, u32 offset, u32 size);
extern "C" void GX_LoadSubBGExtPltt(void *data, u32 offset, u32 size);
extern "C" void GX_LoadTex(void *data, u32 offset, u32 size);
extern "C" void GX_LoadTexPltt(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadOBJ(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadOBJPltt(void *data, u32 offset, u32 size);
extern "C" void GX_LoadOBJPltt(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadBG0Scr(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadBG1Scr(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadBG2Scr(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadBG3Scr(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadBG0Char(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadBG1Char(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadBG2Char(void *data, u32 offset, u32 size);
extern "C" void GXS_LoadBG3Char(void *data, u32 offset, u32 size);
extern "C" void GX_LoadOBJExtPltt(void *data, u32 offset, u32 size);
extern "C" void GX_LoadBGExtPltt(void *data, u32 offset, u32 size);
extern "C" void GX_EndLoadOBJExtPltt();
extern "C" void GX_EndLoadBGExtPltt();
extern "C" void GX_EndLoadTex();
extern "C" void GX_EndLoadTexPltt();
extern "C" void GX_EndLoadSubOBJExtPltt();
extern "C" void GX_EndLoadSubBGExtPltt();
extern "C" void GX_BeginLoadOBJExtPltt();
extern "C" void GX_BeginLoadBGExtPltt();
extern "C" void GX_BeginLoadTex();
extern "C" void GX_BeginLoadTexPltt();
extern "C" void GX_BeginLoadSubOBJExtPltt();
extern "C" void GX_BeginLoadSubBGExtPltt();

BankFunc gEndLoadFuncs[8]  = {GX_EndLoadOBJExtPltt, GX_EndLoadBGExtPltt, GX_EndLoadTex, GX_EndLoadTexPltt,
                              GX_EndLoadSubOBJExtPltt, GX_EndLoadSubBGExtPltt, GX_EndLoadTex, GX_EndLoadTexPltt};
BankFunc gBeginLoadFuncs[8]  = {GX_BeginLoadOBJExtPltt, GX_BeginLoadBGExtPltt, GX_BeginLoadTex, GX_BeginLoadTexPltt,
                              GX_BeginLoadSubOBJExtPltt, GX_BeginLoadSubBGExtPltt, GX_BeginLoadTex, GX_BeginLoadTexPltt};
UploadFunc gUploadFuncs[30] = {
    GX_LoadOBJExtPltt, GX_LoadBGExtPltt, GX_LoadTex, GX_LoadTexPltt, GX_LoadOBJ, GXS_LoadBGPltt, GX_LoadBGPltt,
    GX_LoadBG0Scr, GX_LoadBG1Scr, GX_LoadBG2Scr, GX_LoadBG3Scr, GX_LoadBG0Char, GX_LoadBG1Char, GX_LoadBG2Char,
    GX_LoadBG3Char, GX_LoadSubOBJExtPltt, GX_LoadSubBGExtPltt, GX_LoadTex, GX_LoadTexPltt, GXS_LoadOBJ, GXS_LoadOBJPltt,
    GX_LoadOBJPltt, GXS_LoadBG0Scr, GXS_LoadBG1Scr, GXS_LoadBG2Scr, GXS_LoadBG3Scr, GXS_LoadBG0Char, GXS_LoadBG1Char,
    GXS_LoadBG2Char, GXS_LoadBG3Char};
// Zero-initialised but in .data in the original.
#pragma explicit_zero_data on
void *gPendingFrees[UPLOAD_FREE_COUNT] = {0};
#pragma explicit_zero_data reset

// The queue state is five separate globals, not one struct: as members of one struct, the entry accesses and the
// stores to the free count and the tail reorder across each other, which the original never does. Defined in this
// order they get the original's addresses (count, head, free count, tail, byte total).
u8 gUploadCount;  // queued entries
u8 gUploadReadIndex;  // head index
u8 gUploadWriteIndex;  // tail index
u8 gPendingFreeCount;  // pending frees in gPendingFrees
u32 gQueuedBytes; // bytes queued this frame
UploadEntry gUploadQueue[UPLOAD_QUEUE_SIZE];

void FlushUploadQueue() {
    u32 head = gUploadReadIndex;
    do {
        UploadEntry *e = &gUploadQueue[(u8) head];
        u32 kind       = e->mKind;
        if (kind % UPLOAD_KIND_COUNT < 4) {
            u8 k    = kind;
            u8 bank = k % UPLOAD_KIND_COUNT + (k >= UPLOAD_KIND_COUNT) * 4;
            gBeginLoadFuncs[bank]();
            gUploadFuncs[kind](gUploadQueue[gUploadReadIndex].mData, gUploadQueue[gUploadReadIndex].mOffset,
                                gUploadQueue[gUploadReadIndex].mSize);
            gEndLoadFuncs[bank]();
        } else {
            gUploadFuncs[kind](gUploadQueue[(u8) head].mData, e->mOffset, e->mSize);
        }
        if (gUploadQueue[gUploadReadIndex].mFree != 0) {
            u32 n = gPendingFreeCount++;
            gPendingFrees[n] = gUploadQueue[gUploadReadIndex].mData;
        }
        head          = (gUploadReadIndex + 1) % UPLOAD_QUEUE_SIZE;
        gUploadReadIndex = head;
    } while (--gUploadCount != 0);
}

BOOL TryUploadNow(u32 screen, u32 kind, u32 offset, void *data, u32 size, u32 freeData) {
    BOOL immediate = FALSE;
    int brightness;
    if (screen == 0) {
        brightness = G2x_GetMasterBrightness_((vu16 *) 0x400006c);
    } else if (screen == 1) {
        brightness = G2x_GetMasterBrightness_((vu16 *) 0x400106c);
    }
    if ((brightness == -16) | (brightness == 16)) {
        immediate = TRUE;
    }
    if (func_0200d924() == 0) {
        immediate = TRUE;
    }
    if (immediate) {
        u32 slot = kind + screen * UPLOAD_KIND_COUNT;
        if (slot % UPLOAD_KIND_COUNT < 4) {
            u8 s    = slot;
            u8 bank = s % UPLOAD_KIND_COUNT + (s >= UPLOAD_KIND_COUNT) * 4;
            gBeginLoadFuncs[bank]();
            gUploadFuncs[slot](data, offset, size);
            gEndLoadFuncs[bank]();
        } else {
            gUploadFuncs[slot](data, offset, size);
        }
        if (freeData != 0) {
            OS_Free(data);
        }
    }
    return immediate;
}

BOOL ReplaceQueuedUpload(u32 screen, u32 kind, u32 offset, void *data, u32 size, u32 freeData) {
    u8 n = gUploadCount;
    if (n == 0) {
        return FALSE;
    }
    u32 slot = kind + screen * UPLOAD_KIND_COUNT;
    u32 end  = offset + size;
    u8 i     = gUploadReadIndex;
    do {
        if (slot == gUploadQueue[i].mKind) {
            BOOL after  = offset <= gUploadQueue[i].mOffset;
            BOOL before = end >= gUploadQueue[i].mOffset + gUploadQueue[i].mSize;
            if (after & before) {
                if (gUploadQueue[i].mFree != 0) {
                    OS_Free(gUploadQueue[i].mData);
                }
                gUploadQueue[i].mOffset = offset;
                gUploadQueue[i].mData   = data;
                gUploadQueue[i].mSize   = size;
                gUploadQueue[i].mFree   = freeData;
                return TRUE;
            }
        }
        i = (i + 1) % UPLOAD_QUEUE_SIZE;
    } while (--n != 0);
    return FALSE;
}

static inline void FlushPendingFrees() {
    while (gPendingFreeCount != 0) {
        gPendingFreeCount--;
        OS_Free(gPendingFrees[gPendingFreeCount]);
    }
}

u32 LoadAndQueueUpload(u32 screen, u32 kind, u32 offset, const char *path, u32 unk5, u32 unk6, u32 unk7) {
    FlushPendingFrees();
    u32 size;
    void *data = LoadFile(path, 0, unk5, unk6, &size, unk7);
    return QueueUpload(screen, kind, offset, data, size, 1);
}

u32 QueueUpload(u32 screen, u32 kind, u32 offset, void *data, u32 size, u32 freeData) {
    FlushPendingFrees();
    if (TryUploadNow(screen, kind, offset, data, size, freeData)) {
        return offset + size;
    }
    if (ReplaceQueuedUpload(screen, kind, offset, data, size, freeData)) {
        return offset + size;
    }
    gUploadQueue[gUploadWriteIndex].mKind   = kind + screen * UPLOAD_KIND_COUNT;
    gUploadQueue[gUploadWriteIndex].mOffset = offset;
    gUploadQueue[gUploadWriteIndex].mData   = data;
    gUploadQueue[gUploadWriteIndex].mSize   = size;
    gUploadQueue[gUploadWriteIndex].mFree   = freeData;
    gUploadWriteIndex = (gUploadWriteIndex + 1) % UPLOAD_QUEUE_SIZE;
    gUploadCount++;
    gQueuedBytes += size;
    return offset + size;
}

void UpdateUploadQueue() {
    FlushPendingFrees();
    if (gUploadCount != 0) {
        QueueFrameCallback((FrameCallback) FlushUploadQueue, NULL);
    }
    gQueuedBytes = 0;
}

void RewindUploadQueue() {
    gUploadReadIndex = 0;
    gUploadWriteIndex = 0;
    gPendingFreeCount = 0;
    gQueuedBytes = 0;
}
