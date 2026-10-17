#include "Battle/BattleBase.hpp"
#include "Battle/BattleCharaAct.hpp"

#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/BattleCardSpec.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "System/SoundManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/TouchManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "System/InputManager.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include <nitro/fx.h>
#include <nitro/gx.h>
#include "Battle/BattleUtil.hpp"
#include "System/WirelessManager.hpp"

extern const u8 data_ov007_02279160;
extern const u8 gLargeCardTouchHeight;
extern const u8 gLargeCardTouchWidth;
extern const u8 gSmallCardTouchY;
extern const s16 gLargeCardTouchX;
extern const u8 gSmallCardX[];
extern const s16 gLargeCardTouchY;

BOOL UpdateCameraPath(BattleField3D *field);

// Functions of other battle files.
void FreeAuras(BattleField3D *obj);
void ClearSub48State(BattleSub48 *obj);
void SetSlot6cFlag(BattlePlayer *player, u8 a, int b);
void func_ov007_0222844c(BattleHud *obj, u8 a, u8 b);
BOOL IsHudSequenceRunning(BattleHud *obj);
BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);
void StartAbilitiesSlide(BattleSub48 *obj);
void ShowModels(BattleField3D *obj);
BOOL MoveToEndThenCreateAuras(BattleField3D *obj);
BOOL func_ov007_022297f0(BattleField3D *obj);
void SetHudState(BattleHud *obj, u32 a);
void func_ov007_0222847c(BattleHud *obj);
void MovePlayerAnimX(BattleHud *obj);
extern const u8 data_ov007_0227915c;
extern const u8 data_ov007_02279170;
struct BattleRow {
    s32 mUnk[6];
};
extern const BattleRow gAttributeRates[];
extern const s32 data_ov007_02278b74;
void func_ov007_02229d08(BattleBase *scene);
void func_ov007_02229c88(BattleBase *scene, u16 *a, u16 *b, u16 *c, u8 flag);

void CloseHud(void *);
extern const s8 data_ov007_02279158[];
extern const s8 data_ov007_02279d3c[];
extern const s16 gEntryNumberX[];




BattleBase::BattleBase()
    : IBattle(), SceneBase(0x3e8, 0) {
    mState = 0;
    mStep = 0;
    mLocalSide = 0;
    mUnk_13 = 0;
    mMultiplierIndex = -1;
    mMode = 0;
    mElapsedFrames = 0;
    mBattleEnded = 0;
    mTouched = 0;
    mFlashEnabled = 0;
    mSpectatorTouchSide = 2;
    mUnk_1a = 0;
    mSpectatorHoldFrames = 0;
    mUnk_24 = 0;
    mUnk_25 = 0;
    mFlashBrightness = 0;
    mFlashFalling = 0;
    mRateFloorSide = 2;
    mUnk_2a = 0;
    mBattleData = 0;
    mField3D = 0;
    mHud = 0;
    mSub48 = 0;
    mReturnState = 0;
    mCardStep = 0;
    mCardSide = 0;
    mCardKind = 0;
    mPrevCardKind = 0xff;
    mBlendAlpha = 0;
    mUnk_54_0 = 0;
    mUnk_54_1 = 0;
    mUnk_54_2 = 0;
    mUnk_54_3 = 0;
    mUnk_54_4 = 0;
    mUnk_54_5 = 0;
    mLargeCardPressesRemaining = 0;
    mCardId = 0;
    mCommandPage = 0;
    mCardSpec = 0;
    mSlots[0] = mSlots[1] = mSlots[2] = mSlots[3] = 0;
    mUnk_1c = mUnk_1d = -1;
    mUnk_1e = mUnk_1f = -1;
    mUnk_52[0] = mUnk_52[1] = 0;
    mHelpers[0] = mHelpers[1] = 0;
    if (data_020d433c.mFieldId != 0 && data_020d433c.mFieldId != 7) {
        mMultiplierIndex = data_020d433c.mFieldId - 1;
    }
}

BattleBase::~BattleBase() {
    if (mField3D) delete mField3D;
    mSub48->mState = 4;
    CloseHud(mHud);
    if (mSlots[3]) delete mSlots[3];
    if (mSlots[2]) delete mSlots[2];
    if (mSlots[1]) delete mSlots[1];
    if (mSlots[0]) delete mSlots[0];
    if (mHelpers[1]) delete mHelpers[1];
    if (mHelpers[0]) delete mHelpers[0];
}

