#include "global.h"
#include "types.h"
#include "Scene/SceneBase.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/BattleBase.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Unknown/UnkStruct_02078290.hpp"
#include <nitro/os.h>
#include <stdio.h>

// 0x1c byte object created by this class (ctor 02271410), not kept.
class BattleCardEffect {
public:
    u8 mPad[0x1c];
    BattleCardEffect(u32 a, u32 b, u32 c, u32 d);
};

// Model file loaded from a name and a texture (the pair of calls after the base constructor).
class BattleCharaActModel : public UnkStruct_020289c8 {
public:
    BattleCharaActModel(const char *name, UnkStruct_02028308 *texture) {
        func_020289e0(name, texture);
    }
};

// A character (story / edited) shown on the battle field with its own camera path (vtable 0228a2d8, 0x40 bytes).
// Path strings: /Game/Character/Act, /Game/Character/Character, /Game/Character/EditedCharacter/Cam.
class BattleCharaAct : public SceneBase {
public:
    /* 0c */ Camera *mUnk_0c;
    /* 10 */ Camera *mUnk_10;
    /* 14 */ void *mUnk_14;
    /* 18 */ BattleMapObj *mUnk_18;
    /* 1c */ UnkStruct_0205401c *mUnk_1c;
    /* 20 */ UnkStruct_02029ca8 *mUnk_20;
    /* 24 */ ModelInstance *mUnk_24;
    /* 28 */ ModelInstance *mUnk_28;
    /* 2c */ u32 mUnk_2c;
    /* 30 */ PAD(0x30, 0x32);
    /* 32 */ u16 mUnk_32;
    /* 34 */ PAD(0x34, 0x35);
    /* 35 */ u8 mUnk_35;
    /* 36 */ u8 mUnk_36;
    /* 37 */ u8 mUnk_37;
    /* 38 */ u8 mUnk_38;
    /* 39 */ u8 mUnk_39;
    /* 3a */ u8 mUnk_3a;
    /* 3b */ u8 mUnk_3b;
    /* 3c */ u8 mUnk_3c;
    /* 3d */ u8 mUnk_3d;
    /* 3e */ PAD(0x3e, 0x40);
    /* 40 */

    BattleCharaAct(Camera *camera, u8 player, u32 kind, u8 unk4, u8 unk5);
    virtual ~BattleCharaAct();
    /* 08 */ virtual void Update();
    void func_ov007_02267a8c();
    u8 func_ov007_02268110();
    void func_ov007_02268190();
    void func_ov007_022684f0();
    void func_ov007_02269380();
    void func_ov007_022693f4();
    void func_ov007_02269518();
    void func_ov007_022695f4();
    void func_ov007_0226961c();
};

extern const u8 data_ov007_0227af68[][0x28];
extern VecFx32 data_ov007_0227ac20;
extern VecFx32 data_ov007_0227ac2c;
extern const char *data_ov007_0228a394[22];


BattleCharaAct::BattleCharaAct(Camera *camera, u8 player, u32 kind, u8 unk4, u8 unk5)
    : SceneBase(0x3e8, 0) {
    mUnk_0c = camera;
    mUnk_10 = 0;
    mUnk_14 = 0;
    mUnk_18 = 0;
    mUnk_28 = 0;
    mUnk_35 = 0;
    u8 k    = kind;
    mUnk_36 = k;
    mUnk_37 = k;
    mUnk_39 = player;
    mUnk_3a = 0;
    mUnk_3b = unk4;
    mUnk_3c = unk5;
    func_ov007_02267a8c();
}

