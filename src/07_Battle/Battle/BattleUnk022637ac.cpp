#include "Battle/BattleUnk022637ac.hpp"
#include "Battle/BattleUnk29c.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPrompt.hpp"
#include "System/WirelessManager.hpp"
#include "Battle/BattleTarget.hpp"
#include "Battle/BattleUnionAbility.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Effect.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Graphics/UnkStruct_0203d374.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include <nitro/gx.h>
#include "Game/Random.hpp"
#include <nitro/math.h>

BattleUnk022637ac::BattleUnk022637ac()
    : SceneBase(0x3e8, 0) {
    mState = 0;
    mUnk_0d = 0;
    mUnk_0e = 0;
    mUnk_0f = 0;
    mUnk_11 = 0;
    mUnk_14 = 0;
    mUnk_15 = 0;
    mUnk_16 = 0;
    mUnk_17 = 1;
    mUnk_1a = 0;
    mUnk_20 = 0;
    mUnk_24 = 0;
    mUnk_28 = 0;
    mUnk_29 = 0xff;
    mUnk_2a = 0;
    mUnk_2b = 0;
    mEffects = data_020d433c.mEffects;
    mUnk_38 = 0;
    mUnk_3a = 0;
    mUnk_60 = 0;
    mUnk_61 = 0;
    mUnk_62_0 = 0;
    mUnk_62_1 = 0;
    mUnk_62_2 = 0;
    mUnk_63 = 0;
    mUnk_64 = 0;
    mUnk_68 = 0;
    mUnk_6c = 0;
    mUnk_6e = 0;
    mUnk_70 = 0;
    mUnk_15 = data_020d433c.mBattlePlayers->mUnk_28a;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 own = list->mUnk_2a9;
    mUnk_10 = list->mPlayers[mUnk_15]->mRecord;
    if (mUnk_15 != own) {
        mUnk_0d = 1;
        if (data_020d433c.mBattlePlayers->mPlayers[mUnk_15]->IsActive()) {
            mUnk_0e = 1;
        }
    }
    mUnk_1c = data_020d433c.mBattlePlayers->mUnk_298;
    mUnk_16 = mUnk_1c->mUnk_1e8 - 1;
    mUnk_18 = data_020d433c.mBattlePlayers->mPlayers[mUnk_15]->mUnk_fa;
    mUnk_11 = mUnk_18 - 0xc9;
    mUnk_30 = data_020d433c.mBattlePlayers->mCamera;
    u8 i = 0;
    do {
        mUnk_80[i] = 0;
        i++;
    } while (i < 2);
    func_ov007_02264788();
}

BattleUnk022637ac::~BattleUnk022637ac() {
    u8 i = 0;
    do {
        CellAnimation *a = mUnk_80[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 2);
    mEffects->func_0202be68();
    func_0202bc58(mEffects, 0);
}

BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);
extern UnkStruct_0220f540 data_0220f540;

// Copies the active camera into the 3D engine state and the camera manager's snapshot of it.
static inline void SnapshotCamera() {
    CameraManager &manager        = CameraManager::Get();
    Camera *camera                = manager.mCameras[manager.mActive];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mActive] = data_02211374.mProjection;
    manager.mView[manager.mActive]       = data_02211374.mView;
    manager.mMtxA[manager.mActive]       = *func_02032710();
    manager.mMtxB[manager.mActive]       = *func_02032604();
    manager.mMtxC[manager.mActive]       = *func_020323b8();
}

typedef u8 (BattleUnk022637ac::*BattleUnk022637acHandler)();
static BattleUnk022637acHandler data_ov007_0228a098[12] = {
    &BattleUnk022637ac::func_ov007_02265118,
    &BattleUnk022637ac::func_ov007_022652c8,
    &BattleUnk022637ac::func_ov007_0226547c,
    &BattleUnk022637ac::func_ov007_02265718,
    0,
    &BattleUnk022637ac::func_ov007_02265af8,
    &BattleUnk022637ac::func_ov007_02265fac,
    &BattleUnk022637ac::func_ov007_022663f0,
    &BattleUnk022637ac::func_ov007_022665ec,
    &BattleUnk022637ac::func_ov007_02266868,
    &BattleUnk022637ac::func_ov007_02266a28,
    &BattleUnk022637ac::func_ov007_02266a28,
};

