#include "cri_obj_b.h"

typedef struct CriErrCb { void (*fn)(int, char *); int arg; } CriErrCb;
extern CriErrCb data_02215c7c;       /* error callback + user arg */
extern char data_02215ca4[0x100];    /* message buffer */
extern char data_02215c84[];         /* scratch digits */
extern char data_020aca2c[], data_020aca34[], data_020aca3c[];

extern void *memset(void *, int, int);
extern int func_02015a50(const char *);
extern void CriSendFormattedText(const char *);
extern void CriCopyStringN(char *, int, const char *, int);
extern void CriAppendStringN(char *, int, const char *, int);
extern void func_0203edbc(void);
extern void func_0203edc0(void);
extern void func_0203eda4(void);
extern void func_0203edb0(void);
extern void func_020426bc(void);
extern void StmAdvanceAll(void);
extern void func_02051510(void);
extern void func_020426dc(void);
extern void func_020406b0(void);
extern void func_02040370(void);



/* Same body as func_02040140. */
void func_02040110(void)
{
    memset(data_02215ca4, 0, 0x100);
    data_02215c7c.fn = 0;
    data_02215c7c.arg = 0;
}

/* Same body as func_02040110. */
void func_02040140(void)
{
    memset(data_02215ca4, 0, 0x100);
    data_02215c7c.fn = 0;
    data_02215c7c.arg = 0;
}

void SendErr(const char *msg)
{
    if (msg == 0) {
        CriSendFormattedText(data_020aca2c);
        return;
    }
    CriCopyStringN(data_02215ca4, 0x100, msg, 0xff);
    if (data_02215c7c.fn != 0) data_02215c7c.fn(data_02215c7c.arg, data_02215ca4);
    CriSendFormattedText(data_02215ca4);
}

void SendErrConcat(const char *a, const char *b)
{
    if (a == 0 || b == 0) {
        CriSendFormattedText(data_020aca34);
        return;
    }
    CriCopyStringN(data_02215ca4, 0x100, a, 0xff);
    CriAppendStringN(data_02215ca4, 0x100, b, 0xff);
    if (data_02215c7c.fn != 0) data_02215c7c.fn(data_02215c7c.arg, data_02215ca4);
    CriSendFormattedText(data_02215ca4);
}

void func_02040244(int val, char *buf, int size)
{
    int i = 0;
    int n, k;
    do {
        buf[i] = val % 10;
        val = val / 10;
        if (val == 0) {
            buf[i] = 0;
            break;
        }
        i++;
    } while (i < 0x20);
    n = func_02015a50(data_02215c84);
    if (n >= size - 1) n = size - 1;
    k = 0;
    if (n > 0) {
        do {
            buf[k] = data_02215c84[n - 1 - k];
            k++;
        } while (k < n);
    }
    buf[k] = 0;
}

void func_020402f0(int a, int b, char *buf, int size)
{
    int n;
    func_02040244(a, buf, size);
    CriAppendStringN(buf, size, data_020aca3c, size - func_02015a50(buf) - 1);
    n = func_02015a50(buf);
    func_02040244(b, buf + n, 4 - func_02015a50(buf));
}

void func_0204035c(void)
{
    func_0203edbc();
    func_02040370();
    func_0203edc0();
}

typedef struct CriShutdown {
    void (*done)(int);               /* 0x00 */
    int arg4;
    void (*pre)(int);                /* 0x08 */
    int pad0c;
    int state;                       /* 0x10 */
    int doneArg;                     /* 0x14 */
} CriShutdown;
extern CriShutdown data_02215da4;

void func_02040370(void)
{
    func_0203eda4();
    if (data_02215da4.state != 0) {
        func_0203edb0();
        return;
    }
    data_02215da4.state = 1;
    func_0203edb0();
    if (data_02215da4.pre != 0) data_02215da4.pre(data_02215da4.arg4);
    data_02215da4.state = 3;
    func_020426bc();
    data_02215da4.state = 4;
    StmAdvanceAll();
    data_02215da4.state = 5;
    func_02051510();
    data_02215da4.state = 6;
    StmAdvanceAll();
    data_02215da4.state = 7;
    func_020426bc();
    data_02215da4.state = 9;
    func_020426dc();
    data_02215da4.state = 0;
    if (data_02215da4.done != 0) data_02215da4.done(data_02215da4.doneArg);
}

void func_0204043c(int unused, const char *msg) { SendErr(msg); }

void func_0204044c(int unused, const char *msg) { SendErr(msg); }

int func_0204045c(void)
{
    func_020406b0();
    return 0;
}
