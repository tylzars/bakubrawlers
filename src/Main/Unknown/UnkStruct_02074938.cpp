#include "Unknown/UnkStruct_02074938.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Text/TextManager.hpp"

#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern const u8 data_020a2fd0[][2];
extern const VecFx32 data_020a2fd8;
extern const VecFx32 data_020a2fe4;
extern const u8 data_020a2ff0[][2];
extern const u8 data_020a3004[];
extern const s8 data_020a302c[];
extern const s8 data_020a3044[];
extern char data_020d441a[];

// Position of a model object (CharacterModel / StoryCharacterModel): the VecFx32 at +4.
struct UnkStruct_ModelPos {
    /* 00 */ PAD(0x00, 0x04);
    /* 04 */ VecFx32 mPosition;
    /* 10 */
};

// Sets the position of a model object; the pointer is the first argument so the three values are computed before it.
static inline void SetModelPos(UnkStruct_ModelPos *model, fx32 x, fx32 y, fx32 z) {
    model->mPosition.x = x;
    model->mPosition.y = y;
    model->mPosition.z = z;
}

UnkStruct_02074938::UnkStruct_02074938(u8 unk1, u16 unk2, u32 unk3, u32 unk4) :
    SceneBase(0x3e8, 0) {
    mUnk_44 = NULL;
    mUnk_48 = NULL;
    mUnk_4c = NULL;
    mUnk_50 = NULL;
    mUnk_54 = NULL;
    mUnk_78 = 0;
    mUnk_7c = 0;
    mUnk_7e = unk4;
    mUnk_80 = 0;
    mUnk_84 = 0;
    mUnk_58 = unk1;
    mUnk_5a = unk2;
    mUnk_5c = unk3;
    for (u8 i = 0; i < 4; i++) {
        mUnk_0c[i] = NULL;
    }
    for (u8 i = 0; i < 10; i++) {
        mUnk_1c[i] = NULL;
    }
    mUnk_68 = NULL;
    mUnk_6c = NULL;
    mUnk_70 = 0;
    mUnk_74 = 0;
}

UnkStruct_02074938::~UnkStruct_02074938() {
    CameraManager::Get().func_02026504(0, NULL, 0);
    ModelInstance *instance = mUnk_44;
    if (instance != NULL) {
        instance->mUnk_68 = 0xf;
        delete instance;
    }
    UnkStruct_0205401c *model = mUnk_50;
    if (model != NULL) {
        model->func_02054034();
        delete model;
    }
    delete mUnk_48;
    UnkStruct_02029ca8 *lights = mUnk_4c;
    if (lights != NULL) {
        delete lights;
    }
    u8 i = 0;
    CellAnimation *cellA;
    do {
        cellA = mUnk_0c[i];
        if (cellA != NULL) {
            delete cellA;
        }
        i++;
    } while (i < 4);
    CellAnimation *cellB;
    u8 j = 0;
    do {
        cellB = mUnk_1c[j];
        if (cellB != NULL) {
            delete cellB;
        }
        j++;
    } while (j < 10);
    if (mUnk_54 != NULL) {
        OS_Free(mUnk_54);
    }
    func_02075740();
    if (mUnk_68 != NULL) {
        OS_Free(mUnk_68);
    }
    if (mUnk_6c != NULL) {
        OS_Free(mUnk_6c);
    }
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().func_0202740c();
}

void UnkStruct_02074938::Update() {
    u8 run = 1;
    switch (mUnk_7c) {
        case 0:
            run = 0;
            func_02074964();
            mUnk_7c++;
            break;
        case 1:
            if (mUnk_84 != 0) {
                mUnk_7c = 5;
            } else if (mUnk_80 != 0) {
                mUnk_7c = 2;
            }
            break;
        case 2:
            if (func_02075878(-0x64000)) {
                mUnk_50->func_02054694(0);
                func_02075758();
                func_020754b8();
                mUnk_7c = 3;
            }
            break;
        case 3:
            mUnk_7c = 4;
            break;
        case 4:
            if (func_02075878(0)) {
                mUnk_80 = 0;
                mUnk_7c = run;
            }
            break;
        case 5:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_7c = 6;
            }
            break;
        case 6:
            mFinished = run;
            run       = 0;
            break;
    }
    if (run != 0) {
        func_02075160();
    }
}

