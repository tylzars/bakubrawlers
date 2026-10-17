#include "Battle/BattleAbility.hpp"

// Placeholder: the ROM has thirteen identical {0, 0x4800, 0} constants (one per use below). As literals they
// sort into a different order than the original's, and no include or definition order found reproduces it, so
// they are named here and defined in the order that gives the original layout.
static const VecFx32 sEffectPos_02266868 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_022665ec = {0, 0x4800, 0};
static const VecFx32 sEffectPos_02265718 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_02265af8_1 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_022663f0 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_02266a28_1 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_022652c8 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_02265fac_1 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_02265fac_0 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_02266a28_0 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_02265af8_0 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_02266a28_2 = {0, 0x4800, 0};
static const VecFx32 sEffectPos_02265118 = {0, 0x4800, 0};

#include "Battle/BattleUnitModels.hpp"
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
#include "Graphics/BakuganModel.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include <nitro/gx.h>
#include "Game/Random.hpp"
#include <nitro/math.h>

BattleAbility::BattleAbility()
    : SceneBase(0x3e8, 0) {
    mState = 0;
    mNotLocal = 0;
    mIsCpu = 0;
    mUnk_0f = 0;
    mAbilityIndex = 0;
    mUnk_14 = 0;
    mPlayer = 0;
    mUnit = 0;
    mNextScene = 1;
    mUnk_1a = 0;
    mUnionAbility = 0;
    mCommandPage = 0;
    mStep = 0;
    mDestUnit = 0xff;
    mDestMarker = 0;
    mPickIndex = 0;
    mEffects = data_020d433c.mEffects;
    mCameraStep = 0;
    mOrbitAngle = 0;
    mAmountStep = 0;
    mAmountDigitCount = 0;
    mSubtract = 0;
    mUnk_62_1 = 0;
    mStIconSet = 0;
    mAmountFrames = 0;
    mAmountY = 0;
    mAmountYPerFrame = 0;
    mAmountRemaining = 0;
    mShownAmount = 0;
    mOtherUnitCount = 0;
    mPlayer = data_020d433c.mBattlePlayers->mTurnPlayer;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 own = list->mUnk_2a9;
    mRecord = list->mPlayers[mPlayer]->mRecord;
    if (mPlayer != own) {
        mNotLocal = 1;
        if (data_020d433c.mBattlePlayers->mPlayers[mPlayer]->IsCpu()) {
            mIsCpu = 1;
        }
    }
    mTarget = data_020d433c.mBattlePlayers->mBattleTarget;
    mUnit = mTarget->mUnitHandle - 1;
    mAbilityId = data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mAbilityId;
    mAbilityIndex = mAbilityId - 0xc9; // 0xc9 (ABILITY_ID_REPEAT_TURN) is the lowest id: the sAbilityHandlers row
    mCamera = data_020d433c.mBattlePlayers->mCamera;
    u8 i = 0;
    do {
        mNumberCellAnims[i] = 0;
        i++;
    } while (i < 2);
    LoadGraphics();
}

BattleAbility::~BattleAbility() {
    u8 i = 0;
    do {
        CellAnimation *a = mNumberCellAnims[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 2);
    mEffects->FreeAllEffects();
    FreeEffectGroup(mEffects, 0);
}

BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);
extern UnkStruct_0220f540 data_0220f540;

// Copies the active camera into the 3D engine state and the camera manager's snapshot of it.
static inline void SnapshotCamera() {
    CameraManager &manager        = CameraManager::Get();
    Camera *camera                = manager.mCameras[manager.mCameraIndex];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mCameraIndex] = data_02211374.mProjection;
    manager.mView[manager.mCameraIndex]       = data_02211374.mView;
    manager.mMtxA[manager.mCameraIndex]       = *func_02032710();
    manager.mMtxB[manager.mCameraIndex]       = *func_02032604();
    manager.mMtxC[manager.mCameraIndex]       = *func_020323b8();
}

typedef u8 (BattleAbility::*BattleAbilityHandler)();
static BattleAbilityHandler sAbilityHandlers[12] = {
    &BattleAbility::func_ov007_02265118,
    &BattleAbility::func_ov007_022652c8,
    &BattleAbility::UpdateReassignMarker,
    &BattleAbility::UpdateReplaceRecord,
    0,
    &BattleAbility::UpdateMoveToUnit,
    &BattleAbility::UpdateMoveToMarker,
    &BattleAbility::UpdateDefenseUp,
    &BattleAbility::func_ov007_022665ec,
    &BattleAbility::func_ov007_02266868,
    &BattleAbility::func_ov007_02266a28,
    &BattleAbility::func_ov007_02266a28,
};

