// NitroSystem sound library: wave file streaming (up to four streams read from a file or memory, decoded by a thread). 0x0203913c-0x0203a52c
#include <nnsys/private/nns_snd_arc.h>
#include <nnsys/private/nns_snd_wavstrm.h>

extern void *func_02036e10(void *a, void *b, u32 c);
extern void *func_02036f50(void *b, void *r);
extern void func_02036ea8(void *r);
extern void func_02036eb4(void *r, void *p, u32 z, u32 n);
extern void func_02036b54(void *a, u32 v);
extern void func_02036b68(void *a, u32 v);
extern void func_02036ba8(void *a, u32 v);
extern void func_02036bcc(void *a, u32 v, u32 w);
extern int func_020387b8(int id, u32 flags, void *heap, int flag, u32 *out);
extern int func_020386d4(int id, u32 flags, void *heap, int flag, u32 *out);
extern void func_02037264(WavStrm *s);
extern void func_02037504(WavStrm *s);
extern void func_02037558(WavStrm *s);
extern void func_02037574(WavStrm *s, s32 v);
extern void func_0203733c(WavStrm *s);
extern void *func_02037fd0(int idx);
extern u32 *func_02037f08(u32 id);
extern u32 func_02038098(u32 idx);
extern SndFileId func_020381b0(void);
extern void func_0200a7b4(void *file);
extern int func_0200aa24(void *file, SndFileId id);
extern int func_0200aadc(void *file);
extern int func_0200abd8(void *file);
extern int func_0200ac40(void *file, u32 pos, u32 whence);
extern int func_0200ac30(void *file, void *dst, u32 len);
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32 irq);
extern void OS_InitMutex(void *mutex);
extern void OS_LockMutex(void *mutex);
extern void OS_UnlockMutex(void *mutex);
extern void OS_SleepThread(void *queue);
extern void OS_CreateThread(void *thr, void *fn, void *arg, void *stack, u32 size, u32 prio);
extern void OS_WakeupThreadDirect(void *thr);
extern void MI_CpuCopy8(void *src, void *dst, u32 len);
extern s32 func_0203a4e0(SndLerp *l);
extern s16 data_0208f89c[];
extern void *NNS_FndGetNextListObject(void *list, void *obj);
extern int func_0203a52c(SndLerp *l);
void func_02039a50(WavStrmCmd *c);
extern void func_0203990c(WavStrm *s);
extern void func_02039604(WavStrm *s);
void func_0203969c(WavStrm *s);
void func_02039700(WavStrm *s);
void func_02039784(NNSFndList *list, WavStrm *owner);
WavStrmCmd *func_020397f4(NNSFndList *list);
extern void func_02039844(WavStrmCmd *c);
extern void func_02039870(void *ptr, u32 size, WavStrm *s);
extern void func_02039724(WavStrmMgr *m, u32 prio);
extern void func_0203a410(WavStrmMgr *m);
extern int func_02039400(void *heap);
extern void func_0203a278(WavStrm *s, u32 id);
extern int func_0203a2f8(WavStrm *s, u32 id);
extern int func_0203a36c(WavStrm *s);
extern int func_0203a37c(WavStrm *s, void *dst, u32 len, u32 off);
extern int func_0203a3b4(WavStrm *s);
extern int func_0203a3c4(WavStrm *s, u32 id);

// Sample format passed to the fill callback.
enum StrmFormat { STRM_FORMAT_PCM8, STRM_FORMAT_PCM16 };
extern void func_0203a3ec(WavStrm *s);
extern int func_0203a3f0(WavStrm *s, void *dst, u32 len, u32 off);
extern void func_0203a40c(WavStrm *s);
extern void func_0203a514(SndLerp *l);

// start a stream: sequence + bank + wave setup, returns 1 on success
int func_0203913c(void *a, void *b, int c, u32 d, u8 *e, u32 f) {
    u32 out2;
    u32 out1;
    void *r4;
    void *heap;
    r4 = func_02036e10(a, b, d);
    if (r4 == 0) return 0;
    heap = func_02036f50(b, r4);
    if (func_020387b8(c, 6, heap, 0, &out1) != 0) {
        func_02036ea8(r4);
        return 0;
    }
    if (func_020386d4(f, 1, heap, 0, &out2) != 0) {
        func_02036ea8(r4);
        return 0;
    }
    func_02036eb4(r4, (u8 *)out2 + *(u32 *)(out2 + 0x18), 0, out1);
    func_02036b54(a, e[6]);
    func_02036b68(a, e[7]);
    func_02036ba8(a, f);
    return 1;
}

