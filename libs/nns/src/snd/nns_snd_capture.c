// NitroSystem sound library: reverb/capture effect. 0x02037884-0x02037af8
#include <nnsys/private/nns_snd_types.h>

// data_022143b4 is the start of a block whose SndCapture state sits at +0x48 (data_022143fc)
typedef struct {
    s32 first;               // 0x00
    u8 pad[0x44];
    SndCapture cap;          // 0x48
} SndCaptureBlk;
extern SndCaptureBlk data_022143b4;
extern SndCapture data_022143fc;
extern u8 data_022143bc[];

extern s32 GetRampValue(SndRamp *r);
extern void AdvanceRamp(SndRamp *r);
extern int IsRampDone(SndRamp *r);
extern void func_02008118(u32 mask, u32 a, u32 b);
extern void func_02008034(u32 chMask, u32 capMask, u32 alarmMask, u32 flags);
extern void func_0200800c(u32 chMask, u32 capMask, u32 alarmMask, u32 flags);
extern u32 SND_GetCurrentCommandTag(void);
extern void SND_FlushCommand(int);
extern void SND_WaitForCommandProc(u32);
extern int OS_ReceiveMessage(void *, u32, u32);
extern void func_0200822c(u32, u32, u32, u32);
extern void func_02036878(u32 mask);
extern void UnreserveChannels(u32 mask);
extern void ReleaseSndResBitIndex(int slot);
extern void MIi_CpuClear32(u32 data, void *dest, u32 size);
extern void DC_FlushRange(void *, u32);
void StopSndCapture(void);

void InitSndCapture(void) {   // init
    data_022143b4.first = 0;
    data_022143b4.cap.active = 0;
}

void UpdateSndCapture(void) {   // per-frame volume update
    SndCapture *c = &data_022143fc;
    s32 v;
    if (data_022143b4.cap.active == 0) return;
    if (c->mode != 0) return;
    AdvanceRamp(&c->ramp);
    if (c->rampOn != 0 && IsRampDone(&c->ramp) != 0) {
        StopSndCapture();
        return;
    }
    v = GetRampValue(&c->ramp) >> 8;
    if (v == c->lastVol) return;
    func_02008118(c->chMask, v, 0);
    c->lastVol = v;
}

void StopSndCapture(void) {   // stop
    SndCapture *c = &data_022143fc;
    int use;
    u32 mask;
    if (data_022143b4.cap.active == 0) return;
    use = c->alarm >= 0;
    mask = use ? 1 << c->alarm : 0;
    func_02008034(c->chMask, c->capSlots, mask, 0);
    if (use) {
        u32 irq = SND_GetCurrentCommandTag();
        SND_FlushCommand(1);
        SND_WaitForCommandProc(irq);
        while (OS_ReceiveMessage(data_022143bc, 0, 0) != 0) {}
    }
    if (c->capSlots != 0) func_02036878(c->capSlots);
    if (c->capMask != 0) UnreserveChannels(c->capMask);
    if (use) ReleaseSndResBitIndex(c->alarm);
    if (c->mode == 1) func_0200822c(0, 0, 0, 0);
    c->active = 0;
}

void func_02037a14(void) {   // pause
    SndCapture *c = &data_022143fc;
    u32 irq;
    u32 mask;
    if (data_022143b4.cap.active == 0) return;
    mask = c->alarm >= 0 ? 1 << c->alarm : 0;
    func_02008034(c->chMask, c->capSlots, mask, 0);
    irq = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(irq);
}

void func_02037a74(void) {   // clear buffers and restart
    SndCapture *c = &data_022143fc;
    u32 mask;
    if (data_022143b4.cap.active == 0) return;
    c->pos = 0;
    MIi_CpuClear32(0, c->buf0, c->size);
    MIi_CpuClear32(0, c->buf1, c->size);
    DC_FlushRange(c->buf0, c->size);
    DC_FlushRange(c->buf1, c->size);
    mask = c->alarm >= 0 ? 1 << c->alarm : 0;
    func_0200800c(c->chMask, c->capSlots, mask, 0);
}
