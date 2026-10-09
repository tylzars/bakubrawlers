// NitroSystem sound library: players, player heaps, sound slots and the per-frame sound update. 0x02036918-0x02037090
#include <nnsys/private/nns_types.h>

typedef struct SndHandle SndHandle;
typedef struct { s32 value; s32 target; s32 step; s32 frames; } SndRamp;   // volume ramp

typedef struct SndPlayer {
    NNSFndList sounds;       // 0x00 active sounds, ordered by priority (link offset 0xc)
    NNSFndList heaps;        // 0x0c player heaps (link offset 0)
    u32 maxSounds;           // 0x18
    u32 heapSize;            // 0x1c
    u8 volume;               // 0x20
    u8 pad[3];
} SndPlayer;

typedef struct SndSound {
    SndHandle *handle;   // 0x00
    SndPlayer *player;          // 0x04
    u32 pad8;                   // 0x08
    u32 padc;                   // 0x0c
    u32 pad10;                  // 0x10
    NNSFndLink link;            // 0x14
    SndRamp ramp;               // 0x1c
    u8 status;                  // 0x2c  0 free, 1 playing, 2 fading out
    u8 started;                 // 0x2d
    u8 pad2e;
    u8 pending;                 // 0x2f
    u32 tick;                   // 0x30
    u16 kind;                   // 0x34
    u16 pad36;
    u16 idA;                    // 0x38
    u16 idB;                    // 0x3a
    u8 channel;                 // 0x3c
    u8 priority;                // 0x3d
    s16 volume;                 // 0x3e
    u8 volA;                    // 0x40
    u8 volB;                    // 0x41
    u16 pad42;
} SndSound;

struct SndHandle { struct SndSound * volatile sound; };
static inline int HandleIsValid(SndHandle *h) { return h->sound != 0; }
typedef struct { u32 pad0; u32 pad4; void *heap; void *owner; u32 player; } SndPlayerHeapBlk;

extern SndPlayer data_02213e64[32];
extern SndSound data_02213a24[16];
extern NNSFndList data_02213a18;
extern NNSFndList data_02213a0c;

extern void NNS_FndInitList(void *list, u16 offset);
extern void NNS_FndAppendListObject(void *list, void *obj);
extern void *NNS_FndGetNextListObject(void *list, void *obj);
extern void NNS_FndInsertListObject(void *list, void *target, void *obj);
extern void NNS_FndRemoveListObject(void *list, void *obj);
#define NNS_FndInitList NNS_FndInitList
#define NNS_FndAppendListObject NNS_FndAppendListObject
#define NNS_FndGetNextListObject NNS_FndGetNextListObject
#define NNS_FndInsertListObject NNS_FndInsertListObject
#define NNS_FndRemoveListObject NNS_FndRemoveListObject
extern void func_02007f50(u32 ch);
extern void func_02007f78(u32 ch);
extern void func_02007f98(u32 ch, s32 vol);
extern void func_02007fb0(u32 ch, u32 a);
extern void func_02007fc8(u32 ch, u32 a, int b);
extern void func_02007fe4(u32 ch, u32 a, u32 b);
extern u32 func_02008b1c(void);
extern int SND_IsFinishedCommandTag(u32 tick);
extern u32 SND_GetCurrentCommandTag(void);
extern void *func_02038390(void *heap, u32 size, void *cb, u32 a, u32 b);
extern void *func_0203824c(void *where, u32 size);
extern void func_020382d4(void *heap);
extern void func_020371b4(void);
extern void func_02037090(SndSound *s);
extern void func_02037130(SndSound *s);
extern void func_02037204(SndSound *s, u32 a);
extern SndSound *func_020370c8(u32 prio);
extern void func_0203a4a0(SndRamp *r);
extern void func_0203a4b8(SndRamp *r, s32 value, u32 frames);
extern s32 func_0203a4e0(SndRamp *r);
extern void func_0203a514(SndRamp *r);
extern int func_0203a52c(SndRamp *r);
void func_02036ac0(SndHandle *h);
void func_02036f04(SndSound *s, u32 fade);
void func_02036fa4(SndSound *s);
void func_02036fec(SndPlayer *p, SndSound *s);

