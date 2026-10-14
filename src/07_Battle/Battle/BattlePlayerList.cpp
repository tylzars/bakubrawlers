#include "Game/UnkStruct_020d433c.hpp"
#include "Unknown/UnkStruct_02087898.hpp"
#include "Battle/GimmickTypes.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Effect.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FrameTask.hpp"
#include "System/SoundManager.hpp"
#include "System/WirelessManager.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/math.h>
#include "Battle/BattleCharaAct.hpp"
#include "Battle/BattleCommand.hpp"
#include "Battle/BattleConfrontCard.hpp"
#include "Battle/BattleCpu.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Unknown/UnkStruct_02078e14.hpp"

// Microphone singleton (instance 0x020c189c).
class MicManager {
public:
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u8 mActive;
    /* 21 */ PAD(0x21, 0x28);

    MicManager() {
        mActive = 0;
    }
    ~MicManager();

    static MicManager &Get() {
        static MicManager instance;
        return instance;
    }

    void func_0202d7a0();
    void func_0202d8cc();
    void func_0202d9ac();
};

struct BattleHistory {
    u16 mValue;
    u8 mFlag;
    u8 mPad;
    u16 mPad2;
};

struct BattleCpuRot {
    u16 mX;
    u16 mY;
    u16 mZ;
};

class BattleObj00 : public SceneBase {
public:
    u8 mPad[0x34];
    BattleObj00(u8 flag);
    virtual ~BattleObj00();
    virtual void Update();
};

class BattleOrderSelect : public SceneBase {
public:
    u8 mPad[0xc4];
    BattleOrderSelect();
    virtual ~BattleOrderSelect();
    virtual void Update();
};

class BattleResult : public SceneBase {
public:
    u8 mPad[0xac];
    BattleResult();
    virtual ~BattleResult();
    virtual void Update();
};

class BattleShoot : public SceneBase {
public:
    u8 mPad[0x61a0];
    BattleShoot(Camera *unk1, u8 unk2);
    BattleShoot(Camera *unk1, const u16 *pos, u16 unk3, u16 unk4, u8 unk5, u8 unk6, u8 unk7);
    virtual ~BattleShoot();
    virtual void Update();
};

extern "C" void func_ov007_022633f8(BattlePlayerList *list);
extern "C" void func_02038444(u32 heap, u32 a);
void func_ov007_0226a48c(BattlePlayer *player);
void func_ov007_0226a404(BattlePlayer *player, u32 index, u32 flag);
extern const u8 data_ov007_0227aa5c[];
BOOL func_0202326c(UnkStruct_020d43f0 *d);

void func_ov007_02262328(BattlePlayerList *list);
BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);
void func_ov007_02262acc(BattlePlayerList *list);
void func_ov007_022621dc();
void func_ov007_022626b8(BattlePlayerList *list, u8 index);
extern "C" BOOL func_ov007_02263150(BattlePlayerList *list);
void func_ov007_02269ee4(BattlePlayer *player, u32 reset);
u32 func_ov007_022696a0(u32 a, u32 b);
extern const BattleCpuRot data_ov007_0227aa28;
extern u8 data_020d4444[];
extern "C" void func_020383f8(u32 heap);

BattlePlayerList::BattlePlayerList()
    : SceneBase(0x3e8, 0) {
    mCamera = 0;
    mUnk_2a4 = 0;
    mUnk_2a8 = 0;
    mUnk_2ac = 0;
    mUnk_2ad = 0;
    mUnk_2ae = 0;
    mUnk_2af = 0;
    mUnk_2b0 = 0;
    if (data_020d433c.mUnk_84) {
        func_02038444(SoundManager::Get().mUnk_9c, 2);
    } else {
        SoundManager::Get().func_020245a4();
    }
    SoundManager::Get().func_020246d4(6);
    data_020d433c.mGimmicks = 0;
    data_020d433c.mEffects  = 0;
    data_020d433c.mUnk_0c   = 0;
    data_020d433c.mUnk_10   = 0;
    mUnk_2aa                = 0;
    mUnk_2ab                = 0;
    MI_CpuClearFast(&mPlayers[0], 0x28c);
    mUnk_284[0] = 0;
    mUnk_284[1] = 1;
    mUnk_284[2] = 2;
    mUnk_284[3] = 3;
    if (data_020d433c.mPlayerCount != 3 && data_020d433c.mPlayerCount != 4) {
        data_020d433c.mPlayerCount = 2;
    }
    mUnk_294 = 0;
    mUnk_295 = 0;
    if (data_020d433c.mUnk_99 != 6) {
        data_020d433c.mUnk_99 = 3;
    }
    switch (data_020d433c.mUnk_83) {
        case 5:
        case 6:
            mUnk_2ae              = data_020d433c.mUnk_83;
            data_020d433c.mUnk_83 = 0;
            data_020d433c.mUnk_85 = 0;
    }
    func_ov007_022633f8(this);
    mUnk_298 = 0;
    mUnk_29c = 0;
    MicManager::Get().func_0202d7a0();
}


BattlePlayerList::~BattlePlayerList() {
    MicManager::Get().func_0202d8cc();
    delete mCamera;
    if (data_020d433c.mUnk_0c) {
        ((SceneBase *) data_020d433c.mUnk_0c)->mFinished = 1;
    }
    if (data_020d433c.mEffects) {
        ((SceneBase *) data_020d433c.mEffects)->mFinished = 1;
    }
    if (data_020d433c.mUnk_10) {
        ((SceneBase *) data_020d433c.mUnk_10)->mFinished = 1;
    }
    BattleUnk29c *p = mUnk_29c;
    if (p) {
        delete p;
    }
    GimmickManager *g = data_020d433c.mGimmicks;
    if (g) {
        delete g;
    }
    data_020d433c.mGimmicks = 0;
    CameraManager::Get().func_02026504(0, 0, 0);
    GX_SetDispSelect(0);
}

