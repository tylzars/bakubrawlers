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
extern "C" void DC_InvalidateRange(void *p, u32 size);
extern "C" void MI_CpuCopy8(const void *src, void *dst, u32 size);
extern "C" void MIi_CpuCopy32(const void *src, void *dst, u32 size);
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

static u8 sParentData[0x70] ALIGN32;
static WmParentParam sDefaultParentParam = {sParentData, 0x70, 0, 0x400904, 0, 1, 3, 0, 0, 0, 200, {0}, 0, 1, 0x28008e, {0}};

// The 4-byte aligned statics of the original object share one section, 0x0220f560-0x0220f620, with the WirelessManager
// singleton's atexit node at 0x0220f584 between them; the code addresses them from the start of the section.
struct WmWorkB {
    /* 00 */ PAD(0x00, 0x10);
    /* 10 */ u32 mRecv[4]; // received 4-byte messages (RECV_QUEUE)
    /* 20 */ PAD(0x20, 0x30);
    /* 30 */ u32 mSend[4]; // messages waiting to be sent (SEND_QUEUE)
    /* 40 */ PAD(0x40, 0x50);
    /* 50 */ WmScanParam mScan; // SCAN_PARAM
    /* 60 */ PAD(0x60, 0x70);
    /* 70 */ u32 mTmp; // data_0220f600
    /* 74 */ PAD(0x74, 0x90);
};
static u32 data_0220f570;
static u32 data_0220f57c;
static u32 data_0220f578;
static u32 sCommandWord;
static u32 data_0220f568;
static u32 data_0220f56c;
static u32 data_0220f560;
static u32 data_0220f564;
static u32 data_0220f574;

// Gives the eight statics above a use (no code in the ROM uses them, but the .bss layout has them): without a
// use the compiler drops them, and they are what puts sCommandWord 0x20 bytes into the section.
void WirelessManager_StrippedStatics() {
    data_0220f570 = data_0220f57c = data_0220f578 = data_0220f574 = 0;
    data_0220f568 = data_0220f56c = data_0220f560 = data_0220f564 = 0;
}
static WmWorkB sWmWorkB;

static u8 data_0220f620[0x20] ALIGN32;
static u8 sParentParam[0x40] ALIGN32;
static u8 data_0220f700[0xa0] ALIGN32;
static u8 sEntryBuffer[0xc0] ALIGN32;
static u8 data_0220f860[0x1c0] ALIGN32;
static u8 data_0220fa20[0x200] ALIGN32;
static u8 data_0220fc20[0x820] ALIGN32;
static u8 data_02210440[0xb9c] ALIGN32;
#define RECV_QUEUE (sWmWorkB.mRecv)
#define SEND_QUEUE (sWmWorkB.mSend)
#define SCAN_PARAM (sWmWorkB.mScan)
#define data_0220f600 (sWmWorkB.mTmp)
#define SCAN_ENTRY ((WmEntry *) sEntryBuffer)
#define PARENT_PARAM ((WmParentParam *) sParentParam)

// callbacks defined below
void RecvCallback(WmCallbackArg *arg);
void StartCallback(WmCallbackArg *arg);
void func_0202dd60(WmCallbackArg *arg);
void StopCallback(WmCallbackArg *arg);
void func_0202de58(WmCallbackArg *arg);
void func_0202dee4(WmCallbackArg *arg);
void ScanCallback(WmCallbackArg *arg);
void TestChannelCallback(WmCallbackArg *arg);
void SendCallback(WmCallbackArg *arg);
void func_0202e288(WmCallbackArg *arg);
void JoinCallback(WmCallbackArg *arg);
void func_0202e478(WmCallbackArg *arg);
void func_0202e4e8(WmCallbackArg *arg);

void RecvCallback(WmCallbackArg *arg) {
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
                        if (mgr.mState == 6) {
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
                        if (mgr.mState == 6) {
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
                MI_CpuCopy8(arg->mUnk_0c, &RECV_QUEUE[mgr.mRecvWriteIndex], 4);
                mgr.mRecvWriteIndex = (mgr.mRecvWriteIndex + 1) % 4;
            }
            break;
        case 7:
        case 9:
        case 0x1a:
            break;
    }
}

