// NitroSystem sound library: wave file streaming (up to four streams read from a file or memory, decoded by a thread). 0x0203913c-0x0203a52c
#include <nnsys/private/nns_snd_arc.h>
#include <nnsys/private/nns_snd_wavstrm.h>

extern void *AllocHandleSound(void *a, void *b, u32 c);
extern void *TakePlayerHeap(void *b, void *r);
extern void func_02036ea8(void *r);
extern void StartSndSound(void *r, void *p, u32 z, u32 n);
extern void SetHandleVolA(void *a, u32 v);
extern void func_02036b68(void *a, u32 v);
extern void SetHandleId(void *a, u32 v);
extern void SetHandleIdPair(void *a, u32 v, u32 w);
extern int LoadBankAndWaveArcs(int id, u32 flags, void *heap, int flag, u32 *out);
extern int LoadSeqAndBank(int id, u32 flags, void *heap, int flag, u32 *out);
extern void InitSndStrm(WavStrm *s);
extern void StartSndStrm(WavStrm *s);
extern void StopSndStrmIfActive(WavStrm *s);
extern void SetSndStrmVolume(WavStrm *s, s32 v);
extern void UnreserveSndStrmChannels(WavStrm *s);
extern void *GetWavStrmChannelInfo(int idx);
extern u32 *GetWavStrmFileInfo(u32 id);
extern u32 GetFatEntryOffset(u32 idx);
extern SndFileId GetCurrentArcFileId(void);
extern void FS_InitFile(void *file);
extern int FS_OpenFileById(void *file, SndFileId id);
extern int FS_CloseFile(void *file);
extern int FS_CancelFile(void *file);
extern int FS_SeekFile(void *file, u32 pos, u32 whence);
extern int FS_ReadFile(void *file, void *dst, u32 len);
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32 irq);
extern void OS_InitMutex(void *mutex);
extern void OS_LockMutex(void *mutex);
extern void OS_UnlockMutex(void *mutex);
extern void OS_SleepThread(void *queue);
extern void OS_CreateThread(void *thr, void *fn, void *arg, void *stack, u32 size, u32 prio);
extern void OS_WakeupThreadDirect(void *thr);
extern void MI_CpuCopy8(void *src, void *dst, u32 len);
extern s32 GetRampValue(SndLerp *l);
extern s16 data_0208f89c[];
extern void *NNS_FndGetNextListObject(void *list, void *obj);
extern int IsRampDone(SndLerp *l);
void FillWavStrmBuffers(WavStrmCmd *c);
extern void OpenNextWavStrmFile(WavStrm *s);
extern void StopWavStrm(WavStrm *s);
void CloseWavStrm(WavStrm *s);
void ReleaseWavStrmChannelRef(WavStrm *s);
void RemoveWavStrmCmds(NNSFndList *list, WavStrm *owner);
WavStrmCmd *PopWavStrmCmd(NNSFndList *list);
extern void FreeWavStrmCmd(WavStrmCmd *c);
extern void WavStrmBufferFreeCallback(void *ptr, u32 size, WavStrm *s);
extern void CreateWavStrmThread(WavStrmMgr *m, u32 prio);
extern void WavStrmThreadLoop(WavStrmMgr *m);
extern int SetupWavStrmBuffers(void *heap);
extern void SetWavStrmAccessFns(WavStrm *s, u32 id);
extern int OpenWavStrmFromFile(WavStrm *s, u32 id);
extern int CloseWavStrmFile(WavStrm *s);
extern int ReadWavStrmFile(WavStrm *s, void *dst, u32 len, u32 off);
extern int CancelWavStrmFile(WavStrm *s);
extern int OpenWavStrmFromMemory(WavStrm *s, u32 id);

// Sample format passed to the fill callback.
enum StrmFormat { STRM_FORMAT_PCM8, STRM_FORMAT_PCM16 };
extern void func_0203a3ec(WavStrm *s);
extern int ReadWavStrmMemory(WavStrm *s, void *dst, u32 len, u32 off);
extern void func_0203a40c(WavStrm *s);
extern void AdvanceRamp(SndLerp *l);