void BattleUnk022637ac::Update() {
    switch (mState) {
        case 0:
            mUnk_24 = new BattleCommandPage(mUnk_18, -1);
            mUnk_14 = func_ov007_02264a5c();
            func_02055738(mUnk_24, 0x2000, 7, 1);
            mState++;
            break;
        case 1:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            func_ov007_02264990();
            break;
        case 2: {
            u8 result = mContext->mUnk_12;
            if (result == 0) {
                break;
            }
            mContext->mUnk_12 = 0;
            switch (result) {
                case 1:
                    mUnk_0f = 3;
                    break;
                case 2:
                    SpriteManager::Get().sFade[1] = 3;
                    mUnk_0f                         = 1;
                    break;
            }
            func_02055624(mUnk_24, 1);
            if ((u16) (mUnk_18 + 0xff35) <= 1 && result == 1) {
                mState++;
                break;
            }
            mState = 10;
            if (mUnk_18 == 0xcd) {
                SpriteManager::Get().sFade[1] = 3;
            }
            WirelessManager::Get();
            if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                break;
            }
            ((u8 *) &data_0220f540)[0] = 0x14;
            ((u8 *) &data_0220f540)[4] = mUnk_0f;
            break;
        }
        case 3:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            mContext->mUnk_12 = 0;
            if (mUnk_18 == 0xcb) {
                mUnk_24 = new BattleCommandPage(0, -1);
                func_0205562c(mUnk_24, 1);
                new BattleCommandPrompt(1, mUnk_24, 0, 0, 0);
            } else {
                mUnk_34 = new BattleCommandList(0, 0, 0, 0);
                func_02058228(mUnk_34, 1);
                new BattleCommandPrompt(2, mUnk_34, 0, 0, 0);
            }
            mState++;
            break;
        case 4: {
            u16 result = mContext->mUnk_12;
            if (result == 0) {
                break;
            }
            if (result != 10) {
                mUnk_2b = func_ov007_0225f0bc(result);
                WirelessManager::Get();
                if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                    ((u8 *) &data_0220f540)[0] = 0x14;
                    ((u8 *) &data_0220f540)[4] = mUnk_0f;
                    ((u8 *) &data_0220f540)[5] = mUnk_2b;
                }
                mState = 6;
            } else {
                mState = 5;
            }
            if (mUnk_18 == 0xcb) {
                func_02055624(mUnk_24, 1);
            } else {
                func_0205821c(mUnk_34);
            }
            mContext->mUnk_12 = 0;
            break;
        }
        case 5:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            mContext->mUnk_12 = 0;
            mUnk_24 = new BattleCommandPage(mUnk_18, -1);
            func_ov007_02264990();
            mState = 2;
            break;
        case 6:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            mContext->mUnk_12 = 1;
            if (mUnk_18 == 0xcc) {
                CameraManager::Get().func_020264e0();
                CameraManager::Get().mActive = 0;
                CameraManager::Get().func_02026504(1, mUnk_30, 0);
                SnapshotCamera();
            }
            mState = 10;
            break;
        case 7:
            if ((u8) mContext->mUnk_12 != 1) {
                break;
            }
            mContext->mUnk_12 = 0;
            mUnk_20->func_ov007_02234418(0xff);
            func_02055624(mUnk_24, 3);
            mUnk_17 = 2;
            mState  = 0x10;
            WirelessManager::Get();
            if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                break;
            }
            ((u8 *) &data_0220f540)[0] = 0x14;
            break;
        case 8: {
            BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mUnk_15];
            if (player->mMsg.mType != 0x14) {
                break;
            }
            mContext->mUnk_12 = 0;
            mUnk_20->func_ov007_02234418(0xff);
            if (mUnk_14 == 1) {
                func_02055624(mUnk_24, 3);
                mUnk_17 = 2;
                mState  = 0x10;
                break;
            }
            mUnk_0f = player->mMsg.mTouchDown;
            mUnk_2b = player->mMsg.mTouchX;
            func_02055624(mUnk_24, 1);
            mState = 10;
            if (mUnk_0f == 1 || mUnk_18 == 0xcd) {
                SpriteManager::Get().sFade[1] = 3;
            }
            break;
        }
        case 9:
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                mUnk_0f = mUnk_14 == 1 ? 2 : 3;
                mContext->mUnk_12 = 0;
                mUnk_20->func_ov007_02234418(0xff);
                func_02055624(mUnk_24, 1);
                mState = 10;
                if (mUnk_18 == 0xcd) {
                    SpriteManager::Get().sFade[1] = 3;
                }
            }
            break;
        case 10:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            mContext->mUnk_12 = 0;
            mUnk_1c->mUnk_1e7_1 = 1;
            mState++;
            break;
        case 11:
            if (!func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                break;
            }
            mState++;
        case 12:
            switch (mUnk_0f) {
                case 1:
                case 2:
                    mState = 0xe;
                    break;
                case 3:
                    if (mUnk_18 != 0xcd) {
                        mState = 0xd;
                        break;
                    }
                    data_020d433c.mBattlePlayers->mUnk_2ac = 3;
                    mState                                 = 0xe;
                    break;
            }
            break;
        case 13:
            if ((this->*data_ov007_0228a098[mUnk_11])()) {
                SpriteManager::Get().sFade[0] = 3;
                SpriteManager::sFade[1]        = 3;
                mState                           = 0xe;
            }
            break;
        case 14:
            if (!SpriteManager::Get().IsFading()) {
                mState = 0x12;
            }
            break;
        case 16:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            mContext->mUnk_12 = 0;
            mState++;
            break;
        case 17:
            if (!func_ov007_022631bc(data_020d433c.mBattlePlayers, 0x10)) {
                break;
            }
            mState++;
            break;
        case 18:
            RequestScene(mUnk_17);
            mFinished = 1;
            break;
    }
}

void BattleUnk022637ac::func_ov007_02264788() {
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.func_02027624(1);
    ChangeDir("/Game/Battle/Obj/");
    screen.LoadCharacters("vs_u_obj_num_32k.nce_ncg1D.bin", 0x6000, 0);
    screen.LoadPalette("vs_u_obj_num_32k_ncl.bin", 0x180, 0x40);
    u8 i = 0;
    do {
        mUnk_80[i] = new CellAnimation("vs_u_obj_num_32k_nce.bin", 0, 1, 1);
        mUnk_80[i]->mCharBase = screen.func_020276f4(0x6000);
        mUnk_80[i]->mUnk_11   = 12;
        i++;
    } while (i < 2);
    mUnk_80[0]->SetAnimation(0);
    mUnk_80[1]->SetAnimation(1);
    ChangeDir("/");
    ChangeDir("/Game/2D/union_b");
    func_02025b64(1, 4, 0x2000, "union_b_obj_page.nce_ncg1D.bin", 0, 0, 0);
    func_02025b64(1, 5, 0xe0, "union_b_obj_page_ncl.bin", 0, 0, 0);
    ChangeDir("/");
}

void BattleUnk022637ac::func_ov007_02264990() {
    if (mUnk_0d == 0) {
        switch (mUnk_14) {
            case 0:
                mUnk_20 = new BattleUnionAbility(3, 0x1a000, 0xc);
                mState  = 2;
                break;
            case 1:
                mUnk_20 = new BattleUnionAbility(5, 0x1a000, 0xc);
                mState  = 7;
                break;
        }
    } else {
        mUnk_20 = new BattleUnionAbility(4, 0x1a000, 0xc);
        if (mUnk_0e != 0) {
            mState = 9;
        } else {
            mState = 8;
        }
    }
}

BOOL func_ov007_022677cc(BattleUnk022637ac *self);
u32 func_ov007_02267884(BattleUnk022637ac *self);
BOOL func_ov007_02267944(BattleUnk022637ac *self, u16 *out);

