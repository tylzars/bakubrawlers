#define ALIGN32 __attribute__((aligned(32)))
#include <nitro/mi.h>
#include <nitro/os.h>
#include "System/WirelessManager.hpp"

// Wireless library (SDK WM) entry points, called only from this file. The names are the original addresses; the
// comments give the WM function each most likely is (certain only where noted by the code that surrounds the call).
extern "C" u32 func_020108e4(void *callback);
extern "C" u32 func_02010674(void *buf, void *callback, u32 size);
extern "C" u32 func_020110c0(void *buf);
extern "C" u32 func_020106e4(void *callback);
extern "C" u16 func_02010600();
extern "C" u16 func_020104dc();
extern "C" u16 func_0201056c();
extern "C" u32 func_0201075c(void *callback, void *param);
extern "C" u16 func_0201042c();
extern "C" u32 func_0201044c();
extern "C" u32 func_02010cf8(void *callback, u32 enable);
extern "C" u32 func_020109e0(void *callback, void *desc, void *ssid, u32 a, u32 b);
extern "C" u32 func_020108f4(void *callback, void *param);
extern "C" u32 func_02010d3c(void *callback, u32 a, void *data, u32 size, u32 b, u32 c, u32 d);
extern "C" u32 func_02010e6c(void *buf, u32 a, u32 b, u32 c, u32 d);
extern "C" u32 func_02010224(u32 port, void *callback, void *arg);
extern "C" u32 func_02010c1c(void *callback, void *a, u32 b, void *c, u32 d, u32 e);
extern "C" u32 func_02011108(void *buf, void *a, void *b);
extern "C" u32 func_02011a64(void *callback, u32 arg);
extern "C" u8 *func_02011968(void *buf, void *a, u32 index);
extern "C" u32 func_020119ec(void *callback, u32 a, u32 b, u32 channel, u32 c);
extern "C" void func_02005e64(void *p, u32 size);
extern "C" void func_01ff96c4(const void *src, void *dst, u32 size);
extern "C" void func_01ff9510(const void *src, void *dst, u32 size);
extern "C" u32 func_0201071c(void *callback);

// Parameters of the scan (the wireless library's WMScanParam).
struct WmScanParam {
    /* 00 */ WmEntry *mBuffer;
    /* 04 */ u16 mChannel;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u8 mBssid[6];
    /* 0e */
};

// Parameters of the hosted network (the wireless library's WMParentParam, 0x40 bytes).
struct WmParentParam {
    /* 00 */ void *mUnk_00;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u32 mGameId;
    /* 0c */ u16 mUnk_0c;
    /* 0e */ u16 mUnk_0e;
    /* 10 */ u16 mUnk_10;
    /* 12 */ u16 mUnk_12;
    /* 14 */ u16 mUnk_14;
    /* 16 */ u16 mUnk_16;
    /* 18 */ u16 mUnk_18;
    /* 1a */ PAD(0x1a, 0x30);
    /* 30 */ u16 mUnk_30;
    /* 32 */ u16 mChannel;
    /* 34 */ u32 mUnk_34;
    /* 38 */ PAD(0x38, 0x40);
};

static u8 data_0220f680[0x70] ALIGN32;
static WmParentParam data_020a5744 = {data_0220f680, 0x70, 0, 0x400904, 0, 1, 3, 0, 0, 0, 200, {0}, 0, 1, 0x28008e, {0}};

// The 4-byte aligned statics of the original object, 0x0220f560-0x0220f620 (addressed from the start of the shared
// section, which also holds the 0xc-byte atexit node first, hence offsets are the original ones minus 0xc).
struct WmWork {
    /* 00 */ PAD(0x00, 0x14);
    /* 14 */ u32 mCmd; // message sent to the other side (data_0220f580)
    /* 18 */ PAD(0x18, 0x34);
    /* 34 */ u32 mRecv[4]; // received 4-byte messages (data_0220f5a0)
    /* 44 */ PAD(0x44, 0x54);
    /* 54 */ u32 mSend[4]; // messages waiting to be sent (data_0220f5c0)
    /* 64 */ PAD(0x64, 0x74);
    /* 74 */ WmScanParam mScan; // data_0220f5e0
    /* 82 */ PAD(0x82, 0x94);
    /* 94 */ u32 mTmp; // data_0220f600
    /* 98 */
};
static WmWork data_0220f560;

