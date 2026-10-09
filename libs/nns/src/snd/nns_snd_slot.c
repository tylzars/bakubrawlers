// NitroSystem sound library: sound slot allocation and player heap blocks. 0x02037090-0x02037264
#include <nnsys/private/nns_snd_types.h>

extern SndPlayer data_02213e64[32];
extern NNSFndList data_02213a18;
extern NNSFndList data_02213a0c;

extern void NNS_FndAppendListObject(void *list, void *obj);
extern void *NNS_FndGetNextListObject(void *list, void *obj);
extern void NNS_FndRemoveListObject(void *list, void *obj);
#define NNS_FndAppendListObject NNS_FndAppendListObject
#define NNS_FndGetNextListObject NNS_FndGetNextListObject
#define NNS_FndRemoveListObject NNS_FndRemoveListObject
extern void func_02007f30(u32 ch);
extern void func_02007f98(u32 ch, s32 vol);
extern void func_020382bc(void *heap);
extern void func_0203703c(SndSound *s);
extern void func_02036fec(SndPlayer *p, SndSound *s);
void func_02037130(SndSound *s);

void func_02037090(SndSound *s) {   // stop a sound and release its slot
    if (s->status == 2) {
        func_02007f98(s->channel, -723);
    }
    func_02007f30(s->channel);
    func_02037130(s);
}

SndSound *func_020370c8(int prio) {   // allocate a sound slot, stealing a lower priority one if none is free
    SndSound *s = NNS_FndGetNextListObject(&data_02213a0c, 0);
    if (s == 0) {
        s = NNS_FndGetNextListObject(&data_02213a18, 0);
        if (prio < s->priority) return 0;
        func_02037090(s);
    }
    NNS_FndRemoveListObject(&data_02213a0c, s);
    s->priority = prio;
    func_0203703c(s);
    return s;
}

void func_02037130(SndSound *s) {   // free a sound slot
    SndPlayer *p;
    if (s->handle != 0) {
        s->handle->sound = 0;
        s->handle = 0;
    }
    p = s->player;
    NNS_FndRemoveListObject(p, s);
    s->player = 0;
    if (s->heapBlk != 0) {
        NNS_FndAppendListObject(&p->heaps, s->heapBlk);
        s->heapBlk->owner = 0;
        s->heapBlk = 0;
    }
    NNS_FndRemoveListObject(&data_02213a18, s);
    NNS_FndAppendListObject(&data_02213a0c, s);
    s->status = 0;
}

void func_020371b4(SndPlayerHeapBlk *b) {   // player heap block free callback
    if (b->heap == 0) return;
    func_020382bc(b->heap);
    if (b->owner != 0) {
        b->owner->heapBlk = 0;
        return;
    }
    NNS_FndRemoveListObject(&data_02213e64[b->player].heaps, b);
}

void func_02037204(SndSound *s, u32 prio) {   // change a sound's priority and reinsert it
    SndPlayer *p = s->player;
    if (p != 0) {
        NNS_FndRemoveListObject(p, s);
        s->player = 0;
    }
    NNS_FndRemoveListObject(&data_02213a18, s);
    s->priority = prio;
    if (p != 0) func_02036fec(p, s);
    func_0203703c(s);
}