// non-matching: 38 words differ, register naming in states 13/17 and the predicated mUnk_86 tail of state 25
void BattlePlayerList::Update() {
    u8 owner;
    u8 slot;
    u8 id;
    u8 result;
    u8 rec;
    MicManager::Get().func_0202d9ac();
    if (data_020d433c.mUnk_a8) {
        WirelessManager::Get();
        if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
            SpriteManager::Get().SetFade(1, -16);
            RequestScene(8);
            data_020d433c.mUnk_10 = 0;
            return;
        }
    }
    switch (mUnk_2a8) {
        case 0:
            func_ov007_02262328(this);
            mUnk_2a8++;
            break;
        case 1:
            if (func_ov007_022631bc(this, 0xf)) {
                data_020d433c.mGimmicks->func_ov007_0222e75c(data_020d433c.mUnk_95, (u8) data_020d433c.mUnk_96);
                if (data_020d433c.mUnk_85 != 0) {
                    mUnk_2a8 = 2;
                } else {
                    mUnk_2a8 = 4;
                }
            }
            break;
        case 2:
            func_ov007_02262acc(this);
            break;
        case 4:
            new UnkStruct_02078e14(5, mUnk_2a9);
            mContext->mUnk_12 = 0;
            mUnk_2a8++;
            break;
        case 5: {
            u8 result = mContext->mUnk_12;
            if (result != 0 && data_020d433c.mUnk_10) {
                data_020d433c.mUnk_10->func_020785d8(0);
            }
            if (result == 0) {
                return;
            }
            if (result == 2) {
                RequestScene(4);
                mFinished = 1;
            } else {
                mUnk_2a8++;
            }
            break;
        }
        case 6:
            WirelessManager::Get();
            if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                mUnk_2a8 = 8;
                return;
            }
            mUnk_2a8++;
            break;
        case 7:
            data_0220f540.mUnk_04 = MATH_Rand32(&data_020d42e8, 0);
            if (func_ov007_022631bc(this, 2)) {
                func_020219ec(*(u32 *) ((u8 *) mPlayers[0] + 0x104));
                mUnk_2a8 = 8;
            }
            break;
        case 8:
            mUnk_2b1 = MATH_Rand32(&data_020d42e8, 100);
            if (data_020d433c.mUnk_98) {
                func_ov007_02262a70(data_020d433c.mBattlePlayers, 0, 0);
                u8 i = 1;
                u8 k = 0;
                do {
                    BattlePlayerList *list = data_020d433c.mBattlePlayers;
                    if (i == list->mPlayers[0]->mUnk_f2) {
                        func_ov007_02262a70(list, 1, i);
                    } else {
                        func_ov007_02262a70(list, k + 2, i);
                        k++;
                    }
                    i++;
                } while (i < data_020d433c.mPlayerCount);
                mUnk_2a8 = 10;
                return;
            }
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                new BattleOrderSelect();
                mContext->mUnk_12 = 0;
                mUnk_2a8++;
            } else {
                u8 i = 0;
                do {
                    func_ov007_02262a70(data_020d433c.mBattlePlayers, i, i);
                    i++;
                } while (i < data_020d433c.mPlayerCount);
                mUnk_2a8 = 10;
            }
            return;
        case 9: {
            UnkStruct_SceneContext *context = mContext;
            switch ((u8) context->mUnk_12) {
                case 0:
                    return;
                case 2:
                    RequestScene(4);
                    mFinished = 1;
                    break;
                default:
                    mUnk_2a8++;
                    break;
            }
            break;
        }
        case 10: {
            u8 flag = 0;
            func_ov007_02269ee4(mPlayers[mUnk_28a], flag);
            if (data_020d433c.mUnk_85 == 1) {
                flag = 1;
            }
            new BattleObj00(flag);
            mContext->mUnk_12 = 0;
            mUnk_2a8++;
            break;
        }
        case 11:
            if (mContext->mUnk_12 == 0) {
                return;
            }
            if (data_020d433c.mUnk_85 == 1) {
                mUnk_2a8 = 0x22;
                SpriteManager::Get().StartFade(3);
                return;
            }
            mUnk_2a8++;
            mUnk_28f = 1;
            mUnk_291 = 1;
            if (data_020d433c.mUnk_8a_0 != 0) {
                func_ov007_02269ee4(mPlayers[mUnk_28a], 1);
            }
            break;
        case 12:
            new BattleCommand((u32) mCamera);
            mContext->mUnk_12 = 0;
            mUnk_2a8++;
            break;
        case 13:
            switch ((u8) mContext->mUnk_12) {
                case 2:
                    mUnk_2a8 = 0x1a;
                    break;
                case 1:
                    mUnk_2a8++;
                    ((void (*)(BattlePlayerList *)) func_ov007_022621dc)(this);
                    break;
                case 3: {
                    u8 *msg = (u8 *) mPlayers[mUnk_28a] + 0x100;
                    new BattleShoot(mCamera, (u16 *) (msg + 4), *(u16 *) (msg + 0xa), *(u16 *) (msg + 0xc), *(msg + 0xf), *(msg + 0x10), mUnk_2ae);
                    mContext->mUnk_12 = 0;
                    mUnk_2a8 = 0x11;
                    break;
                }
                case 4: {
                    BattleCpuRot rot = data_ov007_0227aa28;
                    u16 outA         = 0;
                    u16 outB         = 0;
                    u8 outC          = 0;
                    mPlayers[mUnk_28a]->mUnk_c8->func_ov007_02231e2c(&rot, &outA, &outB, &outC);
                    u8 extra = *((u8 *) mPlayers[mUnk_28a]->mUnk_c8 + 0x7e);
                    new BattleShoot(mCamera, &rot.mX, outA, outB, outC, extra, mUnk_2ae);
                    mContext->mUnk_12 = 0;
                    mUnk_2a8 = 0x11;
                    break;
                }
                case 5:
                    mUnk_2a8 = 0x1d;
                    break;
                case 6:
                    mUnk_2a8 = 0x1b;
                    break;
                case 7:
                    mUnk_2a8 = 0x24;
                    break;
            }
            break;
        case 14:
            new BattleShoot(mCamera, mUnk_2ae);
            mContext->mUnk_12 = 0;
            mUnk_2a8 = 0x11;
            break;
        case 17: {
            rec = mPlayers[mUnk_28a]->mRecord;
            result = mContext->mUnk_12;
            if (result == 0) {
                return;
            }
            u8 mode = data_020d433c.mUnk_85;
            if (mode == 2 || mode == 3 || mode == 4 || mode == 0xc) {
                if (result != 5) {
                    mUnk_2a8 = 0x22;
                    SpriteManager::Get().StartFade(3);
                    if (result != 1) {
                        if (data_020d433c.mUnk_85 == 0xc && mUnk_2b0 == 0) {
                            data_020d433c.mUnk_86++;
                        } else {
                            data_020d433c.mUnk_86 = 0;
                        }
                    } else {
                        data_020d433c.mUnk_86++;
                    }
                    return;
                }
                if (mode == 0xc) {
                    mUnk_2af = 0;
                } else {
                    mUnk_2af = 2;
                }
            } else if (mode == 7 || mode == 8) {
                if (result != 5) {
                    if (result == 1) {
                        mUnk_2a8 = 0x22;
                        SpriteManager::Get().StartFade(3);
                        data_020d433c.mUnk_86++;
                        return;
                    } else if (mUnk_2a9 == mUnk_28a) {
                        data_020d433c.mUnk_86 = 0;
                        data_020d433c.mUnk_84++;
                    }
                } else {
                    mUnk_2af = 0;
                }
            }
            switch (mUnk_2ac) {
                case 1:
                    mUnk_2ad = 1;
                    mUnk_2ac = 0;
                    break;
                case 2:
                    if (mUnk_2a9 == mUnk_28a) {
                        mUnk_2a8 = 0xe;
                    } else {
                        mUnk_2a8 = 0xc;
                        mUnk_28f = 0;
                    }
                    mUnk_2ac = 0;
                    if (result == 3) {
                        u8 a  = mUnits[mUnk_28d].mUnk_11;
                        u8 m3 = mUnk_1c[a].mUnk_3;
                        u8 m4 = mUnk_1c[a].mUnk_4;
                        func_ov007_02262828(this, mUnk_28e, 0);
                        mUnk_1f8[m3][m4] = mUnk_28d + 1;
                    } else {
                        func_ov007_02262828(this, mUnk_28d, 0);
                    }
                    return;
                case 3: {
                    mUnk_2ac = 0;
                    owner = mUnits[mUnk_28d].mUnk_0f;
                    if (result != 3) {
                        slot = mUnits[mUnk_28d].mUnk_0e;
                    } else {
                        owner = mUnits[mUnk_28e].mUnk_0f;
                        slot  = mUnits[mUnk_28e].mUnk_0e;
                    }
                    BattlePlayer *p = mPlayers[owner];
                    p->mUnk_fe = 1;
                    p->mUnk_ee++;
                    func_ov007_022626b8(this, mUnits[mUnk_28d].mUnk_11);
                    BattleRecord *used = &mPlayers[owner]->mRecords[slot];
                    mUnk_2aa           = func_ov007_022696a0(used->mUnk_08, used->mUnk_09);
                    func_ov007_02262828(this, mUnk_28d, 2);
                    if (result == 3) {
                        func_ov007_02262828(this, mUnk_28e, 2);
                    }
                    mUnk_2a8 = 0x16;
                    if (data_020d433c.mUnk_7a == 0) {
                        return;
                    }
                    if (mUnk_2a9 != mUnk_28a) {
                        return;
                    }
                    id = mPlayers[mUnk_2a9]->mRecords[rec].mUnk_09;
                    if (data_020d43f0[0].mUnk_4e < 0xffff) {
                        data_020d43f0[0].mUnk_4e++;
                    }
                    if (data_020d4444[id] < 99) {
                        data_020d4444[id]++;
                    }
                    if (data_020d43f0[0].mUnk_50 < 0xffff) {
                        data_020d43f0[0].mUnk_50++;
                    }
                    return;
                }
            }
            switch (result) {
                case 1:
                    if (data_020d433c.mUnk_83 == 0 && data_020d433c.mUnk_7a != 0 && mUnk_2a9 == mUnk_28a) {
                        id = mPlayers[mUnk_2a9]->mRecords[rec].mUnk_09;
                        if (data_020d43f0[0].mUnk_4e < 0xffff) {
                            data_020d43f0[0].mUnk_4e++;
                        }
                        if (data_020d4444[id] < 99) {
                            data_020d4444[id]++;
                        }
                    }
                    {
                        BattlePlayer *current = mPlayers[mUnk_28a];
                        current->mRecords[rec].mUnk_04 = current->mRecords[rec].mUnk_0b * 10;
                        current->mRecords[rec].mUnk_06 = 0;
                    }
                    mUnk_2a8 = 0x12;
                    break;
                case 2:
                    mUnk_2a8 = 0x1a;
                    if (data_020d433c.mUnk_83 != 0) {
                        return;
                    }
                    if (data_020d433c.mUnk_7a == 0) {
                        return;
                    }
                    if (mUnk_2a9 != mUnk_28a) {
                        return;
                    }
                    id = mPlayers[mUnk_2a9]->mRecords[rec].mUnk_09;
                    if (data_020d43f0[0].mUnk_4e < 0xffff) {
                        data_020d43f0[0].mUnk_4e++;
                    }
                    if (data_020d4444[id] < 99) {
                        data_020d4444[id]++;
                    }
                    if (data_020d43f0[0].mUnk_50 < 0xffff) {
                        data_020d43f0[0].mUnk_50++;
                    }
                    break;
                case 3: {
                    u8 ownerA = mUnits[mUnk_28d].mUnk_0f;
                    u8 ownerB = mUnits[mUnk_28e].mUnk_0f;
                    if (data_020d433c.mUnk_83 == 0 && data_020d433c.mUnk_7a != 0 && mUnk_2a9 == mUnk_28a) {
                        id = mPlayers[mUnk_2a9]->mRecords[rec].mUnk_09;
                        if (data_020d43f0[0].mUnk_4e < 0xffff) {
                            data_020d43f0[0].mUnk_4e++;
                        }
                        if (data_020d4444[id] < 99) {
                            data_020d4444[id]++;
                        }
                    }
                    if (ownerA == ownerB) {
                        BattlePlayer *p = mPlayers[ownerA];
                        p->mUnk_fe = 1;
                        p->mUnk_ee++;
                        mUnk_2a8 = 0x14;
                        BattleRecord *recs = mPlayers[ownerA]->mRecords;
                        mUnk_2aa = func_ov007_022696a0(recs[mUnits[mUnk_28d].mUnk_0e].mUnk_08, recs[mUnits[mUnk_28d].mUnk_0e].mUnk_09);
                        recs    = mPlayers[ownerA]->mRecords;
                        mUnk_2ab = func_ov007_022696a0(recs[mUnits[mUnk_28e].mUnk_0e].mUnk_08, recs[mUnits[mUnk_28e].mUnk_0e].mUnk_09);
                        func_ov007_022626b8(this, mUnits[mUnk_28d].mUnk_11);
                        func_ov007_02262828(this, mUnk_28d, 2);
                        func_ov007_02262828(this, mUnk_28e, 2);
                        if (data_020d433c.mUnk_83 != 0) {
                            return;
                        }
                        if (data_020d433c.mUnk_7a == 0) {
                            return;
                        }
                        if (mUnk_2a9 != mUnk_28a) {
                            return;
                        }
                        if (data_020d43f0[0].mUnk_52 < 0x270f) {
                            data_020d43f0[0].mUnk_52++;
                        }
                        if (data_020d43f0[0].mUnk_50 < 0xffff) {
                            data_020d43f0[0].mUnk_50++;
                        }
                    } else {
                        mUnk_2a8 = 0x18;
                        if (data_020d433c.mUnk_83 != 0) {
                            return;
                        }
                        if (data_020d433c.mUnk_7a == 0) {
                            return;
                        }
                        if (mUnk_2a9 != mUnk_28a) {
                            return;
                        }
                        if (data_020d43f0[0].mUnk_50 < 0xffff) {
                            data_020d43f0[0].mUnk_50++;
                        }
                    }
                    break;
                }
                case 4: {
                    u8 slot  = mUnits[mUnk_28e].mUnk_0f;
                    u8 owner = mUnits[mUnk_28d].mUnk_0f;
                    BattlePlayer *p = mPlayers[slot];
                    p->mUnk_fe = 1;
                    p->mUnk_ee++;
                    mUnk_2a8 = 0x16;
                    func_ov007_022626b8(this, mUnits[mUnk_28d].mUnk_11);
                    BattlePlayer *q      = mPlayers[slot];
                    BattleRecord *source = &mPlayers[owner]->mRecords[mUnits[mUnk_28d].mUnk_0e];
                    u8 flagValue  = source->mUnk_09;
                    u16 wordValue = source->mUnk_04;
                    ((BattleHistory *) ((u8 *) q + 0x84))[q->mUnk_f4].mValue = wordValue;
                    ((BattleHistory *) ((u8 *) q + 0x84))[q->mUnk_f4].mFlag  = flagValue;
                    q->mUnk_f4++;
                    BattleRecord *target = &mPlayers[slot]->mRecords[mUnits[mUnk_28e].mUnk_0e];
                    mUnk_2aa             = func_ov007_022696a0(target->mUnk_08, target->mUnk_09);
                    func_ov007_02262828(this, mUnk_28d, 2);
                    func_ov007_02262828(this, mUnk_28e, 2);
                    if (data_020d433c.mUnk_83 != 0) {
                        return;
                    }
                    if (data_020d433c.mUnk_7a == 0) {
                        return;
                    }
                    if (mUnk_2a9 != mUnk_28a) {
                        return;
                    }
                    id = mPlayers[mUnk_2a9]->mRecords[rec].mUnk_09;
                    if (data_020d43f0[0].mUnk_4e < 0xffff) {
                        data_020d43f0[0].mUnk_4e++;
                    }
                    if (data_020d4444[id] < 99) {
                        data_020d4444[id]++;
                    }
                    if (data_020d43f0[0].mUnk_5c < 0x270f) {
                        data_020d43f0[0].mUnk_5c++;
                    }
                    if (data_020d43f0[0].mUnk_50 < 0xffff) {
                        data_020d43f0[0].mUnk_50++;
                    }
                    break;
                }
                case 6:
                    mUnk_2a8 = 0x24;
                    break;
                default:
                    mUnk_2a8 = 0xc;
                    break;
            }
            break;
        }
        case 18:
            new BattleCharaAct(mCamera, mUnk_28a, 4, 0, 0);
            mContext->mUnk_12 = 0;
            mUnk_2a8++;
            break;
        case 19:
            if ((u8) mContext->mUnk_12) {
                mUnk_2a8 = 0x1a;
            }
            break;
        case 20:
            new BattleCharaAct(mCamera, mUnk_28a, 8, mUnk_2aa, mUnk_2ab);
            mContext->mUnk_12 = 0;
            mUnk_2a8++;
            break;
        case 21:
            if ((u8) mContext->mUnk_12 == 0) {
                return;
            }
            mUnk_2a8 = 0x19;
            mContext->mUnk_12 = 1;
            break;
        case 22:
            new BattleCharaAct(mCamera, mUnk_28a, 9, mUnk_2aa, 0);
            mContext->mUnk_12 = 0;
            mUnk_2a8++;
            break;
        case 23:
            if ((u8) mContext->mUnk_12 == 0) {
                return;
            }
            mUnk_2a8 = 0x19;
            mContext->mUnk_12 = 1;
            break;
        case 24:
            if (func_ov007_022631bc(this, 0xf)) {
                new BattleConfrontCard(-1);
                mContext->mUnk_12 = 0;
                mUnk_2a8++;
            }
            break;
        case 25:
            if (mContext->mUnk_12 == 0) {
                return;
            }
            mUnk_2b1 = MATH_Rand32(&data_020d42e8, 100);
            if (data_020d433c.mUnk_83 == 2 || data_020d433c.mUnk_83 == 4) {
                SoundManager::Get().func_020245a4();
                SoundManager::Get().func_020245d4(1);
                func_020383f8(SoundManager::Get().mUnk_9c);
                mUnk_2a8 = 0x22;
                SpriteManager::Get().StartFade(3);
                if (mUnk_2b0 != 0) {
                    data_020d433c.mUnk_86 = 0;
                } else {
                    data_020d433c.mUnk_86++;
                }
                return;
            } else if (func_ov007_02263150(this)) {
                mUnk_2a8 = 0x1d;
            } else {
                mUnk_2a8 = 0x1a;
            }
            break;
        case 26: {
            mUnk_28f = 1;
            mUnk_291 = 1;
            mUnk_293 = 0;
            u8 flag  = 0;
            if (mUnk_2ad != 0) {
                mUnk_2ad = 0;
                flag     = 1;
            } else {
                mUnk_288++;
                if (data_020d433c.mGimmicks) {
                    data_020d433c.mGimmicks->func_ov007_0222e25c();
                }
            }
            u8 idx = (mUnk_288 - 1) % data_020d433c.mPlayerCount;
            if (data_020d433c.mUnk_98) {
                if (idx == 1) {
                    idx = 2;
                } else if (idx == 2) {
                    idx = 1;
                }
            }
            mUnk_28a = mUnk_284[idx];
            mUnk_2a8 = 0xc;
            func_ov007_02269ee4(mPlayers[mUnk_28a], flag);
            u8 found;
            u8 i = 0;
            found = 0;
            do {
                if (mUnk_1c[i].mUnk_2 != 0) {
                    found = 1;
                    break;
                }
                i++;
            } while (i < 12);
            if (!found) {
                mUnk_2a8 = 0x1b;
            }
            u8 current = mUnk_28a;
            u8 j       = 0;
            do {
                if (current == mUnits[j].mUnk_0f && mUnits[j].mUnk_12 != 0) {
                    data_020d433c.mBattlePlayers->mUnk_292 = 2;
                    return;
                }
                j++;
            } while (j < 12);
            break;
        }
        case 27:
            new BattleObj00(1);
            mContext->mUnk_12 = 0;
            mUnk_2a8++;
            break;
        case 28:
            if (mContext->mUnk_12 == 0) {
                return;
            }
            mUnk_2a8 = 0xc;
            if (data_020d433c.mBattlePlayers->mPlayers[mUnk_28a]->mUnk_f8 == 0) {
                data_020d433c.mBattlePlayers->mUnk_28f = 0;
            }
            break;
        case 29:
            if (mUnk_2ae != 0) {
                SoundManager::Get().func_02024688(1);
            }
            new BattleResult();
            mContext->mUnk_12 = 0;
            mUnk_2a8++;
            break;
        case 30:
            if (mContext->mUnk_12 == 0) {
                return;
            }
            if (!func_ov007_022631bc(this, 0x10)) {
                return;
            }
            mUnk_2a8 = 0x1f;
            if (mUnk_2ae == 0) {
                return;
            }
            SoundManager::Get().func_020245a4();
            SoundManager::Get().func_020245d4(1);
            func_020383f8(SoundManager::Get().mUnk_9c);
            break;
        case 31:
            data_020d433c.mUnk_87   = 1;
            data_020d433c.mUnk_88_4 = 0;
            data_020d433c.mUnk_88_0 = 0;
            if (mUnk_2ae != 0) {
                data_020d433c.mUnk_83 = mUnk_2ae;
                RequestScene(10);
                mFinished = 1;
                return;
            }
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228) || data_020d433c.mUnk_7b == 8) {
                if (mContext->mUnk_12 == 1) {
                    data_020d433c.mUnk_88_4 = 1;
                }
                mUnk_2a8++;
            } else {
                u8 next = data_020d433c.mUnk_7b;
                if (next != 0) {
                    if (data_020d433c.mUnk_7a != 0 && next == 0xd && data_020d433c.mUnk_7d == 3) {
                        RequestScene(0xb);
                        mFinished = 1;
                        return;
                    }
                    data_020d433c.mUnk_7b = 0;
                    RequestScene(next);
                    mFinished = 1;
                } else {
                    RequestScene(4);
                    mFinished = 1;
                }
            }
            break;
        case 32: {
            UnkStruct_SceneContext *context = mContext;
            switch (context->mUnk_12) {
                case 1:
                    if (data_020d433c.mUnk_a8) {
                        data_020d433c.mUnk_89 = 1;
                        WirelessManager::Get().func_0202edd0();
                        mUnk_2a8++;
                    } else {
                        data_020d433c.mUnk_89 = 0;
                        RequestScene(8);
                        mFinished = 1;
                    }
                    break;
                case 2:
                    RequestScene(5);
                    mFinished = 1;
                    break;
                case 3:
                    data_020d433c.mUnk_89 = 1;
                    RequestScene(8);
                    mFinished = 1;
                    break;
            }
            break;
        }
        case 33:
            WirelessManager::Get();
            if (data_020c4030.mUnk_228 != 0) {
                return;
            }
            data_020d433c.mUnk_a8 = 0;
            data_020d433c.mUnk_89 = 0;
            RequestScene(8);
            mFinished = 1;
            break;
        case 34:
            mUnk_2a4->func_02087cc0();
            mUnk_2a8++;
            break;
        case 35:
            if (!SpriteManager::Get().IsFading()) {
                RequestScene(10);
                mFinished = 1;
            }
            break;
        case 36:
            SpriteManager::Get().SetFade(1, -16);
            SoundManager::Get().func_02024688(1);
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228) || data_020d433c.mUnk_7b == 8) {
                mContext->mUnk_12       = 1;
                data_020d433c.mUnk_88_4 = 0;
                data_020d433c.mUnk_88_0 = 1;
                data_020d433c.mUnk_87   = 1;
                mUnk_2a8                = 0x20;
            } else {
                RequestScene(9);
                mFinished = 1;
            }
            break;
    }
}