void BattleCharaAct::func_ov007_02267a8c() {
    mUnk_32 = 0;
    mUnk_3d = 0;
    mUnk_38 = data_020d433c.mBattlePlayers->mPlayers[mUnk_39]->mUnk_f6;
    if (mUnk_36 >= 0x10) {
        mUnk_36 = 10;
    }
    switch (mUnk_36) {
    case 0:
        SpriteManager::Get().func_02026dec(1, 7);
        mUnk_10 = new Camera();
        mUnk_10->func_02026004(0x1555);
        mUnk_10->SetClip(0x1000, 0x1388000);
        mUnk_10->SetPosition(0, 0, 0x22c000 - 0x1000);
        mUnk_10->SetTarget(0, 0, 0);
        CameraManager::Get().func_02026504(3, mUnk_0c, (u32) mUnk_10);
        break;
    case 5:
    case 6:
        SpriteManager::Get().func_0202740c();
        CameraManager::Get().mActive = 0;
        CameraManager::Get().func_02026504(1, mUnk_0c, 0);
        break;
    default:
        SpriteManager::Get().func_02026dec(1, 7);
        CameraManager::Get().func_02026504(2, mUnk_0c, 0);
        break;
    }
    mUnk_24 = 0;
    func_ov007_02268190();
    func_ov007_022684f0();
    mUnk_2c = func_ov007_02268110();
    u8 n    = mUnk_36 + 1;
    char buffer[0x20];
    if (mUnk_2c == 1) {
        ChangeDir("/Game/Character/EditedCharacter/Cam/");
        sprintf(buffer, "edit_M_M%02d.cam", n);
    } else {
        ChangeDir("/Game/Character/Character/");
        sprintf(buffer, "chara%02d_M_%02d.cam", mUnk_38, n);
    }
    mUnk_14 = func_02023504(buffer, 0, 0, 0, 0, 0);
    mUnk_0c->func_0202610c(mUnk_14, 0);
    ChangeDir("/");
    mUnk_20 = new UnkStruct_02029ca8(data_ov007_0227af68[data_020d433c.mUnk_95]);
}

BattleCharaAct::~BattleCharaAct() {
    UnkStruct_02029ca8 *lights = mUnk_20;
    if (lights) {
        delete lights;
    }
    delete mUnk_10;
    CameraManager::Get().func_02026504(0, 0, 0);
    UnkStruct_0205401c *character = mUnk_1c;
    if (character) {
        character->func_02054034();
        delete character;
    }
    func_ov007_022695f4();
    ModelInstance *model = mUnk_24;
    if (model) {
        model->mUnk_68 = 0xf;
        delete model;
        mUnk_24 = 0;
    }
    if (mUnk_14) {
        OS_Free(mUnk_14);
    }
    BattleMapObj *field = mUnk_18;
    if (field) {
        delete field;
    }
}

void BattleCharaAct::func_ov007_02268190() {
    char modelName[0x20];
    char textureName[0x20];
    char animName[0x20];
    char paletteA[0x20];
    char paletteB[0x20];
    u32 loaded       = 0;
    u32 paletteAKind = 0;
    u32 paletteBKind = 0;
    ChangeDir("/Game/Character/Act/");
    switch (mUnk_36) {
        case 0:
            if (mUnk_36 == 0) {
                func_ov007_02269518();
            }
        case 3:
        case 15:
            if (mUnk_3b != 0) {
                paletteAKind = 1;
                sprintf(paletteA, "T_sphe_%02d", mUnk_3b);
            }
        case 1:
        case 6:
            sprintf(modelName, "%s%02d.nsbmd", data_ov007_0228a394[mUnk_38], mUnk_36 + 1);
            sprintf(textureName, "%s%02d.nsbtx", data_ov007_0228a394[mUnk_38], mUnk_36 + 1);
            sprintf(animName, "%sM_%02d_data.bin", data_ov007_0228a394[mUnk_38], mUnk_36 + 1);
            loaded = 1;
            if ((mUnk_36 == 1) | (mUnk_36 == 6)) {
                if ((mUnk_3b != 0) & (mUnk_3b <= 2)) {
                    paletteAKind = 2;
                    sprintf(paletteA, "ab_%02d", mUnk_3b);
                }
            }
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            if (mUnk_3b != 0) {
                paletteAKind = 1;
                sprintf(paletteA, "T_sphe_%02d", mUnk_3b);
            }
            if (mUnk_36 == 8 && mUnk_3c != 1) {
                paletteBKind = 1;
                sprintf(paletteB, "T_sphe_%02d", mUnk_3c);
            }
            sprintf(modelName, "all_%02d.nsbmd", mUnk_36 + 1);
            sprintf(textureName, "all_%02d.nsbtx", mUnk_36 + 1);
            sprintf(animName, "all_M_%02d_data.bin", mUnk_36 + 1);
            loaded = 1;
            break;
    }
    if (loaded) {
        UnkStruct_02028308 *texture = new NitroTexture(textureName);
        if (paletteAKind == 1) {
            texture->func_02028858("S_sphe00", paletteA, 0);
        }
        if (paletteBKind == 1) {
            texture->func_02028858("S_sphe01", paletteB, 0);
        }
        if (paletteAKind == 2) {
            texture->func_02028858("ab_00", paletteA, 0);
        }
        UnkStruct_020289c8 *model = new BattleCharaActModel(modelName, texture);
        texture->func_02028344();
        ModelAnimData *data = new ModelAnimData(animName);
        ModelAnim *anim     = new ModelAnim(data, (NitroModel *) model);
        mUnk_24             = model->func_02028b78(anim);
        mUnk_24->func_02028e54(0, 0, 0x1000);
        mUnk_24->mUnk_69 = 0;
    }
    ChangeDir("/");
}

