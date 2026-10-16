#include "global.h"
#include <nitro/fx.h>
#include "types.h"
#include "Scene/SceneBase.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Battle/BattleTarget.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Text/TextManager.hpp"
#include "System/SoundManager.hpp"
#include "System/WirelessManager.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Effect.hpp"
#include "UI/SelectWindow.hpp"
#include "UI/MessageWindow.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include <nitro/math.h>
#include "Game/Random.hpp"
#include "Battle/BattleCpu.hpp"
#include "Battle/GimmickTypes.hpp"
#include "System/TouchManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Battle/BattleCpu.hpp"
#include "System/InputManager.hpp"
#include "Graphics/Display.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include "UI/StorytalkWindow.hpp"
#include "Battle/BattleUnk0227532c.hpp"

BOOL func_ov007_022631bc(BattlePlayerList *list, u32 id);

struct ShootPacket {
    u8 mUnk_00;
    u8 mUnk_01;
    u16 mUnk_02;
    u16 mUnk_04;
    u16 mUnk_06;
    u16 mUnk_08;
    u16 mUnk_0a;
    u16 mUnk_0c;
    u8 mUnk_0e;
    u8 mUnk_0f;
    u8 mUnk_10;
};

struct BattleShootCell { // 0xc bytes
    /* 00 */ CellAnimation *mAnim;
    /* 04 */ s16 mX;
    /* 06 */ s16 mY;
    /* 08 */ u8 mUnk_08;
    /* 09 */ PAD(0x09, 0x0c);
};

struct BattleShootIcon { // 0x10 bytes
    /* 00 */ CellAnimation *mAnim;
    /* 04 */ s16 mUnk_04;
    /* 06 */ s16 mUnk_06;
    /* 08 */ u16 mUnk_08;
    /* 0a */ s8 mUnk_0a;
    /* 0b */ s8 mUnk_0b;
    /* 0c */ u8 mUnk_0c;
    /* 0d */ PAD(0x0d, 0x10);
};

struct ShootWords7 {
    u32 mUnk_00[7];
};

struct BattleShootMsg { // 0x20 bytes: a message of the shoot minigame, sent to or received from a player
    /* 00 */ u8 mUnk_00;
    /* 01 */ u8 mUnk_01;
    /* 02 */ u8 mUnk_02;
    /* 03 */ u8 mUnk_03;
    union {
        struct {
            /* 04 */ s32 mUnk_04;
            /* 08 */ s32 mUnk_08;
            /* 0c */ u8 mUnk_0c;
        };
        /* 04 */ ShootWords7 mWords;
    };
};

struct ShootWords3 {
    u32 mUnk_00;
    u32 mUnk_04;
    u32 mUnk_08;
};


struct ShootMsg32 {
    u8 mUnk_00;
    u8 mUnk_01;
    u8 mUnk_02;
    u8 mUnk_03;
    u32 mUnk_04[7];
};

// The shoot minigame (vtable 0228a5a8, 0x61b0 bytes).
class BattleShoot : public SceneBase {
public:
    /* 000c */ u8 mBitmap[0x6000]; // 4bpp 256x192 bitmap in tile order, at +0x0c
    /* 600c */ BattleShootCell mCells[7];
    /* 6060 */ BattleShootIcon mIcons[5];
    /* 60b0 */ BattleShootMsg mMsgs[4];
    /* 6130 */ BattleTarget *mUnk_6130;
    /* 6134 */ SelectWindow *mUnk_6134;
    /* 6138 */ MessageWindow *mUnk_6138;
    /* 613c */ void *mUnk_613c;
    /* 6140 */ u32 mUnk_6140;
    /* 6144 */ Camera *mUnk_6144;
    /* 6148 */ PAD(0x6148, 0x6154);
    /* 6154 */ u16 mUnk_6154;
    /* 6156 */ u16 mUnk_6156;
    /* 6158 */ u16 mUnk_6158;
    /* 615a */ PAD(0x615a, 0x615c);
    /* 615c */ u32 mUnk_615c;
    /* 6160 */ u32 mUnk_6160;
    /* 6164 */ u32 mUnk_6164;
    /* 6168 */ u16 mUnk_6168;
    /* 616a */ u16 mUnk_616a;
    /* 616c */ s32 mUnk_616c;
    /* 6170 */ s32 mUnk_6170;
    /* 6174 */ u16 mUnk_6174;
    /* 6176 */ u16 mUnk_6176;
    /* 6178 */ u16 mUnk_6178;
    /* 617a */ u16 mUnk_617a;
    /* 617c */ u16 mUnk_617c;
    /* 617e */ u16 mUnk_617e;
    /* 6180 */ u16 mUnk_6180;
    /* 6182 */ u16 mUnk_6182;
    /* 6184 */ u16 mUnk_6184;
    /* 6186 */ u8 mUnk_6186;
    /* 6187 */ u8 mUnk_6187;
    /* 6188 */ u8 mUnk_6188;
    /* 6189 */ u8 mUnk_6189;
    /* 618a */ u8 mUnk_618a;
    /* 618b */ u8 mUnk_618b;
    /* 618c */ u8 mUnk_618c;
    /* 618d */ u8 mUnk_618d;
    /* 618e */ u8 mUnk_618e;
    /* 618f */ u8 mUnk_618f;
    /* 6190 */ u8 mUnk_6190;
    /* 6191 */ u8 mUnk_6191;
    /* 6192 */ u8 mUnk_6192;
    /* 6193 */ u8 mUnk_6193;
    /* 6194 */ u8 mUnk_6194;
    /* 6195 */ u8 mUnk_6195;
    /* 6196 */ u8 mUnk_6196;
    /* 6197 */ u8 mUnk_6197;
    /* 6198 */ u8 mUnk_6198;
    /* 6199 */ u8 mUnk_6199;
    /* 619a */ u8 mUnk_619a;
    /* 619b */ u8 mUnk_619b;
    /* 619c */ u8 mUnk_619c;
    /* 619d */ u8 mUnk_619d;
    /* 619e */ u8 mUnk_619e;
    /* 619f */ u8 mUnk_619f;
    /* 61a0 */ u8 mUnk_61a0;
    /* 61a1 */ u8 mUnk_61a1;
    /* 61a2 */ u8 mUnk_61a2;
    /* 61a3 */ u8 mUnk_61a3;
    /* 61a4 */ u8 mUnk_61a4;
    /* 61a5 */ u8 mUnk_61a5;
    /* 61a6 */ s8 mUnk_61a6;
    /* 61a7 */ u8 mUnk_61a7;
    /* 61a8 */ u8 mUnk_61a8;
    /* 61a9 */ u8 mUnk_61a9;
    /* 61aa */ PAD(0x61aa, 0x61b0);
    /* 61b0 */

    BattleShoot(Camera *unk1, u8 unk2);
    BattleShoot(Camera *unk1, const u16 *pos, u16 unk3, u16 unk4, u8 unk5, u8 unk6, u8 unk7);
    virtual ~BattleShoot();
    /* 08 */ virtual void Update();
    void func_ov007_0226aa14(Camera *unk1);
    void func_ov007_0226bb2c();
    void func_ov007_0226bd3c();
    void func_ov007_0226bdfc();
    void func_ov007_0226f604();
    void func_ov007_0226e884();
    void func_ov007_0226e430();
    void func_ov007_0226f0e8(u8 kind, s32 x, s32 z);
    void func_ov007_0226f378();
    void func_ov007_0226dfb8();
    BOOL func_ov007_0226d23c();
    void func_ov007_0226daa8();
    BOOL func_ov007_0226d7ac();
    void func_ov007_0226cb9c(BOOL canStart);
    void func_ov007_0226f59c();
    void func_ov007_0226f644();
    void func_ov007_0226f698(u8 x0, u8 y0, u8 x1, u8 y1);
    void func_ov007_0226f76c(u8 x, u8 y);
    void func_ov007_0226f820(u8 x, u8 y);
    void func_ov007_0226f8a8();
    BOOL func_ov007_0226f9e8();
    void func_ov007_0226fb2c(u8 value);
    void func_ov007_0226c798();
    void func_ov007_0226cad8();
    void func_ov007_0226d690();
    void func_ov007_0226e234();
    void func_ov007_0226f87c();
    BOOL func_ov007_0226d214();
    u16 func_ov007_0226df24(int a, int b);
    u16 func_ov007_0226e390();
};

extern const u16 data_ov007_0227bcd0[];
extern const s8 data_ov007_0227bce0[12][2];
void func_ov007_0226fb50(void *arg);

BattleShoot::BattleShoot(Camera *unk1, u8 unk2)
    : SceneBase(0x3e8, 0) {
    func_ov007_0226aa14(unk1);
    mUnk_6154 = 0;
    mUnk_6156 = 0x6000;
    mUnk_6158 = 0x38e;
    mUnk_619e = 0;
    mUnk_61a2 = unk2;
    mUnk_6186 = 0;
}

