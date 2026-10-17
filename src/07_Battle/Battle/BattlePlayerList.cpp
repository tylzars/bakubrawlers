#include "Game/UnkStruct_020d433c.hpp"
#include "UI/StorytalkWindow.hpp"
#include "Battle/GimmickTypes.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include "System/FrameTask.hpp"
#include "System/WirelessManager.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/SoundManager.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/math.h>
#include "Battle/BattleCharaAct.hpp"
#include "Battle/BattleCommand.hpp"
#include "Battle/BattleConfrontCard.hpp"
#include "Battle/BattleCpu.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Scene/ListScene.hpp"
// Placeholder for an unidentified object. The original's data layout needs one more participant in this file's
// size sort: a small weak object that no instruction addresses. This accessor's static joins the sort here, its
// call leaves no code, and the link strips it.
inline u32 *DataLayoutSlot() {
    static u32 sSlot[4];
    return sSlot;
}

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

    u8 StartSampling();
    void StopSampling();
    void Update();
};

// Placeholder for an unidentified object. The original's data layout needs one more participant in this file's
// size sort: a small weak object that no instruction addresses. This accessor's static joins the sort here, its
// call leaves no code, and the link strips it.

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
extern "C" void RestoreSndHeapState(u32 heap, u32 a);
void SetupTutorialPlayer(BattlePlayer *player);
void SetSlot54Flag(BattlePlayer *player, u32 index, u32 flag);
BOOL func_0202326c(SaveData *d);

void CreateObjects(BattlePlayerList *list);
BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);
void SetupTutorial(BattlePlayerList *list);
void InitPlayerListDisplay();
void RemoveMarker(BattlePlayerList *list, u8 index);
extern "C" BOOL func_ov007_02263150(BattlePlayerList *list);
void func_ov007_02269ee4(BattlePlayer *player, u32 reset);
u32 GetTexVariant(u32 a, u32 b);
extern u8 data_020d4444[];
extern "C" void RecordSndHeapState(u32 heap);

static const u8 sFrameTaskIds[8] = {6, 0, 8, 9, 4, 2, 10, 1};
BattlePlayerList::BattlePlayerList()
    : SceneBase(0x3e8, 0) {
    mCamera = 0;
    mUnk_2a4 = 0;
    mState = 0;
    mUnk_2ac = 0;
    mUnk_2ad = 0;
    mUnk_2ae = 0;
    mUnk_2af = 0;
    mUnk_2b0 = 0;
    if (data_020d433c.mUnk_84) {
        RestoreSndHeapState(SoundManager::Get().mHeap, 2);
    } else {
        SoundManager::Get().RestoreHeapState();
    }
    SoundManager::Get().LoadSoundGroup(6);
    data_020d433c.mGimmicks = 0;
    data_020d433c.mEffects  = 0;
    data_020d433c.mGimmickCollision   = 0;
    data_020d433c.mUnk_10   = 0;
    mUnk_2aa                = 0;
    mUnk_2ab                = 0;
    MI_CpuClearFast(&mPlayers[0], 0x28c);
    mTurnOrder[0] = 0;
    mTurnOrder[1] = 1;
    mTurnOrder[2] = 2;
    mTurnOrder[3] = 3;
    if (data_020d433c.mPlayerCount != 3 && data_020d433c.mPlayerCount != 4) {
        data_020d433c.mPlayerCount = 2;
    }
    mMarkerCount = 0;
    mUnitCount = 0;
    if (data_020d433c.mUnk_99 != 6) {
        data_020d433c.mUnk_99 = 3;
    }
    switch (data_020d433c.mUnk_83) {
        case 5:
        case 6:
            mUnk_2ae              = data_020d433c.mUnk_83;
            data_020d433c.mUnk_83 = 0;
            data_020d433c.mTutorialId = 0;
    }
    func_ov007_022633f8(this);
    mBattleTarget = 0;
    mUnk_29c = 0;
    MicManager::Get().StartSampling();
}


