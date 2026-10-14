#include <nitro/private/rtcdev.h>

typedef struct RTCDate { s32 year, month, day, week; } RTCDate;
typedef struct RTCTime { s32 hour, minute, second; } RTCTime;
typedef struct RTCDateTime { RTCDate date; RTCTime time; } RTCDateTime;
typedef void (*RTCCallback)(u32 result, void *arg);

typedef struct RTCWork {
    u16 init;        // 0x00
    u16 pad;
    u32 f04, f08;
    u32 busy;        // 0x0c
    RTCCallback cb;  // 0x10
    void *p1;        // 0x14 date/time out
    void *p2;        // 0x18 time out (date+time commands)
    void *arg;       // 0x1c
    u32 command;     // 0x20
    u32 index;       // 0x24
    void (*alarmCb)(void); // 0x28
    u32 result;      // 0x2c
} RTCWork;

extern RTCWork data_020bfd98;
extern vu32 data_020bfda4;
typedef struct { u32 year : 8; u32 month : 5; u32 : 3; u32 day : 6; u32 : 10; } RTCRawDate;
typedef struct { u32 hour : 6; u32 : 2; u32 minute : 7; u32 : 1; u32 second : 7; u32 : 9; } RTCRawTime;
typedef struct { u16 mode : 4; u16 : 2; u16 flag : 1; u16 : 9; } RTCRawStat;
typedef struct { u32 week : 3; u32 : 4; u32 b7 : 1; u32 hour : 6; u32 : 1; u32 b15 : 1; u32 minute : 7; u32 b23 : 1; u32 : 8; } RTCRawAlarm;
#define RAW_DATE ((RTCRawDate *)0x027ffde8)
#define RAW_TIME ((RTCRawTime *)0x027ffdec)
#define RAW_STAT ((RTCRawStat *)0x027ffdea)
#define RAW_ALARM ((RTCRawAlarm *)0x027ffdec)

u32 func_0200e174(u32 bcd);
u32 func_0200e254(RTCDate *date);
BOOL func_0200e20c(void);
BOOL func_0200e21c(void);
void func_0200e1e4(u32 result, void *arg);
void func_0200e1f4(void);
void func_0200dc24(u32 tag, u32 data, u32 err);
extern int func_0200d1e0(int a, int b);
extern int func_0200d924(void);
extern int func_0200d904(int a);

void func_0200da80(void)
{
    SVC_WaitByLoop(0x360000);
    if (func_0200d924() == 1) return;
    while (func_0200d1e0(2, 0)) {
        SVC_WaitByLoop(0xa3a47);
    }
    while (!func_0200d904(1)) {
        SVC_WaitByLoop(10);
    }
}

void func_0200db0c(void)
{
    if (data_020bfd98.init != 0) return;
    data_020bfd98.init = 1;
    data_020bfd98.busy = 0;
    data_020bfd98.cb = 0;
    data_020bfd98.alarmCb = 0;
    data_020bfd98.p1 = 0;
    data_020bfd98.p2 = 0;
    PXI_Init();
    while (!PXI_IsCallbackReady(5, 1)) {}
    PXI_SetFifoRecvCallback(5, func_0200dc24);
}

int func_0200db78(RTCDate *date, RTCCallback cb, void *arg)
{
    u32 e = OS_DisableInterrupts();
    if (data_020bfd98.busy) {
        OS_RestoreInterrupts(e);
        return 1;
    }
    data_020bfd98.busy = 1;
    OS_RestoreInterrupts(e);
    data_020bfd98.command = 1;
    data_020bfd98.index = 0;
    data_020bfd98.p1 = date;
    data_020bfd98.cb = cb;
    data_020bfd98.arg = arg;
    return func_0200e20c() ? 0 : 3;
}

u32 func_0200dbec(RTCDate *date)
{
    u32 r = func_0200db78(date, (RTCCallback)func_0200e1e4, 0);
    data_020bfd98.result = r;
    if (r == 0) func_0200e1f4();
    return data_020bfd98.result;
}

