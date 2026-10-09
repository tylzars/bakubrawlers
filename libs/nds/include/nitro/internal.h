#pragma once
// Declarations shared by the SDK library units in libs/nds/src that the public headers don't have yet.
#include "types.h"
#include <nitro/gx.h>


#define REG_VRAMCNT(n) ((vu8 *) (0x04000240 + (n)))
#define reg_GX_VRAMCNT_A (*(vu8 *) 0x04000240)
#define reg_GX_VRAMCNT_B (*(vu8 *) 0x04000241)
#define reg_GX_VRAMCNT_C (*(vu8 *) 0x04000242)
#define reg_GX_VRAMCNT_D (*(vu8 *) 0x04000243)
#define reg_GX_VRAMCNT_E (*(vu8 *) 0x04000244)
#define reg_GX_VRAMCNT_F (*(vu8 *) 0x04000245)
#define reg_GX_VRAMCNT_G (*(vu8 *) 0x04000246)
#define reg_GX_VRAMCNT_H (*(vu8 *) 0x04000248)
#define reg_GX_VRAMCNT_I (*(vu8 *) 0x04000249)
#define reg_GXS_DISPCNT (*(vu32 *) 0x04001000)
#define reg_GX_DISPCNT_ (*(vu32 *) 0x04000000)

extern u16 data_020bdc08[]; // VRAM bank assignment state: [0] unassigned banks, then one mask per use
extern u16 data_020bdc04[];
extern u16 data_020bdc06[];

extern s32 data_020a4f24; // GX DMA channel, -1 when unused
void func_02001fb8(int banks);
void GX_InitGXState(void);
void func_02007498(u16 banks, u16 prev);

#define HW_BG_VRAM 0x06000000
#define HW_DB_BG_VRAM 0x06200000
#define reg_GX_DISPSTAT (*(vu16 *) 0x04000004)
#define reg_GX_DISP3DCNT (*(vu16 *) 0x04000060)
u32 func_02004f54(void);
void func_02007520(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_0200754c(u32 dmaNo, void *dest, u32 data, u32 size);
#define reg_G3X_GXFIFO (*(vu32 *) 0x04000400)
#define reg_G3X_GXSTAT (*(vu32 *) 0x04000600)
void func_020047d8(const void *src, volatile void *dst); // MI_Copy48B
void func_020047fc(const void *src, volatile void *dst); // MI_Copy128B
void MI_Copy36B(const void *src, volatile void *dst);
#define reg_G3X_END_VTXS (*(vu32 *) 0x04000504)
#define reg_G3X_CLEAR_COLOR (*(vu32 *) 0x04000350)
void G3X_ClearFifo(void);
void G3X_InitMtxStack(void);
void G3X_InitTable(void);
#define reg_G3X_MTX_MODE (*(vu32 *) 0x04000440)
#define reg_G3X_MTX_POP (*(vu32 *) 0x04000448)
#define reg_G3X_MTX_IDENTITY (*(vu32 *) 0x04000454)
int G3X_GetMtxStackLevelPV(u32 *level);
int G3X_GetMtxStackLevelPJ(u32 *level);
void func_020076a8(u32 dmaNo, void *dest, u32 data, u32 size, void *callback, void *arg);
void func_01ff9604(const void *src, void *dst); // MI_Copy64B
void func_01ff94e0(const void *src, void *dst, u32 size); // MIi_CpuCopy16
void func_01ff9510(const void *src, void *dst, u32 size); // MIi_CpuCopy32
void func_0200763c(u32 dmaNo, const void *src, void *dest, u32 size); // MI_DmaCopy16
void func_020075cc(u32 dmaNo, const void *src, void *dest, u32 size); // MI_DmaCopy32
void func_0200776c(u32 dmaNo, const void *src, void *dest, u32 size, void *callback, void *arg); // MI_DmaCopy32Async
void func_02007804(u32 dmaNo); // MI_WaitDma
void GX_SetBankForBGExtPltt(int); void GX_SetBankForOBJExtPltt(int); void GX_SetBankForSubBGExtPltt(int); void GX_SetBankForSubOBJExtPltt(int); void GX_SetBankForTex(int); void GX_SetBankForTexPltt(int);
