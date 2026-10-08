#include "System/FileSystem.hpp"
#include "System/FrameClock.hpp"
#include "Unknown/UnkFunctions.hpp"

#include <nitro/mi.h>
#include <nitro/os.h>

// State of the file load in progress (instance 0x020d53e4, 0x5c bytes).
struct UnkStruct_020d53e4 {
    /* 00 */ PAD(0x00, 0x24); // 0x00: compression header of the blocking loader (data_020d53e4_h)
    /* 24 */ u32 mAsyncHeader; // header read by the asynchronous loader
    /* 28 */ PAD(0x28, 0x3c);
    /* 3c */ u32 *mOutSize;
    /* 40 */ u32 mSize;
    /* 44 */ u32 mRequestedSize;
    /* 48 */ u32 mChunk;
    /* 4c */ u32 mRemaining;
    /* 50 */ void *mBuffer;
    /* 54 */ void *mTemp;
    /* 58 */ u8 mOwnsBuffer;
    /* 59 */ u8 mMode;
    /* 5a */ u8 mActive;
    /* 5b */
};

struct FSFileLike { // data_020d5440
    /* 00 */ PAD(0x00, 0x0c);
    /* 0c */ u32 mFlags;
    /* 10 */ PAD(0x10, 0x24);
    /* 24 */ u32 mStart;
    /* 28 */ u32 mEnd;
};

extern UnkStruct_020d53e4 data_020d53e4;

static inline BOOL IsBusy(const FSFileLike *file) {
    if (file->mFlags & 1) {
        return TRUE;
    }
    return FALSE;
}

extern u32 data_020d53e4_h; // same address as data_020d53e4: the compression header read by the blocking loader
extern u8 data_020d53f4[];
extern u32 data_020d5408;
extern u8 data_020d540c[];
extern FSFileLike data_020d5440;

extern int func_0200ac40(void *file, s32 offset, u32 origin);
extern int func_0200ac30(void *file, void *dst, u32 len);
extern int func_0200ac20(void *file, void *dst, u32 len);
extern int func_0200aadc(void *file);
extern void func_0200ab18(void *file);
extern void func_0200abd8(void *file);
extern int func_0200aa94(void *file, const char *path);
extern void func_0200a7b4(void *file);
extern void func_02007d4c(void *ctx, void *dst, const void *header);
extern int func_02007d94(void *ctx, void *src, u32 len);
extern void func_02005e9c(void *buf, u32 size);

static inline void NextChunk() {
    if (data_020d53e4.mRemaining > 0x2000) {
        data_020d53e4.mChunk = 0x2000;
    } else {
        data_020d53e4.mChunk = data_020d53e4.mRemaining;
    }
}

static inline void ReadChunk() {
    data_020d53e4.mRemaining -= data_020d53e4.mChunk;
    func_0200ac20(&data_020d5440, data_020d53e4.mTemp, data_020d53e4.mChunk);
}

BOOL func_02023ad4(const char *path);

void func_020234c8() {
    sprintf(data_020d5488, "/");
    func_0200a7b4(&data_020d5440);
    MI_CpuFill8(&data_020d5408, 0, 0x38);
}

// non-matching: 12 words, registers of outSize and chunk swapped.
void *func_02023504(const char *path, u32 unk2, u32 unk3, u32 unk4, u32 *outSize, u32 unk6) {
    void *buffer      = (void *) unk2;
    u32 offset        = unk3;
    u32 size          = unk4;
    u32 mode          = unk6;
    if (mode == 0) {
        mode = (offset | size) != 0 ? 1 : 2;
    }
    if (mode == 1) {
        func_02023ad4(path);
        if (size == 0) {
            size = data_020d5440.mEnd - data_020d5440.mStart - offset;
        }
        if (outSize) {
            *outSize = size;
        }
        void *p = buffer;
        if (buffer == NULL) {
            p = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, size);
        }
        func_0200ac40(&data_020d5440, offset, 0);
        if (func_0200ac30(&data_020d5440, p, size) < 0) {
            if (buffer == NULL) {
                OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, p);
                p = NULL;
            }
            if (outSize) {
                *outSize = 0;
            }
        }
        func_0200aadc(&data_020d5440);
        buffer = p;
    } else {
        func_02023ad4(path);
        if (size == 0) {
            size = data_020d5440.mEnd - data_020d5440.mStart - offset;
        }
        func_0200ac40(&data_020d5440, offset, 0);
        func_0200ac30(&data_020d5440, &data_020d53e4, 4);
        u32 unpacked = data_020d53e4_h >> 8;
        if (buffer == NULL) {
            buffer = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, unpacked);
        }
        void *temp = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, 0x2000);
        func_02007d4c(data_020d53f4, buffer, &data_020d53e4);
        u32 chunk;
        do {
            chunk = size > 0x2000 ? 0x2000 : size;
            func_0200ac30(&data_020d5440, temp, chunk);
            size -= chunk;
        } while (func_02007d94(data_020d53f4, temp, chunk) != 0);
        func_0200aadc(&data_020d5440);
        OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, temp);
        size = unpacked;
        if (outSize) {
            *outSize = size;
        }
    }
    if (buffer) {
        func_02005e9c(buffer, size);
    }
    return buffer;
}

