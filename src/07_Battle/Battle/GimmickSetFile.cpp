#include "Battle/GimmickTypes.hpp"
#include "System/FileSystem.hpp"
#include <nitro/os.h>
#include <nitro/mi.h>
#include <stdio.h>

extern const char *data_ov007_0228abc0; // "/Game/Collision/atari_%02d.bin"

int func_ov007_02271a00(GimmickSetFile *file, const char *path);

void func_ov007_022719c8(void *buf, u8 id) {
    char path[0x40];
    sprintf(path, data_ov007_0228abc0, id);
    func_ov007_02271a00((GimmickSetFile *) buf, path);
}

// One 0x20 byte object record of a SetData file as stored on disk (the file's records start 4 bytes in).
struct GimmickSetSrcEntry {
    /* 00 */ s32 mPosition[3];
    /* 0c */ s32 mScale[3]; // stored doubled
    /* 18 */ u16 mRotX;
    /* 1a */ u16 mRotY;
    /* 1c */ u16 mRotZ;
    /* 1e */ u8 mParam;
    /* 1f */ u8 mType;
    /* 20 */
};

struct GimmickSetSrcFile {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u8 mCount;
    /* 03 */ PAD(0x03, 0x04);
    /* 04 */ GimmickSetSrcEntry mEntries[1];
};

int func_ov007_02271a00(GimmickSetFile *file, const char *path) {
    u32 size;
    GimmickSetSrcFile *src = (GimmickSetSrcFile *) func_02023504(path, 0, 0, 0, &size, 0);
    if (src) {
        file->mCount = src->mCount;
        *(u16 *) file = src->mUnk_00;
        MI_CpuFill8((u8 *) file + 4, 0, 0x800);
        for (u8 i = 0; i < src->mCount; i++) {
            GimmickSetSrcEntry *e = &src->mEntries[i];
            if (e) {
                GimmickSetEntry *d = file->Entry(i);
                d->mParam = e->mParam;
                d->mType  = e->mType;
                d->mPosition.x = e->mPosition[0];
                d->mPosition.y = e->mPosition[1];
                d->mPosition.z = e->mPosition[2];
                *(s32 *) ((u8 *) d + 0x10) = e->mScale[0] / 2;
                *(s32 *) ((u8 *) d + 0x14) = e->mScale[1] / 2;
                *(s32 *) ((u8 *) d + 0x18) = e->mScale[2] / 2;
                d->mRotX = e->mRotX;
                d->mRotY = e->mRotY;
                d->mRotZ = e->mRotZ;
            }
        }
        OS_Free(src);
    } else {
        return 0;
    }
    return 1;
}

// Header fields of the per-object data created by func_ov007_02271ca8 (GimmickSetData, +08 / +0a).
struct GimmickSetDataHead {
    /* 00 */ PAD(0x00, 0x08);
    /* 08 */ u16 mMode;
    /* 0a */ u16 mUnk_0a;
};

void func_ov007_02271b24(GimmickSetDataHead *data, int mode) {
    data->mMode = mode;
    switch (mode) {
        case 1:
            data->mUnk_0a = 0xfffe;
            break;
        case 2:
            data->mUnk_0a = 1;
            break;
        case 4:
            data->mUnk_0a = 1;
            break;
        case 8:
            data->mUnk_0a = 1;
            break;
        case 0x10:
            data->mUnk_0a = 0x24;
            break;
        case 0x20:
            data->mUnk_0a = 0x11;
            break;
    }
}