// start a stream: sequence + bank + wave setup, returns 1 on success
int StartSeqSound(void *a, void *b, int c, u32 d, u8 *e, u32 f) {
    u32 out2;
    u32 out1;
    void *r4;
    void *heap;
    r4 = AllocHandleSound(a, b, d);
    if (r4 == 0) return 0;
    heap = TakePlayerHeap(b, r4);
    if (LoadBankAndWaveArcs(c, 6, heap, 0, &out1) != 0) {
        func_02036ea8(r4);
        return 0;
    }
    if (LoadSeqAndBank(f, 1, heap, 0, &out2) != 0) {
        func_02036ea8(r4);
        return 0;
    }
    StartSndSound(r4, (u8 *)out2 + *(u32 *)(out2 + 0x18), 0, out1);
    SetHandleVolA(a, e[6]);
    func_02036b68(a, e[7]);
    SetHandleId(a, f);
    return 1;
}

int func_02039238(void *a, void *b, int c, u32 d, u32 *e, u8 *f, u32 g, u32 h) {
    u32 out;
    void *r5;
    r5 = AllocHandleSound(a, b, d);
    if (r5 == 0) return 0;
    if (LoadBankAndWaveArcs(c, 6, TakePlayerHeap(b, r5), 0, &out) != 0) {
        func_02036ea8(r5);
        return 0;
    }
    StartSndSound(r5, f + *(u32 *)(f + 0x18), e[0], out);
    SetHandleVolA(a, ((u8 *)e)[6]);
    func_02036b68(a, ((u8 *)e)[7]);
    SetHandleIdPair(a, g, h);
    return 1;
}

// The original's loops here are not strength-reduced, which only -O2 produces.
#pragma optimization_level 2
void InitWavStrms(u32 a, void *heap) {   // initialise the stream player
    int i;
    int j;
    WavStrm *s;
    if (data_022149f0.init != 0) {
        SetupWavStrmBuffers(heap);
        return;
    }
    data_022149f0.init = 1;
    NNS_FndInitList(&data_022149fc, 0);
    for (i = 0; i < 8; i++) {
        NNS_FndAppendListObject(&data_022149fc, &data_02214a20[i]);
    }
    OS_InitMutex(data_02214a08);
    data_022149f0.adpcmBuf = data_02215660;
    for (j = 0; j < 4; j++) {
        s = &data_0221508c[j];
        s->active = 0;
        FS_InitFile(s->file);
        InitSndStrm(s);
        s->slot = j;
        s->chCount = 0;
        s->buf = 0;
        s->bufSize = 0;
        s->cnt120 = 0;
    }
    SetupWavStrmBuffers(heap);
    CreateWavStrmThread(&data_02214ba0, a);
}

#pragma optimization_level reset
int SetupWavStrmBuffers(void *heap) {   // reserve each stream's decode buffer from the sound heap
    int i;
    u32 size;
    WavStrm *s = data_0221508c;
    u8 *info;
    void *buf;
    int j;
    for (i = 0; i < 4; i++, s++) {
        info = GetWavStrmChannelInfo(i);
        if (info != 0) {
            s->chCount = info[0];
            for (j = 0; j < info[0]; j++) {
                s->chan[j] = info[1 + j];
            }
            if (heap != 0) {
                size = s->chCount << 11;
                buf = AllocSndHeapBlock(heap, size, WavStrmBufferFreeCallback, s, 0);
                if (buf == 0) return 0;
                StopWavStrm(s);
                s->buf = buf;
                s->bufSize = size;
            }
        }
    }
    return 1;
}

