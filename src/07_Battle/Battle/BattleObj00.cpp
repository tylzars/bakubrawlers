#include "System/InputManager.hpp"
#include "System/WirelessManager.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/TouchManager.hpp"
#include "System/SoundManager.hpp"
#include "Battle/BattleCommand.hpp"
#include "Battle/BattleCharaAct.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Battle/BattleCommandPrompt.hpp"
#include "Battle/BattleUnionAbility.hpp"
#include "Battle/GimmickTypes.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Scene/SceneBase.hpp"
#include "System/FileSystem.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include "UI/StorytalkWindow.hpp"
#include <nitro/gx.h>

struct BattleObjPair {
    u8 mCellX;
    u8 mCellY;
};

BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);
BOOL func_ov007_022632d0(BattlePlayerList *list, u32 flag);
void AddMarker(BattlePlayerList *list, u8 x, u8 y, u8 player, u8 slot);
u8 PickUnflaggedSlot54(BattlePlayer *player);
void InitCommandSubScreen(int x, int y);
void CloseCommandMap(BattleCommandMap *map);
extern "C" void func_ov007_022482a8(BattleCommandMap *map);
extern "C" void func_ov007_02247e00(BattleCommandMap *map);
extern "C" void SetCursorAndMarkerModelsVisible(BattleCommandMap *map, u32 a);

extern const BattleObjPair data_ov007_0227a33c[];
extern const u8 data_ov007_0227a33d[];

class BattleObj00 : public SceneBase {
public:
    /* 0c */ CellAnimation *mCellAnims[3];
    /* 18 */ BattleCommandPage *mPage;
    /* 1c */ Camera *mCamera;
    /* 20 */ BattleCommandMap *mMap;
    /* 24 */ BattleUnionAbility *mUnion;
    /* 28 */ u16 mUnk_28;
    /* 2a */ s16 mUnk_2a;
    /* 2c */ s16 mUnk_2c;
    /* 2e */ u16 mUnk_2e;
    /* 30 */ u16 mUnk_30;
    /* 32 */ u8 mState;
    /* 33 */ u8 mUnk_33;
    /* 34 */ u8 mUnk_34;
    /* 35 */ u8 mUnk_35;
    /* 36 */ u8 mUnk_36;
    /* 37 */ u8 mBrightness;
    /* 38 */ PAD(0x38, 0x39);
    /* 39 */ u8 mSlot;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ PAD(0x3b, 0x3c);
    /* 3c */ u8 mFlipped;
    /* 3d */ PAD(0x3d, 0x3e);
    /* 3e */ u8 mUnk_3e;
    /* 3f */ u8 mOffsetX;
    /* 40 */

    BattleObj00(u8 flag);
    virtual ~BattleObj00();
    virtual void Update();
    void LoadGraphics();
};

BattleObj00::BattleObj00(u8 flag)
    : SceneBase(0x3e8, 0) {
    mUnk_28 = 0;
    mUnk_2a = 0;
    mUnk_2c = 0;
    mUnk_33 = 0;
    mUnk_34 = 0;
    mUnk_35 = 0;
    mUnk_36 = 0;
    mUnk_3e = 0;
    if (gSaveData[0].mUnk_28_5 != 0) {
        mOffsetX = 0;
    } else {
        mOffsetX = 4;
    }
    mCamera = data_020d433c.mBattlePlayers->mCamera;
    mCellAnims[2] = 0;
    mCellAnims[1] = 0;
    mCellAnims[0] = 0;
    mUnion = 0;
    mUnk_2e = 0;
    mUnk_30 = 0;
    mUnk_3a = data_020d433c.mBattlePlayers->mTurnPlayer;
    if (flag == 0) {
        mState = 0;
    } else {
        mState = 11;
    }
    u32 v = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9]->mUnk_f1;
    if (v <= 1) {
        mFlipped = 0;
    } else {
        mFlipped = 1;
    }
    mUnk_34 = data_ov007_0227a33c[v].mCellX;
    u8 b    = data_ov007_0227a33d[v * 2];
    mUnk_35 = b;
    BattlePlayerList *l = data_020d433c.mBattlePlayers;
    l->mUnk_28b = mUnk_34;
    l->mUnk_28c = b;
}