void func_02036918(u32 player, u32 count) {   // NNS_SndPlayerSetPlayableSoundCount
    data_02213e64[player].maxSounds = (u16)count;
}

void func_02036938(u32 player, u32 size) {
    data_02213e64[player].heapSize = size;
}

int func_02036950(u32 player, void *heap, u32 size) {   // NNS_SndPlayerCreateHeap
    SndPlayerHeapBlk *b = func_02038390(heap, size + 0x14, func_020371b4, 0, 0);
    if (b == 0) return 0;
    b->owner = 0;
    b->player = player;
    b->heap = 0;
    {
        void *h = func_0203824c((u8 *)b + 0x14, size);
        if (h == 0) return 0;
        b->heap = h;
    }
    NNS_FndAppendListObject(&data_02213e64[player].heaps, b);
    return 1;
}

void func_020369e4(SndHandle *h, u32 fade) {   // NNS_SndPlayerStopSound
    func_02036f04(h->sound, fade);
}

void func_020369f4(u32 player, u32 fade) {   // stop every sound on a player
    int i;
    SndSound *s = data_02213a24;
    SndPlayer *p = &data_02213e64[player];
    for (i = 0; i < 16; i++, s++) {
        if (s->status != 0 && s->player == p) func_02036f04(s, fade);
    }
}

void func_02036a50(int a, int b, u32 fade) {   // stop every sound with this id pair
    int i;
    for (i = 0; i < 16; i++) {
        SndSound *s = &data_02213a24[i];
        if (s->status != 0 && s->kind == 2 && s->idA == a && s->idB == b) func_02036f04(s, fade);
    }
}

void func_02036ab4(SndHandle *h) {   // NNS_SndHandleInit
    h->sound = 0;
}

void func_02036ac0(SndHandle *h) {   // NNS_SndHandleReleaseSound
    if (h->sound == 0) return;
    h->sound->handle = 0;
    h->sound = 0;
}

int func_02036ae0(int a, int b) {   // number of live sounds with this id pair
    int n = 0;
    SndSound *s = NNS_FndGetNextListObject(&data_02213a18, 0);
    while (s != 0) {
        if (s->kind == 2 && s->idA == a && s->idB == b) n++;
        s = NNS_FndGetNextListObject(&data_02213a18, s);
    }
    return n;
}

void func_02036b40(SndHandle *h, u8 v) {
    if (h->sound != 0) h->sound->volB = v;
}

void func_02036b54(SndHandle *h, u8 v) {
    if (h->sound != 0) h->sound->volA = v;
}

void func_02036b68(SndHandle *h, u32 a) {
    if (h->sound == 0) return;
    func_02007fb0(h->sound->channel, a);
}

void func_02036b88(SndHandle *h, u32 a, int b) {
    if (h->sound == 0) return;
    func_02007fc8(h->sound->channel, a, b);
}

void func_02036ba8(SndHandle *h, u16 id) {   // tag the sound with a single id
    if (h->sound == 0) return;
    h->sound->kind = 1;
    h->sound->idA = id;
}

void func_02036bcc(SndHandle *h, u16 a, u16 b) {   // tag the sound with an id pair
    if (h->sound == 0) return;
    h->sound->kind = 2;
    h->sound->idA = a;
    h->sound->idB = b;
}

void func_02036bf8(void) {   // reset the sound slots and players
    int i;
    NNS_FndInitList(&data_02213a18, 0x14);
    NNS_FndInitList(&data_02213a0c, 0x14);
    for (i = 0; i < 16; i++) {
        data_02213a24[i].status = 0;
        data_02213a24[i].channel = i;
        NNS_FndAppendListObject(&data_02213a0c, &data_02213a24[i]);
    }
    for (i = 0; i < 32; i++) {
        NNS_FndInitList(&data_02213e64[i].sounds, 0xc);
        NNS_FndInitList(&data_02213e64[i].heaps, 0);
        data_02213e64[i].volume = 0x7f;
        data_02213e64[i].maxSounds = 1;
        data_02213e64[i].heapSize = 0;
    }
}

extern s16 data_0208f89c[];