void BattleAbility::Update() {
    switch (mState) {
        case 0:
            mCommandPage = new BattleCommandPage(mAbilityId, -1);
            mUnk_14 = func_ov007_02264a5c();
            func_02055738(mCommandPage, 0x2000, 7, 1);
            mState++;
            break;
        case 1:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            OpenUnionAbility();
            break;
        case 2: {
            u8 result = mContext->mRequest;
            if (result == 0) {
                break;
            }
            mContext->mRequest = 0;
            switch (result) {
                case 1:
                    mUnk_0f = 3;
                    break;
                case 2:
                    SpriteManager::Get().sFade[1] = FADE_MODE_DOWN;
                    mUnk_0f                         = 1;
                    break;
            }
            CloseCommandPage(mCommandPage, 1);
            if ((u16) (mAbilityId + 0xff35) <= 1 && result == 1) { // id 0xcb or 0xcc (REASSIGN_MARKER, REPLACE_RECORD)
                mState++;
                break;
            }
            mState = 10;
            if (mAbilityId == 0xcd) {
                SpriteManager::Get().sFade[1] = FADE_MODE_DOWN;
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
            if (mContext->mRequest == 0) {
                break;
            }
            mContext->mRequest = 0;
            if (mAbilityId == ABILITY_ID_REASSIGN_MARKER) {
                mCommandPage = new BattleCommandPage(0, -1);
                SetCommandPageCardHidden(mCommandPage, 1);
                new BattleCommandPrompt(1, mCommandPage, 0, 0, 0);
            } else {
                mCommandList = new BattleCommandList(0, 0, 0, 0);
                SetCommandListBakuganHidden(mCommandList, 1);
                new BattleCommandPrompt(2, mCommandList, 0, 0, 0);
            }
            mState++;
            break;
        case 4: {
            u16 result = mContext->mRequest;
            if (result == 0) {
                break;
            }
            if (result != 10) {
                mPickIndex = GetLowByte(result);
                WirelessManager::Get();
                if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                    ((u8 *) &data_0220f540)[0] = 0x14;
                    ((u8 *) &data_0220f540)[4] = mUnk_0f;
                    ((u8 *) &data_0220f540)[5] = mPickIndex;
                }
                mState = 6;
            } else {
                mState = 5;
            }
            if (mAbilityId == ABILITY_ID_REASSIGN_MARKER) {
                CloseCommandPage(mCommandPage, 1);
            } else {
                CloseCommandList(mCommandList);
            }
            mContext->mRequest = 0;
            break;
        }
        case 5:
            if (mContext->mRequest == 0) {
                break;
            }
            mContext->mRequest = 0;
            mCommandPage = new BattleCommandPage(mAbilityId, -1);
            OpenUnionAbility();
            mState = 2;
            break;
        case 6:
            if (mContext->mRequest == 0) {
                break;
            }
            mContext->mRequest = 1;
            if (mAbilityId == ABILITY_ID_REPLACE_RECORD) {
                CameraManager::Get().Reset();
                CameraManager::Get().mCameraIndex = 0;
                CameraManager::Get().SetCamerasAndRequestMode(1, mCamera, 0);
                SnapshotCamera();
            }
            mState = 10;
            break;
        case 7:
            if ((u8) mContext->mRequest != 1) {
                break;
            }
            mContext->mRequest = 0;
            mUnionAbility->Close(0xff);
            CloseCommandPage(mCommandPage, 3);
            mNextScene = 2;
            mState  = 0x10;
            WirelessManager::Get();
            if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                break;
            }
            ((u8 *) &data_0220f540)[0] = 0x14;
            break;
        case 8: {
            BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
            if (player->mMsg.mType != 0x14) {
                break;
            }
            mContext->mRequest = 0;
            mUnionAbility->Close(0xff);
            if (mUnk_14 == 1) {
                CloseCommandPage(mCommandPage, 3);
                mNextScene = 2;
                mState  = 0x10;
                break;
            }
            mUnk_0f = player->mMsg.mTouchDown;
            mPickIndex = player->mMsg.mTouchX;
            CloseCommandPage(mCommandPage, 1);
            mState = 10;
            if (mUnk_0f == 1 || mAbilityId == 0xcd) {
                SpriteManager::Get().sFade[1] = FADE_MODE_DOWN;
            }
            break;
        }
        case 9:
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                mUnk_0f = mUnk_14 == 1 ? 2 : 3;
                mContext->mRequest = 0;
                mUnionAbility->Close(0xff);
                CloseCommandPage(mCommandPage, 1);
                mState = 10;
                if (mAbilityId == 0xcd) {
                    SpriteManager::Get().sFade[1] = FADE_MODE_DOWN;
                }
            }
            break;
        case 10:
            if (mContext->mRequest == 0) {
                break;
            }
            mContext->mRequest = 0;
            mTarget->mUnk_1e7_1 = 1;
            mState++;
            break;
        case 11:
            if (!SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
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
                    if (mAbilityId != 0xcd) {
                        mState = 0xd;
                        break;
                    }
                    data_020d433c.mBattlePlayers->mUnk_2ac = 3;
                    mState                                 = 0xe;
                    break;
            }
            break;
        case 13:
            if ((this->*sAbilityHandlers[mAbilityIndex])()) {
                SpriteManager::Get().sFade[0] = FADE_MODE_DOWN;
                SpriteManager::sFade[1]        = FADE_MODE_DOWN;
                mState                           = 0xe;
            }
            break;
        case 14:
            if (!SpriteManager::Get().IsFading()) {
                mState = 0x12;
            }
            break;
        case 16:
            if (mContext->mRequest == 0) {
                break;
            }
            mContext->mRequest = 0;
            mState++;
            break;
        case 17:
            if (!SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0x10)) {
                break;
            }
            mState++;
            break;
        case 18:
            RequestScene(mNextScene);
            mFinished = 1;
            break;
    }
}

void BattleAbility::LoadGraphics() {
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.SetObjVramMode(1);
    ChangeDir("/Game/Battle/Obj/");
    screen.LoadCharacters("vs_u_obj_num_32k.nce_ncg1D.bin", 0x6000, 0);
    screen.LoadPalette("vs_u_obj_num_32k_ncl.bin", 0x180, 0x40);
    u8 i = 0;
    do {
        mNumberCellAnims[i] = new CellAnimation("vs_u_obj_num_32k_nce.bin", 0, 1, 1);
        mNumberCellAnims[i]->mCharBase = screen.GetCharBase(0x6000);
        mNumberCellAnims[i]->mPaletteBase   = 12;
        i++;
    } while (i < 2);
    mNumberCellAnims[0]->SetAnimation(0);
    mNumberCellAnims[1]->SetAnimation(1);
    ChangeDir("/");
    ChangeDir("/Game/2D/union_b");
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ, 0x2000, "union_b_obj_page.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_OBJ_PLTT, 0xe0, "union_b_obj_page_ncl.bin", 0, 0, 0);
    ChangeDir("/");
}

void BattleAbility::OpenUnionAbility() {
    if (mNotLocal == 0) {
        switch (mUnk_14) {
            case 0:
                mUnionAbility = new BattleUnionAbility(3, 0x1a000, 0xc);
                mState  = 2;
                break;
            case 1:
                mUnionAbility = new BattleUnionAbility(5, 0x1a000, 0xc);
                mState  = 7;
                break;
        }
    } else {
        mUnionAbility = new BattleUnionAbility(4, 0x1a000, 0xc);
        if (mIsCpu != 0) {
            mState = 9;
        } else {
            mState = 8;
        }
    }
}