BattleShoot::BattleShoot(Camera *unk1, const u16 *pos, u16 unk3, u16 unk4, u8 unk5, u8 unk6, u8 unk7)
    : SceneBase(0x3e8, 0) {
    func_ov007_0226aa14(unk1);
    mUnk_6154 = pos[0];
    mUnk_6156 = pos[1];
    mUnk_6158 = pos[2];
    mUnk_6193 = unk5;
    mUnk_6168 = unk3;
    mUnk_616a = unk4;
    mUnk_6198 = unk6;
    mUnk_61a2 = unk7;
    mUnk_6186 = 7;
}

void BattleShoot::func_ov007_0226aa14(Camera *unk1) {
    mUnk_61a7 = 0;
    mUnk_6188 = 0;
    mUnk_6193 = 0;
    mUnk_6189 = 0;
    mUnk_618a = 0;
    mUnk_618b = 0;
    mUnk_618c = 0;
    mUnk_6164 = 0;
    mUnk_618f = 0;
    mUnk_6190 = 0;
    mUnk_618d = 0;
    mUnk_618e = 0;
    mUnk_6191 = 0;
    mUnk_6192 = 0;
    mUnk_615c = 0;
    mUnk_6160 = 0;
    mUnk_6194 = 0;
    mUnk_6168 = 0;
    mUnk_616a = data_ov007_0227bcd0[data_020d433c.mUnk_95];
    mUnk_6195 = 1;
    mUnk_6196 = 0;
    mUnk_6197 = 0;
    mUnk_6198 = 0;
    mUnk_6199 = 0;
    mUnk_6182 = 0x122;
    mUnk_6144 = unk1;
    mUnk_6134 = 0;
    mUnk_6130 = 0;
    mUnk_6138 = 0;
    mUnk_616c = 0x5b;
    mUnk_6170 = 0xb6;
    mUnk_6174 = 0;
    mUnk_6176 = 0;
    mUnk_6178 = 0;
    mUnk_617a = 0;
    mUnk_617c = 0;
    mUnk_617e = 0;
    mUnk_6180 = 0;
    mUnk_61a0 = 1;
    mUnk_61a1 = 1;
    mUnk_61a5 = 0;
    mUnk_61a4 = 0;
    mUnk_61a6 = 0x30;
    mUnk_6184 = 0;
    mUnk_619a = 0x80;
    mUnk_619b = 0x80;
    mUnk_613c = 0;
    mUnk_6140 = 0;
    mUnk_619c = 0;
    mUnk_619d = 0;
    mUnk_619f = 0;
    mUnk_61a2 = 0;
    if (data_020d433c.mBattlePlayers->mUnk_28a == data_020d433c.mBattlePlayers->mUnk_2a9) {
        mUnk_6197 = 1;
    }
    MI_CpuClear32(mCells, 0x54);
    MI_CpuClear32(mIcons, 0x50);
    MI_CpuClear32(mMsgs, 0x80);
    mUnk_61a9 = data_020d433c.mPlayerCount - 1;
}

BattleShoot::~BattleShoot() {
    u8 i = 0;
    do {
        CellAnimation *a = mCells[i].mAnim;
        if (a) {
            delete a;
        }
        i++;
    } while (i < 7);
    u8 j = 0;
    do {
        CellAnimation *a = mIcons[j].mAnim;
        if (a) {
            delete a;
        }
        j++;
    } while (j < 5);
    if (mUnk_613c) {
        OS_Free(mUnk_613c);
    }
    data_020d433c.mEffects->func_0202be68();
    func_0202bc58(data_020d433c.mEffects, 0);
    data_020d433c.mBattlePlayers->mUnk_298 = 0;
    if (mUnk_6134) {
        delete mUnk_6134;
    }
    if (mUnk_6138) {
        delete mUnk_6138;
    }
    G2_BlendNone();
    G2S_BlendNone();
}

static inline bool CpuActive(BattleCpu *cpu) {
    bool active = false;
    if (cpu) {
        active = true;
    }
    return active;
}

