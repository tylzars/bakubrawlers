#include "Battle/BattleBase.hpp"
#include "Battle/BattleMapTypes.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/ModelTransform.hpp"
#include "System/FileSystem.hpp"
#include <nitro/mi.h>
#include <stdio.h>

BattleMapObjPart::~BattleMapObjPart() {
    ModelInstance *model = mModel;
    if (model) {
        model->mOwnFlags = 0xb;
        delete model;
        mModel = 0;
    }
}

void BattleMapObjPart::func_ov007_022490ac(TextureFile *texture, const char *nsbmd, const char *data, u32 unk0) {
    NitroModel *model = new NitroModel(nsbmd, texture);
    if (data != 0) {
        ModelAnimData *animData = new ModelAnimData(data);
        ModelAnim *anim         = new ModelAnim(animData, model);
        mModel                  = model->CreateInstance(anim);
        mModel->StartAnimWithIncrement(0, 1, 0x1000);
        mAnimEnabled = unk0;
    } else {
        mModel  = model->CreateInstance(0);
        mAnimEnabled = 0;
    }
}

BattleMapObj::BattleMapObj(int unk1) {
    mUnk_20 = unk1;
    MI_CpuClear32(this, 0x20);
    mCount = 0;
}

BattleMapObj::~BattleMapObj() {
    u8 i = 0;
    TextureFile *texture = mParts[0]->mModel->mModelFile->mTexture;
    do {
        BattleMapObjPart *part = mParts[i];
        if (part) {
            delete part;
        }
        i++;
    } while (i < 8);
    if (texture) {
        texture->Release();
        delete texture;
    }
}

void BattleMapObj::func_ov007_02249200(const char *nsbtx, const char *nsbmd, const char *data, u32 flag) {
    mParts[mCount] = new BattleMapObjPart();
    mParts[mCount]->LoadFiles(nsbtx, nsbmd, data, 1);
    mParts[mCount]->mModel->mUseBoxTest = flag;
    mCount++;
}

void BattleMapObj::func_ov007_0224927c(TextureFile *texture, const char *nsbmd, const char *data, u32 flag) {
    mParts[mCount] = new BattleMapObjPart();
    mParts[mCount]->func_ov007_022490ac(texture, nsbmd, data, 1);
    mParts[mCount]->mModel->mUseBoxTest = flag;
    mCount++;
}

void BattleMapObj::func_ov007_022492f8(const BattleMapObjTable *table) {
    char path[0x20];
    char name[0x20];
    char data[0x20];
    sprintf(path, "%s.nsbtx", table->mEntries[0].mName);
    NitroTexture *texture = new NitroTexture(path);
    u8 i                  = 0;
    do {
        sprintf(name, "%s.nsbmd", table->mEntries[i].mName);
        const BattleMapObjEntry *entry = &table->mEntries[i];
        if (entry->mHasData != 0) {
            sprintf(data, "%s_data.bin", table->mEntries[i].mName);
            func_ov007_0224927c(texture, name, data, table->mEntries[i].mFlag);
        } else {
            func_ov007_0224927c(texture, name, 0, entry->mFlag);
        }
        i++;
    } while (i < table->mCount);
    texture->FreeFile();
}

extern const BattleMapObjTable data_ov007_0227a2bc[];
extern const BattleMapObjTable data_ov007_0227a2fc[];
extern const VecFx32 data_ov007_0227a13c;
extern const VecFx32 data_ov007_0227a148;

void BattleMapObj::func_ov007_022493fc() {
    ChangeDir("/Game/Battle/3d/Map");
    switch (mUnk_20) {
        case 9:
            func_ov007_02249200("throwfield00.nsbtx", "throwfield00.nsbmd", "throwfield00_data.bin", 1);
            break;
        case 11:
            func_ov007_02249200("throwfield01.nsbtx", "throwfield01.nsbmd", "throwfield01_data.bin", 0);
            break;
        case 12:
            func_ov007_02249200("realpop.nsbtx", "realpop.nsbmd", 0, 0);
            break;
        case 13:
            func_ov007_02249200("commandfiled0.nsbtx", "commandfiled0.nsbmd", "commandfiled0_data.bin", 0);
            break;
        case 10:
            func_ov007_022492f8(&data_ov007_0227a2bc[data_020d433c.mFieldId]);
            break;
        default:
            func_ov007_022492f8(&data_ov007_0227a2fc[mUnk_20]);
            break;
    }
    ChangeDir("/");
}

void BattleMapObj::func_ov007_0224954c() {
    if (mUnk_20 == 9) {
        MtxFx43 *camera = func_020323b8();
        MtxFx43 mtx     = gIdentityMtx43;
        VecFx32 scale   = data_ov007_0227a13c;
        VecFx32 offset  = data_ov007_0227a148;
        mtx             = gIdentityMtx43;
        func_01ffcf64(&mtx, &offset);
        func_01ff89c0(&mtx, camera, &mtx);
        mParts[0]->mModel->AdvanceAnim();
        mParts[0]->mModel->Draw(&mtx, &scale);
    } else {
        if (mCount != 0) {
            u8 i = 0;
            do {
                mParts[i]->func_020526bc();
                i++;
            } while (i < mCount);
        }
    }
}
