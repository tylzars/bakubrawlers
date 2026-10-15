// Interrupt handler table: setting and reading handlers, the DMA/timer callback entries (0x020047d8-0x02004b64).

#include <nitro/private/os_types.h>

extern void OS_DisableIrqMask(u32);
extern u32 OS_EnableIrqMask(u32);

// One callback slot: function, "stay enabled" flag, argument.
typedef struct {
    void (*fn)(void *);
    u32 keep;
    void *arg;
} IrqSlot;

extern IrqSlot data_020bdc60[8];   // DMA slots 0-3, timer slots 4-7
extern IrqSlot data_020bdc90[4];   // timer slots (same storage as data_020bdc60[4..7])
extern u16 data_020a4f28[8];       // slot number -> interrupt bit index
extern struct { u32 a; u32 b; } data_027e0060;
extern OSThreadQueue data_027e0060q;

#define IRQ_CHECK (*(vu32 *) (HW_DTCM + 0x3ff8))

// func_020047d8: copies 12 words from src, three at a time, to the same
// destination address dst (nothing advances dst).
asm void func_020047d8(const u32 *src, vu32 *dst) {
    ldmia r0!, {r2, r3, ip}
    stmia r1, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1, {r2, r3, ip}
    ldmia r0!, {r2, r3, ip}
    stmia r1, {r2, r3, ip}
    bx lr
}

// func_020047fc: copies 32 words from src, eight at a time, to the single
// destination address dst (nothing advances dst).
asm void func_020047fc(const u32 *src, vu32 *dst) {
    stmfd sp!, {r4, r5, r6, r7, r8}
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, ip}
    stmia r1, {r2, r3, r4, r5, r6, r7, r8, ip}
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, ip}
    stmia r1, {r2, r3, r4, r5, r6, r7, r8, ip}
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, ip}
    stmia r1, {r2, r3, r4, r5, r6, r7, r8, ip}
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, ip}
    stmia r1, {r2, r3, r4, r5, r6, r7, r8, ip}
    ldmfd sp!, {r4, r5, r6, r7, r8}
    bx lr
}

// func_02004828(clear, mask): optionally clears 'mask' in the interrupt-check
// word, then sleeps on a thread queue until any bit of mask is set there.
void func_02004828(BOOL clear, u32 mask) {
    OSIntrMode e = OS_DisableInterrupts();
    if (clear) {
        IRQ_CHECK &= ~mask;
    }
    OS_RestoreInterrupts(e);
    while ((mask & IRQ_CHECK) == 0) {
        OS_SleepThread((OSThreadQueue *) &data_027e0060);
    }
}

// func_0200489c: empty.
void func_0200489c(void) {
}

// func_020048a0(n): common interrupt trampoline for slot n. Takes the slot's
// callback and clears it, marks the slot's interrupt bit in the check word,
// calls the callback with its argument, and masks the interrupt again unless
// the slot is flagged to stay enabled.
void func_020048a0(int n) {
    void (*fn)(void *) = data_020bdc60[n].fn;
    u32 bit = 1 << data_020a4f28[n];
    data_020bdc60[n].fn = 0;
    if (fn != 0) {
        fn(data_020bdc60[n].arg);
    }
    IRQ_CHECK |= bit;
    if (data_020bdc60[n].keep == 0) {
        OS_DisableIrqMask(bit);
    }
}

void func_02004928(void) { func_020048a0(0); }
void func_02004938(void) { func_020048a0(1); }
void func_02004948(void) { func_020048a0(2); }
void func_02004958(void) { func_020048a0(3); }
void func_02004968(void) { func_020048a0(4); }
void func_02004978(void) { func_020048a0(5); }
void func_02004988(void) { func_020048a0(6); }
void func_02004998(void) { func_020048a0(7); }

// func_020049a8: zeroes two words of the block at data_027e0060.
void func_020049a8(void) {
    data_027e0060.b = 0;
    data_027e0060.a = 0;
}

// OS_SetIrqFunction(mask, fn): for each interrupt bit set in mask (0..21),
// installs fn: bits 3-6 and 8-11 go to the callback slots (with keep=1,
// arg=0), every other bit goes into the handler table at the fast-memory base.
void OS_SetIrqFunction(u32 mask, void (*fn)(void)) {
    void (**t)(void) = (void (**)(void)) HW_DTCM;
    IrqSlot *base = data_020bdc60;
    int i;
    IrqSlot *s;
    for (i = 0; i < 22; i++, mask >>= 1) {
        if (mask & 1) {
            s = 0;
            if (i >= 8 && i <= 11) {
                s = &base[i - 8];
            } else if (i >= 3 && i <= 6) {
                s = &base[i + 1];
            } else {
                t[i] = fn;
            }
            if (s != 0) {
                s->fn = (void (*)(void *)) fn;
                s->arg = 0;
                s->keep = 1;
            }
        }
    }
}

// OS_GetIrqFunction(mask): returns the handler for the lowest interrupt bit set
// in mask (looked up in the slot array for bits 3-6 and 8-11, else in the
// handler table), or 0 if no bit below 22 is set.
void *OS_GetIrqFunction(u32 mask) {
    int i;
    void (**p)(void) = (void (**)(void)) HW_DTCM;
    for (i = 0; i < 22; i++, mask >>= 1, p++) {
        if (mask & 1) {
            if (i >= 8 && i <= 11) {
                return (void *) data_020bdc60[i - 8].fn;
            }
            if (i >= 3 && i <= 6) {
                return (void *) data_020bdc60[i + 1].fn;
            }
            return (void *) *p;
        }
    }
    return 0;
}

// OSi_EnterDmaCallback(n, fn, arg): fills DMA slot n (fn, arg), enables the
// DMA-n interrupt, and records in keep whether it was already enabled.
void OSi_EnterDmaCallback(int n, void (*fn)(void *), void *arg) {
    u32 bit = 1 << (n + 8);
    data_020bdc60[n].fn = fn;
    data_020bdc60[n].arg = arg;
    data_020bdc60[n].keep = OS_EnableIrqMask(bit) & bit;
}

// OSi_EnterTimerCallback(n, fn, arg): fills timer slot n, enables the timer-n
// interrupt, and marks the slot to stay enabled.
void OSi_EnterTimerCallback(int n, void (*fn)(void *), void *arg) {
    data_020bdc90[n].fn = fn;
    data_020bdc90[n].arg = arg;
    OS_EnableIrqMask(1 << (n + 3));
    data_020bdc90[n].keep = 1;
}