// non-matching: 7 words in case 8: the original puts the player list in r4 (ours r3); it loads the player pointer into the register that held the mUnk_0f index and gives the mUnk_0e load a new one.
BOOL BattleUnk022637ac::func_ov007_02264a5c() {
    BOOL result = 1;
    switch (mUnk_18 - 0xc9) {
        case 0:
            result = 0;
            break;
        case 1:
            result = 0;
            break;
        case 2:
            if (*((u8 *) data_020d433c.mBattlePlayers->mPlayers[mUnk_15] + 0xf8) != 0) {
                result = 0;
            }
            if (mUnk_0e != 0 && result == 0) {
                u8 i        = 0;
                u8 slots[3] = {0, 0, 0};
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                u8 n        = 0;
                u8 count    = data_020d433c.mUnk_99;
                do {
                    if (((u8 *) list->mPlayers[mUnk_15] + 0x56)[i * 4] == 0) {
                        slots[n] = i;
                        n++;
                    }
                    i++;
                } while (i < count);
                mUnk_2b = slots[MATH_Rand32(&data_020d42e8, n)];
            }
            break;
        case 3: {
            u8 i             = 0;
            BattlePlayer *pl = data_020d433c.mBattlePlayers->mPlayers[mUnk_15];
            u8 count         = data_020d433c.mUnk_99;
            do {
                if (pl->mRecords[i].mUnk_0a == 0) {
                    result = 0;
                    break;
                }
                i++;
            } while (i < count);
            if (mUnk_0e != 0 && result == 0) {
                u8 n;
                BattlePlayerList *list;
                u8 count2;
                u8 j        = 0;
                u8 slots[3] = {0, 0, 0};
                count2   = data_020d433c.mUnk_99;
                list = data_020d433c.mBattlePlayers;
                n        = 0;
                do {
                    if (list->mPlayers[mUnk_15]->mRecords[j].mUnk_0a == 0) {
                        slots[n] = j;
                        n++;
                    }
                    j++;
                } while (j < count2);
                mUnk_2b = slots[MATH_Rand32(&data_020d42e8, n)];
            }
            break;
        }
        case 4:
            result = 0;
            break;
        case 5:
            if (mUnk_1c->mUnk_1d2 == 2) {
                u8 x;
                u8 y;
                u8 n         = 0;
                u8 found[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                x = 0;
                do {
                    y = 0;
                    do {
                        u8 v = data_020d433c.mBattlePlayers->mUnk_1f8[y][x];
                        if (v != 0) {
                            u8 unit = v - 1;
                            u8 me = mUnk_15;
                            u8 owner = data_020d433c.mBattlePlayers->mUnits[unit].mUnk_0f;
                            if (owner != me) {
                                if (data_020d433c.mUnk_98 != 0) {
                                    if (owner != data_020d433c.mBattlePlayers->mPlayers[me]->mUnk_f2) {
                                        found[n] = unit;
                                        n++;
                                    }
                                } else {
                                    found[n] = unit;
                                    n++;
                                }
                            }
                        }
                        y++;
                    } while (y < 14);
                    x++;
                } while (x < 10);
                if (n != 0) {
                    u8 pick = MATH_Rand32(&data_020d42e8, n);
                    mUnk_29 = found[pick];
                    result  = 0;
                }
            }
            break;
        case 6: {
            u8 pairs[12][2] = {0};
            u8 n;
            u8 y;
            u8 x;
            n                   = 0;
            x                   = 0;
            do {
                y = 0;
                do {
                    u8 v = data_020d433c.mBattlePlayers->mUnk_16c[y][x];
                    if (v != 0) {
                        pairs[n][0] = v - 1;
                        pairs[n][1] = 0xff;
                        u8 w        = data_020d433c.mBattlePlayers->mUnk_1f8[y][x];
                        if (w == 0) {
                            n++;
                        } else {
                            u8 unit  = w - 1;
                            u8 owner = data_020d433c.mBattlePlayers->mUnits[unit].mUnk_0f;
                            if (owner != mUnk_15) {
                                if (data_020d433c.mUnk_98 != 0) {
                                    if (owner != data_020d433c.mBattlePlayers->mPlayers[mUnk_15]->mUnk_f2) {
                                        pairs[n][1] = unit;
                                        n++;
                                    }
                                } else {
                                    pairs[n][1] = unit;
                                    n++;
                                }
                            }
                        }
                    }
                    y++;
                } while (y < 14);
                x++;
            } while (x < 10);
            if (n != 0) {
                u8 pick = MATH_Rand32(&data_020d42e8, n);
                mUnk_2a = pairs[pick][0];
                result  = 0;
                mUnk_29 = pairs[pick][1];
            }
            break;
        }
        case 7:
            if (mUnk_1c->mUnk_1d2 == 2) {
                result = 0;
            }
            break;
        case 8: {
            BattleUnit *unit      = &data_020d433c.mBattlePlayers->mUnits[mUnk_16];
            BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[unit->mUnk_0f]->mRecords[unit->mUnk_0e];
            u16 b                 = record->mUnk_06;
            if (record->mUnk_04 + b >= 0x3de || b == 0) {
                result = 1;
            } else {
                result = 0;
            }
            break;
        }
        case 9:
            result = 0;
            break;
        case 10: {
            if (mUnk_0e == 0) {
                result = 0;
                break;
            }
            u32 r = func_ov007_02267884(this);
            if (r == 1) {
                result = 0;
            } else if (r == 0) {
                u16 s[2] = {0, 0};
                if (func_ov007_02267944(this, s)) {
                    if (s[1] >= s[0]) {
                        result = 0;
                    }
                }
            }
            break;
        }
        case 11: {
            if (func_ov007_022677cc(this) == 0) {
                result = 1;
                break;
            }
            if (mUnk_0e == 0) {
                result = 0;
                break;
            }
            u32 r = func_ov007_02267884(this);
            if (r == 2) {
                result = 0;
            } else if (r == 0) {
                u16 s[2] = {0, 0};
                if (func_ov007_02267944(this, s)) {
                    if (s[0] >= s[1]) {
                        result = 0;
                    }
                }
            }
            break;
        }
    }
    return result;
}

void func_ov007_02267098(BattleUnk022637ac *self);

u8 BattleUnk022637ac::func_ov007_02265118() {
    u8 done = 0;
    switch (mUnk_28) {
        case 0: {
            VecFx32 pos = {0, 0x4800, 0};
            pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
            pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
            func_0202ba64(mEffects, 0, "shuto_return");
            s16 handle = func_0202bd40(mEffects, 0, 0, 0, 1);
            func_0202bef4(mEffects, handle, &pos);
            SoundManager::Get().func_020246f8(0x5a);
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mUnk_28++;
            break;
        }
        case 1:
            func_ov007_02267098(this);
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                done    = 1;
                *((u8 *) data_020d433c.mBattlePlayers + 0x2ac) = 1;
            }
            break;
    }
    return done;
}