// Record of a command cell: u8 table helpers below
void func_ov007_022621dc() {
    SpriteManager::Get().func_02026dec(1, 7);
    GX_SetDispSelect(1);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetVisiblePlane(0x11);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    GXS_SetGraphicsMode(5);
    G2S_SetBG0Control(0, 1, 0, 1, 1);
    G2S_SetBG1Control(0, 0, 1, 4, 0);
    G2_SetBG0Offset(0, 0);
    G2_BlendNone();
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_BlendNone();
    G2S_SetBG0Priority(2);
    G2S_SetBG1Priority(1);
}

void func_ov007_02262328(BattlePlayerList *list) {
    list->mCamera = new Camera();
    list->mCamera->SetUp(0, 0x1000, 0);
    list->mCamera->SetTarget(0, 0, 0);
    list->mCamera->SetClip(0x1000, 0x1388000);
    list->mCamera->func_02026004(0x2aaa);
    list->mCamera->SetPosition(0, 0, list->mCamera->mUnk_34);
    data_020d433c.mEffects = new EffectManager();
    data_020d433c.mUnk_0c  = new UnkStruct_020d433c_0c();
    data_020d433c.mGimmicks = new GimmickManager();
    list->mUnk_29c = new BattleUnk29c();
    if (data_020d433c.mUnk_a8) {
        data_020d433c.mUnk_10 = new UnkStruct_02078290(0);
    }
    u8 i = 0;
    do {
        list->mPlayers[i] = new BattlePlayer(i);
        i++;
    } while (i < data_020d433c.mPlayerCount);
    WirelessManager::Get();
    list->mUnk_2a9 = IsWirelessMode(&data_020c4030.mUnk_228) ? data_020d433c.mUnk_ae : 0;
}
u16 *func_ov007_02262510(BattlePlayerList *list, u8 index) {
    return &list->mPlayers[list->mUnk_1c[index].mUnk_1]->Slot54(list->mUnk_1c[index].mUnk_0);
}

