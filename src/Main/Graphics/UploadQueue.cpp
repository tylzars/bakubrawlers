#include "System/FrameUpdate.hpp"

#include <nitro/os.h>

void *func_02023504(const char *path, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6);
u32 func_02025b64(u32 screen, u32 kind, u32 offset, const char *path, u32 unk5, u32 unk6, u32 unk7);
u32 func_02025c1c(u32 bg, u32 kind, u32 offset, void *data, u32 size, u32 unk6);
int func_02001f14(vu16 *reg);
BOOL func_0200d924();

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
void func_02003938(void *data, u32 offset, u32 size);
void func_02003a94(void *data, u32 offset, u32 size);
void func_0200388c(void *data, u32 offset, u32 size);
void func_02003b44(void *data, u32 offset, u32 size);
void func_02003c04(void *data, u32 offset, u32 size);
void func_02003cc4(void *data, u32 offset, u32 size);
void func_02003d84(void *data, u32 offset, u32 size);
void func_02003e44(void *data, u32 offset, u32 size);
void func_02003f04(void *data, u32 offset, u32 size);
void func_02003fc4(void *data, u32 offset, u32 size);
void func_02004084(void *data, u32 offset, u32 size);
void func_02004468(void *data, u32 offset, u32 size);
void func_020043ac(void *data, u32 offset, u32 size);
void func_02004568(void *data, u32 offset, u32 size);
void func_02004728(void *data, u32 offset, u32 size);
void func_02003aec(void *data, u32 offset, u32 size);
void func_02003990(void *data, u32 offset, u32 size);
void func_020038e0(void *data, u32 offset, u32 size);
void func_02003ba4(void *data, u32 offset, u32 size);
void func_02003c64(void *data, u32 offset, u32 size);
void func_02003d24(void *data, u32 offset, u32 size);
void func_02003de4(void *data, u32 offset, u32 size);
void func_02003ea4(void *data, u32 offset, u32 size);
void func_02003f64(void *data, u32 offset, u32 size);
void func_02004024(void *data, u32 offset, u32 size);
void func_020040e4(void *data, u32 offset, u32 size);
void func_020042e4(void *data, u32 offset, u32 size);
void func_020041e4(void *data, u32 offset, u32 size);
void func_02004350();
void func_02004254();
void func_020046a8();
void func_02004794();
void func_020044cc();
void func_02004410();
void func_0200429c();
void func_02004144();
void func_0200450c();
void func_020046f4();
void func_02004450();
void func_02004394();

BankFunc data_020a54ec[8]  = {func_02004350, func_02004254, func_020046a8, func_02004794,
                              func_020044cc, func_02004410, func_020046a8, func_02004794};
BankFunc data_020a550c[8]  = {func_0200429c, func_02004144, func_0200450c, func_020046f4,
                              func_02004450, func_02004394, func_0200450c, func_020046f4};
UploadFunc data_020a552c[30] = {
    func_020042e4, func_020041e4, func_02004568, func_02004728, func_02003a94, func_02003938, func_0200388c,
    func_02003b44, func_02003c04, func_02003cc4, func_02003d84, func_02003e44, func_02003f04, func_02003fc4,
    func_02004084, func_02004468, func_020043ac, func_02004568, func_02004728, func_02003aec, func_02003990,
    func_020038e0, func_02003ba4, func_02003c64, func_02003d24, func_02003de4, func_02003ea4, func_02003f64,
    func_02004024, func_020040e4};
void *data_020a55a4[UPLOAD_FREE_COUNT] = {0};

UploadQueue data_0220e630;
UploadEntry data_0220e638[UPLOAD_QUEUE_SIZE];

// non-matching: 4 bytes short: the original carries mHead through the loop back edge.
void func_020257c8() {
    do {
        u32 kind = data_0220e638[data_0220e630.mHead].mKind;
        if (kind % UPLOAD_KIND_COUNT < 4) {
            u8 k    = kind;
            u8 bank = k % UPLOAD_KIND_COUNT + (k >= UPLOAD_KIND_COUNT) * 4;
            data_020a550c[bank]();
            data_020a552c[kind](data_0220e638[data_0220e630.mHead].mData, data_0220e638[data_0220e630.mHead].mOffset,
                                data_0220e638[data_0220e630.mHead].mSize);
            data_020a54ec[bank]();
        } else {
            data_020a552c[kind](data_0220e638[data_0220e630.mHead].mData, data_0220e638[data_0220e630.mHead].mOffset,
                                data_0220e638[data_0220e630.mHead].mSize);
        }
        if (data_0220e638[data_0220e630.mHead].mFree != 0) {
            data_020a55a4[data_0220e630.mFreeCount++] = data_0220e638[data_0220e630.mHead].mData;
        }
        data_0220e630.mHead = (data_0220e630.mHead + 1) % UPLOAD_QUEUE_SIZE;
    } while (--data_0220e630.mCount != 0);
}

BOOL func_020258f4(u32 screen, u32 kind, u32 offset, void *data, u32 size, u32 freeData) {
    BOOL immediate = FALSE;
    int brightness;
    if (screen == 0) {
        brightness = func_02001f14((vu16 *) 0x400006c);
    } else if (screen == 1) {
        brightness = func_02001f14((vu16 *) 0x400106c);
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
    void *data = func_02023504(path, 0, unk5, unk6, (u32) &size, unk7);
    return func_02025c1c(screen, kind, offset, data, size, 1);
}

// non-matching: the original stores the tail and count after the five entry fields.
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