u8 BattleUnk022637ac::func_ov007_022652c8() {
    u8 done = 0;
    switch (mUnk_28) {
        case 0: {
            VecFx32 pos = {0, 0x4800, 0};
            pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
            pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
            func_0202ba64(mEffects, 0, "shuto_return");
            s16 handle = func_0202bd40(mEffects, 0, 0, 0, 1);
            func_0202bef4(mEffects, handle, &pos);
            SoundManager::Get().func_020246f8(0x5a);
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mUnk_28++;
            break;
        }
        case 1:
            func_ov007_02267098(this);
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                done    = 1;
                *((u8 *) data_020d433c.mBattlePlayers + 0x2ac) = 2;
            }
            break;
    }
    return done;
}

extern const s16 data_0208bd24[2];
void func_ov007_02262714(BattlePlayerList *list, u8 a, u8 b, u8 c);
void func_ov007_02248944(VecFx32 *out, u8 a, u8 b);
void func_ov007_02248910(void *p);

// non-matching: 0x298 vs 0x29c, the original has one more `mov r2, ip` and schedules SetTarget/SetPosition differently.
u8 BattleUnk022637ac::func_ov007_0226547c() {
    u8 done = 0;
    switch (mUnk_28) {
        case 0:
            SoundManager::Get().func_020246f8(0x5b);
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mUnk_28++;
            break;
        case 1: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            u8 marker              = list->mUnits[mUnk_16].mUnk_11;
            BattleMarker *entries  = list->Entries();
            list->mUnk_1c[marker].mUnk_2 = 3;
            VecFx32 pos;
            func_ov007_02248944(&pos, entries[marker].mUnk_3, entries[marker].mUnk_4);
            s16 dx = data_0208bd24[1];
            s16 dz = data_0208bd24[0];
            fx32 x = pos.x;
            fx32 z = pos.z;
            mUnk_30->SetTarget(x, 0xc000, z);
            mUnk_30->SetPosition(x + dx * 50, 0x64000, z + dz * 50);
            func_ov007_02248910(mUnk_1c->mUnk_178);
            mUnk_28++;
            break;
        }
        case 2:
            if (data_020d433c.mBattlePlayers->mUnk_1c[data_020d433c.mBattlePlayers->mUnits[mUnk_16].mUnk_11].mUnk_2 != 3) {
                mUnk_28++;
            }
            break;
        case 3:
            mUnk_1a++;
            if (mUnk_1a > 0x78) {
                func_ov007_02262714(data_020d433c.mBattlePlayers, data_020d433c.mBattlePlayers->mUnits[mUnk_16].mUnk_11, mUnk_15, mUnk_2b);
                mUnk_1a = 0;
                done    = 1;
            }
            break;
    }
    return done;
}

void func_ov007_022629cc(BattlePlayerList *list, u8 a, u8 b);

u8 BattleUnk022637ac::func_ov007_02265718() {
    u8 done = 0;
    switch (mUnk_28) {
        case 0: {
            VecFx32 pos = {0, 0x4800, 0};
            pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
            pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
            func_0202ba64(mEffects, 0, "bkn_exchange");
            s16 handle = func_0202bd40(mEffects, 0, 0, 0, 1);
            func_0202bef4(mEffects, handle, &pos);
            SoundManager::Get().func_020246f8(0x5c);
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mUnk_28++;
            break;
        }
        case 1:
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                SpriteManager::Get();
                SpriteManager::sFade[0] = 4;
                mUnk_28++;
            }
            break;
        case 2:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            func_ov007_022629cc(data_020d433c.mBattlePlayers, mUnk_16, mUnk_2b);
            {
                BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[mUnk_15]->mRecords[mUnk_2b];
                ((UnkStruct_0203d374 *) mUnk_1c->mUnk_1a4)->func_0203d4dc(record->mUnk_08, record->mUnk_09, 3);
            }
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mUnk_28++;
            break;
        case 3:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mUnk_28++;
            break;
        case 4:
            mUnk_1a++;
            if (mUnk_1a > 0x78) {
                mUnk_1a = 0;
                done    = 1;
            }
            break;
    }
    func_ov007_02267098(this);
    return done;
}

extern "C" void MI_CpuCopy8(const void *src, void *dst, u32 size);
void func_0203db8c(void *obj, fx32 x, fx32 y, fx32 z);
void func_ov007_02262828(BattlePlayerList *list, u8 a, u8 b);
void func_ov007_02262768(BattlePlayerList *list, BattleUnk022637acUnit *unit, u16 a, u8 b, u8 c, u8 d, u8 e);
void func_ov007_02262a64(BattlePlayerList *list, u8 a, u8 b);
BOOL func_ov007_02267304(BattleUnk022637ac *self, VecFx32 *out);