static u8 data_0220f620[0x20] ALIGN32;
static u8 data_0220f640[0x40] ALIGN32;
static u8 data_0220f700[0xa0] ALIGN32;
static u8 data_0220f7a0[0xc0] ALIGN32;
static u8 data_0220f860[0x1c0] ALIGN32;
static u8 data_0220fa20[0x200] ALIGN32;
static u8 data_0220fc20[0x820] ALIGN32;
static u8 data_02210440[0xb9c] ALIGN32;
#define data_0220f580 (data_0220f560.mCmd)
#define data_0220f5a0 (data_0220f560.mRecv)
#define data_0220f5c0 (data_0220f560.mSend)
#define data_0220f5e0 (data_0220f560.mScan)
#define data_0220f600 (data_0220f560.mTmp)
#define SCAN_ENTRY ((WmEntry *) data_0220f7a0)
#define PARENT_PARAM ((WmParentParam *) data_0220f640)

// callbacks defined below
void func_0202db04(WmCallbackArg *arg);
void func_0202dcdc(WmCallbackArg *arg);
void func_0202dd60(WmCallbackArg *arg);
void func_0202ddd4(WmCallbackArg *arg);
void func_0202de58(WmCallbackArg *arg);
void func_0202dee4(WmCallbackArg *arg);
void func_0202e06c(WmCallbackArg *arg);
void func_0202e18c(WmCallbackArg *arg);
void func_0202e224(WmCallbackArg *arg);
void func_0202e288(WmCallbackArg *arg);
void func_0202e374(WmCallbackArg *arg);
void func_0202e478(WmCallbackArg *arg);
void func_0202e4e8(WmCallbackArg *arg);

void func_0202db04(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult != 0) {
        return;
    }
    switch (arg->mUnk_04) {
        case 0x15:
            if (arg->mUnk_10 == 4) {
                u8 *p   = arg->mUnk_0c;
                u32 cmd = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
                switch (cmd) {
                    case 0:
                        if (mgr.mUnk_1228 == 6) {
                            u32 v = mgr.mUnk_124f | (1 << arg->mUnk_12);
                            mgr.mUnk_124f = v;
                            if (mgr.mUnk_1230 == (u8) ((v & 0xff) & mgr.mUnk_1230)) {
                                mgr.func_0202f1a4(0);
                            }
                        } else {
                            mgr.func_0202ec9c(0);
                        }
                        break;
                    case 1:
                        if (mgr.mUnk_1228 == 6) {
                            mgr.func_0202f1a4(1);
                            mgr.mUnk_124f = 1;
                        } else {
                            mgr.func_0202ec9c(1);
                        }
                        break;
                    case 2:
                        mgr.mUnk_1230 |= 1 << arg->mUnk_12;
                        break;
                    case 3:
                        mgr.func_0202ed28();
                        break;
                }
            } else {
                func_01ff96c4(arg->mUnk_0c, &data_0220f5a0[mgr.mUnk_1249], 4);
                mgr.mUnk_1249 = (mgr.mUnk_1249 + 1) % 4;
            }
            break;
        case 7:
        case 9:
        case 0x1a:
            break;
    }
}

void func_0202dcdc(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult == 0) {
        mgr.mUnk_1228 = 1;
        mgr.mUnk_122a = 0;
        return;
    }
    mgr.mUnk_122a = 6;
}

void func_0202dd60(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult == 0) {
        mgr.mUnk_122a = 0;
    } else {
        mgr.mUnk_122a = 6;
    }
}

void func_0202ddd4(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult != 0) {
        mgr.mUnk_122a = 6;
        return;
    }
    mgr.mUnk_122a = 5;
    mgr.mUnk_1228 = 0;
}

void func_0202de58(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult != 0) {
        mgr.mUnk_122a = 6;
        return;
    }
    func_020108e4((void *) func_0202dee4);
    mgr.mUnk_122a = 4;
}

void func_0202dee4(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult == 0) {
        switch (arg->mUnk_08) {
            case 0:
                mgr.func_0202ea34();
                break;
            case 2:
                break;
            case 7: {
                mgr.mUnk_1228 = 6;
                mgr.mUnk_122a = 2;
                mgr.mUnk_122e++;
                if (mgr.mUnk_122e >= mgr.mUnk_122d) {
                    mgr.func_0202ec74(0);
                }
                if (mgr.mUnk_0004 != 0) {
                    ((void (*)(u8, void *)) mgr.mUnk_0004)(arg->mUnk_10, arg->mUnk_14);
                }
                if (mgr.mUnk_1230 != 0) {
                    mgr.func_0202ed00();
                }
                break;
            }
            case 9:
                if (mgr.mUnk_1250 == 1) {
                    if (mgr.mUnk_122e != 0) {
                        mgr.mUnk_122e--;
                    }
                    mgr.func_0202ec74(1);
                } else {
                    mgr.mUnk_122a = 6;
                }
                break;
            case 0x1a:
                mgr.mUnk_122a = 6;
                break;
        }
    } else {
        mgr.mUnk_122a = 6;
    }
}