BattleObj00::~BattleObj00() {
    CellAnimation *anim = mCellAnims[2];
    if (anim) {
        delete anim;
    }
    mCellAnims[2] = 0;
    anim = mCellAnims[1];
    if (anim) {
        delete anim;
    }
    mCellAnims[1] = 0;
    anim = mCellAnims[0];
    if (anim) {
        delete anim;
    }
    mCellAnims[0] = 0;
    SpriteManager::Get().ResetSlots();
}

void BattleObj00::Update() {
    switch (mState) {
        case 0: {
            if (!SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0x10)) {
                return;
            }
            u8 player = data_020d433c.mBattlePlayers->mUnk_2a9;
            new BattleCharaAct(mCamera, player, 0, 0, 0);
            mContext->mRequest = 0;
            mState           = 1;
            return;
        }
        case 1:
            mUnk_28 = 0;
            mState++;
            return;
        case 2:
            if (mContext->mRequest != 0) {
                mState++;
            }
            return;
        case 3:
            if (!SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                return;
            }
            SpriteManager::Get().SetupMode(1, 7);
            mMap = new BattleCommandMap(data_020d433c.mFieldId);
            mMap->Init();
            data_020d433c.mGimmicks->mState = 1;
            func_ov007_022482a8(mMap);
            mState++;
            return;
        case 4:
            if (data_020d433c.mUnk_10) {
                data_020d433c.mUnk_10->SetupLayout(0xd);
            }
            SpriteManager::Get().StartFade(2);
            mState++;
            return;
        case 5: {
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            Camera *camera         = list->mCamera;
            if (((camera->mFrameIndex + 1 >= camera->mFrameCount) & 0xffff) != 0) {
                mState = 7;
                return;
            }
            if (func_ov007_022632d0(list, 0)) {
                mState = 7;
                return;
            }
            if (!InputManager::Get().IsPressed(8)) {
                return;
            }
            mState++;
            return;
        }
        case 6:
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                if (!func_ov007_022632d0(data_020d433c.mBattlePlayers, 1)) {
                    return;
                }
                mState++;
            } else {
                mState++;
            }
            return;
        case 7:
            CloseCommandMap(mMap);
            mContext->mRequest = 0;
            mState++;
            return;
        case 8:
            if (mContext->mRequest != 0) {
                mState++;
            }
            return;
        case 9: {
            u8 player = data_020d433c.mBattlePlayers->mUnk_2a9;
            new BattleCharaAct(mCamera, player, 0xe, 0, 0);
            mContext->mRequest = 0;
            mState++;
            return;
        }
        case 10:
            if (mContext->mRequest == 0) {
                return;
            }
            if (!SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                return;
            }
            InitCommandSubScreen((s16) (mUnk_2a - mOffsetX), mUnk_2c);
            func_ov007_0223ad70(mFlipped);
            if (data_020d433c.mUnk_9a_3) {
                mContext->mRequest = 1;
                mState           = 0x10;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                if (list->mPlayers[list->mUnk_2a9]->mSlots54Remaining == 0) {
                    mUnk_3e = 1;
                }
                return;
            }
            SpriteManager::Get().StartSubFade(2);
            mState = 0xc;
            return;
        case 11:
            InitCommandSubScreen((s16) (mUnk_2a - mOffsetX), mUnk_2c);
            func_ov007_0223ad70(mFlipped);
            LoadGraphics();
            if (data_020d433c.mUnk_9a_3) {
                mContext->mRequest = 1;
                mState           = 0x10;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                if (list->mPlayers[list->mUnk_2a9]->mSlots54Remaining == 0) {
                    mUnk_3e = 1;
                }
                return;
            }
            SpriteManager::Get().StartSubFade(2);
            mState = 0xc;
            return;
        case 12:
            if (data_020d433c.mUnk_8a_0 != 0) {
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                if ((u8) (list->mPlayers[0]->mUnk_ee + list->mPlayers[1]->mUnk_ee) % 2 == 0) {
                    mPage = new BattleCommandPage(0, -1);
                    func_0205562c(mPage, 1);
                    func_02055738(mPage, 0x18000, 0xe, 0);
                    new BattleCommandPrompt(0, mPage, 0, 0, 0);
                    mContext->mRequest = 0;
                } else {
                    mPage = new BattleCommandPage(0, -1);
                    func_0205562c(mPage, 1);
                    func_02055738(mPage, 0x18000, 0xe, 0);
                    mUnk_3e           = 1;
                    mContext->mRequest = 1;
                }
                mState++;
                return;
            } else {
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                if (list->mPlayers[list->mUnk_2a9]->mSlots54Remaining == 0) {
                    mPage = new BattleCommandPage(0, -1);
                    func_0205562c(mPage, 1);
                    func_02055738(mPage, 0x18000, 0xe, 0);
                    mUnk_3e           = 1;
                    mContext->mRequest = 1;
                } else {
                    mPage = new BattleCommandPage(0, -1);
                    func_0205562c(mPage, 1);
                    func_02055738(mPage, 0x18000, 0xe, 0);
                    if (data_020d433c.mUnk_9a_3) {
                        mContext->mRequest = 1;
                    } else {
                        new BattleCommandPrompt(0, mPage, 0, 0, 0);
                        mContext->mRequest = 0;
                    }
                }
            }
            mState++;
            return;
        case 13: {
            u32 v = (u16) mContext->mRequest;
            if (v == 0) {
                return;
            }
            mSlot = GetLowByte(v);
            mState++;
            WirelessManager::Get();
            if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                return;
            }
            mUnion = new BattleUnionAbility(4, 0x1a000, 0xc);
            return;
        }
        case 14:
            if (!SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                return;
            }
            SpriteManager::Get().StartFade(3);
            if (mUnion) {
                mUnion->Close(0);
            }
            mState++;
            return;
        case 15:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            CloseCommandPage(mPage, 1);
            mContext->mRequest = 0;
            mState           = 0x10;
            return;
        case 16: {
            if (mContext->mRequest == 0) {
                return;
            }
            if (mUnk_3e != 0) {
                u8 idx = 0;
                if (data_020d433c.mUnk_8a_0 != 0) {
                    idx = 1;
                } else {
                    u8 i = 0;
                    do {
                        if (data_020d433c.mBattlePlayers->mPlayers[i]->mSlots54Remaining != 0) {
                            idx = i;
                            break;
                        }
                        i++;
                    } while (i < data_020d433c.mPlayerCount);
                }
                new BattleCharaAct(mCamera, idx, 1, 0, 0);
                mContext->mRequest = 0;
            } else {
                u8 player = data_020d433c.mBattlePlayers->mUnk_2a9;
                new BattleCharaAct(mCamera, player, 1, 0, 0);
                mContext->mRequest = 0;
            }
            mState = 0x11;
            return;
        }
        case 17:
            if (mContext->mRequest == 0) {
                return;
            }
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                data_0220f540.mUnk_04 = mSlot;
                if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 7)) {
                    mState = 0x12;
                }
            } else {
                data_0220f540.mUnk_04 = mSlot;
                mState               = 0x12;
            }
            return;
        case 18:
            InitCommandSubScreen((s16) (mUnk_2a - mOffsetX), mUnk_2c);
            func_ov007_0223ad70(mFlipped);
            mBrightness = 0;
            mUnk_28 = 0;
            GXS_SetVisibleWnd(4);
            G2S_SetWndOBJInsidePlane(0x21, FALSE);
            G2S_SetWndOutsidePlane(0x11, FALSE);
            mMap = new BattleCommandMap(data_020d433c.mFieldId);
            mMap->mUnk_151 = 0;
            mMap->Init();
            func_ov007_02247e00(mMap);
            data_020d433c.mGimmicks->mState = 0;
            mState                         = 0x13;
            SpriteManager::Get().StartFade(2);
            return;
        case 19: {
            if (SpriteManager::Get().sFade[0] > 1) {
                return;
            }
            mUnk_28 = mUnk_28 + 1;
            if (mUnk_28 % 4 == 0 && mBrightness < 0x10) {
                mBrightness = mBrightness + 1;
                G2S_SetBlendBrightness(0x11, mBrightness);
            }
            if (mUnk_28 == 1) {
                SoundManager::Get().StartSeqArc(6);
            }
            if ((s32) mUnk_28 <= 150) {
                return;
            }
            mUnk_36 = 0;
            G2S_SetBlendBrightness(0x11, 0);
            GXS_SetVisibleWnd(0);
            if (data_020d433c.mUnk_8a_0 != 0) {
                if (mUnk_3e != 0) {
                    AddMarker(data_020d433c.mBattlePlayers, 6, 4, 1, PickUnflaggedSlot54(data_020d433c.mBattlePlayers->mPlayers[1]));
                } else {
                    AddMarker(data_020d433c.mBattlePlayers, 6, 4, 0, mSlot);
                }
            } else {
                u8 i = 0; 
                BattlePlayer *p; 
                u8 kind; 
                do {
                    p = data_020d433c.mBattlePlayers->mPlayers[i];
                    kind         = p->mUnk_f1;
                    if (p->mSlots54Remaining != 0) {
                        if (p->IsCpu() || data_020d433c.mUnk_9a_3) {
                            AddMarker(data_020d433c.mBattlePlayers, data_ov007_0227a33c[kind].mCellX, data_ov007_0227a33c[kind].mCellY, i, PickUnflaggedSlot54(p));
                        } else {
                            if (IsWirelessMode((const u8 *) &WirelessManager::Get().mState)) {
                                AddMarker(data_020d433c.mBattlePlayers, data_ov007_0227a33c[kind].mCellX, data_ov007_0227a33c[kind].mCellY, i, (u8) *(u32 *) ((u8 *) data_020d433c.mBattlePlayers->mPlayers[i] + 0x104));
                            } else {
                                AddMarker(data_020d433c.mBattlePlayers, data_ov007_0227a33c[kind].mCellX, data_ov007_0227a33c[kind].mCellY, i, mSlot);
                            }
                        }
                    }
                    i++;
                } while (i < data_020d433c.mPlayerCount);
            }
            if (data_020d433c.mTutorialId != 0) {
                mState = 0x14;
                data_020d433c.mBattlePlayers->mUnk_2a4->LoadGraphics();
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                list->mUnk_2a4->SetMessage(list->mUnk_2af, 0x64, 0, 0);
                G2S_SetBlendBrightness(0x1f, -10);
                SetCursorAndMarkerModelsVisible(mMap, 0);
                return;
            }
            CloseCommandMap(mMap);
            mContext->mRequest = 0;
            mState           = 0x16;
            return;
        }
        case 20:
            if (InputManager::Get().IsPressed(1) || TouchManager::Get().mReleased) {
                data_020d433c.mBattlePlayers->mUnk_2a4->NextPage(1);
            }
            if (data_020d433c.mBattlePlayers->mUnk_2a4->func_02087cac()) {
                return;
            }
            data_020d433c.mBattlePlayers->mUnk_2af++;
            if (data_020d433c.mBattlePlayers->mUnk_2af == 2) {
                mState++;
                G2S_SetBlendBrightness(0x1f, 0);
                return;
            }
            data_020d433c.mBattlePlayers->mUnk_2a4->SetMessage(data_020d433c.mBattlePlayers->mUnk_2af, 0x64, 0, 0);
            return;
        case 21:
            CloseCommandMap(mMap);
            mContext->mRequest = 0;
            mState           = 0x16;
            return;
        case 22:
            if (mContext->mRequest != 0) {
                mState++;
            }
            return;
        case 23:
            mState++;
            SpriteManager::Get().StartFade(3);
            return;
        case 24:
            if (!SpriteManager::Get().IsFading()) {
                mState = 0x19;
            }
            return;
        case 25:
            RequestScene(1);
            mFinished = 1;
            return;
    }
}

void BattleObj00::LoadGraphics() {
    ChangeDir("/Game/Battle/Obj");
    LoadAndQueueUpload(0, 4, 0, "battle_u.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 5, 0, "battle_u_ncl.bin", 0, 0, 0);
    ChangeDir("/");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Battle/Obj");
    mCellAnims[0] = new CellAnimation("battle_b_obj_00_nce.bin", 0x15, 1, 1);
    mCellAnims[0]->mCharBase = screen.GetCharBase(0x10000);
    mCellAnims[0]->mPaletteBase = 0;
    mCellAnims[1] = new CellAnimation("battle_b_obj_00_nce.bin", 0x16, 1, 1);
    mCellAnims[1]->mCharBase = screen.GetCharBase(0x10000);
    mCellAnims[1]->mPaletteBase = 0;
    mCellAnims[2] = new CellAnimation("battle_b_obj_00_nce.bin", 0x1b, 1, 1);
    mCellAnims[2]->mCharBase = screen.GetCharBase(0x10000);
    mCellAnims[2]->mPaletteBase = 0;
    ChangeDir("/Game/2D/union_b");
}
