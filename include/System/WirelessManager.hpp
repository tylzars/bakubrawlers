#pragma once

#include "global.h"
#include "types.h"

// Argument handed to the callbacks of the wireless library (its WM callback structures: the first two
// words are the API id and the error code, the rest depends on the call that completed).
struct WmCallbackArg {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u16 mResult; // 0 = success
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06;
    /* 08 */ u16 mUnk_08;
    /* 0a */ u16 mUnk_0a;
    /* 0c */ u8 *mUnk_0c;
    /* 10 */ u16 mUnk_10;
    /* 12 */ u16 mUnk_12;
    /* 14 */ u8 mUnk_14[24];
};

#define WM_ENTRY_SIZE 0xc0 // one scanned/listed network record (the wireless library's WMBssDesc with its game info)
#define WM_ENTRY_COUNT 12

// Status of the last wireless library call (WirelessManager::mUnk_122a, also written as data_020c4030.mUnk_22a).
// Update starts the current mState's next call when it is WM_CALL_DONE, and on WM_CALL_FAILED (outside mState 9) calls
// func_0202e544 and sets mState 9 (unless it is 8). 2 (set with mState 6 by the parent callback's event 7) is
// never read and stays a literal.
#define WM_CALL_DONE             0 // the last call completed (its callback succeeded, or TestNextChannel ran out of
                                   // channels) and nothing is pending
#define WM_CALL_SCAN_REQUESTED   1 // RequestScan issued the scan; Update (mState 4) then runs UpdateScanResultTimers
#define WM_CALL_JOINED           3 // func_0202e288 sets it when it moves mState 5 (Join) to 7; JoinCallback then skips
                                   // func_0202ea34
#define WM_CALL_PENDING          4 // a call was issued and its callback has not run yet
#define WM_CALL_STOPPED          5 // StopCallback succeeded (mState 0); Update in mState 9 sets it every frame
#define WM_CALL_FAILED           6 // a call or its callback failed, or the mExchangeTimer countdown ran out. BattleShoot and
                                   // BattleCommand write it on purpose (mRequest 2 while data_020d433c.mWireless is
                                   // set) to send Update down this path; nothing has failed there

// Scan result of one beacon (the wireless library's WMBssDesc followed by its game info), as far as it is read.
struct WmEntry {
    /* 00 */ PAD(0x00, 0x04);
    /* 04 */ u8 mBssid[6];
    /* 0a */ PAD(0x0a, 0x3c);
    /* 3c */ u16 mUnk_3c;
    /* 3e */ PAD(0x3e, 0x44);
    /* 44 */ u32 mGameId;
    /* 48 */ PAD(0x48, 0x4b);
    /* 4b */ u8 mUnk_4b;
    /* 4c */ PAD(0x4c, 0xc0);
};


// Singleton (instance 0x020c3030, vtable 0x020a5738), constructed on first use. It drives the DS wireless library
// (local multiplayer: scanning for parents, joining; port 0xc carries 4-byte messages and command words (QueueSend /
// SendNext / Send, RecvCallback); port 0xd carries the per-frame exchange of 0x20-byte blocks (SetSendBlock,
// UpdateExchange, GetRecvBlock)); the library calls at
// 0x0200f8e8-0x02011c34 are the wireless (WM) library, not CARD.
class WirelessManager {
public:
    /* 0000 */ // vptr
    /* 0004 */ u32 mParentCallback; // set by StartParent, called with (arg->mUnk_10, arg->mUnk_14) on event 7 of the parent
                                    // callback ParentEventCallback; the only StartParent call passes 0, so it never runs in this build
    /* 0008 */ WmEntry mScanResults[WM_ENTRY_COUNT]; // collected scan results
    /* 0908 */ WmEntry mScanSnapshot[WM_ENTRY_COUNT]; // snapshot handed to the game; ArenaScene reads it as data_020c3938
    /* 1208 */ u16 mScanResultTimers[WM_ENTRY_COUNT]; // per mScanResults slot: 300 when stored, counted down by
                                                      // UpdateScanResultTimers; 0 = slot free
    /* 1220 */ u16 mSendBlockIndex;
    /* 1222 */ u16 mTestedChannel; // 1-based channel TestNextChannel last passed to func_020119ec; StartChannelTest resets it to 0
    /* 1224 */ u16 mMinTestResult;
    /* 1226 */ u16 mParentChannel;
    /* 1228 */ volatile u8 mState; // 0 off, 1 started, 2 parent starting, 3 channels measured, 4 scanning, 5 joining,
                                   // 6 connected as parent, 7 joined, 8 stopping, 9 failed; also read as
                                   // data_020c4030.mUnk_228 / data_020c4258 (IsWirelessMode tests 6 and 7)
    /* 1229 */ u8 mJoinIndex;
    /* 122a */ u8 mUnk_122a; // WM_CALL_* status of the last library call; 2 is written with mState 6 by ParentEventCallback
                             // and never read. Also written as data_020c4030.mUnk_22a
    /* 122b */ u8 mSendIdle;
    /* 122c */ u8 mExchangeStarted;
    /* 122d */ u8 mMaxJoinedCount;
    /* 122e */ u8 mJoinedCount;
    /* 122f */ u8 mPaused;
    /* 1230 */ u8 mUnk_1230;
    /* 1231 */ u8 mJoinData[24];
    /* 1249 */ u8 mRecvWriteIndex; // the received ring (sWmWorkB.mRecv) is written but never read in this build
    /* 124a */ u8 mUnk_124a;
    /* 124b */ u8 mSendWriteIndex;
    /* 124c */ u8 mSendReadIndex;
    /* 124d */ u8 mUnk_124d;
    /* 124e */ u8 mHasNewRecvBlocks;
    /* 124f */ u8 mUnk_124f;
    /* 1250 */ u8 mUnk_1250;
    /* 1251 */ PAD(0x1251, 0x1254);
    /* 1254 */ s32 mExchangeTimer;
    /* 1258 */