int func_02039238(void *a, void *b, int c, u32 d, u32 *e, u8 *f, u32 g, u32 h) {
    u32 out;
    void *r5;
    r5 = func_02036e10(a, b, d);
    if (r5 == 0) return 0;
    if (func_020387b8(c, 6, func_02036f50(b, r5), 0, &out) != 0) {
        func_02036ea8(r5);
        return 0;
    }
    func_02036eb4(r5, f + *(u32 *)(f + 0x18), e[0], out);
    func_02036b54(a, ((u8 *)e)[6]);
    func_02036b68(a, ((u8 *)e)[7]);
    func_02036bcc(a, g, h);
    return 1;
}

void func_020392fc(u32 a, void *heap) {   // initialise the stream player
    int i;
    WavStrm *s;
    if (data_022149f0.init != 0) {
        func_02039400(heap);
        return;
    }
    data_022149f0.init = 1;
    NNS_FndInitList(&data_022149fc, 0);
    for (i = 0; i < 8; i++) {
        NNS_FndAppendListObject(&data_022149fc, &data_02214a20[i]);
    }
    OS_InitMutex(data_02214a08);
    data_022149f0.adpcmBuf = data_02215660;
    for (i = 0; i < 4; i++) {
        s = &data_0221508c[i];
        s->active = 0;
        func_0200a7b4(s->file);
        func_02037264(s);
        s->slot = i;
        s->chCount = 0;
        s->buf = 0;
        s->bufSize = 0;
        s->cnt120 = 0;
    }
    func_02039400(heap);
    func_02039724(&data_02214ba0, a);
}

int func_02039400(void *heap) {   // reserve each stream's decode buffer from the sound heap
    int i;
    u32 size;
    WavStrm *s = data_0221508c;
    u8 *info;
    void *buf;
    int j;
    for (i = 0; i < 4; i++, s++) {
        info = func_02037fd0(i);
        if (info != 0) {
            s->chCount = info[0];
            for (j = 0; j < info[0]; j++) {
                s->chan[j] = info[1 + j];
            }
            if (heap != 0) {
                size = s->chCount << 11;
                buf = func_02038390(heap, size, func_02039870, s, 0);
                if (buf == 0) return 0;
                func_02039604(s);
                s->buf = buf;
                s->bufSize = size;
            }
        }
    }
    return 1;
}

void func_020394c8(void) {   // per-frame update of all streams
    int i;
    s16 v;
    s32 sum;
    WavStrm *s = data_0221508c;
    for (i = 0; i < 4; i++, s++) {
        if (!s->active) continue;
        if (s->delay == 0) {
            func_02039604(s);
            continue;
        }
        if (s->b2 && s->f118 != 0) {
            func_02037504(s);
            s->b1 = 1;
            s->b2 = 0;
        }
        if (s->b1) {
            func_0203a514(&s->pan);
            v = data_0208f89c[s->f154];
            sum = v + data_0208f89c[func_0203a4e0(&s->pan) >> 8] + data_0208f89c[s->f158];
            if (sum != s->f15c) {
                func_02037574(s, sum);
                s->f15c = sum;
            }
            if (s->b3 && func_0203a52c(&s->pan)) {
                func_02039604(s);
            }
        }
    }
}

void func_020395d8(WavStrm *s) {
    if (s->ptr14c != 0) {
        *s->ptr14c = 0;
        s->ptr14c = 0;
    }
    s->active = 0;
    s->b2 = 0;
    s->b1 = 0;
}

void func_02039604(WavStrm *s) {   // stop a stream
    OS_LockMutex(data_02215068);
    if (data_022149f0.mgr != 0) OS_LockMutex(data_022149f0.mgr->mutex);
    if (s->b1) func_02037558(s);
    if (s->active) s->done(s);
    func_0203969c(s);
    OS_UnlockMutex(data_02215068);
    if (data_022149f0.mgr != 0) OS_UnlockMutex(data_022149f0.mgr->mutex);
}

void func_0203969c(WavStrm *s) {
    if (!s->active) return;
    func_02039700(s);
    s->close(s);
    func_02039784(&data_02215080, s);
    if (data_022149f0.mgr != 0) func_02039784(&data_022149f0.mgr->cmds, s);
    func_020395d8(s);
}

void func_02039700(WavStrm *s) {
    if (s->cnt120 == 0) return;
    if (--s->cnt120 != 0) return;
    func_0203733c(s);
}

