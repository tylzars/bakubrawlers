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
    u8 mRow;
    u8 mCol;
};

BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);
BOOL func_ov007_022632d0(BattlePlayerList *list, u32 flag);
void func_ov007_02262638(BattlePlayerList *list, u8 row, u8 col, u8 player, u8 slot);
u8 func_ov007_0226a5e8(BattlePlayer *player);
void func_ov007_0223a690(int x, int y);
void func_ov007_02245888(BattleCommandMap *map);
extern "C" void func_ov007_022482a8(BattleCommandMap *map);
extern "C" void func_ov007_02247e00(BattleCommandMap *map);
extern "C" void func_ov007_02247a08(BattleCommandMap *map, u32 a);

extern const BattleObjPair data_ov007_0227a33c[];
extern const u8 data_ov007_0227a33d[];

class BattleObj00 : public SceneBase {
public:
    /* 0c */ CellAnimation *mAnims[3];
    /* 18 */ BattleCommandPage *mPage;
    /* 1c */ Camera *mCamera;
    /* 20 */ BattleCommandMap *mMap;
    /* 24 */ BattleUnionAbility *mUnion;
    /* 28 */ u16 mUnk_28;
    /* 2a */ s16 mUnk_2a;
    /* 2c */ s16 mUnk_2c;
    /* 2e */ u16 mUnk_2e;
    /* 30 */ u16 mUnk_30;
    /* 32 */ u8 mUnk_32;
    /* 33 */ u8 mUnk_33;
    /* 34 */ u8 mUnk_34;
    /* 35 */ u8 mUnk_35;
    /* 36 */ u8 mUnk_36;
    /* 37 */ u8 mUnk_37;
    /* 38 */ PAD(0x38, 0x39);
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ PAD(0x3b, 0x3c);
    /* 3c */ u8 mUnk_3c;
    /* 3d */ PAD(0x3d, 0x3e);
    /* 3e */ u8 mUnk_3e;
    /* 3f */ u8 mUnk_3f;
    /* 40 */

    BattleObj00(u8 flag);
    virtual ~BattleObj00();
    virtual void Update();
    void func_ov007_0224ad2c();
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
        mUnk_3f = 0;
    } else {
        mUnk_3f = 4;
    }
    mCamera = data_020d433c.mBattlePlayers->mCamera;
    mAnims[2] = 0;
    mAnims[1] = 0;
    mAnims[0] = 0;
    mUnion = 0;
    mUnk_2e = 0;
    mUnk_30 = 0;
    mUnk_3a = data_020d433c.mBattlePlayers->mTurnPlayer;
    if (flag == 0) {
        mUnk_32 = 0;
    } else {
        mUnk_32 = 11;
    }
    u32 v = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9]->mUnk_f1;
    if (v <= 1) {
        mUnk_3c = 0;
    } else {
        mUnk_3c = 1;
    }
    mUnk_34 = data_ov007_0227a33c[v].mRow;
    u8 b    = data_ov007_0227a33d[v * 2];
    mUnk_35 = b;
    BattlePlayerList *l = data_020d433c.mBattlePlayers;
    l->mUnk_28b = mUnk_34;
    l->mUnk_28c = b;
}

BattleObj00::~BattleObj00() {
    CellAnimation *anim = mAnims[2];
    if (anim) {
        delete anim;
    }
    mAnims[2] = 0;
    anim = mAnims[1];
    if (anim) {
        delete anim;
    }
    mAnims[1] = 0;
    anim = mAnims[0];
    if (anim) {
        delete anim;
    }
    mAnims[0] = 0;
    SpriteManager::Get().func_0202740c();
}