BOOL func_ov007_022677cc(BattleAbility *self);
u32 func_ov007_02267884(BattleAbility *self);
BOOL func_ov007_02267944(BattleAbility *self, u16 *out);

// Record and marker indices read into enum-typed locals: a real load (mwcc does not substitute enum locals), so the
// index is coloured after the player or entry base.
enum UnitIndex { UNIT_INDEX_0 };

BOOL BattleAbility::func_ov007_02264a5c() {
    BOOL result = 1;
    switch (mAbilityId - 0xc9) { // sAbilityHandlers row (0xc9 = ABILITY_ID_REPEAT_TURN)
        case 0:
            result = 0;
            break;
        case 1:
            result = 0;
            break;
        case 2:
            if (*((u8 *) data_020d433c.mBattlePlayers->mPlayers[mPlayer] + 0xf8) != 0) {
                result = 0;
            }
            if (mIsCpu != 0 && result == 0) {
                u8 i        = 0;
                u8 slots[3] = {0, 0, 0};
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                u8 n        = 0;
                u8 count    = data_020d433c.mUnk_99;
                do {
                    if (((u8 *) list->mPlayers[mPlayer] + 0x56)[i * 4] == 0) {
                        slots[n] = i;
                        n++;
                    }
                    i++;
                } while (i < count);
                mPickIndex = slots[MATH_Rand32(&gRandom, n)];
            }
            break;
        case 3: {
            u8 i             = 0;
            BattlePlayer *pl = data_020d433c.mBattlePlayers->mPlayers[mPlayer];
            u8 count         = data_020d433c.mUnk_99;
            do {
                if (pl->mRecords[i].mStatus == 0) {
                    result = 0;
                    break;
                }
                i++;
            } while (i < count);
            if (mIsCpu != 0 && result == 0) {
                u8 n;
                BattlePlayerList *list;
                u8 count2;
                u8 j        = 0;
                u8 slots[3] = {0, 0, 0};
                count2   = data_020d433c.mUnk_99;
                list = data_020d433c.mBattlePlayers;
                n        = 0;
                do {
                    if (list->mPlayers[mPlayer]->mRecords[j].mStatus == 0) {
                        slots[n] = j;
                        n++;
                    }
                    j++;
                } while (j < count2);
                mPickIndex = slots[MATH_Rand32(&gRandom, n)];
            }
            break;
        }
        case 4:
            result = 0;
            break;
        case 5:
            if (mTarget->mResult == 2) {
                u8 x;
                u8 y;
                u8 n         = 0;
                u8 found[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                x = 0;
                do {
                    y = 0;
                    do {
                        u8 v = data_020d433c.mBattlePlayers->mUnitHandles[y][x];
                        if (v != 0) {
                            u8 unit = v - 1;
                            u8 me = mPlayer;
                            u8 owner = data_020d433c.mBattlePlayers->mUnits[unit].mPlayer;
                            if (owner != me) {
                                if (data_020d433c.mHasPartners != 0) {
                                    if (owner != data_020d433c.mBattlePlayers->mPlayers[me]->mPartner) {
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
                    u8 pick = MATH_Rand32(&gRandom, n);
                    mDestUnit = found[pick];
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
                    u8 v = data_020d433c.mBattlePlayers->mMarkerHandles[y][x];
                    if (v != 0) {
                        pairs[n][0] = v - 1;
                        pairs[n][1] = 0xff;
                        u8 w        = data_020d433c.mBattlePlayers->mUnitHandles[y][x];
                        if (w == 0) {
                            n++;
                        } else {
                            u8 unit  = w - 1;
                            u8 owner = data_020d433c.mBattlePlayers->mUnits[unit].mPlayer;
                            if (owner != mPlayer) {
                                if (data_020d433c.mHasPartners != 0) {
                                    if (owner != data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mPartner) {
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
                u8 pick = MATH_Rand32(&gRandom, n);
                mDestMarker = pairs[pick][0];
                result  = 0;
                mDestUnit = pairs[pick][1];
            }
            break;
        }
        case 7:
            if (mTarget->mResult == 2) {
                result = 0;
            }
            break;
        case 8: {
            BattleUnit *unit      = &data_020d433c.mBattlePlayers->mUnits[mUnit];
            UnitIndex rec         = (UnitIndex) unit->mRecord;
            BattleRecord *record  = &data_020d433c.mBattlePlayers->mPlayers[unit->mPlayer]->mRecords[rec];
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
            if (mIsCpu == 0) {
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
            if (mIsCpu == 0) {
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

void OrbitCamera(BattleAbility *self);

u8 BattleAbility::func_ov007_02265118() {
    u8 done = 0;
    switch (mStep) {
        case 0: {
            VecFx32 pos = sEffectPos_02265118;
            pos.x       = mTarget->mBakugan->mPosition.x;
            pos.z       = mTarget->mBakugan->mPosition.z;
            LoadEffectGroup(mEffects, 0, "shuto_return");
            s16 handle = StartEffect(mEffects, 0, 0, 0, 1);
            SetEffectPosition(mEffects, handle, &pos);
            SoundManager::Get().StartSeqArc(0x5a);
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            mStep++;
            break;
        }
        case 1:
            OrbitCamera(this);
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

u8 BattleAbility::func_ov007_022652c8() {
    u8 done = 0;
    switch (mStep) {
        case 0: {
            VecFx32 pos = sEffectPos_022652c8;
            pos.x       = mTarget->mBakugan->mPosition.x;
            pos.z       = mTarget->mBakugan->mPosition.z;
            LoadEffectGroup(mEffects, 0, "shuto_return");
            s16 handle = StartEffect(mEffects, 0, 0, 0, 1);
            SetEffectPosition(mEffects, handle, &pos);
            SoundManager::Get().StartSeqArc(0x5a);
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            mStep++;
            break;
        }
        case 1:
            OrbitCamera(this);
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
void SetMarkerPlayerSlot(BattlePlayerList *list, u8 a, u8 b, u8 c);
void GetCellPosition(VecFx32 *out, u8 a, u8 b);
extern "C" void StartMarkerSlide(BattleCommandMap *map);

// The target and eye are built as whole vectors and stored by struct copy (the `mov r2, ip` copy of pos.x), with the
// eye computed before either store; the camera pointer is read once up front and again for the eye store.
u8 BattleAbility::UpdateReassignMarker() {
    u8 done = 0;
    switch (mStep) {
        case 0:
            SoundManager::Get().StartSeqArc(0x5b);
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            mStep++;
            break;
        case 1: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            UnitIndex marker       = (UnitIndex) list->mUnits[mUnit].mMarker;
            BattleMarker *entries  = list->Markers();
            list->mMarkers[marker].mStatus = 3;
            VecFx32 pos;
            GetCellPosition(&pos, entries[marker].mCellX, entries[marker].mCellY);
            Camera *cam = mCamera;
            u16 angle   = 0x2000;
            int idx     = angle >> 4;
            s32 sinV    = FX_SinCosTable_[idx * 2 + 1];
            s32 cosV    = FX_SinCosTable_[idx * 2];
            VecFx32 target;
            target.x = pos.x;
            target.y = 0xc000;
            target.z = pos.z;
            VecFx32 eye;
            eye.x = target.x + sinV * 50;
            eye.y = 0x64000;
            eye.z = target.z + cosV * 50;
            cam->mTarget       = target;
            mCamera->mPosition = eye;
            StartMarkerSlide(mTarget->mCommandMap);
            mStep++;
            break;
        }
        case 2:
            if (data_020d433c.mBattlePlayers->mMarkers[data_020d433c.mBattlePlayers->mUnits[mUnit].mMarker].mStatus != 3) {
                mStep++;
            }
            break;
        case 3:
            mUnk_1a++;
            if (mUnk_1a > 0x78) {
                SetMarkerPlayerSlot(data_020d433c.mBattlePlayers, data_020d433c.mBattlePlayers->mUnits[mUnit].mMarker, mPlayer, mPickIndex);
                mUnk_1a = 0;
                done    = 1;
            }
            break;
    }
    return done;
}

void ReplaceUnitRecord(BattlePlayerList *list, u8 a, u8 b);

u8 BattleAbility::UpdateReplaceRecord() {
    u8 done = 0;
    switch (mStep) {
        case 0: {
            VecFx32 pos = sEffectPos_02265718;
            pos.x       = mTarget->mBakugan->mPosition.x;
            pos.z       = mTarget->mBakugan->mPosition.z;
            LoadEffectGroup(mEffects, 0, "bkn_exchange");
            s16 handle = StartEffect(mEffects, 0, 0, 0, 1);
            SetEffectPosition(mEffects, handle, &pos);
            SoundManager::Get().StartSeqArc(0x5c);
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            mStep++;
            break;
        }
        case 1:
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                SpriteManager::Get();
                SpriteManager::sFade[0] = FADE_MODE_UP;
                mStep++;
            }
            break;
        case 2:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            ReplaceUnitRecord(data_020d433c.mBattlePlayers, mUnit, mPickIndex);
            {
                BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mRecords[mPickIndex];
                ((BakuganModel *) mTarget->mBakugan)->Setup(record->mBakuganId, record->mAttribute,
                                                            BAKUGAN_MODEL_MODE_H_HOLD_FRAME);
            }
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            mStep++;
            break;
        case 3:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mStep++;
            break;
        case 4:
            mUnk_1a++;
            if (mUnk_1a > 0x78) {
                mUnk_1a = 0;
                done    = 1;
            }
            break;
    }
    OrbitCamera(this);
    return done;
}

extern "C" void MI_CpuCopy8(const void *src, void *dst, u32 size);
void SetTransformPosition(void *obj, fx32 x, fx32 y, fx32 z);
void RemoveUnit(BattlePlayerList *list, u8 a, u8 b);
u8 AddUnit(BattlePlayerList *list, const VecFx32 *pos, u16 value, u8 player, u8 record, u8 marker, u8 flag);
void SetUnitAB(BattlePlayerList *list, u8 a, u8 b);
BOOL UpdateCameraShiftTo(BattleAbility *self, VecFx32 *out);

// The unit stride 0x14 is an enum-typed value, so it is kept in a register across the calls (sl).
enum UnitSize { UNIT_SIZE = 0x14 };

// Unit `index` of the table: an inline, so the address is a temporary (coloured before `this`).
static inline BattleUnit *UnitAt(BattleUnit *units, u32 index, UnitSize size) {
    return (BattleUnit *) ((u8 *) units + index * size);
}

u8 BattleAbility::UpdateMoveToUnit() {
    // Declared here, in this order, for the original's register assignment.
    u8 caster;
    BattleUnit *units;
    u8 done = 0;
    BattleMarker *marker;
    UnitSize sz;
    BattlePlayerList *list;
    BattleUnit *unit;
    switch (mStep) {
        case 0: {
            VecFx32 pos = sEffectPos_02265af8_0;
            pos.x       = mTarget->mBakugan->mPosition.x;
            pos.z       = mTarget->mBakugan->mPosition.z;
            LoadEffectGroup(mEffects, 0, "bkn_move");
            s16 handle = StartEffect(mEffects, 0, 0, 0, 1);
            SetEffectPosition(mEffects, handle, &pos);
            SoundManager::Get().StartSeqArc(0x5d);
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            mUnk_62_1                  = 0;
            mStep++;
            break;
        }
        case 1:
            if (mUnk_1a >= 0x50 && !mUnk_62_1) {
                mUnk_62_1               = 1;
                caster    = mUnit;
                sz        = UNIT_SIZE;
                list      = data_020d433c.mBattlePlayers;
                units     = list->mUnits;
                unit      = UnitAt(units, mDestUnit, sz);
                marker    = &list->Markers()[unit->mMarker];
                GetCellPosition(&mDestPosition, marker->mCellX, marker->mCellY);
                SetTransformPosition(mTarget->mBakugan, mDestPosition.x, 0x899a, mDestPosition.z);
                VecFx32 a = mTarget->mBakugan->mPosition;
                VecFx32 b = *(VecFx32 *) unit;
                mTarget->MoveClearOfUnit(marker->mCellX, marker->mCellY, &a, &b);
                mTarget->mBakugan->mPosition = a;
                mTarget->mResult       = 3;
                mTarget->mBakugan->mVisible = 0;
                // Both indices are read as their own statements (volatile placeholders), the list before them.
                u8 ri;
                BattlePlayer *pl     = data_020d433c.mBattlePlayers->mPlayers[*(volatile u8 *) &mPlayer];
                ri                   = *(volatile u8 *) &mRecord;
                BattleRecord *record = &pl->mRecords[ri];
                u16 r4               = record->mUnk_04;
                u16 r6               = record->mUnk_06;
                u8 r9                = record->mAttribute;
                BattleUnk022637acUnit tmp;
                MI_CpuCopy8((u8 *) units + caster * sz, &tmp, sizeof(tmp));
                tmp.mPos.x = mTarget->mBakugan->mPosition.x;
                tmp.mPos.z = mTarget->mBakugan->mPosition.z;
                RemoveUnit(data_020d433c.mBattlePlayers, mUnit, 2);
                AddUnit(data_020d433c.mBattlePlayers, &tmp.mPos, tmp.mUnk_0c, tmp.mUnk_0f, tmp.mUnk_0e, unit->mMarker, 1);
                u8 *rec2 = (u8 *) data_020d433c.mBattlePlayers->mPlayers[mPlayer] + mRecord * 12;
                *(u16 *) (rec2 + 0x10) = r4;
                *(u16 *) (rec2 + 0x12) = r6;
                rec2[0x15]             = r9;
                SetUnitAB(data_020d433c.mBattlePlayers, mDestUnit, mUnit);
            }
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                mStep++;
            }
            break;
        case 2:
            if (UpdateCameraShiftTo(this, &mDestPosition)) {
                s16 handle  = StartEffect(mEffects, 0, 0, 0, 1);
                VecFx32 pos = sEffectPos_02265af8_1;
                pos.x       = mTarget->mBakugan->mPosition.x;
                pos.z       = mTarget->mBakugan->mPosition.z;
                SetEffectPosition(mEffects, handle, &pos);
                SoundManager::Get().StartSeqArc(0x5d);
                mUnk_62_1 = 0;
                mStep++;
            }
            break;
        case 3:
            if (mUnk_1a >= 0x50 && !mUnk_62_1) {
                mUnk_62_1                  = 1;
                mTarget->mBakugan->mVisible = 1;
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


u8 BattleAbility::UpdateMoveToMarker() {
    u8 done = 0;
    switch (mStep) {
        case 0: {
            VecFx32 pos = sEffectPos_02265fac_0;
            pos.x       = mTarget->mBakugan->mPosition.x;
            pos.z       = mTarget->mBakugan->mPosition.z;
            LoadEffectGroup(mEffects, 0, "bkn_move");
            s16 handle = StartEffect(mEffects, 0, 0, 0, 1);
            SetEffectPosition(mEffects, handle, &pos);
            SoundManager::Get().StartSeqArc(0x5d);
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            mUnk_62_1                  = 0;
            mStep++;
            break;
        }
        case 1:
            if (mUnk_1a >= 0x50 && !mUnk_62_1) {
                mUnk_62_1            = 1;
                BattleMarker *marker = &data_020d433c.mBattlePlayers->Markers()[mDestMarker];
                GetCellPosition(&mDestPosition, marker->mCellX, marker->mCellY);
                SetTransformPosition(mTarget->mBakugan, mDestPosition.x, 0x899a, mDestPosition.z);
                u8 idx = mDestUnit;
                if (idx == 0xff) {
                    mTarget->mResult = 2;
                } else {
                    VecFx32 a = mTarget->mBakugan->mPosition;
                    VecFx32 b = *(VecFx32 *) &data_020d433c.mBattlePlayers->mUnits[idx];
                    mTarget->MoveClearOfUnit(marker->mCellX, marker->mCellY, &a, &b);
                    mTarget->mBakugan->mPosition = a;
                    SetUnitAB(data_020d433c.mBattlePlayers, mDestUnit, mUnit);
                    mTarget->mResult = 3;
                }
                mTarget->mBakugan->mVisible = 0;
                VecFx32 c;
                c.x = mTarget->mBakugan->mPosition.x;
                c.y = mTarget->mBakugan->mPosition.y;
                c.z = mTarget->mBakugan->mPosition.z;
                MoveUnit(data_020d433c.mBattlePlayers, mUnit, &c, mDestMarker);
            }
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                mStep++;
            }
            break;
        case 2:
            if (UpdateCameraShiftTo(this, &mDestPosition)) {
                s16 handle  = StartEffect(mEffects, 0, 0, 0, 1);
                VecFx32 pos = sEffectPos_02265fac_1;
                pos.x       = mTarget->mBakugan->mPosition.x;
                pos.z       = mTarget->mBakugan->mPosition.z;
                SetEffectPosition(mEffects, handle, &pos);
                SoundManager::Get().StartSeqArc(0x5d);
                mUnk_62_1 = 0;
                mStep++;
            }
            break;
        case 3:
            if (mUnk_1a >= 0x50 && !mUnk_62_1) {
                mUnk_62_1                  = 1;
                mTarget->mBakugan->mVisible = 1;
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

u8 BattleAbility::UpdateDefenseUp() {
    u8 done = 0;
    switch (mStep) {
        case 0: {
            VecFx32 pos = sEffectPos_022663f0;
            pos.x       = mTarget->mBakugan->mPosition.x;
            pos.z       = mTarget->mBakugan->mPosition.z;
            LoadEffectGroup(mEffects, 0, "defense_up");
            s16 handle = StartEffect(mEffects, 0, 0, 0, 1);
            SetEffectPosition(mEffects, handle, &pos);
            SoundManager::Get().StartSeqArc(0x5e);
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            mStep++;
            break;
        }
        case 1:
            OrbitCamera(this);
            if (!mStIconSet && mUnk_1a > 0x32) {
                mStIconSet = 1;
                data_020d433c.mBattlePlayers->mUnk_29c->SetStIconUnit(mUnit, 0);
            }
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                data_020d433c.mBattlePlayers->mUnits[mUnit].mStatus = 3;
                done                                                   = 1;
            }
            break;
    }
    return done;
}

BOOL UpdateRecordAmount(BattleAbility *self, u8 a, u8 b);

u8 BattleAbility::func_ov007_022665ec() {
    u8 done = 0;
    switch (mStep) {
        case 0:
            LoadEffectGroup(mEffects, 0, "gp_pickup2");
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            mStep++;
            break;
        case 1: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            VecFx32 pos = sEffectPos_022665ec;
            pos.x       = mTarget->mBakugan->mPosition.x;
            pos.z       = mTarget->mBakugan->mPosition.z;
            s16 handle  = StartEffect(mEffects, 0, 0, 0, 1);
            SetEffectPosition(mEffects, handle, &pos);
            SoundManager::Get().StartSeqArc(0x5f);
            mSubtract = 0;
            mStep++;
            break;
        }
        case 2:
            if (UpdateRecordAmount(this, mPlayer, mRecord)) {
                mStep++;
            }
            break;
        case 3:
            UpdateRecordAmount(this, mPlayer, mRecord);
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                done    = 1;
            }
            break;
    }
    mTarget->DrawRecordTotal(0xff, 0xff);
    OrbitCamera(this);
    return done;
}

u8 BattleAbility::func_ov007_02266868() {
    u8 done = 0;
    switch (mStep) {
        case 0: {
            VecFx32 pos = sEffectPos_02266868;
            pos.x       = mTarget->mBakugan->mPosition.x;
            pos.z       = mTarget->mBakugan->mPosition.z;
            LoadEffectGroup(mEffects, 0, "gp_pickup2");
            s16 handle = StartEffect(mEffects, 0, 0, 0, 1);
            SetEffectPosition(mEffects, handle, &pos);
            SoundManager::Get().StartSeqArc(0x5f);
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            mStep++;
            break;
        }
        case 1:
            OrbitCamera(this);
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                done    = 1;
                data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mUnk_f3 = 0xbe;
            }
            break;
    }
    return done;
}

// Effect placement of a field (0x24 bytes): position, camera position and a scale. sFieldEffects[mFieldId] is read
// by BattleAbility::func_ov007_02266a28 (case 4) and UpdateCameraToFieldEffect.
struct BattleAbilityFieldEffect {
    /* 00 */ VecFx32 mPos;
    /* 0c */ VecFx32 mCamera;
    /* 18 */ VecFx32 mScale;
    /* 24 */
};

static const BattleAbilityFieldEffect sFieldEffects[8] = {
    {{0, 0x5000, 0}, {0, 0x140000, 0xf0000}, {0x9000, 0xa000, 0x9000}},
    {{0, 0x5000, 0}, {0, 0x15e000, 0x15e000}, {0xc000, 0xa000, 0xc000}},
    {{0, 0x5000, 0}, {0, 0x1f4000, 0x1f4000}, {0x11800, 0xa000, 0x11800}},
    {{0, 0x5000, 0}, {0, 0x118000, 0x118000}, {0x9800, 0xa000, 0x9800}},
    {{0, 0x5000, 0}, {0, 0x1c2000, 0x1c2000}, {0xf800, 0xa000, 0xf800}},
    {{0, 0x5000, 0}, {0, 0x1f4000, 0x1f4000}, {0x11000, 0xa000, 0x11000}},
    {{0, 0x5000, 0}, {0, 0x1f4000, 0x1f4000}, {0x12000, 0xa000, 0x12000}},
    {{0, 0x5000, 0}, {0, 0x208000, 0x208000}, {0x12000, 0xa000, 0x12000}},
};

void SetEffectScale(EffectManager *manager, u16 handle, VecFx32 *scale);
BOOL UpdateCameraToFieldEffect(BattleAbility *self);

u8 BattleAbility::func_ov007_02266a28() {
    u8 done = 0;
    switch (mStep) {
        case 0:
            LoadEffectGroup(mEffects, 0, mAbilityId == 0xd4 ? "gp_pickup2" : "gp_down");
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_TO_ZERO;
            OrbitCamera(this);
            mStep++;
            break;
        case 1: {
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            VecFx32 pos = sEffectPos_02266a28_0;
            pos.x       = mTarget->mBakugan->mPosition.x;
            pos.z       = mTarget->mBakugan->mPosition.z;
            s16 handle  = StartEffect(mEffects, 0, 0, 0, 1);
            SetEffectPosition(mEffects, handle, &pos);
            u32 sfx = mAbilityId == 0xd4 ? 0x5f : 0x6c;
            SoundManager::Get().StartSeqArc(sfx);
            if (mAbilityId == 0xd3) {
                mSubtract = 1;
            } else {
                mSubtract = 0;
            }
            OrbitCamera(this);
            mStep++;
            break;
        }
        case 2:
            mTarget->DrawRecordTotal(0xff, 0xff);
            OrbitCamera(this);
            if (UpdateRecordAmount(this, mPlayer, mRecord)) {
                mStep++;
            }
            break;
        case 3:
            UpdateRecordAmount(this, mPlayer, mRecord);
            mTarget->DrawRecordTotal(0xff, 0xff);
            OrbitCamera(this);
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                mStep++;
            }
            break;
        case 4:
            if (UpdateCameraToFieldEffect(this)) {
                mUnk_1a = 0;
                const BattleAbilityFieldEffect *entry = &sFieldEffects[data_020d433c.mFieldId];
                VecFx32 pos                          = sEffectPos_02266a28_1;
                pos.x                                = entry->mPos.x;
                pos.z                                = entry->mPos.z;
                u16 handle                           = (u16) (s16) StartEffect(mEffects, 0, 0, 0, 1);
                SetEffectPosition(mEffects, handle, &pos);
                SetEffectScale(mEffects, handle, (VecFx32 *) &entry->mScale);
                u32 sfx = mAbilityId == 0xd4 ? 0x5f : 0x6c;
                SoundManager::Get().StartSeqArc(sfx);
                mStep++;
            }
            break;
        case 5:
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                u8 i    = 0;
                do {
                    BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[i];
                    if (unit->mStatus != 0) {
                        if (!(unit->mPlayer == mPlayer && unit->mRecord == mRecord)) {
                            mOtherUnits[mOtherUnitCount++] = i;
                        }
                    }
                    i++;
                } while (i < 12);
                mStep++;
            }
            break;
        case 6: {
            if (mOtherUnitCount == 0) {
                done = 1;
                break;
            }
            mOtherUnitCount--;
            BattleUnit *units = data_020d433c.mBattlePlayers->Units();
            VecFx32 *p        = (VecFx32 *) &units[mOtherUnits[mOtherUnitCount]];
            fx32 x, y, z;
            z = p->z;
            y = p->y;
            x = p->x;
            mCamera->SetTarget(x, y, z);
            OrbitCamera(this);
            s16 handle  = StartEffect(mEffects, 0, 0, 0, 1);
            VecFx32 pos = sEffectPos_02266a28_2;
            pos.x       = x;
            pos.z       = z;
            SetEffectPosition(mEffects, handle, &pos);
            u32 sfx = mAbilityId == 0xd4 ? 0x5f : 0x6c;
            SoundManager::Get().StartSeqArc(sfx);
            mAmountStep = 0;
            mStep++;
            break;
        }
        case 7: {
            BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[mOtherUnits[mOtherUnitCount]];
            mTarget->DrawRecordTotal(unit->mPlayer, unit->mRecord);
            OrbitCamera(this);
            if (UpdateRecordAmount(this, unit->mPlayer, unit->mRecord)) {
                mStep++;
            }
            break;
        }
        case 8: {
            BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[mOtherUnits[mOtherUnitCount]];
            mTarget->DrawRecordTotal(unit->mPlayer, unit->mRecord);
            OrbitCamera(this);
            UpdateRecordAmount(this, unit->mPlayer, unit->mRecord);
            mUnk_1a++;
            if (mUnk_1a > 0xa0) {
                mUnk_1a = 0;
                mStep = 6;
            }
            break;
        }
    }
    return done;
}

void func_ov007_0226a380(BattlePlayer *player, u8 record, int unk);

// Camera position on a circle (65 times the sine table) around the target in x/z, y set to 0xc000; mUnk_3a += 100.
void OrbitCamera(BattleAbility *self) {
    u16 angle   = self->mOrbitAngle;
    Camera *cam = self->mCamera;
    int idx     = angle >> 4;
    s32 sinV    = FX_SinCosTable_[idx * 2 + 1];
    s32 cosV    = FX_SinCosTable_[idx * 2];
    fx32 x      = cam->mTarget.x + sinV * 65;
    fx32 z      = cam->mTarget.z + cosV * 65;
    self->mOrbitAngle = angle + 100;
    cam         = self->mCamera;
    cam->mPosition.x = x;
    cam->mPosition.y = 0xc000;
    cam->mPosition.z = z;
}

BOOL UpdateCameraToFieldEffect(BattleAbility *self) {
    BOOL done = 0;
    VecFx32 tmp;
    VecFx32 d1;
    VecFx32 d2;
    Camera *cam;
    const BattleAbilityFieldEffect *entry = &sFieldEffects[data_020d433c.mFieldId];
    switch (self->mCameraStep) {
        case 0:
            cam   = self->mCamera;
            tmp.x = cam->mPosition.x;
            tmp.y = cam->mPosition.y;
            tmp.z = cam->mPosition.z;
            VEC_Subtract(&entry->mCamera, &tmp, &d1);
            cam   = self->mCamera;
            tmp.x = cam->mTarget.x;
            tmp.y = cam->mTarget.y;
            tmp.z = cam->mTarget.z;
            VEC_Subtract(&entry->mPos, &tmp, &d2);
            self->mCameraPositionPerFrame.x = d1.x / 70;
            self->mCameraPositionPerFrame.y = d1.y / 70;
            self->mCameraPositionPerFrame.z = d1.z / 70;
            self->mCameraTargetPerFrame.x = d2.x / 70;
            self->mCameraTargetPerFrame.y = d2.y / 70;
            self->mCameraTargetPerFrame.z = d2.z / 70;
            self->mCameraStep++;
        case 1:
            cam  = self->mCamera;
            d1.x = cam->mPosition.x;
            d1.y = cam->mPosition.y;
            d1.z = cam->mPosition.z;
            cam  = self->mCamera;
            d2.x = cam->mTarget.x;
            d2.y = cam->mTarget.y;
            d2.z = cam->mTarget.z;
            VEC_Add(&d1, &self->mCameraPositionPerFrame, &d1);
            VEC_Add(&d2, &self->mCameraTargetPerFrame, &d2);
            cam                = self->mCamera;
            cam->mPosition.x   = d1.x;
            cam->mPosition.y   = d1.y;
            cam->mPosition.z   = d1.z;
            cam                = self->mCamera;
            cam->mTarget.x     = d2.x;
            cam->mTarget.y     = d2.y;
            cam->mTarget.z     = d2.z;
            self->mUnk_1a++;
            if (self->mUnk_1a > 0x46) {
                self->mUnk_1a = 0;
                self->mCameraStep = 0;
                done          = 1;
            }
            break;
    }
    return done;
}

// Step 0 only sets up: *out becomes its offset from the camera target, mUnk_3c 10 times its direction in x/z (y 0),
// mUnk_1a the step count; nothing moves. Later calls move position and target by mUnk_3c; TRUE once mUnk_1a is 0.
BOOL UpdateCameraShiftTo(BattleAbility *self, VecFx32 *out) {
    BOOL done = 0;
    VecFx32 tmp;
    VecFx32 d1;
    VecFx32 d2;
    Camera *cam;
    switch (self->mCameraStep) {
        case 0:
            cam   = self->mCamera;
            tmp.x = cam->mTarget.x;
            tmp.y = cam->mTarget.y;
            tmp.z = cam->mTarget.z;
            VEC_Subtract(out, &tmp, out);
            VEC_Normalize(out, &self->mCameraPositionPerFrame);
            self->mCameraPositionPerFrame.x *= 10;
            self->mCameraPositionPerFrame.z *= 10;
            self->mCameraPositionPerFrame.y = done;
            self->mUnk_1a   = out->x / self->mCameraPositionPerFrame.x;
            self->mCameraStep++;
            break;
        case 1:
            cam  = self->mCamera;
            d1.x = cam->mPosition.x;
            d1.y = cam->mPosition.y;
            d1.z = cam->mPosition.z;
            cam  = self->mCamera;
            d2.x = cam->mTarget.x;
            d2.y = cam->mTarget.y;
            d2.z = cam->mTarget.z;
            VEC_Add(&d1, &self->mCameraPositionPerFrame, &d1);
            VEC_Add(&d2, &self->mCameraPositionPerFrame, &d2);
            cam              = self->mCamera;
            cam->mPosition.x = d1.x;
            cam->mPosition.y = d1.y;
            cam->mPosition.z = d1.z;
            cam              = self->mCamera;
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

BOOL UpdateRecordAmount(BattleAbility *self, u8 player, u8 slot) {
    BOOL done = 0;
    switch (self->mAmountStep) {
        case 0: {
            u16 value      = data_020d433c.mBattlePlayers->mPlayers[player]->mRecords[slot].mUnk_06;
            self->mAmountRemaining = value;
            self->mShownAmount = value;
            self->mAmountYPerFrame = 0x4000;
            self->mAmountY = 0x60000;
            if (self->mSubtract) {
                u8 i = 0;
                do {
                    self->mNumberCellAnims[i]->mPaletteBase = 13;
                    i++;
                } while (i < 2);
            } else {
                u16 sum = data_020d433c.mBattlePlayers->mPlayers[player]->mRecords[slot].mUnk_04;
                sum += self->mAmountRemaining;
                if (self->mAmountRemaining + sum > 0x3de) {
                    self->mAmountRemaining = 0x3de - sum;
                    self->mShownAmount = self->mAmountRemaining;
                }
            }
            self->mAmountDigitCount = 4;
            if (self->mAmountRemaining < 1000) {
                self->mAmountDigitCount--;
                if (self->mAmountRemaining < 100) {
                    self->mAmountDigitCount--;
                    if ((int) self->mAmountRemaining < 10) {
                        self->mAmountDigitCount--;
                    }
                }
            }
            self->mAmountStep++;
            break;
        }
        case 1:
            self->mAmountYPerFrame -= 0x800;
            self->mAmountY -= self->mAmountYPerFrame;
            if (self->mAmountYPerFrame < -0x4000) {
                if (self->mAmountRemaining == 0) {
                    self->mAmountStep = 3;
                } else {
                    self->mAmountStep++;
                }
            }
            break;
        case 2:
            self->mAmountFrames++;
            if (self->mAmountFrames > 10) {
                self->mAmountFrames = 0;
                if (self->mAmountRemaining == 0) {
                    self->mAmountStep++;
                } else {
                    s16 amount;
                    if ((int) self->mAmountRemaining >= 30) {
                        self->mAmountRemaining -= 30;
                        amount = 30;
                    } else {
                        self->mAmountRemaining = 0;
                        amount        = 30 - self->mAmountRemaining;
                    }
                    BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[player];
                    if (!self->mSubtract) {
                        func_ov007_0226a380(p, slot, amount);
                    } else {
                        func_ov007_0226a380(p, slot, (s16) -amount);
                    }
                }
                SoundManager::Get().StartSeqArc(8);
            }
            break;
        case 3:
            done = 1;
            break;
    }
    u8 i      = 0;
    s16 x     = 0x50;
    s16 y     = (s16) (int) ((float) self->mAmountY / 4096.0f);
    x += 0x30;
    u32 value = self->mShownAmount;
    do {
        self->mNumberCellAnims[0]->DrawFrame(0, (u8) (u16) ((int) value % 10), x, y, 0);
        x -= 12;
        value /= 10;
        i++;
    } while (i < self->mAmountDigitCount);
    self->mNumberCellAnims[1]->DrawFrame(0, self->mSubtract, x, y, 0);
    return done;
}

BOOL func_ov007_022677cc(BattleAbility *self) {
    BOOL r = 1;
    u8 i                   = 0;
    u16 a                  = 0;
    u16 b                  = 0;
    u16 c                  = 0;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    do {
        BattleUnit *unit = &list->mUnits[i];
        if (unit->mStatus) {
            UnitIndex e       = (UnitIndex) unit->mRecord;
            BattleRecord *rec = &list->mPlayers[unit->mPlayer]->mRecords[e];
            c += 0x3de;
            a += rec->mUnk_04;
            b += rec->mUnk_06;
            a += rec->mUnk_06;
        }
        i++;
    } while (i < 12);
    if (a >= c || b == 0) {
        r = 0;
    }
    return r;
}

enum UnitCompare { UNIT_COMPARE_0, UNIT_COMPARE_1, UNIT_COMPARE_2 };
u32 func_ov007_02267884(BattleAbility *self) {
    if (self->mTarget->mResult != 3) {
        return 0;
    }
    BattleUnit *units    = data_020d433c.mBattlePlayers->mUnits;
    u8 i                 = 0;
    BattleUnit *selfUnit = &units[self->mUnit];
    UnitCompare result   = UNIT_COMPARE_0;
    do {
        if (self->mUnit != i) {
            BattleUnit *unit = &units[i];
            if (selfUnit->mMarker == unit->mMarker) {
                u16 a = data_020d433c.mBattlePlayers->mPlayers[selfUnit->mPlayer]->mRecords[selfUnit->mRecord].mUnk_06;
                u16 b = data_020d433c.mBattlePlayers->mPlayers[unit->mPlayer]->mRecords[unit->mRecord].mUnk_06;
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

BOOL func_ov007_02267944(BattleAbility *self, u16 *out) {
    u8 i = 0;
    do {
        BattlePlayerList *list = data_020d433c.mBattlePlayers;
        BattleUnit *unit       = &list->mUnits[i];
        if (unit->mStatus) {
            u8 pidx = unit->mPlayer;
            u16 v   = list->mPlayers[pidx]->mRecords[unit->mRecord].mUnk_06;
            if (pidx == self->mPlayer) {
                out[0] += v;
            } else if (data_020d433c.mHasPartners && pidx == list->mPlayers[self->mPlayer]->mPartner) {
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
