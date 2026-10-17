// NitroSystem sound library: sound archive file cache and archive loader callbacks. 0x020381b0-0x02038240
#include <nnsys/private/nns_snd_arc.h>

void GetCurrentArcFileId(SndFileId *out) {   // copy the current archive's FSFileID
    SndArc *a = data_022149b0;
    SndFileId id = a->fileId;
    *out = id;
}

u32 GetFatEntryCache(u32 idx) {   // cached copy of a FAT entry, or 0
    SndArcFat *fat = data_022149b0->fat;
    if (idx >= fat->count) return 0;
    return fat->ent[idx].cache;
}

void SetFatEntryCache(u32 idx, u32 v) {   // set the cached copy of a FAT entry
    data_022149b0->fat->ent[idx].cache = v;
}

void SetArcReadChunk(u32 chunk) {   // set the archive read chunk size
    data_022149b0->chunk = chunk;
}

// heap-block free callbacks: the third argument is the owning SndArc
void ArcInfoFreeCallback(void *p, u32 size, SndArc *arc) { arc->info = 0; }
void ArcFatFreeCallback(void *p, u32 size, SndArc *arc) { arc->fat = 0; }
void ArcSymbFreeCallback(void *p, u32 size, SndArc *arc) { arc->symb = 0; }
