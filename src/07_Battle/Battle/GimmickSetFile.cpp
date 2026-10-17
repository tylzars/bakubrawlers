#include "Battle/GimmickTypes.hpp"
#include "System/FileSystem.hpp"
#include <nitro/os.h>
#include <nitro/mi.h>
#include <stdio.h>

extern const char *data_ov007_0228abc0; // "/Game/Collision/atari_%02d.bin"

int LoadGimmickSetFile(GimmickSetFile *file, const char *path);

void LoadCollisionFile(void *buf, u8 id) {
    char path[0x40];
    sprintf(path, data_ov007_0228abc0, id);
    LoadGimmickSetFile((GimmickSetFile *) buf, path);
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

int LoadGimmickSetFile(GimmickSetFile *file, const char *path) {
    u32 size;
    GimmickSetSrcFile *src = (GimmickSetSrcFile *) LoadFile(path, 0, 0, 0, &size, 0);
    if (src) {
        file->mCount = src->mCount;
        *(u16 *) file = src->mUnk_00;
        MI_CpuFill8((u8 *) file + 4, 0, 0x800);
        for (u8 i = 0; i < src->mCount; i++) {
            GimmickSetSrcEntry *e = &src->mEntries[i];
            if (e) {
                file->mEntries[i].mParam = e->mParam;
                file->mEntries[i].mType = e->mType;
                file->mEntries[i].mPosition.x = e->mPosition[0];
                file->mEntries[i].mPosition.y = e->mPosition[1];
                file->mEntries[i].mPosition.z = e->mPosition[2];
                file->mEntries[i].mScale.x = e->mScale[0] / 2;
                file->mEntries[i].mScale.y = e->mScale[1] / 2;
                file->mEntries[i].mScale.z = e->mScale[2] / 2;
                file->mEntries[i].mRotX = e->mRotX;
                file->mEntries[i].mRotY = e->mRotY;
                file->mEntries[i].mRotZ = e->mRotZ;
            }
        }
        OS_Free(src);
    } else {
        return 0;
    }
    return 1;
}

// Header fields of the per-object data created by AddCollisionSet (GimmickSetData, +08 / +0a).
struct GimmickSetDataHead {
    /* 00 */ PAD(0x00, 0x08);
    /* 08 */ u16 mMode;
    /* 0a */ u16 mModeMask;
};

void SetCollisionMode(GimmickSetDataHead *data, int mode) {
    data->mMode = mode;
    switch (mode) {
        case COLLISION_GROUP_BAKUGAN:
            data->mModeMask = 0xfffe; // every group but COLLISION_GROUP_BAKUGAN
            break;
        case COLLISION_GROUP_UNIT:
            data->mModeMask = COLLISION_GROUP_BAKUGAN;
            break;
        case COLLISION_GROUP_FIELD:
            data->mModeMask = COLLISION_GROUP_BAKUGAN;
            break;
        case COLLISION_GROUP_GIMMICK:
            data->mModeMask = COLLISION_GROUP_BAKUGAN;
            break;
        case COLLISION_GROUP_BAKUGAN_SP5:
            data->mModeMask = (COLLISION_GROUP_FIELD | COLLISION_GROUP_GIMMICK_HIT_BY_SP5);
            break;
        case COLLISION_GROUP_GIMMICK_HIT_BY_SP5:
            data->mModeMask = (COLLISION_GROUP_BAKUGAN | COLLISION_GROUP_BAKUGAN_SP5);
            break;
    }
}
