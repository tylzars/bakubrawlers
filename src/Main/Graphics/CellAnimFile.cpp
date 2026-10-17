#include "Graphics/CellAnimation.hpp"
#include "System/FileSystem.hpp"

#include <nitro/fs.h>
#include <nitro/mi.h>
#include <nitro/os.h>

void *operator new[](size_t size);
void operator delete[](void *ptr);

CellAnimData *LoadCellAnimFile(const char *path);
CellAnimData *RelocateCellAnimData(void *data);
CellAnimData *LoadCellAnimData(void *data);
void FreeCellAnimData(void *data);
void ScaleCellAnimChars(CellAnimFile *file, CellAnimData *data, u16 den, s32 num);

// Loaded files are shared: one entry per file id, with a reference count.
struct CellAnimCacheEntry {
    /* 00 */ u64 mKey;
    /* 08 */ CellAnimData *mData;
    /* 0c */ u8 mRefs;
    /* 0d */ PAD(0x0d, 0x10);
    /* 10 */
};

static CellAnimCacheEntry *sCache;

CellAnimFile::CellAnimFile(const char *path, u16 scaleDen, u16 scaleNum) {
    mData = NULL;
    if (path == NULL) {
        return;
    }
    if (sCache == NULL) {
        sCache = (CellAnimCacheEntry *) new u8[0x400];
        MI_CpuFill8(sCache, 0, 0x400);
    }
    FSFileID id;
    FS_ConvertPathToFileID(&id, path);
    BOOL found                = FALSE;
    u8 i;
    u64 key                   = id.file_id;
    for (i = 0; i < 64; i++) {
        if (sCache[i].mRefs != 0 && key == sCache[i].mKey) {
            mData = sCache[i].mData;
            sCache[i].mRefs++;
            found = TRUE;
            break;
        }
    }
    if (!found) {
        for (i = 0; i < 64; i++) {
            CellAnimCacheEntry *e = &sCache[i];
            if (e->mRefs == 0) {
                e->mKey = id.file_id;
                mData   = LoadCellAnimFile(path);
                if (scaleDen != scaleNum) {
                    ScaleCellAnimChars(this, mData, scaleDen, scaleNum);
                }
                sCache[i].mData = mData;
                sCache[i].mRefs = 1;
                break;
            }
        }
    }
}

CellAnimFile::~CellAnimFile() {
    CellAnimCacheEntry *cache = sCache;
    if (cache != NULL) {
        u8 i;
        for (i = 0; i < 64; i++) {
            CellAnimCacheEntry *e = &cache[i];
            if (e->mRefs != 0 && e->mData == mData) {
                cache[i].mRefs--;
                if (sCache[i].mRefs == 0) {
                    FreeCellAnimData(sCache[i].mData);
                }
                break;
            }
        }
        cache = sCache;
        for (i = 0; i < 64; i++) {
            if (cache[i].mRefs != 0) {
                return;
            }
        }
        delete[] sCache;
        sCache = NULL;
    }
}

// Scales by num / den the character numbers of the OAM entries up to the last one a cel uses (at least one).
// `den` is 16-bit all the way down (CellAnimation -> CellAnimFile -> here): a narrow parameter's stack copy comes
// after the loads, as in the original; as s32 the store came first.
void ScaleCellAnimChars(CellAnimFile *file, CellAnimData *data, u16 den, s32 num) {
    u16 count            = data->mNumFrames;
    CellAnimFrame *frame = data->mFrames;
    CellAnimCel *cel     = data->mCels;
    CellAnimOam *oam     = data->mOams;
    u32 total            = 1;
    u32 i                = 0;
    do {
        u32 j = 0;
        do {
            u32 end = cel->mOamCount + ((u32) cel->mOam - (u32) oam) / 12;
            if (total < end) {
                total = end;
            }
            j++;
            cel++;
        } while (j < frame->mFrameCount);
        i++;
        frame++;
    } while (i < count);
    i = 0;
    do {
        u16 chr = oam->mChar & 0x3ff;
        u16 v   = chr * num / den;
        oam->mChar &= ~0x3ff;
        oam->mChar |= v;
        i++;
        oam++;
    } while (i < total);
}

CellAnimFileData::CellAnimFileData(void *data) : CellAnimFile(NULL, 1, 1) {
    mData = LoadCellAnimData(data);
}

CellAnimData *LoadCellAnimFile(const char *path) {
    return RelocateCellAnimData(LoadFile(path, 0, 0, 0, 0, 0));
}

CellAnimData *LoadCellAnimData(void *data) {
    return RelocateCellAnimData(data);
}

// Turns the offsets of a loaded file into pointers.
CellAnimData *RelocateCellAnimData(void *data) {
    CellAnimData *file = (CellAnimData *) data;
    u32 oams           = (u32) file->mOams;
    u32 cels           = (u32) file->mCels;
    u32 frames         = (u32) file->mFrames;
    file->mOams        = (CellAnimOam *) ((u32) file + oams);
    file->mCels        = (CellAnimCel *) ((u32) file + cels);
    CellAnimFrame *frame = (CellAnimFrame *) ((u32) file + frames);
    file->mFrames        = frame;
    u32 i                = 0;
    do {
        u32 j            = 0;
        CellAnimCel *cel = (CellAnimCel *) ((u32) file->mCels + (u32) frame->mCels * sizeof(CellAnimCel));
        frame->mCels     = cel;
        do {
            cel->mOam = (void *) ((u32) file->mOams + (u32) cel->mOam * sizeof(CellAnimOam));
            j++;
            cel++;
        } while (j < frame->mFrameCount);
        i++;
        frame++;
    } while (i < file->mNumFrames);
    return file;
}

void FreeCellAnimData(void *data) {
    OS_Free(data);
}