// non-matching: 0x4bc vs 0x4b4, the original keeps this in r5, unit in r4 and 0x14 live in sl.
u8 BattleUnk022637ac::func_ov007_02265af8() {
    u8 done = 0;
    switch (mUnk_28) {
        case 0: {
            VecFx32 pos = {0, 0x4800, 0};
            pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
            pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
            func_0202ba64(mEffects, 0, "bkn_move");
            s16 handle = func_0202bd40(mEffects, 0, 0, 0, 1);
            func_0202bef4(mEffects, handle, &pos);
            SoundManager::Get().func_020246f8(0x5d);
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mUnk_62_1                  = 0;
            mUnk_28++;
            break;
        }
        case 1:
            if (mUnk_1a >= 0x50 && !mUnk_62_1) {
                mUnk_62_1               = 1;
                BattlePlayerList *list  = data_020d433c.mBattlePlayers;
                BattleUnit *units       = list->Units();
                BattleUnit *unit        = &units[mUnk_29];
                BattleMarker *marker    = &list->Entries()[unit->mUnk_11];
                u8 caster               = mUnk_16;
                func_ov007_02248944(&mUnk_54, marker->mUnk_3, marker->mUnk_4);
                func_0203db8c(mUnk_1c->mUnk_1a4, mUnk_54.x, 0x899a, mUnk_54.z);
                VecFx32 a = mUnk_1c->mUnk_1a4->mPosition;
                VecFx32 b = *(VecFx32 *) unit;
                mUnk_1c->func_ov007_02259a40(marker->mUnk_3, marker->mUnk_4, &a, &b);
                mUnk_1c->mUnk_1a4->mPosition = a;
                mUnk_1c->mUnk_1d2       = 3;
                mUnk_1c->mUnk_1a4->mUnk_4a = 0;
                BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[mUnk_15]->mRecords[mUnk_10];
                u16 r4               = record->mUnk_04;
                u16 r6               = record->mUnk_06;
                u8 r9                = record->mUnk_09;
                BattleUnk022637acUnit tmp;
                MI_CpuCopy8(&units[caster], &tmp, 0x14);
                tmp.mPos.x = mUnk_1c->mUnk_1a4->mPosition.x;
                tmp.mPos.z = mUnk_1c->mUnk_1a4->mPosition.z;
                func_ov007_02262828(data_020d433c.mBattlePlayers, mUnk_16, 2);
                func_ov007_02262768(data_020d433c.mBattlePlayers, &tmp, tmp.mUnk_0c, tmp.mUnk_0f, tmp.mUnk_0e, unit->mUnk_11, 1);
                u8 *rec2 = (u8 *) data_020d433c.mBattlePlayers->mPlayers[mUnk_15] + mUnk_10 * 12;
                *(u16 *) (rec2 + 0x10) = r4;
                *(u16 *) (rec2 + 0x12) = r6;
                rec2[0x15]             = r9;
                func_ov007_02262a64(data_020d433c.mBattlePlayers, mUnk_29, mUnk_16);
            }
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                mUnk_28++;
            }
            break;
        case 2:
            if (func_ov007_02267304(this, &mUnk_54)) {
                s16 handle  = func_0202bd40(mEffects, 0, 0, 0, 1);
                VecFx32 pos = {0, 0x4800, 0};
                pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
                pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
                func_0202bef4(mEffects, handle, &pos);
                SoundManager::Get().func_020246f8(0x5d);
                mUnk_62_1 = 0;
                mUnk_28++;
            }
            break;
        case 3:
            if (mUnk_1a >= 0x50 && !mUnk_62_1) {
                mUnk_62_1                  = 1;
                mUnk_1c->mUnk_1a4->mUnk_4a = 1;
            }
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                done    = 1;
            }
            break;
    }
    return done;
}


u8 BattleUnk022637ac::func_ov007_02265fac() {
    u8 done = 0;
    switch (mUnk_28) {
        case 0: {
            VecFx32 pos = {0, 0x4800, 0};
            pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
            pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
            func_0202ba64(mEffects, 0, "bkn_move");
            s16 handle = func_0202bd40(mEffects, 0, 0, 0, 1);
            func_0202bef4(mEffects, handle, &pos);
            SoundManager::Get().func_020246f8(0x5d);
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mUnk_62_1                  = 0;
            mUnk_28++;
            break;
        }
        case 1:
            if (mUnk_1a >= 0x50 && !mUnk_62_1) {
                mUnk_62_1            = 1;
                BattleMarker *marker = &data_020d433c.mBattlePlayers->Entries()[mUnk_2a];
                func_ov007_02248944(&mUnk_54, marker->mUnk_3, marker->mUnk_4);
                func_0203db8c(mUnk_1c->mUnk_1a4, mUnk_54.x, 0x899a, mUnk_54.z);
                u8 idx = mUnk_29;
                if (idx == 0xff) {
                    mUnk_1c->mUnk_1d2 = 2;
                } else {
                    VecFx32 a = mUnk_1c->mUnk_1a4->mPosition;
                    VecFx32 b = *(VecFx32 *) &data_020d433c.mBattlePlayers->mUnits[idx];
                    mUnk_1c->func_ov007_02259a40(marker->mUnk_3, marker->mUnk_4, &a, &b);
                    mUnk_1c->mUnk_1a4->mPosition = a;
                    func_ov007_02262a64(data_020d433c.mBattlePlayers, mUnk_29, mUnk_16);
                    mUnk_1c->mUnk_1d2 = 3;
                }
                mUnk_1c->mUnk_1a4->mUnk_4a = 0;
                VecFx32 c;
                c.x = mUnk_1c->mUnk_1a4->mPosition.x;
                c.y = mUnk_1c->mUnk_1a4->mPosition.y;
                c.z = mUnk_1c->mUnk_1a4->mPosition.z;
                func_ov007_022628e0(data_020d433c.mBattlePlayers, mUnk_16, &c, mUnk_2a);
            }
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                mUnk_28++;
            }
            break;
        case 2:
            if (func_ov007_02267304(this, &mUnk_54)) {
                s16 handle  = func_0202bd40(mEffects, 0, 0, 0, 1);
                VecFx32 pos = {0, 0x4800, 0};
                pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
                pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
                func_0202bef4(mEffects, handle, &pos);
                SoundManager::Get().func_020246f8(0x5d);
                mUnk_62_1 = 0;
                mUnk_28++;
            }
            break;
        case 3:
            if (mUnk_1a >= 0x50 && !mUnk_62_1) {
                mUnk_62_1                  = 1;
                mUnk_1c->mUnk_1a4->mUnk_4a = 1;
            }
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                done    = 1;
            }
            break;
    }
    return done;
}

u8 BattleUnk022637ac::func_ov007_022663f0() {
    u8 done = 0;
    switch (mUnk_28) {
        case 0: {
            VecFx32 pos = {0, 0x4800, 0};
            pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
            pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
            func_0202ba64(mEffects, 0, "defense_up");
            s16 handle = func_0202bd40(mEffects, 0, 0, 0, 1);
            func_0202bef4(mEffects, handle, &pos);
            SoundManager::Get().func_020246f8(0x5e);
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mUnk_28++;
            break;
        }
        case 1:
            func_ov007_02267098(this);
            if (!mUnk_62_2 && mUnk_1a > 0x32) {
                mUnk_62_2 = 1;
                data_020d433c.mBattlePlayers->mUnk_29c->func_ov007_02270a7c(mUnk_16, 0);
            }
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                data_020d433c.mBattlePlayers->mUnits[mUnk_16].mUnk_10 = 3;
                done                                                   = 1;
            }
            break;
    }
    return done;
}