// non-matching: 11 words, register naming only (the original loads the data_020a302c table first and keeps the tables in r1/r0 swapped); size matches.
void UnkStruct_02074938::func_02074828(u32 kind) {
    mUnk_50 = new UnkStruct_0205401c();
    SpriteManager::Get().func_02027504(0);
    mUnk_50->func_0205411c(kind & 0xff, 0, 0, 0);
    mUnk_50->func_0205461c(0, 1);
    mUnk_50->func_02054694(1);
    VecFx32 pos;
    pos.x = 0;
    pos.y = data_020a3044[kind] << 12;
    pos.z = data_020a302c[kind] << 12;
    UnkStruct_ModelPos *model;
    if (kind == 0x15) {
        model = (UnkStruct_ModelPos *) mUnk_50->mUnk_04;
    } else {
        model = (UnkStruct_ModelPos *) mUnk_50->mUnk_08;
    }
    model->mPosition = pos;
    u16 *rot = mUnk_50->func_020546cc();
    rot[1] += 0x4cd;
}

BOOL UnkStruct_02074938::func_02074938(u8 unk1, u16 unk2, u32 unk3) {
    if (mUnk_80 != 0) {
        return FALSE;
    }
    mUnk_60 = unk1;
    mUnk_62 = unk2;
    mUnk_64 = unk3;
    mUnk_80 = 1;
    return TRUE;
}

