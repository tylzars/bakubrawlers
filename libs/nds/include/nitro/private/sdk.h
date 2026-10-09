#pragma once
#include "types.h"

#define REG_IME (*(vu16 *) 0x4000208)
#define REG_IE  (*(vu32 *) 0x4000210)
#define REG_IF  (*(vu32 *) 0x4000214)
#define REG_EXMEMCNT (*(vu16 *) 0x4000204)
extern char SDK_AUTOLOAD_DTCM_START[];
#define HW_DTCM ((u32) SDK_AUTOLOAD_DTCM_START)
extern char SDK_IRQ_STACKSIZE[];
extern char SDK_SYS_STACKSIZE[];

typedef u32 OSIntrMode;
typedef struct {
    u32 lockFlag;
    u16 ownerID;
    u16 extension;
} OSLockWord;

// asm functions
extern OSIntrMode OS_DisableInterrupts(void);   // 0x0200710c
extern OSIntrMode OS_RestoreInterrupts(OSIntrMode); // 0x02007120
extern OSIntrMode OS_DisableInterrupts_IrqAndFiq(void); // 0x02007138
extern OSIntrMode OS_RestoreInterrupts_IrqAndFiq(OSIntrMode); // 0x0200714c
extern OSIntrMode OS_EnableInterrupts(void);
extern void WaitByLoop(u32 count); // BIOS thunk
extern void OS_SpinWait(u32 cycles); // 0x0200717c
extern u32 MI_SwapWord(u32 setData, vu32 *destp); // 0x02007cd0
