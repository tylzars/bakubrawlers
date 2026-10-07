#include "Game/UnkStruct_020d43f0.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d48f0.hpp"
#include "System/FrameTask.hpp"
#include "System/SoundManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "System/WirelessManager.hpp"

#include <nitro/mi.h>

// Tables that sit in the code region (0x020906f0-0x02090754).
extern const u8 data_020906f0[];
extern const u8 data_020906f8[];
extern const float data_02090724[];
extern const float data_0209070c[];
extern const float data_0209073c[];
extern const u16 data_02090754[][6];
extern u8 data_020d4946[];
extern u16 data_020c4250;
int func_0205b1c4(u32 a, u32 b);

void func_02022d48(UnkStruct_020d43f0 *d);
void func_02022db4(UnkStruct_020d43f0 *d);

void func_02022b0c() {
    MI_CpuFill8(&data_020d433c, 0, 0xb4);
}

// non-matching: 10 words, store and flag-bit scheduling in the second loop.
void func_02022b28() {
    MI_CpuFill8(data_020d43f0, 0, 0xff4);
    func_02022d48(data_020d43f0);
    data_020d43f0[0].mMoney = 3000;
    func_02086938();
    func_02086a58();
    func_02086b74();
    data_020d48f0.mUnk_bc = 999;
    data_020d48f0.mUnk_ba = 999;
    u8 i = 0;
    data_020d43f0[0].mUnk_1a_0 = 1;
    data_020d43f0[0].mUnk_14_1 = 0;
    data_020d43f0[0].mUnk_14_4 = 0;
    data_020d43f0[0].mUnk_14_7 = 0;
    data_020d43f0[0].mUnk_14_10 = 0;
    data_020d43f0[0].mUnk_16_12 = 0;
    data_020d43f0[0].mUnk_18_0 = 2;
    data_020d43f0[0].mUnk_18_3 = 0;
    data_020d43f0[0].mUnk_18_8 = 2;
    data_020d43f0[0].mUnk_18_14 = 0;
    data_020d43f0[0].mUnk_18_11 = 1;
    data_020d43f0[0].mUnk_27_5 = 0;
    data_020d43f0[0].mUnk_27 |= 0x1c;
    data_020d433c.mUnk_9a = data_020d43f0[0].mUnk_27;
    do {
        func_02023168(data_020d43f0, 0, i, 0xffff);
        i++;
    } while (i < 3);
    u8 n = 0;
    const u16 (*tab)[6] = data_02090754;
    UnkStruct_020d43f0 *base = data_020d43f0;
    do {
        const u16 *rec = tab[n];
        int idx = func_0202317c(base, tab[n][0] & 0xff, rec[1] & 0xff, rec[2] & 0xff);
        func_02023168(base, 0, n, idx);
        func_020231c0(base, idx, rec[3]);
        func_0205b1c4(rec[1] & 0xff, rec[2] & 0xff);
        u16 a = rec[4];
        u16 b = rec[5];
        base->mUnk_06[n] = a;
        SetFlagBit(data_020d4946, a);
        base->mUnk_0c[n] = b;
        SetFlagBit(data_020d4946, b);
        n++;
    } while (n < 3);
}

void func_02022d48(UnkStruct_020d43f0 *d) {
    UnkStruct_020d43f0 *p = data_020d43f0;
    p->mUnk_28_5 = 1;
    p->mUnk_28_6 = 1;
    p->mUnk_28_7 = 5;
    p->mUnk_28_10 = 5;
    p->mUnk_28_13 = 5;
    p->mUnk_27_0 = p->mUnk_28_6;
    func_02022db4(d);
}

void func_02022db4(UnkStruct_020d43f0 *d) {
    SoundManager::Get().mUnk_ae = data_020906f0[d->mUnk_28_7];
    SoundManager::Get().mUnk_ad = data_020906f8[d->mUnk_28_10];
    FrameTask::Get().mVolume0 = data_02090724[d->mUnk_28_7];
    FrameTask::Get().mVolume1 = data_0209070c[d->mUnk_28_10];
    FrameTask::Get().mVolume2 = data_0209073c[d->mUnk_28_13];
    SoundManager::Get().func_020246a4(0x7f);
    SoundManager::Get().func_020247cc(0x7f);
    FrameTask::Get().func_0203ddb0(0, 4);
    FrameTask::Get().func_0203ddb0(1, 4);
}

