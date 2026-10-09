// NitroSystem sound library: sound heap (NNSSndHeap). 0x0203824c-0x020385b8
#include <nnsys/private/nns_snd_arc.h>

extern u32 SND_GetCurrentCommandTag(void);
extern void SND_FlushCommand(int);
extern void SND_WaitForCommandProc(u32);

SndHeap *func_0203824c(void *where, u32 size) {   // NNS_SndHeapCreate
    u8 *end = (u8 *)where + size;
    u8 *start = (u8 *)(((u32)where + 3) & ~3);
    void *frm;
    u32 avail;
    if (start > end) return 0;
    avail = end - start;
    if (avail < 0x10) return 0;
    frm = func_020311a8(start + 0x10, avail - 0x10, 0);
    if (frm == 0) return 0;
    if (func_02038528((SndHeap *)start, frm) == 0) {
        func_020311e0(frm);
        return 0;
    }
    return (SndHeap *)start;
}

void func_020382bc(SndHeap *h) {   // NNS_SndHeapDestroy
    func_020382d4(h);
    func_020311e0(h->heap);
}

void func_020382d4(SndHeap *h) {   // free every block, running the free callbacks
    void *st;
    int called = 0;
    for (st = func_02030a44(&h->states, 0); st != 0; st = func_02030a44(&h->states, 0)) {
        SndHeapBlk *b;
        for (b = func_02030a44(st, 0); b != 0; b = func_02030a44(st, b)) {
            if (b->cb != 0) {
                b->cb((u8 *)b + 0x20, b->size, b->a, b->b);
                called = 1;
            }
        }
        NNS_FndRemoveListObject(&h->states, st);
    }
    func_02031220(h->heap, 3);
    if (called != 0) func_02038598();
    func_0203855c(h);
}

void *func_02038390(SndHeap *h, u32 size, void *cb, u32 a, u32 b) {   // NNS_SndHeapAlloc
    SndHeapBlk *blk = func_020311ec(h->heap, ((size + 0x1f) & ~0x1f) + 0x20, 0x20);
    void *last;
    if (blk == 0) return 0;
    last = func_02030a44(&h->states, 0);
    blk->size = size;
    blk->cb = cb;
    blk->a = a;
    blk->b = b;
    NNS_FndAppendListObject(last, blk);
    return (u8 *)blk + 0x20;
}

s32 func_020383f8(SndHeap *h) {   // NNS_SndHeapSaveState
    if (func_0203124c(h->heap, h->states.count) == 0) return -1;
    if (func_0203855c(h) != 0) return h->states.count - 1;
    func_0203129c(h->heap, 0);
    return -1;
}

void func_02038444(SndHeap *h, u32 level) {   // NNS_SndHeapLoadState
    void *st;
    int called;
    SndHeapBlk *b;
    b = 0;
    called = 0;
    if (level == 0) {
        func_020382d4(h);
        return;
    }
    if ((s32)level < h->states.count) {
        do {
            st = func_02030a44(&h->states, 0);
            for (b = func_02030a44(st, b); b != 0; b = func_02030a44(st, b)) {
                if (b->cb != 0) {
                    b->cb((u8 *)b + 0x20, b->size, b->a, b->b);
                    called = 1;
                }
            }
            NNS_FndRemoveListObject(&h->states, st);
        } while ((s32)level < h->states.count);
    }
    func_0203129c(h->heap, level);
    if (called != 0) func_02038598();
    func_0203124c(h->heap, h->states.count);
    func_0203855c(h);
}

void func_02038518(void *list) {   // init a callback list
    NNS_FndInitList(list, 0);
}

int func_02038528(SndHeap *h, void *frm) {   // init heap header
    NNS_FndInitList(&h->states, 0xc);
    h->heap = frm;
    return func_0203855c(h) != 0 ? 1 : 0;
}

int func_0203855c(SndHeap *h) {   // push a new state block
    void *st = func_020311ec(h->heap, 0x14, 4);
    if (st == 0) return 0;
    func_02038518(st);
    NNS_FndAppendListObject(&h->states, st);
    return 1;
}

void func_02038598(void) {   // flush after callbacks
    u32 irq = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(irq);
}
