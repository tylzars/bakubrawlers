// NitroSystem sound library: PCM stream (NNSSndStrm). 0x02037264-0x02037884
#include <nnsys/private/nns_snd_types.h>

extern u32 data_022142e4;             // list initialised flag
extern NNSFndList data_022142e8;      // active streams
extern SndStrmChan data_02214334[16]; // per hardware channel buffer address
extern void *data_022142f4[16];       // callback buffer pointers

extern void NNS_FndInitList(void *list, u16 offset);
extern void NNS_FndAppendListObject(void *list, void *obj);
extern void NNS_FndRemoveListObject(void *list, void *obj);
#define NNS_FndInitList NNS_FndInitList
#define NNS_FndAppendListObject NNS_FndAppendListObject
#define NNS_FndRemoveListObject NNS_FndRemoveListObject
extern int ReserveChannels(u32 mask);
extern void UnreserveChannels(u32 mask);
extern int AllocSndResBitIndex(void);
extern void FreeSndResBitIndex(int slot);
extern void func_02008160(u32 ch, u32 fmt, void *addr, u32 loop, u32 loopStart, u32 loopEnd, u32 vol, u32 shift, u32 timer, u32 pan);
extern void func_02008098(int slot, u32 start, u32 period, void (*cb)(void *), void *arg);
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32 irq);
extern void func_0200800c(u32 chMask, u32 capMask, u32 alarmMask, u32 flags);
extern void func_02008034(u32 chMask, u32 capMask, u32 alarmMask, u32 flags);
extern void func_0200da20(void *);
extern void func_0200da38(void *);
extern void func_0200da50(void *);
extern void func_0200da68(void *);
extern u32 SND_GetCurrentCommandTag(void);
extern void SND_FlushCommand(int);
extern void SND_WaitForCommandProc(u32);
extern s32 func_02008bec(u32);
extern void func_02008118(u32 mask, u32 a, u32 b);
extern void func_02008140(u32 mask, u32 a, u32 b, u32 c);
void StopSndStrmIfActive(SndStrm *s);
void StopSndStrm(SndStrm *s);
void DeactivateSndStrm(SndStrm *s);
void func_02037708(SndStrm *s);
void RunSndStrmCallback(SndStrm *s, int phase);
void func_020377c8(SndStrm *s);
void func_02037814(SndStrm *s);

void InitSndStrm(SndStrm *s) {   // stream init
    if (data_022142e4 == 0) {
        NNS_FndInitList(&data_022142e8, 0);
        data_022142e4 = 1;
    }
    s->cbA.fn = (void (*)(void *))func_020377c8;
    s->cbA.arg = s;
    s->cbB.fn = (void (*)(void *))func_02037814;
    s->cbB.arg = s;
    s->chMask = 0;
    s->chCount = 0;
    s->active = 0;
    s->registered = 0;
}

int ReserveSndStrmChannels(SndStrm *s, int count, u8 *chans) {   // reserve channels
    u32 mask = 0;
    int i;
    for (i = 0; i < count; i++) {
        s->chan[i] = chans[i];
        mask |= 1 << chans[i];
    }
    if (ReserveChannels(mask) == 0) return 0;
    s->chCount = count;
    s->chMask = mask;
    return 1;
}

void UnreserveSndStrmChannels(SndStrm *s) {   // release channels
    if (s->chMask == 0) return;
    UnreserveChannels(s->chMask);
    s->chMask = 0;
    s->chCount = 0;
}

