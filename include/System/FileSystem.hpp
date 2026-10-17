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

// Upload kinds (the kind argument of QueueUpload / LoadAndQueueUpload): gUploadFuncs[kind + screen * UPLOAD_KIND_COUNT]
// in UploadQueue.cpp is the function the upload goes through; each kind is named after it (screen 0 / screen 1), and
// kinds 0-3 are also bracketed by the gBeginLoadFuncs / gEndLoadFuncs entry. Kinds 5 and 6 also agree with their uses:
// every use queues 5 with OBJ kinds (beside kind 4 in SpriteScreen, the counterpart of kind 0 in
// TextManager::SetColor) and 6 with BG kinds (with kinds 11 and 7 in LogoScene, with kind 9 for the same file stem in
// BattleShoot, the counterpart of kind 1 in UploadBgPalette).
#define UPLOAD_KIND_OBJ_EXT_PLTT 0  // GX_LoadOBJExtPltt / GX_LoadSubOBJExtPltt
#define UPLOAD_KIND_BG_EXT_PLTT  1  // GX_LoadBGExtPltt / GX_LoadSubBGExtPltt
#define UPLOAD_KIND_TEX          2  // GX_LoadTex on both
#define UPLOAD_KIND_TEX_PLTT     3  // GX_LoadTexPltt on both
#define UPLOAD_KIND_OBJ          4  // GX_LoadOBJ / GXS_LoadOBJ
#define UPLOAD_KIND_OBJ_PLTT     5  // GX_LoadOBJPltt / GXS_LoadOBJPltt
#define UPLOAD_KIND_BG_PLTT      6  // GX_LoadBGPltt / GXS_LoadBGPltt
#define UPLOAD_KIND_BG0_SCR      7  // GX_LoadBG0Scr / GXS_LoadBG0Scr
#define UPLOAD_KIND_BG1_SCR      8
#define UPLOAD_KIND_BG2_SCR      9
#define UPLOAD_KIND_BG3_SCR      10
#define UPLOAD_KIND_BG0_CHAR     11 // GX_LoadBG0Char / GXS_LoadBG0Char
#define UPLOAD_KIND_BG1_CHAR     12
#define UPLOAD_KIND_BG2_CHAR     13
#define UPLOAD_KIND_BG3_CHAR     14

// Loads a file and hands it to QueueUpload with freeData = 1. Returns offset + size.
u32 LoadAndQueueUpload(u32 screen, u32 kind, u32 offset, const char *path, u32 unk5, u32 unk6, u32 unk7);
// Uploads a buffer to VRAM at once while the screen's master brightness is -16 or 16 (or func_0200d924 returns 0);
// otherwise queues it, replacing a queued upload of the same screen and kind whose range it covers. Returns offset +
// size.
u32 QueueUpload(u32 bg, u32 kind, u32 offset, void *data, u32 size, u32 unk6);
