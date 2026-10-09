// NitroSystem sound library: sound archive file cache and archive loader callbacks. 0x020381b0-0x02038240
#include <nnsys/private/nns_snd_arc.h>

void func_020381b0(SndFileId *out) {   // copy the current archive's FSFileID
    SndArc *a = data_022149b0;
    SndFileId id = a->fileId;
    *out = id;
}

u32 func_020381d0(u32 idx) {   // cached copy of a FAT entry, or 0
    SndArcFat *fat = data_022149b0->fat;
    if (idx >= fat->count) return 0;
    return fat->ent[idx].cache;
}

void func_020381f8(u32 idx, u32 v) {   // set the cached copy of a FAT entry
    data_022149b0->fat->ent[idx].cache = v;
}

void func_02038214(u32 chunk) {   // set the archive read chunk size
    data_022149b0->chunk = chunk;
}

// heap-block free callbacks: the third argument is the owning SndArc
void func_02038228(void *p, u32 size, SndArc *arc) { arc->info = 0; }
void func_02038234(void *p, u32 size, SndArc *arc) { arc->fat = 0; }
void func_02038240(void *p, u32 size, SndArc *arc) { arc->symb = 0; }
