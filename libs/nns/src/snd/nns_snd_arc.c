// NitroSystem sound library: sound archive (SDAT) access. 0x02037af8-0x020381b0
#include <nnsys/private/nns_snd_types.h>

extern SndArc *data_022149b0;   // current archive

extern int FS_ConvertPathToFileID(void *fileId, const char *path);
extern void FS_InitFile(void *file);
extern int FS_OpenFileById(void *file, SndFileId id);
extern int FS_SeekFile(void *file, u32 pos, u32 whence);
extern int FS_ReadFile(void *file, void *dst, u32 len);
extern void *AllocSndHeapBlock(void *heap, u32 size, void *cb, void *arg, u32 flags);
extern void ArcInfoFreeCallback(void);
extern void ArcFatFreeCallback(void);
extern void ArcSymbFreeCallback(void);
int LoadSndArcBlocks(SndArc *arc, void *heap, u32 flag);

void OpenSndArc(SndArc *arc, const char *path, void *heap, u32 flag) {   // open an archive
    arc->info = 0;
    arc->fat = 0;
    arc->symb = 0;
    arc->chunk = 0;
    if (FS_ConvertPathToFileID(&arc->fileId, path) == 0) return;
    FS_InitFile(arc->file);
    if (FS_OpenFileById(arc->file, arc->fileId) == 0) return;
    arc->opened = 1;
    if (LoadSndArcBlocks(arc, heap, flag) != 0) {
        data_022149b0 = arc;
    }
}

// load the archive's blocks
int LoadSndArcBlocks(SndArc *arc, void *heap, u32 flag) {
    int n;
    if (FS_SeekFile(arc->file, 0, 0) == 0) return 0;
    if (FS_ReadFile(arc->file, arc, 0x30) != 0x30) return 0;
    if (heap != 0) {
        arc->info = AllocSndHeapBlock(heap, arc->infoSize, ArcInfoFreeCallback, arc, 0);
        if (arc->info == 0) return 0;
        if (FS_SeekFile(arc->file, arc->infoOff, 0) == 0) return 0;
        n = FS_ReadFile(arc->file, arc->info, arc->infoSize);
        if (n == arc->infoSize) {} else return 0;
        arc->fat = AllocSndHeapBlock(heap, arc->fatSize, ArcFatFreeCallback, arc, 0);
        if (arc->fat == 0) return 0;
        if (FS_SeekFile(arc->file, arc->fatOff, 0) == 0) return 0;
        n = FS_ReadFile(arc->file, arc->fat, arc->fatSize);
        if (n == arc->fatSize) {} else return 0;
        if (flag != 0 && arc->symbSize != 0) {
            arc->symb = AllocSndHeapBlock(heap, arc->symbSize, ArcSymbFreeCallback, arc, 0);
            if (arc->symb == 0) return 0;
            if (FS_SeekFile(arc->file, arc->symbOff, 0) == 0) return 0;
            n = FS_ReadFile(arc->file, arc->symb, arc->symbSize);
            if (n == arc->symbSize) {} else return 0;
        }
    }
    return 1;
}

SndArc *SetCurrentSndArc(SndArc *arc) {   // set current archive, return the old one
    SndArc *old = data_022149b0;
    data_022149b0 = arc;
    return old;
}

SndArc *GetCurrentSndArc(void) {   // current archive
    return data_022149b0;
}

static inline void *InfoPtr(void *base, u32 off) { return off == 0 ? 0 : (u8 *)base + off; }

// Look up record `idx` of an INFO block section at section-offset `sec`; 0 if missing.
#define INFO_LOOKUP(sec) \
    SndArc *arc = data_022149b0; \
    SndArcTable *t = InfoPtr(arc->info, *(u32 *)(arc->info + (sec))); \
    if (t == 0) return 0; \
    if (idx < 0) return 0; \
    if (idx >= t->count) return 0; \
    return InfoPtr(arc->info, t->off[idx]);

void *GetSeqInfo(int idx) {   // INFO sequence record
    INFO_LOOKUP(0x8)
}

void *func_02037ddc(int idx) {   // INFO sequence archive record
    INFO_LOOKUP(0xc)
}

void *GetBankInfo(int idx) {   // INFO bank record
    INFO_LOOKUP(0x10)
}

void *GetWaveArcInfo(int idx) {   // INFO wave archive record
    INFO_LOOKUP(0x14)
}

void *GetWavStrmFileInfo(int idx) {   // INFO stream record
    INFO_LOOKUP(0x24)
}

void *GetPlayerInfo(int idx) {   // INFO player record
    INFO_LOOKUP(0x18)
}

void *GetWavStrmChannelInfo(int idx) {   // INFO stream player record
    INFO_LOOKUP(0x20)
}

void *GetGroupInfo(int idx) {   // INFO group record
    INFO_LOOKUP(0x1c)
}

u32 GetFatEntryOffset(u32 idx) {   // FAT entry offset
    SndArcFat *fat = data_022149b0->fat;
    if (idx >= fat->count) return 0;
    return fat->ent[idx].offset;
}

u32 GetFatEntrySize(u32 idx) {   // FAT entry size
    SndArcFat *fat = data_022149b0->fat;
    if (idx >= fat->count) return 0;
    return fat->ent[idx].size;
}

int ReadFatEntryData(u32 idx, void *dst, int len, u32 off) {
    int total;
    SndArc *arc;
    SndArcFatEnt *ent;
    int chunk;
    int n;
    SndArcFat *fat;
    arc = data_022149b0;
    fat = arc->fat;
    if (idx >= fat->count) return -1;
    ent = &fat->ent[idx];
    chunk = arc->chunk;
    if (chunk == 0) chunk = len;
    total = 0;
    while (total < len) {
        int r;
        n = len - total;
        if (n > chunk) n = chunk;
        if ((u32)n > ent->size - off) n = ent->size - off;
        if (n == 0) break;
        if (FS_SeekFile(arc->file, ent->offset + off, 0) == 0) return -1;
        r = FS_ReadFile(arc->file, dst, n);
        if (r < 0) return r;
        total += r;
        off += r;
        dst = (u8 *)dst + r;
    }
    return total;
}
