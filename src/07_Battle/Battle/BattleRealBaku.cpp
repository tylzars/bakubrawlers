#include "Battle/BattleBase.hpp"

#include "Graphics/Model.hpp"
#include "Graphics/ModelResource.hpp"
#include "System/FileSystem.hpp"
#include <nitro/os.h>
#include <stdio.h>

extern const u8 gBakuganNumbers[];

struct BattleRealBakuTexEntry { // 0xc bytes, table at 022858b8
    /* 00 */ u8 mCount;
    /* 01 */ u8 mIds[11];
};
extern BattleRealBakuTexEntry gRealBakuTexEntries[];

BattleRealBaku::BattleRealBaku(u8 id, int unk2, u8 unk3, int unk4, int unk5, int unk6, int unk7) {
    mId       = id;
    mAnimDataIndex   = unk2;
    mVariant = unk3;
    mUnk_06_3 = unk4;
    mUseBoxTest = unk5;
    mUseCallbackMtx = unk6;
    mUnk_06_6 = unk7;
    mModel    = 0;
    mAnim     = 0;
    LoadModel();
}

BattleRealBaku::~BattleRealBaku() {
    ModelInstance *model = mModel;
    if (model) {
        model->mOwnFlags = 0xf;
        delete model;
        mModel = 0;
    }
}

void BattleRealBaku::LoadModel() {
    char buf[0x80];
    char name1[0x40];
    char name2[0x40];
    u8 charId = gBakuganNumbers[mId];
    bool flag = 1;
    NitroTexture *tex;
    NitroModel *model;
    ChangeDir("/Game/Realbaku/Mdl");
    if (mId == 0x1d) {
        sprintf(buf, "realb%02d_%02d.nsbtx", charId, mVariant);
        tex = new NitroTexture(buf);
        sprintf(buf, "realb%02d_%02d.nsbmd", charId, mVariant);
        model = new NitroModel(buf, tex);
    } else if (mId == 0x22 && mVariant == 0) {
        sprintf(buf, "realb83.nsbtx", charId); // the original passes the (unused) id here too
        tex = new NitroTexture(buf);
        sprintf(buf, "realb83.nsbmd", charId);
        model = new NitroModel(buf, tex);
        flag  = 0;
    } else {
        if (charId <= 0xe && mUnk_06_6) {
            sprintf(buf, "realb%02d_20.nsbtx", charId);
        } else {
            sprintf(buf, "realb%02d.nsbtx", charId);
        }
        tex = new NitroTexture(buf);
        if (charId <= 0xe && mUnk_06_6) {
            sprintf(buf, "realb%02d_20.nsbmd", charId);
        } else {
            sprintf(buf, "realb%02d.nsbmd", charId);
        }
        model = new NitroModel(buf, tex);
    }
    sprintf(buf, "realb%02d_%02d_data.bin", charId, mAnimDataIndex);
    ModelAnimData *data = new ModelAnimData(buf);
    mAnim               = new ModelAnim(data, model);
    mModel              = model->CreateInstance(mAnim);
    ChangeDir("/");
    mModel->mUseBoxTest = mUseBoxTest;
    mModel->StartAnim(0, mUnk_06_3);
    if (mUseCallbackMtx) {
        mModel->EnableCallbackMtx();
    }
    if (flag) {
        BattleRealBakuTexEntry *entry = &gRealBakuTexEntries[mId];
        if (entry->mCount) {
            ChangeDir("/Game/Realbaku/Tex");
            u8 i = 0;
            do {
                sprintf(name1, "realb%02d_%02d", charId, entry->mIds[i]);
                sprintf(name2, "realb%02d_%02d_%02d", charId, entry->mIds[i], mVariant);
                tex->LoadPaletteFile(name1, name2, 0);
                i++;
            } while (i < entry->mCount);
            ChangeDir("/");
        }
    }
    tex->FreeFile();
}
