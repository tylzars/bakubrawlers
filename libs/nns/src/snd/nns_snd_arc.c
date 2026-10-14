// NitroSystem sound library: sound archive (SDAT) access. 0x02037af8-0x020381b0
#include <nnsys/private/nns_snd_types.h>

extern SndArc *data_022149b0;   // current archive

extern int FS_ConvertPathToFileID(void *fileId, const char *path);
extern void func_0200a7b4(void *file);
extern int func_0200aa24(void *file, SndFileId id);
extern int func_0200ac40(void *file, u32 pos, u32 whence);
extern int func_0200ac30(void *file, void *dst, u32 len);
extern void *func_02038390(void *heap, u32 size, void *cb, void *arg, u32 flags);
extern void func_02038228(void);
extern void func_02038234(void);
extern void func_02038240(void);
int func_02037b78(SndArc *arc, void *heap, u32 flag);

void func_02037af8(SndArc *arc, const char *path, void *heap, u32 flag) {   // open an archive
    arc->info = 0;
    arc->fat = 0;
    arc->symb = 0;
    arc->chunk = 0;
    if (FS_ConvertPathToFileID(&arc->fileId, path) == 0) return;
    func_0200a7b4(arc->file);
    if (func_0200aa24(arc->file, arc->fileId) == 0) return;
    arc->opened = 1;
    if (func_02037b78(arc, heap, flag) != 0) {
        data_022149b0 = arc;
    }
}

// load the archive's blocks
int func_02037b78(SndArc *arc, void *heap, u32 flag) {
    int n;
    if (func_0200ac40(arc->file, 0, 0) == 0) return 0;
    if (func_0200ac30(arc->file, arc, 0x30) != 0x30) return 0;
    if (heap != 0) {
        arc->info = func_02038390(heap, arc->infoSize, func_02038228, arc, 0);
        if (arc->info == 0) return 0;
        if (func_0200ac40(arc->file, arc->infoOff, 0) == 0) return 0;
        n = func_0200ac30(arc->file, arc->info, arc->infoSize);
        if (n == arc->infoSize) {} else return 0;
        arc->fat = func_02038390(heap, arc->fatSize, func_02038234, arc, 0);
        if (arc->fat == 0) return 0;
        if (func_0200ac40(arc->file, arc->fatOff, 0) == 0) return 0;
        n = func_0200ac30(arc->file, arc->fat, arc->fatSize);
        if (n == arc->fatSize) {} else return 0;
        if (flag != 0 && arc->symbSize != 0) {
            arc->symb = func_02038390(heap, arc->symbSize, func_02038240, arc, 0);
            if (arc->symb == 0) return 0;
            if (func_0200ac40(arc->file, arc->symbOff, 0) == 0) return 0;
            n = func_0200ac30(arc->file, arc->symb, arc->symbSize);
            if (n == arc->symbSize) {} else return 0;
        }
    }
    return 1;
}

SndArc *func_02037d50(SndArc *arc) {   // set current archive, return the old one
    SndArc *old = data_022149b0;
    data_022149b0 = arc;
    return old;
}

SndArc *func_02037d68(void) {   // current archive
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

void *func_02037d78(int idx) {   // INFO sequence record
    INFO_LOOKUP(0x8)
}

void *func_02037ddc(int idx) {   // INFO sequence archive record
    INFO_LOOKUP(0xc)
}

void *func_02037e40(int idx) {   // INFO bank record
    INFO_LOOKUP(0x10)
}

void *func_02037ea4(int idx) {   // INFO wave archive record
    INFO_LOOKUP(0x14)
}

void *func_02037f08(int idx) {   // INFO stream record
    INFO_LOOKUP(0x24)
}

void *func_02037f6c(int idx) {   // INFO player record
    INFO_LOOKUP(0x18)
}

void *func_02037fd0(int idx) {   // INFO stream player record
    INFO_LOOKUP(0x20)
}

void *func_02038034(int idx) {   // INFO group record
    INFO_LOOKUP(0x1c)
}

u32 func_02038098(u32 idx) {   // FAT entry offset
    SndArcFat *fat = data_022149b0->fat;
    if (idx >= fat->count) return 0;
    return fat->ent[idx].offset;
}

u32 func_020380c0(u32 idx) {   // FAT entry size
    SndArcFat *fat = data_022149b0->fat;
    if (idx >= fat->count) return 0;
    return fat->ent[idx].size;
}

int func_020380e8(u32 idx, void *dst, int len, u32 off) {
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
        if (func_0200ac40(arc->file, ent->offset + off, 0) == 0) return -1;
        r = func_0200ac30(arc->file, dst, n);
        if (r < 0) return r;
        total += r;
        off += r;
        dst = (u8 *)dst + r;
    }
    return total;
}
