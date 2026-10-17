#pragma once

#include "global.h"
#include "types.h"

#include <stdio.h>

// Path prefix applied to relative file names.
extern char gCurrentDir[];

extern "C" BOOL func_0200acac(const char *dir);

static inline void ChangeDir(const char *dir) {
    sprintf(gCurrentDir, dir);
    func_0200acac(dir);
}

void *LoadFile(const char *path, u32 unk2, u32 unk3, u32 unk4, u32 *outSize, u32 unk6);
void *LoadFileAsync(const char *path, u32 unk2, u32 unk3, u32 unk4, u32 *outSize, u32 unk6); // starts an asynchronous load
void *GetAsyncLoadBuffer(); // data of the finished asynchronous load
u8 IsAsyncLoadActive();    // nonzero while the asynchronous load is busy
void StopAsyncLoad();

// Loads a BG resource file (palette, characters or screen) and uploads it. Returns offset + size.
u32 LoadAndQueueUpload(u32 screen, u32 kind, u32 offset, const char *path, u32 unk5, u32 unk6, u32 unk7);
// Uploads a buffer to BG memory. Returns offset + size.
u32 QueueUpload(u32 bg, u32 kind, u32 offset, void *data, u32 size, u32 unk6);