BattlePlayerList::~BattlePlayerList() {
    MicManager::Get().StopSampling();
    delete mCamera;
    if (data_020d433c.mGimmickCollision) {
        ((SceneBase *) data_020d433c.mGimmickCollision)->mFinished = 1;
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
    CameraManager::Get().SetCamerasAndRequestMode(0, 0, 0);
    GX_SetDispSelect(0);
}

// Placeholder (case 4, second record): the two volatile reads stand in for an unknown source form. The original
// numbers the reloaded mPlayers[tp] first, the slot index second and the `+ 12` record base last, so the colourer
// (which takes these temporaries in falling number order) gives the base r0 and the index r2. With plain reads the
// slot and player loads are folded into the `target` expression and the index is coloured first (r0, 3 words); a
// volatile slot read alone fixes the index but numbers the player load last (5 words); a volatile player read alone
// changes nothing.
void BattlePlayerList::Update() {
    u8 owner;
    u8 slot;
    u8 id;
    u8 result;
    u8 rec;
    MicManager::Get().Update();
    if (data_020d433c.mWireless) {
        WirelessManager::Get();
        if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
            SpriteManager::Get().SetFade(1, -16);
            RequestScene(SCENE_ARENA_WINDOW);
            data_020d433c.mUnk_10 = 0;
            return;
        }
    }
    switch (mState) {
        case 0:
            CreateObjects(this);
            mState++;
            break;
        case 1:
            if (SetMessageTypeAndCheckAll(this, 0xf)) {
                data_020d433c.mGimmicks->LoadSetData(data_020d433c.mFieldId, (u8) data_020d433c.mUnk_96);
                if (data_020d433c.mTutorialId != 0) {
                    mState = 2;
                } else {
                    mState = 4;
                }
            }
            break;
        case 2:
            SetupTutorial(this);
            break;
        case 4:
            new ListScene(5, mUnk_2a9);
            mContext->mRequest = 0;
            mState++;
            break;
        case 5: {
            u8 result = mContext->mRequest;
            if (result != 0 && data_020d433c.mUnk_10) {
                data_020d433c.mUnk_10->SetupLayout(0);
            }
            if (result == 0) {
                return;
            }
            if (result == 2) {
                RequestScene(SCENE_TITLE);
                mFinished = 1;
            } else {
                mState++;
            }
            break;
        }
        case 6:
            WirelessManager::Get();
            if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                mState = 8;
                return;
            }
            mState++;
            break;
        case 7:
            data_0220f540.mUnk_04 = MATH_Rand32(&gRandom, 0);
            if (SetMessageTypeAndCheckAll(this, 2)) {
                SeedRandom(*(u32 *) ((u8 *) mPlayers[0] + 0x104));
                mState = 8;
            }
            break;
        case 8:
            mUnk_2b1 = MATH_Rand32(&gRandom, 100);
            if (data_020d433c.mHasPartners) {
                SetTurnOrder(data_020d433c.mBattlePlayers, 0, 0);
                u8 i = 1;
                u8 k = 0;
                do {
                    BattlePlayerList *list = data_020d433c.mBattlePlayers;
                    if (i == list->mPlayers[0]->mPartner) {
                        SetTurnOrder(list, 1, i);
                    } else {
                        SetTurnOrder(list, k + 2, i);
                        k++;
                    }
                    i++;
                } while (i < data_020d433c.mPlayerCount);
                mState = 10;
                return;
            }
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                new BattleOrderSelect();
                mContext->mRequest = 0;
                mState++;
            } else {
                u8 i = 0;
                do {
                    SetTurnOrder(data_020d433c.mBattlePlayers, i, i);
                    i++;
                } while (i < data_020d433c.mPlayerCount);
                mState = 10;
            }
            return;
        case 9: {
            SceneContext *context = mContext;
            switch ((u8) context->mRequest) {
                case 0:
                    return;
                case 2:
                    RequestScene(SCENE_TITLE);
                    mFinished = 1;
                    break;
                default:
                    mState++;
                    break;
            }
            break;
        }
        case 10: {
            u8 flag = 0;
            func_ov007_02269ee4(mPlayers[mTurnPlayer], flag);
            if (data_020d433c.mTutorialId == 1) {
                flag = 1;
            }
            new BattleObj00(flag);
            mContext->mRequest = 0;
            mState++;
            break;
        }
        case 11:
            if (mContext->mRequest == 0) {
                return;
            }
            if (data_020d433c.mTutorialId == 1) {
                mState = 0x22;
                SpriteManager::Get().StartFade(3);
                return;
            }
            mState++;
            mUnk_28f = 1;
            mUnk_291 = 1;
            if (data_020d433c.mUnk_8a_0 != 0) {
                func_ov007_02269ee4(mPlayers[mTurnPlayer], 1);
            }
            break;
        case 12:
            new BattleCommand((u32) mCamera);
            mContext->mRequest = 0;
            mState++;
            break;
        case 13:
            switch ((u8) mContext->mRequest) {
                case 2:
                    mState = 0x1a;
                    break;
                case 1:
                    mState++;
                    ((void (*)(BattlePlayerList *)) InitPlayerListDisplay)(this);
                    break;
                case 3: {
                    u8 *msg = (u8 *) mPlayers[mTurnPlayer] + 0x100;
                    new BattleShoot(mCamera, (u16 *) (msg + 4), *(u16 *) (msg + 0xa), *(u16 *) (msg + 0xc), *(msg + 0xf), *(msg + 0x10), mUnk_2ae);
                    mContext->mRequest = 0;
                    mState = 0x11;
                    break;
                }
                case 4: {
                    BattleCpuRot rot = {0x71c, 0x6000, 0x71c};
                    u16 outA         = 0;
                    u16 outB         = 0;
                    u8 outC          = 0;
                    mPlayers[mTurnPlayer]->mCpu->func_ov007_02231e2c(&rot, &outA, &outB, &outC);
                    u8 extra = *((u8 *) mPlayers[mTurnPlayer]->mCpu + 0x7e);
                    new BattleShoot(mCamera, &rot.mX, outA, outB, outC, extra, mUnk_2ae);
                    mContext->mRequest = 0;
                    mState = 0x11;
                    break;
                }
                case 5:
                    mState = 0x1d;
                    break;
                case 6:
                    mState = 0x1b;
                    break;
                case 7:
                    mState = 0x24;
                    break;
            }
            break;
        case 14:
            new BattleShoot(mCamera, mUnk_2ae);
            mContext->mRequest = 0;
            mState = 0x11;
            break;
        case 17: {
            rec = mPlayers[mTurnPlayer]->mRecord;
            result = mContext->mRequest;
            if (result == 0) {
                return;
            }
            u8 mode = data_020d433c.mTutorialId;
            if (mode == 2 || mode == 3 || mode == 4 || mode == 0xc) {
                if (result != 5) {
                    mState = 0x22;
                    SpriteManager::Get().StartFade(3);
                    if (result != 1) {
                        if (data_020d433c.mTutorialId == 0xc && mUnk_2b0 == 0) {
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
                        mState = 0x22;
                        SpriteManager::Get().StartFade(3);
                        data_020d433c.mUnk_86++;
                        return;
                    } else if (mUnk_2a9 == mTurnPlayer) {
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
                    if (mUnk_2a9 == mTurnPlayer) {
                        mState = 0xe;
                    } else {
                        mState = 0xc;
                        mUnk_28f = 0;
                    }
                    mUnk_2ac = 0;
                    if (result == 3) {
                        u8 a  = mUnits[mUnitA].mMarker;
                        u8 m3 = mMarkers[a].mCellX;
                        u8 m4 = mMarkers[a].mCellY;
                        RemoveUnit(this, mUnitB, 0);
                        mUnitHandles[m3][m4] = mUnitA + 1;
                    } else {
                        RemoveUnit(this, mUnitA, 0);
                    }
                    return;
                case 3: {
                    mUnk_2ac = 0;
                    owner = mUnits[mUnitA].mPlayer;
                    if (result != 3) {
                        slot = mUnits[mUnitA].mRecord;
                    } else {
                        owner = mUnits[mUnitB].mPlayer;
                        slot  = mUnits[mUnitB].mRecord;
                    }
                    BattlePlayer *p = mPlayers[owner];
                    p->mUnk_fe = 1;
                    p->mUnk_ee++;
                    RemoveMarker(this, mUnits[mUnitA].mMarker);
                    BattleRecord *used = &mPlayers[owner]->mRecords[slot];
                    mUnk_2aa           = GetTexVariant(used->mBakuganId, used->mAttribute);
                    RemoveUnit(this, mUnitA, 2);
                    if (result == 3) {
                        RemoveUnit(this, mUnitB, 2);
                    }
                    mState = 0x16;
                    if (data_020d433c.mUnk_7a == 0) {
                        return;
                    }
                    if (mUnk_2a9 != mTurnPlayer) {
                        return;
                    }
                    id = mPlayers[mUnk_2a9]->mRecords[rec].mAttribute;
                    if (gSaveData[0].mUnk_4e < 0xffff) {
                        gSaveData[0].mUnk_4e++;
                    }
                    if (data_020d4444[id] < 99) {
                        data_020d4444[id]++;
                    }
                    if (gSaveData[0].mUnk_50 < 0xffff) {
                        gSaveData[0].mUnk_50++;
                    }
                    return;
                }
            }
            switch (result) {
                case 1:
                    if (data_020d433c.mUnk_83 == 0 && data_020d433c.mUnk_7a != 0 && mUnk_2a9 == mTurnPlayer) {
                        id = mPlayers[mUnk_2a9]->mRecords[rec].mAttribute;
                        if (gSaveData[0].mUnk_4e < 0xffff) {
                            gSaveData[0].mUnk_4e++;
                        }
                        if (data_020d4444[id] < 99) {
                            data_020d4444[id]++;
                        }
                    }
                    {
                        BattlePlayer *current = mPlayers[mTurnPlayer];
                        current->mRecords[rec].mUnk_04 = current->mRecords[rec].mUnk_0b * 10;
                        current->mRecords[rec].mUnk_06 = 0;
                    }
                    mState = 0x12;
                    break;
                case 2:
                    mState = 0x1a;
                    if (data_020d433c.mUnk_83 != 0) {
                        return;
                    }
                    if (data_020d433c.mUnk_7a == 0) {
                        return;
                    }
                    if (mUnk_2a9 != mTurnPlayer) {
                        return;
                    }
                    id = mPlayers[mUnk_2a9]->mRecords[rec].mAttribute;
                    if (gSaveData[0].mUnk_4e < 0xffff) {
                        gSaveData[0].mUnk_4e++;
                    }
                    if (data_020d4444[id] < 99) {
                        data_020d4444[id]++;
                    }
                    if (gSaveData[0].mUnk_50 < 0xffff) {
                        gSaveData[0].mUnk_50++;
                    }
                    break;
                case 3: {
                    u8 ownerA = mUnits[mUnitA].mPlayer;
                    u8 ownerB = mUnits[mUnitB].mPlayer;
                    if (data_020d433c.mUnk_83 == 0 && data_020d433c.mUnk_7a != 0 && mUnk_2a9 == mTurnPlayer) {
                        id = mPlayers[mUnk_2a9]->mRecords[rec].mAttribute;
                        if (gSaveData[0].mUnk_4e < 0xffff) {
                            gSaveData[0].mUnk_4e++;
                        }
                        if (data_020d4444[id] < 99) {
                            data_020d4444[id]++;
                        }
                    }
                    if (ownerA == ownerB) {
                        BattlePlayer *p = mPlayers[ownerA];
                        p->mUnk_fe = 1;
                        p->mUnk_ee++;
                        mState = 0x14;
                        // Each record index goes through the reused `slot`: a reassigned local keeps its own late register.
                        slot = mUnits[mUnitA].mRecord;
                        BattleRecord *recs = mPlayers[ownerA]->mRecords;
                        mUnk_2aa           = GetTexVariant(recs[slot].mBakuganId, recs[slot].mAttribute);
                        recs    = mPlayers[ownerA]->mRecords;
                        slot = mUnits[mUnitB].mRecord;
                        mUnk_2ab = GetTexVariant(recs[slot].mBakuganId, recs[slot].mAttribute);
                        RemoveMarker(this, mUnits[mUnitA].mMarker);
                        RemoveUnit(this, mUnitA, 2);
                        RemoveUnit(this, mUnitB, 2);
                        if (data_020d433c.mUnk_83 != 0) {
                            return;
                        }
                        if (data_020d433c.mUnk_7a == 0) {
                            return;
                        }
                        if (mUnk_2a9 != mTurnPlayer) {
                            return;
                        }
                        if (gSaveData[0].mUnk_52 < 0x270f) {
                            gSaveData[0].mUnk_52++;
                        }
                        if (gSaveData[0].mUnk_50 < 0xffff) {
                            gSaveData[0].mUnk_50++;
                        }
                    } else {
                        mState = 0x18;
                        if (data_020d433c.mUnk_83 != 0) {
                            return;
                        }
                        if (data_020d433c.mUnk_7a == 0) {
                            return;
                        }
                        if (mUnk_2a9 != mTurnPlayer) {
                            return;
                        }
                        if (gSaveData[0].mUnk_50 < 0xffff) {
                            gSaveData[0].mUnk_50++;
                        }
                    }
                    break;
                }
                case 4: {
                    u8 tp  = mUnits[mUnitB].mPlayer;
                    u8 sp = mUnits[mUnitA].mPlayer;
                    BattlePlayer *p = mPlayers[tp];
                    p->mUnk_fe = 1;
                    p->mUnk_ee++;
                    mState = 0x16;
                    RemoveMarker(this, mUnits[mUnitA].mMarker);
                    slot = mUnits[mUnitA].mRecord;
                    BattleRecord *source = &mPlayers[sp]->mRecords[slot];
                    mPlayers[tp]->PushHistory(source->mUnk_04, source->mAttribute);
                    BattlePlayer *holder = *(BattlePlayer *volatile *) &mPlayers[tp];
                    slot                 = *(volatile u8 *) &mUnits[mUnitB].mRecord;
                    BattleRecord *target = &holder->mRecords[slot];
                    mUnk_2aa             = GetTexVariant(target->mBakuganId, target->mAttribute);
                    RemoveUnit(this, mUnitA, 2);
                    RemoveUnit(this, mUnitB, 2);
                    if (data_020d433c.mUnk_83 != 0) {
                        return;
                    }
                    if (data_020d433c.mUnk_7a == 0) {
                        return;
                    }
                    if (mUnk_2a9 != mTurnPlayer) {
                        return;
                    }
                    id = mPlayers[mUnk_2a9]->mRecords[rec].mAttribute;
                    if (gSaveData[0].mUnk_4e < 0xffff) {
                        gSaveData[0].mUnk_4e++;
                    }
                    if (data_020d4444[id] < 99) {
                        data_020d4444[id]++;
                    }
                    if (gSaveData[0].mUnk_5c < 0x270f) {
                        gSaveData[0].mUnk_5c++;
                    }
                    if (gSaveData[0].mUnk_50 < 0xffff) {
                        gSaveData[0].mUnk_50++;
                    }
                    break;
                }
                case 6:
                    mState = 0x24;
                    break;
                default:
                    mState = 0xc;
                    break;
            }
            break;
        }
        case 18:
            new BattleCharaAct(mCamera, mTurnPlayer, 4, 0, 0);
            mContext->mRequest = 0;
            mState++;
            break;
        case 19:
            if ((u8) mContext->mRequest) {
                mState = 0x1a;
            }
            break;
        case 20:
            new BattleCharaAct(mCamera, mTurnPlayer, 8, mUnk_2aa, mUnk_2ab);
            mContext->mRequest = 0;
            mState++;
            break;
        case 21:
            if ((u8) mContext->mRequest == 0) {
                return;
            }
            mState = 0x19;
            mContext->mRequest = 1;
            break;
        case 22:
            new BattleCharaAct(mCamera, mTurnPlayer, 9, mUnk_2aa, 0);
            mContext->mRequest = 0;
            mState++;
            break;
        case 23:
            if ((u8) mContext->mRequest == 0) {
                return;
            }
            mState = 0x19;
            mContext->mRequest = 1;
            break;
        case 24:
            if (SetMessageTypeAndCheckAll(this, 0xf)) {
                new BattleConfrontCard(-1);
                mContext->mRequest = 0;
                mState++;
            }
            break;
        case 25:
            if (mContext->mRequest == 0) {
                return;
            }
            mUnk_2b1 = MATH_Rand32(&gRandom, 100);
            if (data_020d433c.mUnk_83 == 2 || data_020d433c.mUnk_83 == 4) {
                SoundManager::Get().RestoreHeapState();
                SoundManager::Get().LoadAndStartSeq(1);
                RecordSndHeapState(SoundManager::Get().mHeap);
                mState = 0x22;
                SpriteManager::Get().StartFade(3);
                if (mUnk_2b0 != 0) {
                    data_020d433c.mUnk_86 = 0;
                } else {
                    data_020d433c.mUnk_86++;
                }
                return;
            } else if (func_ov007_02263150(this)) {
                mState = 0x1d;
            } else {
                mState = 0x1a;
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
                mTurnCount++;
                if (data_020d433c.mGimmicks) {
                    data_020d433c.mGimmicks->CountTurnAll();
                }
            }
            u8 idx = (mTurnCount - 1) % data_020d433c.mPlayerCount;
            if (data_020d433c.mHasPartners) {
                if (idx == 1) {
                    idx = 2;
                } else if (idx == 2) {
                    idx = 1;
                }
            }
            mTurnPlayer = mTurnOrder[idx];
            mState = 0xc;
            func_ov007_02269ee4(mPlayers[mTurnPlayer], flag);
            u8 found;
            u8 i = 0;
            found = 0;
            do {
                if (mMarkers[i].mStatus != 0) {
                    found = 1;
                    break;
                }
                i++;
            } while (i < 12);
            if (!found) {
                mState = 0x1b;
            }
            u8 current = mTurnPlayer;
            u8 j       = 0;
            do {
                if (current == mUnits[j].mPlayer && mUnits[j].mUnk_12 != 0) {
                    data_020d433c.mBattlePlayers->mUnk_292 = 2;
                    return;
                }
                j++;
            } while (j < 12);
            break;
        }
        case 27:
            new BattleObj00(1);
            mContext->mRequest = 0;
            mState++;
            break;
        case 28:
            if (mContext->mRequest == 0) {
                return;
            }
            mState = 0xc;
            if (data_020d433c.mBattlePlayers->mPlayers[mTurnPlayer]->mSlots54Remaining == 0) {
                data_020d433c.mBattlePlayers->mUnk_28f = 0;
            }
            break;
        case 29:
            if (mUnk_2ae != 0) {
                SoundManager::Get().StopSeq(1);
            }
            new BattleResult();
            mContext->mRequest = 0;
            mState++;
            break;
        case 30:
            if (mContext->mRequest == 0) {
                return;
            }
            if (!SetMessageTypeAndCheckAll(this, 0x10)) {
                return;
            }
            mState = 0x1f;
            if (mUnk_2ae == 0) {
                return;
            }
            SoundManager::Get().RestoreHeapState();
            SoundManager::Get().LoadAndStartSeq(1);
            RecordSndHeapState(SoundManager::Get().mHeap);
            break;
        case 31:
            data_020d433c.mUnk_87   = 1;
            data_020d433c.mUnk_88_4 = 0;
            data_020d433c.mUnk_88_0 = 0;
            if (mUnk_2ae != 0) {
                data_020d433c.mUnk_83 = mUnk_2ae;
                RequestScene(SCENE_TUTORIAL);
                mFinished = 1;
                return;
            }
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228) || data_020d433c.mNextScene == SCENE_ARENA_WINDOW) {
                if (mContext->mRequest == 1) {
                    data_020d433c.mUnk_88_4 = 1;
                }
                mState++;
            } else {
                u8 next = data_020d433c.mNextScene;
                if (next != 0) {
                    if (data_020d433c.mUnk_7a != 0 && next == SCENE_TOURNAMENT && data_020d433c.mUnk_7d == 3) {
                        RequestScene(SCENE_GAME_OVER);
                        mFinished = 1;
                        return;
                    }
                    data_020d433c.mNextScene = 0;
                    RequestScene(next);
                    mFinished = 1;
                } else {
                    RequestScene(SCENE_TITLE);
                    mFinished = 1;
                }
            }
            break;
        case 32: {
            SceneContext *context = mContext;
            switch (context->mRequest) {
                case 1:
                    if (data_020d433c.mWireless) {
                        data_020d433c.mUnk_89 = 1;
                        WirelessManager::Get().Stop();
                        mState++;
                    } else {
                        data_020d433c.mUnk_89 = 0;
                        RequestScene(SCENE_ARENA_WINDOW);
                        mFinished = 1;
                    }
                    break;
                case 2:
                    RequestScene(SCENE_BATTLE_PLAYER_LIST);
                    mFinished = 1;
                    break;
                case 3:
                    data_020d433c.mUnk_89 = 1;
                    RequestScene(SCENE_ARENA_WINDOW);
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
            data_020d433c.mWireless = 0;
            data_020d433c.mUnk_89 = 0;
            RequestScene(SCENE_ARENA_WINDOW);
            mFinished = 1;
            break;
        case 34:
            mUnk_2a4->Close();
            mState++;
            break;
        case 35:
            if (!SpriteManager::Get().IsFading()) {
                RequestScene(SCENE_TUTORIAL);
                mFinished = 1;
            }
            break;
        case 36:
            SpriteManager::Get().SetFade(1, -16);
            SoundManager::Get().StopSeq(1);
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228) || data_020d433c.mNextScene == SCENE_ARENA_WINDOW) {
                mContext->mRequest       = 1;
                data_020d433c.mUnk_88_4 = 0;
                data_020d433c.mUnk_88_0 = 1;
                data_020d433c.mUnk_87   = 1;
                mState                = 0x20;
            } else {
                RequestScene(SCENE_MAP);
                mFinished = 1;
            }
            break;
    }
}

