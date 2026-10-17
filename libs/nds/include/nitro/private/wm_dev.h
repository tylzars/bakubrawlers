#ifndef WM_DEV_H
#define WM_DEV_H
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed short s16; typedef signed int s32; typedef int BOOL;

typedef void (*WMCb)();

/* shared status block (0x800 bytes read from ARM7) */
typedef struct WMStatus {
    u16 h00;
    u8 pad02[0xa];
    u32 w0c;
    u8 pad10[0x2c];
    u16 h3c;
    u16 h3e;
    u8 pad40[0x32];
    u16 h72;
    u8 pad74[0x8];
    u32 w7c;
    u8 pad80[6];
    u16 h86;
    u8 pad88[0x14];
    u16 h9c;
    u8 pad9e[0x1e];
    u16 hbc;
    u8 padbe[0x8];
    u16 hc6;
    u8 padc8[0x30];
    u16 hf8;
    u8 padfa[0x88];
    u16 h182;
    u8 pad184[0x2];
    u16 h186;
    u16 h188;
    u8 pad18a[0x6];
} WMStatus;

/* ARM9 work area (pointed to by WMDev.work) */
typedef struct WMWork {
    u32 f00;
    WMStatus *st;          /* 0x04 */
    u8 pad08[8];
    void *recv;            /* 0x10 receive buffer (0x100 bytes) */
    u16 pad14;
    u16 h16;
    WMCb cbTable[0x2c];    /* 0x18 per-command callbacks */
    WMCb cbInd;            /* 0xc8 */
    WMCb indCb[16];        /* 0xcc */
    u32 indArg[16];        /* 0x10c */
    u32 bitmap;            /* 0x14c */
    u16 h150;
} WMWork;

/* indication / callback argument block (0x44 bytes) */
typedef struct WMInd {
    u16 id;     /* 0x00 */
    u16 h2;
    u16 h4;
    u16 h6;
    u32 w8;
    u32 wc;
    u16 h10;
    u16 h12;
    u8 mac[6];  /* 0x14 */
    u16 h1a;
    u32 w1c;
    u16 h20;
    u16 h22;
    u8 data[0x18]; /* 0x24 */
    u16 h3c;
    u16 pad3e;
    u16 h40;
    u16 h42;
} WMInd;

/* ARM9 global (data_020c06e0) */
typedef struct WMDev {
    u16 h00;
    u16 pad02;
    WMWork *work;          /* 0x04 */
    u8 pad08[0x48];
    WMInd ind;             /* 0x50 */
} WMDev;
extern WMDev data_020c06e0;

typedef struct { u16 h0; u8 pad2[3]; struct { u8 f0 : 1; u8 f1 : 1; } b; u16 pad6; u32 w8; } CtrdgInfo;

typedef struct { u8 pad[5]; u8 f0 : 1; u8 f1 : 1; } CtrdgBits;

extern void DC_InvalidateRange(void *p, u32 n);   /* 0x02005e64 */
extern void DC_StoreRange(void *p, u32 n);        /* 0x02005e80 */
extern void MI_CpuCopy8(const void *s, void *d, u32 n);   /* 0x01ff96c4 */
extern void MIi_CpuCopy16(const void *s, void *d, u32 n);  /* 0x01ff94e0 */
extern void MIi_CpuCopy32(const void *s, void *d, u32 n);  /* 0x01ff9510 */
extern void MI_CpuFill8(void *d, u8 v, u32 n);            /* 0x01ff9630 */
extern void MIi_CpuClear16(u16 v, void *d, u32 n);        /* 0x01ff94c8 */
extern void MIi_CpuClear32(u32 v, void *d, u32 n);        /* 0x01ff94fc */
extern void MIi_CpuClearFast(u32 v, void *d, u32 n);      /* 0x01ff9540 */
extern u32 OS_DisableInterrupts(void);                    /* 0x0200710c */
extern void OS_RestoreInterrupts(u32 e);                   /* 0x02007120 */
extern void CpuSet(void *src, void *dst, u32 ctl);
extern void OS_Terminate(void);
extern void OS_GetMacAddress(void *p);
extern u16 OS_GetLockID(void);
extern void func_0200faf8(void);
extern WMWork *func_0200fcf4(void);
extern int func_0200fd04(void);
extern int func_0200fd20(void);
extern int func_0200fd68(int n, ...);
extern int func_0200f8e8(int a, int b);
extern void func_0200fb68(int cmd, WMCb cb);
extern int func_0200fbd8(int cmd, int n, ...);
extern int func_0200fc84(void *p, u32 n);
extern u32 CountSetBits(u32 a);
extern int RTC_Init(void);
extern int RTC_GetDate(u32 *p);
extern void func_02011c34(void *p);
extern void func_02011cb0(u16 a, void *p);
extern void func_02011c7c(void *p);
extern void func_02011d0c(u16 a, void *p);

extern void func_020101a8(void);
extern u16 func_020101c4(void);
extern u16 func_020101f4(void);
extern int func_02010224(u32 idx, WMCb cb, u32 arg);
extern int func_020102fc(void);
extern int func_02010368(void);
extern void func_02011450(u16 *m);
extern void func_02011528(WMInd *m);
extern void func_02011654(WMInd *m);
extern int func_02010d3c(WMCb cb, void *a, void *b, u32 size, u16 x, u16 y, u16 z);
extern void func_02011748(u8 *buf, u32 idx, void *src);
extern void func_02011814(u8 *buf, int flag);
extern u8 *func_020119bc(u8 *buf, u32 mask, u8 *base, u32 n);
extern u8 *func_02011968(u8 *buf, u16 *p, u32 n);
extern int func_02011b10(void);
extern int func_02011b28(void);
extern int func_02010c84(WMCb cb, void *p);
extern int func_02010aac(WMCb cb, u32 a, int b, u32 c, u16 e, void *f);
extern int func_02010888(WMCb cb, int x);
extern int func_02010684(WMCb cb, int b, int c, int d);
extern u8 data_020c1184[];
#endif
