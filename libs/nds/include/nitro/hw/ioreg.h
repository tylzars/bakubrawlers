#pragma once

#include "types.h"

// ARM9 I/O registers used so far. Main engine at 0x04000000, sub engine (DB) at 0x04001000.
#define REG_ADDR_GX_DISPCNT     0x04000000
#define REG_ADDR_G2_BG0CNT      0x04000008
#define REG_ADDR_G2_BG0OFS      0x04000010
#define REG_ADDR_G2_WINOUT      0x0400004a
#define REG_ADDR_G2_BLDCNT      0x04000050
#define REG_ADDR_GX_POWCNT      0x04000304
#define REG_ADDR_GXS_DB_DISPCNT 0x04001000
#define REG_ADDR_G2S_DB_BG0CNT  0x04001008
#define REG_ADDR_G2S_DB_BG1CNT  0x0400100a
#define REG_ADDR_G2S_DB_BG0OFS  0x04001010
#define REG_ADDR_G2S_DB_BG1OFS  0x04001014
#define REG_ADDR_G2S_DB_WINOUT  0x0400104a
#define REG_ADDR_G2S_DB_BLDCNT  0x04001050

#define reg_GX_DISPCNT     (*(vu32 *) REG_ADDR_GX_DISPCNT)
#define reg_G2_BG0CNT      (*(vu16 *) REG_ADDR_G2_BG0CNT)
#define reg_G2_BG0OFS      (*(vu32 *) REG_ADDR_G2_BG0OFS)
#define reg_G2_WINOUT      (*(vu16 *) REG_ADDR_G2_WINOUT)
#define reg_G2_BLDCNT      (*(vu16 *) REG_ADDR_G2_BLDCNT)
#define reg_GX_POWCNT      (*(vu16 *) REG_ADDR_GX_POWCNT)
#define reg_GXS_DB_DISPCNT (*(vu32 *) REG_ADDR_GXS_DB_DISPCNT)
#define reg_G2S_DB_BG0CNT  (*(vu16 *) REG_ADDR_G2S_DB_BG0CNT)
#define reg_G2S_DB_BG1CNT  (*(vu16 *) REG_ADDR_G2S_DB_BG1CNT)
#define reg_G2S_DB_BG0OFS  (*(vu32 *) REG_ADDR_G2S_DB_BG0OFS)
#define reg_G2S_DB_BG1OFS  (*(vu32 *) REG_ADDR_G2S_DB_BG1OFS)
#define reg_G2S_DB_WINOUT  (*(vu16 *) REG_ADDR_G2S_DB_WINOUT)
#define reg_G2S_DB_BLDCNT  (*(vu16 *) REG_ADDR_G2S_DB_BLDCNT)