void UnkStruct_02074938::func_02074964() {
    mUnk_48 = new Camera();
    mUnk_48->SetUp(0, 0x1000, 0);
    mUnk_48->SetTarget(0x28000, 0x6e000, 0);
    mUnk_48->SetClip(0x1000, 0x1388000);
    mUnk_48->func_02026004(0x2aaa);
    mUnk_48->SetPosition(0x14000, 0x5a000, 0x96000);
    CameraManager::Get().mActive = 0;
    CameraManager::Get().func_02026504(1, mUnk_48, 0);
    CameraManager &manager = CameraManager::Get();
    Camera *camera         = manager.mCameras[manager.mActive];
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
    mUnk_4c = new UnkStruct_02029ca8(data_020a3004);
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.func_02027624(1);
    ChangeDir("/Game/Myroom/");
    screen.LoadCharacters("myroom_ranking_u_obj_00.nce_ncg1D.bin", 0, 0);
    screen.LoadPalette("myroom_ranking_u_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i < 4; i++) {
        mUnk_0c[i]            = new CellAnimation("myroom_ranking_u_obj_00_nce.bin", 0, 1, 1);
        mUnk_0c[i]->mCharBase = screen.func_020276f4(0);
        mUnk_0c[i]->mUnk_11   = 0;
    }
    switch (mUnk_7e) {
        case 0:
            mUnk_0c[0]->SetAnimation(0);
            break;
        case 1:
            mUnk_0c[0]->SetAnimation(1);
            break;
        default:
            mUnk_0c[0]->SetAnimation(0);
            break;
    }
    mUnk_0c[1]->SetAnimation(2);
    ChangeDir("/Game/2d/union_u/");
    screen.LoadCharacters("union_u_obj_number_32k.nce_ncg1D.bin", 0x5000, 0);
    screen.LoadPalette("union_u_obj_number_32k_ncl.bin", 0xa0, 0);
    for (u8 i = 0; i < 10; i++) {
        mUnk_1c[i]            = new CellAnimation("union_u_obj_number_32k_nce.bin", 0, 1, 1);
        mUnk_1c[i]->mCharBase = screen.func_020276f4(0x5000);
        mUnk_1c[i]->mUnk_11   = 5;
        mUnk_1c[i]->SetAnimation(0);
    }
    ChangeDir("/");
    mUnk_54 = (MessageFile *) func_02023504("/font/mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
    TextManager::Get();
    mUnk_70 = 0x400;
    TextManager::Get();
    mUnk_74 = 4;
    mUnk_68 = OS_Alloc(mUnk_70);
    MI_CpuClear16(mUnk_68, mUnk_70);
    mUnk_6c = OS_Alloc(mUnk_74 * 0xc);
    TextManager::Get().Init(mUnk_6c, 0, 0, 0x10, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    func_020754b8();
    ChangeDir("/Game/Myroom/Map/");
    NitroModel *model     = new NitroModel;
    NitroTexture *texture = new NitroTexture("myroombg.nsbtx");
    model->func_020289e0("myroombg.nsbmd", texture);
    texture->func_02028344();
    mUnk_44 = model->func_02028b78(NULL);
    ChangeDir("/");
    func_02074828(mUnk_58);
    ChangeDir("/");
}

void UnkStruct_02074938::func_02075160() {
    func_0207520c();
    MtxFx43 *camera = func_020323b8();
    MtxFx43 mtx     = data_0220e86c;
    VecFx32 pos     = data_020a2fd8;
    VecFx32 offset  = data_020a2fe4;
    func_01ffcf64(&mtx, &offset);
    func_01ff89c0(&mtx, camera, &mtx);
    mUnk_44->func_02028d80();
    mUnk_44->func_02029174(&mtx, &pos);
    mUnk_50->func_02054568();
}

void UnkStruct_02074938::func_0207520c() {
    for (u8 i = 0; i < 4; i++) {
        SpriteManager::Get().Screen(0).func_02027778(mUnk_6c, 1, (s16) (i * 0x20 + 0x6a), 0x36, 0,
                                                     (u16) (i * 8 + 0x300), 0xf, 0);
    }
    u32 high = mUnk_5a;
    u32 low  = mUnk_5c;
    for (u8 i = 0; i < 10; i++) {
        CellAnimation *cell = mUnk_1c[i];
        if (cell == NULL) {
            continue;
        }
        u32 special = 0;
        u8 digit    = 0;
        if (i < 3) {
            if (high != 0) {
                digit = high % 10;
                high /= 10;
            } else if (i != 0) {
                continue;
            }
        } else if (i < 10) {
            if (low != 0) {
                digit = low % 10;
                low /= 10;
            } else if (i != 3) {
                continue;
            }
        } else if (mUnk_58 == 0x15) {
            special = 1;
        }
        if (special != 0) {
            cell->mUnk_0e = 2;
            cell->func_02029f6c();
            mUnk_1c[i]->func_02029fb8(5);
            mUnk_1c[i]->Update();
            mUnk_1c[i]->Draw(0, data_020a2ff0[i][0], data_020a2ff0[i][1], 0);
        } else {
            cell->mUnk_0e = 0;
            cell->func_02029f6c();
            mUnk_1c[i]->func_02029fb8(5);
            mUnk_1c[i]->Update();
            mUnk_1c[i]->func_0202a3ec(0, digit, data_020a2ff0[i][0], data_020a2ff0[i][1], 0);
        }
    }
    for (u8 i = 0; i < 2; i++) {
        if (mUnk_0c[i] != NULL) {
            mUnk_0c[i]->Update();
            mUnk_0c[i]->Draw(0, data_020a2fd0[i][0], data_020a2fd0[i][1], 0);
        }
    }
}

void UnkStruct_02074938::func_020754b8() {
    MI_CpuClear16(mUnk_68, mUnk_70);
    TextManager::Get().mUnk_5b = 1;
    if (mUnk_58 == 0x15) {
        s16 width  = 0;
        s16 height = 0;
        TextManager::Get().Print(data_020d441a, 0x18, mUnk_68, 0x10, 2, &width, &height, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    } else {
        TextManager &text = TextManager::Get();
        s16 height;
        s16 width;
        width  = 0;
        height = 0;
        u32 kind          = mUnk_58;
        MessageFile *file = mUnk_54;
        text.Print(file->GetText(kind), file->GetLength(kind), mUnk_68, 0x10, 2, &height, &width, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    }
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x6000, mUnk_68, mUnk_70, 0);
}

void UnkStruct_02074938::func_02075740() {
    MI_CpuClear16(mUnk_68, mUnk_70);
}

void UnkStruct_02074938::func_02075758() {
    UnkStruct_0205401c *old = mUnk_50;
    if (old != NULL) {
        old->func_02054034();
        delete old;
    }
    mUnk_50 = new UnkStruct_0205401c();
    mUnk_58 = mUnk_60;
    mUnk_59 = mUnk_61;
    mUnk_5a = mUnk_62;
    mUnk_5c = mUnk_64;
    u32 kind = mUnk_58;
    SpriteManager::Get().func_0202753c(0);
    mUnk_50->func_0205411c(kind & 0xff, 0, 0, 0);
    mUnk_50->func_0205461c(0, 1);
    mUnk_50->func_02054694(1);
    u16 *rot = mUnk_50->func_020546cc();
    rot[1] += 0x4cd;
    func_02075878(0);
}

BOOL UnkStruct_02074938::func_02075878(s32 target) {
    if (mUnk_78 == target) {
        return TRUE;
    }
    if (mUnk_78 > target) {
        mUnk_78 -= 0x14000;
        if (mUnk_78 < target) {
            mUnk_78 = target;
        }
    } else {
        mUnk_78 += 0x14000;
        if (mUnk_78 > target) {
            mUnk_78 = target;
        }
    }
    u8 kind = mUnk_58;
    SetModelPos(mUnk_58 == 0x15 ? (UnkStruct_ModelPos *) mUnk_50->mUnk_04 : (UnkStruct_ModelPos *) mUnk_50->mUnk_08, mUnk_78,
                data_020a3044[kind] << 12, data_020a302c[kind] << 12);
    return FALSE;
}