void func_0202e06c(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult == 0) {
        switch (arg->mUnk_08) {
          case 5:
            func_02005e64(data_0220f7a0, 0xc0);
            if ((SCAN_ENTRY->mUnk_4b & 3) == 1) {
                bool big   = SCAN_ENTRY->mUnk_3c >= 8;
                bool equal = SCAN_ENTRY->mUnk_44 == data_020a5744.mGameId;
                if (big & equal) {
                    mgr.func_0202e864();
                }
            }
          case 4:
            if (mgr.mUnk_1228 == 4) {
                mgr.func_0202e76c();
                mgr.func_0202e7c8();
            }
            break;
          default:
            mgr.mUnk_122a = 6;
            break;
        }
    } else {
        mgr.mUnk_122a = 6;
    }
}

void func_0202e18c(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult != 0) {
        mgr.mUnk_122a = 6;
        return;
    }
    u16 a = arg->mUnk_0a;
    if (mgr.mUnk_1224 >= a) {
        mgr.mUnk_1224 = a;
        mgr.mUnk_1226 = arg->mUnk_08;
    }
    mgr.func_0202e618();
}

void func_0202e224(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    mgr.mUnk_122b = 1;
}

void func_0202e288(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult == 0) {
        switch (arg->mUnk_04) {
            case 0xa:
                mgr.func_0202eb70();
                mgr.mUnk_122b = 1;
                switch (mgr.mUnk_1228) {
                    case 3:
                        mgr.func_0202ea18();
                        break;
                    case 5:
                        mgr.mUnk_1228 = 7;
                        mgr.mUnk_122a = 3;
                        mgr.func_0202ea18();
                        break;
                }
                break;
            case 0xb:
            case 0xc:
                break;
        }
    } else {
        u16 e = arg->mResult;
        if (e != 9 && e != 0xd && e != 0xf) {
            mgr.mUnk_122a = 6;
        }
    }
}

void func_0202e374(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult == 0) {
        switch (arg->mUnk_08) {
            case 0:
            case 1:
            case 2:
                break;
            case 7: {
                mgr.mUnk_1220 = arg->mUnk_0a;
                if (mgr.mUnk_122a != 3) {
                    mgr.func_0202ea34();
                }
                break;
            }
            case 9:
                mgr.mUnk_122a = 6;
                break;
            case 0x1a:
                mgr.mUnk_122a = 6;
                break;
        }
    } else {
        mgr.mUnk_122a = 6;
    }
}

void func_0202e478(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult != 0) {
        mgr.mUnk_122a = 6;
    }
}

void func_0202e4e8(WmCallbackArg *arg) {}

void WirelessManager::func_0202e4ec() {
    func_02010674(data_02210440, (void *) func_0202dcdc, 2);
    mUnk_122a = 4;
    mUnk_122c = 0;
}

void WirelessManager::func_0202e524() {
    mUnk_122a = 4;
    func_0201071c((void *) func_0202ddd4);
}

void WirelessManager::func_0202e544() {
    mUnk_122a = 4;
    if (mUnk_122c != 0) {
        func_020110c0(data_0220fc20);
        mUnk_122c = 0;
    }
    if (func_020106e4((void *) func_0202dd60) != 2) {
        mUnk_122a = 6;
    }
}

void WirelessManager::func_0202e59c() {
    func_01ff9510(&data_020a5744, data_0220f640, 0x40);
    PARENT_PARAM->mUnk_10 = mUnk_122d;
    PARENT_PARAM->mUnk_0c     = func_02010600();
    PARENT_PARAM->mChannel  = mUnk_1226;
    PARENT_PARAM->mUnk_18 = func_020104dc();
    mUnk_122e = 0;
    func_0201075c((void *) func_0202de58, data_0220f640);
    mUnk_122a = 4;
}

