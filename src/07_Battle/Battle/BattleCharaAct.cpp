#include "global.h"
#include "Battle/BattleCardEffect.hpp"
#include "types.h"
#include "Battle/BattleCharaAct.hpp"
#include "Scene/SceneBase.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/BattleBase.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"
#include "System/FileSystem.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include "System/InputManager.hpp"
#include "System/WirelessManager.hpp"
#include "System/FrameTask.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Graphics/Camera.hpp"
#include <nitro/os.h>
#include <stdio.h>

// Model file loaded from a name and a texture (the pair of calls after the base constructor).
class BattleCharaActModel : public ModelFile {
public:
    BattleCharaActModel(const char *name, TextureFile *texture) {
        Load(name, texture);
    }
};

// The declaration order of these arrays and tables, and the order of the five singleton headers above, are
// load-bearing: mwcc size-sorts all of them together, and this order gives the original layout.
static const u16 data_ov007_0227ac58[16] = {0xeb, 0x11e, 0x1ee, 0xb9, 0x151, 0x10d, 0xa8, 0x172, 0xda, 0x140, 0xc9, 0x151, 0x12f, 0xfc, 0x1eb, 0x162};
static const u16 data_ov007_0227ac78[16] = {0xec, 0x11f, 0x1ee, 0xba, 0x152, 0x10e, 0xa9, 0x173, 0xdb, 0x141, 0xca, 0x152, 0x130, 0xfd, 0x1e5, 0x163};
static const u8 data_ov007_0227ae78[16] = {0x28, 0x28, 0x0, 0x28, 0x14, 0x1e, 0x28, 0x14, 0x28, 0x1e, 0x1e, 0x14, 0x1e, 0x14, 0x78, 0x28};
static const u16 data_ov007_0227ac98[16] = {0xed, 0x120, 0x1ee, 0xbb, 0x153, 0x10f, 0xaa, 0x174, 0xdc, 0x142, 0xcb, 0x153, 0x131, 0xfe, 0x1e7, 0x164};
static const u8 data_ov007_0227ae88[16] = {0x28, 0x28, 0x0, 0x28, 0x14, 0x1e, 0x28, 0x14, 0x1e, 0x1e, 0x1e, 0x14, 0x14, 0x14, 0x64, 0x3c};
static const u8 data_ov007_0227aea8[16] = {0x28, 0x28, 0x0, 0x2d, 0x14, 0x1e, 0x14, 0x0, 0x1e, 0x1e, 0x28, 0x14, 0x32, 0x0, 0x28, 0x28};
static const u16 data_ov007_0227acb8[16] = {0xef, 0x122, 0x1ee, 0x1ee, 0x155, 0x111, 0xac, 0x176, 0xde, 0x144, 0xcd, 0x155, 0x133, 0x100, 0x1dc, 0x1ee};
static const u8 data_ov007_0227aeb8[16] = {0x28, 0x28, 0x0, 0x28, 0x14, 0x1e, 0x28, 0x14, 0x1e, 0x1e, 0x1e, 0x14, 0x14, 0x14, 0x3c, 0x28};
static const VecFx32 sActModelScale = {0x1000, 0x1000, 0x1000};
static const u16 data_ov007_0227acd8[16] = {0xf0, 0x123, 0x1ee, 0xbd, 0x156, 0x112, 0xad, 0x177, 0xdf, 0x145, 0xce, 0x156, 0x134, 0x101, 0x1ed, 0x166};
static const u16 data_ov007_0227acf8[16] = {0xf1, 0x124, 0x1ee, 0xbe, 0x157, 0x113, 0xae, 0x178, 0xe0, 0x146, 0xcf, 0x157, 0x135, 0x102, 0x1ec, 0x167};
static const u16 data_ov007_0227ad18[16] = {0xf2, 0x125, 0x1ee, 0xbf, 0x158, 0x114, 0xaf, 0x179, 0xe1, 0x147, 0xd0, 0x158, 0x136, 0x103, 0x1e3, 0x168};
static const u16 data_ov007_0227ad38[16] = {0xf3, 0x126, 0x1ee, 0xc0, 0x159, 0x115, 0xb0, 0x17a, 0xe2, 0x148, 0xd1, 0x159, 0x137, 0x104, 0x1e8, 0x169};
static const u16 data_ov007_0227ad58[16] = {0xf4, 0x127, 0x1ee, 0xc1, 0x15a, 0x116, 0xb1, 0x17b, 0xe3, 0x149, 0xd2, 0x15a, 0x138, 0x105, 0x1e6, 0x16a};
static const u8 data_ov007_0227aee8[16] = {0x3c, 0x0, 0x0, 0x1e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x28, 0x0, 0x0, 0x0, 0x0, 0x50, 0x0};
static const u16 data_ov007_0227ad98[16] = {0xf6, 0x129, 0x1ee, 0xc3, 0x15c, 0x118, 0xb3, 0x17d, 0xe5, 0x14b, 0xd4, 0x15c, 0x13a, 0x107, 0x1de, 0x16c};
static const u8 data_ov007_0227aef8[16] = {0x37, 0x46, 0x0, 0x0, 0x0, 0x46, 0x3c, 0x0, 0x32, 0x0, 0x8c, 0x0, 0x0, 0x0, 0x82, 0x78};
static const u8 data_ov007_0227af18[16] = {0xa, 0x1e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1e, 0x0, 0x0, 0x0, 0x50, 0x0, 0x0, 0x5a};
static const u8 data_ov007_0227af28[16] = {0x4b, 0x50, 0x0, 0x50, 0x14, 0xa, 0x1e, 0x14, 0x0, 0x0, 0x5a, 0x1e, 0x0, 0x0, 0xa0, 0x0};
static const u16 data_ov007_0227adb8[16] = {0xf7, 0x12a, 0x1ee, 0xc4, 0x15d, 0x119, 0xb4, 0x17e, 0xe6, 0x14c, 0xd5, 0x15d, 0x13b, 0x108, 0x1e1, 0x16d};
static const u16 data_ov007_0227add8[16] = {0xf8, 0x12b, 0x1ee, 0xc5, 0x15e, 0x11a, 0xb5, 0x17f, 0xe7, 0x14d, 0xd6, 0x15e, 0x13c, 0x109, 0x1e4, 0x16e};
static const u8 data_ov007_0227af38[16] = {0x28, 0x50, 0x0, 0x5a, 0x1e, 0x3c, 0x3c, 0x0, 0x50, 0x32, 0x0, 0x1e, 0x0, 0x0, 0x0, 0x46};
static const u16 data_ov007_0227adf8[16] = {0xf9, 0x12c, 0x1ee, 0xc6, 0x15f, 0x11b, 0xb6, 0x180, 0xe8, 0x14e, 0xd7, 0x15f, 0x13d, 0x10a, 0x1e0, 0x16f};
static const char *sActFilePrefixes[22] = {
    "dan_",
    "runo_",
    "marucho_",
    "shun_",
    "julie_",
    NULL,
    "masquerade_",
    "rival_",
    "billy_",
    "klaus_",
    "chan_lee_",
    "julio_",
    "komba_",
    "shuji_",
    "akira_",
    "jewls_",
    "jenny_",
    NULL,
    NULL,
    NULL,
    NULL,
    "edit_",
};
static const u8 data_ov007_0227af58[16] = {0x5a, 0x2d, 0x0, 0x0, 0x14, 0x0, 0x19, 0x0, 0x0, 0x0, 0x0, 0x19, 0x64, 0x0, 0x2c, 0x0};
static const u16 data_ov007_0227ae18[16] = {0xfa, 0x12d, 0x1ee, 0xc7, 0x160, 0x11c, 0xb7, 0x181, 0xe9, 0x14f, 0xd8, 0x160, 0x13e, 0x10b, 0x1ea, 0x170};
static const u16 data_ov007_0227ac38[16] = {0xee, 0x121, 0x1ee, 0xbc, 0x154, 0x110, 0xab, 0x175, 0xdd, 0x143, 0xcc, 0x154, 0x132, 0xff, 0x1df, 0x165};
static const u8 data_ov007_0227ae68[16] = {0x28, 0x28, 0x0, 0x0, 0x14, 0x1e, 0x14, 0x0, 0x0, 0x0, 0x0, 0x14, 0x1e, 0x14, 0x0, 0x0};
static const u8 data_ov007_0227ae98[16] = {0x28, 0x28, 0x0, 0x2d, 0x14, 0x1e, 0x28, 0x14, 0x1e, 0x1e, 0x1e, 0x14, 0x14, 0xa, 0x28, 0x28};
static const u8 data_ov007_0227aec8[16] = {0x1e, 0x14, 0x0, 0x0, 0x0, 0x0, 0x14, 0x0, 0x0, 0x1e, 0x0, 0x0, 0x0, 0x0, 0x32, 0x28};
static const u16 data_ov007_0227ad78[16] = {0xf5, 0x128, 0x1ee, 0xc2, 0x15b, 0x117, 0xb2, 0x17c, 0xe4, 0x14a, 0xd3, 0x15b, 0x139, 0x106, 0x1e9, 0x16b};
static const u8 data_ov007_0227af08[16] = {0x32, 0x5a, 0x0, 0x0, 0x0, 0x64, 0x0, 0x0, 0x5a, 0x0, 0x0, 0x0, 0x46, 0x0, 0x32, 0x0};
static const u8 data_ov007_0227af48[16] = {0x28, 0x46, 0x0, 0x1e, 0x0, 0x1e, 0x3c, 0x3c, 0x0, 0x0, 0x0, 0x0, 0x46, 0x0, 0x32, 0x46};
static const u16 data_ov007_0227ae38[16] = {0xfb, 0x12e, 0x1ee, 0xc8, 0x161, 0x11d, 0xb8, 0x182, 0xea, 0x150, 0xd9, 0x161, 0x13f, 0x10c, 0x1e2, 0x171};
static const u8 data_ov007_0227ae58[16] = {0x1e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1e, 0x28, 0x0, 0x28, 0x0, 0x14, 0x28};
static const u8 data_ov007_0227aed8[16] = {0x46, 0x50, 0x0, 0x0, 0x0, 0x0, 0x32, 0x0, 0x14, 0x1e, 0x28, 0x0, 0x0, 0x0, 0x64, 0xbe};
static const u8 *sActFrameTaskFrameCounts[22] = {
    data_ov007_0227aee8,
    data_ov007_0227aef8,
    data_ov007_0227af08,
    data_ov007_0227af18,
    data_ov007_0227aed8,
    NULL,
    data_ov007_0227af28,
    data_ov007_0227af38,
    data_ov007_0227af48,
    data_ov007_0227aec8,
    data_ov007_0227ae58,
    data_ov007_0227ae68,
    data_ov007_0227ae78,
    data_ov007_0227aeb8,
    data_ov007_0227ae88,
    data_ov007_0227ae98,
    data_ov007_0227aea8,
    NULL,
    NULL,
    NULL,
    NULL,
    data_ov007_0227af58,
};
static const VecFx32 sFieldopModelScale = {0x1000, 0x1000, 0x1000};
static const u16 *sActFrameTaskIds[22] = {
    data_ov007_0227ac38,
    data_ov007_0227adf8,
    data_ov007_0227adb8,
    data_ov007_0227ae38,
    data_ov007_0227ad18,
    NULL,
    data_ov007_0227add8,
    data_ov007_0227ad98,
    data_ov007_0227ac78,
    data_ov007_0227ad58,
    data_ov007_0227ac98,
    data_ov007_0227ad38,
    data_ov007_0227ad78,
    data_ov007_0227ae18,
    data_ov007_0227ac58,
    data_ov007_0227acf8,
    data_ov007_0227acd8,
    NULL,
    NULL,
    NULL,
    NULL,
    data_ov007_0227acb8,
};
// Light setups (0x0227af68) indexed by data_020d433c.mUnk_95: four lights each, as ModelLightGroup reads them
// (enable flag, colour, direction). The table is a section of its own, after the 16-byte tables above: a
// function-local static of an inline accessor gets one.
struct LightRecord {
    /* 00 */ u16 mEnabled;
    /* 02 */ u16 mColor;
    /* 04 */ s16 mDirection[3];
    /* 0a */
};
typedef LightRecord LightSetup[4];
inline const LightSetup *LightSetups() {
    static const LightRecord sLights[8][4] = {
        {{1, 0x7fff, {-0x452, -0x6e1, -0xdc3}}, {0}, {0}, {0}},
        {{1, 0x7fff, {-0x452, -0x6e1, -0xdc3}}, {0}, {0}, {0}},
        {{1, 0x7fff, {-0x452, -0x6e1, -0xdc3}}, {0}, {0}, {0}},
        {{1, 0x7fff, {-0x452, -0x6e1, -0xdc3}}, {0}, {0}, {0}},
        {{1, 0x7fff, {-0x452, -0x6e1, -0xdc3}}, {0}, {0}, {0}},
        {{1, 0x7fff, {-0x452, -0x6e1, -0xdc3}}, {0}, {0}, {0}},
        {{1, 0x7fff, {-0x452, -0x6e1, -0xdc3}}, {0}, {0}, {0}},
        {{1, 0x7fff, {-0x452, -0x6e1, -0xdc3}}, {0}, {0}, {0}},
    };
    return sLights;
}
BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);
BOOL func_ov007_022632d0(BattlePlayerList *list, u32 flag);