void BattleShoot::Update() {
    func_ov007_0226cb9c(mUnk_6186 == 12);
    switch (mUnk_6186) {
        case 0:
            func_ov007_0226bd3c();
            mUnk_6187 = 4;
            mUnk_6186++;
            break;
        case 1:
            mUnk_6187--;
            if (mUnk_6187 == 0) {
                if (data_020d433c.mUnk_85 == 3) {
                    mUnk_6134->mUnk_064 = 0;
                } else {
                    mUnk_6134->mUnk_064 = 1;
                }
                SpriteManager::Get().StartFade(2);
                u8 mode = data_020d433c.mUnk_85;
                if (mode >= 3 && mode <= 12) {
                    data_020d433c.mBattlePlayers->mUnk_2a4->LoadGraphics();
                    data_020d433c.mBattlePlayers->mUnk_2a4->SetMessage(data_020d433c.mBattlePlayers->mUnk_2af, 0x64, 0, 0);
                    G2S_SetBlendBrightness(0x1f, -10);
                    mUnk_6186 = 2;
                } else {
                    mUnk_6186 = 3;
                }
            }
            break;
        case 2: {
            mCells[0].mX = (mCells[0].mX + 0x80) / 2;
            mCells[0].mY = (mCells[0].mY + 0xa0) / 2;
            if (InputManager::Get().IsHeld(1) || TouchManager::Get().mTouching) {
                data_020d433c.mBattlePlayers->mUnk_2a4->NextPage(1);
            }
            u8 limit;
            if (data_020d433c.mUnk_83 == 3) {
                limit = 2;
            } else if (data_020d433c.mUnk_83 == 2) {
                limit = 2;
            } else {
                limit = 4;
            }
            if (!data_020d433c.mBattlePlayers->mUnk_2a4->func_02087cac()) {
                data_020d433c.mBattlePlayers->mUnk_2af++;
                if (data_020d433c.mBattlePlayers->mUnk_2af > limit) {
                    mUnk_6186 = 3;
                } else {
                    data_020d433c.mBattlePlayers->mUnk_2a4->SetMessage(data_020d433c.mBattlePlayers->mUnk_2af, 0x64, 0, 0);
                }
            }
            break;
        }
        case 3:
            func_ov007_0226fb2c(0x17);
            G2S_SetBlendBrightness(0x1f, 0);
            mUnk_6186++;
            break;
        case 4:
            if (func_ov007_0226f9e8()) {
                mUnk_61a0 = 0;
                break;
            }
            if (data_020d433c.mUnk_85 != 3) {
                mUnk_6134->Draw();
            }
            if (func_ov007_0226d23c()) {
                mUnk_6134->mUnk_064 = 0;
                func_ov007_0226dfb8();
                mUnk_61a1 = 0;
                mUnk_6186++;
                break;
            }
            if (func_ov007_0226d214()) {
                SpriteManager::Get().StartFade(3);
                mUnk_6186 = 0x10;
                func_ov007_0226f378();
                return;
            }
            break;
        case 5:
            if (mCells[0].mY > -0x40) {
                mCells[0].mX += (s8) mUnk_6191;
                mCells[0].mY += (s8) mUnk_6192;
            } else {
                func_ov007_0226fb2c(0x13);
                WirelessManager::Get();
                if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                    mUnk_6186 = 6;
                } else {
                    mUnk_61a4 = 1;
                    mUnk_619c = 3;
                    mUnk_619d = 0;
                    if (data_020d433c.mUnk_83 != 0 || mUnk_61a2 != 0) {
                        mUnk_619d = 1;
                    }
                    mUnk_6186 = 11;
                }
            }
            break;
        case 6: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            ((ShootPacket *) &data_0220f540)->mUnk_04     = mUnk_6154;
            ((ShootPacket *) &data_0220f540)->mUnk_06     = mUnk_6156;
            ((ShootPacket *) &data_0220f540)->mUnk_08     = mUnk_6158;
            ((ShootPacket *) &data_0220f540)->mUnk_0a     = mUnk_6168;
            ((ShootPacket *) &data_0220f540)->mUnk_0c     = mUnk_616a;
            ((ShootPacket *) &data_0220f540)->mUnk_0f     = mUnk_6193;
            ((ShootPacket *) &data_0220f540)->mUnk_10     = mUnk_6198;
            ((ShootPacket *) &data_0220f540)->mUnk_0e           = list->mPlayers[list->mUnk_28a]->mRecord;
            if (func_ov007_022631bc(list, 8)) {
                mUnk_6186 = 10;
            }
            break;
        }
        case 7:
            func_ov007_0226bd3c();
            func_ov007_0226fb2c(0x11);
            mUnk_6187 = 4;
            mUnk_6186++;
            break;
        case 8:
            mUnk_6187--;
            if (mUnk_6187 == 0) {
                SpriteManager::Get().StartFade(2);
                mUnk_6186++;
            }
            break;
        case 9: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            ((ShootPacket *) &data_0220f540)->mUnk_04     = mUnk_6154;
            ((ShootPacket *) &data_0220f540)->mUnk_06     = mUnk_6156;
            ((ShootPacket *) &data_0220f540)->mUnk_08     = mUnk_6158;
            ((ShootPacket *) &data_0220f540)->mUnk_0a     = mUnk_6168;
            ((ShootPacket *) &data_0220f540)->mUnk_0c     = mUnk_616a;
            ((ShootPacket *) &data_0220f540)->mUnk_0f     = mUnk_6193;
            ((ShootPacket *) &data_0220f540)->mUnk_10     = mUnk_6198;
            ((ShootPacket *) &data_0220f540)->mUnk_0e     = 0;
            if (func_ov007_022631bc(list, 8)) {
                mUnk_6186++;
            }
            break;
        }
        case 10: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            BattleCpu *cpu = list->mPlayers[list->mUnk_28a]->mUnk_c8;
            if (CpuActive(cpu)) {
                u8 kind = mUnk_6198;
                if (kind == 1 || kind == 3 || kind == 4) {
                    mUnk_616a = 0;
                    mUnk_6168 = func_ov007_0226e390();
                    mUnk_6156 = 0x6000;
                    mUnk_6158 = 0;
                    *(u16 *) ((u8 *) mUnk_6130 + 0x1b8) = mUnk_6168;
                    *(u16 *) ((u8 *) mUnk_6130 + 0x1ba) = mUnk_616a;
                } else if (kind == 2) {
                    cpu->func_ov007_022329cc(0x14000);
                }
            }
            u8 idx = data_020d433c.mBattlePlayers->mUnk_28a;
            data_020d433c.mBattlePlayers->mPlayers[idx]->mUnk_ec = mUnk_6193;
            BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[idx];
            p->mUnk_d4 = 0;
            p->mUnk_d8 = 0;
            p = data_020d433c.mBattlePlayers->mPlayers[idx];
            p->mUnk_cc[0]              = mUnk_6154;
            p->mUnk_cc[1]              = mUnk_6156;
            p->mUnk_cc[2]              = mUnk_6158;
            mUnk_6130->mUnk_1df        = mUnk_6198;
            mUnk_6130->func_ov007_0225b6d0();
            if (mUnk_6198 == 0) {
                mUnk_6182 = 0x122;
            }
            mUnk_619e = 0x14;
            mUnk_61a4 = 1;
            if (data_020d433c.mUnk_85 != 0 && data_020d433c.mBattlePlayers->mUnk_28a != 0) {
                mUnk_61a4 = 0;
            }
            mUnk_61a1 = 0;
            mUnk_619c = 3;
            mUnk_619d = 0;
            if (data_020d433c.mUnk_83 != 0) {
                mUnk_619c = 0;
            } else if (mUnk_61a2 != 0) {
                mUnk_619d = 1;
            }
            if (mUnk_6198 == 5) {
                mUnk_619c = 0;
                mUnk_619d = 0;
            }
            mUnk_6186++;
            break;
        }
        case 11: {
            BattlePlayerList *l = data_020d433c.mBattlePlayers;
            if (l->mPlayers[l->mUnk_28a]->mUnk_c8 == 0) {
                u8 i = 0;
                do {
                    BattlePlayerList *l3 = data_020d433c.mBattlePlayers;
                    if (l3->mPlayers[i]->mUnk_c8 != 0 && i != l3->mUnk_28a) {
                        l3->mPlayers[i]->mUnk_c8->func_ov007_02232c88();
                    }
                    i++;
                } while (i < data_020d433c.mPlayerCount);
            }
            mUnk_6186++;
            break;
        }
        case 12:
            if (!func_ov007_0226f9e8()) {
                if (mContext->mUnk_12 != 0) {
                    mUnk_61a0 = 0;
                    mUnk_61a6 = 0x30;
                    mUnk_6182 = 0x122;
                    mUnk_6186 = 15;
                } else {
                    if (mUnk_61a0 != 0 && mUnk_6130->mUnk_1d7 != 0) {
                        u8 i = 0;
                        do {
                            mIcons[i].mUnk_0c = 0;
                            i++;
                        } while (i < 5);
                        mUnk_61a0 = 0;
                        mUnk_61a6 = 0x30;
                        mUnk_6182 = 0x122;
                    }
                    if (mUnk_6130->func_ov007_0225d078()) {
                        G2S_SetBG1Priority(3);
                        mUnk_61a6 = 0x30;
                    }
                    if (mUnk_6130->mUnk_1cb == 0) {
                        BattlePlayerList *l = data_020d433c.mBattlePlayers;
                        if (l->mPlayers[l->mUnk_28a]->IsActive()) {
                            func_ov007_0226e430();
                        }
                        func_ov007_0226e234();
                        func_ov007_0226e884();
                    }
                    func_ov007_0226d690();
                }
            }
            break;
        case 13:
            new BattleUnk0227532c(3);
            mContext->mUnk_12 = 0;
            mUnk_6186++;
            break;
        case 14:
            switch (mContext->mUnk_12) {
                case 1:
                    mUnk_6186 = mUnk_61a8;
                    if (mUnk_6186 == 4) {
                        mUnk_61a0 = 1;
                        mUnk_61a1 = 1;
                        mUnk_6186 = 3;
                    } else {
                        func_ov007_0226fb2c(0x11);
                    }
                    func_ov007_0226f604();
                    data_020d433c.mUnk_14 &= ~8;
                    mContext->mUnk_12 = 0;
                    break;
                case 2:
                    if (data_020d433c.mUnk_a8 != 0) {
                        WirelessManager::Get();
                        data_020c4030.mUnk_22a = 6;
                        mUnk_6186++;
                    } else {
                        SpriteManager::Get().StartFade(3);
                        mUnk_6186 = 0x10;
                    }
                    break;
            }
            break;
        case 15:
            u16 scene = mContext->mUnk_12;
            if (scene != 0) {
                RequestScene(scene);
                mFinished = 1;
            }
            break;
        case 16:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_6130->func_ov007_02253fbc();
                mUnk_6186 = 15;
                data_020d433c.mUnk_14 &= ~8;
            }
            break;
    }
    func_ov007_0226f378();
    if (mUnk_61a0 != 0 && mUnk_6186 != 14) {
        func_ov007_0226f8a8();
    }
    if (mUnk_61a4 != 0) {
        func_ov007_0226f59c();
    }
}

void BattleShoot::func_ov007_0226bb2c() {
    reg_GX_POWCNT |= 0x8000;
    SpriteManager::Get().func_02026dec(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(3);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 1, 0);
    G2S_SetBG2Control(0, 0, 4, 6, 0);
    G2S_SetBG3Control(1, 0, 6, 6, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(2);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(1);
    G2S_SetBG3Priority(1);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x17);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, 0);
    G2_BlendNone();
    G2S_BlendNone();
    func_ov007_0226f87c();
}

static const WindowCellRecord sShootCells[1] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x6000, 5, 0, 1, {2, 3, 3, 0}},
};