    virtual void func_0202ed28();
    virtual ~WirelessManager() {}

    static WirelessManager &Get() {
        static WirelessManager instance;
        return instance;
    }

    void RequestStart();
    void RequestStop();
    void func_0202e544();
    void RequestParent();
    void TestNextChannel();
    void StartChannelTest();
    void SetupScanParam();
    void SelectNextScanChannel();
    void RequestScan();
    void Join(u32 unk1);
    void StoreScanResult();
    void UpdateScanResultTimers();
    void SetRecvCallback();
    void func_0202ea34();
    void SendNext();
    u32 Send(void *unk1, u32 unk2);
    void StartExchange();
    void UpdateExchange();
    void func_0202ec74(u32 unk1);
    void SetPaused(u32 unk1);
    void func_0202ed00();
    void Init();
    void func_0202f1a4(u32 unk1);
    u32 func_0202f238();
    void Update();

    u32 Start();
    u32 Stop();
    u32 StartParent(u32 unk1, void *unk2, u32 unk3, u32 unk4);
    u32 StartJoin(u32 unk1, void *unk2, u32 unk3);
    u32 TakeScanSnapshot();
    u32 StartScan();
    u32 QueueSend(void *unk1);
    u32 SetSendBlock(void *unk1);
    u8 *GetRecvBlock(u32 unk1);
    u32 GetRecvBlockCount();
    void func_0202f184();
};

struct UnkStruct_020c4030 {
    /* 000 */ PAD(0x000, 0x228);
    /* 228 */ u8 mUnk_228; // the same byte as WirelessManager::mState
    /* 229 */ PAD(0x229, 0x22a);
    /* 22a */ u8 mUnk_22a; // the same byte as WirelessManager::mUnk_122a
    /* 22b */ PAD(0x22b, 0x24d);
    /* 24d */ u8 mUnk_24d; // the same byte as WirelessManager::mUnk_124d
};

extern UnkStruct_020c4030 data_020c4030;
extern u8 data_020c4258; // the same byte as data_020c4030.mUnk_228 (WirelessManager::mState)
// TRUE for WirelessManager::mState 6 (connected as parent) and 7 (joined).
static inline BOOL IsWirelessMode(const u8 *mode) {
    switch (*mode) {
        case 6:
        case 7:
            return TRUE;
    }
    return FALSE;
}

struct UnkStruct_0220f540 {
    /* 00 */ u8 mUnk_00;
    /* 01 */ u8 mUnk_01;
    /* 02 */ PAD(0x02, 0x04);
    union {
        /* 04 */ u32 mUnk_04;
        struct {
            /* 04 */ u8 mTouchDown;
            /* 05 */ u8 mTouchX;
            /* 06 */ u8 mTouchY;
        };
        struct {
            /* 04 */ u16 mKeys;
            /* 06 */ u8 mUnk_06;
            /* 07 */ u8 mUnk_07;
            /* 08 */ u8 mUnk_08;
            /* 09 */ u8 mUnk_09;
            /* 0a */ PAD(0x0a, 0x0b);
            /* 0b */ u8 mUnk_0b;
            /* 0c */ u8 mUnk_0c;
            /* 0d */ u8 mUnk_0d;
            /* 0e */ u8 mUnk_0e;
            /* 0f */ u8 mUnk_0f;
            /* 10 */ PAD(0x10, 0x1f);
            /* 1f */ u8 mUnk_1f;
        };
    };
    /* 20 */ PAD(0x20, 0x40); // the slot-message stores at 0x1c/0x1d schedule like an object of at least 0x3c bytes
};

extern UnkStruct_0220f540 data_0220f540;