void BattleBase::Init(BattleData *data, u32 mode) {
    mBattleData = data;
    mParam = &gBattleParams[mode];
    mMode = mode;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 sel = list->mUnk_2a9;
    BattleEntry *entries = data->mEntries;
    u8 first = entries[0].mPlayer;
    if (sel == first) {
        mLocalSide = 0;
    } else if (sel == entries[1].mPlayer) {
        mLocalSide = 1;
    } else {
        if (data_020d433c.mHasPartners != 0) {
            if (sel == list->mPlayers[first]->mPartner) {
                mLocalSide = 3;
                if (data_020d433c.mUnk_7a != 0) {
                    mRateFloorSide = 0;
                }
            } else {
                mLocalSide = 4;
                if (data_020d433c.mUnk_7a != 0) {
                    mRateFloorSide = 1;
                }
            }
        } else {
            mLocalSide = 2;
        }
        mMode = BATTLE_MODE_SPECTATOR;
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
        if (data_020d433c.mBattlePlayers->mPlayers[i]->IsCpu()) {
            BattleSlot *slot = new BattleSlot(data, i, mode, data_020d433c.mBattlePlayers->mPlayers[i]->mCharacterId,
                                              entries[0].mPlayer, entries[1].mPlayer, mParam, mUnk_13);
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
            u8 nib = entries[j].mPlayer;
            if (!data_020d433c.mBattlePlayers->mPlayers[nib]->IsCpu()) {
                mHelpers[j] = new BattleSideHelper(j, data_020d433c.mBattlePlayers->mPlayers[nib]->mCharacterId, entries);
            }
            j++;
        } while (j < 2);
    }
    if (count == 2 && (mode == BATTLE_MODE_CONFRONT_BG01 || mode == BATTLE_MODE_CONFRONT_BG04)) {
        mSlots[order[0]]->mUnk_14 |= 4;
        mSlots[order[1]]->mUnk_14 |= 4;
    }
    Init(data, mode); // virtual call: the derived screen's Init (Confront03, ConfrontBg01/02/04/05/06), not this body
    mHud = new BattleHud(data, mParam, mLocalSide, (s8 *) &mUnk_1c);
    mSub48 = new BattleSub48(data, mLocalSide);
    mField3D = new BattleField3D(data, 0);
    if (data_020d433c.mAntenna) {
        data_020d433c.mAntenna->SetupLayout(0xb);
    }
}

void BattleBase::Update() {
    BattleEntry *entries = mBattleData->Entries();
    switch (mState) {
        case 0:
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mState++;
        case 1:
            if (UpdateCameraPath(mField3D)) {
                mState++;
            }
            break;
        case 2:
            if (mSub48->UpdateCircleAndSidesMove()) {
                mState++;
            }
            break;
        case 3: {
            s8 i       = 1;
            BOOL found = 0;
            do {
                u32 v = 0xff;
                if (entries[i].mCardId != 0) {
                    v = GetCardInfo(entries[i].mCardId, 3);
                }
                if (v == 5) {
                    mCardSide = i;
                    found   = 1;
                    break;
                }
                i--;
            } while (i >= 0);
            if (found) {
                mUnk_54_0 = 1;
                mReturnState   = 5;
                mState   = 8;
                SetHudState(mHud, HUD_SEQUENCE_NONE);
            } else {
                mState++;
            }
            break;
        }
        case 4: {
            s8 i       = 1;
            BOOL found = 0;
            do {
                if (entries[i].mCardId != 0) {
                    u8 idx = i;
                    if (func_ov007_02222764(idx, entries) == 5) {
                        mCardSide = idx;
                        found   = 1;
                        break;
                    }
                }
                i--;
            } while (i >= 0);
            if (found) {
                mUnk_54_0 = 1;
                mReturnState   = 5;
                mState   = 8;
                SetHudState(mHud, HUD_SEQUENCE_NONE);
            } else {
                SetHudState(mHud, HUD_SEQUENCE_BAR_FILL_COUNTDOWN);
                mState++;
            }
            break;
        }
        case 5: {
            s8 i       = 1;
            BOOL found = 0;
            do {
                u16 card = entries[i].mCardId;
                if (card >= 0x8e && card <= 0x91) {
                    mCardSide = i;
                    found   = 1;
                    break;
                }
                i--;
            } while (i >= 0);
            if (found) {
                mUnk_54 |= 0x20;
                mState = 8;
                SetHudState(mHud, HUD_SEQUENCE_NONE);
            } else {
                SetHudState(mHud, HUD_SEQUENCE_BAR_FILL_COUNTDOWN);
                mState++;
            }
            break;
        }
        case 6:
            if (BattleBase::func_ov007_0221bd84()) {
                u8 i = 0;
                do {
                    if (entries[i].mUnk_0f_2) {
                        mUnk_52[i] = 1;
                        mHud->func_ov007_02227bbc(i);
                    }
                    i++;
                } while (i < 2);
                func_ov007_0221bd84();
                mState++;
            }
            break;
        case 7:
            if (!CheckBattleEnd()) {
                vfunc_34();
                func_ov007_0221c938();
            } else {
                mUnk_1c = mUnk_1d = -1;
                mUnk_1e = mUnk_1f = -1;
                mState = 14;
            }
            break;
        case 8:
            mCardStep = 0;
            mState++;
        case 9:
            if (UpdateCardSequence()) {
                if (mCardKind == 2 || mUnk_54_5 == 1) {
                    mState = 12;
                } else {
                    if (mUnk_54_0) {
                        mField3D = new BattleField3D(mBattleData, 2);
                    } else {
                        mField3D = new BattleField3D(mBattleData, 1);
                        SoundManager::Get().StartSeqArc(0x24);
                    }
                    switch (mCardKind) {
                        case 0:
                            SoundManager::Get().StartSeqArc(0x2f);
                            func_020247fc(&SoundManager::Get(), 0x400);
                            if (mHud->mPushPercent == 0x32) {
                                SetHudState(mHud, HUD_SEQUENCE_COUNTDOWN);
                                mCardKind = 4;
                            } else {
                                SetHudState(mHud, HUD_SEQUENCE_SWAP);
                            }
                            StartAbilitiesSlide(mSub48);
                            break;
                        case 1:
                            if ((mCardSide == 0 && mLocalSide == 1) || (mCardSide == 1 && mLocalSide == 0)) {
                                mHud->OpenLargeCard(mCardId);
                                mLargeCardPressesRemaining = 10;
                            } else {
                                u8 j = 0;
                                do {
                                    if (data_020d433c.mBattlePlayers->mPlayers[j]->IsCpu()) {
                                        mSlots[j]->StartCardDelay(mCardSide);
                                    }
                                    j++;
                                } while (j < data_020d433c.mPlayerCount);
                            }
                            func_ov007_0223e214(mBattleData, mCardSide, 1);
                            SetHudState(mHud, HUD_SEQUENCE_COUNTDOWN);
                            StartAbilitiesSlide(mSub48);
                            break;
                        case 3:
                            mUnk_52[mCardSide] = 1;
                            mHud->func_ov007_02227bbc(mCardSide);
                            SetHudState(mHud, HUD_SEQUENCE_COUNTDOWN);
                            StartAbilitiesSlide(mSub48);
                            break;
                        case 4:
                            if (!mUnk_54_0) {
                                SetHudState(mHud, HUD_SEQUENCE_COUNTDOWN);
                                StartAbilitiesSlide(mSub48);
                            }
                            break;
                    }
                    mState++;
                }
            }
            break;
        case 10:
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
                mState++;
            }
            break;
        case 11:
            if (!SpriteManager::Get().IsFading()) {
                func_ov007_0221b8e8();
            }
            break;
        case 12:
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                SoundManager::Get().StartSeqArc(0x32);
                SpriteManager::Get().StartFade(FADE_MODE_UP);
                mState++;
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
    if (mFlashEnabled) {
        if (UpdateFlash()) {
            mFlashEnabled = 0;
        }
    }
    if (!mUnk_54_3) {
        vfunc_3c();
    }
    if (mField3D) {
        mField3D->Draw(mHud->mPushPercent);
    }
}