void WirelessManager::func_0202e618() {
    u32 mask = func_0201042c();
    if (mask == 0x8000) {
        mUnk_122a = 6;
        return;
    }
    if (mask == 0) {
        mUnk_122a = 6;
        return;
    }
    do {
        mUnk_1222++;
        if (mask & (1 << (mUnk_1222 - 1))) {
            if (func_020119ec((void *) func_0202e18c, 3, 0x11, mUnk_1222, 0x1e) == 2) {
                mUnk_122a = 4;
                break;
            }
            mUnk_122a = 6;
        }
    } while (mUnk_1222 < 0x10);
    if (mUnk_1222 < 0x10) {
        return;
    }
    mUnk_1228 = 3;
    mUnk_122a = 0;
}

void WirelessManager::func_0202e6f8() {
    mUnk_1222 = 0;
    mUnk_1224 = 0x65;
    func_0202e618();
}

void WirelessManager::func_0202e718() {
    data_0220f5e0.mBuffer = SCAN_ENTRY;
    data_0220f5e0.mChannel = data_020a5744.mChannel;
    data_0220f5e0.mUnk_06 = func_0201056c();
    data_0220f5e0.mBssid[0] = 0xff;
    data_0220f5e0.mBssid[1] = 0xff;
    data_0220f5e0.mBssid[2] = 0xff;
    data_0220f5e0.mBssid[3] = 0xff;
    data_0220f5e0.mBssid[4] = 0xff;
    data_0220f5e0.mBssid[5] = 0xff;
}

// non-matching: 4 words, `start` and the constant 1 swap registers.
void WirelessManager::func_0202e76c() {
    u16 mask = func_0201042c();
    if (mask == 0) { return; }
    int start = data_0220f5e0.mChannel;
    int i;
    for (i = 0; i < 16; i++) {
        int ch = (start + i) % 16;
        if (mask & (1 << ch)) { data_0220f5e0.mChannel = ch + 1; return; }
    }
}

void WirelessManager::func_0202e7c8() {
    if (func_020108f4((void *) func_0202e06c, &data_0220f5e0) != 2) {
        mUnk_122a = 6;
    } else {
        mUnk_122a = 1;
    }
}

void WirelessManager::func_0202e7fc(u32 unk1) {
    func_01ff96c4(&mUnk_0908[unk1], data_0220f7a0, 0xc0);
    if (func_020109e0((void *) func_0202e374, data_0220f7a0, mUnk_1231, 1, 0) != 2) {
        mUnk_122a = 6;
    } else {
        mUnk_122a = 4;
    }
}

void WirelessManager::func_0202e864() {
    WmEntry *scan = (WmEntry *) data_0220f7a0;
    u8 i = 0;
    do {
        if (mUnk_1208[i] != 0) {
            bool m4 = false;
            bool m3 = false;
            bool m2 = false;
            bool m1 = false;
            bool m0 = false;
            if (scan->mBssid[0] == mUnk_0008[i].mBssid[0]) { if (scan->mBssid[1] == mUnk_0008[i].mBssid[1]) { m0 = true; } }
            if (m0) { if (scan->mBssid[2] == mUnk_0008[i].mBssid[2]) { m1 = true; } }
            if (m1) { if (scan->mBssid[3] == mUnk_0008[i].mBssid[3]) { m2 = true; } }
            if (m2) { if (scan->mBssid[4] == mUnk_0008[i].mBssid[4]) { m3 = true; } }
            if (m3) { if (scan->mBssid[5] == mUnk_0008[i].mBssid[5]) { m4 = true; } }
            if (m4) {
                func_01ff96c4(data_0220f7a0, &mUnk_0008[i], 0xc0);
                mUnk_1208[i] = 300;
                return;
            }
        }
        i++;
    } while (i < WM_ENTRY_COUNT);
    i = 0;
    do {
        if (mUnk_1208[i] == 0) {
            func_01ff96c4(data_0220f7a0, &mUnk_0008[i], 0xc0);
            mUnk_1208[i] = 300;
            return;
        }
        i++;
    } while (i < WM_ENTRY_COUNT);
}

void WirelessManager::func_0202e9e0() {
    for (u8 i = 0; i < WM_ENTRY_COUNT; i++) {
        if (mUnk_1208[i] != 0) {
            mUnk_1208[i]--;
        }
    }
}

void WirelessManager::func_0202ea18() {
    func_02010224(0xc, (void *) func_0202db04, 0);
}

void WirelessManager::func_0202ea34() {
    func_02010c1c((void *) func_0202e288, data_0220f860, 0x1c0, data_0220f700, 0xa0, 1);
    mUnk_122a = 4;
}

