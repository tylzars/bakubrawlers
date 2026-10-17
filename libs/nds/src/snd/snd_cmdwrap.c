#include <nitro/private/snd_h.h>

void func_02007f30(u32 a) {
    PushNewCommand(1, a, 0, 0, 0);
}

void func_02007f50(u32 a, u32 b, u32 c, u32 d) {
    PushNewCommand(2, a, b, c, d);
}

void func_02007f78(u32 a) {
    PushNewCommand(3, a, 0, 0, 0);
}

void func_02007f98(u32 a, u32 b) {
    func_02008254(a, 6, b, 2);
}

void func_02007fb0(u32 a, u32 b) {
    func_02008254(a, 4, b, 1);
}

void func_02007fc8(u32 a, u32 b, u32 c) {
    func_0200827c(a, b, 0xc, c, 2);
}

void func_02007fe4(u32 a, u32 b, u32 c) {
    PushNewCommand(9, a, b, c, 0);
}

void func_0200800c(u32 a, u32 b, u32 c, u32 d) {
    PushNewCommand(0xc, a, b, c, d);
}

void func_02008034(u32 a, u32 b, u32 c, u32 d) {
    int i;
    u32 mask = c;
    for (i = 0; i < 8 && mask != 0; i++, mask >>= 1) {
        if (mask & 1) {
            IncrementAlarmCount(i);
        }
    }
    PushNewCommand(0xd, a, b, c, d);
}

void func_02008098(u32 a, u32 b, u32 c, void (*cb)(void *), void *arg) {
    u32 tag = SetAlarmCallback(a, cb, arg);
    PushNewCommand(0x12, a, b, c, tag);
}

void func_020080d8(u32 a, u32 b) {
    PushNewCommand(0x1a, a, b, 0, 0);
}

void func_020080f8(u32 a, u32 b) {
    PushNewCommand(0x1b, a, b, 0, 0);
}

void func_02008118(u32 a, u32 b, u32 c) {
    PushNewCommand(0x14, a, b, c, 0);
}

void func_02008140(u32 a, u32 b) {
    PushNewCommand(0x15, a, b, 0, 0);
}

void func_02008160(u32 p0, u32 p1, u32 p2, u32 p3, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9, u32 a10) {
    PushNewCommand(0xe, p0 | (a9 << 16), p2, (a7 << 24) | (a8 << 22) | a6, (p3 << 26) | (p1 << 24) | (a10 << 16) | a5);
}

void func_020081ac(u32 a, u32 b) {
    PushNewCommand(0x1e, a, b, 0, 0);
}

void func_020081cc(u32 a, u32 b) {
    PushNewCommand(0x1f, a, b, 0, 0);
}

void func_020081ec(u32 a, u32 b) {
    PushNewCommand(0x20, a, b, 0, 0);
}

void func_0200820c(u32 a) {
    PushNewCommand(0x17, a, 0, 0, 0);
}

void func_0200822c(u32 a, u32 b, u32 c, u32 d) {
    PushNewCommand(0x19, a, b, c, d);
}

void func_02008254(u32 a, u32 b, u32 c, u32 d) {
    PushNewCommand(6, a, b, c, d);
}

void func_0200827c(u32 a, u32 b, u32 c, u32 d, u32 e) {
    PushNewCommand(7, a | (e << 24), b, c, d);
}

void PushNewCommand(u32 id, u32 a, u32 b, u32 c, u32 d) {
    SNDCommand *cmd = SND_AllocCommand(1);
    if (cmd == NULL) {
        return;
    }
    cmd->id = id;
    cmd->arg[0] = a;
    cmd->arg[1] = b;
    cmd->arg[2] = c;
    cmd->arg[3] = d;
    SND_PushCommand(cmd);
}
