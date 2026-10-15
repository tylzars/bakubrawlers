#include "Battle/BattleBase.hpp"
#include "Battle/BattleCharaAct.hpp"

#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/BattleCardSpec.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "System/WirelessManager.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include <nitro/fx.h>
#include <nitro/gx.h>
#include "Battle/BattleUtil.hpp"
#include "System/InputManager.hpp"

extern const u8 data_ov007_02279160;
extern const u8 data_ov007_02279164;
extern const u8 data_ov007_02279168;
extern const u8 data_ov007_0227916c;
extern const s16 data_ov007_02279174;
extern const u8 data_ov007_02279178[];
extern const s16 data_ov007_0227917c;

BOOL func_ov007_02229524(BattleField3D *field);

// Functions of other battle files.
void func_ov007_02229998(BattleField3D *obj);
void func_ov007_022252c4(BattleSub48 *obj);
void func_ov007_0226a448(BattlePlayer *player, u8 a, int b);
void func_ov007_0222844c(BattleHud *obj, u8 a, u8 b);
BOOL func_ov007_02226aa0(BattleHud *obj);
BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);
void func_ov007_022252a8(BattleSub48 *obj);
void func_ov007_02229758(BattleField3D *obj);
BOOL func_ov007_02229770(BattleField3D *obj);
BOOL func_ov007_022297f0(BattleField3D *obj);
void func_ov007_02226a44(BattleHud *obj, u32 a);
void func_ov007_0222847c(BattleHud *obj);
void func_ov007_02227434(BattleHud *obj);
extern const u8 data_ov007_0227915c;
extern const u8 data_ov007_02279170;
struct BattleRow {
    s32 mUnk[6];
};
extern const BattleRow data_ov007_02278b68[];
extern const s32 data_ov007_02278b74;
void func_ov007_02229d08(BattleBase *scene);
void func_ov007_02229c88(BattleBase *scene, u16 *a, u16 *b, u16 *c, u8 flag);

void func_ov007_02226a34(void *);
extern const s8 data_ov007_02279158[];
extern const s8 data_ov007_02279d3c[];
extern const s16 data_ov007_02278b1c[];




BattleBase::BattleBase()
    : IBattle(), SceneBase(0x3e8, 0) {
    mUnk_10 = 0;
    mUnk_11 = 0;
    mUnk_12 = 0;
    mUnk_13 = 0;
    mUnk_14 = -1;
    mUnk_15 = 0;
    mUnk_16 = 0;
    mUnk_18_0 = 0;
    mUnk_18_1 = 0;
    mUnk_18_2 = 0;
    mUnk_19 = 2;
    mUnk_1a = 0;
    mUnk_1b = 0;
    mUnk_24 = 0;
    mUnk_25 = 0;
    mUnk_26 = 0;
    mUnk_28 = 0;
    mUnk_29 = 2;
    mUnk_2a = 0;
    mUnk_2c = 0;
    mUnk_40 = 0;
    mUnk_44 = 0;
    mUnk_48 = 0;
    mUnk_4c = 0;
    mUnk_4d = 0;
    mUnk_4e = 0;
    mUnk_4f = 0;
    mUnk_50 = 0xff;
    mUnk_51 = 0;
    mUnk_54_0 = 0;
    mUnk_54_1 = 0;
    mUnk_54_2 = 0;
    mUnk_54_3 = 0;
    mUnk_54_4 = 0;
    mUnk_54_5 = 0;
    mUnk_55 = 0;
    mUnk_56 = 0;
    mUnk_58 = 0;
    mUnk_5c = 0;
    mSlots[0] = mSlots[1] = mSlots[2] = mSlots[3] = 0;
    mUnk_1c = mUnk_1d = -1;
    mUnk_1e = mUnk_1f = -1;
    mUnk_52[0] = mUnk_52[1] = 0;
    mHelpers[0] = mHelpers[1] = 0;
    if (data_020d433c.mUnk_95 != 0 && data_020d433c.mUnk_95 != 7) {
        mUnk_14 = data_020d433c.mUnk_95 - 1;
    }
}

BattleBase::~BattleBase() {
    if (mUnk_40) delete mUnk_40;
    mUnk_48->mUnk_0c = 4;
    func_ov007_02226a34(mUnk_44);
    if (mSlots[3]) delete mSlots[3];
    if (mSlots[2]) delete mSlots[2];
    if (mSlots[1]) delete mSlots[1];
    if (mSlots[0]) delete mSlots[0];
    if (mHelpers[1]) delete mHelpers[1];
    if (mHelpers[0]) delete mHelpers[0];
}

