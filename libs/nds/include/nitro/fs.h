#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FSArchive FSArchive;

typedef struct FSFileID {
    FSArchive *arc;
    u32 file_id;
} FSFileID;

BOOL FS_ConvertPathToFileID(FSFileID *p_fileid, const char *path); // 0x0200a99c

#ifdef __cplusplus
}
#endif