void BattleObj00::Update() {
    switch (mUnk_32) {
        case 0: {
            if (!func_ov007_022631bc(data_020d433c.mBattlePlayers, 0x10)) {
                return;
            }
            u8 player = data_020d433c.mBattlePlayers->mUnk_2a9;
            new BattleCharaAct(mCamera, player, 0, 0, 0);
            mContext->mRequest = 0;
            mUnk_32           = 1;
            return;
        }
        case 1:
            mUnk_28 = 0;
            mUnk_32++;
            return;
        case 2:
            if (mContext->mRequest != 0) {
                mUnk_32++;
            }
            return;
        case 3:
            if (!func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                return;
            }
            SpriteManager::Get().SetupMode(1, 7);
            mMap = new BattleCommandMap(data_020d433c.mFieldId);
            mMap->Init();
            data_020d433c.mGimmicks->mState = 1;
            func_ov007_022482a8(mMap);
            mUnk_32++;
            return;
        case 4:
            if (data_020d433c.mUnk_10) {
                data_020d433c.mUnk_10->SetupLayout(0xd);
            }
            SpriteManager::Get().StartFade(2);
            mUnk_32++;
            return;
        case 5: {
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            Camera *camera         = list->mCamera;
            if (((camera->mFrameIndex + 1 >= camera->mFrameCount) & 0xffff) != 0) {
                mUnk_32 = 7;
                return;
            }
            if (func_ov007_022632d0(list, 0)) {
                mUnk_32 = 7;
                return;
            }
            if (!InputManager::Get().IsHeld(8)) {
                return;
            }
            mUnk_32++;
            return;
        }
        case 6:
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                if (!func_ov007_022632d0(data_020d433c.mBattlePlayers, 1)) {
                    return;
                }
                mUnk_32++;
            } else {
                mUnk_32++;
            }
            return;
        case 7:
            func_ov007_02245888(mMap);
            mContext->mRequest = 0;
            mUnk_32++;
            return;
        case 8:
            if (mContext->mRequest != 0) {
                mUnk_32++;
            }
            return;
        case 9: {
            u8 player = data_020d433c.mBattlePlayers->mUnk_2a9;
            new BattleCharaAct(mCamera, player, 0xe, 0, 0);
            mContext->mRequest = 0;
            mUnk_32++;
            return;
        }
        case 10:
            if (mContext->mRequest == 0) {
                return;
            }
            if (!func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                return;
            }
            func_ov007_0223a690((s16) (mUnk_2a - mUnk_3f), mUnk_2c);
            func_ov007_0223ad70(mUnk_3c);
            if (data_020d433c.mUnk_9a_3) {
                mContext->mRequest = 1;
                mUnk_32           = 0x10;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                if (list->mPlayers[list->mUnk_2a9]->mSlots54Remaining == 0) {
                    mUnk_3e = 1;
                }
                return;
            }
            SpriteManager::Get().StartSubFade(2);
            mUnk_32 = 0xc;
            return;
        case 11:
            func_ov007_0223a690((s16) (mUnk_2a - mUnk_3f), mUnk_2c);
            func_ov007_0223ad70(mUnk_3c);
            func_ov007_0224ad2c();
            if (data_020d433c.mUnk_9a_3) {
                mContext->mRequest = 1;
                mUnk_32           = 0x10;
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                if (list->mPlayers[list->mUnk_2a9]->mSlots54Remaining == 0) {
                    mUnk_3e = 1;
                }
                return;
            }
            SpriteManager::Get().StartSubFade(2);
            mUnk_32 = 0xc;
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
                mUnk_32++;
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
            mUnk_32++;
            return;
        case 13: {
            u32 v = (u16) mContext->mRequest;
            if (v == 0) {
                return;
            }
            mUnk_39 = func_ov007_0225f0bc(v);
            mUnk_32++;
            WirelessManager::Get();
            if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
                return;
            }
            mUnion = new BattleUnionAbility(4, 0x1a000, 0xc);
            return;
        }
        case 14:
            if (!func_ov007_022631bc(data_020d433c.mBattlePlayers, 0xf)) {
                return;
            }
            SpriteManager::Get().StartFade(3);
            if (mUnion) {
                mUnion->Close(0);
            }
            mUnk_32++;
            return;
        case 15:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            CloseCommandPage(mPage, 1);
            mContext->mRequest = 0;
            mUnk_32           = 0x10;
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
            mUnk_32 = 0x11;
            return;
        }
        case 17:
            if (mContext->mRequest == 0) {
                return;
            }
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                data_0220f540.mUnk_04 = mUnk_39;
                if (func_ov007_022631bc(data_020d433c.mBattlePlayers, 7)) {
                    mUnk_32 = 0x12;
                }
            } else {
                data_0220f540.mUnk_04 = mUnk_39;
                mUnk_32               = 0x12;
            }
            return;
        case 18:
            func_ov007_0223a690((s16) (mUnk_2a - mUnk_3f), mUnk_2c);
            func_ov007_0223ad70(mUnk_3c);
            mUnk_37 = 0;
            mUnk_28 = 0;
            GXS_SetVisibleWnd(4);
            G2S_SetWndOBJInsidePlane(0x21, FALSE);
            G2S_SetWndOutsidePlane(0x11, FALSE);
            mMap = new BattleCommandMap(data_020d433c.mFieldId);
            mMap->mUnk_151 = 0;
            mMap->Init();
            func_ov007_02247e00(mMap);
            data_020d433c.mGimmicks->mState = 0;
            mUnk_32                         = 0x13;
            SpriteManager::Get().StartFade(2);
            return;
        case 19: {
            if (SpriteManager::Get().sFade[0] > 1) {
                return;
            }
            mUnk_28 = mUnk_28 + 1;
            if (mUnk_28 % 4 == 0 && mUnk_37 < 0x10) {
                mUnk_37 = mUnk_37 + 1;
                G2S_SetBlendBrightness(0x11, mUnk_37);
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
                    func_ov007_02262638(data_020d433c.mBattlePlayers, 6, 4, 1, func_ov007_0226a5e8(data_020d433c.mBattlePlayers->mPlayers[1]));
                } else {
                    func_ov007_02262638(data_020d433c.mBattlePlayers, 6, 4, 0, mUnk_39);
                }
            } else {
                u8 i = 0; 
                BattlePlayer *p; 
                u8 kind; 
                do {
                    p = data_020d433c.mBattlePlayers->mPlayers[i];
                    kind         = p->mUnk_f1;
                    if (p->mSlots54Remaining != 0) {
                        if (p->IsActive() || data_020d433c.mUnk_9a_3) {
                            func_ov007_02262638(data_020d433c.mBattlePlayers, data_ov007_0227a33c[kind].mRow, data_ov007_0227a33c[kind].mCol, i, func_ov007_0226a5e8(p));
                        } else {
                            if (IsWirelessMode((const u8 *) &WirelessManager::Get().mState)) {
                                func_ov007_02262638(data_020d433c.mBattlePlayers, data_ov007_0227a33c[kind].mRow, data_ov007_0227a33c[kind].mCol, i, (u8) *(u32 *) ((u8 *) data_020d433c.mBattlePlayers->mPlayers[i] + 0x104));
                            } else {
                                func_ov007_02262638(data_020d433c.mBattlePlayers, data_ov007_0227a33c[kind].mRow, data_ov007_0227a33c[kind].mCol, i, mUnk_39);
                            }
                        }
                    }
                    i++;
                } while (i < data_020d433c.mPlayerCount);
            }
            if (data_020d433c.mTutorialId != 0) {
                mUnk_32 = 0x14;
                data_020d433c.mBattlePlayers->mUnk_2a4->LoadGraphics();
                BattlePlayerList *list = data_020d433c.mBattlePlayers;
                list->mUnk_2a4->SetMessage(list->mUnk_2af, 0x64, 0, 0);
                G2S_SetBlendBrightness(0x1f, -10);
                func_ov007_02247a08(mMap, 0);
                return;
            }
            func_ov007_02245888(mMap);
            mContext->mRequest = 0;
            mUnk_32           = 0x16;
            return;
        }
        case 20:
            if (InputManager::Get().IsHeld(1) || TouchManager::Get().mTouching) {
                data_020d433c.mBattlePlayers->mUnk_2a4->NextPage(1);
            }
            if (data_020d433c.mBattlePlayers->mUnk_2a4->func_02087cac()) {
                return;
            }
            data_020d433c.mBattlePlayers->mUnk_2af++;
            if (data_020d433c.mBattlePlayers->mUnk_2af == 2) {
                mUnk_32++;
                G2S_SetBlendBrightness(0x1f, 0);
                return;
            }
            data_020d433c.mBattlePlayers->mUnk_2a4->SetMessage(data_020d433c.mBattlePlayers->mUnk_2af, 0x64, 0, 0);
            return;
        case 21:
            func_ov007_02245888(mMap);
            mContext->mRequest = 0;
            mUnk_32           = 0x16;
            return;
        case 22:
            if (mContext->mRequest != 0) {
                mUnk_32++;
            }
            return;
        case 23:
            mUnk_32++;
            SpriteManager::Get().StartFade(3);
            return;
        case 24:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_32 = 0x19;
            }
            return;
        case 25:
            RequestScene(1);
            mFinished = 1;
            return;
    }
}

void BattleObj00::func_ov007_0224ad2c() {
    ChangeDir("/Game/Battle/Obj");
    LoadAndQueueUpload(0, 4, 0, "battle_u.nce_ncg1D.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 5, 0, "battle_u_ncl.bin", 0, 0, 0);
    ChangeDir("/");
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Battle/Obj");
    mAnims[0] = new CellAnimation("battle_b_obj_00_nce.bin", 0x15, 1, 1);
    mAnims[0]->mCharBase = screen.GetCharBase(0x10000);
    mAnims[0]->mPaletteBase = 0;
    mAnims[1] = new CellAnimation("battle_b_obj_00_nce.bin", 0x16, 1, 1);
    mAnims[1]->mCharBase = screen.GetCharBase(0x10000);
    mAnims[1]->mPaletteBase = 0;
    mAnims[2] = new CellAnimation("battle_b_obj_00_nce.bin", 0x1b, 1, 1);
    mAnims[2]->mCharBase = screen.GetCharBase(0x10000);
    mAnims[2]->mPaletteBase = 0;
    ChangeDir("/Game/2D/union_b");
}