BattleRecord *func_ov007_02262530(BattlePlayerList *list, u8 row, u8 col) {
    u8 index = (u8) (list->mUnk_1f8[row][col] - 1);
    return &list->mPlayers[list->mUnits[index].mUnk_0f]->mRecords[list->mUnits[index].mUnk_0e];
}

static const s8 sNeighbors[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

BOOL func_ov007_02262574(BattlePlayerList *list, u8 row, u8 col) {
    if (list->mUnk_294 == 0 && row >= 3 && row <= 4 && col >= 2 && col <= 3) {
        return TRUE;
    }
    if (list->mUnk_16c[row][col] != 0) {
        return FALSE;
    }
    for (u8 i = 0; i < 4; i++) {
        u8 r = (u8) (row + sNeighbors[i][0]);
        u8 c = (u8) (col + sNeighbors[i][1]);
        if (r < 14 && c < 10 && list->mUnk_16c[r][c] > 0) {
            return TRUE;
        }
    }
    return FALSE;
}


void func_ov007_02262638(BattlePlayerList *list, u8 row, u8 col, u8 player, u8 slot) {
    for (u8 i = 0; i < 12; i++) {
        if (list->mUnk_1c[i].mUnk_2 == 0) {
            list->mUnk_1c[i].mUnk_2 = 1;
            list->mUnk_1c[i].mUnk_3 = row;
            list->mUnk_1c[i].mUnk_4 = col;
            list->mUnk_1c[i].mUnk_1 = player;
            list->mUnk_1c[i].mUnk_0 = slot;
            list->mUnk_16c[row][col] = i + 1;
            func_ov007_0226a404(list->mPlayers[player], slot, 1);
            list->mUnk_294++;
            return;
        }
    }
}

void func_ov007_022626b8(BattlePlayerList *list, u8 index) {
    func_ov007_0226a404(list->mPlayers[list->mUnk_1c[index].mUnk_1], list->mUnk_1c[index].mUnk_0, 2);
    list->mUnk_1c[index].mUnk_2                                      = 0;
    list->mUnk_16c[list->mUnk_1c[index].mUnk_3][list->mUnk_1c[index].mUnk_4] = 0;
    list->mUnk_294--;
}

void func_ov007_02262714(BattlePlayerList *list, u8 index, u8 player, u8 slot) {
    func_ov007_0226a404(list->mPlayers[list->mUnk_1c[index].mUnk_1], list->mUnk_1c[index].mUnk_0, 0);
    list->mUnk_1c[index].mUnk_1 = player;
    list->mUnk_1c[index].mUnk_0 = slot;
    func_ov007_0226a404(list->mPlayers[player], slot, 1);
}

u8 func_ov007_02262768(BattlePlayerList *list, const VecFx32 *pos, u16 value, u8 player, u8 record, u8 marker, u8 flag) {
    for (u8 i = 0; i < 12; i++) {
        if (list->mUnits[i].mUnk_10 == 0) {
            list->mUnits[i].mUnk_10 = flag;
            list->mUnits[i].mPos = *pos;
            list->mUnits[i].mUnk_0f = player;
            list->mUnits[i].mUnk_0e = record;
            list->mUnits[i].mUnk_0c = value;
            list->mUnits[i].mUnk_11 = marker;
            list->mUnk_1c[marker].mUnk_5 = i + 1;
            list->mUnk_1f8[list->mUnk_1c[marker].mUnk_3][list->mUnk_1c[marker].mUnk_4] = i + 1;
            list->mUnk_295++;
            list->mPlayers[player]->mRecords[record].mUnk_0a = 1;
            return i + 1;
        }
    }
    return 0;
}

void func_ov007_02262828(BattlePlayerList *list, u8 index, u8 value) {
    u8 player         = list->mUnits[index].mUnk_0f;
    u8 record         = list->mUnits[index].mUnk_0e;
    BattlePlayer *p   = list->mPlayers[player];
    BattleRecord *rec = &p->mRecords[record];
    p->mRecords[record].mUnk_0a = value;
    BattlePlayer *p2 = list->mPlayers[player];
    p2->mRecords[record].mUnk_04 = p2->mRecords[record].mUnk_0b * 10;
    p2->mRecords[record].mUnk_06 = 0;
    if (rec->mUnk_08 == 4) {
        if (rec->mUnk_09 != 1) {
            rec->mUnk_09 = 1;
        }
    }
    list->mUnits[index].mUnk_10 = 0;
    list->mUnits[index].mUnk_12 = 0;
    u8 m = list->mUnits[index].mUnk_11;
    list->mUnk_1c[m].mUnk_5 = 0;
    list->mUnk_1f8[list->mUnk_1c[m].mUnk_3][list->mUnk_1c[m].mUnk_4] = 0;
    list->mUnk_295--;
}

// non-matching: 3 words differ, base (list + 0x1f8) and the row temp swap registers (r7/r8)
void func_ov007_022628e0(BattlePlayerList *list, u8 index, const VecFx32 *pos, u8 marker) {
    u8 old      = list->mUnits[index].mUnk_11;
    u8 r        = list->mUnk_1c[old].mUnk_3;
    u8 col      = list->mUnk_1c[old].mUnk_4;
    s8 *row     = list->mUnk_1f8[r];
    u8 i        = 0;
    BOOL found  = FALSE;
    u8 saved    = row[col];
    do {
        if (i != index && list->mUnits[i].mUnk_10 != 0 && list->mUnits[i].mUnk_11 == old) {
            row[col] = i + 1;
            found    = TRUE;
            break;
        }
        i++;
    } while (i < 12);
    if (!found) {
        list->mUnk_1f8[list->mUnk_1c[old].mUnk_3][list->mUnk_1c[old].mUnk_4] = 0;
    }
    list->mUnits[index].mPos    = *pos;
    list->mUnits[index].mUnk_11 = marker;
    list->mUnk_1f8[list->mUnk_1c[marker].mUnk_3][list->mUnk_1c[marker].mUnk_4] = saved;
}

void func_ov007_022629cc(BattlePlayerList *list, u8 index, u8 value) {
    u8 player         = list->mUnits[index].mUnk_0f;
    u8 record         = list->mUnits[index].mUnk_0e;
    BattlePlayer *p   = list->mPlayers[player];
    p->mRecords[record].mUnk_0a = 0;
    BattlePlayer *p2 = list->mPlayers[player];
    p2->mRecords[record].mUnk_04 = p2->mRecords[record].mUnk_0b * 10;
    p2->mRecords[record].mUnk_06 = 0;
    BattleRecord *rec = &list->mPlayers[player]->mRecords[record];
    if (rec->mUnk_08 == 4) {
        rec->mUnk_09 = 1;
    }
    list->mUnits[index].mUnk_0e = value;
    list->mUnits[index].mUnk_10 = 1;
    list->mPlayers[player]->mRecords[value].mUnk_0a = 1;
}

void func_ov007_02262a64(BattlePlayerList *list, u8 a, u8 b) {
    list->mUnk_28d = a;
    list->mUnk_28e = b;
}

void func_ov007_02262a70(BattlePlayerList *list, u8 slot, u8 player) {
    if (slot == 0) {
        list->mUnk_28a = player;
    }
    list->mUnk_284[slot] = player;
    if (data_020d433c.mPlayerCount == 2) {
        if (slot == 0) {
            list->mPlayers[player]->mUnk_f1 = 0;
        } else {
            list->mPlayers[player]->mUnk_f1 = 2;
        }
    } else {
        list->mPlayers[player]->mUnk_f1 = slot;
    }
}

void func_ov007_02262acc(BattlePlayerList *list) {
    list->mUnk_2a4 = new UnkStruct_02087898();
    MIi_CpuCopyFast(data_020d4404, (u8 *) list->mPlayers[0] + 0xa8, 8);
    switch (data_020d433c.mUnk_85) {
        case 1:
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->func_02087cf8("font/mes_T001_ba.mes");
            } else {
                list->mUnk_2a4->func_02087cf8("font/mes_T001_ba_Ali.mes");
            }
            list->mUnk_2af = 0;
            func_ov007_0226a404(list->mPlayers[0], 1, 2);
            func_ov007_0226a404(list->mPlayers[0], 2, 2);
            list->mUnk_2a8 = 10;
            break;
        case 2:
            list->mUnk_28f = 0;
            list->mUnk_288 = 2;
            list->mUnk_28a = 1;
            list->mUnk_2a8 = 12;
            func_ov007_02262638(list, 6, 4, 0, 0);
            func_ov007_02262638(list, 7, 5, 1, 0);
            break;
        case 3:
        case 4:
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->func_02087cf8("font/mes_T001_ba.mes");
            } else {
                list->mUnk_2a4->func_02087cf8("font/mes_T001_ba_Ali.mes");
            }
            list->mUnk_2af = 2;
            list->mUnk_288 = 3;
            func_ov007_02262638(list, 6, 4, 0, 0);
            func_ov007_02262638(list, 7, 5, 1, 0);
            list->mUnk_28a = 0;
            list->mUnk_2a8 = 12;
            if (data_020d433c.mUnk_85 == 4) {
                list->mUnk_2af = 3;
                list->mUnk_2a8 = 14;
            }
            list->mPlayers[0]->mRecords[1].mUnk_0a = 2;
            list->mPlayers[0]->mRecords[2].mUnk_0a = 2;
            break;
        case 7: {
            list->mUnk_28f = 0;
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->func_02087cf8("font/mes_T002_ba.mes");
            } else {
                list->mUnk_2a4->func_02087cf8("font/mes_T002_ba_Ali.mes");
            }
            list->mUnk_2af = 0;
            list->mUnk_288 = 10;
            list->mUnk_28a = 1;
            list->mUnk_2a8 = 12;
            func_ov007_02262638(list, 6, 4, 1, 0);
            func_ov007_0226a48c(list->mPlayers[0]);
            func_ov007_0226a48c(list->mPlayers[1]);
            u8 i = 0;
            do {
                BattlePlayer *a = list->mPlayers[0];
                a->mUnk_fe      = 1;
                a->mUnk_ee++;
                i++;
                BattlePlayer *b = list->mPlayers[1];
                b->mUnk_fe      = 1;
                b->mUnk_ee++;
            } while (i < 2);
            break;
        }
        case 8: {
            list->mUnk_28f = 0;
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->func_02087cf8("font/mes_T002_ba.mes");
            } else {
                list->mUnk_2a4->func_02087cf8("font/mes_T002_ba_Ali.mes");
            }
            list->mUnk_2af = 0;
            list->mUnk_288 = 11;
            list->mUnk_28a = 0;
            list->mUnk_2a8 = 12;
            VecFx32 pos    = {-0x11800, 0x899a, -0x17800};
            func_ov007_02262638(list, 6, 4, 1, 0);
            func_ov007_02262768(list, &pos, 0, 1, 0, 0, 1);
            func_ov007_0226a48c(list->mPlayers[0]);
            func_ov007_0226a48c(list->mPlayers[1]);
            u8 i = 0;
            do {
                BattlePlayer *a = list->mPlayers[0];
                a->mUnk_fe      = 1;
                a->mUnk_ee++;
                i++;
                BattlePlayer *b = list->mPlayers[1];
                b->mUnk_fe      = 1;
                b->mUnk_ee++;
            } while (i < 2);
            break;
        }
        case 10: {
            list->mUnk_28f = 0;
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->func_02087cf8("font/mes_T002_ba.mes");
            } else {
                list->mUnk_2a4->func_02087cf8("font/mes_T002_ba_Ali.mes");
            }
            list->mUnk_2af = 3;
            list->mUnk_288 = 11;
            list->mUnk_28a = 0;
            list->mUnk_2a8 = 24;
            VecFx32 pos    = {-0x11800, 0x899a, -0x17800};
            func_ov007_02262638(list, 6, 4, 1, 0);
            func_ov007_02262768(list, &pos, 0, 1, 0, 0, 1);
            func_ov007_02262768(list, &pos, 0, 0, 0, 0, 1);
            list->mUnk_28d = 0;
            list->mUnk_28e = 1;
            func_ov007_0226a48c(list->mPlayers[0]);
            func_ov007_0226a48c(list->mPlayers[1]);
            u8 i = 0;
            do {
                BattlePlayer *a = list->mPlayers[0];
                a->mUnk_fe      = 1;
                a->mUnk_ee++;
                i++;
                BattlePlayer *b = list->mPlayers[1];
                b->mUnk_fe      = 1;
                b->mUnk_ee++;
            } while (i < 2);
            break;
        }
        case 12:
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->func_02087cf8("font/mes_T003_ba.mes");
            } else {
                list->mUnk_2a4->func_02087cf8("font/mes_T003_ba_Ali.mes");
            }
            list->mUnk_2af = 0;
            list->mUnk_2a8 = 14;
            list->mUnk_28a = 0;
            list->mUnk_288 = 1;
            func_ov007_02262638(list, 4, 5, 0, 0);
            break;
        case 14: {
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->func_02087cf8("font/mes_T004_ba.mes");
            } else {
                list->mUnk_2a4->func_02087cf8("font/mes_T004_ba_Ali.mes");
            }
            list->mUnk_2af = 0;
            list->mUnk_2a8 = 24;
            list->mUnk_28a = 0;
            list->mUnk_288 = 1;
            VecFx32 pos    = {-0x11800, 0x899a, -0x17800};
            func_ov007_02262638(list, 6, 4, 1, 0);
            func_ov007_02262768(list, &pos, 0, 1, 0, 0, 1);
            func_ov007_02262768(list, &pos, 0, 0, 0, 0, 1);
            list->mUnk_28d = 0;
            list->mUnk_28e = 1;
            func_ov007_0226a48c(list->mPlayers[0]);
            func_ov007_0226a48c(list->mPlayers[1]);
            u8 i = 0;
            do {
                BattlePlayer *a = list->mPlayers[0];
                a->mUnk_fe      = 1;
                a->mUnk_ee++;
                i++;
                BattlePlayer *b = list->mPlayers[1];
                b->mUnk_fe      = 1;
                b->mUnk_ee++;
            } while (i < 2);
            break;
        }
    }
}
extern "C" BOOL func_ov007_02263150(BattlePlayerList *list) {
    BOOL result = FALSE;
    u8 i        = 0;
    u8 value;
    u8 mode  = data_020d433c.mUnk_98;
    u8 count = data_020d433c.mPlayerCount;
    BattlePlayer *p;
    do {
        p     = list->mPlayers[i];
        value = p->mUnk_ee;
        if (mode) {
            value = value + list->mPlayers[p->mUnk_f2]->mUnk_ee;
        }
        if (value >= 3) {
            result = TRUE;
            break;
        }
        i++;
    } while (i < count);
    return result;
}

BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id) {
    WirelessManager::Get();
    if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
        return TRUE;
    }
    u32 n = WirelessManager::Get().func_0202f134();
    data_0220f540.mUnk_00 = id;
    u8 count = 0;
    u8 i     = 0;
    do {
        if (id == list->mPlayers[i]->mMsg.mType) {
            count++;
        }
        i++;
    } while (i < n);
    BOOL result = FALSE;
    if (count == n) {
        result = TRUE;
    }
    return result;
}

BOOL func_ov007_022632d0(BattlePlayerList *list, u32 flag) {
    WirelessManager::Get();
    if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
        return FALSE;
    }
    if (flag) {
        if (data_0220f540.mUnk_00 == 0) {
            data_0220f540.mUnk_00 = 0x19;
        }
    }
    BOOL result = FALSE;
    u8 i        = 0;
    do {
        if (list->mPlayers[i]->mMsg.mType == 0x19) {
            result = TRUE;
            break;
        }
    } while (++i < WirelessManager::Get().func_0202f134());
    return result;
}

extern "C" void func_ov007_022633f8(BattlePlayerList *list) {
    if (data_020d433c.mUnk_84) {
        return;
    }
    if (data_020d433c.mUnk_7b != 0xd) {
        FrameTask::Get().func_0203dc34(0, data_ov007_0227aa5c[data_020d433c.mUnk_95]);
        return;
    }
    if (data_020d433c.mUnk_8a_0) {
        FrameTask::Get().func_0203dc34(0, 3);
        return;
    }
    if (func_0202326c(data_020d43f0)) {
        if (data_020d43f0[0].mUnk_42 == 9) {
            FrameTask::Get().func_0203dc34(0, 7);
        } else {
            FrameTask::Get().func_0203dc34(0, 5);
        }
        return;
    }
    if (data_020d433c.mUnk_8a_4) {
        FrameTask::Get().func_0203dc34(0, 10);
        return;
    }
    FrameTask::Get().func_0203dc34(0, data_ov007_0227aa5c[data_020d433c.mUnk_95]);
}