void WirelessManager::func_0202ea84() {
    if (mUnk_122b == 0 || mUnk_124b == mUnk_124c) {
        return;
    }
    mUnk_122b = 0;
    func_01ff96c4(&data_0220f5c0[mUnk_124c], &data_0220f600, 4);
    if (func_0202eb08(&data_0220f600, 4) == 0) {
        return;
    }
    mUnk_124c = (mUnk_124c + 1) % 4;
}

u32 WirelessManager::func_0202eb08(void *unk1, u32 unk2) {
    if (func_02010d3c((void *) func_0202e224, 0, unk1, unk2, 0xffff, 0xc, 2) == 2) {
        return 1;
    }
    mUnk_122b = 1;
    return 0;
}

void WirelessManager::func_0202eb70() {
    u32 two = 0xf;
    u32 one = 0xd;
    if (func_02010e6c(data_0220fc20, one, two, 0x20, 1) == 0) {
        mUnk_122c = 1;
    }
    mUnk_1254 = 0x708;
}

void WirelessManager::func_0202ebc4() {
    switch (func_02011108(data_0220fc20, data_0220f620, data_0220fa20)) {
        case 0:
            mUnk_124e = 1;
            break;
        case 5:
            mUnk_124e = 0;
            break;
        case 1:
        case 3:
            mUnk_122a = 6;
            mUnk_124e = 0;
            break;
    }
    if (mUnk_124e != 0) {
        mUnk_1254 = 0x708;
    } else {
        if (--mUnk_1254 == 0) {
            mUnk_122a = 6;
        }
    }
}

void WirelessManager::func_0202ec74(u32 unk1) {
    if (func_02011a64((void *) func_0202e478, unk1) != 2) {
        mUnk_122a = 6;
    }
}

void WirelessManager::func_0202ec9c(u32 unk1) {
    if (unk1 != 0) {
        if (mUnk_122f == 0) {
            func_02010cf8((void *) func_0202e4e8, 0);
            mUnk_122f = 1;
        }
    } else {
        if (mUnk_122f == 1) {
            func_02010cf8((void *) func_0202e4e8, 1);
            mUnk_122f = 0;
        }
    }
}

void WirelessManager::func_0202ed00() {
    data_0220f580 = 2;
    func_0202eb08(&data_0220f580, 4);
}

void WirelessManager::func_0202ed28() {}

void WirelessManager::func_0202ed2c() {
    mUnk_1228 = 0;
}

u32 WirelessManager::func_0202ed3c() {
    if (mUnk_1228 != 0) {
        return 0;
    }
    func_0202e4ec();
    mUnk_1249 = 0;
    mUnk_124a = 0;
    mUnk_124b = 0;
    mUnk_124c = 0;
    mUnk_1220 = 0;
    MI_CpuFill8(data_0220fc20, 0, 0x820);
    MI_CpuFill8(data_0220fa20, 0, 0x200);
    MI_CpuFill8(data_0220f620, 0, 0x20);
    mUnk_0004 = 0;
    mUnk_124d = 0;
    mUnk_1230 = 0;
    return 1;
}

u32 WirelessManager::func_0202edd0() {
    u8 mode = mUnk_1228;
    if (mode == 0) {
        return 1;
    }
    if (mode == 8) {
        return 1;
    }
    func_0202e544();
    mUnk_1228 = 8;
    return 1;
}

u32 WirelessManager::func_0202ee10(u32 unk1, void *unk2, u32 unk3, u32 unk4) {
    if (mUnk_1228 == 0) {
        return 0;
    }
    if (unk1 > 3) {
        return 0;
    }
    mUnk_122d = unk1;
    mUnk_1250 = 1;
    if (unk3 > 0x70) {
        unk3 = 0x70;
    }
    MI_CpuFill8(data_0220f680, 0, 0x70);
    if (unk2 != 0 && unk3 != 0) {
        func_01ff96c4(unk2, data_0220f680, unk3);
    }
    func_0202e544();
    mUnk_0004 = unk4;
    mUnk_1228 = 2;
    mUnk_122f = 0;
    return 1;
}

u32 WirelessManager::func_0202eeb0() {
    if (mUnk_1228 == 0) {
        return 0;
    }
    MI_CpuFill8(mUnk_1208, 0, 0x18);
    func_0202e544();
    mUnk_1228 = 4;
    return 1;
}