void UpdateWavStrms(void) {   // per-frame update of all streams
    int i;
    s16 v;
    s32 sum;
    WavStrm *s = data_0221508c;
    for (i = 0; i < 4; i++, s++) {
        if (!s->active) continue;
        if (s->delay == 0) {
            StopWavStrm(s);
            continue;
        }
        if (s->b2 && s->f118 != 0) {
            StartSndStrm(s);
            s->b1 = 1;
            s->b2 = 0;
        }
        if (s->b1) {
            AdvanceRamp(&s->pan);
            v = data_0208f89c[s->f154];
            // Placeholder: `- -v` only reproduces the original's operand order of the first add (`add r1, r1, r6`);
            // the real source form is not known.
            sum = data_0208f89c[GetRampValue(&s->pan) >> 8] - -v + data_0208f89c[s->f158];
            if (sum != s->f15c) {
                SetSndStrmVolume(s, sum);
                s->f15c = sum;
            }
            if (s->b3 && IsRampDone(&s->pan)) {
                StopWavStrm(s);
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

void StopWavStrm(WavStrm *s) {   // stop a stream
    OS_LockMutex(data_02215068);
    if (data_022149f0.mgr != 0) OS_LockMutex(data_022149f0.mgr->mutex);
    if (s->b1) StopSndStrmIfActive(s);
    if (s->active) s->done(s);
    CloseWavStrm(s);
    OS_UnlockMutex(data_02215068);
    if (data_022149f0.mgr != 0) OS_UnlockMutex(data_022149f0.mgr->mutex);
}

void CloseWavStrm(WavStrm *s) {
    if (!s->active) return;
    ReleaseWavStrmChannelRef(s);
    s->close(s);
    RemoveWavStrmCmds(&data_02215080, s);
    if (data_022149f0.mgr != 0) RemoveWavStrmCmds(&data_022149f0.mgr->cmds, s);
    func_020395d8(s);
}

void ReleaseWavStrmChannelRef(WavStrm *s) {
    if (s->cnt120 == 0) return;
    if (--s->cnt120 != 0) return;
    UnreserveSndStrmChannels(s);
}

void CreateWavStrmThread(WavStrmMgr *m, u32 prio) {   // create the decode thread
    OS_CreateThread(m, WavStrmThreadLoop, m, m->sleepQueue, 0x400, prio);
    NNS_FndInitList(&m->cmds, 0);
    OS_InitMutex(m->mutex);
    ((u32 *)m->sleepQueue)[1] = 0;
    ((u32 *)m->sleepQueue)[0] = 0;
    OS_WakeupThreadDirect(m);
}

void RemoveWavStrmCmds(NNSFndList *list, WavStrm *owner) {   // drop every queued command of a stream
    u32 irq = OS_DisableInterrupts();
    WavStrmCmd *n = NNS_FndGetNextListObject(list, 0);
    WavStrmCmd *next;
    while (n != 0) {
        next = NNS_FndGetNextListObject(list, n);
        if (n->strm == owner) {
            NNS_FndRemoveListObject(list, n);
            FreeWavStrmCmd(n);
        }
        n = next;
    }
    OS_RestoreInterrupts(irq);
}

WavStrmCmd *PopWavStrmCmd(NNSFndList *list) {   // pop the next queued command
    u32 irq = OS_DisableInterrupts();
    WavStrmCmd *n = NNS_FndGetNextListObject(list, 0);
    if (n != 0) {
        NNS_FndRemoveListObject(list, n);
        n->strm->queued--;
    }
    OS_RestoreInterrupts(irq);
    return n;
}

void FreeWavStrmCmd(WavStrmCmd *n) {   // return a command node to the free list
    u32 irq = OS_DisableInterrupts();
    NNS_FndAppendListObject(&data_022149fc, n);
    OS_RestoreInterrupts(irq);
}

void WavStrmBufferFreeCallback(void *ptr, u32 size, WavStrm *s) {   // sound-heap free callback of a stream buffer
    if (ptr != s->buf) return;
    OS_LockMutex(data_02215068);
    if (data_022149f0.mgr != 0) OS_LockMutex(data_022149f0.mgr->mutex);
    StopWavStrm(s);
    s->buf = 0;
    s->bufSize = 0;
    s->chCount = 0;
    if (s->cnt120 > 0) {
        UnreserveSndStrmChannels(s);
        s->cnt120 = 0;
    }
    OS_UnlockMutex(data_02215068);
    if (data_022149f0.mgr != 0) OS_UnlockMutex(data_022149f0.mgr->mutex);
}

void OpenNextWavStrmFile(WavStrm *s) {   // switch a stream over to its next file
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
    info = GetWavStrmFileInfo(out.id);
    if (info == 0) return;
    fmt = s->format;
    rate = s->sampleRate;
    s->close(s);
    SetWavStrmAccessFns(s, *info);
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

// ADPCM index adjustment per 4-bit code (sign bit ignored).
static const s8 sAdpcmIndexAdjust[16] = {-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8};
// ADPCM step sizes, indexed by the running index (0..88).
static const s16 sAdpcmDiffScale[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21,
    23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66,
    73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209,
    230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658,
    724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
    7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350,
    22385, 24623, 27086, 29794, 32767,
};
extern void MI_CpuFill8(void *dst, u32 data, u32 size);
extern void DC_FlushRange(void *addr, u32 size);   // DC_FlushRange

#define ADPCM_STEP(st, x, dst) { \
    s32 step_; \
    s32 pred_; \
    s32 idx_; \
    s32 diff_; \
    u32 n_; \
    pred_ = (st)->pred; \
    idx_ = *(u8 *)&(st)->index; \
    step_ = sAdpcmDiffScale[idx_]; \
    n_ = (x) & 0xf; \
    diff_ = step_ >> 3; \
    if (n_ & 4) diff_ += step_; \
    if (n_ & 2) diff_ += step_ >> 1; \
    if (n_ & 1) diff_ += step_ >> 2; \
    if (n_ & 8) { pred_ -= diff_; if (pred_ < -0x8000) pred_ = -0x8000; } \
    else { pred_ += diff_; if (pred_ > 0x7fff) pred_ = 0x7fff; } \
    idx_ += sAdpcmIndexAdjust[n_]; \
    if (idx_ < 0) idx_ = 0; else if (idx_ > 0x58) idx_ = 0x58; \
    (st)->pred = (s16)pred_; \
    *(u8 *)&(st)->index = idx_; \
    *(dst) = (s16)pred_; \
}

static inline void AdpcmStep(SndAdpcmState *st, u32 x, s16 *dst) {
    u32 n;
    s32 diff;
    s32 idx;
    s32 pred;
    s32 step;
    pred = st->pred;
    idx = *(u8 *)&st->index;
    step = sAdpcmDiffScale[idx];
    n = x & 0xf;
    diff = step >> 3;
    if (n & 4) diff += step;
    if (n & 2) diff += step >> 1;
    if (n & 1) diff += step >> 2;
    if (n & 8) { pred -= diff; if (pred < -0x8000) pred = -0x8000; }
    else { pred += diff; if (pred > 0x7fff) pred = 0x7fff; }
    idx += sAdpcmIndexAdjust[n];
    if (idx < 0) idx = 0; else if (idx > 0x58) idx = 0x58;
    st->pred = (s16)pred;
    *(u8 *)&st->index = idx;
    *dst = (s16)pred;
}

static inline void DecodeAdpcm(u32 i, u32 n, s16 *dst, u8 *src, SndAdpcmState *st) {
    u32 endSample = i + n;
    if (i & 1) {
        ADPCM_STEP(st, *src >> 4, dst++);
        i++;
        src++;
    }
    while (i < (endSample & ~1)) {
        AdpcmStep(st, *src, dst++);
        ADPCM_STEP(st, *src >> 4, dst++);
        i += 2;
        src++;
    }
    if (i < endSample) {
        AdpcmStep(st, *src, dst);
    }
}
// The low-nibble steps go through AdpcmStep and the high-nibble steps through ADPCM_STEP. The two forms number
// their locals differently (the macro's block locals in source order, the inline function's after them and in
// reverse declaration order), and that numbering is what gives the pair loop's low-nibble step the original's
// registers. The mixed form is a structural stand-in for that order; a single helper of either kind (all macro or
// all inline) does not match. ADPCM_STEP evaluates its dst argument exactly once, so `dst++` there is safe.
void FillWavStrmBuffers(WavStrmCmd *c) {   // fill one decode buffer of a stream
    WavStrm *s;
    int loopNow;
    unsigned long offset;
    u32 remaining;
    u32 blockBytes;
    u32 startSample;
    u32 readPos;
    u32 samples;
    u32 outBytes;
    u32 readBytes;
    int ch;
    u32 blk;
    u32 blockSamples;
    u32 i;
    u8 *src;
    s16 *dst;
    SndAdpcmState *st;
    u32 got;
    s = c->strm;
    if (s->b5 && s->delay > 0) s->delay--;
    remaining = c->size;
    offset = 0;
    if (remaining != 0) {
        do {
            if (s->b5) {
                for (ch = 0; ch < c->channels; ch++) {
                    MI_CpuFill8(c->buf[ch] + offset, 0, remaining);
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
                if (startSample == 0) {
                    s->b4 = 0;
                } else {
                    samples = startSample;
                    startSample = 0;
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
            i = startSample;
            outBytes = samples;
            switch (s->format) {
            case 0:
                readBytes = samples;
                break;
            case 1:
                outBytes *= 2;
                readBytes = outBytes;
                i = startSample * 2;
                break;
            case 2:
                readBytes = ((startSample + samples + 1) >> 1) - (startSample >> 1);
                i = startSample >> 1;
                if (startSample == 0) {
                    readBytes += 4;
                } else {
                    i += 4;
                }
                outBytes *= 2;
                break;
            }
            readPos = s->channels * (blk * s->blockSize) + i;
            readPos += s->dataOff;
            ch = 0;
            if (c->channels > 0) {
                do {
                    st = &s->adpcm[ch];
                    src = c->buf[ch] + offset;
                    dst = (s16 *)src;
                    if (ch < s->channels) {
                        if (s->format == 2) {
                            OS_LockMutex(data_02214a08);
                            src = (u8 *)data_022149f0.adpcmBuf;
                        }
                        got = s->read(s, src, readBytes, readPos + ch * blockBytes);
                        if (got != readBytes) {
                            outBytes = 0;
                            s->b5 = 1;
                            samples = 0;
                            loopNow = 0;
                            if (s->format == 2) OS_UnlockMutex(data_02214a08);
                            break;
                        }
                        if (s->format == 2) {
                            {
                            SndAdpcmState *h = (SndAdpcmState *)data_022149f0.adpcmBuf;
                            src = (u8 *)h;
                            if (startSample == 0) {
                                src = (u8 *)(h + 1);
                                *st = *h;
                            }
                            }
                            DecodeAdpcm(startSample, samples, dst, src, st);
                            OS_UnlockMutex(data_02214a08);
                        }
                    } else if (s->b6) {
                        MI_CpuFill8(dst, 0, outBytes);
                    } else {
                        MI_CpuCopy8(c->buf[0] + offset, dst, outBytes);
                    }
                } while (++ch < c->channels);
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
                if (s->b5 && s->next != 0) OpenNextWavStrmFile(s);
            }
        } while (remaining != 0);
    }
    if (s->fillCb != 0) {
        s->fillCb(c->phase, c->channels, c->buf, c->size, s->format == 0 ? STRM_FORMAT_PCM8 : STRM_FORMAT_PCM16, s->fillArg);
    }
    for (ch = 0; ch < c->channels; ch++) {
        DC_FlushRange(c->buf[ch], c->size);
    }
    if (c->phase == 0) s->f118 = 1;
}

void SetWavStrmAccessFns(WavStrm *s, u32 id) {   // choose file or memory access functions
    if (GetFatEntryCache(id) == 0) {
        s->open = (WavStrmOpenFn)OpenWavStrmFromFile;
        s->close = (WavStrmCloseFn)CloseWavStrmFile;
        s->read = (WavStrmReadFn)ReadWavStrmFile;
        s->done = (WavStrmDoneFn)CancelWavStrmFile;
    } else {
        s->open = (WavStrmOpenFn)OpenWavStrmFromMemory;
        s->close = (WavStrmCloseFn)func_0203a3ec;
        s->read = (WavStrmReadFn)ReadWavStrmMemory;
        s->done = (WavStrmDoneFn)func_0203a40c;
    }
}

int OpenWavStrmFromFile(WavStrm *s, u32 id) {   // open from file
    SndFileId fid;
    if (ReadFatEntryData(id, s->hdrPad, 0x40, 0) != 0x40) return 0;
    fid = GetCurrentArcFileId();
    if (FS_OpenFileById(s->file, fid) == 0) return 0;
    s->dataBase = GetFatEntryOffset(id);
    return 1;
}

int CloseWavStrmFile(WavStrm *s) {
    return FS_CloseFile(s->file);
}

int ReadWavStrmFile(WavStrm *s, void *dst, u32 len, u32 off) {
    FS_SeekFile(s->file, s->dataBase + off, 0);
    return FS_ReadFile(s->file, dst, len);
}

int CancelWavStrmFile(WavStrm *s) {
    return FS_CancelFile(s->file);
}

int OpenWavStrmFromMemory(WavStrm *s, u32 id) {   // open from memory
    s->dataBase = GetFatEntryCache(id);
    MI_CpuCopy8((void *)s->dataBase, s->hdrPad, 0x40);
    return 1;
}

void func_0203a3ec(WavStrm *s) {
}

int ReadWavStrmMemory(WavStrm *s, void *dst, u32 len, u32 off) {
    MI_CpuCopy8((u8 *)s->dataBase + off, dst, len);
    return len;
}

void func_0203a40c(WavStrm *s) {
}

void WavStrmThreadLoop(WavStrmMgr *m) {   // decode thread
    WavStrmCmd *c;
    for (;;) {
        OS_SleepThread(m->sleepQueue);
        for (;;) {
            OS_LockMutex(m->mutex);
            c = PopWavStrmCmd(&m->cmds);
            if (c == 0) {
                OS_UnlockMutex(m->mutex);
                break;
            }
            FillWavStrmBuffers(c);
            FreeWavStrmCmd(c);
            OS_UnlockMutex(m->mutex);
        }
    }
}

// The record table inside a loaded sequence-archive file: func_02039090 (nns_snd_player_init.c) looks up the
// file of an INFO sequence archive record and takes record idx from it. 12 bytes each, id -1 for an empty slot;
// bytes 4-9 are read like SndSeqRec's (nns_snd_player_init.c).
typedef struct { s32 id; u32 a, b; } SndSeqArcEnt;
typedef struct { u8 pad[0x1c]; u32 count; SndSeqArcEnt ent[1]; } SndSeqArcTable;
SndSeqArcEnt *func_0203a460(SndSeqArcTable *t, int idx) {
    if (idx < 0) return 0;
    if (idx >= t->count) return 0;
    if (t->ent[idx].id == -1) return 0;
    return &t->ent[idx];
}

void ClearRamp(SndLerp *l) {   // reset a ramp
    l->target = 0;
    l->cur = 0;
    l->len = 0;
    l->pos = 0;
}

void StartRamp(SndLerp *l, s32 target, s32 len) {   // start a ramp from the current value
    l->cur = GetRampValue(l);
    l->target = target;
    l->len = len;
    l->pos = 0;
}

s32 GetRampValue(SndLerp *l) {   // current ramp value
    s32 from;
    if (l->pos >= l->len) return l->target;
    from = l->cur;
    return from + (l->target - from) * l->pos / l->len;
}

void AdvanceRamp(SndLerp *l) {   // advance a ramp
    if (l->pos < l->len) l->pos++;
}

int IsRampDone(SndLerp *l) {   // has the ramp finished
    return l->pos >= l->len;
}