void func_02039724(WavStrmMgr *m, u32 prio) {   // create the decode thread
    OS_CreateThread(m, func_0203a410, m, m->sleepQueue, 0x400, prio);
    NNS_FndInitList(&m->cmds, 0);
    OS_InitMutex(m->mutex);
    ((u32 *)m->sleepQueue)[1] = 0;
    ((u32 *)m->sleepQueue)[0] = 0;
    OS_WakeupThreadDirect(m);
}

void func_02039784(NNSFndList *list, WavStrm *owner) {   // drop every queued command of a stream
    u32 irq = OS_DisableInterrupts();
    WavStrmCmd *n = NNS_FndGetNextListObject(list, 0);
    WavStrmCmd *next;
    while (n != 0) {
        next = NNS_FndGetNextListObject(list, n);
        if (n->strm == owner) {
            NNS_FndRemoveListObject(list, n);
            func_02039844(n);
        }
        n = next;
    }
    OS_RestoreInterrupts(irq);
}

WavStrmCmd *func_020397f4(NNSFndList *list) {   // pop the next queued command
    u32 irq = OS_DisableInterrupts();
    WavStrmCmd *n = NNS_FndGetNextListObject(list, 0);
    if (n != 0) {
        NNS_FndRemoveListObject(list, n);
        n->strm->queued--;
    }
    OS_RestoreInterrupts(irq);
    return n;
}

void func_02039844(WavStrmCmd *n) {   // return a command node to the free list
    u32 irq = OS_DisableInterrupts();
    NNS_FndAppendListObject(&data_022149fc, n);
    OS_RestoreInterrupts(irq);
}

void func_02039870(void *ptr, u32 size, WavStrm *s) {   // sound-heap free callback of a stream buffer
    if (ptr != s->buf) return;
    OS_LockMutex(data_02215068);
    if (data_022149f0.mgr != 0) OS_LockMutex(data_022149f0.mgr->mutex);
    func_02039604(s);
    s->buf = 0;
    s->bufSize = 0;
    s->chCount = 0;
    if (s->cnt120 > 0) {
        func_0203733c(s);
        s->cnt120 = 0;
    }
    OS_UnlockMutex(data_02215068);
    if (data_022149f0.mgr != 0) OS_UnlockMutex(data_022149f0.mgr->mutex);
}

void func_0203990c(WavStrm *s) {   // switch a stream over to its next file
    WavStrmNextIn in;
    WavStrmNextOut out;
    u32 *info;
    u8 fmt;
    u16 rate;
    in.slot = s->slot;
    in.id = s->f144;
    out.id = s->f144;
    out.extra = 0;
    if (s->next(0, &in, &out, s->f140) == 0) return;
    info = func_02037f08(out.id);
    if (info == 0) return;
    fmt = s->format;
    rate = s->sampleRate;
    s->close(s);
    func_0203a278(s, *info);
    if (s->open(s, *info) == 0) return;
    if (rate != s->sampleRate) return;
    if (fmt == 0) {
        if (s->format != 0) return;
    }
    if (fmt != 0) {
        if (s->format == 0) return;
    }
    s->f144 = out.id;
    s->pos = (u32)(((unsigned long long)s->sampleRate * out.extra) / 1000);
    if (s->pos != 0 && s->format == 2) {
        s->b4 = 1;
    } else {
        s->b4 = 0;
    }
    s->b5 = 0;
}

extern s16 data_02090a70[];   // ADPCM step table
extern s8 data_02090a60[];    // ADPCM index adjustment table
extern void MI_CpuFill8(void *dst, u32 data, u32 size);
extern void DC_FlushRange(void *addr, u32 size);   // DC_FlushRange

#define ADPCM_STEP(st, nib, dst) { \
    s32 pred_ = (st)->pred; \
    s32 idx_ = *(u8 *)&(st)->index; \
    s32 step_ = data_02090a70[idx_]; \
    s32 diff_ = step_ >> 3; \
    if ((nib) & 4) diff_ += step_; \
    if ((nib) & 2) diff_ += step_ >> 1; \
    if ((nib) & 1) diff_ += step_ >> 2; \
    if ((nib) & 8) { pred_ -= diff_; if (pred_ < -0x8000) pred_ = -0x8000; } \
    else { pred_ += diff_; if (pred_ > 0x7fff) pred_ = 0x7fff; } \
    idx_ += data_02090a60[nib]; \
    if (idx_ < 0) idx_ = 0; else if (idx_ > 0x58) idx_ = 0x58; \
    (st)->pred = (s16)pred_; \
    *(u8 *)&(st)->index = idx_; \
    *(dst) = (s16)pred_; \
}