void StartCallback(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult == 0) {
        mgr.mState = 1;
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

void StopCallback(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult != 0) {
        mgr.mUnk_122a = 6;
        return;
    }
    mgr.mUnk_122a = 5;
    mgr.mState = 0;
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
                mgr.mState = 6;
                mgr.mUnk_122a = 2;
                mgr.mUnk_122e++;
                if (mgr.mUnk_122e >= mgr.mUnk_122d) {
                    mgr.func_0202ec74(0);
                }
                if (mgr.mParentCallback != 0) {
                    ((void (*)(u8, void *)) mgr.mParentCallback)(arg->mUnk_10, arg->mUnk_14);
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

void ScanCallback(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult == 0) {
        switch (arg->mUnk_08) {
          case 5:
            DC_InvalidateRange(sEntryBuffer, 0xc0);
            if ((SCAN_ENTRY->mUnk_4b & 3) == 1) {
                bool big   = SCAN_ENTRY->mUnk_3c >= 8;
                bool equal = SCAN_ENTRY->mUnk_44 == sDefaultParentParam.mGameId;
                if (big & equal) {
                    mgr.StoreScanResult();
                }
            }
          case 4:
            if (mgr.mState == 4) {
                mgr.SelectNextScanChannel();
                mgr.RequestScan();
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

void TestChannelCallback(WmCallbackArg *arg) {
    WirelessManager &mgr = WirelessManager::Get();
    if (arg->mResult != 0) {
        mgr.mUnk_122a = 6;
        return;
    }
    u16 a = arg->mUnk_0a;
    if (mgr.mMinTestResult >= a) {
        mgr.mMinTestResult = a;
        mgr.mParentChannel = arg->mUnk_08;
    }
    mgr.TestNextChannel();
}

void SendCallback(WmCallbackArg *arg) {
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
                switch (mgr.mState) {
                    case 3:
                        mgr.SetRecvCallback();
                        break;
                    case 5:
                        mgr.mState = 7;
                        mgr.mUnk_122a = 3;
                        mgr.SetRecvCallback();
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

void JoinCallback(WmCallbackArg *arg) {
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
    func_02010674(data_02210440, (void *) StartCallback, 2);
    mUnk_122a = 4;
    mUnk_122c = 0;
}

void WirelessManager::func_0202e524() {
    mUnk_122a = 4;
    func_0201071c((void *) StopCallback);
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
    MIi_CpuCopy32(&sDefaultParentParam, sParentParam, 0x40);
    PARENT_PARAM->mUnk_10 = mUnk_122d;
    PARENT_PARAM->mUnk_0c     = func_02010600();
    PARENT_PARAM->mChannel  = mParentChannel;
    PARENT_PARAM->mUnk_18 = func_020104dc();
    mUnk_122e = 0;
    func_0201075c((void *) func_0202de58, sParentParam);
    mUnk_122a = 4;
}

void WirelessManager::TestNextChannel() {
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
        mTestedChannel++;
        if (mask & (1 << (mTestedChannel - 1))) {
            if (func_020119ec((void *) TestChannelCallback, 3, 0x11, mTestedChannel, 0x1e) == 2) {
                mUnk_122a = 4;
                break;
            }
            mUnk_122a = 6;
        }
    } while (mTestedChannel < 0x10);
    if (mTestedChannel < 0x10) {
        return;
    }
    mState = 3;
    mUnk_122a = 0;
}

void WirelessManager::StartChannelTest() {
    mTestedChannel = 0;
    mMinTestResult = 0x65;
    TestNextChannel();
}

void WirelessManager::SetupScanParam() {
    SCAN_PARAM.mBuffer = SCAN_ENTRY;
    SCAN_PARAM.mChannel = sDefaultParentParam.mChannel;
    SCAN_PARAM.mUnk_06 = func_0201056c();
    SCAN_PARAM.mBssid[0] = 0xff;
    SCAN_PARAM.mBssid[1] = 0xff;
    SCAN_PARAM.mBssid[2] = 0xff;
    SCAN_PARAM.mBssid[3] = 0xff;
    SCAN_PARAM.mBssid[4] = 0xff;
    SCAN_PARAM.mBssid[5] = 0xff;
}

// The mask bit is kept in a register: an enum-typed one is not constant-propagated.
enum ChannelBit { CHANNEL_BIT_NONE, CHANNEL_BIT };
void WirelessManager::SelectNextScanChannel() {
    u16 mask = func_0201042c();
    if (mask == 0) { return; }
    int start;
    ChannelBit one;
    int i;
    i = 0;
    start = SCAN_PARAM.mChannel;
    one = CHANNEL_BIT;
    for (; i < 16; i++) {
        int ch = (start + i) % 16;
        if (mask & (one << ch)) { SCAN_PARAM.mChannel = ch + 1; return; }
    }
}

void WirelessManager::RequestScan() {
    if (func_020108f4((void *) ScanCallback, &SCAN_PARAM) != 2) {
        mUnk_122a = 6;
    } else {
        mUnk_122a = 1;
    }
}

void WirelessManager::Join(u32 unk1) {
    MI_CpuCopy8(&mScanSnapshot[unk1], sEntryBuffer, 0xc0);
    if (func_020109e0((void *) JoinCallback, sEntryBuffer, mUnk_1231, 1, 0) != 2) {
        mUnk_122a = 6;
    } else {
        mUnk_122a = 4;
    }
}

void WirelessManager::StoreScanResult() {
    WmEntry *scan = (WmEntry *) sEntryBuffer;
    u8 i = 0;
    do {
        if (mScanResultTimers[i] != 0) {
            bool m4 = false;
            bool m3 = false;
            bool m2 = false;
            bool m1 = false;
            bool m0 = false;
            if (scan->mBssid[0] == mScanResults[i].mBssid[0]) { if (scan->mBssid[1] == mScanResults[i].mBssid[1]) { m0 = true; } }
            if (m0) { if (scan->mBssid[2] == mScanResults[i].mBssid[2]) { m1 = true; } }
            if (m1) { if (scan->mBssid[3] == mScanResults[i].mBssid[3]) { m2 = true; } }
            if (m2) { if (scan->mBssid[4] == mScanResults[i].mBssid[4]) { m3 = true; } }
            if (m3) { if (scan->mBssid[5] == mScanResults[i].mBssid[5]) { m4 = true; } }
            if (m4) {
                MI_CpuCopy8(sEntryBuffer, &mScanResults[i], 0xc0);
                mScanResultTimers[i] = 300;
                return;
            }
        }
        i++;
    } while (i < WM_ENTRY_COUNT);
    i = 0;
    do {
        if (mScanResultTimers[i] == 0) {
            MI_CpuCopy8(sEntryBuffer, &mScanResults[i], 0xc0);
            mScanResultTimers[i] = 300;
            return;
        }
        i++;
    } while (i < WM_ENTRY_COUNT);
}

void WirelessManager::UpdateScanResultTimers() {
    for (u8 i = 0; i < WM_ENTRY_COUNT; i++) {
        if (mScanResultTimers[i] != 0) {
            mScanResultTimers[i]--;
        }
    }
}

void WirelessManager::SetRecvCallback() {
    func_02010224(0xc, (void *) RecvCallback, 0);
}

void WirelessManager::func_0202ea34() {
    func_02010c1c((void *) func_0202e288, data_0220f860, 0x1c0, data_0220f700, 0xa0, 1);
    mUnk_122a = 4;
}

void WirelessManager::SendNext() {
    if (mUnk_122b == 0 || mSendWriteIndex == mSendReadIndex) {
        return;
    }
    mUnk_122b = 0;
    MI_CpuCopy8(&SEND_QUEUE[mSendReadIndex], &data_0220f600, 4);
    if (Send(&data_0220f600, 4) == 0) {
        return;
    }
    mSendReadIndex = (mSendReadIndex + 1) % 4;
}

u32 WirelessManager::Send(void *unk1, u32 unk2) {
    if (func_02010d3c((void *) SendCallback, 0, unk1, unk2, 0xffff, 0xc, 2) == 2) {
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
    sCommandWord = 2;
    Send(&sCommandWord, 4);
}

void WirelessManager::func_0202ed28() {}

void WirelessManager::func_0202ed2c() {
    mState = 0;
}

u32 WirelessManager::Start() {
    if (mState != 0) {
        return 0;
    }
    func_0202e4ec();
    mRecvWriteIndex = 0;
    mUnk_124a = 0;
    mSendWriteIndex = 0;
    mSendReadIndex = 0;
    mUnk_1220 = 0;
    MI_CpuFill8(data_0220fc20, 0, 0x820);
    MI_CpuFill8(data_0220fa20, 0, 0x200);
    MI_CpuFill8(data_0220f620, 0, 0x20);
    mParentCallback = 0;
    mUnk_124d = 0;
    mUnk_1230 = 0;
    return 1;
}

u32 WirelessManager::Stop() {
    u8 mode = mState;
    if (mode == 0) {
        return 1;
    }
    if (mode == 8) {
        return 1;
    }
    func_0202e544();
    mState = 8;
    return 1;
}

u32 WirelessManager::StartParent(u32 unk1, void *unk2, u32 unk3, u32 unk4) {
    if (mState == 0) {
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
    MI_CpuFill8(sParentData, 0, 0x70);
    if (unk2 != 0 && unk3 != 0) {
        MI_CpuCopy8(unk2, sParentData, unk3);
    }
    func_0202e544();
    mParentCallback = unk4;
    mState = 2;
    mUnk_122f = 0;
    return 1;
}

u32 WirelessManager::StartScan() {
    if (mState == 0) {
        return 0;
    }
    MI_CpuFill8(mScanResultTimers, 0, 0x18);
    func_0202e544();
    mState = 4;
    return 1;
}

u32 WirelessManager::StartJoin(u32 unk1, void *unk2, u32 unk3) {
    if (mState == 0) {
        return 0;
    }
    if (unk3 > 0x18) {
        unk3 = 0x18;
    }
    MI_CpuFill8(mUnk_1231, 0, 0x18);
    if (unk2 != 0 && unk3 != 0) {
        MI_CpuCopy8(unk2, mUnk_1231, unk3);
    }
    func_0202e544();
    mState = 5;
    mJoinIndex = unk1;
    return 1;
}

u32 WirelessManager::TakeScanSnapshot() {
    if (mState != 4) {
        return 0;
    }
    vu16 *ime = (vu16 *) 0x04000208;
    u16 old   = *ime;
    *ime      = 0;
    u8 n = 0;
    u8 i = 0;
    for (i = 0; i < WM_ENTRY_COUNT; i++) {
        if (mScanResultTimers[i] != 0) {
            MI_CpuCopy8(&mScanResults[i], &mScanSnapshot[n++], WM_ENTRY_SIZE);
        }
    }
    u16 tmp = *ime;
    *ime    = old;
    return n;
}

u32 WirelessManager::QueueSend(void *unk1) {
    MI_CpuCopy8(unk1, &SEND_QUEUE[mSendWriteIndex], 4);
    mSendWriteIndex = (mSendWriteIndex + 1) % 4;
    SendNext();
    return 1;
}

u32 WirelessManager::func_0202f078(void *unk1) {
    if ((mState != 6) & (mState != 7)) {
        return 0;
    }
    MI_CpuCopy8(unk1, data_0220f620, 0x20);
    return 1;
}

u8 *WirelessManager::func_0202f0cc(u32 unk1) {
    if ((mState != 6) & (mState != 7)) {
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
    if (!((mState == 6) | (mState == 7))) {
        return;
    }
    if (unk1 != mUnk_122f && mState == 6) {
        func_0202ec9c(unk1);
    }
    sCommandWord = unk1 != 0 ? 1 : 0;
    Send(&sCommandWord, 4);
}

u32 WirelessManager::func_0202f238() {
    return mUnk_1230;
}

void WirelessManager::Update() {
    if (mUnk_122a == 6 && mState != 9) {
        func_0202e544();
        if (mState != 8) {
            mState = 9;
        }
    }
    switch (mState) {
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
                StartChannelTest();
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
                    SetupScanParam();
                    RequestScan();
                case 1:
                    UpdateScanResultTimers();
                    break;
            }
            break;
        case 5:
            mUnk_124d = func_0201044c();
            if (mUnk_122a == 0) {
                Join(mJoinIndex);
            }
            break;
        case 6:
            mUnk_124d = func_0201044c();
            if (mUnk_122f != 0) {
                mUnk_124e = 1;
                break;
            }
            func_0202ebc4();
            SendNext();
            break;
        case 7:
            mUnk_124d = func_0201044c();
            if (mUnk_122f != 0) {
                mUnk_124e = 1;
                break;
            }
            func_0202ebc4();
            SendNext();
            break;
        case 9:
            mUnk_124d = func_0201044c();
            mUnk_122a = 5;
            break;
    }
}