BattleCharaAct::BattleCharaAct(Camera *camera, u8 player, u32 kind, u8 unk4, u8 unk5)
    : SceneBase(0x3e8, 0) {
    mCamera = camera;
    mSecondCamera = 0;
    mCamFile = 0;
    mMapObj = 0;
    mFieldopModel = 0;
    mState = 0;
    u8 k    = kind;
    mKind = k;
    mUnclampedKind = k;
    mPlayer = player;
    mActDrawFrames = 0;
    mTexVariant = unk4;
    mSecondTexVariant = unk5;
    Init();
}

void BattleCharaAct::Init() {
    mActFrames = 0;
    mFadeBrightness = 0;
    mCharacterId = data_020d433c.mBattlePlayers->mPlayers[mPlayer]->mCharacterId;
    if (mKind >= 0x10) {
        mKind = 10;
    }
    switch (mKind) {
    case 0:
        SpriteManager::Get().SetupMode(1, 7);
        mSecondCamera = new Camera();
        mSecondCamera->SetFov(0x1555);
        mSecondCamera->SetClip(0x1000, 0x1388000);
        mSecondCamera->SetPosition(0, 0, 0x22c000 - 0x1000);
        mSecondCamera->SetTarget(0, 0, 0);
        CameraManager::Get().SetCamerasAndRequestMode(3, mCamera, (u32) mSecondCamera);
        break;
    case 5:
    case 6:
        SpriteManager::Get().func_0202740c();
        CameraManager::Get().mActive = 0;
        CameraManager::Get().SetCamerasAndRequestMode(1, mCamera, 0);
        break;
    default:
        SpriteManager::Get().SetupMode(1, 7);
        CameraManager::Get().SetCamerasAndRequestMode(2, mCamera, 0);
        break;
    }
    mActModel = 0;
    LoadActModels();
    StartCardEffect();
    mEdited = CreateCharacter();
    u8 n    = mKind + 1;
    char buffer[0x20];
    if (mEdited == 1) {
        ChangeDir("/Game/Character/EditedCharacter/Cam/");
        sprintf(buffer, "edit_M_M%02d.cam", n);
    } else {
        ChangeDir("/Game/Character/Character/");
        sprintf(buffer, "chara%02d_M_%02d.cam", mCharacterId, n);
    }
    mCamFile = LoadFile(buffer, 0, 0, 0, 0, 0);
    mCamera->SetPath(mCamFile, 0);
    ChangeDir("/");
    mLights = new ModelLightGroup(LightSetups()[data_020d433c.mFieldId]);
}