void func_0200dc24(u32 tag, u32 data, u32 err)
{
    u32 res;
    u8 cmd;
    u32 e2;
    if (err) {
        if (data_020bfd98.index) data_020bfd98.index = 0;
        if (data_020bfd98.busy) data_020bfd98.busy = 0;
        if (data_020bfd98.cb) {
            RTCCallback cb = data_020bfd98.cb;
            data_020bfd98.cb = 0;
            cb(6, data_020bfd98.arg);
        }
        return;
    }
    cmd = (data & 0x7f00) >> 8;
    err = data & 0xff;
    if (cmd == 0x30) {
        if (data_020bfd98.alarmCb) data_020bfd98.alarmCb();
        return;
    }
    res = 0;
    if (err == 0) {
        switch (data_020bfd98.command) {
        case 0: {
            RTCDate *d = (RTCDate *)data_020bfd98.p1;
            d->year = func_0200e174(RAW_DATE->year);
            d->month = func_0200e174(RAW_DATE->month);
            d->day = func_0200e174(RAW_DATE->day);
            d->week = func_0200e254(d);
            break;
        }
        case 1: {
            RTCTime *t = (RTCTime *)data_020bfd98.p1;
            t->hour = func_0200e174(RAW_TIME->hour);
            t->minute = func_0200e174(RAW_TIME->minute);
            t->second = func_0200e174(RAW_TIME->second);
            break;
        }
        case 2: {
            RTCDate *d = (RTCDate *)data_020bfd98.p1;
            RTCTime *t;
            d->year = func_0200e174((u8)*(vu32 *)0x027ffde8);
            d->month = func_0200e174(RAW_DATE->month);
            d->day = func_0200e174(RAW_DATE->day);
            d->week = func_0200e254(d);
            t = (RTCTime *)data_020bfd98.p2;
            t->hour = func_0200e174(RAW_TIME->hour);
            t->minute = func_0200e174(RAW_TIME->minute);
            t->second = func_0200e174(RAW_TIME->second);
            break;
        }
        case 6: {
            s32 *p = (s32 *)data_020bfd98.p1;
            if (RAW_STAT->mode != 4) { *p = res; } else { *p = 1; }
            break;
        }
        case 7: {
            s32 *p = (s32 *)data_020bfd98.p1;
            if (RAW_STAT->flag) *p = 1; else *p = res;
            break;
        }
        case 8: {
            s32 *p = (s32 *)data_020bfd98.p1;
            p[0] = RAW_ALARM->week;
            p[1] = func_0200e174(RAW_ALARM->hour);
            p[2] = func_0200e174(RAW_ALARM->minute);
            p[3] = res;
            if (RAW_ALARM->b7) p[3] += 1;
            if (RAW_ALARM->b15) p[3] += 2;
            if (RAW_ALARM->b23) p[3] += 4;
            break;
        }
        case 9: {
            u32 i = data_020bfd98.index;
            if (i == 0) {
                if (*(s32 *)data_020bfd98.p1 == 1) {
                    if (RAW_STAT->mode == 4) break;
                    data_020bfd98.index = i + 1;
                    RAW_STAT->mode = 4;
                    if (!func_0200e21c()) {
                        data_020bfd98.index = 0;
                        res = 3;
                    }
                } else {
                    if (RAW_STAT->mode == 0) break;
                    data_020bfd98.index = i + 1;
                    RAW_STAT->mode = 0;
                    if (!func_0200e21c()) {
                        data_020bfd98.index = 0;
                        res = 3;
                    }
                }
            } else {
                data_020bfd98.index = 0;
            }
            break;
        }
        case 10: {
            u32 i = data_020bfd98.index;
            if (i == 0) {
                if (*(s32 *)data_020bfd98.p1 == 1) {
                    if (RAW_STAT->flag) break;
                    data_020bfd98.index = i + 1;
                    RAW_STAT->flag = 1;
                    if (!func_0200e21c()) {
                        data_020bfd98.index = 0;
                        res = 3;
                    }
                } else {
                    if (!RAW_STAT->flag) break;
                    data_020bfd98.index = i + 1;
                    RAW_STAT->flag = 0;
                    if (!func_0200e21c()) {
                        data_020bfd98.index = 0;
                        res = 3;
                    }
                }
            } else {
                data_020bfd98.index = 0;
            }
            break;
        }
        case 3: case 4: case 5: case 11: case 12: case 13: case 14: case 15:
            break;
        default:
            data_020bfd98.index = 0;
            res = 4;
            break;
        }
    } else {
        data_020bfd98.index = 0;
        switch (err) {
        case 1: res = 4; break;
        case 2: res = 5; break;
        case 3: res = 1; break;
        case 0:
        case 4:
        default: res = 6; break;
        }
    }
    if (data_020bfd98.index == 0) {
        if (data_020bfd98.busy) data_020bfd98.busy = 0;
        if (data_020bfd98.cb) {
            RTCCallback cb = data_020bfd98.cb;
            data_020bfd98.cb = 0;
            cb(res, data_020bfd98.arg);
        }
    }
}

u32 func_0200e174(u32 bcd)
{
    int i;
    u32 r = 0;
    u32 m;
    for (i = 0; i < 8; i++) {
        if (((bcd >> (i * 4)) & 0xf) >= 10) return 0;
    }
    m = 1;
    for (i = 0; i < 8; i++) {
        r += m * ((bcd >> (i * 4)) & 0xf);
        m *= 10;
    }
    return r;
}

void func_0200e1e4(u32 result, void *arg)
{
    data_020bfd98.result = result;
}

asm void func_0200e1f4(void)
{
    ldr ip, =data_020bfda4
loop:
    ldr r0, [ip, #0]
    cmp r0, #1
    beq loop
    bx lr
}

BOOL func_0200e20c(void) { return func_0200e22c(0x12); }
BOOL func_0200e21c(void) { return func_0200e22c(0x27); }

BOOL func_0200e22c(u32 cmd)
{
    return PXI_SendWordByFifo(5, (cmd << 8) & 0x7f00, 0) >= 0;
}

u32 func_0200e254(RTCDate *date)
{
    s32 m;
    s32 y;
    s32 c;
    s32 yy;
    s32 t;
    s32 d;
    d = date->day;
    y = date->year + 2000;
    m = date->month - 2;
    if (m < 1) { y -= 1; m += 12; }
    c = y / 100;
    yy = y % 100;
    t = yy + (d + (m * 26 - 2) / 10) + yy / 4 + c / 4 + c * 5;
    return t % 7;
}