int SetupSndStrm(SndStrm *s, int fmt, void *buf, u32 size, u32 timer, s32 frames, SndStrmCallback cb, void *cbArg) {   // setup
    int i;
    u32 alarmPeriod;
    u32 q;
    u32 n;
    if (s->active) StopSndStrmIfActive(s);
    q = size / (s->chCount * (frames << 5));
    n = q * frames;
    s->bufSize = n << 5;
    {
        u32 half = s->bufSize;
        if (fmt == 1) half >>= 1;
        alarmPeriod = timer * half / frames;
    }
    s->slot = AllocSndResBitIndex();
    if (s->slot < 0) return 0;
    for (i = 0; i < s->chCount; i++) {
        u32 ch = s->chan[i];
        data_02214334[ch].addr = (u8 *)buf + s->bufSize * i;
        data_02214334[ch].off = 0;
        func_02008160(ch, fmt, data_02214334[ch].addr, 1, 0, s->bufSize >> 2, 127, 0, timer << 5, 64);
    }
    func_02008098(s->slot, alarmPeriod, alarmPeriod, (void (*)(void *))func_02037708, s);
    NNS_FndAppendListObject(&data_022142e8, s);
    s->fmt = fmt;
    s->frames = frames;
    s->cb = cb;
    s->cbArg = cbArg;
    s->pos = 0;
    s->offset = 0;
    s->active = 1;
    {
        u32 irq = OS_DisableInterrupts();
        s->frames = 1;
        RunSndStrmCallback(s, 0);
        s->frames = frames;
        OS_RestoreInterrupts(irq);
    }
    return 1;
}

void func_02037504(SndStrm *s) {   // start
    func_0200800c(s->chMask, 0, 1 << s->slot, 0);
    if (s->registered) return;
    func_0200da20(&s->cbA);
    func_0200da38(&s->cbB);
    s->registered = 1;
}

void StopSndStrmIfActive(SndStrm *s) {   // stop if running
    if (!s->active) return;
    StopSndStrm(s);
}

void func_02037574(SndStrm *s, int pos) {   // seek all channels
    u32 ch;
    int i;
    s->offset = pos;
    for (i = 0; i < s->chCount; i++) {
        s32 v;
        ch = s->chan[i];
        v = func_02008bec(s->offset + data_02214334[ch].off);
        func_02008118(1 << ch, v & 0xff, v >> 8);
    }
}

void func_020375e0(SndStrm *s, int idx, u32 off) {   // seek one channel
    s32 v;
    if (idx > s->chCount - 1) return;
    idx = s->chan[idx];
    data_02214334[idx].off = off;
    v = func_02008bec(s->offset + data_02214334[idx].off);
    func_02008118(1 << idx, v & 0xff, v >> 8);
}

void func_02037634(SndStrm *s, int idx, u32 v) {
    if (idx > s->chCount - 1) return;
    func_02008140(1 << s->chan[idx], v, v, 1);
}

void StopSndStrm(SndStrm *s) {   // stop
    if (s->registered) {
        u32 irq;
        func_02008034(s->chMask, 0, 1 << s->slot, 0);
        func_0200da50(&s->cbA);
        func_0200da68(&s->cbB);
        s->registered = 0;
        irq = SND_GetCurrentCommandTag();
        SND_FlushCommand(1);
        SND_WaitForCommandProc(irq);
    }
    DeactivateSndStrm(s);
}

void DeactivateSndStrm(SndStrm *s) {   // release slot and unlink
    FreeSndResBitIndex(s->slot);
    NNS_FndRemoveListObject(&data_022142e8, s);
    s->active = 0;
}

void func_02037708(SndStrm *s) {
    RunSndStrmCallback(s, 1);
}

void RunSndStrmCallback(SndStrm *s, int phase) {   // fill callback
    u32 size = s->bufSize / s->frames;
    u32 off = size * s->pos;
    u32 base;
    int i;
    for (i = 0; i < s->chCount; i++) {
        base = (u32)data_02214334[s->chan[i]].addr;
        data_022142f4[i] = (void *)(base + off);
    }
    s->cb(phase, s->chCount, data_022142f4, size, s->fmt, s->cbArg);
    s->pos++;
    if (s->pos >= s->frames) s->pos = 0;
}

void func_020377c8(SndStrm *s) {
    u32 irq;
    if (!s->registered) return;
    func_02008034(s->chMask, 0, 1 << s->slot, 0);
    irq = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(irq);
}

void func_02037814(SndStrm *s) {
    if (!s->registered) return;
    while (s->pos != 0) {
        u32 irq = OS_DisableInterrupts();
        RunSndStrmCallback(s, 1);
        OS_RestoreInterrupts(irq);
    }
    func_0200800c(s->chMask, 0, 1 << s->slot, 0);
}
