// NitroSystem sound library: loading archive files into the sound heap (sequences, banks, wave archives). 0x02038984-0x02038fac
#include <nnsys/private/nns_snd_arc.h>

extern void DC_StoreRange(void *p, u32 size);        // DC_FlushRange
extern u32 OS_DisableInterrupts(void);                      // OS_DisableInterrupts
extern void OS_RestoreInterrupts(u32 state);                // OS_RestoreInterrupts
extern void func_020081ac(void *start, void *end);
extern void func_020081cc(void *start, void *end);
extern void func_020081ec(void *start, void *end);
extern void func_02008d28(void *p);
extern void func_02008dc0(void *p);
extern u32 func_02009014(u32 wave, int idx);
extern void func_02008fe0(u32 wave, int idx, u32 data);
extern u32 func_02008fd8(u32 wave);
extern void func_02008e08(void *iter);
extern int func_02008e28(u32 bank, void *entry, void *iter);
extern void func_01ff96c4(void *dst, void *src, u32 n);
extern void MI_CpuFill8(void *dst, u32 data, u32 size);
typedef struct { u8 pad[0x38]; u32 count; } SndWaveHdr;   // 0x3c bytes of wave archive header
extern SndWaveHdr data_022149b4;

typedef void (*SndFreeCb)(void *, u32, u32, u32);
void func_02038d34(void *p, u32 size, u32 x, u32 id);
void func_02038d5c(void *p, u32 size, u32 x, u32 id);
void func_02038d8c(void *p, u32 size, u32 x, u32 id);
void func_02038dbc(void *p, u32 size, u32 x, u32 id);
void func_02038ddc(void *p, u32 size, u32 wave, u32 idx);
void func_02038cdc(void *p, u32 x, u32 id);
// read file `a` into a new heap block, with `cb` run when the block is freed
void *func_02038984(u32 a, void *cb, u32 c, u32 d, SndHeap *heap) {
    u32 size = func_020380c0(a);
    void *blk;
    if (size == 0) return 0;
    if (heap == 0) return 0;
    blk = func_02038390(heap, size + 0x20, cb, c, d);
    if (blk == 0) return 0;
    if (func_020380e8(a, blk, size, 0) != size) return 0;
    DC_StoreRange(blk, size);
    return blk;
}

u32 func_02038a24(u32 id, SndHeap *heap, int flag) {
    u32 p = func_020381d0(id);
    if (p == 0) {
        u32 x = flag != 0 ? (u32)func_02037d68() : 0;
        p = (u32)func_02038984(id, func_02038d34, x, id, heap);
        if (flag != 0 && p != 0) func_020381f8(id, p);
    }
    return p;
}

u32 func_02038a90(u32 id, SndHeap *heap, int flag) {
    u32 p = func_020381d0(id);
    if (p == 0) {
        u32 x = flag != 0 ? (u32)func_02037d68() : 0;
        p = (u32)func_02038984(id, func_02038d34, x, id, heap);
        if (flag != 0 && p != 0) func_020381f8(id, p);
    }
    return p;
}

u32 func_02038afc(u32 id, SndHeap *heap, int flag) {
    u32 p = func_020381d0(id);
    if (p == 0) {
        u32 x = flag != 0 ? (u32)func_02037d68() : 0;
        p = (u32)func_02038984(id, func_02038d5c, x, id, heap);
        if (flag != 0 && p != 0) func_020381f8(id, p);
    }
    return p;
}

u32 func_02038b68(u32 id, SndHeap *heap, int flag) {
    u32 p = func_020381d0(id);
    if (p == 0) {
        u32 x = flag != 0 ? (u32)func_02037d68() : 0;
        p = (u32)func_02038984(id, func_02038d8c, x, id, heap);
        if (flag != 0 && p != 0) func_020381f8(id, p);
    }
    return p;
}