void BattleBase::InitMainScreen() {
    SpriteManager::Get().SetupMode(0, 7);
    GX_SetGraphicsMode(1, 0, 1);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(2);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(2);
}

void BattleBase::InitSubScreen() {
    mBlendAlpha = 7;
    G2S_SetBG2Priority(0);
    G2S_SetBlendAlpha(4, 0x1b, 0, mBlendAlpha);
}

BOOL BattleBase::RaiseBlendAlpha() {
    u8 ev = ++mBlendAlpha;
    if (ev >= 0x1f) {
        mBlendAlpha = 0x1f;
        return 1;
    }
    G2S_SetBlendAlpha(4, 0x1b, 0, ev);
    return 0;
}

void BattleBase::SetupCardSequence() {
    BattleEntry *entries = mBattleData->mEntries;
    mCardId = entries[mCardSide].mCardId;
    mCardKind = GetCardInfo(entries[mCardSide].mCardId, 3);
    mUnk_2a = 0;
    SetSlot6cFlag(data_020d433c.mBattlePlayers->mPlayers[entries[mCardSide].mPlayer], entries[mCardSide].mSlot6cIndex, 2);
    BattleBase::InitSubScreen();
    mUnk_54_3 = 1;
    if (!mUnk_54_0) {
        mHud->mUnk_22 &= ~0x10;
    }
    func_ov007_0222844c(mHud, mCardSide, mCardKind);
    ClearSub48State(mSub48);
    mUnk_1d = -1;
    mUnk_1c = mUnk_1d;
    mUnk_1f = -1;
    mUnk_1e = mUnk_1f;
    if (entries[0].mCardId == 0x8d && entries[1].mCardId == 0x8d) {
        mUnk_54_2 = 1;
    }
    if (entries[0].mCardId == 0x8c || entries[1].mCardId == 0x8c) {
        mUnk_54_2 = 1;
    }
}

