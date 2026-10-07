#pragma once

#include "global.h"
#include "types.h"

#include <stdio.h>

// Path prefix applied to relative file names.
extern char data_020d5488[];

void func_0200acac(const char *dir);

static inline void ChangeDir(const char *dir) {
    sprintf(data_020d5488, dir);
    func_0200acac(dir);
}

void *func_02023504(const char *path, u32 unk2, u32 unk3, u32 unk4, u32 unk5, u32 unk6);

// Loads a BG resource file (palette, characters or screen) and uploads it.
void func_02025b64(u32 screen, u32 kind, u32 offset, const char *path, u32 unk5, u32 unk6, u32 unk7);
void func_02025c1c(u32 bg, u32 kind, u32 offset, void *data, u32 size, u32 unk6);
