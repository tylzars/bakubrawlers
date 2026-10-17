// NitroSystem Gfd: link-list texture/palette VRAM manager entry points (continues gfd_vram.c). 0x02036538-0x020366f8
#include <nnsys/private/nns_types.h>

typedef struct { u32 pad0; u32 pad4; u32 vramSize; u8 *mem; u32 memSize; } GfdLnkMan;
extern GfdLnkMan data_022115dc;
extern u8 data_022115e0[];
extern u8 data_022115dc_h[];   // the manager again under a second name: the binary's literal pool holds the address twice
extern u32 (*data_020a5888)(u32 size, int is4pltt, u32 opt);
extern int (*data_020a588c)(u32 key);

extern u32 ChainNodes(void *mem, u32 n);
extern void ClearFreeRegions(void *man);
extern void AddFreeRegion(void *man, void *tbl, u32 start, u32 size);
extern void MergeAllFreeRegions(void *man, void *tbl);
extern int AllocFromFreeRegions(void *man, void *tbl, u32 *out, u32 size, u32 align);
extern int MergeAndAddFreeRegion(void *man, void *tbl, u32 pos, u32 size);

u32 AllocPlttLnkKey(u32 size, int is4x4, u32 opt);
int FreePlttLnkKey(u32 key);
void ResetPlttLnkMan(void);

void SetupPlttLnkMan(u32 vramSize, void *mem, u32 memSize, u32 install) {   // initialise the link-list VRAM manager, optionally hook it as the allocator
    data_022115dc.vramSize = vramSize;
    data_022115dc.mem = mem;
    data_022115dc.memSize = memSize;
    ResetPlttLnkMan();
    if (install) {
        data_020a5888 = AllocPlttLnkKey;
        data_020a588c = FreePlttLnkKey;
    }
}

u32 AllocPlttLnkKey(u32 size, int is4x4, u32 opt) {   // allocate a texture / palette block, returns packed key
    u32 pos;
    u32 sz;
    int ok;
    if (size == 0) sz = 8; else sz = (size + 7) & ~7;
    if (sz >= 0x7fff8) return 0;
    if (is4x4) {
        ok = AllocFromFreeRegions(&data_022115dc, data_022115e0, &pos, sz, 8);
        if (pos + sz > 0x10000) {
            MergeAndAddFreeRegion(&data_022115dc, data_022115e0, pos, sz);
            return 0;
        }
    } else {
        ok = AllocFromFreeRegions(&data_022115dc, data_022115e0, &pos, sz, 16);
    }
    if (ok == 0) return 0;
    return ((sz >> 3) << 16) | ((pos << 13) >> 16);
}

int FreePlttLnkKey(u32 key) {   // free by key
    u32 sz = (key & 0xffff0000) >> 16;
    u32 pos = key << 16;
    return MergeAndAddFreeRegion(&data_022115dc, data_022115e0, pos >> 13, sz << 3) == 0;
}

void ResetPlttLnkMan(void) {   // reset
    data_022115dc.pad4 = ChainNodes(data_022115dc.mem, data_022115dc.memSize >> 4);
    ClearFreeRegions(data_022115dc_h);
    AddFreeRegion(data_022115dc_h, data_022115e0, 0, data_022115dc.vramSize);
    MergeAllFreeRegions(data_022115dc_h, data_022115e0);
}
