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

struct UploadQueue {
    /* 0 */ u8 mCount;
    /* 1 */ u8 mHead;
    /* 2 */ u8 mFreeCount;
    /* 3 */ u8 mTail;
    /* 4 */ u32 mBytes;
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

BankFunc data_020a54ec[8]  = {GX_EndLoadOBJExtPltt, GX_EndLoadBGExtPltt, GX_EndLoadTex, GX_EndLoadTexPltt,
                              GX_EndLoadSubOBJExtPltt, GX_EndLoadSubBGExtPltt, GX_EndLoadTex, GX_EndLoadTexPltt};
BankFunc data_020a550c[8]  = {GX_BeginLoadOBJExtPltt, GX_BeginLoadBGExtPltt, GX_BeginLoadTex, GX_BeginLoadTexPltt,
                              GX_BeginLoadSubOBJExtPltt, GX_BeginLoadSubBGExtPltt, GX_BeginLoadTex, GX_BeginLoadTexPltt};
UploadFunc data_020a552c[30] = {
    GX_LoadOBJExtPltt, GX_LoadBGExtPltt, GX_LoadTex, GX_LoadTexPltt, GX_LoadOBJ, GXS_LoadBGPltt, GX_LoadBGPltt,
    GX_LoadBG0Scr, GX_LoadBG1Scr, GX_LoadBG2Scr, GX_LoadBG3Scr, GX_LoadBG0Char, GX_LoadBG1Char, GX_LoadBG2Char,
    GX_LoadBG3Char, GX_LoadSubOBJExtPltt, GX_LoadSubBGExtPltt, GX_LoadTex, GX_LoadTexPltt, GXS_LoadOBJ, GXS_LoadOBJPltt,
    GX_LoadOBJPltt, GXS_LoadBG0Scr, GXS_LoadBG1Scr, GXS_LoadBG2Scr, GXS_LoadBG3Scr, GXS_LoadBG0Char, GXS_LoadBG1Char,
    GXS_LoadBG2Char, GXS_LoadBG3Char};
void *data_020a55a4[UPLOAD_FREE_COUNT] = {0};

UploadQueue data_0220e630;
UploadEntry data_0220e638[UPLOAD_QUEUE_SIZE];

// non-matching: 5 words, the original increments mFreeCount before loading the entry's data (the queue and the entries
// probably form one object, which keeps their accesses in order).
void func_020257c8() {
    u32 head = data_0220e630.mHead;
    do {
        UploadEntry *e = &data_0220e638[(u8) head];
        u32 kind       = e->mKind;
        if (kind % UPLOAD_KIND_COUNT < 4) {
            u8 k    = kind;
            u8 bank = k % UPLOAD_KIND_COUNT + (k >= UPLOAD_KIND_COUNT) * 4;
            data_020a550c[bank]();
            data_020a552c[kind](data_0220e638[data_0220e630.mHead].mData, data_0220e638[data_0220e630.mHead].mOffset,
                                data_0220e638[data_0220e630.mHead].mSize);
            data_020a54ec[bank]();
        } else {
            data_020a552c[kind](data_0220e638[(u8) head].mData, e->mOffset, e->mSize);
        }
        if (data_0220e638[data_0220e630.mHead].mFree != 0) {
            u32 n = data_0220e630.mFreeCount++;
            data_020a55a4[n] = data_0220e638[data_0220e630.mHead].mData;
        }
        head                = (data_0220e630.mHead + 1) % UPLOAD_QUEUE_SIZE;
        data_0220e630.mHead = head;
    } while (--data_0220e630.mCount != 0);
}

BOOL func_020258f4(u32 screen, u32 kind, u32 offset, void *data, u32 size, u32 freeData) {
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
            data_020a550c[bank]();
            data_020a552c[slot](data, offset, size);
            data_020a54ec[bank]();
        } else {
            data_020a552c[slot](data, offset, size);
        }
        if (freeData != 0) {
            OS_Free(data);
        }
    }
    return immediate;
}

BOOL func_02025a4c(u32 screen, u32 kind, u32 offset, void *data, u32 size, u32 freeData) {
    u8 n = data_0220e630.mCount;
    if (n == 0) {
        return FALSE;
    }
    u32 slot = kind + screen * UPLOAD_KIND_COUNT;
    u32 end  = offset + size;
    u8 i     = data_0220e630.mHead;
    do {
        if (slot == data_0220e638[i].mKind) {
            BOOL after  = offset <= data_0220e638[i].mOffset;
            BOOL before = end >= data_0220e638[i].mOffset + data_0220e638[i].mSize;
            if (after & before) {
                if (data_0220e638[i].mFree != 0) {
                    OS_Free(data_0220e638[i].mData);
                }
                data_0220e638[i].mOffset = offset;
                data_0220e638[i].mData   = data;
                data_0220e638[i].mSize   = size;
                data_0220e638[i].mFree   = freeData;
                return TRUE;
            }
        }
        i = (i + 1) % UPLOAD_QUEUE_SIZE;
    } while (--n != 0);
    return FALSE;
}

static inline void FlushPendingFrees() {
    while (data_0220e630.mFreeCount != 0) {
        data_0220e630.mFreeCount--;
        OS_Free(data_020a55a4[data_0220e630.mFreeCount]);
    }
}

u32 func_02025b64(u32 screen, u32 kind, u32 offset, const char *path, u32 unk5, u32 unk6, u32 unk7) {
    FlushPendingFrees();
    u32 size;
    void *data = func_02023504(path, 0, unk5, unk6, &size, unk7);
    return func_02025c1c(screen, kind, offset, data, size, 1);
}

// non-matching: 19 words, the original stores mTail after the five entry fields (same ordering as above).
u32 func_02025c1c(u32 screen, u32 kind, u32 offset, void *data, u32 size, u32 freeData) {
    FlushPendingFrees();
    if (func_020258f4(screen, kind, offset, data, size, freeData)) {
        return offset + size;
    }
    if (func_02025a4c(screen, kind, offset, data, size, freeData)) {
        return offset + size;
    }
    data_0220e638[data_0220e630.mTail].mKind   = kind + screen * UPLOAD_KIND_COUNT;
    data_0220e638[data_0220e630.mTail].mOffset = offset;
    data_0220e638[data_0220e630.mTail].mData   = data;
    data_0220e638[data_0220e630.mTail].mSize   = size;
    data_0220e638[data_0220e630.mTail].mFree   = freeData;
    data_0220e630.mTail                        = (data_0220e630.mTail + 1) % UPLOAD_QUEUE_SIZE;
    data_0220e630.mCount++;
    data_0220e630.mBytes += size;
    return offset + size;
}

void func_02025d74() {
    FlushPendingFrees();
    if (data_0220e630.mCount != 0) {
        func_020254f0((FrameCallback) func_020257c8, NULL);
    }
    data_0220e630.mBytes = 0;
}

void func_02025df8() {
    data_0220e630.mHead      = 0;
    data_0220e630.mTail      = 0;
    data_0220e630.mFreeCount = 0;
    data_0220e630.mBytes     = 0;
}