BOOL func_ov007_0226745c(BattleUnk022637ac *self, u8 a, u8 b);

u8 BattleUnk022637ac::func_ov007_022665ec() {
    u8 done = 0;
    switch (mUnk_28) {
        case 0:
            func_0202ba64(mEffects, 0, "gp_pickup2");
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mUnk_28++;
            break;
        case 1: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            VecFx32 pos = {0, 0x4800, 0};
            pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
            pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
            s16 handle  = func_0202bd40(mEffects, 0, 0, 0, 1);
            func_0202bef4(mEffects, handle, &pos);
            SoundManager::Get().func_020246f8(0x5f);
            mUnk_62_0 = 0;
            mUnk_28++;
            break;
        }
        case 2:
            if (func_ov007_0226745c(this, mUnk_15, mUnk_10)) {
                mUnk_28++;
            }
            break;
        case 3:
            func_ov007_0226745c(this, mUnk_15, mUnk_10);
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                done    = 1;
            }
            break;
    }
    mUnk_1c->func_ov007_0225ca48(0xff, 0xff);
    func_ov007_02267098(this);
    return done;
}

u8 BattleUnk022637ac::func_ov007_02266868() {
    u8 done = 0;
    switch (mUnk_28) {
        case 0: {
            VecFx32 pos = {0, 0x4800, 0};
            pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
            pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
            func_0202ba64(mEffects, 0, "gp_pickup2");
            s16 handle = func_0202bd40(mEffects, 0, 0, 0, 1);
            func_0202bef4(mEffects, handle, &pos);
            SoundManager::Get().func_020246f8(0x5f);
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            mUnk_28++;
            break;
        }
        case 1:
            func_ov007_02267098(this);
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                done    = 1;
                data_020d433c.mBattlePlayers->mPlayers[mUnk_15]->mUnk_f3 = 0xbe;
            }
            break;
    }
    return done;
}

// Effect placement of a field (0x24 bytes): position, camera position and a scale.
struct BattleUnk022637acEffect {
    /* 00 */ VecFx32 mPos;
    /* 0c */ VecFx32 mCamera;
    /* 18 */ VecFx32 mScale;
    /* 24 */
};

static const BattleUnk022637acEffect data_ov007_0227ab00[8] = {
    {{0, 0x5000, 0}, {0, 0x140000, 0xf0000}, {0x9000, 0xa000, 0x9000}},
    {{0, 0x5000, 0}, {0, 0x15e000, 0x15e000}, {0xc000, 0xa000, 0xc000}},
    {{0, 0x5000, 0}, {0, 0x1f4000, 0x1f4000}, {0x11800, 0xa000, 0x11800}},
    {{0, 0x5000, 0}, {0, 0x118000, 0x118000}, {0x9800, 0xa000, 0x9800}},
    {{0, 0x5000, 0}, {0, 0x1c2000, 0x1c2000}, {0xf800, 0xa000, 0xf800}},
    {{0, 0x5000, 0}, {0, 0x1f4000, 0x1f4000}, {0x11000, 0xa000, 0x11000}},
    {{0, 0x5000, 0}, {0, 0x1f4000, 0x1f4000}, {0x12000, 0xa000, 0x12000}},
    {{0, 0x5000, 0}, {0, 0x208000, 0x208000}, {0x12000, 0xa000, 0x12000}},
};

void func_0202bf18(EffectManager *manager, u16 handle, VecFx32 *scale);
BOOL func_ov007_02267100(BattleUnk022637ac *self);

u8 BattleUnk022637ac::func_ov007_02266a28() {
    u8 done = 0;
    switch (mUnk_28) {
        case 0:
            func_0202ba64(mEffects, 0, mUnk_18 == 0xd4 ? "gp_pickup2" : "gp_down");
            SpriteManager::Get();
            SpriteManager::sFade[0] = 2;
            func_ov007_02267098(this);
            mUnk_28++;
            break;
        case 1: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            VecFx32 pos = {0, 0x4800, 0};
            pos.x       = mUnk_1c->mUnk_1a4->mPosition.x;
            pos.z       = mUnk_1c->mUnk_1a4->mPosition.z;
            s16 handle  = func_0202bd40(mEffects, 0, 0, 0, 1);
            func_0202bef4(mEffects, handle, &pos);
            u32 sfx = mUnk_18 == 0xd4 ? 0x5f : 0x6c;
            SoundManager::Get().func_020246f8(sfx);
            if (mUnk_18 == 0xd3) {
                mUnk_62_0 = 1;
            } else {
                mUnk_62_0 = 0;
            }
            func_ov007_02267098(this);
            mUnk_28++;
            break;
        }
        case 2:
            mUnk_1c->func_ov007_0225ca48(0xff, 0xff);
            func_ov007_02267098(this);
            if (func_ov007_0226745c(this, mUnk_15, mUnk_10)) {
                mUnk_28++;
            }
            break;
        case 3:
            func_ov007_0226745c(this, mUnk_15, mUnk_10);
            mUnk_1c->func_ov007_0225ca48(0xff, 0xff);
            func_ov007_02267098(this);
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                mUnk_28++;
            }
            break;
        case 4:
            if (func_ov007_02267100(this)) {
                mUnk_1a = 0;
                const BattleUnk022637acEffect *entry = &data_ov007_0227ab00[data_020d433c.mUnk_95];
                VecFx32 pos                          = {0, 0x4800, 0};
                pos.x                                = entry->mPos.x;
                pos.z                                = entry->mPos.z;
                u16 handle                           = (u16) (s16) func_0202bd40(mEffects, 0, 0, 0, 1);
                func_0202bef4(mEffects, handle, &pos);
                func_0202bf18(mEffects, handle, (VecFx32 *) &entry->mScale);
                u32 sfx = mUnk_18 == 0xd4 ? 0x5f : 0x6c;
                SoundManager::Get().func_020246f8(sfx);
                mUnk_28++;
            }
            break;
        case 5:
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                u8 i    = 0;
                do {
                    BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[i];
                    if (unit->mUnk_10 != 0) {
                        if (!(unit->mUnk_0f == mUnk_15 && unit->mUnk_0e == mUnk_10)) {
                            mUnk_71[mUnk_70++] = i;
                        }
                    }
                    i++;
                } while (i < 12);
                mUnk_28++;
            }
            break;
        case 6: {
            if (mUnk_70 == 0) {
                done = 1;
                break;
            }
            mUnk_70--;
            BattleUnit *units = data_020d433c.mBattlePlayers->Units();
            VecFx32 *p        = (VecFx32 *) &units[mUnk_71[mUnk_70]];
            fx32 x, y, z;
            z = p->z;
            y = p->y;
            x = p->x;
            mUnk_30->SetTarget(x, y, z);
            func_ov007_02267098(this);
            s16 handle  = func_0202bd40(mEffects, 0, 0, 0, 1);
            VecFx32 pos = {0, 0x4800, 0};
            pos.x       = x;
            pos.z       = z;
            func_0202bef4(mEffects, handle, &pos);
            u32 sfx = mUnk_18 == 0xd4 ? 0x5f : 0x6c;
            SoundManager::Get().func_020246f8(sfx);
            mUnk_60 = 0;
            mUnk_28++;
            break;
        }
        case 7: {
            BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[mUnk_71[mUnk_70]];
            mUnk_1c->func_ov007_0225ca48(unit->mUnk_0f, unit->mUnk_0e);
            func_ov007_02267098(this);
            if (func_ov007_0226745c(this, unit->mUnk_0f, unit->mUnk_0e)) {
                mUnk_28++;
            }
            break;
        }
        case 8: {
            BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[mUnk_71[mUnk_70]];
            mUnk_1c->func_ov007_0225ca48(unit->mUnk_0f, unit->mUnk_0e);
            func_ov007_02267098(this);
            func_ov007_0226745c(this, unit->mUnk_0f, unit->mUnk_0e);
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                mUnk_28 = 6;
            }
            break;
        }
    }
    return done;
}

