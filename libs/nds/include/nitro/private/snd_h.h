#pragma once
#include <nitro/private/os_types.h>

typedef struct SNDCommand SNDCommand;
struct SNDCommand {
    SNDCommand *next;   // 0x00
    u32 id;             // 0x04
    u32 arg[4];         // 0x08
};                      // 0x18

extern void DC_FlushRange(void *ptr, u32 size);   // 0x02005e9c
extern void DC_StoreRange(void *ptr, u32 size);   // 0x02005e80
extern void DC_InvalidateRange(void *ptr, u32 size);   // 0x02005e64
extern void OS_SpinWait(u32 cycles);              // 0x0200717c
extern void PXI_SetFifoRecvCallback(int fifoTag, void (*callback)(int tag, u32 data, BOOL err));   // 0x0200915c
extern BOOL PXI_IsCallbackReady(int fifoTag, int proc);               // 0x020091a8
extern int PXI_SendWordByFifo(int fifoTag, u32 data, BOOL err);       // 0x020091cc

// snd_cmdwrap / snd_command
extern void func_020082a4(u32 id, u32 a, u32 b, u32 c, u32 d);
extern void func_02008254(u32 a, u32 b, u32 c, u32 d);
extern void func_0200827c(u32 a, u32 b, u32 c, u32 d, u32 e);
extern SNDCommand *SND_AllocCommand(u32 flags);
extern void SND_PushCommand(SNDCommand *cmd);
extern BOOL SND_FlushCommand(u32 flags);
extern SNDCommand *SND_RecvCommandReply(u32 flags);
extern void SND_WaitForCommandProc(u32 tag);
extern BOOL SND_IsFinishedCommandTag(u32 tag);
extern u32 SND_CountFreeCommand(void);
extern u32 SND_CountWaitingCommand(void);
extern u32 func_02008908(void);
extern void SND_CommandInit(void);
extern void SND_LockMutex(void);
extern void SND_UnlockMutex(void);
extern void func_02008948(void);
extern void func_020089a8(void);
extern SNDCommand *func_020089d0(void);
extern BOOL func_02008a18(void);
extern void func_02008a58(void);
extern void func_02008a88(int idx);
extern u32 func_02008aa8(int idx, void (*cb)(void *), void *arg);
extern void func_02008ad8(u32 data);
extern u32 func_02008b1c(void);
extern u32 func_02008b48(void);
extern void func_02008b70(void *work);
extern u32 func_02005a60(void);
extern void OS_InitMutex(OSMutex *mutex);
extern void OS_LockMutex(OSMutex *mutex);
extern void OS_UnlockMutex(OSMutex *mutex);
extern void func_02008924(int tag, u32 data, BOOL err);