// Record of a command cell: u8 table helpers below
void InitPlayerListDisplay() {
    SpriteManager::Get().SetupMode(1, 7);
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

void CreateObjects(BattlePlayerList *list) {
    list->mCamera = new Camera();
    list->mCamera->SetUp(0, 0x1000, 0);
    list->mCamera->SetTarget(0, 0, 0);
    list->mCamera->SetClip(0x1000, 0x1388000);
    list->mCamera->SetFov(0x2aaa);
    list->mCamera->SetPosition(0, 0, list->mCamera->mUnk_34);
    data_020d433c.mEffects = new EffectManager();
    data_020d433c.mGimmickCollision  = new GimmickCollision();
    data_020d433c.mGimmicks = new GimmickManager();
    list->mUnk_29c = new BattleUnk29c();
    if (data_020d433c.mWireless) {
        data_020d433c.mUnk_10 = new AntennaIndicator(0);
    }
    u8 i = 0;
    do {
        list->mPlayers[i] = new BattlePlayer(i);
        i++;
    } while (i < data_020d433c.mPlayerCount);
    WirelessManager::Get();
    list->mUnk_2a9 = IsWirelessMode(&data_020c4030.mUnk_228) ? data_020d433c.mUnk_ae : 0;
}
u16 *GetMarkerSlotValue(BattlePlayerList *list, u8 index) {
    return &list->mPlayers[list->mMarkers[index].mPlayer]->Slot54(list->mMarkers[index].mSlot);
}

BattleRecord *GetUnitRecordAt(BattlePlayerList *list, u8 x, u8 y) {
    u8 index = (u8) (list->mUnitHandles[x][y] - 1);
    return &list->mPlayers[list->mUnits[index].mPlayer]->mRecords[list->mUnits[index].mRecord];
}

static const s8 sNeighbors[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

BOOL IsMarkerPlaceable(BattlePlayerList *list, u8 x, u8 y) {
    if (list->mMarkerCount == 0 && x >= 3 && x <= 4 && y >= 2 && y <= 3) {
        return TRUE;
    }
    if (list->mMarkerHandles[x][y] != 0) {
        return FALSE;
    }
    for (u8 i = 0; i < 4; i++) {
        u8 r = (u8) (x + sNeighbors[i][0]);
        u8 c = (u8) (y + sNeighbors[i][1]);
        if (r < 14 && c < 10 && list->mMarkerHandles[r][c] > 0) {
            return TRUE;
        }
    }
    return FALSE;
}


void AddMarker(BattlePlayerList *list, u8 x, u8 y, u8 player, u8 slot) {
    for (u8 i = 0; i < 12; i++) {
        if (list->mMarkers[i].mStatus == 0) {
            list->mMarkers[i].mStatus = 1;
            list->mMarkers[i].mCellX = x;
            list->mMarkers[i].mCellY = y;
            list->mMarkers[i].mPlayer = player;
            list->mMarkers[i].mSlot = slot;
            list->mMarkerHandles[x][y] = i + 1;
            SetSlot54Flag(list->mPlayers[player], slot, 1);
            list->mMarkerCount++;
            return;
        }
    }
}

void RemoveMarker(BattlePlayerList *list, u8 index) {
    SetSlot54Flag(list->mPlayers[list->mMarkers[index].mPlayer], list->mMarkers[index].mSlot, 2);
    list->mMarkers[index].mStatus                                      = 0;
    list->mMarkerHandles[list->mMarkers[index].mCellX][list->mMarkers[index].mCellY] = 0;
    list->mMarkerCount--;
}

void SetMarkerPlayerSlot(BattlePlayerList *list, u8 index, u8 player, u8 slot) {
    SetSlot54Flag(list->mPlayers[list->mMarkers[index].mPlayer], list->mMarkers[index].mSlot, 0);
    list->mMarkers[index].mPlayer = player;
    list->mMarkers[index].mSlot = slot;
    SetSlot54Flag(list->mPlayers[player], slot, 1);
}

u8 AddUnit(BattlePlayerList *list, const VecFx32 *pos, u16 value, u8 player, u8 record, u8 marker, u8 flag) {
    for (u8 i = 0; i < 12; i++) {
        if (list->mUnits[i].mStatus == 0) {
            list->mUnits[i].mStatus = flag;
            list->mUnits[i].mPos = *pos;
            list->mUnits[i].mPlayer = player;
            list->mUnits[i].mRecord = record;
            list->mUnits[i].mUnk_0c = value;
            list->mUnits[i].mMarker = marker;
            list->mMarkers[marker].mUnitHandle = i + 1;
            list->mUnitHandles[list->mMarkers[marker].mCellX][list->mMarkers[marker].mCellY] = i + 1;
            list->mUnitCount++;
            list->mPlayers[player]->mRecords[record].mStatus = 1;
            return i + 1;
        }
    }
    return 0;
}

void RemoveUnit(BattlePlayerList *list, u8 index, u8 value) {
    u8 player         = list->mUnits[index].mPlayer;
    u8 record         = list->mUnits[index].mRecord;
    BattlePlayer *p   = list->mPlayers[player];
    BattleRecord *rec = &p->mRecords[record];
    p->mRecords[record].mStatus = value;
    BattlePlayer *p2 = list->mPlayers[player];
    p2->mRecords[record].mUnk_04 = p2->mRecords[record].mUnk_0b * 10;
    p2->mRecords[record].mUnk_06 = 0;
    if (rec->mBakuganId == 4) {
        if (rec->mAttribute != 1) {
            rec->mAttribute = 1;
        }
    }
    list->mUnits[index].mStatus = 0;
    list->mUnits[index].mUnk_12 = 0;
    u8 m = list->mUnits[index].mMarker;
    list->mMarkers[m].mUnitHandle = 0;
    list->mUnitHandles[list->mMarkers[m].mCellX][list->mMarkers[m].mCellY] = 0;
    list->mUnitCount--;
}

// Grid row of a marker: an enum-typed copy is a real load (mwcc does not substitute enum locals), so it is coloured
// after the row base.
enum MarkerRow { MARKER_ROW_0 };

void MoveUnit(BattlePlayerList *list, u8 index, const VecFx32 *pos, u8 marker) {
    u8 old      = list->mUnits[index].mMarker;
    MarkerRow r = (MarkerRow) list->mMarkers[old].mCellX;
    u8 col      = list->mMarkers[old].mCellY;
    s8 *row     = list->mUnitHandles[r];
    u8 i        = 0;
    BOOL found  = FALSE;
    u8 saved    = row[col];
    do {
        if (i != index && list->mUnits[i].mStatus != 0 && list->mUnits[i].mMarker == old) {
            row[col] = i + 1;
            found    = TRUE;
            break;
        }
        i++;
    } while (i < 12);
    if (!found) {
        list->mUnitHandles[list->mMarkers[old].mCellX][list->mMarkers[old].mCellY] = 0;
    }
    list->mUnits[index].mPos    = *pos;
    list->mUnits[index].mMarker = marker;
    list->mUnitHandles[list->mMarkers[marker].mCellX][list->mMarkers[marker].mCellY] = saved;
}

void ReplaceUnitRecord(BattlePlayerList *list, u8 index, u8 value) {
    u8 player         = list->mUnits[index].mPlayer;
    u8 record         = list->mUnits[index].mRecord;
    BattlePlayer *p   = list->mPlayers[player];
    p->mRecords[record].mStatus = 0;
    BattlePlayer *p2 = list->mPlayers[player];
    p2->mRecords[record].mUnk_04 = p2->mRecords[record].mUnk_0b * 10;
    p2->mRecords[record].mUnk_06 = 0;
    BattleRecord *rec = &list->mPlayers[player]->mRecords[record];
    if (rec->mBakuganId == 4) {
        rec->mAttribute = 1;
    }
    list->mUnits[index].mRecord = value;
    list->mUnits[index].mStatus = 1;
    list->mPlayers[player]->mRecords[value].mStatus = 1;
}

void SetUnitAB(BattlePlayerList *list, u8 a, u8 b) {
    list->mUnitA = a;
    list->mUnitB = b;
}

void SetTurnOrder(BattlePlayerList *list, u8 slot, u8 player) {
    if (slot == 0) {
        list->mTurnPlayer = player;
    }
    list->mTurnOrder[slot] = player;
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

void SetupTutorial(BattlePlayerList *list) {
    list->mUnk_2a4 = new StorytalkWindow();
    MIi_CpuCopyFast(gCharacterLook, (u8 *) list->mPlayers[0] + 0xa8, 8);
    switch (data_020d433c.mTutorialId) {
        case 1:
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->LoadMessageFile("font/mes_T001_ba.mes");
            } else {
                list->mUnk_2a4->LoadMessageFile("font/mes_T001_ba_Ali.mes");
            }
            list->mUnk_2af = 0;
            SetSlot54Flag(list->mPlayers[0], 1, 2);
            SetSlot54Flag(list->mPlayers[0], 2, 2);
            list->mState = 10;
            break;
        case 2:
            list->mUnk_28f = 0;
            list->mTurnCount = 2;
            list->mTurnPlayer = 1;
            list->mState = 12;
            AddMarker(list, 6, 4, 0, 0);
            AddMarker(list, 7, 5, 1, 0);
            break;
        case 3:
        case 4:
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->LoadMessageFile("font/mes_T001_ba.mes");
            } else {
                list->mUnk_2a4->LoadMessageFile("font/mes_T001_ba_Ali.mes");
            }
            list->mUnk_2af = 2;
            list->mTurnCount = 3;
            AddMarker(list, 6, 4, 0, 0);
            AddMarker(list, 7, 5, 1, 0);
            list->mTurnPlayer = 0;
            list->mState = 12;
            if (data_020d433c.mTutorialId == 4) {
                list->mUnk_2af = 3;
                list->mState = 14;
            }
            list->mPlayers[0]->mRecords[1].mStatus = 2;
            list->mPlayers[0]->mRecords[2].mStatus = 2;
            break;
        case 7: {
            list->mUnk_28f = 0;
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->LoadMessageFile("font/mes_T002_ba.mes");
            } else {
                list->mUnk_2a4->LoadMessageFile("font/mes_T002_ba_Ali.mes");
            }
            list->mUnk_2af = 0;
            list->mTurnCount = 10;
            list->mTurnPlayer = 1;
            list->mState = 12;
            AddMarker(list, 6, 4, 1, 0);
            SetupTutorialPlayer(list->mPlayers[0]);
            SetupTutorialPlayer(list->mPlayers[1]);
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
                list->mUnk_2a4->LoadMessageFile("font/mes_T002_ba.mes");
            } else {
                list->mUnk_2a4->LoadMessageFile("font/mes_T002_ba_Ali.mes");
            }
            list->mUnk_2af = 0;
            list->mTurnCount = 11;
            list->mTurnPlayer = 0;
            list->mState = 12;
            VecFx32 pos    = {-0x11800, 0x899a, -0x17800};
            AddMarker(list, 6, 4, 1, 0);
            AddUnit(list, &pos, 0, 1, 0, 0, 1);
            SetupTutorialPlayer(list->mPlayers[0]);
            SetupTutorialPlayer(list->mPlayers[1]);
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
                list->mUnk_2a4->LoadMessageFile("font/mes_T002_ba.mes");
            } else {
                list->mUnk_2a4->LoadMessageFile("font/mes_T002_ba_Ali.mes");
            }
            list->mUnk_2af = 3;
            list->mTurnCount = 11;
            list->mTurnPlayer = 0;
            list->mState = 24;
            VecFx32 pos    = {-0x11800, 0x899a, -0x17800};
            AddMarker(list, 6, 4, 1, 0);
            AddUnit(list, &pos, 0, 1, 0, 0, 1);
            AddUnit(list, &pos, 0, 0, 0, 0, 1);
            list->mUnitA = 0;
            list->mUnitB = 1;
            SetupTutorialPlayer(list->mPlayers[0]);
            SetupTutorialPlayer(list->mPlayers[1]);
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
                list->mUnk_2a4->LoadMessageFile("font/mes_T003_ba.mes");
            } else {
                list->mUnk_2a4->LoadMessageFile("font/mes_T003_ba_Ali.mes");
            }
            list->mUnk_2af = 0;
            list->mState = 14;
            list->mTurnPlayer = 0;
            list->mTurnCount = 1;
            AddMarker(list, 4, 5, 0, 0);
            break;
        case 14: {
            if (data_020d433c.mUnk_82) {
                list->mUnk_2a4->LoadMessageFile("font/mes_T004_ba.mes");
            } else {
                list->mUnk_2a4->LoadMessageFile("font/mes_T004_ba_Ali.mes");
            }
            list->mUnk_2af = 0;
            list->mState = 24;
            list->mTurnPlayer = 0;
            list->mTurnCount = 1;
            VecFx32 pos    = {-0x11800, 0x899a, -0x17800};
            AddMarker(list, 6, 4, 1, 0);
            AddUnit(list, &pos, 0, 1, 0, 0, 1);
            AddUnit(list, &pos, 0, 0, 0, 0, 1);
            list->mUnitA = 0;
            list->mUnitB = 1;
            SetupTutorialPlayer(list->mPlayers[0]);
            SetupTutorialPlayer(list->mPlayers[1]);
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
    u8 mode  = data_020d433c.mHasPartners;
    u8 count = data_020d433c.mPlayerCount;
    BattlePlayer *p;
    do {
        p     = list->mPlayers[i];
        value = p->mUnk_ee;
        if (mode) {
            value = value + list->mPlayers[p->mPartner]->mUnk_ee;
        }
        if (value >= 3) {
            result = TRUE;
            break;
        }
        i++;
    } while (i < count);
    return result;
}

BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id) {
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
    (void) DataLayoutSlot();
    if (data_020d433c.mUnk_84) {
        return;
    }
    if (data_020d433c.mNextScene != SCENE_TOURNAMENT) {
        FrameTask::Get().StartChannel(0, sFrameTaskIds[data_020d433c.mFieldId]);
        return;
    }
    if (data_020d433c.mUnk_8a_0) {
        FrameTask::Get().StartChannel(0, 3);
        return;
    }
    if (func_0202326c(gSaveData)) {
        if (gSaveData[0].mUnk_42 == 9) {
            FrameTask::Get().StartChannel(0, 7);
        } else {
            FrameTask::Get().StartChannel(0, 5);
        }
        return;
    }
    if (data_020d433c.mUnk_8a_4) {
        FrameTask::Get().StartChannel(0, 10);
        return;
    }
    FrameTask::Get().StartChannel(0, sFrameTaskIds[data_020d433c.mFieldId]);
}