void func_ov007_0226a380(BattlePlayer *player, u8 record, int unk);

void func_ov007_02267098(BattleUnk022637ac *self) {
    u16 angle   = self->mUnk_3a;
    Camera *cam = self->mUnk_30;
    int idx     = angle >> 4;
    s32 sinV    = FX_SinCosTable_[idx * 2 + 1];
    s32 cosV    = FX_SinCosTable_[idx * 2];
    fx32 x      = cam->mTarget.x + sinV * 65;
    fx32 z      = cam->mTarget.z + cosV * 65;
    self->mUnk_3a = angle + 100;
    cam         = self->mUnk_30;
    cam->mPosition.x = x;
    cam->mPosition.y = 0xc000;
    cam->mPosition.z = z;
}

BOOL func_ov007_02267100(BattleUnk022637ac *self) {
    BOOL done = 0;
    VecFx32 tmp;
    VecFx32 d1;
    VecFx32 d2;
    Camera *cam;
    const BattleUnk022637acEffect *entry = &data_ov007_0227ab00[data_020d433c.mUnk_95];
    switch (self->mUnk_38) {
        case 0:
            cam   = self->mUnk_30;
            tmp.x = cam->mPosition.x;
            tmp.y = cam->mPosition.y;
            tmp.z = cam->mPosition.z;
            VEC_Subtract(&entry->mCamera, &tmp, &d1);
            cam   = self->mUnk_30;
            tmp.x = cam->mTarget.x;
            tmp.y = cam->mTarget.y;
            tmp.z = cam->mTarget.z;
            VEC_Subtract(&entry->mPos, &tmp, &d2);
            self->mUnk_3c.x = d1.x / 70;
            self->mUnk_3c.y = d1.y / 70;
            self->mUnk_3c.z = d1.z / 70;
            self->mUnk_48.x = d2.x / 70;
            self->mUnk_48.y = d2.y / 70;
            self->mUnk_48.z = d2.z / 70;
            self->mUnk_38++;
        case 1:
            cam  = self->mUnk_30;
            d1.x = cam->mPosition.x;
            d1.y = cam->mPosition.y;
            d1.z = cam->mPosition.z;
            cam  = self->mUnk_30;
            d2.x = cam->mTarget.x;
            d2.y = cam->mTarget.y;
            d2.z = cam->mTarget.z;
            VEC_Add(&d1, &self->mUnk_3c, &d1);
            VEC_Add(&d2, &self->mUnk_48, &d2);
            cam                = self->mUnk_30;
            cam->mPosition.x   = d1.x;
            cam->mPosition.y   = d1.y;
            cam->mPosition.z   = d1.z;
            cam                = self->mUnk_30;
            cam->mTarget.x     = d2.x;
            cam->mTarget.y     = d2.y;
            cam->mTarget.z     = d2.z;
            self->mUnk_1a++;
            if (self->mUnk_1a > 0x46) {
                self->mUnk_1a = 0;
                self->mUnk_38 = 0;
                done          = 1;
            }
            break;
    }
    return done;
}

BOOL func_ov007_02267304(BattleUnk022637ac *self, VecFx32 *out) {
    BOOL done = 0;
    VecFx32 tmp;
    VecFx32 d1;
    VecFx32 d2;
    Camera *cam;
    switch (self->mUnk_38) {
        case 0:
            cam   = self->mUnk_30;
            tmp.x = cam->mTarget.x;
            tmp.y = cam->mTarget.y;
            tmp.z = cam->mTarget.z;
            VEC_Subtract(out, &tmp, out);
            VEC_Normalize(out, &self->mUnk_3c);
            self->mUnk_3c.x *= 10;
            self->mUnk_3c.z *= 10;
            self->mUnk_3c.y = done;
            self->mUnk_1a   = out->x / self->mUnk_3c.x;
            self->mUnk_38++;
            break;
        case 1:
            cam  = self->mUnk_30;
            d1.x = cam->mPosition.x;
            d1.y = cam->mPosition.y;
            d1.z = cam->mPosition.z;
            cam  = self->mUnk_30;
            d2.x = cam->mTarget.x;
            d2.y = cam->mTarget.y;
            d2.z = cam->mTarget.z;
            VEC_Add(&d1, &self->mUnk_3c, &d1);
            VEC_Add(&d2, &self->mUnk_3c, &d2);
            cam              = self->mUnk_30;
            cam->mPosition.x = d1.x;
            cam->mPosition.y = d1.y;
            cam->mPosition.z = d1.z;
            cam              = self->mUnk_30;
            cam->mTarget.x   = d2.x;
            cam->mTarget.y   = d2.y;
            cam->mTarget.z   = d2.z;
            if (self->mUnk_1a != 0) {
                self->mUnk_1a--;
            } else {
                done = 1;
            }
            break;
    }
    return done;
}