static inline Camera *GetCam() {
    return data_020d433c.mBattlePlayers->mCamera;
}
BOOL BattleBase::UpdateCardSequence() {
    BattleEntry *entries = mBattleData->mEntries;
    switch (mCardStep) {
        case 0:
            SoundManager::Get().StopSeqArcPlayerSounds(1);
            SoundManager::Get().StartSeqArc(0x30);
            SpriteManager::Get().StartFade(FADE_MODE_UP);
            mCardStep++;
        case 1:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            if (mField3D) {
                delete mField3D;
            }
            mField3D = 0;
            SetupCardSequence();
            mCardStep++;
            break;
        case 2:
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                InitMainScreen();
                new BattleCharaAct(GetCam(), entries[mCardSide].mPlayer, 6, (GetCardNumber(mCardId) - 0x190) / 100, 0);
                mCardStep++;
            }
            break;
        case 3:
            if (mContext->mRequest != 0) {
                mContext->mRequest = 0;
                SpriteManager::Get().ResetSlots();
                mHud->mUnk_118[mCardSide] = 0xff;
                mCardStep++;
            }
            break;
        case 4:
            mHud->StartAbilitySlideIn(mCardSide);
            mCommandPage = (u32) new BattleCommandPage(mCardId, -1);
            mCardStep++;
            break;
        case 5:
            if (mUnk_54_4) {
                if (mHud->IsAbilitySlideDone()) {
                    mUnk_54_4 = 0;
                    SoundManager::Get().StartSeqArc(0x33);
                }
            }
            mUnk_2a++;
            if (mUnk_2a > 0xf0) {
                mUnk_2a = 0;
                CloseCommandPage((BattleCommandPage *) mCommandPage, 1);
                mCardStep = 6;
                if (mUnk_54_5 && mCardKind == 4 && func_ov007_0221ba0c((int) this, mCardSide, entries)) {
                    mCardStep = 7;
                } else if (!mUnk_54_1 && mUnk_54_0 && func_ov007_0221b998((int) this, mCardSide, entries)) {
                    mCardStep = 8;
                } else if (!mUnk_54_1 && mUnk_54_0 && func_ov007_0221b9d0((int) this, mCardSide, entries)) {
                    mUnk_54_4 = 1;
                    mCardStep   = 8;
                } else if (mCardKind != 4 && func_ov007_0221ba38((int) this, mCardSide, mCardKind, entries)) {
                    mCardStep = 9;
                }
                if (mCardStep != 6) {
                    SoundManager::Get().StartSeqArc(0x30);
                    CloseCommandPage((BattleCommandPage *) mCommandPage, 2);
                }
            }
            break;
        case 6:
            if (mContext->mRequest != 0) {
                mContext->mRequest = 0;
                if (mCardKind == 4) {
                    if (mUnk_54_5) {
                        if (entries[0].mUnk_00 == entries[1].mUnk_00) {
                            u8 idx = MATH_Rand32(&gRandom, 2);
                            BattleData *d = mBattleData;
                            d->mEntries[idx].mUnk_10   = 1;
                            d->mEntries[idx].mUnk_0f_6 = 0;
                        } else if (entries[0].mUnk_00 > entries[1].mUnk_00) {
                            BattleData *d = mBattleData;
                            d->mEntries[0].mUnk_10   = 1;
                            d->mEntries[0].mUnk_0f_6 = 0;
                        } else {
                            BattleData *d = mBattleData;
                            d->mEntries[1].mUnk_10   = 1;
                            d->mEntries[1].mUnk_0f_6 = 0;
                        }
                        mCardStep = 0x12;
                    } else if (mUnk_54_0) {
                        mCardStep = 0xe;
                    } else {
                        mHud->StartAbilitySlideAcross(mCardSide);
                        mCardStep = 0x12;
                    }
                } else if (mUnk_54_0) {
                    mCardStep = 0xe;
                } else {
                    int k = func_ov007_0221ba88((int) this, mCardSide, entries);
                    if (k) {
                        mBattleData->func_ov007_0223e1a4(k, mCardSide);
                    }
                    switch (mCardKind) {
                        case 0:
                            mCardStep = 0xb;
                            break;
                        case 1:
                        case 2:
                        case 3:
                            mHud->StartAbilitySlideOut(mCardSide);
                            mCardStep = 0x12;
                            break;
                    }
                }
                mBattleData->func_ov007_0223e1cc(mCardSide, mCardKind);
            }
            break;
        case 7:
            if (mContext->mRequest != 0) {
                mContext->mRequest = 0;
                mCardStep           = 10;
                mBattleData->func_ov007_0223e1cc(mCardSide, mCardKind);
            }
            break;
        case 8:
            if (mContext->mRequest != 0) {
                mContext->mRequest = 0;
                mUnk_54_1         = 1;
                mCardStep           = 10;
                mBattleData->func_ov007_0223e1cc(mCardSide, mCardKind);
            }
            break;
        case 9:
            if (mContext->mRequest != 0) {
                mContext->mRequest = 0;
                mUnk_54_4         = 1;
                mCardStep           = 10;
                if (mCardKind == 3) {
                    mCardKind = 4;
                }
                mBattleData->func_ov007_0223e1cc(mCardSide, mCardKind);
            }
            break;
        case 11: {
            InitMainScreen();
            u8 side = mCardSide;
            mCardSpec = (u32) new BattleCardSpec(0xb, entries[(u8) (side ^ 1)].mBakuganId, entries[(u8) (side ^ 1)].mAttribute,
                                               entries[(u8) (side ^ 1)].mCharacterId == 0x15 ? 1 : 0);
            mContext->mRequest = 0;
            mUnk_2a           = 0x40;
            mCardStep++;
            break;
        }
        case 12:
            mUnk_2a++;
            if (mUnk_2a > 0xa0) {
                mUnk_2a = 0;
                BattleCardSpec *spec = (BattleCardSpec *) mCardSpec;
                spec->mUnk_1e = 0;
                CloseCardModel(spec->mCardModel);
                mCardStep++;
            }
            break;
        case 13:
            if (mContext->mRequest != 0) {
                mContext->mRequest = 0;
                SpriteManager::Get().ResetSlots();
                mHud->StartAbilitySlideOut(mCardSide);
                mCardStep = 0x12;
            }
            break;
        case 10:
            if (mHud->IsAbilitySlideDone()) {
                s32 other = mCardSide ^ 1;
                mCardSide   = other;
                mCardId   = entries[other & 0xff].mCardId;
                mPrevCardKind   = mCardKind;
                mCardKind   = GetCardInfo(entries[mCardSide].mCardId, 3);
                SetSlot6cFlag(data_020d433c.mBattlePlayers->mPlayers[entries[mCardSide].mPlayer], entries[mCardSide].mSlot6cIndex, 2);
                mHud->mUnk_118[mCardSide] = 0;
                mCardStep                    = 2;
            }
            break;
        case 14:
            InitMainScreen();
            if (mUnk_54_2) {
                mCardId = 0x8c;
            }
            switch (mCardId) {
                case 0x8c: {
                    u8 side = mCardSide;
                    if (mLocalSide >= 2) {
                        side = 0;
                    }
                    mCardSpec = (u32) new BattleCardSpec(0xc, entries[side].mBakuganId, entries[side].mAttribute, entries[side].mCharacterId == 0x15 ? 1 : 0);
                    mBattleData->mEntries[0].mUnk_0a = entries[0].mUnk_06 + entries[0].mUnk_08;
                    mBattleData->mEntries[1].mUnk_0a = entries[1].mUnk_06 + entries[1].mUnk_08;
                    mBattleData->mEntries[0].mUnk_02 = entries[0].mUnk_04;
                    mBattleData->mEntries[1].mUnk_02 = entries[1].mUnk_04;
                    mUnk_54_1                    = 1;
                    mBattleData->mCellAnims[4]->mFrameIndex = 1;
                    s8 v = data_ov007_02279158[0];
                    mBattleData->mCellAnims[4]->mPaletteBase = v;
                    mBattleData->mCellAnims[3]->mPaletteBase = v;
                    break;
                }
                case 0x8d: {
                    mCardSide ^= 1;
                    mCardSpec = (u32) new BattleCardSpec(0xc, entries[mCardSide].mBakuganId, entries[mCardSide].mAttribute, entries[mCardSide].mCharacterId == 0x15 ? 1 : 0);
                    mBattleData->mEntries[mCardSide].mUnk_0a = entries[mCardSide].mUnk_06 + entries[mCardSide].mUnk_08;
                    mBattleData->mEntries[mCardSide].mUnk_02 = entries[mCardSide].mUnk_04;
                    s8 v = data_ov007_02279158[0];
                    mBattleData->mCellAnims[4]->mFrameIndex = 1;
                    mBattleData->mCellAnims[4]->mPaletteBase = v;
                    mBattleData->mCellAnims[3]->mPaletteBase = v;
                    break;
                }
                default: {
                    if (mUnk_54_1 && mLocalSide != 1) {
                        mCardSide = 0;
                    }
                    mCardSpec = (u32) new BattleCardSpec(5, entries[mCardSide].mBakuganId, entries[mCardSide].mAttribute, entries[mCardSide].mCharacterId == 0x15 ? 1 : 0);
                    if (mUnk_54_1) {
                        mBattleData->mEntries[0].mUnk_02 = entries[0].mUnk_00 + entries[0].mUnk_0a;
                        mBattleData->mEntries[1].mUnk_02 = entries[1].mUnk_00 + entries[1].mUnk_0a;
                    } else {
                        mBattleData->mEntries[mCardSide].mUnk_02 = entries[mCardSide].mUnk_00 + entries[mCardSide].mUnk_0a;
                    }
                    mBattleData->mCellAnims[4]->mFrameIndex = 0;
                    s8 v = data_ov007_02279d3c[0];
                    mBattleData->mCellAnims[4]->mPaletteBase = v;
                    mBattleData->mCellAnims[3]->mPaletteBase = v;
                    break;
                }
            }
            mContext->mRequest = 0;
            mUnk_2a           = 0x40;
            mCardStep++;
            break;
        case 15:
            RaiseBlendAlpha();
            if (mUnk_54_1) {
                mBattleData->DrawEntryNumber(0, 2, 8, 0x64, 0, 0, mUnk_2a, 0x40);
                mBattleData->DrawEntryNumber(1, 2, 0xb8, 0x64, 0, 0, mUnk_2a, 0x40);
            } else {
                mBattleData->DrawEntryNumber(mCardSide, 2, gEntryNumberX[mCardSide], 0x64, 0, 0, mUnk_2a, 0x40);
            }
            mUnk_2a--;
            if (mUnk_2a == 0) {
                mUnk_2a = 0;
                mCardStep++;
            }
            break;
        case 16: {
            u8 flag = 0;
            if (mUnk_54_1) {
                if (mBattleData->func_ov007_0223dd6c()) {
                    flag = 1;
                }
                mHud->mDrawnValues[0] = (u32) entries[0].mUnk_00;
                mHud->mDrawnValues[1] = (u32) entries[1].mUnk_00;
                mBattleData->DrawEntryNumber(0, 2, 8, 0x64, 0, 1, 0, 0);
                mBattleData->DrawEntryNumber(1, 2, 0xb8, 0x64, 0, 1, 0, 0);
            } else {
                if (mBattleData->func_ov007_0223ddac(mCardSide)) {
                    flag = 1;
                }
                mHud->mDrawnValues[mCardSide] = (u32) entries[mCardSide].mUnk_00;
                mBattleData->DrawEntryNumber(mCardSide, 2, gEntryNumberX[mCardSide], 0x64, 0, 1, 0, 0);
            }
            if (flag) {
                if (mCardId == 0x8d) {
                    mCardSide ^= 1;
                }
                if (mPrevCardKind == 5 && mCardKind == 4) {
                    mHud->StartAbilitySlideAcross(mCardSide);
                } else if (mUnk_54_1) {
                    mHud->StartAbilitySlideOut(0);
                    mHud->StartAbilitySlideOut(1);
                } else {
                    mHud->StartAbilitySlideOut(mCardSide);
                }
                BattleCardSpec *spec = (BattleCardSpec *) mCardSpec;
                spec->mUnk_1e = 0;
                CloseCardModel(spec->mCardModel);
                mCardStep++;
            }
            break;
        }
        case 17:
            if (mContext->mRequest != 0) {
                mContext->mRequest = 0;
                SpriteManager::Get().ResetSlots();
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                mCardStep                      = 0x12;
                mBattleData->mCellAnims[4]->mFrameIndex = 0;
                s8 v = data_ov007_02279d3c[0];
                mBattleData->mCellAnims[4]->mPaletteBase = v;
                mBattleData->mCellAnims[3]->mPaletteBase = v;
            }
            break;
        case 18:
            if (!mHud->IsAbilitySlideDone()) {
                break;
            }
            InitMainScreen();
            InitSubScreen();
            mUnk_54_3 = 0;
            mHud->mUnk_117_3 = 0;
            mHud->LoadCountdown();
            mSub48->LoadCharactersAndPalettes();
            mCardStep++;
        case 19:
            return 1;
    }
    return 0;
}