BattleCharaAct::~BattleCharaAct() {
    ModelLightGroup *lights = mLights;
    if (lights) {
        delete lights;
    }
    delete mSecondCamera;
    CameraManager::Get().SetCamerasAndRequestMode(0, 0, 0);
    UnkStruct_0205401c *character = mCharacter;
    if (character) {
        character->Release();
        delete character;
    }
    DeleteFieldopModel();
    ModelInstance *model = mActModel;
    if (model) {
        model->mOwnFlags = 0xf;
        delete model;
        mActModel = 0;
    }
    if (mCamFile) {
        OS_Free(mCamFile);
    }
    BattleMapObj *field = mMapObj;
    if (field) {
        delete field;
    }
}

u8 BattleCharaAct::CreateCharacter() {
    mCharacter = new UnkStruct_0205401c();
    mCharacter->Create(mCharacterId, 1, data_020d433c.mBattlePlayers->mPlayers[mPlayer]->NameInfo(), (u8) (mKind + 1));
    mCharacter->func_0205461c(mKind, 0);
    return mCharacter->mUseCharacter;
}

void BattleCharaAct::LoadActModels() {
    char modelName[0x20];
    char textureName[0x20];
    char animName[0x20];
    char paletteA[0x20];
    char paletteB[0x20];
    u32 loaded       = 0;
    u32 paletteAKind = 0;
    u32 paletteBKind = 0;
    ChangeDir("/Game/Character/Act/");
    switch (mKind) {
        case 0:
            if (mKind == 0) {
                LoadFieldopModel();
            }
        case 3:
        case 15:
            if (mTexVariant != 0) {
                paletteAKind = 1;
                sprintf(paletteA, "T_sphe_%02d", mTexVariant);
            }
        case 1:
        case 6:
            sprintf(modelName, "%s%02d.nsbmd", sActFilePrefixes[mCharacterId], mKind + 1);
            sprintf(textureName, "%s%02d.nsbtx", sActFilePrefixes[mCharacterId], mKind + 1);
            sprintf(animName, "%sM_%02d_data.bin", sActFilePrefixes[mCharacterId], mKind + 1);
            loaded = 1;
            if ((mKind == 1) | (mKind == 6)) {
                if ((mTexVariant != 0) & (mTexVariant <= 2)) {
                    paletteAKind = 2;
                    sprintf(paletteA, "ab_%02d", mTexVariant);
                }
            }
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            if (mTexVariant != 0) {
                paletteAKind = 1;
                sprintf(paletteA, "T_sphe_%02d", mTexVariant);
            }
            if (mKind == 8 && mSecondTexVariant != 1) {
                paletteBKind = 1;
                sprintf(paletteB, "T_sphe_%02d", mSecondTexVariant);
            }
            sprintf(modelName, "all_%02d.nsbmd", mKind + 1);
            sprintf(textureName, "all_%02d.nsbtx", mKind + 1);
            sprintf(animName, "all_M_%02d_data.bin", mKind + 1);
            loaded = 1;
            break;
    }
    if (loaded) {
        TextureFile *texture = new NitroTexture(textureName);
        if (paletteAKind == 1) {
            texture->LoadPaletteFile("S_sphe00", paletteA, 0);
        }
        if (paletteBKind == 1) {
            texture->LoadPaletteFile("S_sphe01", paletteB, 0);
        }
        if (paletteAKind == 2) {
            texture->LoadPaletteFile("ab_00", paletteA, 0);
        }
        ModelFile *model = new BattleCharaActModel(modelName, texture);
        texture->FreeFile();
        ModelAnimData *data = new ModelAnimData(animName);
        ModelAnim *anim     = new ModelAnim(data, (NitroModel *) model);
        mActModel             = model->CreateInstance(anim);
        mActModel->StartAnimWithIncrement(0, 0, 0x1000);
        mActModel->mUseBoxTest = 0;
    }
    ChangeDir("/");
}