void BattleBase::func_ov007_02219888(BattleData *data, u32 mode) {
    mUnk_2c = data;
    mUnk_20 = &data_ov007_02278b20[mode];
    mUnk_15 = mode;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 sel = list->mUnk_2a9;
    BattleEntry *entries = data->mEntries;
    u8 first = entries[0].mUnk_0d_4;
    if (sel == first) {
        mUnk_12 = 0;
    } else if (sel == entries[1].mUnk_0d_4) {
        mUnk_12 = 1;
    } else {
        if (data_020d433c.mUnk_98 != 0) {
            if (sel == list->mPlayers[first]->mUnk_f2) {
                mUnk_12 = 3;
                if (data_020d433c.mUnk_7a != 0) {
                    mUnk_29 = 0;
                }
            } else {
                mUnk_12 = 4;
                if (data_020d433c.mUnk_7a != 0) {
                    mUnk_29 = 1;
                }
            }
        } else {
            mUnk_12 = 2;
        }
        mUnk_15 = 7;
    }
    if (data_020d433c.mPlayerCount == 3) {
        mUnk_13 = 30;
    } else if (data_020d433c.mPlayerCount == 4) {
        mUnk_13 = 15;
    }
    u8 order[2] = {0, 0};
    u8 count = 0;
    u8 i = 0;
    do {
        if (data_020d433c.mBattlePlayers->mPlayers[i]->IsActive()) {
            BattleSlot *slot = new BattleSlot(data, i, mode, data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_f6,
                                              entries[0].mUnk_0d_4, entries[1].mUnk_0d_4, mUnk_20, mUnk_13);
            mSlots[i] = slot;
            if (slot->mRole == 2 || slot->mRole == 3) {
                order[count++] = i;
            }
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    if (data_020d433c.mUnk_9a_2) {
        u8 j = 0;
        do {
            u8 nib = entries[j].mUnk_0d_4;
            if (!data_020d433c.mBattlePlayers->mPlayers[nib]->IsActive()) {
                mHelpers[j] = new BattleSideHelper(j, data_020d433c.mBattlePlayers->mPlayers[nib]->mUnk_f6, entries);
            }
            j++;
        } while (j < 2);
    }
    if (count == 2 && (mode == 0 || mode == 3)) {
        mSlots[order[0]]->mUnk_14 |= 4;
        mSlots[order[1]]->mUnk_14 |= 4;
    }
    func_ov007_02219888(data, mode);
    mUnk_44 = new BattleHud(data, mUnk_20, mUnk_12, (s8 *) &mUnk_1c);
    mUnk_48 = new BattleSub48(data, mUnk_12);
    mUnk_40 = new BattleField3D(data, 0);
    if (data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->func_020785d8(0xb);
    }
}

void BattleBase::Update() {
    BattleEntry *entries = mUnk_2c->Entries();
    switch (mUnk_10) {
        case 0:
            SpriteManager::Get().StartFade(2);
            mUnk_10++;
        case 1:
            if (func_ov007_02229524(mUnk_40)) {
                mUnk_10++;
            }
            break;
        case 2:
            if (mUnk_48->func_ov007_02224a70()) {
                mUnk_10++;
            }
            break;
        case 3: {
            s8 i       = 1;
            BOOL found = 0;
            do {
                u32 v = 0xff;
                if (entries[i].mUnk_12 != 0) {
                    v = func_02065c0c(entries[i].mUnk_12, 3);
                }
                if (v == 5) {
                    mUnk_4e = i;
                    found   = 1;
                    break;
                }
                i--;
            } while (i >= 0);
            if (found) {
                mUnk_54_0 = 1;
                mUnk_4c   = 5;
                mUnk_10   = 8;
                func_ov007_02226a44(mUnk_44, 0);
            } else {
                mUnk_10++;
            }
            break;
        }
        case 4: {
            s8 i       = 1;
            BOOL found = 0;
            do {
                if (entries[i].mUnk_12 != 0) {
                    u8 idx = i;
                    if (func_ov007_02222764(idx, entries) == 5) {
                        mUnk_4e = idx;
                        found   = 1;
                        break;
                    }
                }
                i--;
            } while (i >= 0);
            if (found) {
                mUnk_54_0 = 1;
                mUnk_4c   = 5;
                mUnk_10   = 8;
                func_ov007_02226a44(mUnk_44, 0);
            } else {
                func_ov007_02226a44(mUnk_44, 1);
                mUnk_10++;
            }
            break;
        }
        case 5: {
            s8 i       = 1;
            BOOL found = 0;
            do {
                u16 card = entries[i].mUnk_12;
                if (card >= 0x8e && card <= 0x91) {
                    mUnk_4e = i;
                    found   = 1;
                    break;
                }
                i--;
            } while (i >= 0);
            if (found) {
                mUnk_54 |= 0x20;
                mUnk_10 = 8;
                func_ov007_02226a44(mUnk_44, 0);
            } else {
                func_ov007_02226a44(mUnk_44, 1);
                mUnk_10++;
            }
            break;
        }
        case 6:
            if (BattleBase::func_ov007_0221bd84()) {
                u8 i = 0;
                do {
                    if (entries[i].mUnk_0f_2) {
                        mUnk_52[i] = 1;
                        mUnk_44->func_ov007_02227bbc(i);
                    }
                    i++;
                } while (i < 2);
                func_ov007_0221bd84();
                mUnk_10++;
            }
            break;
        case 7:
            if (!func_ov007_0221bb84()) {
                vfunc_34();
                func_ov007_0221c938();
            } else {
                mUnk_1c = mUnk_1d = -1;
                mUnk_1e = mUnk_1f = -1;
                mUnk_10 = 14;
            }
            break;
        case 8:
            mUnk_4d = 0;
            mUnk_10++;
        case 9:
            if (func_ov007_0221a7d0()) {
                if (mUnk_4f == 2 || mUnk_54_5 == 1) {
                    mUnk_10 = 12;
                } else {
                    if (mUnk_54_0) {
                        mUnk_40 = new BattleField3D(mUnk_2c, 2);
                    } else {
                        mUnk_40 = new BattleField3D(mUnk_2c, 1);
                        SoundManager::Get().func_020246f8(0x24);
                    }
                    switch (mUnk_4f) {
                        case 0:
                            SoundManager::Get().func_020246f8(0x2f);
                            func_020247fc(&SoundManager::Get(), 0x400);
                            if (mUnk_44->mUnk_13 == 0x32) {
                                func_ov007_02226a44(mUnk_44, 4);
                                mUnk_4f = 4;
                            } else {
                                func_ov007_02226a44(mUnk_44, 2);
                            }
                            func_ov007_022252a8(mUnk_48);
                            break;
                        case 1:
                            if ((mUnk_4e == 0 && mUnk_12 == 1) || (mUnk_4e == 1 && mUnk_12 == 0)) {
                                mUnk_44->func_ov007_02228530(mUnk_56);
                                mUnk_55 = 10;
                            } else {
                                u8 j = 0;
                                do {
                                    if (data_020d433c.mBattlePlayers->mPlayers[j]->IsActive()) {
                                        mSlots[j]->func_ov007_02223484(mUnk_4e);
                                    }
                                    j++;
                                } while (j < data_020d433c.mPlayerCount);
                            }
                            func_ov007_0223e214(mUnk_2c, mUnk_4e, 1);
                            func_ov007_02226a44(mUnk_44, 4);
                            func_ov007_022252a8(mUnk_48);
                            break;
                        case 3:
                            mUnk_52[mUnk_4e] = 1;
                            mUnk_44->func_ov007_02227bbc(mUnk_4e);
                            func_ov007_02226a44(mUnk_44, 4);
                            func_ov007_022252a8(mUnk_48);
                            break;
                        case 4:
                            if (!mUnk_54_0) {
                                func_ov007_02226a44(mUnk_44, 4);
                                func_ov007_022252a8(mUnk_48);
                            }
                            break;
                    }
                    mUnk_10++;
                }
            }
            break;
        case 10:
            if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                SpriteManager::Get().StartFade(2);
                mUnk_10++;
            }
            break;
        case 11:
            if (!SpriteManager::Get().IsFading()) {
                func_ov007_0221b8e8();
            }
            break;
        case 12:
            if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                SoundManager::Get().func_020246f8(0x32);
                SpriteManager::Get().StartFade(4);
                mUnk_10++;
            }
            break;
        case 13:
            if (!SpriteManager::Get().IsFading()) {
                RequestScene(1);
                mFinished = 1;
            }
            break;
        case 14:
            func_ov007_0221bfa4();
            if (BattleBase::func_ov007_0221bfa4()) {
                RequestScene(1);
                mFinished = 1;
            }
            break;
    }
    if (mUnk_18_2) {
        if (func_ov007_0221d398()) {
            mUnk_18_2 = 0;
        }
    }
    if (!mUnk_54_3) {
        vfunc_3c();
    }
    if (mUnk_40) {
        mUnk_40->func_ov007_02229b2c(mUnk_44->mUnk_13);
    }
}

