#pragma once

#include "global.h"
#include "types.h"

#include <stdio.h>

// Text copy of the directory last passed to ChangeDir ("/" after InitFileSystem); nothing reads it.
extern char gCurrentDir[];

extern "C" BOOL FS_ChangeDir(const char *dir);

static inline void ChangeDir(const char *dir) {
    sprintf(gCurrentDir, dir);
    FS_ChangeDir(dir);
}

void *LoadFile(const char *path, u32 unk2, u32 unk3, u32 unk4, u32 *outSize, u32 unk6);
void *LoadFileAsync(const char *path, u32 unk2, u32 unk3, u32 unk4, u32 *outSize, u32 unk6); // starts an asynchronous load
void *GetAsyncLoadBuffer(); // data of the finished asynchronous load
u8 IsAsyncLoadActive();    // nonzero while the asynchronous load is busy
void StopAsyncLoad();

// Loads a file and hands it to QueueUpload with freeData = 1. Returns offset + size.
u32 LoadAndQueueUpload(u32 screen, u32 kind, u32 offset, const char *path, u32 unk5, u32 unk6, u32 unk7);
// Uploads a buffer to VRAM at once while the screen's master brightness is -16 or 16 (or func_0200d924 returns 0);
// otherwise queues it, replacing a queued upload of the same screen and kind whose range it covers. Returns offset +
// size.
u32 QueueUpload(u32 bg, u32 kind, u32 offset, void *data, u32 size, u32 unk6);