void BattleCharaAct::StartCardEffect() {
    switch (mKind) {
        case 10:
            switch (mUnclampedKind - 0x10) {
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

void BattleCharaAct::Update() {
    u32 run = 1;
    switch (mState) {
        case 0:
            run = 0;
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0x1a)) {
                mState++;
            }
            break;
        case 1:
            mNextScene = run;
            run     = 0;
            func_ov007_02269380();
            mState++;
            break;
        case 2:
            mFrameTaskId = 0x1ee;
            if (sActFrameTaskIds[mCharacterId] != NULL) {
                mFrameTaskId = sActFrameTaskIds[mCharacterId][mKind];
                if (mFrameTaskId != 0x1ee) {
                    if (mFrameTaskId == 0x1dc && gSaveData[0].mUnk_42 == 0) {
                        mFrameTaskId = 0x1ee - 0x11;
                    }
const u8 *t = sActFrameTaskFrameCounts[mCharacterId];
 if (t) { mFrameTaskTimer = t[mKind]; } else { mFrameTaskTimer = 0; }
                    FrameTask::Get().StartChannel(1, mFrameTaskId);
                    FrameTask::Get().func_0203dd4c(1);
                }
            }
            mState++;
            break;
        case 4:
            SpriteManager::Get().StartFade(2);
        case 3:
            mState++;
            break;
        case 5:
            if (!SpriteManager::Get().IsFading()) {
                mState++;
            }
            break;
        case 6:
            if (mKind == 0 && mFadeBrightness >= 0x10) {
                mState = 8;
                break;
            }
            if (mCharacter->IsAnimEndReached()) {
                mState = 8;
                break;
            }
            if (func_ov007_022632d0(data_020d433c.mBattlePlayers, 0)) {
                mNextScene = 2;
                mState = 8;
                break;
            }
            if (InputManager::Get().IsDown(8)) {
                mState++;
            }
            break;
        case 7:
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                if (func_ov007_022632d0(data_020d433c.mBattlePlayers, 1)) {
                    mNextScene = 2;
                    mState++;
                }
            } else {
                mNextScene = 2;
                mState++;
            }
            break;
        case 8:
            if (mKind == 0) {
                if (CameraManager::Get().mMode != 1) {
                    SpriteManager::Get().StartFade(4);
                    mActFrames = 0xffff;
                }
            } else {
                if (CameraManager::Get().mMode != 1) {
                    SpriteManager::Get().StartFade(3);
                } else {
                    SpriteManager::Get();
                    SpriteManager::sFade[0] = 3;
                }
            }
            mState++;
            break;
        case 9:
            if (!SpriteManager::Get().IsFading()) {
                CameraManager::Get().SetCamerasAndRequestMode(0, NULL, 0);
                data_020d433c.mUnk_14 |= 4;
                mState++;
            }
            break;
        case 10:
            run = 0;
            if (mFrameTaskId != 0x1ee) {
                FrameTask::Get().func_0203dd60(1);
            }
            mState++;
            break;
        case 11:
            run = 0;
            if (SetMessageTypeAndCheckAll(data_020d433c.mBattlePlayers, 0xf)) {
                RequestScene(mNextScene);
                mFinished = 1;
            }
            break;
    }
    if (!run) {
        return;
    }
    if (mFrameTaskId != 0x1ee) {
        if (mFrameTaskTimer != 0) {
            mFrameTaskTimer--;
        }
        if (mFrameTaskTimer == 0) {
            FrameTask::Get().func_0203dd60(1);
            mFrameTaskId = 0x1ee;
        }
    }
    CameraManager &manager         = CameraManager::Get();
    Camera *camera                 = manager.mCameras[manager.mActive];
    data_02211374.mCameraPosition  = camera->mPosition;
    data_02211374.mCameraUp        = camera->mUp;
    data_02211374.mCameraTarget    = camera->mTarget;
    MTX_LookAt(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mActive] = data_02211374.mProjection;
    manager.mView[manager.mActive]       = data_02211374.mView;
    manager.mMtxA[manager.mActive]       = *func_02032710();
    manager.mMtxB[manager.mActive]       = *func_02032604();
    manager.mMtxC[manager.mActive]       = *func_020323b8();
    if (mKind == 0) {
        if (CameraManager::Get().mActive == 0) {
            u8 saved                     = CameraManager::Get().mModelsVisible;
            CameraManager::Get().mModelsVisible = 0;
            Draw();
            CameraManager::Get().mModelsVisible = saved;
            Draw();
        } else {
            DrawFieldopModel();
        }
        if (SpriteManager::Get().IsFading()) {
            return;
        }
        if (mActFrames == 0xffff) {
            return;
        }
        mActFrames++;
        int frames = mActFrames;
        if (frames > 0x5a && frames % 7 == 0) {
            if (mFadeBrightness < 0x10) {
                mFadeBrightness++;
                SpriteManager::Get().SetFade(1, (u8) mFadeBrightness);
            }
        }
    } else {
        Draw();
    }
}