void BattleBase::func_ov007_0221a55c() {
    SpriteManager::Get().func_02026dec(0, 7);
    GX_SetGraphicsMode(1, 0, 1);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(2);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(2);
}

void BattleBase::func_ov007_0221a624() {
    mUnk_51 = 7;
    G2S_SetBG2Priority(0);
    G2S_SetBlendAlpha(4, 0x1b, 0, mUnk_51);
}

BOOL BattleBase::func_ov007_0221a664() {
    u8 ev = ++mUnk_51;
    if (ev >= 0x1f) {
        mUnk_51 = 0x1f;
        return 1;
    }
    G2S_SetBlendAlpha(4, 0x1b, 0, ev);
    return 0;
}

void BattleBase::func_ov007_0221a6b4() {
    BattleEntry *entries = mUnk_2c->mEntries;
    mUnk_56 = entries[mUnk_4e].mUnk_12;
    mUnk_4f = func_02065c0c(entries[mUnk_4e].mUnk_12, 3);
    mUnk_2a = 0;
    func_ov007_0226a448(data_020d433c.mBattlePlayers->mPlayers[entries[mUnk_4e].mUnk_0d_4], entries[mUnk_4e].mUnk_11, 2);
    BattleBase::func_ov007_0221a624();
    mUnk_54_3 = 1;
    if (!mUnk_54_0) {
        mUnk_44->mUnk_22 &= ~0x10;
    }
    func_ov007_0222844c(mUnk_44, mUnk_4e, mUnk_4f);
    func_ov007_022252c4(mUnk_48);
    mUnk_1d = -1;
    mUnk_1c = mUnk_1d;
    mUnk_1f = -1;
    mUnk_1e = mUnk_1f;
    if (entries[0].mUnk_12 == 0x8d && entries[1].mUnk_12 == 0x8d) {
        mUnk_54_2 = 1;
    }
    if (entries[0].mUnk_12 == 0x8c || entries[1].mUnk_12 == 0x8c) {
        mUnk_54_2 = 1;
    }
}

