// NitroSystem sound library: sound heap (NNSSndHeap). 0x0203824c-0x020385b8
#include <nnsys/private/nns_snd_arc.h>

extern u32 SND_GetCurrentCommandTag(void);
extern void SND_FlushCommand(int);
extern void SND_WaitForCommandProc(u32);

SndHeap *CreateSndHeap(void *where, u32 size) {
    u8 *end = (u8 *)where + size;
    u8 *start = (u8 *)(((u32)where + 3) & ~3);
    void *frm;
    u32 avail;
    if (start > end) return 0;
    avail = end - start;
    if (avail < 0x10) return 0;
    frm = CreateFrmHeapInRange(start + 0x10, avail - 0x10, 0);
    if (frm == 0) return 0;
    if (InitSndHeap((SndHeap *)start, frm) == 0) {
        UnlinkFrmHeap(frm);
        return 0;
    }
    return (SndHeap *)start;
}

void ReleaseSndHeap(SndHeap *h) {
    ResetSndHeap(h);
    UnlinkFrmHeap(h->heap);
}

void ResetSndHeap(SndHeap *h) {   // free every block, running the free callbacks
    void *st;
    int called = 0;
    for (st = NNS_FndGetPrevListObject(&h->states, 0); st != 0; st = NNS_FndGetPrevListObject(&h->states, 0)) {
        SndHeapBlk *b;
        for (b = NNS_FndGetPrevListObject(st, 0); b != 0; b = NNS_FndGetPrevListObject(st, b)) {
            if (b->cb != 0) {
                b->cb((u8 *)b + 0x20, b->size, b->a, b->b);
                called = 1;
            }
        }
        NNS_FndRemoveListObject(&h->states, st);
    }
    ResetFrmHeapByMask(h->heap, 3);
    if (called != 0) FlushSndCommandsAndWait();
    AppendSndHeapState(h);
}

void *AllocSndHeapBlock(SndHeap *h, u32 size, void *cb, u32 a, u32 b) {
    SndHeapBlk *blk = FrmHeapAlloc(h->heap, ((size + 0x1f) & ~0x1f) + 0x20, 0x20);
    void *last;
    if (blk == 0) return 0;
    last = NNS_FndGetPrevListObject(&h->states, 0);
    blk->size = size;
    blk->cb = cb;
    blk->a = a;
    blk->b = b;
    NNS_FndAppendListObject(last, blk);
    return (u8 *)blk + 0x20;
}

s32 RecordSndHeapState(SndHeap *h) {
    if (PushFrmState(h->heap, h->states.count) == 0) return -1;
    if (AppendSndHeapState(h) != 0) return h->states.count - 1;
    RestoreFrmState(h->heap, 0);
    return -1;
}

void RestoreSndHeapState(SndHeap *h, u32 level) {
    void *st;
    int called;
    SndHeapBlk *b;
    b = 0;
    called = 0;
    if (level == 0) {
        ResetSndHeap(h);
        return;
    }
    if ((s32)level < h->states.count) {
        do {
            st = NNS_FndGetPrevListObject(&h->states, 0);
            for (b = NNS_FndGetPrevListObject(st, b); b != 0; b = NNS_FndGetPrevListObject(st, b)) {
                if (b->cb != 0) {
                    b->cb((u8 *)b + 0x20, b->size, b->a, b->b);
                    called = 1;
                }
            }
            NNS_FndRemoveListObject(&h->states, st);
        } while ((s32)level < h->states.count);
    }
    RestoreFrmState(h->heap, level);
    if (called != 0) FlushSndCommandsAndWait();
    PushFrmState(h->heap, h->states.count);
    AppendSndHeapState(h);
}

void InitSndHeapBlockList(void *list) {   // init a callback list
    NNS_FndInitList(list, 0);
}

int InitSndHeap(SndHeap *h, void *frm) {   // init heap header
    NNS_FndInitList(&h->states, 0xc);
    h->heap = frm;
    return AppendSndHeapState(h) != 0 ? 1 : 0;
}

int AppendSndHeapState(SndHeap *h) {   // push a new state block
    void *st = FrmHeapAlloc(h->heap, 0x14, 4);
    if (st == 0) return 0;
    InitSndHeapBlockList(st);
    NNS_FndAppendListObject(&h->states, st);
    return 1;
}

void FlushSndCommandsAndWait(void) {   // flush after callbacks
    u32 irq = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(irq);
}