void BattleBase::func_ov007_0221b8e8() {
    BOOL done = 0;
    if (mUnk_54_0) {
        done = 1;
        mState = mReturnState;
    } else {
        switch (mCardKind) {
        case 0:
            if (!IsHudSequenceRunning(mHud)) {
                mState = 7;
                done = 1;
            }
            break;
        case 1:
        case 3:
        case 4:
            if (!IsHudSequenceRunning(mHud)) {
                mState = 7;
                done = 1;
            }
            break;
        }
    }
    if (done) {
        mHud->mUnk_117 &= ~4;
        mUnk_54_0 = 0;
    }
}

BOOL func_ov007_0221b998(int a, int mode, BattleEntry *entry) {
    if (mode != 1) {
        return 0;
    }
    BOOL result = 0;
    if (entry->mCardId != 0) {
        if (GetCardInfo(entry->mCardId, 3) == 5) {
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
    if (entry->mCardId != 0) {
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
    if (entry->mCardId >= 0x8e && entry->mCardId <= 0x91) {
        return 1;
    }
    return 0;
}

BOOL func_ov007_0221ba38(int a, u8 side, int value, BattleEntry *entries) {
    u8 other = side ^ 1;
    if (entries[other].mCardId == 0) {
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
    if (GetCardInfo(entries[index].mCardId, 3) == 2) {
        switch (entries[index].mCardId) {
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

void BattleBase::SetPenPosition(s16 a, s16 b) {
    BattleHud *p = mHud;
    p->mPenX = a;
    p->mPenY = b;
}

void BattleBase::SetTouched() {
    if (mTouched) {
        return;
    }
    mTouched = 1;
    mHud->mUnk_23 |= 4;
}

extern "C" void G2x_SetMasterBrightness_(vu16 *reg, int value);

void BattleBase::func_ov007_0221bb68() {
    mUnk_25 = mParam->mUnk_0b;
    mUnk_24 = mParam->mUnk_08;
}

BOOL BattleBase::CheckBattleEnd() {
    BattleHud *sub = mHud;
    u8 n = sub->mPushPercent;
    BOOL done = 0;
    mElapsedFrames++;
    if (mElapsedFrames >= mParam->mBattleFrameCount) {
        if (n == data_ov007_0227915c) {
            u8 idx = MATH_Rand32(&gRandom, 2);
            BattleData *d = mBattleData;
            d->mEntries[idx].mUnk_10 = 1;
            d->mEntries[idx].mUnk_0f_6 = 0;
        } else if (n > data_ov007_0227915c) {
            BattleData *data = mBattleData;
            data->mEntries[0].mUnk_10 = 1;
            data->mEntries[0].mUnk_0f_6 = 0;
        } else {
            BattleData *data = mBattleData;
            data->mEntries[1].mUnk_10 = 1;
            data->mEntries[1].mUnk_0f_6 = 0;
        }
        done = 1;
    } else if (n >= data_ov007_02279170 - 1) {
        BattleData *data = mBattleData;
        done = 1;
        data->mEntries[0].mUnk_10 = 1;
        data->mEntries[0].mUnk_0f_6 = 1;
        mHud->mUnk_22 |= 0x20;
    } else if (n <= 1) {
        BattleData *data = mBattleData;
        done = 1;
        data->mEntries[1].mUnk_10 = 1;
        data->mEntries[1].mUnk_0f_6 = 1;
        mHud->mUnk_22 |= 0x20;
    }
    if (done) {
        func_ov007_0222847c(mHud);
        mBattleEnded = 1;
        StopSeqArc(&SoundManager::Get(), 0x24, 1);
    } else if (!mBattleEnded) {
        MovePlayerAnimX(mHud);
    }
    return done;
}

BOOL BattleBase::func_ov007_0221bd84() {
    BOOL done = 0;
    switch (mStep) {
    case 0:
        if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
            StartAbilitiesSlide(mSub48);
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            mStep++;
        }
        break;
    case 1:
        mUnk_2a++;
        if (mUnk_2a >= 0x3c) {
            mUnk_2a = 0;
            mStep++;
        }
        break;
    case 2:
        if (!IsHudSequenceRunning(mHud)) {
            ShowModels(mField3D);
            mStep++;
        }
        break;
    case 3:
        if (MoveToEndThenCreateAuras(mField3D)) {
            SoundManager::Get().StartSeqArc(0x23);
            SoundManager::Get().StartSeqArc(0x24);
            mFlashEnabled = 1;
            mHud->mUnk_22 |= 0x80;
            mHud->mUnk_22 |= 0x10;
            mStep++;
        }
        break;
    case 4:
        if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0x10)) {
            mStep = 0;
            done = 1;
        }
        break;
    }
    return done;
}

BOOL BattleBase::func_ov007_0221bfa4() {
    BOOL done = 0;
    switch (mStep) {
    case 0:
        SetHudState(mHud, HUD_SEQUENCE_BAR_SLIDE);
        mFlashEnabled = 1;
        mStep++;
        break;
    case 1:
        if (!IsHudSequenceRunning(mHud)) {
            if (func_ov007_022297f0(mField3D)) {
                mUnk_2a = 0;
                mStep++;
            }
        }
        break;
    case 2:
        mUnk_2a++;
        if (mUnk_2a >= 0x64) {
            mUnk_2a = 0;
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            mStep++;
        }
        break;
    case 3:
        if (!SpriteManager::Get().IsFading()) {
            mStep++;
        }
        break;
    case 4:
        if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
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
    BattleEntry *entries   = mBattleData->mEntries;
    if (mLocalSide <= 1) {
        if (mLargeCardPressesRemaining != 0) {
            if (TouchManager::Get().IsPressedIn(gLargeCardTouchX, gLargeCardTouchY, gLargeCardTouchWidth,
                                                  gLargeCardTouchHeight)) {
                SoundManager::Get().StartSeqArc(0x31);
                mHud->mShakesRemaining = 5;
                mLargeCardPressesRemaining--;
                if (mLargeCardPressesRemaining == 0) {
                    u8 other = mLocalSide ^ 1;
                    WirelessManager::Get();
                    if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                        func_ov007_0223e214(mBattleData, other, 0);
                    } else {
                        *flag = other + 1;
                    }
                    mHud->CloseLargeCard();
                }
            }
        } else {
            if (data_020d433c.mUnk_9a_2) {
                WirelessManager::Get();
                if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                    if (mHelpers[mLocalSide]->RollForCard(mLocalSide, mHud->mPushPercent)) {
                        c[mLocalSide] = 1;
                    }
                }
            } else if (entries[mLocalSide].mCardId != 0) {
                if (mHud->mUnk_118[mLocalSide] == 1) {
                    if (GetCardInfo(entries[mLocalSide].mCardId, 3) != 4) {
                        if (TouchManager::Get().IsPressedIn(gSmallCardX[mLocalSide], gSmallCardTouchY, 0x10, 0x18) ||
                            InputManager::Get().IsPressed(0x300)) {
                            c[mLocalSide] = 1;
                        }
                    }
                }
            }
            if (c[mLocalSide] == 0) {
                u8 n   = mHud->mPushPercent;
                u8 low = 0;
                if (mLocalSide == 0 && n < 0x32) {
                    low = 1;
                } else if (mLocalSide == 1 && (u8) (100 - n) < 0x32) {
                    low = 1;
                }
                u8 r = UpdateTouch(low);
                if (r) {
                    result = (s8) mLocalSide;
                    if (mLocalSide == 0) {
                        a[side] = r;
                    } else {
                        b[side] = r;
                    }
                }
            }
        }
    } else {
        u8 base = data_ov007_02279160 - 0x10;
        if (mSpectatorTouchSide == 2) {
            if (mLocalSide == 2 || mLocalSide == 3) {
                if (func_ov007_02229ee4((u16) mHud->mSide0X, base, 0x50, 0x90, 10, &mUnk_1a)) {
                    mSpectatorTouchSide = 0;
                    mSpectatorHoldFrames = 0;
                    SetTouched();
                }
            }
            if (mSpectatorTouchSide == 2) {
                if (mLocalSide == 2 || mLocalSide == 4) {
                    if (func_ov007_02229ee4((u16) ((u16) mHud->mSide1X - 0x10), base, 0x50, 0x90, 10, &mUnk_1a)) {
                        mSpectatorTouchSide = 1;
                        mSpectatorHoldFrames = 0;
                        SetTouched();
                    }
                }
            }
        } else if (TouchManager::Get().mReleased) {
            mSpectatorTouchSide = 2;
            mUnk_1a = 0;
        } else {
            u16 v;
            if (mSpectatorTouchSide == 0) {
                v = (u16) mHud->mSide0X;
            } else {
                v = (u16) mHud->mSide1X - 0x10;
            }
            if (func_ov007_02229ee4(v, base, 0x50, 0x90, 10, &mUnk_1a)) {
                WirelessManager::Get();
                if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                    mSpectatorHoldFrames++;
                    if (mSpectatorHoldFrames >= 0x14) {
                        mSpectatorHoldFrames = 0;
                        result = (s8) mSpectatorTouchSide;
                        if (mSpectatorTouchSide == 0) {
                            a[side] = mUnk_13;
                        } else {
                            b[side] = mUnk_13;
                        }
                    }
                    s8 *pair = (s8 *) this + mSpectatorTouchSide * 2;
                    if (pair[0x1c] == -1) {
                        pair[0x1c] = side;
                    } else {
                        pair[0x1d] = side;
                    }
                } else {
                    mSpectatorHoldFrames++;
                    if (mSpectatorHoldFrames >= 0x14) {
                        mSpectatorHoldFrames = 0;
                        result = (s8) mSpectatorTouchSide;
                        if (mSpectatorTouchSide == 0) {
                            a[side] = 2;
                        } else {
                            b[side] = 2;
                        }
                    } else {
                        if (mSpectatorTouchSide == 0) {
                            a[side] = 1;
                        } else {
                            b[side] = 1;
                        }
                    }
                }
            } else {
                mSpectatorTouchSide = 2;
                mUnk_1a = 0;
            }
        }
    }
    return result;
}

int BattleBase::UpdateSlots(u16 *a, u16 *b, u16 *c, u8 *d) {
    u8 i = 0;
    do {
        if (data_020d433c.mBattlePlayers->mPlayers[i]->IsCpu()) {
            if (mSlots[i]->Update(a, b, c, (s8 (*)[2]) &mUnk_1c, mHud->mPushPercent, d) != -1) {
                break;
            }
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    return -1;
}

void BattleBase::TryStartCardSequence(u16 *p) {
    if (p[0] != 0) {
        mCardSide = 0;
        mState = 8;
    } else if (p[1] != 0) {
        mCardSide = 1;
        mState = 8;
    }
}

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
    const u8 *mode = &data_020c4258;
    if (IsWirelessMode(mode)) {
        func_ov007_02229d08(this);
    }
    int found = func_ov007_0221c178(a, b, c, &flag);
    BOOL doneB = 1;
    BOOL doneA = 1;
    if (found != -1) {
        if (mLocalSide <= 1) {
            mUnk_25++;
            if (mUnk_25 >= mParam->mUnk_0b) {
                mUnk_25 = 0;
            } else {
                doneB = 0;
            }
            mUnk_24++;
            if (mUnk_24 >= mParam->mUnk_08) {
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
        if (!IsWirelessMode(mode)) {
            if (doneB) {
                k = 0;
                u32 n;
                do {
                    mHud->StartObject(side, TouchManager::Get().mX, TouchManager::Get().mY, mMode);
                    n = mHud->ComputeObjectCount(value, mParam->mUnk_09, mParam->mUnk_0a);
                    k++;
                } while (k < n);
            }
            if (!doneA) {
                a[sel] = 0;
                b[sel] = 0;
            }
        } else {
            if (doneB) {
                u16 n = mHud->ComputeObjectCount(value, mParam->mUnk_09, mParam->mUnk_0a) << 8;
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
    if (!IsWirelessMode(mode)) {
        UpdateSlots(a, b, c, &flag);
        u8 i = 0;
        u8 count = data_020d433c.mPlayerCount;
        do {
            sums[0] += a[i];
            sums[1] += b[i];
            i++;
        } while (i < count);
        BattleEntry *entries = mBattleData->Entries();
        i = 0;
        do {
            if (sums[i] != 0) {
                s32 v = 0x1000;
                s8 cur = mMultiplierIndex;
                if (cur != -1) {
                    v = gAttributeRates[cur].mUnk[entries[i].mAttribute];
                    if (entries[i].mBakuganId == 5) {
                        if (v < gAttributeRates[cur].mUnk[3]) {
                            v = gAttributeRates[cur].mUnk[3];
                        }
                    } else if (entries[i].mBakuganId == 6) {
                        if (v < gAttributeRates[cur].mUnk[0]) {
                            v = gAttributeRates[cur].mUnk[0];
                        }
                    }
                }
                if (mRateFloorSide == i && v < 0x1000) {
                    v = 0x1000;
                }
                mHud->Push(i, FX_Mul(v, mParam->mPushScale), sums[i], mUnk_52[i], 1);
            }
            i++;
        } while (i < 2);
        TryStartCardSequence(c);
    } else {
        func_ov007_02229c88(this, a, b, c, flag);
    }
}

class Camera;




void BattleBase::func_ov007_0221cebc(int a) {
    func_ov007_0223e214(mBattleData, a, 0);
}

void BattleBase::func_ov007_0221ced0(u16 *a, u16 *b, u16 *c) {
    if (!data_020d433c.mUnk_9a_2) {
        return;
    }
    u8 i = 0;
    BattleEntry *entries = mBattleData->mEntries;
    do {
        if (mHelpers[i]) {
            u8 other = i ^ 1;
            if (!entries[other].mUnk_0f_3) {
                if (mHelpers[i]->RollForCard(i, mHud->mPushPercent)) {
                    c[i] = 1;
                    if (i == 0) {
                        a[entries[0].mPlayer] = 0;
                    } else {
                        b[entries[1].mPlayer] = 0;
                    }
                }
            }
        }
        i++;
    } while (i < 2);
}

void BattleBase::func_ov007_0221cfac(u16 *a, u16 *b) {
    BattleData *data = mBattleData;
    u8 i = 0;
    do {
        if (i != data->mEntries[0].mPlayer && i != data->mEntries[1].mPlayer) {
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
                    mHud->StartObject(a, TouchManager::Get().mX, TouchManager::Get().mY, mMode);
                } else if (!data_020d433c.mBattlePlayers->mPlayers[i]->IsCpu()) {
                    BattleRand32(0xffff);
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
        BattleEntry *entries = mBattleData->mEntries;
        s8 cur = mMultiplierIndex;
        if (cur != -1) {
            BattleEntry *e = &entries[a];
            v = gAttributeRates[cur].mUnk[e->mAttribute];
            if (e->mBakuganId == 5) {
                if (v < gAttributeRates[cur].mUnk[3]) {
                    v = gAttributeRates[cur].mUnk[3];
                }
            } else if (e->mBakuganId == 6) {
                if (v < gAttributeRates[cur].mUnk[0]) {
                    v = gAttributeRates[cur].mUnk[0];
                }
            }
        }
        mHud->Push(a, FX_Mul(v, mParam->mPushScale), total, mUnk_52[a], 1);
    }
    if (mParam->mKind == 5) {
        vfunc_44(a, total);
    }
}

extern "C" void StopSeqArc(SoundManager *manager, u32 a, u32 b);

u32 BattleRand32(u32 max) {
    return MATH_Rand32(&gRandom, max);
}

BOOL BattleBase::UpdateFlash() {
    BOOL done = 0;
    if (mFlashFalling == 0) {
        mFlashBrightness += 3;
        if (mFlashBrightness >= 15) {
            if (mBattleEnded) {
                FreeAuras(mField3D);
                SoundManager::Get().StartSeqArc(0x25);
                ClearSub48State(mSub48);
            }
            mFlashFalling++;
        }
    } else {
        mFlashBrightness -= 3;
        if (mFlashBrightness <= 0) {
            mFlashBrightness = 0;
            mFlashFalling = 0;
            done = 1;
        }
    }
    G2x_SetMasterBrightness_((vu16 *) 0x400006c, mFlashBrightness);
    G2x_SetMasterBrightness_((vu16 *) 0x400106c, mFlashBrightness);
    return done;
}

void BattleBase::func_ov007_0221d488(u16 *p) {
    TryStartCardSequence(p);
}

int BattleBase::func_ov007_0221d494(u16 *a, u16 *b, u16 *c, u8 *d) {
    return UpdateSlots(a, b, c, d);
}