static const WindowItemRecord sShootItems[22] = { // only the first entry is used
    {0, 2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
};

void BattleShoot::func_ov007_0226bd3c() {
    func_ov007_0226bb2c();
    func_ov007_0226bdfc();
    func_ov007_0226c798();
    u8 player = data_020d433c.mBattlePlayers->mUnk_28a;
    u8 record = data_020d433c.mBattlePlayers->mPlayers[player]->mRecord;
    delete mUnk_6130;
    mUnk_6130                              = new BattleTarget(mUnk_6144, player, record);
    data_020d433c.mBattlePlayers->mUnk_298 = mUnk_6130;
    mContext->mUnk_12                      = 0;
    mUnk_6130->mUnk_1b8                    = mUnk_6168;
    mUnk_6130->mUnk_1ba                    = mUnk_616a;
}

void BattleShoot::func_ov007_0226bdfc() {
    char buffer[0x80];
    u8 player;
    u8 record;
    func_0202ba64(data_020d433c.mEffects, 0, "jammer");
    player = data_020d433c.mBattlePlayers->mUnk_28a;
    record = data_020d433c.mBattlePlayers->mPlayers[player]->RecordIndex();
    ChangeDir("/Game/Battle/Bg/");
    func_02025b64(1, 1, 0, "battle_shoot_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 0xb, 0, "battle_shoot_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "battle_shoot_b_bg_00_b_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "battle_shoot_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "battle_shoot_b_bg_00_a_nsc.bin", 0, 0, 0);
    func_02025b64(1, 6, 0, "pentest_pen_ncl.bin", 0, 0, 0);
    func_02025b64(1, 9, 0, "pentest_pen_nsc.bin", 0, 0, 0);
    func_ov007_0226f604();
    u8 id = data_020d433c.mBattlePlayers->mPlayers[player]->mRecords[record].mUnk_09;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    ChangeDir("/Game/Battle/Obj/");
    sprintf(buffer, "battle_shoot_b_ball_%02d.nce_ncg1D.bin", id);
    screen.LoadCharacters(buffer, 0, 0);
    sprintf(buffer, "battle_shoot_b_ball_%02d_ncl.bin", id);
    screen.func_02027fec(buffer, 0, 0);
    sprintf(buffer, "battle_shoot_b_ball_%02d.nce_ncg1D.bin", id);
    screen.LoadCharacters("battle_shoot_b_ball_sp.nce_ncg1D.bin", 0x2000, 0);
    void *palette = func_02023504("battle_shoot_b_ball_sp_ncl.bin", 0, id << 5, 0x20, 0, 0);
    func_02025c1c(screen.mBgIndex, 5, 0, palette, 0x20, 1);
    mCells[0].mAnim = new CellAnimation("battle_shoot_b_ball_00_nce.bin", 1, 1, 1);
    mCells[0].mAnim->mCharBase = screen.func_020276f4(0);
    mCells[0].mAnim->mUnk_11   = 0;
    mCells[0].mX               = 0x80;
    mCells[0].mY               = -0x40;
    mCells[0].mUnk_08          = 1;
    mCells[1].mAnim = new CellAnimation("battle_shoot_b_ball_sp_nce.bin", 0, 1, 1);
    mCells[1].mAnim->mCharBase = screen.func_020276f4(0x2000);
    mCells[1].mAnim->mUnk_11   = 0;
    mCells[1].mX               = 0x80;
    mCells[1].mY               = -0x40;
    mCells[1].mUnk_08          = 1;
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x6000, 0);
    screen.LoadPalette("union_b_obj_cockpit_ncl.bin", 0xa0, 0);
    screen.LoadCharacters("union_b_obj_mic.nce_ncg1D.bin", 0x7000, 0);
    screen.LoadPalette("union_b_obj_mic_ncl.bin", 0xc0, 0);
    mCells[4].mAnim = new CellAnimation("union_b_obj_mic_nce.bin", 1, 1, 1);
    mCells[4].mAnim->mCharBase = screen.func_020276f4(0x7000);
    mCells[4].mAnim->mUnk_11   = 6;
    mCells[4].mX               = 0x80;
    mCells[4].mY               = 0x30;
    mCells[4].mUnk_08          = 2;
    if (data_020d433c.mUnk_9a_0 && data_020d433c.mUnk_83 == 0) {
        mCells[4].mAnim->SetAnimation(0);
    }
    mUnk_61a6 = 0x60;
    ChangeDir("/Game/Battle/Obj/");
    screen.LoadCharacters("battle_shoot_b_effect.nce_ncg1D.bin", 0xf000, 0);
    screen.LoadPalette("battle_shoot_b_effect_ncl.bin", 0x140, 0);
    u8 i = 0;
    do {
        mIcons[i].mAnim = new CellAnimation("battle_shoot_b_effect_nce.bin", 0, 1, 1);
        mIcons[i].mAnim->mCharBase = screen.func_020276f4(0xf000);
        mIcons[i].mAnim->mUnk_11   = 10;
        mIcons[i].mUnk_0c          = 0;
        i++;
    } while (i < 5);
    mCells[5].mAnim = new CellAnimation("battle_shoot_b_effect_nce.bin", 1, 1, 1);
    mCells[5].mAnim->mCharBase = screen.func_020276f4(0xf000);
    mCells[5].mAnim->mUnk_11   = 10;
    mCells[6].mAnim = new CellAnimation("battle_shoot_b_effect_nce.bin", 2, 1, 1);
    mCells[6].mAnim->mCharBase = screen.func_020276f4(0xf000);
    mCells[6].mAnim->mUnk_11   = 10;
    sprintf(buffer, "battle_shoot_b_obj_%02d.nce_ncg1D.bin", id);
    screen.LoadCharacters(buffer, 0x4000, 0);
    sprintf(buffer, "battle_shoot_b_obj_%02d_ncl.bin", id);
    screen.LoadPalette(buffer, 0x20, 0);
    sprintf(buffer, "battle_shoot_b_obj_%02d_nce.bin", id);
    mCells[2].mAnim = new CellAnimation(buffer, 0, 1, 1);
    mCells[2].mAnim->mCharBase = screen.func_020276f4(0x4000);
    mCells[2].mAnim->mUnk_11   = 1;
    mCells[2].mX               = 0x122;
    mCells[2].mY               = 0x18;
    mCells[2].mUnk_08          = 1;
    bool flag = false;
    if (data_020d433c.mBattlePlayers->Current()->mUnk_f3 >= 0xbe) {
        u8 kind = data_020d433c.mBattlePlayers->Current()->CurrentRecord()->mUnk_08;
        if ((kind == 0x24) | (kind == 0x25)) {
            flag = true;
        } else if ((u8) data_020d433c.mBattlePlayers->Current()->CurrentRecord()->mUnk_00_0 >= 2) {
            flag = true;
        }
    }
    if (!flag) {
        mUnk_6196 = 0;
    } else {
        mCells[2].mAnim->SetAnimation(2);
        mUnk_6196 = 1;
        mUnk_6182 = 0xdc;
    }
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(buffer, "vs_b_obj_pen.nce_ncg1D.bin", id);
    screen.LoadCharacters(buffer, 0xe000, 0);
    sprintf(buffer, "vs_cutin_perfect_ncl.bin", id);
    screen.LoadPalette(buffer, 0x120, 0);
    sprintf(buffer, "vs_b_obj_pen_nce.bin", id);
    mCells[3].mAnim = new CellAnimation(buffer, 4, 1, 1);
    mCells[3].mAnim->mCharBase = screen.func_020276f4(0xe000);
    mCells[3].mAnim->mUnk_11   = 9;
    mCells[3].mX               = 0x80;
    mCells[3].mY               = 0x60;
    mCells[3].mUnk_08          = 1;
    ChangeDir("/");
    mUnk_6134 = new SelectWindow(1, sShootItems, 0);
    mUnk_6134->SetCells(1, sShootCells);
    mUnk_6134->mCursor->mSelection = -1;
    mUnk_6134->Update(1);
    mUnk_6138 = new MessageWindow("mes_CardName", 1, 0x9000);
    mUnk_6138->func_02065bd4(0x20, 0xa2, 2);
    mUnk_6138->func_02065be4(0xe0);
    mUnk_6138->func_02065890(0, 0, 1);
    if (data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->SetupLayout(9);
    }
}

void BattleShoot::func_ov007_0226c798() {
    MessageFile *file = (MessageFile *) func_02023504("/font/mes_shoot.mes", 0, 0, 0, 0, 0);
    u32 size          = TextManager::Get().BufferSize(0x18, 0xa);
    void *buffer      = OS_Alloc(size);
    MI_CpuClear16(buffer, size);
    mUnk_6140 = TextManager::Get().GlyphCount(0x1e);
    mUnk_613c = OS_Alloc(mUnk_6140 * (s16) 0xc);
    TextManager::Get().Init(mUnk_613c, 1, 0, 0x18, 0xa, 0x21);
    u16 pixelWidth = 0;
    TextManager::Get().mUnk_5b = 1;
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width  = 0;
    height = 0;
    text.Print(file->GetText(0), file->GetLength(0), buffer, 0x18, 0xa, &height, &width, &pixelWidth, 0, 2, 0x21, 0x10, 0, 0, 0, 1, 1);
    TextManager::Get().mUnk_5b = 0;
    mUnk_619f = pixelWidth;
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x14000, buffer, size, 0);
    if (file) {
        OS_Free(file);
    }
    if (buffer) {
        OS_Free(buffer);
    }
}

static inline u16 KeysReleased(InputManager &input, u16 keys) {
    return input.mReleased & keys;
}

void BattleShoot::func_ov007_0226cad8() {
    SpriteManager::Get().Screen(1).func_02027778(mUnk_613c, mUnk_6140, (s16) (0x80 - ((u32) mUnk_619f >> 1)), 0x50, *(u8 *) ((u8 *) this + 0x6038), 0x280, 0xf, 0);
}

void BattleShoot::func_ov007_0226cb9c(BOOL canStart) {
    mUnk_618a = mUnk_6189;
    if (canStart) {
        if (mUnk_61a7 != 0) {
            mUnk_61a7--;
        }
    } else {
        mUnk_61a7 = 0;
    }
    if (mUnk_6189 != 0) {
        if (TouchManager::Get().mUnk_56 != 0) {
            mUnk_618d = mUnk_618f;
            mUnk_618e = mUnk_6190;
            mUnk_618f = TouchManager::Get().mX;
            mUnk_6190 = TouchManager::Get().mY;
            BOOL draw = FALSE;
            if (mUnk_6189 < 0x1e) {
                mUnk_6189++;
            }
            if (mUnk_618a != 0) {
                if (mUnk_6199 == 0) {
                    draw = TRUE;
                }
            }
            if (draw) {
                func_ov007_0226f698(mUnk_618f, mUnk_6190, mUnk_618d, mUnk_618e);
            }
        } else {
            mUnk_6189 = 0;
            func_ov007_0226f604();
        }
    } else {
        mUnk_6199 = 0;
        if (TouchManager::Get().mUnk_56 != 0) {
            if (TouchManager::Get().mUnk_57 != 0) {
                mUnk_618f = TouchManager::Get().mX;
                mUnk_6190 = TouchManager::Get().mY;
            }
            if ((mUnk_618f != TouchManager::Get().mX) | (mUnk_6190 != TouchManager::Get().mY)) {
                mUnk_618f = TouchManager::Get().mX;
                mUnk_6190 = TouchManager::Get().mY;
                mUnk_618d = mUnk_618f;
                mUnk_618e = mUnk_6190;
                mUnk_618b = mUnk_618f;
                mUnk_618c = mUnk_6190;
                mUnk_6189++;
                if (mUnk_6196 != 0) {
                    if ((mUnk_618f > 0xc8) & (mUnk_6190 < 0x30)) {
                        mUnk_6199 = 1;
                    }
                }
                if ((mUnk_6190 > 0xa0) & (mUnk_618f > 0xc8)) {
                    mUnk_6199 = 1;
                }
            }
        } else if (canStart) {
            if (mUnk_61a7 == 0) {
                mUnk_618b = mUnk_618d = 0x80;
                mUnk_618c = mUnk_618e = 0x60;
                mUnk_618f = 0x80;
                mUnk_6190 = 0x60;
                if (InputManager::Get().IsDown(0x820)) {
                    mUnk_618b = mUnk_618d = 0xa8;
                    mUnk_618f = 0x58;
                    mUnk_618a = mUnk_6189 = 1;
                    mUnk_61a7 = 0xf;
                } else if (InputManager::Get().IsDown(0x11)) {
                    mUnk_618b = mUnk_618d = 0x58;
                    mUnk_618f = 0xa8;
                    mUnk_618a = mUnk_6189 = 1;
                    mUnk_61a7 = 0xf;
                }
                if (InputManager::Get().IsDown(0x440)) {
                    mUnk_618c = mUnk_618e = 0x88;
                    mUnk_6190 = 0x38;
                    mUnk_618a = mUnk_6189 = 1;
                    mUnk_61a7 = 0xf;
                } else if (InputManager::Get().IsDown(0x82)) {
                    mUnk_618c = mUnk_618e = 0x38;
                    mUnk_6190 = 0x88;
                    mUnk_618a = mUnk_6189 = 1;
                    mUnk_61a7 = 0xf;
                }
            }
        }
    }
}