void func_02036cac(void) {   // per-frame update of every active sound
    u32 doneMask = func_02008b1c();
    SndSound *s = NNS_FndGetNextListObject(&data_02213a18, 0);
    if (s == 0) return;
    do {
        SndSound *next = NNS_FndGetNextListObject(&data_02213a18, s);
        if (s->started == 0) {
            if (SND_IsFinishedCommandTag(s->tick) != 0) s->started = 1;
        }
        if (s->started != 0 && !(doneMask & (1 << s->channel))) {
            func_02037130(s);
        } else {
            s32 vol, vB, vA, vp;
            func_0203a514(&s->ramp);
            vB = data_0208f89c[s->volB];
            vA = data_0208f89c[s->volA];
            vp = data_0208f89c[s->player->volume];
            vol = data_0208f89c[func_0203a4e0(&s->ramp) >> 8];
            vol += vp + (vA + vB);
            if (vol < -0x8000) vol = -0x8000;
            else if (vol > 0x7fff) vol = 0x7fff;
            if (vol != s->volume) {
                func_02007f98(s->channel, vol);
                s->volume = vol;
            }
            if (s->status == 2 && func_0203a52c(&s->ramp) != 0) func_02037090(s);
            if (s->pending != 0) {
                func_02007f78(s->channel);
                s->pending = 0;
            }
        }
        s = next;
    } while (s != 0);
}

SndSound *func_02036e10(SndHandle *h, u32 player, int prio) {   // allocate a sound slot for a handle
    SndPlayer *p = &data_02213e64[player];
    SndSound *s = *(SndSound **)h;
    if (s != 0) func_02036ac0(h);
    if (p->sounds.count >= p->maxSounds) {
        SndSound *old = NNS_FndGetNextListObject(p, 0);
        if (old == 0) return 0;
        if (prio < old->priority) return 0;
        func_02037090(old);
    }
    s = func_020370c8(prio);
    if (s == 0) return 0;
    func_02036fec(p, s);
    s->handle = h;
    h->sound = s;
    return s;
}

void func_02036ea8(SndSound *s) {
    func_02037130(s);
}

void func_02036eb4(SndSound *s) {   // start the sound's channel
    SndPlayer *p = s->player;
    func_02007f50(s->channel);
    if (p->heapSize != 0) func_02007fe4(s->channel, 0xffff, p->heapSize);
    func_02036fa4(s);
    s->tick = SND_GetCurrentCommandTag();
    s->pending = 1;
    s->status = 1;
}

void func_02036f04(SndSound *s, u32 fade) {   // stop a sound, optionally fading out
    if (s == 0 || s->status == 0) return;
    if (fade == 0) {
        func_02037090(s);
        return;
    }
    func_0203a4b8(&s->ramp, 0, fade);
    func_02037204(s, 0);
    s->status = 2;
}

void *func_02036f50(u32 player, SndPlayerHeapBlk *dst) {   // take a player heap block for use
    SndPlayer *p = &data_02213e64[player];
    SndPlayerHeapBlk *b = NNS_FndGetNextListObject(&p->heaps, 0);
    if (b == 0) return 0;
    NNS_FndRemoveListObject(&p->heaps, b);
    b->owner = dst;
    *(void **)((u8 *)dst + 8) = b;
    func_020382d4(b->heap);
    return b->heap;
}

void func_02036fa4(SndSound *s) {   // reset a slot to defaults
    s->pad2e = 0;
    s->started = 0;
    s->pending = 0;
    s->kind = 0;
    s->volume = 0;
    s->volA = 0x7f;
    s->volB = 0x7f;
    func_0203a4a0(&s->ramp);
    func_0203a4b8(&s->ramp, 0x7f00, 1);
}

void func_02036fec(SndPlayer *p, SndSound *s) {   // insert into the player's priority-ordered list
    SndSound *o;
    for (o = NNS_FndGetNextListObject(p, 0); o != 0; o = NNS_FndGetNextListObject(p, o)) {
        if (s->priority < o->priority) break;
    }
    NNS_FndInsertListObject(p, o, s);
    s->player = p;
}

void func_0203703c(SndSound *s) {   // insert into the global active list
    SndSound *o;
    for (o = NNS_FndGetNextListObject(&data_02213a18, 0); o != 0; o = NNS_FndGetNextListObject(&data_02213a18, o)) {
        if (s->priority < o->priority) break;
    }
    NNS_FndInsertListObject(&data_02213a18, o, s);
}