// wave archive stored as separate waves: load the header, keep the offset table
u32 func_02038bd4(u32 id, SndHeap *heap, int flag) {
    u32 total;
    u32 p = func_020381d0(id);
    u32 tblBytes;
    if (p == 0) {
        u32 x;
        int n;
        if (func_020380e8(id, &data_022149b4, 0x3c, 0) != 0x3c) return 0;
        tblBytes = data_022149b4.count * 4;
        total = tblBytes * 2;
        if (heap == 0) return 0;
        x = flag != 0 ? (u32)func_02037d68() : 0;
        p = (u32)func_02038390(heap, total + 0x5c, func_02038dbc, x, id);
        if (p == 0) return 0;
        n = func_020380e8(id, (void *)p, tblBytes + 0x3c, 0);
        if (n != tblBytes + 0x3c) return 0;
        func_01ff96c4((u8 *)p + 0x3c, (u8 *)p + 0x3c + ((SndWaveHdr *)p)->count * 4, tblBytes);
        MI_CpuFill8((u8 *)p + 0x3c, 0, tblBytes);
        DC_StoreRange((void *)p, total + 0x3c);
        if (flag != 0) func_020381f8(id, p);
    }
    return p;
}

// free callback helper: drop the cache entry if it still points at the freed block
void func_02038cdc(void *p, u32 x, u32 id) {
    SndArc *old;
    u32 irq;
    if (x == 0) return;
    irq = OS_DisableInterrupts();
    old = func_02037d50((SndArc *)x);
    if ((u32)p == func_020381d0(id)) func_020381f8(id, 0);
    func_02037d50(old);
    OS_RestoreInterrupts(irq);
}

void func_02038d34(void *p, u32 size, u32 x, u32 id) {
    func_02038cdc(p, x, id);
    func_020081ac(p, (u8 *)p + size);
}

void func_02038d5c(void *p, u32 size, u32 x, u32 id) {
    func_02038cdc(p, x, id);
    func_020081cc(p, (u8 *)p + size);
    func_02008d28(p);
}

void func_02038d8c(void *p, u32 size, u32 x, u32 id) {
    func_02038cdc(p, x, id);
    func_020081ec(p, (u8 *)p + size);
    func_02008dc0(p);
}

void func_02038dbc(void *p, u32 size, u32 x, u32 id) {
    func_02038cdc(p, x, id);
    func_02008dc0(p);
}

void func_02038ddc(void *p, u32 size, u32 wave, u32 idx) {   // free callback for a single wave
    if ((u32)p == func_02009014(wave, idx)) func_02008fe0(wave, idx, 0);
    func_020081ec(p, (u8 *)p + size);
}

int func_02038e24(u32 wave, u32 idx, u32 file, SndHeap *heap) {   // load wave `idx` of a wave archive
    u32 *w = (u32 *)wave;
    u32 *ent;
    u32 size, start, end, last;
    u8 *blk;
    if (func_02009014(wave, idx) != 0) return 1;
    last = func_02008fd8(wave);
    ent = w + (w[0x38 / 4] + idx);
    start = ent[0x3c / 4];
    end = idx < last - 1 ? ent[0x40 / 4] : w[2];
    size = end - start;
    if (heap == 0) return 0;
    blk = func_02038390(heap, size + 0x20, func_02038ddc, wave, idx);
    if (blk == 0) return 0;
    if (size != func_020380e8(file, blk, size, start)) return 0;
    DC_StoreRange(blk, size);
    func_02008fe0(wave, idx, (u32)blk);
    return 1;
}

typedef struct { u32 a, b; } SndIter;
typedef struct { u8 type; u8 pad; u16 a; u16 b; u16 pad6; u32 pad8; } SndWaveEnt;

int func_02038eec(u32 wave, u32 bank, int idx, u32 file, SndHeap *heap) {
    SndWaveEnt ent;
    SndIter it1;
    SndIter it0;
    func_02008e08(&it0);
    it1 = it0;
    if (bank == 0) return 0;
    while (func_02008e28(bank, &ent, &it1) != 0) {
        if (ent.type == 1 && idx == ent.b) {
            if (func_02038e24(wave, ent.a, file, heap) == 0) return 0;
        }
    }
    return 1;
}
