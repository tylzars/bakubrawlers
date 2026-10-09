#ifndef SDK_TP_H
#define SDK_TP_H
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef short s16; typedef int s32; typedef long long s64; typedef unsigned long long u64;
typedef int BOOL;
typedef volatile u8 vu8; typedef volatile u16 vu16; typedef volatile u32 vu32; typedef volatile u64 vu64;
#define NULL ((void*)0)
#define TRUE 1
#define FALSE 0

// OS / PXI (earlier ranges)
u32  OS_DisableInterrupts(void);                 // 0x0200710c
u32  OS_RestoreInterrupts(u32 state);            // 0x02007120
void OS_Terminate(void);                         // 0x02007520 (called as panic path)
void PXI_Init(void);                             // 0x02009050
void PXI_SetFifoRecvCallback(u32 tag, void (*cb)(u32 tag, u32 data, int err)); // 0x0200915c
BOOL PXI_IsCallbackReady(u32 tag, u32 proc);     // 0x020091a8
int  PXI_SendWordByFifo(u32 tag, u32 data, int err); // 0x020091cc

// Touch panel
typedef struct { u16 x, y, touch, validity; } TPData;
typedef struct { u32 x:12; u32 y:12; u32 touch:1; u32 validity:2; u32 pad:5; } TPRawBits;
typedef struct {
    u16 initialized;      // 0x00
    u16 pad02;
    void (*callback)(u32 cmd, u32 arg, u32 arg2); // 0x04
    TPData raw;           // 0x08
    u16 index;            // 0x10
    u16 pad12;
    TPData *buf;          // 0x14
    u16 bufSize;          // 0x18
    u16 pad1a;
    struct { s32 x0, dx, xs, y0, dy, ys; } cal; // 0x1c
    u16 calibrate;        // 0x34
    u16 state;            // 0x36
    u16 err;              // 0x38
    u16 busy;             // 0x3a
} TPWork;
extern TPWork tpWork;     // data_020bfd10

#define REG_DIVCNT   (*(vu16*)0x04000280)
#define REG_DIV_NUMER (*(vu32*)0x04000290)
#define REG_DIV_DENOM (*(vu32*)0x04000298)
#define REG_DIV_DENOM_H (*(vu32*)0x0400029c)
#define REG_DIV_RESULT (*(vu32*)0x040002a0)


// ---- MIC
typedef void (*MICCallback)(u32 type, void *arg);
typedef struct {
    u16 init; u16 pad0;  // 0x00
    vu32 busy;            // 0x04
    void (*cb)(u32 cmd, void *arg);  // 0x08
    void *cbArg;         // 0x0c
    u32 result;          // 0x10
    void (*fullCb)(u32 a, void *arg); // 0x14
    void *fullArg;       // 0x18
    u16 *lastAddr;       // 0x1c
} MICWork;
typedef struct {
    u32 type;            // 0x00
    void *buffer;        // 0x04
    u32 size;            // 0x08
    u32 rate;            // 0x0c
    u32 loop;            // 0x10
    void (*full)(u32, void *); // 0x14
    void *fullArg;       // 0x18
} MICAutoParam;

extern MICWork micWork;  // data_020bfd4c

// ---- PM
typedef struct PMCallbackInfo PMCallbackInfo;
struct PMCallbackInfo { void (*func)(void *arg); void *arg; PMCallbackInfo *next; };
typedef struct {
    u16 init;            // 0x00
    u32 vcount4;         // 0x04
    u32 vcount8;         // 0x08
    u32 flag;            // 0x0c
    u32 field10;         // 0x10
    PMCallbackInfo *postList; // 0x14
    PMCallbackInfo *preList;  // 0x18
    u32 lock;            // 0x1c
    void (*cb)(u32 result, void *arg); // 0x20
    void *cbArg;         // 0x24
    u16 *resultPtr;      // 0x28
} PMWork;
extern PMWork pmWork;    // data_020bfd6c
extern void (*pmWaitFunc)(void); // data_020a5068

u32 OS_GetCpsrIrqBits(void);  // 0x02007164 (returns CPSR I-bit mask)
u32 OS_GetProcMode(void);     // 0x02007170
void OS_SpinWait(u32 cycles); // 0x0200717c
void PXI_ProcessRecvFifo(void); // 0x02009254
void SVC_WaitByLoop(u32 n);   // WaitByLoop
#endif
typedef struct { s16 x0, y0, xDotSize, yDotSize; } TPCalibrateParam;