static inline Camera *GetCam() {
    return data_020d433c.mBattlePlayers->mCamera;
}
BOOL BattleBase::func_ov007_0221a7d0() {
    BattleEntry *entries = mUnk_2c->mEntries;
    switch (mUnk_4d) {
        case 0:
            SoundManager::Get().func_02024754(1);
            SoundManager::Get().func_020246f8(0x30);
            SpriteManager::Get().StartFade(4);
            mUnk_4d++;
        case 1:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            if (mUnk_40) {
                delete mUnk_40;
            }
            mUnk_40 = 0;
            func_ov007_0221a6b4();
            mUnk_4d++;
            break;
        case 2:
            if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                func_ov007_0221a55c();
                new BattleCharaAct(GetCam(), entries[mUnk_4e].mUnk_0d_4, 6, (func_02065c20(mUnk_56) - 0x190) / 100, 0);
                mUnk_4d++;
            }
            break;
        case 3:
            if (mContext->mUnk_12 != 0) {
                mContext->mUnk_12 = 0;
                SpriteManager::Get().func_0202740c();
                mUnk_44->mUnk_118[mUnk_4e] = 0xff;
                mUnk_4d++;
            }
            break;
        case 4:
            mUnk_44->func_ov007_022284a0(mUnk_4e);
            mUnk_58 = (u32) new BattleCommandPage(mUnk_56, -1);
            mUnk_4d++;
            break;
        case 5:
            if (mUnk_54_4) {
                if (mUnk_44->func_ov007_02228514()) {
                    mUnk_54_4 = 0;
                    SoundManager::Get().func_020246f8(0x33);
                }
            }
            mUnk_2a++;
            if (mUnk_2a > 0xf0) {
                mUnk_2a = 0;
                func_02055624((BattleCommandPage *) mUnk_58, 1);
                mUnk_4d = 6;
                if (mUnk_54_5 && mUnk_4f == 4 && func_ov007_0221ba0c((int) this, mUnk_4e, entries)) {
                    mUnk_4d = 7;
                } else if (!mUnk_54_1 && mUnk_54_0 && func_ov007_0221b998((int) this, mUnk_4e, entries)) {
                    mUnk_4d = 8;
                } else if (!mUnk_54_1 && mUnk_54_0 && func_ov007_0221b9d0((int) this, mUnk_4e, entries)) {
                    mUnk_54_4 = 1;
                    mUnk_4d   = 8;
                } else if (mUnk_4f != 4 && func_ov007_0221ba38((int) this, mUnk_4e, mUnk_4f, entries)) {
                    mUnk_4d = 9;
                }
                if (mUnk_4d != 6) {
                    SoundManager::Get().func_020246f8(0x30);
                    func_02055624((BattleCommandPage *) mUnk_58, 2);
                }
            }
            break;
        case 6:
            if (mContext->mUnk_12 != 0) {
                mContext->mUnk_12 = 0;
                if (mUnk_4f == 4) {
                    if (mUnk_54_5) {
                        if (entries[0].mUnk_00 == entries[1].mUnk_00) {
                            u8 idx = MATH_Rand32(&data_020d42e8, 2);
                            BattleData *d = mUnk_2c;
                            d->mEntries[idx].mUnk_10   = 1;
                            d->mEntries[idx].mUnk_0f_6 = 0;
                        } else if (entries[0].mUnk_00 > entries[1].mUnk_00) {
                            BattleData *d = mUnk_2c;
                            d->mEntries[0].mUnk_10   = 1;
                            d->mEntries[0].mUnk_0f_6 = 0;
                        } else {
                            BattleData *d = mUnk_2c;
                            d->mEntries[1].mUnk_10   = 1;
                            d->mEntries[1].mUnk_0f_6 = 0;
                        }
                        mUnk_4d = 0x12;
                    } else if (mUnk_54_0) {
                        mUnk_4d = 0xe;
                    } else {
                        mUnk_44->func_ov007_022284e0(mUnk_4e);
                        mUnk_4d = 0x12;
                    }
                } else if (mUnk_54_0) {
                    mUnk_4d = 0xe;
                } else {
                    int k = func_ov007_0221ba88((int) this, mUnk_4e, entries);
                    if (k) {
                        mUnk_2c->func_ov007_0223e1a4(k, mUnk_4e);
                    }
                    switch (mUnk_4f) {
                        case 0:
                            mUnk_4d = 0xb;
                            break;
                        case 1:
                        case 2:
                        case 3:
                            mUnk_44->func_ov007_022284c0(mUnk_4e);
                            mUnk_4d = 0x12;
                            break;
                    }
                }
                mUnk_2c->func_ov007_0223e1cc(mUnk_4e, mUnk_4f);
            }
            break;
        case 7:
            if (mContext->mUnk_12 != 0) {
                mContext->mUnk_12 = 0;
                mUnk_4d           = 10;
                mUnk_2c->func_ov007_0223e1cc(mUnk_4e, mUnk_4f);
            }
            break;
        case 8:
            if (mContext->mUnk_12 != 0) {
                mContext->mUnk_12 = 0;
                mUnk_54_1         = 1;
                mUnk_4d           = 10;
                mUnk_2c->func_ov007_0223e1cc(mUnk_4e, mUnk_4f);
            }
            break;
        case 9:
            if (mContext->mUnk_12 != 0) {
                mContext->mUnk_12 = 0;
                mUnk_54_4         = 1;
                mUnk_4d           = 10;
                if (mUnk_4f == 3) {
                    mUnk_4f = 4;
                }
                mUnk_2c->func_ov007_0223e1cc(mUnk_4e, mUnk_4f);
            }
            break;
        case 11: {
            func_ov007_0221a55c();
            u8 side = mUnk_4e;
            mUnk_5c = (u32) new BattleCardSpec(0xb, entries[(u8) (side ^ 1)].mUnk_0c, entries[(u8) (side ^ 1)].mUnk_0d_0,
                                               entries[(u8) (side ^ 1)].mUnk_0e == 0x15 ? 1 : 0);
            mContext->mUnk_12 = 0;
            mUnk_2a           = 0x40;
            mUnk_4d++;
            break;
        }
        case 12:
            mUnk_2a++;
            if (mUnk_2a > 0xa0) {
                mUnk_2a = 0;
                BattleCardSpec *spec = (BattleCardSpec *) mUnk_5c;
                spec->mUnk_1e = 0;
                func_ov007_02273e3c(spec->mUnk_0c);
                mUnk_4d++;
            }
            break;
        case 13:
            if (mContext->mUnk_12 != 0) {
                mContext->mUnk_12 = 0;
                SpriteManager::Get().func_0202740c();
                mUnk_44->func_ov007_022284c0(mUnk_4e);
                mUnk_4d = 0x12;
            }
            break;
        case 10:
            if (mUnk_44->func_ov007_02228514()) {
                s32 other = mUnk_4e ^ 1;
                mUnk_4e   = other;
                mUnk_56   = entries[other & 0xff].mUnk_12;
                mUnk_50   = mUnk_4f;
                mUnk_4f   = func_02065c0c(entries[mUnk_4e].mUnk_12, 3);
                func_ov007_0226a448(data_020d433c.mBattlePlayers->mPlayers[entries[mUnk_4e].mUnk_0d_4], entries[mUnk_4e].mUnk_11, 2);
                mUnk_44->mUnk_118[mUnk_4e] = 0;
                mUnk_4d                    = 2;
            }
            break;
        case 14:
            func_ov007_0221a55c();
            if (mUnk_54_2) {
                mUnk_56 = 0x8c;
            }
            switch (mUnk_56) {
                case 0x8c: {
                    u8 side = mUnk_4e;
                    if (mUnk_12 >= 2) {
                        side = 0;
                    }
                    mUnk_5c = (u32) new BattleCardSpec(0xc, entries[side].mUnk_0c, entries[side].mUnk_0d_0, entries[side].mUnk_0e == 0x15 ? 1 : 0);
                    mUnk_2c->mEntries[0].mUnk_0a = entries[0].mUnk_06 + entries[0].mUnk_08;
                    mUnk_2c->mEntries[1].mUnk_0a = entries[1].mUnk_06 + entries[1].mUnk_08;
                    mUnk_2c->mEntries[0].mUnk_02 = entries[0].mUnk_04;
                    mUnk_2c->mEntries[1].mUnk_02 = entries[1].mUnk_04;
                    mUnk_54_1                    = 1;
                    mUnk_2c->mUnk_34[4]->mUnk_0f = 1;
                    s8 v = data_ov007_02279158[0];
                    mUnk_2c->mUnk_34[4]->mUnk_11 = v;
                    mUnk_2c->mUnk_34[3]->mUnk_11 = v;
                    break;
                }
                case 0x8d: {
                    mUnk_4e ^= 1;
                    mUnk_5c = (u32) new BattleCardSpec(0xc, entries[mUnk_4e].mUnk_0c, entries[mUnk_4e].mUnk_0d_0, entries[mUnk_4e].mUnk_0e == 0x15 ? 1 : 0);
                    mUnk_2c->mEntries[mUnk_4e].mUnk_0a = entries[mUnk_4e].mUnk_06 + entries[mUnk_4e].mUnk_08;
                    mUnk_2c->mEntries[mUnk_4e].mUnk_02 = entries[mUnk_4e].mUnk_04;
                    s8 v = data_ov007_02279158[0];
                    mUnk_2c->mUnk_34[4]->mUnk_0f = 1;
                    mUnk_2c->mUnk_34[4]->mUnk_11 = v;
                    mUnk_2c->mUnk_34[3]->mUnk_11 = v;
                    break;
                }
                default: {
                    if (mUnk_54_1 && mUnk_12 != 1) {
                        mUnk_4e = 0;
                    }
                    mUnk_5c = (u32) new BattleCardSpec(5, entries[mUnk_4e].mUnk_0c, entries[mUnk_4e].mUnk_0d_0, entries[mUnk_4e].mUnk_0e == 0x15 ? 1 : 0);
                    if (mUnk_54_1) {
                        mUnk_2c->mEntries[0].mUnk_02 = entries[0].mUnk_00 + entries[0].mUnk_0a;
                        mUnk_2c->mEntries[1].mUnk_02 = entries[1].mUnk_00 + entries[1].mUnk_0a;
                    } else {
                        mUnk_2c->mEntries[mUnk_4e].mUnk_02 = entries[mUnk_4e].mUnk_00 + entries[mUnk_4e].mUnk_0a;
                    }
                    mUnk_2c->mUnk_34[4]->mUnk_0f = 0;
                    s8 v = data_ov007_02279d3c[0];
                    mUnk_2c->mUnk_34[4]->mUnk_11 = v;
                    mUnk_2c->mUnk_34[3]->mUnk_11 = v;
                    break;
                }
            }
            mContext->mUnk_12 = 0;
            mUnk_2a           = 0x40;
            mUnk_4d++;
            break;
        case 15:
            func_ov007_0221a664();
            if (mUnk_54_1) {
                mUnk_2c->func_ov007_0223dfcc(0, 2, 8, 0x64, 0, 0, mUnk_2a, 0x40);
                mUnk_2c->func_ov007_0223dfcc(1, 2, 0xb8, 0x64, 0, 0, mUnk_2a, 0x40);
            } else {
                mUnk_2c->func_ov007_0223dfcc(mUnk_4e, 2, data_ov007_02278b1c[mUnk_4e], 0x64, 0, 0, mUnk_2a, 0x40);
            }
            mUnk_2a--;
            if (mUnk_2a == 0) {
                mUnk_2a = 0;
                mUnk_4d++;
            }
            break;
        case 16: {
            u8 flag = 0;
            if (mUnk_54_1) {
                if (mUnk_2c->func_ov007_0223dd6c()) {
                    flag = 1;
                }
                mUnk_44->mUnk_b0[0] = (u32) entries[0].mUnk_00;
                mUnk_44->mUnk_b0[1] = (u32) entries[1].mUnk_00;
                mUnk_2c->func_ov007_0223dfcc(0, 2, 8, 0x64, 0, 1, 0, 0);
                mUnk_2c->func_ov007_0223dfcc(1, 2, 0xb8, 0x64, 0, 1, 0, 0);
            } else {
                if (mUnk_2c->func_ov007_0223ddac(mUnk_4e)) {
                    flag = 1;
                }
                mUnk_44->mUnk_b0[mUnk_4e] = (u32) entries[mUnk_4e].mUnk_00;
                mUnk_2c->func_ov007_0223dfcc(mUnk_4e, 2, data_ov007_02278b1c[mUnk_4e], 0x64, 0, 1, 0, 0);
            }
            if (flag) {
                if (mUnk_56 == 0x8d) {
                    mUnk_4e ^= 1;
                }
                if (mUnk_50 == 5 && mUnk_4f == 4) {
                    mUnk_44->func_ov007_022284e0(mUnk_4e);
                } else if (mUnk_54_1) {
                    mUnk_44->func_ov007_022284c0(0);
                    mUnk_44->func_ov007_022284c0(1);
                } else {
                    mUnk_44->func_ov007_022284c0(mUnk_4e);
                }
                BattleCardSpec *spec = (BattleCardSpec *) mUnk_5c;
                spec->mUnk_1e = 0;
                func_ov007_02273e3c(spec->mUnk_0c);
                mUnk_4d++;
            }
            break;
        }
        case 17:
            if (mContext->mUnk_12 != 0) {
                mContext->mUnk_12 = 0;
                SpriteManager::Get().func_0202740c();
                SpriteManager::Get().StartFade(3);
                mUnk_4d                      = 0x12;
                mUnk_2c->mUnk_34[4]->mUnk_0f = 0;
                s8 v = data_ov007_02279d3c[0];
                mUnk_2c->mUnk_34[4]->mUnk_11 = v;
                mUnk_2c->mUnk_34[3]->mUnk_11 = v;
            }
            break;
        case 18:
            if (!mUnk_44->func_ov007_02228514()) {
                break;
            }
            func_ov007_0221a55c();
            func_ov007_0221a624();
            mUnk_54_3 = 0;
            mUnk_44->mUnk_117_3 = 0;
            mUnk_44->func_ov007_02226614();
            mUnk_48->func_ov007_02224270();
            mUnk_4d++;
        case 19:
            return 1;
    }
    return 0;
}