void BattleCharaAct::func_ov007_02269380() {
    u8 type = 0xa;
    if (mKind != 0) {
        if (mKind == 0xf) {
            type = 0xb;
        }
    } else {
        type = 9;
    }
    mMapObj = new BattleMapObj(type);
    mMapObj->Load();
    if (mKind != 5 && mKind != 6 && data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->SetupLayout(0xd);
    }
}

void BattleCharaAct::Draw() {
    mCamera->AdvanceFrame(0);
    mCamera->ApplyPath();
    mMapObj->Draw();
    mCharacter->Draw();
    if (!mActModel) {
        return;
    }
    BOOL draw    = 1;
    MtxFx43 mtx  = gIdentityMtx43;
    VecFx32 pos  = sActModelScale;
    VecFx32 zero = {0, 0, 0};
    MTX_SetTranslation43(&mtx, &zero);
    if ((mKind == 10) | ((mKind == 7) | ((mKind == 8) | (mKind == 9)))) {
        mActDrawFrames++;
        if (mActDrawFrames >= 0x16) {
            draw = 0;
        }
    }
    if (!draw) {
        return;
    }
    mActModel->AdvanceAnim();
    mActModel->Draw(&mtx, &pos);
}

void BattleCharaAct::LoadFieldopModel() {
    TextureFile *texture = new NitroTexture("fieldop_00.nsbtx");
    ModelFile *model   = new BattleCharaActModel("fieldop_00.nsbmd", texture);
    texture->FreeFile();
    ModelAnimData *data = new ModelAnimData("fieldop_00_data.bin");
    ModelAnim *anim     = new ModelAnim(data, (NitroModel *) model);
    mFieldopModel             = model->CreateInstance(anim);
    mFieldopModel->StartAnim(0, 1);
    mFieldopModel->mUseBoxTest = 0;
    mFieldopModel->StartAnimWithIncrement(0, 1, 0x2000);
}

void BattleCharaAct::DeleteFieldopModel() {
    ModelInstance *model = mFieldopModel;
    if (model) {
        model->mOwnFlags = 0xf;
        delete model;
    }
}

void BattleCharaAct::DrawFieldopModel() {
    if (!mFieldopModel) {
        return;
    }
    MtxFx43 mtx = gIdentityMtx43;
    VecFx32 pos = sFieldopModelScale;
    MTX_ApplyRotX43(&mtx, 0x4000);
    mFieldopModel->AdvanceAnim();
    mFieldopModel->Draw(&mtx, &pos);
}

u32 GetTexVariant(u32 a, u32 b) {
    if (a == 0x22 && b == 0) {
        b = 2;
    }
    return b;
}
