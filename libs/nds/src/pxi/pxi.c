#include <nitro/private/os_types.h>

extern u32 OS_ResetRequestIrqMask(u32 mask);   // 0x02004bf0
extern u32 OS_EnableIrqMask(u32 mask);         // 0x02004b90
extern void OS_SetIrqFunction(u32 mask, void (*func)(void));   // 0x020049c0
extern void PxiFifoSetup(void);
extern void PXIi_HandlerRecvFifoNotEmpty(void);

#define REG_IPCSYNC (*(vu16 *) 0x4000180)
#define REG_IPCFIFOCNT (*(vu16 *) 0x4000184)
#define REG_IPCFIFOSEND (*(vu32 *) 0x4000188)
#define REG_IPCFIFORECV (*(vu32 *) 0x4100000)
typedef struct {
    u8 pad[0x388];
    vu32 flags[2];    // 0x388: registered tag bits, per processor
} PXIWork;
static PXIWork *const pxiWork = (PXIWork *) 0x27ffc00;
#define PXI_WORK pxiWork
#define PXI_PROC_FLAGS(proc) (PXI_WORK->flags[proc])

typedef void (*PXIFifoCallback)(int tag, u32 data, BOOL err);

typedef union {
    u32 raw;
    struct {
        u32 tag : 5;
        u32 err : 1;
        u32 data : 26;
    } bits;
} PXIFifoMessage;

static u16 pxiInitialized;                  // 0x020bfc04
static PXIFifoCallback pxiCallback[32];    // 0x020bfc08

void PXI_Init(void) {
    PxiFifoSetup();
}

void PxiFifoSetup(void) {
    OSIntrMode e = OS_DisableInterrupts();
    int count;
    u16 remote;
    int timeout;
    if (pxiInitialized == 0) {
        int i;
        pxiInitialized = 1;
        PXI_PROC_FLAGS(0) = 0;
        for (i = 0; i < 32; i++) {
            pxiCallback[i] = NULL;
        }
        REG_IPCFIFOCNT = 0xc408;
        OS_ResetRequestIrqMask(0x40000);
        OS_SetIrqFunction(0x40000, PXIi_HandlerRecvFifoNotEmpty);
        OS_EnableIrqMask(0x40000);
        count = 0;
        while (1) {
            remote = REG_IPCSYNC & 0xf;
            REG_IPCSYNC = remote << 8;
            if (remote == 0 && count > 4) {
                break;
            }
            timeout = 1000;
            if ((REG_IPCSYNC & 0xf) == remote) {
                while (1) {
                    if (timeout <= 0) {
                        count = 0;
                        break;
                    }
                    timeout--;
                    if ((REG_IPCSYNC & 0xf) != remote) {
                        break;
                    }
                }
            }
            count++;
        }
    }
    OS_RestoreInterrupts(e);
}

void PXI_SetFifoRecvCallback(int fifoTag, PXIFifoCallback callback) {
    OSIntrMode e = OS_DisableInterrupts();
    pxiCallback[fifoTag] = callback;
    {
        u32 v;
        if (callback != NULL) {
            v = PXI_PROC_FLAGS(0) | (1 << fifoTag);
        } else {
            v = PXI_PROC_FLAGS(0) & ~(1 << fifoTag);
        }
        PXI_PROC_FLAGS(0) = v;
    }
    OS_RestoreInterrupts(e);
}

BOOL PXI_IsCallbackReady(int fifoTag, int proc) {
    BOOL ready = TRUE;
    if (!(PXI_PROC_FLAGS(proc) & (1 << fifoTag))) {
        ready = FALSE;
    }
    return ready;
}

int PXI_SendWordByFifo(int fifoTag, u32 data, BOOL err) {
    PXIFifoMessage msg;
    OSIntrMode e;
    msg.bits.tag = fifoTag;
    msg.bits.err = err;
    msg.bits.data = data;
    if (REG_IPCFIFOCNT & 0x4000) {
        REG_IPCFIFOCNT |= 0xc000;
        return -1;
    }
    e = OS_DisableInterrupts();
    if (REG_IPCFIFOCNT & 2) {
        OS_RestoreInterrupts(e);
        return -2;
    }
    REG_IPCFIFOSEND = msg.raw;
    OS_RestoreInterrupts(e);
    return 0;
}

void PXIi_HandlerRecvFifoNotEmpty(void) {
    PXIFifoMessage msg;
    int result;
    OSIntrMode e;
    PXIFifoCallback cb;
    while (1) {
        if (REG_IPCFIFOCNT & 0x4000) {
            REG_IPCFIFOCNT |= 0xc000;
            result = -3;
        } else {
            e = OS_DisableInterrupts();
            if (REG_IPCFIFOCNT & 0x100) {
                OS_RestoreInterrupts(e);
                result = -4;
            } else {
                msg.raw = REG_IPCFIFORECV;
                OS_RestoreInterrupts(e);
                result = 0;
            }
        }
        if (result == -4) {
            return;
        }
        if (result == -3) {
            continue;
        }
        if (msg.bits.tag == 0) {
            continue;
        }
        cb = pxiCallback[msg.bits.tag];
        if (cb != NULL) {
            cb(msg.bits.tag, msg.bits.data, msg.bits.err);
            continue;
        }
        if (msg.bits.err) {
            continue;
        }
        msg.bits.err = 1;
        if (REG_IPCFIFOCNT & 0x4000) {
            REG_IPCFIFOCNT |= 0xc000;
            continue;
        }
        e = OS_DisableInterrupts();
        if (REG_IPCFIFOCNT & 2) {
            OS_RestoreInterrupts(e);
            continue;
        }
        REG_IPCFIFOSEND = msg.raw;
        OS_RestoreInterrupts(e);
    }
}