void func_02039a50(WavStrmCmd *c) {   // fill one decode buffer of a stream
    WavStrm *s;
    int loopNow;
    u32 offset;
    u32 remaining;
    u32 blockBytes;
    u32 startSample;
    u32 readPos;
    u32 samples;
    u32 outBytes;
    u32 readBytes;
    u32 endSample;
    u32 odd;
    int ch;
    u32 blk;
    u32 blockSamples;
    u32 i;
    u8 *src;
    s16 *dst;
    SndAdpcmState *st;
    u32 nib;
    s = c->strm;
    if (s->b5 && s->delay > 0) s->delay--;
    remaining = c->size;
    offset = 0;
    if (remaining != 0) {
        do {
            if (s->b5) {
                for (i = 0; (s32)i < c->channels; i++) {
                    MI_CpuFill8(c->buf[i] + offset, 0, remaining);
                }
                break;
            }
            blockSamples = s->blockSamples;
            blk = s->pos / blockSamples;
            if (blk < s->blocks - 1) {
                blockBytes = s->blockSize;
                i = blockSamples;
            } else {
                blockBytes = s->lastBlockSize;
                i = s->lastBlockSamples;
            }
            startSample = s->pos - blk * blockSamples;
            samples = remaining;
            if (s->format != 0) samples = remaining >> 1;
            if (s->b4) {
                if (startSample != 0) {
                    samples = startSample;
                    startSample = 0;
                } else {
                    s->b4 = 0;
                }
            }
            loopNow = 0;
            if (startSample + samples >= i) {
                samples = i - startSample;
                if (blk >= s->blocks - 1) {
                    if (s->loop != 0) {
                        loopNow = 1;
                    } else {
                        s->b5 = 1;
                    }
                }
            }
            outBytes = samples;
            i = startSample;
            switch (s->format) {
            case 0:
                readBytes = samples;
                break;
            case 1:
                outBytes = samples * 2;
                readBytes = outBytes;
                i = startSample * 2;
                break;
            case 2:
                readBytes = ((startSample + samples + 1) >> 1) - (startSample >> 1);
                if (startSample == 0) readBytes += 4;
                i = startSample >> 1;
                if (startSample != 0) i += 4;
                outBytes = samples * 2;
                break;
            }
            readPos = s->dataOff + s->channels * (blk * s->blockSize) + i;
            endSample = startSample + samples;
            odd = startSample & 1;
            st = s->adpcm;
            for (ch = 0; ch < c->channels; ch++, st++) {
                dst = (s16 *)(c->buf[ch] + offset);
                if (ch < s->channels) {
                    src = (u8 *)dst;
                    if (s->format == 2) {
                        OS_LockMutex(data_02214a08);
                        src = (u8 *)data_022149f0.adpcmBuf;
                    }
                    if (s->read(s, src, readBytes, readPos + ch * blockBytes) != readBytes) {
                        outBytes = 0;
                        s->b5 = 1;
                        samples = 0;
                        loopNow = 0;
                        if (s->format == 2) OS_UnlockMutex(data_02214a08);
                        break;
                    }
                    if (s->format == 2) {
                        if (startSample == 0) {
                            st->pred = ((u16 *)src)[0];
                            st->index = ((u16 *)src)[1];
                            src += 4;
                        }
                        i = startSample;
                        if (odd != 0) {
                            nib = (*src >> 4) & 0xf;
                            ADPCM_STEP(st, nib, dst);
                            dst++;
                            i++;
                            src++;
                        }
                        while (i < (endSample & ~1)) {
                            nib = *src & 0xf;
                            ADPCM_STEP(st, nib, dst);
                            dst++;
                            nib = (*src >> 4) & 0xf;
                            ADPCM_STEP(st, nib, dst);
                            dst++;
                            i += 2;
                            src++;
                        }
                        if (i < endSample) {
                            nib = *src & 0xf;
                            ADPCM_STEP(st, nib, dst);
                        }
                        OS_UnlockMutex(data_02214a08);
                    }
                } else if (s->b6) {
                    MI_CpuFill8(dst, 0, outBytes);
                } else {
                    MI_CpuCopy8(c->buf[0] + offset, dst, outBytes);
                }
            }
            if (s->b4) {
                s->b4 = 0;
            } else {
                if (loopNow != 0) {
                    s->pos = s->loopStart;
                } else {
                    s->pos = s->pos + samples;
                }
                offset += outBytes;
                remaining -= outBytes;
                if (s->b5 && s->next != 0) func_0203990c(s);
            }
        } while (remaining != 0);
    }
    if (s->fillCb != 0) {
        s->fillCb(c->phase, c->channels, c->buf, c->size, s->format == 0 ? STRM_FORMAT_PCM8 : STRM_FORMAT_PCM16, s->fillArg);
    }
    for (i = 0; (s32)i < c->channels; i++) {
        DC_FlushRange(c->buf[i], c->size);
    }
    if (c->phase == 0) s->f118 = 1;
}