s32 func_020230a8(UnkStruct_020d43f0 *d, u32 a, u32 b) {
    u16 *rec = (u16 *) ((u8 *) d + a * 0x14);
    return (u8) (rec[b] % 6);
}

u8 func_020230dc(UnkStruct_020d43f0 *d, u32 a, u32 b) {
    u16 *rec = (u16 *) ((u8 *) d + a * 0x14);
    u32 v = rec[b];
    if (v >= 0xf6) {
        return 0xff;
    }
    u16 q = v / 6;
    return q % 41;
}

int func_0202317c(UnkStruct_020d43f0 *d, u32 a, u32 b, u32 c) {
    if (a >= 1) {
        return 0;
    }
    if (b >= 0x29) {
        return 0;
    }
    if (c >= 6) {
        return 0;
    }
    return (u16) (c + (b * 6 + a * 0xf6));
}

void func_02023134(UnkStruct_020d43f0 *d, u32 a, u32 b, u32 c, u32 e, u32 f) {
    u16 *rec = (u16 *) ((u8 *) d + a * 0x14);
    rec[b] = func_0202317c(d, c, *(u8 *) &e, *(u8 *) &f);
}

void func_02023168(UnkStruct_020d43f0 *d, u32 a, u32 b, int c) {
    u16 *rec = (u16 *) ((u8 *) d + a * 0x14);
    rec[b] = c;
}

void func_020231c0(UnkStruct_020d43f0 *d, int idx, u32 v) {
    if (v >= 10) {
        v = (u16) ((int) v / 10);
    }
    d->mUnk_460[idx] = v;
}

u16 func_020231f4(UnkStruct_020d43f0 *d, u32 a, u32 b, u32 c) {
    int idx = func_0202317c(d, a, b, c);
    return d->mUnk_460[idx] * 10;
}

void func_0202321c(UnkStruct_020d43f0 *d, const u16 *src, u32 b, u32 c) {
    int idx = func_0202317c(d, 0, b, c);
    d->mUnk_88[idx][0] = src[0];
    d->mUnk_88[idx][1] = src[1];
}

UnkStruct_02023248 *func_02023248(UnkStruct_020d43f0 *d, u32 a, u32 b) {
    int idx = func_0202317c(d, 0, a, b);
    return (UnkStruct_02023248 *) &d->mUnk_88[idx];
}

BOOL func_0202326c(UnkStruct_020d43f0 *d) {
    BOOL found = FALSE;
    u8 pairs[6][2] = {{1, 2}, {3, 3}, {6, 2}, {7, 4}, {8, 3}, {9, 6}};
    u8 i = 0;
    u8 flag = data_020d433c.mUnk_7a;
    do {
        if (flag && d->mUnk_42 == pairs[i][0] && d->mUnk_43 == pairs[i][1]) {
            found = TRUE;
        }
        i++;
    } while (i < 6);
    return found;
}

void func_0202330c(UnkStruct_020d433c *d) {
    MI_CpuFill8(d->mUnk_af, 0, 4);
    u8 i;
    u8 n = 0;
    i = 0;
    do {
        WirelessManager::Get();
        if (i == data_020c4250) {
            d->mUnk_ae = n;
        }
        if (WirelessManager::Get().func_0202f0cc(i)) {
            d->mUnk_af[n++] = i;
        }
        i++;
    } while (i < 4);
}

void func_020233f8(UnkStruct_020d433c *d) {
    u8 n = 0;
    u8 i = 0;
    do {
        WirelessManager::Get();
        if (i == data_020c4250) {
            d->mUnk_ae = n;
        }
        if (WirelessManager::Get().func_0202f0cc(i)) {
            n++;
        }
        i++;
    } while (i < 4);
}
