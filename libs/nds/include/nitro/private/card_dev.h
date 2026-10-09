#pragma once
/* Local definitions for the CARD and WM units (not part of the public headers). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef volatile unsigned int vu32;
typedef volatile unsigned short vu16;
typedef int BOOL;
#define NULL ((void *)0)

/* Command/argument block shared with the ARM7 (data_020bfe00, 0x60 bytes) */
typedef struct CardArg {
    u32 result;   // 0x00
    u32 id;       // 0x04
    u32 f08, f0c, f10, f14;
    u32 f18, f1c, f20, f24, f28, f2c;
    u32 f30, f34, f38, f3c, f40, f44, f48, f4c, f50;
    u8 f54;
    u8 pad55[3];
    u32 f58;
    u32 f5c;
} CardArg;

typedef struct { u8 b[0xc0]; } CardThread;
typedef struct { u32 a, b; } CardQueue;
/* data_020bfe60 */
typedef struct CardCommon {
    CardArg *arg;     // 0x00
    u32 f04;          // last word sent
    u32 owner;        // 0x08 lock owner (-3 = free)
    u32 lockCount;    // 0x0c
    CardQueue lockQueue; // 0x10
    u32 lockTarget;   // 0x18
    u32 src;          // 0x1c
    u32 dst;          // 0x20
    u32 len;          // 0x24
    u32 dma;          // 0x28
    u32 cmd;          // 0x2c
    u32 retry;        // 0x30
    u32 mode;         // 0x34
    void (*callback)(void *); // 0x38
    void *cbArg;      // 0x3c
    void (*task)(struct CardCommon *); // 0x40
    CardThread thread; // 0x44
    void *curThread;  // 0x104
    u32 priority;     // 0x108
    CardQueue queue;     // 0x10c
    vu32 f114;         // flags
    u32 f118, f11c;
    u8 buf[0x200];    // 0x120
} CardCommon;
#define CF_READY 1
#define CF_PXI 2
#define CF_BUSY 4
#define CF_TASK 8
#define CF_TASKWAIT 0x10
#define CF_PXIWAIT 0x20
#define CF_CANCEL 0x40

/* data_020c04a0: ROM read request (page cache) */
typedef struct CardRomReq {
    void (*func)(struct CardRomReq *); // 0
    u32 ctrl;   // 4
    u32 page;   // 8
    u8 pad[0x14];
    u8 cache[0x200]; // 0x20
} CardRomReq;

typedef struct { u32 base; u8 pad[0x1c]; void (*romfn)(CardRomReq *); } CardRomInfo; /* data_020c0480 */

typedef struct { u32 pad; void *curThread; } OSThreadInfo; /* data_020bdce8 */
typedef struct { u8 pad[0x60]; u32 romctrl; } CardCfg;      /* *data_020a506c */

/* data_020c06c0 */
typedef struct { BOOL pulledOut; BOOL (*callback)(void); } CardPullout;

/* WM work buffer */
typedef struct WMWork {
    u32 p00, p04, p08, p0c, p10;
    u16 dma;   // 0x14
    u16 f16;
    u32 cb[45]; // 0x18
    u32 a[16];  // 0xcc
    u32 b[16];  // 0x10c
    u32 f14c;
    u16 f150;
} WMWork;
typedef struct { u16 init; u16 pad; WMWork *work; } WMState; /* data_020c06e0 */

extern CardArg data_020bfe00;
extern CardRomInfo data_020c0480;
extern OSThreadInfo data_020bdce8;
extern CardCfg *data_020a506c;
extern u32 data_020bfde0;
extern u8 data_020c0480_end[];

/* OS / MI externs (addresses in comments) */
u32 OS_DisableInterrupts(void);                 // 0x0200710c
void OS_RestoreInterrupts(u32);                 // 0x02007120
void OS_SleepThread(void *queue);               // 0x02005610
void OS_WakeupThread(void *queue);              // 0x02005660
void OS_WakeupThreadDirect(void *thread);       // 0x020056c8
void OS_SetThreadPriority(void *thread, u32 p); // 0x02005734
void OS_CreateThread(void *t, void *f, void *arg, void *stack, u32 size, u32 prio); // 0x02005404
void OS_Terminate(void);                        // 0x02007520
void OS_SpinWait(u32);                          // 0x0200717c
void func_02000b9c(const void *);               // 0x02000b9c stub taking a string
void MI_CpuCopy8(const void *src, void *dst, u32 n); // 0x01ff96c4
void MI_CpuFill8(void *dst, u8 v, u32 n);
void MIi_CpuClearFast(u32 v, void *dst, u32 n);
static inline void MI_CpuClearFast(void *d, u32 n) { MIi_CpuClearFast(0, d, n); }
void DC_InvalidateRange(void *p, u32 n);        // 0x02005e64
void DC_StoreRange2(void *p, u32 n);            // 0x02005e80
void DC_StoreRange(void *p, u32 n);             // 0x02005e9c
void DC_WaitWriteBufferEmpty(void);             // 0x02005ec0
void DC_FlushAll(void);                         // 0x02005ecc
void DC_FlushRange(void *p, u32 n);             // 0x02005ed8
void DC_InvalidateAll(void);                    // 0x02005e30
void PXI_Init(void);                            // 0x02009050
void PXI_SetCallback(u32 tag, void *cb);        // 0x0200915c
int PXI_IsCallbackReady(u32 tag, int);          // 0x020091a8
int PXI_SendWord(u32 tag, u32 data, int err);   // 0x020091cc
void OS_InitMessageQueue(void *q, void *buf, u32 n); // 0x02005a80
int OS_SendMessage(void *q, void *msg, int flag);    // 0x02005aa8
int OS_ReceiveMessage(void *q, void *msg, int flag); // 0x02005b3c
int OS_JamMessage(void *q, void *msg, int flag);     // 0x02005be0
void func_02007cd8(u32 dma, u32 src, u32 dst, u32 n); // DMA recv
void func_0200786c(u32 dma);                    // stop DMA
void func_02004bc0(u32);  void func_02004bf0(u32); void func_02004b90(u32);
void func_020049c0(u32, void *);
void func_02004ee4(u32); void func_02004f00(u32);
void *func_0200682c(void);
void func_0200754c(u32 dma, void *dst, u32 v, u32 n);
void WaitByLoop(u32);
u32 func_0200d2b4(void);
void func_020101a8(void);
