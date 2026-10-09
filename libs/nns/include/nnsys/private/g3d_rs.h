// Declarations for the NitroSystem G3d SBC renderer in ITCM (render state "Rs", render object "RObj").
#include <nnsys/private/nns_g3d.h>
typedef long long s64;
typedef unsigned long long u64;
typedef volatile u32 vu32;
typedef volatile u16 vu16;
typedef volatile u8 vu8;

typedef struct Rs Rs;
typedef struct RObj RObj;
typedef void (*SbcFn)(Rs *rs, u32 op);

// Render object (caller-owned).
struct RObj {
    u32 flag;                  // 0x00: bit0 = init pending, bit1..3 = option flags
    u8 *mdl;                   // 0x04: model block
    u32 ofs08;                 // placeholder for the three bit-list heads below
    u32 pad0c;
    u32 ofs10;
    u32 pad14;
    u32 ofs18;
    u8 pad1c[0x14];
    u8 *sbcOverride;           // 0x30
};

struct Rs {
    u8 *cmd;                   // 0x00 current SBC command
    RObj *obj;                 // 0x04
    u32 flag;                  // 0x08
    void *slot[32];            // 0x0c
    u8 slotId[32];             // 0x8c
    u8 curNode;                // 0xac
    u8 curMat;                 // 0xad
    u8 curShp;                 // 0xae
    u8 pad_af;
    void *pMat;                // 0xb0
    void *pShp;                // 0xb4
    s32 *pVis;                 // 0xb8
    u32 matDone[2];            // 0xbc
    u32 one;                   // 0xc4
    u32 padc8;
    u32 shpDone[2];            // 0xcc
    Dict *dNode;               // 0xd4
    ResMat *dMat;              // 0xd8
    Dict *dShp;                // 0xdc
    s32 mdlScale;              // 0xe0
    s32 mdlScaleInv;           // 0xe4
    void *cbE8;                // 0xe8
    void *cbEc;                // 0xec
    void *cbF0;                // 0xf0
    u8 matStore[0x38];         // 0xf4
    u8 shpStore[0x58];         // 0x12c
    s32 visStore;              // 0x184
};
extern Rs *data_020baf00;
extern void MIi_CpuClearFast(u32 data, void *dst, u32 size);
extern void MIi_CpuClear32(u32 data, void *dst, u32 size);
extern void func_01ff93d8(u32 cmd, const void *params, u32 count);
extern void func_01ff9328(const void *data, u32 size);

// Material data in the model resource (parts used by the SBC renderer).
typedef struct MatRes {
    u32 w0;
    u32 diffAmb;       // 0x04
    u32 specEmi;       // 0x08
    u32 polyAttr;      // 0x0c
    u32 polyMask;      // 0x10
    u32 texParam;      // 0x14
    u16 pad18, pad1a;
    u16 plttBase;      // 0x1c
    u16 flag;          // 0x1e
    u16 texW, texH;  // 0x20, 0x22
    s32 scaleW, scaleH;    // 0x24, 0x28
    s32 texMtx[1];     // 0x2c...
} MatRes;

// Per-material render storage (0x38 bytes).
typedef struct MatSt {
    u32 flag;          // 0x00
    u32 diffAmb;       // 0x04
    u32 specEmi;       // 0x08
    u32 polyAttr;      // 0x0c
    u32 texParam;      // 0x10
    u32 plttBase;      // 0x14
    s32 scaleS, scaleT;   // 0x18, 0x1c
    s16 rotSin, rotCos;   // 0x20, 0x22
    s32 transS, transT;   // 0x24, 0x28
    u16 texW, texH;     // 0x2c, 0x2e
    s32 scaleW, scaleH;       // 0x30, 0x34
} MatSt;

// Per-node render storage (0x58 bytes).
typedef struct ShpSt {
    u32 flag;          // 0x00: bit0 = no rotation, bit1 = no translation? (see code)
    u8 pad04[0x24];
    s32 mtx[9];        // 0x28
    s32 trans[3];      // 0x4c
} ShpSt;
typedef struct NodeRes {
    u16 flag;          // 0x00
    s16 hw02;          // 0x02
    s32 data[1];       // 0x04...
} NodeRes;
extern void func_01ff97f4(void *mtx33);   // MTX_Identity33 (thumb)
