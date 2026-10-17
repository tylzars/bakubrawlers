#include "System/FileSystem.hpp"
#include "System/GameSystem.hpp"
#include "Unknown/UnkFunctions.hpp"

#include <nitro/mi.h>
#include <nitro/os.h>

// State of the file load in progress (instance 0x020d53e4, 0x5c bytes).
struct FileLoadState {
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

struct FSFileLike { // gFsFile
    /* 00 */ PAD(0x00, 0x0c);
    /* 0c */ u32 mFlags;
    /* 10 */ PAD(0x10, 0x24);
    /* 24 */ u32 mStart;
    /* 28 */ u32 mEnd;
};

extern FileLoadState gFileLoad;

static inline BOOL IsBusy(const FSFileLike *file) {
    if (file->mFlags & 1) {
        return TRUE;
    }
    return FALSE;
}

extern u32 data_020d53e4_h; // same address as gFileLoad: the compression header read by the blocking loader
extern u8 gLoadFileUncompContext[];
extern u32 gAsyncLoadHeader; // same address as gFileLoad.mAsyncHeader (0x020d53e4 + 0x24)
extern u8 gAsyncUncompContext[];
extern FSFileLike gFsFile;

extern "C" int FS_SeekFile(void *file, s32 offset, u32 origin);
extern "C" int FS_ReadFile(void *file, void *dst, u32 len);
extern "C" int FS_ReadFileAsync(void *file, void *dst, u32 len);
extern "C" int FS_CloseFile(void *file);
extern "C" void FS_WaitFile(void *file);
extern "C" void FS_CancelFile(void *file);
extern "C" int FS_OpenFile(void *file, const char *path);
extern "C" void FS_InitFile(void *file);
extern "C" void MI_InitUncompContextLZ(void *ctx, void *dst, const void *header);
extern "C" int MI_UncompLZChunk(void *ctx, void *src, u32 len);
extern "C" void DC_FlushRange(void *buf, u32 size);

static inline void NextChunk() {
    if (gFileLoad.mRemaining > 0x2000) {
        gFileLoad.mChunk = 0x2000;
    } else {
        gFileLoad.mChunk = gFileLoad.mRemaining;
    }
}

static inline void ReadChunk() {
    gFileLoad.mRemaining -= gFileLoad.mChunk;
    FS_ReadFileAsync(&gFsFile, gFileLoad.mTemp, gFileLoad.mChunk);
}

BOOL OpenFileWithLanguageSuffix(const char *path);

void InitFileSystem() {
    sprintf(gCurrentDir, "/");
    FS_InitFile(&gFsFile);
    MI_CpuFill8(&gAsyncLoadHeader, 0, 0x38);
}

void *LoadFile(const char *path, u32 unk2, u32 unk3, u32 unk4, u32 *outSize, u32 unk6) {
    void *buffer      = (void *) unk2;
    u32 offset        = unk3;
    u32 size          = unk4;
    u32 mode          = unk6;
    void *p;
    if (mode == 0) {
        mode = (offset | size) != 0 ? 1 : 2;
    }
    if (mode == 1) {
        OpenFileWithLanguageSuffix(path);
        if (size == 0) {
            size = gFsFile.mEnd - gFsFile.mStart - offset;
        }
        if (outSize) {
            *outSize = size;
        }
        p = buffer;
        if (buffer == NULL) {
            p = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, size);
        }
        FS_SeekFile(&gFsFile, offset, 0);
        if (FS_ReadFile(&gFsFile, p, size) < 0) {
            if (buffer == NULL) {
                OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, p);
                p = NULL;
            }
            if (outSize) {
                *outSize = 0;
            }
        }
        FS_CloseFile(&gFsFile);
        buffer = p;
    } else {
        OpenFileWithLanguageSuffix(path);
        if (size == 0) {
            size = gFsFile.mEnd - gFsFile.mStart - offset;
        }
        FS_SeekFile(&gFsFile, offset, 0);
        FS_ReadFile(&gFsFile, &gFileLoad, 4);
        u32 unpacked = data_020d53e4_h >> 8;
        if (buffer == NULL) {
            buffer = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, unpacked);
        }
        p = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, 0x2000);
        MI_InitUncompContextLZ(gLoadFileUncompContext, buffer, &gFileLoad);
        u32 chunk;
        do {
            chunk = size > 0x2000 ? 0x2000 : size;
            FS_ReadFile(&gFsFile, p, chunk);
            size -= chunk;
        } while (MI_UncompLZChunk(gLoadFileUncompContext, p, chunk) != 0);
        FS_CloseFile(&gFsFile);
        OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, p);
        size = unpacked;
        if (outSize) {
            *outSize = size;
        }
    }
    if (buffer) {
        DC_FlushRange(buffer, size);
    }
    return buffer;
}

