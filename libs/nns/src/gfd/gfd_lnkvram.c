// NitroSystem Gfd: link-list texture/palette VRAM manager entry points (continues gfd_vram.c). 0x02036538-0x020366f8
#include <nnsys/private/nns_types.h>

typedef struct { u32 pad0; u32 pad4; u32 vramBase; u8 *mem; u32 memSize; } GfdLnkMan;
extern GfdLnkMan data_022115dc;
extern u8 data_022115e0[];
extern u32 (*data_020a5888)(u32 size, int is4pltt, u32 opt);
extern int (*data_020a588c)(u32 key);

extern u32 func_0203623c(void *mem, u32 n);
extern void func_02036230(void *man);
extern void func_02036288(void *man, void *tbl, u32 a, u32 base);
extern void func_02036424(void *man, void *tbl);
extern int func_020362e4(void *man, void *tbl, u32 *out, u32 size, u32 align);
extern int func_020364a8(void *man, void *tbl, u32 pos, u32 size);

u32 func_0203658c(u32 size, int is4x4, u32 opt);
int func_02036658(u32 key);
void func_0203669c(void);

void func_02036538(u32 base, void *mem, u32 memSize, u32 install) {   // initialise the link-list VRAM manager, optionally hook it as the allocator
    data_022115dc.vramBase = base;
    data_022115dc.mem = mem;
    data_022115dc.memSize = memSize;
    func_0203669c();
    if (install) {
        data_020a5888 = func_0203658c;
        data_020a588c = func_02036658;
    }
}

u32 func_0203658c(u32 size, int is4x4, u32 opt) {   // allocate a texture / palette block, returns packed key
    u32 pos;
    u32 sz;
    int ok;
    if (size == 0) sz = 8; else sz = (size + 7) & ~7;
    if (sz >= 0x7fff8) return 0;
    if (is4x4) {
        ok = func_020362e4(&data_022115dc, data_022115e0, &pos, sz, 8);
        if (pos + sz > 0x10000) {
            func_020364a8(&data_022115dc, data_022115e0, pos, sz);
            return 0;
        }
    } else {
        ok = func_020362e4(&data_022115dc, data_022115e0, &pos, sz, 16);
    }
    if (ok == 0) return 0;
    return ((sz >> 3) << 16) | ((pos << 13) >> 16);
}

int func_02036658(u32 key) {   // free by key
    u32 pos = (key & 0xffff0000) >> 16;
    u32 sz = key << 16;
    return func_020364a8(&data_022115dc, data_022115e0, sz >> 13, pos << 3) == 0;
}

void func_0203669c(void) {   // reset
    data_022115dc.pad4 = func_0203623c(data_022115dc.mem, data_022115dc.memSize >> 4);
    func_02036230(&data_022115dc);
    func_02036288(&data_022115dc, data_022115e0, 0, data_022115dc.vramBase);
    func_02036424(&data_022115dc, data_022115e0);
}