BOOL BattleShoot::func_ov007_0226d214() {
    return (u8) mUnk_6134->Update(1) == 2;
}

struct BattleShootCurve { // pairs at data_ov007_0227bca0
    u8 mUnk_00;
    u8 mUnk_01;
};
extern const BattleShootCurve data_ov007_0227bca0[5];

BOOL BattleShoot::func_ov007_0226d23c() {
    BOOL moving = FALSE;
    if (mUnk_618a != 0 && mUnk_6189 != 0) {
        if (mUnk_6199 == 0) {
            moving = TRUE;
        }
    }
    if (moving) {
        mCells[0].mX = (mCells[0].mX + mUnk_618f) / 2;
        mCells[0].mY = (mCells[0].mY + mUnk_6190) / 2;
    } else {
        mCells[0].mX = (mCells[0].mX + 0x80) / 2;
        mCells[0].mY = (mCells[0].mY + 0xa0) / 2;
    }
    if (func_ov007_0226d7ac()) {
        return 0;
    }
    if (mUnk_618a != 0) {
        if (mUnk_6189 != 0) {
            if (mUnk_618e <= mUnk_6190) {
                mUnk_6189 = 1;
                mUnk_618b = mUnk_618f;
                mUnk_618c = mUnk_6190;
                func_ov007_0226f604();
            }
        } else {
          if (mUnk_6199 != 0) {
            if (((mUnk_618f > 0xc8) & (mUnk_6190 < 0x30)) && mUnk_6196 != 0) {
                u8 record = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_28a]->mRecords[data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_28a]->mRecord].mUnk_09;
                if (mUnk_6198 == 0) {
                    mCells[2].mAnim->SetAnimation(0);
                    mCells[0].mAnim->SetAnimation(2);
                    mUnk_6198 = record + 1;
                    SoundManager::Get().func_020246f8(0);
                } else {
                    mCells[2].mAnim->SetAnimation(2);
                    mCells[0].mAnim->SetAnimation(1);
                    mUnk_6198 = 0;
                    SoundManager::Get().func_020246f8(1);
                }
                mUnk_6130->mUnk_1df = mUnk_6198;
                mUnk_6189 = 0;
                return 0;
            }
          } else {
            s32 dist = func_ov007_0226df24((s16) (mUnk_618b - mUnk_618f), (s16) (mUnk_618c - mUnk_6190));
            if (dist > 0x14 && mUnk_618e >= mUnk_6190) {
                s32 dx = (mUnk_618f - mUnk_618b) / 8;
                if (dx > 8) {
                    dx = 8;
                } else if (dx < -8) {
                    dx = -8;
                }
                s32 dx2 = mUnk_618f - mUnk_618d;
                if (dx2 > 8) {
                    dx2 = 8;
                } else if (dx2 < -8) {
                    dx2 = -8;
                }
                mUnk_6191 = (dx2 + dx) / 2;
                s32 dy = (mUnk_6190 - mUnk_618c) / 8;
                if (dy > -8) {
                    dy = -8;
                } else if (dy < -16) {
                    dy = -16;
                }
                s32 dy2 = mUnk_6190 - mUnk_618e;
                if (dy2 > -8) {
                    dy2 = -8;
                } else if (dy2 < -16) {
                    dy2 = -16;
                }
                mUnk_6192 = (dy2 + dy) / 2;
                return 1;
            }
          }
          mUnk_6189 = 0;
        }
    } else {
        func_ov007_0226daa8();
        mUnk_6130->mUnk_1b8 = mUnk_6168;
        mUnk_6130->mUnk_1ba = mUnk_616a;
    }
    if (InputManager::Get().IsHeld(0x400)) {
        mUnk_6195 = (mUnk_6195 + 1) & 1;
        mUnk_6130->func_ov007_02254928(mUnk_6195);
    }
    return 0;
}

void BattleShoot::func_ov007_0226d690() {
    if (mUnk_6130->mUnk_1e5 != 0) {
        mUnk_6180               = 0;
        mUnk_6130->mUnk_1e5 = 0;
    }
    if (InputManager::Get().IsDown(0x200) | InputManager::Get().IsDown(0x100)) {
        if (mUnk_6180 < 0x1e) {
            mUnk_6180++;
        }
    } else {
        if (mUnk_6180 != 0) {
            mUnk_6180--;
        }
    }
    u16 value            = mUnk_6180;
    s32 spread           = value * 0x1800;
    BattleTarget *target = mUnk_6130;
    target->mUnk_188.x   = spread;
    target->mUnk_188.y   = value << 12;
    target->mUnk_188.z   = spread;
}

extern "C" void func_01ffd138(MtxFx43 *mtx, s32 angle);
extern const u8 data_ov007_0227bc9c[];
extern const VecFx32 data_ov007_0227bcac;

BOOL BattleShoot::func_ov007_0226d7ac() {
    if (InputManager::Get().IsDown(0x200) | InputManager::Get().IsDown(0x100)) {
        if (mUnk_617e < 0x14) {
            mUnk_617e++;
        }
    } else {
        if (mUnk_617e != 0) {
            mUnk_617e--;
        }
    }
    if (mUnk_617e != 0) {
        mUnk_6176 = 0;
        mUnk_6174 = 0;
        mUnk_6130->mUnk_1e4 = 1;
        if (InputManager::Get().IsDown(0x20)) {
            mUnk_6178 -= mUnk_617a;
            mUnk_617c++;
            if (mUnk_617c == 0) {
                mUnk_617c = 0xffff;
            }
        }
        if (InputManager::Get().IsDown(0x10)) {
            mUnk_6178 += mUnk_617a;
            mUnk_617c++;
            if (mUnk_617c == 0) {
                mUnk_617c = 0xffff;
            }
        }
        if (KeysReleased(InputManager::Get(), 0x20) | KeysReleased(InputManager::Get(), 0x10)) {
            mUnk_617c = 0;
        }
        if (mUnk_617c >= 0x19) {
            mUnk_617a = (mUnk_617c - 0x19) / 2 * 0x12 + 0x5b;
            if (mUnk_617a > 0x12 + 0xff) {
                mUnk_617a = 0x12 + 0xff;
            }
        } else {
            mUnk_617a = 0x5b;
        }
        mUnk_6130->mUnk_1b0 = mUnk_6178;
        mUnk_6130->mUnk_17c = mUnk_617e - 1;
        func_ov007_0226f604();
        return 1;
    }
    return 0;
}

extern const s32 data_ov007_0227bcc4[3];
struct BattleShootRange { // at data_ov007_0227bcf8
    u32 mLow;
    u32 mHigh;
};
extern const BattleShootRange data_ov007_0227bcf8[3];
extern MtxFx33 data_0220e848;
extern "C" void func_01ffcfd8(MtxFx33 *m, u16 angle);
extern "C" void func_01ff84d0(VecFx32 *v, MtxFx33 *m, VecFx32 *out);