u8 BattleCharaAct::func_ov007_02268110() {
    mUnk_1c = new UnkStruct_0205401c();
    mUnk_1c->func_0205411c(mUnk_38, 1, data_020d433c.mBattlePlayers->mPlayers[mUnk_39]->NameInfo(), (u8) (mUnk_36 + 1));
    mUnk_1c->func_0205461c(mUnk_36, 0);
    return mUnk_1c->mUnk_0c;
}

void BattleCharaAct::func_ov007_022684f0() {
    switch (mUnk_36) {
        case 10:
            switch (mUnk_37 - 0x10) {
                case 0:
                    new BattleCardEffect(0, 0, 6, 0);
                    break;
                case 1:
                    new BattleCardEffect(0, 0, 7, 0);
                    break;
                case 2:
                    new BattleCardEffect(0, 0, 8, 0);
                    break;
                case 3:
                    new BattleCardEffect(0, 0, 9, 0);
                    break;
                default:
                    new BattleCardEffect(0, 0, 0xd, 0);
                    break;
            }
            break;
        case 11:
            new BattleCardEffect(0, 0, 0xc, 0);
            break;
        case 12:
            new BattleCardEffect(0, 0, 0xb, 0);
            break;
        case 13:
            new BattleCardEffect(0, 0, 5, 0);
            break;
        case 7:
        case 8:
        case 9:
            new BattleCardEffect(0, 0, 4, 0);
            break;
    }
}

void BattleCharaAct::func_ov007_02269380() {
    u8 type = 0xa;
    if (mUnk_36 != 0) {
        if (mUnk_36 == 0xf) {
            type = 0xb;
        }
    } else {
        type = 9;
    }
    mUnk_18 = new BattleMapObj(type);
    mUnk_18->func_ov007_022493fc();
    if (mUnk_36 != 5 && mUnk_36 != 6 && data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->func_020785d8(0xd);
    }
}

// non-matching: 16 words, register assignment only (original keeps the matrix copy in r4/lr and the vector copy in ip).
void BattleCharaAct::func_ov007_022693f4() {
    mUnk_0c->func_020264a4(0);
    mUnk_0c->func_02026180();
    mUnk_18->func_ov007_0224954c();
    mUnk_1c->func_02054568();
    if (!mUnk_24) {
        return;
    }
    MtxFx43 mtx = data_0220e86c;
    VecFx32 pos = data_ov007_0227ac20;
    VecFx32 zero = {0, 0, 0};
    BOOL draw   = 1;
    func_01ffcf64(&mtx, &zero);
    if ((mUnk_36 == 10) | ((mUnk_36 == 7) | ((mUnk_36 == 8) | (mUnk_36 == 9)))) {
        mUnk_3a++;
        if (mUnk_3a >= 0x16) {
            draw = 0;
        }
    }
    if (!draw) {
        return;
    }
    mUnk_24->func_02028d80();
    mUnk_24->func_02029174(&mtx, &pos);
}

void BattleCharaAct::func_ov007_02269518() {
    UnkStruct_02028308 *texture = new NitroTexture("fieldop_00.nsbtx");
    UnkStruct_020289c8 *model   = new BattleCharaActModel("fieldop_00.nsbmd", texture);
    texture->func_02028344();
    ModelAnimData *data = new ModelAnimData("fieldop_00_data.bin");
    ModelAnim *anim     = new ModelAnim(data, (NitroModel *) model);
    mUnk_28             = model->func_02028b78(anim);
    mUnk_28->func_02028de8(0, 1);
    mUnk_28->mUnk_69 = 0;
    mUnk_28->func_02028e54(0, 1, 0x2000);
}

void BattleCharaAct::func_ov007_022695f4() {
    ModelInstance *model = mUnk_28;
    if (model) {
        model->mUnk_68 = 0xf;
        delete model;
    }
}

void BattleCharaAct::func_ov007_0226961c() {
    if (!mUnk_28) {
        return;
    }
    MtxFx43 mtx = data_0220e86c;
    VecFx32 pos = data_ov007_0227ac2c;
    func_01ffd088(&mtx, 0x4000);
    mUnk_28->func_02028d80();
    mUnk_28->func_02029174(&mtx, &pos);
}

u32 func_ov007_022696a0(u32 a, u32 b) {
    if (a == 0x22 && b == 0) {
        b = 2;
    }
    return b;
}