void BattleBase::func_ov007_0221b8e8() {
    BOOL done = 0;
    if (mUnk_54_0) {
        done = 1;
        mUnk_10 = mUnk_4c;
    } else {
        switch (mUnk_4f) {
        case 0:
            if (!func_ov007_02226aa0(mUnk_44)) {
                mUnk_10 = 7;
                done = 1;
            }
            break;
        case 1:
        case 3:
        case 4:
            if (!func_ov007_02226aa0(mUnk_44)) {
                mUnk_10 = 7;
                done = 1;
            }
            break;
        }
    }
    if (done) {
        mUnk_44->mUnk_117 &= ~4;
        mUnk_54_0 = 0;
    }
}

BOOL func_ov007_0221b998(int a, int mode, BattleEntry *entry) {
    if (mode != 1) {
        return 0;
    }
    BOOL result = 0;
    if (entry->mUnk_12 != 0) {
        if (func_02065c0c(entry->mUnk_12, 3) == 5) {
            result = 1;
        }
    }
    return result;
}

BOOL func_ov007_0221b9d0(int a, int mode, BattleEntry *entry) {
    if (mode != 1) {
        return 0;
    }
    BOOL result;
    if (entry->mUnk_12 != 0) {
        result = 0;
        if (func_ov007_02222764(result, entry) == 5) {
            result = 1;
        }
    } else {
        result = 0;
    }
    return result;
}

BOOL func_ov007_0221ba0c(int a, int mode, BattleEntry *entry) {
    if (mode != 1) {
        return 0;
    }
    if (entry->mUnk_12 >= 0x8e && entry->mUnk_12 <= 0x91) {
        return 1;
    }
    return 0;
}

BOOL func_ov007_0221ba38(int a, u8 side, int value, BattleEntry *entries) {
    u8 other = side ^ 1;
    if (entries[other].mUnk_12 == 0) {
        return 0;
    }
    BOOL result = 0;
    int r = func_ov007_02222764(other, entries);
    if (r != -1) {
        if (value == r) {
            result = 1;
        }
    }
    return result;
}

int func_ov007_0221ba88(int a, int index, BattleEntry *entries) {
    int result = 0;
    if (func_02065c0c(entries[index].mUnk_12, 3) == 2) {
        switch (entries[index].mUnk_12) {
        case 0x78: result = 1; break;
        case 0x79: result = 2; break;
        case 0x7a: result = 3; break;
        case 0x7b: result = 4; break;
        case 0x7c: result = 5; break;
        case 0x7d: result = 6; break;
        case 0x7e: result = 7; break;
        case 0x7f: result = 8; break;
        }
    }
    return result;
}

