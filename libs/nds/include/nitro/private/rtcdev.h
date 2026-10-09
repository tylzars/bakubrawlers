#pragma once
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
typedef volatile u16 vu16; typedef volatile u32 vu32;
typedef int BOOL;
#define NULL ((void *)0)
int func_02011ae8(void);
u32 OS_DisableInterrupts(void);          // 0x0200710c
void OS_RestoreInterrupts(u32 mode);     // 0x02007120
void OS_SleepThread(void *queue);        // 0x02005610
void SVC_WaitByLoop(u32 count);          // 0x0200717c
void PXI_Init(void);                // PXI init
BOOL PXI_IsCallbackReady(u32 tag, BOOL port);  // PXI start-up handshake
void PXI_SetFifoRecvCallback(u32 tag, void *cb);   // PXI set callback
int PXI_SendWordByFifo(u32 tag, u32 data, BOOL err); // PXI send
void MI_CpuFill8(void *dst, u8 data, u32 size);
u32 OS_ReadOwnerOfLockWord(void *p);              // OS: read lock word
int OS_TryLockCartridge(u16 id);               // OS_TryLockCartridge-like
void OS_UnLockCartridge(u16 id);              // OS_UnlockCartridge-like
void func_02006860(u32 a, u32 b);        // OS: set cartridge/exmem wait
void OS_CreateThread(void *thread, void *func, void *arg, void *stackTop, u32 stackSize, u32 prio); // OS_CreateThread
void OS_WakeupThreadDirect(void *thread);        // OS_WakeupThreadDirect / resume
void OS_ExitThread(void);                // OS_ExitThread
void func_02005e64(void *p, u32 size);   // DC_InvalidateRange
void func_02005e30(void);                // DC_WaitWriteBufferEmpty
int MI_DmaCopy16(int a, u32 b, void *c, u32 d); // DMA
u32 OS_SetIrqMask(u32 mask);             // OS_SetIrqMask-like
void OS_Terminate(void);                // OS_Terminate
void MI_CpuCopy8(const void *src, void *dst, u32 size);