void *func_02023708(const char *path, u32 unk2, u32 unk3, u32 unk4, u32 *outSize, u32 unk6) {
    void *buffer = (void *) unk2;
    u32 offset   = unk3;
    u32 size     = unk4;
    u32 compressed;
    u32 mode     = unk6;
    switch (mode) {
        case 2:
            compressed = 1;
            break;
        case 1:
            compressed = 0;
            break;
        case 0:
            if ((offset | size) != 0) {
                compressed = 0;
            } else {
                compressed = 1;
            }
            break;
    }
    data_020d53e4.mRequestedSize = size;
    if (compressed == 0) {
        func_02023ad4(path);
        if (size == 0) {
            size = data_020d5440.mEnd - data_020d5440.mStart - offset;
        }
        if (outSize) {
            *outSize = size;
        }
        if (buffer == NULL) {
            buffer = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, size);
            data_020d53e4.mOwnsBuffer = 1;
        } else {
            data_020d53e4.mOwnsBuffer = 0;
        }
        func_0200ac40(&data_020d5440, offset, 0);
        func_0200ac20(&data_020d5440, buffer, size);
    } else {
        func_02023ad4(path);
        if (size == 0) {
            size = data_020d5440.mEnd - data_020d5440.mStart - offset;
        }
        func_0200ac40(&data_020d5440, offset, 0);
        func_0200ac20(&data_020d5440, &data_020d5408, 4);
        data_020d53e4.mOwnsBuffer = 0;
    }
    data_020d53e4.mMode      = compressed;
    data_020d53e4.mBuffer    = buffer;
    data_020d53e4.mOutSize   = outSize;
    data_020d53e4.mSize      = size;
    data_020d53e4.mActive    = 1;
    data_020d53e4.mTemp      = NULL;
    return buffer;
}

void *func_02023864() {
    return data_020d53e4.mBuffer;
}

u8 func_02023874() {
    return data_020d53e4.mActive;
}

void func_02023884() {
    if (!data_020d53e4.mActive) {
        return;
    }
    if (IsBusy(&data_020d5440)) {
        return;
    }
    if (data_020d53e4.mMode == 0) {
        func_0200aadc(&data_020d5440);
        data_020d53e4.mActive = 0;
        func_02005e9c(data_020d53e4.mBuffer, data_020d53e4.mSize);
        return;
    }
    if (data_020d53e4.mMode == 1) {
        u32 unpacked = data_020d53e4.mAsyncHeader >> 8;
        if (data_020d53e4.mBuffer == NULL) {
            data_020d53e4.mBuffer     = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, unpacked);
            data_020d53e4.mOwnsBuffer = 1;
        }
        data_020d53e4.mTemp = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, 0x2000);
        func_02007d4c(data_020d540c, data_020d53e4.mBuffer, &data_020d5408);
        data_020d53e4.mRemaining = data_020d53e4.mSize;
        if (data_020d53e4.mRequestedSize == 0) {
            data_020d53e4.mSize = unpacked;
        } else {
            data_020d53e4.mSize = data_020d53e4.mRequestedSize;
        }
        if (data_020d53e4.mOutSize) {
            *data_020d53e4.mOutSize = data_020d53e4.mSize;
        }
        NextChunk();
        ReadChunk();
        data_020d53e4.mMode = 2;
        return;
    }
    if (func_02007d94(data_020d540c, data_020d53e4.mTemp, data_020d53e4.mChunk) != 0) {
        NextChunk();
        ReadChunk();
        return;
    }
    func_0200aadc(&data_020d5440);
    data_020d53e4.mActive = 0;
    OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, data_020d53e4.mTemp);
    func_02005e9c(data_020d53e4.mBuffer, data_020d53e4.mSize);
}

void func_02023a54() {
    if (!data_020d53e4.mActive) {
        return;
    }
    func_0200abd8(&data_020d5440);
    func_0200ab18(&data_020d5440);
    func_0200aadc(&data_020d5440);
    if (data_020d53e4.mOwnsBuffer) {
        OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, data_020d53e4.mBuffer);
    }
    if (data_020d53e4.mTemp) {
        OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, data_020d53e4.mTemp);
    }
    data_020d53e4.mActive = 0;
}

BOOL func_02023ad4(const char *path) {
    char buf[0x80];
    BOOL ok = FALSE;
    const char *prefix = UnkStruct_020c1884::Get().func_0202030c();
    if (prefix) {
        sprintf(buf, "%s_%s", path, prefix);
        ok = func_0200aa94(&data_020d5440, buf);
    }
    if (!ok) {
        ok = func_0200aa94(&data_020d5440, path);
    }
    return ok;
}