void BattleBase::func_ov007_0221bb28(s16 a, s16 b) {
    BattleHud *p = mUnk_44;
    p->mUnk_12c = a;
    p->mUnk_12e = b;
}

void BattleBase::func_ov007_0221bb3c() {
    if (mUnk_18_1) {
        return;
    }
    mUnk_18_1 = 1;
    mUnk_44->mUnk_23 |= 4;
}

extern "C" void G2x_SetMasterBrightness_(vu16 *reg, int value);

// Differs from the shared MATH_Rand32: max == 0 returns the raw high word.

void BattleBase::func_ov007_0221bb68() {
    mUnk_25 = mUnk_20->mUnk_0b;
    mUnk_24 = mUnk_20->mUnk_08;
}

BOOL BattleBase::func_ov007_0221bb84() {
    BattleHud *sub = mUnk_44;
    u8 n = sub->mUnk_13;
    BOOL done = 0;
    mUnk_16++;
    if (mUnk_16 >= mUnk_20->mUnk_02) {
        if (n == data_ov007_0227915c) {
            u8 idx = MATH_Rand32(&data_020d42e8, 2);
            BattleData *d = mUnk_2c;
            d->mEntries[idx].mUnk_10 = 1;
            d->mEntries[idx].mUnk_0f_6 = 0;
        } else if (n > data_ov007_0227915c) {
            BattleData *data = mUnk_2c;
            data->mEntries[0].mUnk_10 = 1;
            data->mEntries[0].mUnk_0f_6 = 0;
        } else {
            BattleData *data = mUnk_2c;
            data->mEntries[1].mUnk_10 = 1;
            data->mEntries[1].mUnk_0f_6 = 0;
        }
        done = 1;
    } else if (n >= data_ov007_02279170 - 1) {
        BattleData *data = mUnk_2c;
        done = 1;
        data->mEntries[0].mUnk_10 = 1;
        data->mEntries[0].mUnk_0f_6 = 1;
        mUnk_44->mUnk_22 |= 0x20;
    } else if (n <= 1) {
        BattleData *data = mUnk_2c;
        done = 1;
        data->mEntries[1].mUnk_10 = 1;
        data->mEntries[1].mUnk_0f_6 = 1;
        mUnk_44->mUnk_22 |= 0x20;
    }
    if (done) {
        func_ov007_0222847c(mUnk_44);
        mUnk_18_0 = 1;
        func_02024764(&SoundManager::Get(), 0x24, 1);
    } else if (!mUnk_18_0) {
        func_ov007_02227434(mUnk_44);
    }
    return done;
}

BOOL BattleBase::func_ov007_0221bd84() {
    BOOL done = 0;
    switch (mUnk_11) {
    case 0:
        if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
            func_ov007_022252a8(mUnk_48);
            SpriteManager::Get().StartFade(2);
            mUnk_11++;
        }
        break;
    case 1:
        mUnk_2a++;
        if (mUnk_2a >= 0x3c) {
            mUnk_2a = 0;
            mUnk_11++;
        }
        break;
    case 2:
        if (!func_ov007_02226aa0(mUnk_44)) {
            func_ov007_02229758(mUnk_40);
            mUnk_11++;
        }
        break;
    case 3:
        if (func_ov007_02229770(mUnk_40)) {
            SoundManager::Get().func_020246f8(0x23);
            SoundManager::Get().func_020246f8(0x24);
            mUnk_18_2 = 1;
            mUnk_44->mUnk_22 |= 0x80;
            mUnk_44->mUnk_22 |= 0x10;
            mUnk_11++;
        }
        break;
    case 4:
        if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0x10)) {
            mUnk_11 = 0;
            done = 1;
        }
        break;
    }
    return done;
}

BOOL BattleBase::func_ov007_0221bfa4() {
    BOOL done = 0;
    switch (mUnk_11) {
    case 0:
        func_ov007_02226a44(mUnk_44, 3);
        mUnk_18_2 = 1;
        mUnk_11++;
        break;
    case 1:
        if (!func_ov007_02226aa0(mUnk_44)) {
            if (func_ov007_022297f0(mUnk_40)) {
                mUnk_2a = 0;
                mUnk_11++;
            }
        }
        break;
    case 2:
        mUnk_2a++;
        if (mUnk_2a >= 0x64) {
            mUnk_2a = 0;
            SpriteManager::Get().StartFade(3);
            mUnk_11++;
        }
        break;
    case 3:
        if (!SpriteManager::Get().IsFading()) {
            mUnk_11++;
        }
        break;
    case 4:
        if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
            done = 1;
        }
        break;
    }
    return done;
}