u32 WirelessManager::func_0202eefc(u32 unk1, void *unk2, u32 unk3) {
    if (mUnk_1228 == 0) {
        return 0;
    }
    if (unk3 > 0x18) {
        unk3 = 0x18;
    }
    MI_CpuFill8(mUnk_1231, 0, 0x18);
    if (unk2 != 0 && unk3 != 0) {
        func_01ff96c4(unk2, mUnk_1231, unk3);
    }
    func_0202e544();
    mUnk_1228 = 5;
    mUnk_1229 = unk1;
    return 1;
}

u32 WirelessManager::func_0202ef80() {
    if (mUnk_1228 != 4) {
        return 0;
    }
    vu16 *ime = (vu16 *) 0x04000208;
    u16 old   = *ime;
    *ime      = 0;
    u8 n = 0;
    u8 i = 0;
    for (i = 0; i < WM_ENTRY_COUNT; i++) {
        if (mUnk_1208[i] != 0) {
            func_01ff96c4(&mUnk_0008[i], &mUnk_0908[n++], WM_ENTRY_SIZE);
        }
    }
    u16 tmp = *ime;
    *ime    = old;
    return n;
}

u32 WirelessManager::func_0202f024(void *unk1) {
    func_01ff96c4(unk1, &data_0220f5c0[mUnk_124b], 4);
    mUnk_124b = (mUnk_124b + 1) % 4;
    func_0202ea84();
    return 1;
}

u32 WirelessManager::func_0202f078(void *unk1) {
    if ((mUnk_1228 != 6) & (mUnk_1228 != 7)) {
        return 0;
    }
    func_01ff96c4(unk1, data_0220f620, 0x20);
    return 1;
}

u8 *WirelessManager::func_0202f0cc(u32 unk1) {
    if ((mUnk_1228 != 6) & (mUnk_1228 != 7)) {
        return 0;
    }
    if (mUnk_122f != 0) {
        return 0;
    }
    return func_02011968(data_0220fc20, data_0220fa20, unk1);
}

u32 WirelessManager::func_0202f134() {
    u8 count = 0;
    u8 i = 0;
    do {
        if (func_02011968(data_0220fc20, data_0220fa20, i) != 0) {
            count++;
        }
        i++;
    } while (i <= 3);
    return count;
}

void WirelessManager::func_0202f184() {
    func_0202ec74(0);
    mUnk_1250 = 0;
}

void WirelessManager::func_0202f1a4(u32 unk1) {
    if (!((mUnk_1228 == 6) | (mUnk_1228 == 7))) {
        return;
    }
    if (unk1 != mUnk_122f && mUnk_1228 == 6) {
        func_0202ec9c(unk1);
    }
    data_0220f580 = unk1 != 0 ? 1 : 0;
    func_0202eb08(&data_0220f580, 4);
}

u32 WirelessManager::func_0202f238() {
    return mUnk_1230;
}

void WirelessManager::func_0202f244() {
    if (mUnk_122a == 6 && mUnk_1228 != 9) {
        func_0202e544();
        if (mUnk_1228 != 8) {
            mUnk_1228 = 9;
        }
    }
    switch (mUnk_1228) {
        case 8:
            switch (mUnk_122a) {
                case 0:
                    func_0202e524();
                    break;
                case 5:
                    break;
            }
            break;
        case 2:
            mUnk_124d = func_0201044c();
            if (mUnk_122a == 0) {
                func_0202e6f8();
            }
            break;
        case 3:
            mUnk_124d = func_0201044c();
            if (mUnk_122a == 0) {
                func_0202e59c();
            }
            break;
        case 4:
            mUnk_124d = func_0201044c();
            switch (mUnk_122a) {
                case 0:
                    func_0202e718();
                    func_0202e7c8();
                case 1:
                    func_0202e9e0();
                    break;
            }
            break;
        case 5:
            mUnk_124d = func_0201044c();
            if (mUnk_122a == 0) {
                func_0202e7fc(mUnk_1229);
            }
            break;
        case 6:
            mUnk_124d = func_0201044c();
            if (mUnk_122f != 0) {
                mUnk_124e = 1;
                break;
            }
            func_0202ebc4();
            func_0202ea84();
            break;
        case 7:
            mUnk_124d = func_0201044c();
            if (mUnk_122f != 0) {
                mUnk_124e = 1;
                break;
            }
            func_0202ebc4();
            func_0202ea84();
            break;
        case 9:
            mUnk_124d = func_0201044c();
            mUnk_122a = 5;
            break;
    }
}