BOOL func_ov007_0226745c(BattleUnk022637ac *self, u8 player, u8 slot) {
    BOOL done = 0;
    switch (self->mUnk_60) {
        case 0: {
            u16 value      = data_020d433c.mBattlePlayers->mPlayers[player]->mRecords[slot].mUnk_06;
            self->mUnk_6c = value;
            self->mUnk_6e = value;
            self->mUnk_68 = 0x4000;
            self->mUnk_64 = 0x60000;
            if (self->mUnk_62_0) {
                u8 i = 0;
                do {
                    self->mUnk_80[i]->mUnk_11 = 13;
                    i++;
                } while (i < 2);
            } else {
                u16 sum = data_020d433c.mBattlePlayers->mPlayers[player]->mRecords[slot].mUnk_04;
                sum += self->mUnk_6c;
                if (self->mUnk_6c + sum > 0x3de) {
                    self->mUnk_6c = 0x3de - sum;
                    self->mUnk_6e = self->mUnk_6c;
                }
            }
            self->mUnk_61 = 4;
            if (self->mUnk_6c < 1000) {
                self->mUnk_61--;
                if (self->mUnk_6c < 100) {
                    self->mUnk_61--;
                    if ((int) self->mUnk_6c < 10) {
                        self->mUnk_61--;
                    }
                }
            }
            self->mUnk_60++;
            break;
        }
        case 1:
            self->mUnk_68 -= 0x800;
            self->mUnk_64 -= self->mUnk_68;
            if (self->mUnk_68 < -0x4000) {
                if (self->mUnk_6c == 0) {
                    self->mUnk_60 = 3;
                } else {
                    self->mUnk_60++;
                }
            }
            break;
        case 2:
            self->mUnk_63++;
            if (self->mUnk_63 > 10) {
                self->mUnk_63 = 0;
                if (self->mUnk_6c == 0) {
                    self->mUnk_60++;
                } else {
                    s16 amount;
                    if ((int) self->mUnk_6c >= 30) {
                        self->mUnk_6c -= 30;
                        amount = 30;
                    } else {
                        self->mUnk_6c = 0;
                        amount        = 30 - self->mUnk_6c;
                    }
                    BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[player];
                    if (!self->mUnk_62_0) {
                        func_ov007_0226a380(p, slot, amount);
                    } else {
                        func_ov007_0226a380(p, slot, (s16) -amount);
                    }
                }
                SoundManager::Get().func_020246f8(8);
            }
            break;
        case 3:
            done = 1;
            break;
    }
    u8 i      = 0;
    s16 x     = 0x50;
    s16 y     = (s16) (int) ((float) self->mUnk_64 / 4096.0f);
    x += 0x30;
    u32 value = self->mUnk_6e;
    do {
        self->mUnk_80[0]->func_0202a3ec(0, (u8) (u16) ((int) value % 10), x, y, 0);
        x -= 12;
        value /= 10;
        i++;
    } while (i < self->mUnk_61);
    self->mUnk_80[1]->func_0202a3ec(0, self->mUnk_62_0, x, y, 0);
    return done;
}

// non-matching: 4 words, the same player/mUnk_0e register swap as case 8 of func_ov007_02264a5c.
BOOL func_ov007_022677cc(BattleUnk022637ac *self) {
    BOOL r = 1;
    u8 i                   = 0;
    u16 a                  = 0;
    u16 b                  = 0;
    u16 c                  = 0;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    do {
        BattleUnit *unit = &list->mUnits[i];
        if (unit->mUnk_10) {
            BattleRecord *rec = &list->mPlayers[unit->mUnk_0f]->mRecords[unit->mUnk_0e];
            c += 0x3de;
            a += rec->mUnk_04;
            b += rec->mUnk_06;
            a += rec->mUnk_06;
        }
        i++;
    } while (i < 12);
if (a >= c || b == 0) { r = 0; } return r;
}

enum UnitCompare { UNIT_COMPARE_0, UNIT_COMPARE_1, UNIT_COMPARE_2 };
u32 func_ov007_02267884(BattleUnk022637ac *self) {
    if (self->mUnk_1c->mUnk_1d2 != 3) {
        return 0;
    }
    BattleUnit *units    = data_020d433c.mBattlePlayers->mUnits;
    u8 i                 = 0;
    BattleUnit *selfUnit = &units[self->mUnk_16];
    UnitCompare result   = UNIT_COMPARE_0;
    do {
        if (self->mUnk_16 != i) {
            BattleUnit *unit = &units[i];
            if (selfUnit->mUnk_11 == unit->mUnk_11) {
                u16 a = data_020d433c.mBattlePlayers->mPlayers[selfUnit->mUnk_0f]->mRecords[selfUnit->mUnk_0e].mUnk_06;
                u16 b = data_020d433c.mBattlePlayers->mPlayers[unit->mUnk_0f]->mRecords[unit->mUnk_0e].mUnk_06;
                if (a > b) {
                    result = UNIT_COMPARE_2;
                } else if (a < b) {
                    result = UNIT_COMPARE_1;
                }
                return result;
            }
        }
        i++;
    } while (i < 12);
    return result;
}

BOOL func_ov007_02267944(BattleUnk022637ac *self, u16 *out) {
    u8 i = 0;
    do {
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        BattleUnit *unit       = &list->mUnits[i];
        if (unit->mUnk_10) {
            u8 pidx = unit->mUnk_0f;
            u16 v   = list->mPlayers[pidx]->mRecords[unit->mUnk_0e].mUnk_06;
            if (pidx == self->mUnk_15) {
                out[0] += v;
            } else if (data_020d433c.mUnk_98 && pidx == list->mPlayers[self->mUnk_15]->mUnk_f2) {
                out[0] += v;
            } else {
                out[1] += v;
            }
        }
        i++;
    } while (i < 12);
    if (out[0] != 0 || out[1] != 0) {
        return 1;
    }
    return 0;
}