int BattleBase::func_ov007_0221c178(u16 *a, u16 *b, u16 *c, u8 *flag) {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    int result             = -1;
    u8 side                = list->mUnk_2a9;
    BattleEntry *entries   = mUnk_2c->mEntries;
    if (mUnk_12 <= 1) {
        if (mUnk_55 != 0) {
            if (TouchManager::Get().func_02022860(data_ov007_02279174, data_ov007_0227917c, data_ov007_02279168,
                                                  data_ov007_02279164)) {
                SoundManager::Get().func_020246f8(0x31);
                mUnk_44->mUnk_116 = 5;
                mUnk_55--;
                if (mUnk_55 == 0) {
                    u8 other = mUnk_12 ^ 1;
                    WirelessManager::Get();
                    if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                        func_ov007_0223e214(mUnk_2c, other, 0);
                    } else {
                        *flag = other + 1;
                    }
                    mUnk_44->func_ov007_022286c8();
                }
            }
        } else {
            if (data_020d433c.mUnk_9a_2) {
                WirelessManager::Get();
                if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                    if (mHelpers[mUnk_12]->func_ov007_02223638(mUnk_12, mUnk_44->mUnk_13)) {
                        c[mUnk_12] = 1;
                    }
                }
            } else if (entries[mUnk_12].mUnk_12 != 0) {
                if (mUnk_44->mUnk_118[mUnk_12] == 1) {
                    if (func_02065c0c(entries[mUnk_12].mUnk_12, 3) != 4) {
                        if (TouchManager::Get().func_02022860(data_ov007_02279178[mUnk_12], data_ov007_0227916c, 0x10, 0x18) ||
                            InputManager::Get().IsHeld(0x300)) {
                            c[mUnk_12] = 1;
                        }
                    }
                }
            }
            if (c[mUnk_12] == 0) {
                u8 n   = mUnk_44->mUnk_13;
                u8 low = 0;
                if (mUnk_12 == 0 && n < 0x32) {
                    low = 1;
                } else if (mUnk_12 == 1 && (u8) (100 - n) < 0x32) {
                    low = 1;
                }
                u8 r = vfunc_40(low);
                if (r) {
                    result = (s8) mUnk_12;
                    if (mUnk_12 == 0) {
                        a[side] = r;
                    } else {
                        b[side] = r;
                    }
                }
            }
        }
    } else {
        u8 base = data_ov007_02279160 - 0x10;
        if (mUnk_19 == 2) {
            if (mUnk_12 == 2 || mUnk_12 == 3) {
                if (func_ov007_02229ee4((u16) mUnk_44->mUnk_18, base, 0x50, 0x90, 10, &mUnk_1a)) {
                    mUnk_19 = 0;
                    mUnk_1b = 0;
                    func_ov007_0221bb3c();
                }
            }
            if (mUnk_19 == 2) {
                if (mUnk_12 == 2 || mUnk_12 == 4) {
                    if (func_ov007_02229ee4((u16) ((u16) mUnk_44->mUnk_1a - 0x10), base, 0x50, 0x90, 10, &mUnk_1a)) {
                        mUnk_19 = 1;
                        mUnk_1b = 0;
                        func_ov007_0221bb3c();
                    }
                }
            }
        } else if (TouchManager::Get().mTouching) {
            mUnk_19 = 2;
            mUnk_1a = 0;
        } else {
            u16 v;
            if (mUnk_19 == 0) {
                v = (u16) mUnk_44->mUnk_18;
            } else {
                v = (u16) mUnk_44->mUnk_1a - 0x10;
            }
            if (func_ov007_02229ee4(v, base, 0x50, 0x90, 10, &mUnk_1a)) {
                WirelessManager::Get();
                if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                    mUnk_1b++;
                    if (mUnk_1b >= 0x14) {
                        mUnk_1b = 0;
                        result = (s8) mUnk_19;
                        if (mUnk_19 == 0) {
                            a[side] = mUnk_13;
                        } else {
                            b[side] = mUnk_13;
                        }
                    }
                    s8 *pair = (s8 *) this + mUnk_19 * 2;
                    if (pair[0x1c] == -1) {
                        pair[0x1c] = side;
                    } else {
                        pair[0x1d] = side;
                    }
                } else {
                    mUnk_1b++;
                    if (mUnk_1b >= 0x14) {
                        mUnk_1b = 0;
                        result = (s8) mUnk_19;
                        if (mUnk_19 == 0) {
                            a[side] = 2;
                        } else {
                            b[side] = 2;
                        }
                    } else {
                        if (mUnk_19 == 0) {
                            a[side] = 1;
                        } else {
                            b[side] = 1;
                        }
                    }
                }
            } else {
                mUnk_19 = 2;
                mUnk_1a = 0;
            }
        }
    }
    return result;
}

int BattleBase::func_ov007_0221c850(u16 *a, u16 *b, u16 *c, u8 *d) {
    u8 i = 0;
    do {
        if (data_020d433c.mBattlePlayers->mPlayers[i]->IsActive()) {
            if (mSlots[i]->func_ov007_02223260(a, b, c, (s8 (*)[2]) &mUnk_1c, mUnk_44->mUnk_13, d) != -1) {
                break;
            }
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    return -1;
}

void BattleBase::func_ov007_0221c8f8(u16 *p) {
    if (p[0] != 0) {
        mUnk_4e = 0;
        mUnk_10 = 8;
    } else if (p[1] != 0) {
        mUnk_4e = 1;
        mUnk_10 = 8;
    }
}

// non-matching: 6 words at 0x284-0x29c, the s16 conversion of the touch x/y arguments: the original loads both
// fields, then shifts each into r2/r3; ours converts each in place with the `this` load between the shifts.
// (The `(u16) value` cast fixes the value/TouchManager register swap.) func_ov007_0221d088 makes the same call with
// the in-place form and matches, so this call is written differently in the original.
void BattleBase::func_ov007_0221c938() {
    u16 a[4] = {0, 0, 0, 0};
    u16 b[4] = {0, 0, 0, 0};
    s32 sums[2] = {0, 0};
    u16 c[2] = {0, 0};
    u8 flag = 0;
    u32 sel = data_020d433c.mBattlePlayers->mUnk_2a9;
    mUnk_1d = -1;
    mUnk_1c = mUnk_1d;
    mUnk_1f = -1;
    mUnk_1e = mUnk_1f;
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4258)) {
        func_ov007_02229d08(this);
    }
    int found = func_ov007_0221c178(a, b, c, &flag);
    BOOL doneB = 1;
    BOOL doneA = 1;
    if (found != -1) {
        if (mUnk_12 <= 1) {
            mUnk_25++;
            if (mUnk_25 >= mUnk_20->mUnk_0b) {
                mUnk_25 = 0;
            } else {
                doneB = 0;
            }
            mUnk_24++;
            if (mUnk_24 >= mUnk_20->mUnk_08) {
                mUnk_24 = 0;
            } else {
                doneA = 0;
            }
        }
        u8 side;
        u8 k;
        u16 value = 0;
        if (a[sel] != 0) {
            side = 0;
            value = a[sel];
        } else if (b[sel] != 0) {
            value = b[sel];
            side = 1;
        }
        WirelessManager::Get();
        if (!IsWirelessMode(&data_020c4258)) {
            if (doneB) {
                k = 0;
                u32 n;
                do {
                    mUnk_44->func_ov007_022275c0(side, TouchManager::Get().mX, TouchManager::Get().mY, mUnk_15);
                    n = mUnk_44->func_ov007_022275a0((u16) value, mUnk_20->mUnk_09, mUnk_20->mUnk_0a);
                    k++;
                } while (k < n);
            }
            if (!doneA) {
                a[sel] = 0;
                b[sel] = 0;
            }
        } else {
            if (doneB) {
                u16 n = mUnk_44->func_ov007_022275a0(value, mUnk_20->mUnk_09, mUnk_20->mUnk_0a) << 8;
                if (a[sel] != 0) {
                    a[sel] |= n;
                } else {
                    b[sel] |= n;
                }
            }
            if (!doneA) {
                a[sel] &= 0xff00;
                b[sel] &= 0xff00;
            }
        }
    }
    WirelessManager::Get();
    if (!IsWirelessMode(&data_020c4258)) {
        func_ov007_0221c850(a, b, c, &flag);
        u8 i = 0;
        u8 count = data_020d433c.mPlayerCount;
        do {
            sums[0] += a[i];
            sums[1] += b[i];
            i++;
        } while (i < count);
        BattleEntry *entries = mUnk_2c->Entries();
        i = 0;
        do {
            if (sums[i] != 0) {
                s32 v = 0x1000;
                s8 cur = mUnk_14;
                if (cur != -1) {
                    v = data_ov007_02278b68[cur].mUnk[entries[i].mUnk_0d_0];
                    if (entries[i].mUnk_0c == 5) {
                        if (v < data_ov007_02278b68[cur].mUnk[3]) {
                            v = data_ov007_02278b68[cur].mUnk[3];
                        }
                    } else if (entries[i].mUnk_0c == 6) {
                        if (v < data_ov007_02278b68[cur].mUnk[0]) {
                            v = data_ov007_02278b68[cur].mUnk[0];
                        }
                    }
                }
                if (mUnk_29 == i && v < 0x1000) {
                    v = 0x1000;
                }
                mUnk_44->func_ov007_02227490(i, FX_Mul(v, mUnk_20->mUnk_04), sums[i], mUnk_52[i], 1);
            }
            i++;
        } while (i < 2);
        func_ov007_0221c8f8(c);
    } else {
        func_ov007_02229c88(this, a, b, c, flag);
    }
}