void BattleShoot::func_ov007_0226daa8() {
    mUnk_617c = 0;
    mUnk_6130->mUnk_1e4 = 0;
    if (InputManager::Get().IsDown(0x20)) {
        if ((u16) (mUnk_6168 + 0x8000 - mUnk_616c) > 0x6aab) {
            mUnk_6168 = mUnk_6168 - mUnk_616c;
            mUnk_6174++;
            if (mUnk_6174 == 0) {
                mUnk_6174 = 0xffff;
            }
        } else {
            mUnk_6168 = 0xeaab;
        }
    } else if (InputManager::Get().IsDown(0x10)) {
        if ((u16) (mUnk_616c + (mUnk_6168 + 0x8000)) < 0x9555) {
            mUnk_6168 += mUnk_616c;
            mUnk_6174++;
            if (mUnk_6174 == 0) {
                mUnk_6174 = 0xffff;
            }
        } else {
            mUnk_6168 = 0x1555;
        }
    }
    if (KeysReleased(InputManager::Get(), 0x20) | KeysReleased(InputManager::Get(), 0x10)) {
        mUnk_6174 = 0;
    }
    if (InputManager::Get().IsDown(0x40)) {
        if ((u16) (mUnk_616a + 0x8000 - mUnk_6170) > 0x8000) {
            mUnk_616a = mUnk_616a - mUnk_6170;
            mUnk_6176++;
            if (mUnk_6176 == 0) {
                mUnk_6176 = 0xffff;
            }
        } else {
            mUnk_616a = 0;
        }
    } else if (InputManager::Get().IsDown(0x80)) {
        if ((u16) (mUnk_6170 + (mUnk_616a + 0x8000)) < 0xaaaa) {
            mUnk_616a += mUnk_6170;
            mUnk_6176++;
            if (mUnk_6176 == 0) {
                mUnk_6176 = 0xffff;
            }
        } else {
            mUnk_616a = 0x2aaa;
        }
    }
    if (KeysReleased(InputManager::Get(), 0x40) | KeysReleased(InputManager::Get(), 0x80)) {
        mUnk_6176 = 0;
    }
    if (mUnk_6174 >= 10) {
        mUnk_616c = (mUnk_6174 - 10) / 2 * 0x24 + 0x5b;
        if (mUnk_616c > 0x1c7) {
            mUnk_616c = 0x1c7;
        }
    } else {
        mUnk_616c = 0x5b;
    }
    if (mUnk_6176 >= 10) {
        mUnk_6170 = (mUnk_6176 - 10) * 0x24 + 0xb6;
        if (mUnk_6170 > 0x222) {
            mUnk_6170 = 0x222;
        }
    } else {
        mUnk_6170 = 0xb6;
    }
}

class MicManager { // the microphone singleton of Game.cpp and BattlePlayerList.cpp (instance 0x020c189c)
public:
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u8 mActive;
    /* 21 */ PAD(0x21, 0x24);
    /* 24 */ u8 mUnk_24;
    /* 25 */ PAD(0x25, 0x28);

    MicManager() {
        mActive = 0;
    }
    ~MicManager();

    static MicManager &Get() {
        static MicManager instance;
        return instance;
    }
};

extern const s32 data_ov007_0227bcb8[3];

#define INT_TO_FX(n) ((fx32) ((n) > 0 ? 0.5f + (float) ((n) << 12) : (float) ((n) << 12) - 0.5f))

u16 BattleShoot::func_ov007_0226df24(int a, int b) {
    VecFx32 v;
    v.x = INT_TO_FX(a);
    v.y = INT_TO_FX(b);
    v.z = 0;
    return VEC_Mag(&v) >> 12;
}

void BattleShoot::func_ov007_0226dfb8() {
    s32 diff = mUnk_618b - mUnk_618f;
    if (diff < 0) {
        diff = -diff;
    }
    u16 value = diff << 6;
    if (value > 0x31c7) {
        value = 0x31c7;
    }
    int angle;
    if (mUnk_618b < mUnk_618f) {
        angle = (s16) -value;
    } else {
        angle = (s16) value;
    }
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 curve = list->mPlayers[list->mUnk_28a]->mRecords[list->mPlayers[list->mUnk_28a]->mRecord].mUnk_02_3;
    mUnk_6156 = angle * data_ov007_0227bca0[curve].mUnk_00 / data_ov007_0227bca0[curve].mUnk_01 + 0x6000;
    mUnk_6158 = 0x38e;
    u8 power = mUnk_618a;
    if (power > 0x1e) {
        power = 0x1e;
    } else if (power < 0xa) {
        power = 0xa;
    }
    fx32 distance = func_ov007_0226df24((s16) (mUnk_618b - mUnk_618f), (s16) (mUnk_618c - mUnk_6190)) << 12;
    u8 limit = 0x50 - (power - 10) * 50 / 20;
    u32 result;
    int speed = (u16) (FX_Mul(distance, 0xa66) / 4096);
    if (limit < speed) {
        result = limit;
    } else {
        result = speed < 0x1e ? 0x1e : speed;
    }
    mUnk_6193 = result;
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228)) {
        return;
    }
    u8 index = data_020d433c.mBattlePlayers->mUnk_28a;
    data_020d433c.mBattlePlayers->mPlayers[index]->mUnk_ec = mUnk_6193;
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[index];
    player->mUnk_d4 = 0;
    player->mUnk_d8 = 0;
    player = data_020d433c.mBattlePlayers->mPlayers[index];
    player->mUnk_cc[0] = mUnk_6154;
    player->mUnk_cc[1] = mUnk_6156;
    player->mUnk_cc[2] = mUnk_6158;
    mUnk_6130->func_ov007_0225b6d0();
    if (mUnk_6198 == 0) {
        mUnk_6182 = 0x122;
    }
}

void BattleShoot::func_ov007_0226e234() {
    if (!mIcons[0].mAnim) {
        return;
    }
    if (mUnk_6188 < 0x10) {
        mUnk_6188++;
        func_ov007_0226f87c();
    }
    if (mUnk_6194 != 0) {
        if (mUnk_6189 == 0) {
            mUnk_6194 = 0;
        }
        return;
    }
    if (mUnk_6189 == 0) {
        return;
    }
    s32 dx = mUnk_618c - mUnk_6190;
    if (dx < 0) {
        dx = -dx;
    }
    s32 dy = mUnk_618b - mUnk_618f;
    if (dy < 0) {
        dy = -dy;
    }
    u8 i;
    int distance = (u16) (dy + dx);
    if (distance >= 0x20) {
        i = 0;
        do {
            if (mIcons[i].mUnk_0c == 0) {
                mIcons[i].mAnim->SetAnimation(0);
                mIcons[i].mUnk_04 = mUnk_618f;
                mIcons[i].mUnk_06 = mUnk_6190;
                mIcons[i].mUnk_0a = 0;
                mIcons[i].mUnk_0b = 0;
                mIcons[i].mUnk_08 = FX_Atan2Idx(-(mUnk_618b - mUnk_618f), mUnk_618c - mUnk_6190);
                mIcons[i].mUnk_0c = distance > 0x20 ? 0x20 : distance;
                mUnk_6194         = 1;
                break;
            }
            i++;
        } while (i < 5);
        if (mUnk_619c != 0) {
            mUnk_619c--;
        }
    }
}

u16 BattleShoot::func_ov007_0226e390() {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 index               = list->mUnk_28a;
    VecFx32 cpuPos         = list->mPlayers[index]->mUnk_c8->mPos;
    VecFx32 *pos = &mUnk_6130->mUnk_1a4->mPosition;
    VecFx32 delta;
    VecFx32 offset = {0, 0, 0};
    VEC_Subtract(&offset, pos, &offset);
    VEC_Subtract(&cpuPos, pos, &delta);
    u16 first = FX_Atan2Idx(offset.z, offset.x);
    u16 second = FX_Atan2Idx(delta.z, delta.x);
    return second - first;
}

void BattleShoot::func_ov007_0226e430() {
    VecFx32 v;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    BattleCpu *cpu         = list->mPlayers[list->mUnk_28a]->mUnk_c8;
    if (cpu == 0) {
        return;
    }
    if (!(cpu->mUnk_77 == 0 || cpu->mUnk_77 == 1 || cpu->mUnk_77 == 2)) {
        return;
    }
    if (cpu->mUnk_58 != 0) {
        if (cpu->mUnk_58 <= mUnk_6130->mUnk_4c) {
            return;
        }
    }
    if (cpu->mUnk_74 != 0) {
        cpu->mUnk_74--;
        return;
    }
    if (cpu->mUnk_76 == 1) {
        BattleTarget *bt = mUnk_6130;
        if (bt->mUnk_1dd == 4 && bt->mUnk_38 == 0 && bt->mUnk_40 == 0) {
            cpu->func_ov007_02232bfc();
        }
        if (mUnk_6130->mUnk_4c == cpu->mUnk_54) {
            cpu->func_ov007_02232c5c();
        }
    }
    BattleTarget *target = mUnk_6130;
    if (target->mUnk_1d0 == 0 || target->mUnk_1dd == 4) {
        return;
    }
    if (target->mUnk_4c % mUnk_619e != 0) {
        return;
    }
    UnkStruct_0203d374 *pose = target->mUnk_1a4;
    VecFx32 pos2             = cpu->mPos;
    VEC_Subtract(&pos2, &pose->mPosition, &v);
    VEC_Normalize(&v, &v);
    if (data_020d433c.mUnk_85 != 0) {
        v.x = 0;
        v.y = 0;
        v.z = 0;
    } else {
        s32 k = data_ov007_0227bcc4[cpu->mUnk_77];
        v.x   = FX_Mul(v.x, k);
        v.y   = FX_Mul(v.y, k);
        v.z   = FX_Mul(v.z, k);
    }
    if (mUnk_6130->mUnk_1e3 != 0) {
        MtxFx33 m = data_0220e848;
        func_01ffcfd8(&m, 0x5555);
        func_01ff84d0(&v, &m, &v);
        v.x = FX_Mul(v.x, 0xa000);
        v.y = FX_Mul(v.y, 0xa000);
        v.z = FX_Mul(v.z, 0xa000);
        mUnk_6130->mUnk_1e3 = 0;
    }
    if (mUnk_6130->mUnk_1d8 == 6) {
        if (VEC_Mag(&mUnk_6130->mUnk_64) > 0) {
            BattleTarget *t = mUnk_6130;
            v.x             = t->mUnk_64.x;
            v.y             = t->mUnk_64.y;
            v.z             = t->mUnk_64.z;
            VEC_Normalize(&v, &v);
            s32 k = data_ov007_0227bcc4[cpu->mUnk_77];
            v.x   = FX_Mul(v.x, k);
            v.y   = FX_Mul(v.y, k);
            v.z   = FX_Mul(v.z, k);
        }
    }
    mUnk_6130->func_ov007_0225c2e4(&v);
    if (mUnk_6164 % 5 == 0) {
        s32 out[2];
        mUnk_6130->func_ov007_02255c04(&v, out);
        func_ov007_0226f0e8(data_020d433c.mBattlePlayers->mUnk_28a, out[0], out[1]);
    }
    u8 roll = MATH_Rand32(&data_020d42e8, (u8) (data_ov007_0227bcf8[cpu->mUnk_77].mHigh - data_ov007_0227bcf8[cpu->mUnk_77].mLow));
    mUnk_619e = roll + data_ov007_0227bcf8[cpu->mUnk_77].mLow;
    mUnk_6164++;
}

extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y);

struct Bytes2 { u8 b[2]; };

void BattleShoot::func_ov007_0226e884() {
    s32 animState;
    s32 sumX;
    s32 sumZ;
    s32 power;
    s32 spread;
    u8 charged;
    u8 sent;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 self                = list->mUnk_28a;
    data_0220f540.mUnk_00  = 9;
    data_0220f540.mUnk_04  = 0;
    *(s32 *) &data_0220f540.mUnk_08 = 0;
    data_0220f540.mUnk_0c  = 0;
    if (MicManager::Get().mUnk_24 != 0) {
        if (data_020d433c.mUnk_9a_0) {
            data_0220f540.mUnk_0c = 1;
        }
    } else {
        data_0220f540.mUnk_0c = 0;
    }
    if (mUnk_618a != 0) {
        if (mUnk_6189 == 0) {
            s32 dy = mUnk_618c - mUnk_6190;
            if (dy < 0) {
                dy = -dy;
            }
            s32 dx = mUnk_618b - mUnk_618f;
            if (dx < 0) {
                dx = -dx;
            }
            s32 distance = (u16) (dx + dy);
            if (distance >= 0x20) {
                data_0220f540.mUnk_00 = 9;
                s32 idx               = FX_Atan2Idx(-mUnk_615c, mUnk_6160) >> 4;
                data_0220f540.mUnk_04 = -FX_Mul(FX_SinCosTable_[idx * 2], 0x4cd);
                *(s32 *) &data_0220f540.mUnk_08 = -FX_Mul(FX_SinCosTable_[idx * 2 + 1], 0x4cd);
            }
            mUnk_615c = 0;
            mUnk_6160 = 0;
        } else {
            u8 prevX;
            u8 prevY;
            u8 curY;
            u8 curX;
            curX  = mUnk_618f;
            prevX = mUnk_618d;
            curY  = mUnk_6190;
            prevY = mUnk_618e;
            if ((prevX != curX) | (prevY != curY)) {
                mUnk_615c += (s16) (curX - prevX);
                mUnk_6160 += (s16) (curY - prevY);
            }
        }
    }
    const u8 *mode = &data_020c4258;
    u8 i;
    animState = 0;
    i = 0;
    do {
        BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[i];
        if (i == data_020d433c.mBattlePlayers->mUnk_2a9) {
            WirelessManager::Get();
            if (!IsWirelessMode(mode)) {
                mMsgs[i].mUnk_00 = ((u8 *) &data_0220f540)[0];
                mMsgs[i].mUnk_01 = ((u8 *) &data_0220f540)[1];
                *(Bytes2 *) &mMsgs[i].mUnk_02 = *(Bytes2 *) (((u8 *) &data_0220f540) + 2);
                mMsgs[i].mWords = *(ShootWords7 *) ((u8 *) &data_0220f540 + 4);
            } else {
                mMsgs[i].mUnk_00 = ((u8 *) p + 0x100)[0];
                mMsgs[i].mUnk_01 = ((u8 *) p + 0x100)[1];
                *(Bytes2 *) &mMsgs[i].mUnk_02 = *(Bytes2 *) (((u8 *) p + 0x100) + 2);
                mMsgs[i].mWords = *(ShootWords7 *) ((u8 *) p + 0x100 + 4);
            }
        } else {
            mMsgs[i].mUnk_00 = ((u8 *) p + 0x100)[0];
                mMsgs[i].mUnk_01 = ((u8 *) p + 0x100)[1];
                *(Bytes2 *) &mMsgs[i].mUnk_02 = *(Bytes2 *) (((u8 *) p + 0x100) + 2);
                mMsgs[i].mWords = *(ShootWords7 *) ((u8 *) p + 0x100 + 4);
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    i = 0;
    do {
        BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[i];
        BattleCpu *cpu  = p->mUnk_c8;
        if (cpu != 0 && self != i && self != p->mUnk_f2) {
            s32 outX = 0;
            s32 outZ = 0;
            if (mUnk_6130->mUnk_4c <= cpu->mUnk_5c) {
                cpu->func_ov007_02232eb8(&outX, &outZ);
                mMsgs[i].mUnk_00        = 9;
                mMsgs[i].mUnk_04        = outX;
                mMsgs[i].mUnk_08        = outZ;
                mMsgs[i].mUnk_0c        = 0;
                u16 limit = cpu->mUnk_60[cpu->mUnk_8d];
                cpu->mUnk_70++;
                if (cpu->mUnk_70 >= limit) {
                    cpu->mUnk_70 = 0;
                    cpu->mUnk_8d = (cpu->mUnk_8d + 1) % 8;
                    cpu->func_ov007_02232d8c();
                }
            }
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    BattlePlayer *me = data_020d433c.mBattlePlayers->mPlayers[self];
    if (me->mUnk_c8 != 0) {
        i = 0;
        do {
            if (data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_c8 == 0) {
                if (mMsgs[i].mUnk_04 != 0) {
                    mMsgs[i].mUnk_04 = FX_Mul(mMsgs[i].mUnk_04, data_ov007_0227bcb8[me->mUnk_f7]);
                }
                if (mMsgs[i].mUnk_08 != 0) {
                    mMsgs[i].mUnk_08 = FX_Mul(mMsgs[i].mUnk_08, data_ov007_0227bcb8[me->mUnk_f7]);
                }
            }
            i++;
        } while (i < data_020d433c.mPlayerCount);
    }
    sent    = 0;
    sumX    = 0;
    sumZ    = 0;
    charged = 0;
    power   = data_020d433c.mBattlePlayers->mPlayers[self]->mUnk_dc;
    spread  = data_020d433c.mBattlePlayers->mPlayers[self]->mUnk_e4;
    if (!(mUnk_6130->mUnk_1dd == 4 && mUnk_6130->mUnk_38 <= 0x1e)) {
        i = 0;
        do {
            s32 msgX;
            s32 msgZ;
            BattleShootMsg *m = &mMsgs[i];
            if (m->mUnk_00 == 9) {
                msgX = m->mUnk_04;
                msgZ = m->mUnk_08;
                if (self == i) {
                    sumX += msgX;
                    sumZ += msgZ;
                    if (!(m->mUnk_0c != 0 && mUnk_6130->mUnk_1cc != 1)) {
                    } else {
                        animState = 1;
                        charged   = 1;
                        power += 0x19a;
                    }
                } else if (!(mUnk_6130->mUnk_1df == 5 || mUnk_6130->mUnk_1cc == 1)) {
                    u8 count = mUnk_61a9;
                    sumX += msgX * 30 / 100 / count;
                    sumZ += msgZ * 30 / 100 / count;
                    if (m->mUnk_0c != 0) {
                        animState = 1;
                        spread += 0x7b / count;
                        charged = 1;
                    }
                }
                sent = 1;
                if (self == i || !(mUnk_6130->mUnk_1df == 5 || mUnk_6130->mUnk_1cc == 1)) {
                    func_ov007_0226f0e8(i, msgX, msgZ);
                }
            }
            i++;
        } while (i < data_020d433c.mPlayerCount);
    }
    if (power > 0x1333) {
        power = 0x1333;
    }
    if (spread > 0x1333) {
        spread = 0x1333;
    }
    if (charged == 0) {
        if (power > 0) {
            power -= 0x800;
            if (power < 0) {
                power = 0;
            }
        }
        if (spread > 0) {
            spread -= 0x800;
            if (spread < 0) {
                spread = 0;
            }
        }
    }
    data_020d433c.mBattlePlayers->mPlayers[self]->mUnk_dc = power;
    data_020d433c.mBattlePlayers->mPlayers[self]->mUnk_e4 = spread;
    if (sent == 1) {
        BattlePlayer *pl = data_020d433c.mBattlePlayers->mPlayers[self];
        pl->mUnk_d4 = sumX;
        pl->mUnk_d8 = sumZ;
        mUnk_6130->func_ov007_0225bf38();
    }
    if (data_020d433c.mUnk_9a_0 && data_020d433c.mUnk_83 == 0) {
        if (mUnk_61a5 != animState) {
            mUnk_61a5 = animState;
            if (mUnk_61a5) {
                mCells[4].mAnim->SetAnimation(2);
            } else {
                mCells[4].mAnim->SetAnimation(0);
            }
        }
    }
}

extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y);

void BattleShoot::func_ov007_0226f0e8(u8 kind, s32 x, s32 z) {
    if ((x | z) == 0) {
        return;
    }
    u8 handle = func_0202bd40(data_020d433c.mEffects, 0, data_ov007_0227bc9c[kind], 0, 1);
    if (handle == 0) {
        return;
    }
    SoundManager::Get().func_020246f8(0x43);
    VecFx32 pos = mUnk_6130->mUnk_1a4->mPosition;
    VecFx32 off;
    VecFx32 dir = {0, 0, 0};
    VecFx32 ref = data_ov007_0227bcac;
    dir.x       = x;
    dir.z       = z;
    VEC_Normalize(&dir, &dir);
    fx32 dot    = VEC_DotProduct(&dir, &ref);
    off.x       = 0;
    off.y       = 0;
    off.z       = 0xa000;
    s32 angle   = z >= 0 ? 0xc000 : 0x4000;
    if ((dot < 0x800) & (dot > -0x800)) {
        if (x > 0) {
            off.x = -0x5000;
            angle = 0x8000;
        } else if (x < 0) {
            off.x = 0x5000;
            angle = 0;
        }
    }
    CameraManager::Get();
    MtxFx43 mtx = data_020c1cf4;
    mtx._[9]  = 0;
    mtx._[10] = 0;
    mtx._[11] = 0;
    MTX_MultVec43(&off, &mtx, &off);
    VEC_Add(&pos, &off, &pos);
    if (angle != 0) {
        MtxFx43 rot = data_0220e86c;
        func_01ffd138(&rot, angle);
        func_01ff89c0(&rot, &mtx, &mtx);
    }
    func_01ffcf64(&mtx, &pos);
    func_0202bf60(data_020d433c.mEffects, handle, &mtx);
}

void BattleShoot::func_ov007_0226f378() {
    if (mUnk_6138) {
        u16 id = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_28a]->mUnk_fa;
        if (mUnk_6184 != id) {
            if (mUnk_619a == 0x80) {
                mUnk_6184 = id;
                mUnk_6138->func_02065890((s16) id, 0, 0);
                mUnk_619b = 0x60;
            } else if (mUnk_619a == 0x60) {
                mUnk_619b = 0x80;
            }
        }
        if (mUnk_619a != mUnk_619b) {
            if (mUnk_619a < mUnk_619b) {
                mUnk_619a += 4;
            } else {
                mUnk_619a -= 4;
            }
        }
        if (mUnk_6184 != 0) {
            mCells[6].mAnim->Draw(1, 0x80, mUnk_619a, 2);
            mUnk_6138->func_02065bd4(0x20, (s16) (mUnk_619a + 0x42), 0);
            mUnk_6138->func_020658d0();
            mCells[5].mAnim->Draw(1, 0x80, mUnk_619a, 2);
        }
    }
    if (mCells[0].mAnim) {
        mCells[0].mAnim->Update();
        mCells[0].mAnim->Draw(1, (s16) (mCells[0].mX + 0x10), (s16) (mCells[0].mY + 0x10), mCells[0].mUnk_08);
        if (mUnk_6197 != 0 && mUnk_6196 != 0) {
            mCells[2].mX = (mUnk_6182 + mCells[2].mX) / 2;
            mCells[2].mAnim->Update();
            mCells[2].mAnim->Draw(1, mCells[2].mX, mCells[2].mY, mCells[2].mUnk_08);
            if (mUnk_6198 != 0) {
                mCells[1].mAnim->Update();
                mCells[1].mAnim->Draw(1, mCells[0].mX, mCells[0].mY, mCells[0].mUnk_08);
            }
        }
    }
}
void BattleShoot::func_ov007_0226f59c() {
    mCells[4].mAnim->Update();
    mCells[4].mAnim->Draw(1, mCells[4].mX, mCells[4].mY, mCells[4].mUnk_08);
    mCells[4].mY = (mCells[4].mY * 3 + mUnk_61a6) / 4;
}

void BattleShoot::func_ov007_0226f604() {
    if (data_020d433c.mBattlePlayers->mUnk_28a == data_020d433c.mBattlePlayers->mUnk_2a9) {
        MI_CpuClearFast(mBitmap, 0x6000);
        func_ov007_0226f644();
    }
}

void BattleShoot::func_ov007_0226f644() {
    if (mUnk_61a1 != 0) {
        DC_FlushRange(mBitmap, 0x6000);
        func_02025c1c(1, 0xd, 0, mBitmap, 0x6000, 0);
    }
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

void BattleShoot::func_ov007_0226f698(u8 x0, u8 y0, u8 x1, u8 y1) {
    u8 steps;
    u8 i;
    if (mUnk_61a1 == 0) {
        return;
    }
    if (data_020d433c.mBattlePlayers->mUnk_28a != data_020d433c.mBattlePlayers->mUnk_2a9) {
        return;
    }
    steps = ABS(x0 - x1) >= ABS(y0 - y1) ? ABS(x0 - x1) : ABS(y0 - y1);
    i = 0;
    do {
        s32 a = (i * (x0 - x1)) / steps;
        s32 b = (i * (y0 - y1)) / steps;
        func_ov007_0226f76c(x0 - a, y0 - b);
        i++;
    } while (i < steps);
    func_ov007_0226f644();
}

void BattleShoot::func_ov007_0226f76c(u8 x, u8 y) {
    s16 px;
    s16 py;
    u8 i = 0;
    do {
        px = x + data_ov007_0227bce0[i][0];
        py = y + data_ov007_0227bce0[i][1];
        BOOL nonNegative = (px >= 0) & (py >= 0);
        if (nonNegative & ((u32) px < 0x100) & ((u32) py < 0xc0)) {
            func_ov007_0226f820(px, py);
        }
        i++;
    } while (i < 12);
}

void BattleShoot::func_ov007_0226f820(u8 x, u8 y) {
    if (y >= 0xc0) {
        return;
    }
    u32 index = (x / 8) * 64 + x % 8 + ((y / 8) * 2048 + (y % 8) * 8);
    mBitmap[index >> 1] |= (index & 1) ? 0x20 : 2;
}

void BattleShoot::func_ov007_0226f87c() {
    u8 value = mUnk_6188;
    G2S_SetBlendAlpha(2, 0x31, 0x10 - value, value);
}

void BattleShoot::func_ov007_0226f8a8() {
    if (!mIcons[0].mAnim) {
        return;
    }
    if (mUnk_619c != 0) {
        mCells[3].mAnim->Update();
        mCells[3].mAnim->Draw(1, mCells[3].mX, mCells[3].mY, mCells[3].mUnk_08);
        if (mUnk_619d != 0) {
            func_ov007_0226cad8();
        }
    }
    u8 i = 0;
    do {
        if (mIcons[i].mUnk_0c != 0) {
            mIcons[i].mUnk_0c--;
            mIcons[i].mAnim->Update();
            mIcons[i].mAnim->func_0202a494(1, mIcons[i].mUnk_04, mIcons[i].mUnk_06, 2, 0x1000, 0x1000, mIcons[i].mUnk_08);
            if (mIcons[i].mAnim->mUnk_12 != 0) {
                mIcons[i].mUnk_0c = 0;
            } else {
                mIcons[i].mUnk_04 += mIcons[i].mUnk_0a;
                mIcons[i].mUnk_06 += mIcons[i].mUnk_0b;
            }
        }
        i++;
    } while (i < 5);
}

BOOL BattleShoot::func_ov007_0226f9e8() {
    u8 ok = 1;
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228) || data_020d433c.mUnk_83 != 0) {
        ok = 0;
    }
    if (mUnk_6130->mUnk_1eb == 0) {
        ok = 0;
    }
    if (ok) {
        if (InputManager::Get().IsHeld(8)) {
            mUnk_61a8 = mUnk_6186;
            mUnk_6186 = 0xd;
            data_020d433c.mUnk_14 |= 8;
            mUnk_61a1 = 0;
            return 1;
        }
    }
    return 0;
}

void BattleShoot::func_ov007_0226fb2c(u8 value) {
    mUnk_61a3 = value;
    func_020254f0(func_ov007_0226fb50, this);
}

void func_ov007_0226fb50(void *arg) {
    GXS_SetVisiblePlane(((BattleShoot *) arg)->mUnk_61a3);
}