void func_0203a278(WavStrm *s, u32 id) {   // choose file or memory access functions
    if (func_020381d0(id) == 0) {
        s->open = (WavStrmOpenFn)func_0203a2f8;
        s->close = (WavStrmCloseFn)func_0203a36c;
        s->read = (WavStrmReadFn)func_0203a37c;
        s->done = (WavStrmDoneFn)func_0203a3b4;
    } else {
        s->open = (WavStrmOpenFn)func_0203a3c4;
        s->close = (WavStrmCloseFn)func_0203a3ec;
        s->read = (WavStrmReadFn)func_0203a3f0;
        s->done = (WavStrmDoneFn)func_0203a40c;
    }
}

int func_0203a2f8(WavStrm *s, u32 id) {   // open from file
    SndFileId fid;
    if (func_020380e8(id, s->hdrPad, 0x40, 0) != 0x40) return 0;
    fid = func_020381b0();
    if (func_0200aa24(s->file, fid) == 0) return 0;
    s->dataBase = func_02038098(id);
    return 1;
}

int func_0203a36c(WavStrm *s) {
    return func_0200aadc(s->file);
}

int func_0203a37c(WavStrm *s, void *dst, u32 len, u32 off) {
    func_0200ac40(s->file, s->dataBase + off, 0);
    return func_0200ac30(s->file, dst, len);
}

int func_0203a3b4(WavStrm *s) {
    return func_0200abd8(s->file);
}

int func_0203a3c4(WavStrm *s, u32 id) {   // open from memory
    s->dataBase = func_020381d0(id);
    MI_CpuCopy8((void *)s->dataBase, s->hdrPad, 0x40);
    return 1;
}

void func_0203a3ec(WavStrm *s) {
}

int func_0203a3f0(WavStrm *s, void *dst, u32 len, u32 off) {
    MI_CpuCopy8((u8 *)s->dataBase + off, dst, len);
    return len;
}

void func_0203a40c(WavStrm *s) {
}

void func_0203a410(WavStrmMgr *m) {   // decode thread
    WavStrmCmd *c;
    for (;;) {
        OS_SleepThread(m->sleepQueue);
        for (;;) {
            OS_LockMutex(m->mutex);
            c = func_020397f4(&m->cmds);
            if (c == 0) {
                OS_UnlockMutex(m->mutex);
                break;
            }
            func_02039a50(c);
            func_02039844(c);
            OS_UnlockMutex(m->mutex);
        }
    }
}

typedef struct { s32 id; u32 a, b; } WavStrmEnt;   // 12 bytes, id -1 for an empty slot
typedef struct { u8 pad[0x1c]; u32 count; WavStrmEnt ent[1]; } WavStrmTable;
WavStrmEnt *func_0203a460(WavStrmTable *t, int idx) {
    if (idx < 0) return 0;
    if (idx >= t->count) return 0;
    if (t->ent[idx].id == -1) return 0;
    return &t->ent[idx];
}

void func_0203a4a0(SndLerp *l) {   // reset a ramp
    l->target = 0;
    l->cur = 0;
    l->len = 0;
    l->pos = 0;
}

void func_0203a4b8(SndLerp *l, s32 target, s32 len) {   // start a ramp from the current value
    l->cur = func_0203a4e0(l);
    l->target = target;
    l->len = len;
    l->pos = 0;
}

s32 func_0203a4e0(SndLerp *l) {   // current ramp value
    s32 from;
    if (l->pos >= l->len) return l->target;
    from = l->cur;
    return from + (l->target - from) * l->pos / l->len;
}

void func_0203a514(SndLerp *l) {   // advance a ramp
    if (l->pos < l->len) l->pos++;
}

int func_0203a52c(SndLerp *l) {   // has the ramp finished
    return l->pos >= l->len;
}