void *LoadFileAsync(const char *path, u32 unk2, u32 unk3, u32 unk4, u32 *outSize, u32 unk6) {
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
    gFileLoad.mRequestedSize = size;
    if (compressed == 0) {
        OpenFileWithLanguageSuffix(path);
        if (size == 0) {
            size = gFsFile.mEnd - gFsFile.mStart - offset;
        }
        if (outSize) {
            *outSize = size;
        }
        if (buffer == NULL) {
            buffer = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, size);
            gFileLoad.mOwnsBuffer = 1;
        } else {
            gFileLoad.mOwnsBuffer = 0;
        }
        FS_SeekFile(&gFsFile, offset, 0);
        FS_ReadFileAsync(&gFsFile, buffer, size);
    } else {
        OpenFileWithLanguageSuffix(path);
        if (size == 0) {
            size = gFsFile.mEnd - gFsFile.mStart - offset;
        }
        FS_SeekFile(&gFsFile, offset, 0);
        FS_ReadFileAsync(&gFsFile, &gAsyncLoadHeader, 4);
        gFileLoad.mOwnsBuffer = 0;
    }
    gFileLoad.mMode      = compressed;
    gFileLoad.mBuffer    = buffer;
    gFileLoad.mOutSize   = outSize;
    gFileLoad.mSize      = size;
    gFileLoad.mActive    = 1;
    gFileLoad.mTemp      = NULL;
    return buffer;
}

void *GetAsyncLoadBuffer() {
    return gFileLoad.mBuffer;
}

u8 IsAsyncLoadActive() {
    return gFileLoad.mActive;
}

void UpdateAsyncLoad() {
    if (!gFileLoad.mActive) {
        return;
    }
    if (IsBusy(&gFsFile)) {
        return;
    }
    if (gFileLoad.mMode == 0) {
        FS_CloseFile(&gFsFile);
        gFileLoad.mActive = 0;
        DC_FlushRange(gFileLoad.mBuffer, gFileLoad.mSize);
        return;
    }
    if (gFileLoad.mMode == 1) {
        u32 unpacked = gFileLoad.mAsyncHeader >> 8;
        if (gFileLoad.mBuffer == NULL) {
            gFileLoad.mBuffer     = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, unpacked);
            gFileLoad.mOwnsBuffer = 1;
        }
        gFileLoad.mTemp = OS_AllocFromHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, 0x2000);
        MI_InitUncompContextLZ(gAsyncUncompContext, gFileLoad.mBuffer, &gAsyncLoadHeader);
        gFileLoad.mRemaining = gFileLoad.mSize;
        if (gFileLoad.mRequestedSize == 0) {
            gFileLoad.mSize = unpacked;
        } else {
            gFileLoad.mSize = gFileLoad.mRequestedSize;
        }
        if (gFileLoad.mOutSize) {
            *gFileLoad.mOutSize = gFileLoad.mSize;
        }
        NextChunk();
        ReadChunk();
        gFileLoad.mMode = 2;
        return;
    }
    if (MI_UncompLZChunk(gAsyncUncompContext, gFileLoad.mTemp, gFileLoad.mChunk) != 0) {
        NextChunk();
        ReadChunk();
        return;
    }
    FS_CloseFile(&gFsFile);
    gFileLoad.mActive = 0;
    OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, gFileLoad.mTemp);
    DC_FlushRange(gFileLoad.mBuffer, gFileLoad.mSize);
}

void StopAsyncLoad() {
    if (!gFileLoad.mActive) {
        return;
    }
    FS_CancelFile(&gFsFile);
    FS_WaitFile(&gFsFile);
    FS_CloseFile(&gFsFile);
    if (gFileLoad.mOwnsBuffer) {
        OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, gFileLoad.mBuffer);
    }
    if (gFileLoad.mTemp) {
        OS_FreeToHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE, gFileLoad.mTemp);
    }
    gFileLoad.mActive = 0;
}

// Opens gFsFile on "<path>_<GetLanguageSuffix()>"; when the suffix is NULL or that open fails, on <path>.
// TRUE when one opened (the callers ignore it).
BOOL OpenFileWithLanguageSuffix(const char *path) {
    char buf[0x80];
    BOOL ok = FALSE;
    const char *suffix = GameSystem::Get().GetLanguageSuffix();
    if (suffix) {
        sprintf(buf, "%s_%s", path, suffix);
        ok = FS_OpenFile(&gFsFile, buf);
    }
    if (!ok) {
        ok = FS_OpenFile(&gFsFile, path);
    }
    return ok;
}