class Camera;




void BattleBase::func_ov007_0221cebc(int a) {
    func_ov007_0223e214(mUnk_2c, a, 0);
}

void BattleBase::func_ov007_0221ced0(u16 *a, u16 *b, u16 *c) {
    if (!data_020d433c.mUnk_9a_2) {
        return;
    }
    u8 i = 0;
    BattleEntry *entries = mUnk_2c->mEntries;
    do {
        if (mHelpers[i]) {
            u8 other = i ^ 1;
            if (!entries[other].mUnk_0f_3) {
                if (mHelpers[i]->func_ov007_02223638(i, mUnk_44->mUnk_13)) {
                    c[i] = 1;
                    if (i == 0) {
                        a[entries[0].mUnk_0d_4] = 0;
                    } else {
                        b[entries[1].mUnk_0d_4] = 0;
                    }
                }
            }
        }
        i++;
    } while (i < 2);
}

void BattleBase::func_ov007_0221cfac(u16 *a, u16 *b) {
    BattleData *data = mUnk_2c;
    u8 i = 0;
    do {
        if (i != data->mEntries[0].mUnk_0d_4 && i != data->mEntries[1].mUnk_0d_4) {
            u8 flag = a[i];
            if (flag != 0) {
                if (mUnk_1c == -1) {
                    mUnk_1c = i;
                } else {
                    mUnk_1d = i;
                }
                a[i] = a[i] & 0xff00;
                if (flag == 2) {
                    a[i] |= mUnk_13;
                }
            } else {
                flag = b[i];
                if (flag != 0) {
                    if (mUnk_1e == -1) {
                        mUnk_1e = i;
                    } else {
                        mUnk_1f = i;
                    }
                    b[i] = b[i] & 0xff00;
                    if (flag == 2) {
                        b[i] |= mUnk_13;
                    }
                }
            }
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
}

void BattleBase::func_ov007_0221d088(u8 a, u16 *counts) {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 sel = list->mUnk_2a9;
    u16 hi[4] = {0, 0, 0, 0};
    u8 i = 0;
    do {
        hi[i] = counts[i] >> 8;
        counts[i] &= 0xff;
        i++;
    } while (i < data_020d433c.mPlayerCount);
    u8 k;
    i = 0;
    do {
        if (hi[i] != 0) {
            k = 0;
            do {
                if (sel == i) {
                    mUnk_44->func_ov007_022275c0(a, TouchManager::Get().mX, TouchManager::Get().mY, mUnk_15);
                } else if (!data_020d433c.mBattlePlayers->mPlayers[i]->IsActive()) {
                    func_ov007_0221d348(0xffff);
                }
                k++;
            } while (k < hi[i]);
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    s32 total = 0;
    i = 0;
    do {
        total += counts[i];
        i++;
    } while (i < data_020d433c.mPlayerCount);
    if (total != 0) {
        s32 v = 0x1000;
        BattleEntry *entries = mUnk_2c->mEntries;
        s8 cur = mUnk_14;
        if (cur != -1) {
            BattleEntry *e = &entries[a];
            v = data_ov007_02278b68[cur].mUnk[e->mUnk_0d_0];
            if (e->mUnk_0c == 5) {
                if (v < data_ov007_02278b68[cur].mUnk[3]) {
                    v = data_ov007_02278b68[cur].mUnk[3];
                }
            } else if (e->mUnk_0c == 6) {
                if (v < data_ov007_02278b68[cur].mUnk[0]) {
                    v = data_ov007_02278b68[cur].mUnk[0];
                }
            }
        }
        mUnk_44->func_ov007_02227490(a, FX_Mul(v, mUnk_20->mUnk_04), total, mUnk_52[a], 1);
    }
    if (mUnk_20->mUnk_00 == 5) {
        vfunc_44(a, total);
    }
}

extern "C" void func_02024764(SoundManager *manager, u32 a, u32 b);

u32 func_ov007_0221d348(u32 max) {
    return MATH_Rand32(&data_020d42e8, max);
}

BOOL BattleBase::func_ov007_0221d398() {
    BOOL done = 0;
    if (mUnk_28 == 0) {
        mUnk_26 += 3;
        if (mUnk_26 >= 15) {
            if (mUnk_18_0) {
                func_ov007_02229998(mUnk_40);
                SoundManager::Get().func_020246f8(0x25);
                func_ov007_022252c4(mUnk_48);
            }
            mUnk_28++;
        }
    } else {
        mUnk_26 -= 3;
        if (mUnk_26 <= 0) {
            mUnk_26 = 0;
            mUnk_28 = 0;
            done = 1;
        }
    }
    G2x_SetMasterBrightness_((vu16 *) 0x400006c, mUnk_26);
    G2x_SetMasterBrightness_((vu16 *) 0x400106c, mUnk_26);
    return done;
}

void BattleBase::func_ov007_0221d488(u16 *p) {
    func_ov007_0221c8f8(p);
}

int BattleBase::func_ov007_0221d494(u16 *a, u16 *b, u16 *c, u8 *d) {
    return func_ov007_0221c850(a, b, c, d);
}
