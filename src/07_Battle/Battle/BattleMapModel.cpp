#include "Battle/BattleMapTypes.hpp"
#include "System/GameSystem.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/ModelTransform.hpp"
#include "System/FileSystem.hpp"

extern "C" void func_020330fc(void *model, u32 a, u32 b);
extern "C" void func_02032370(u16 a, u16 b, BOOL c);
extern "C" void func_02032394(u16 a, u16 b, BOOL c);

BattleMapModel::BattleMapModel(u32 unk1) {
    mKind = unk1;
    LoadModel();
    mColorLevel = 0x1f;
}

BattleMapModel::~BattleMapModel() {
    ModelInstance *model = mModel;
    if (model) {
        model->mOwnFlags = MODEL_OWN_ALL;
        delete model;
        mModel = 0;
    }
}

void BattleMapModel::LoadModel() {
    ChangeDir("/Game/Battle/3d/MapObj");
    switch (mKind) {
        case MAP_MODEL_KIND_CARD_00_00:
            LoadFiles("card_00_00.nsbtx", "card_00_00.nsbmd", "card_00_00_data.bin", 0);
            break;
        case MAP_MODEL_KIND_CARD_FUTI:
            LoadFilesNoAnim("card_futi.nsbtx", "card_futi.nsbmd");
            break;
        case MAP_MODEL_KIND_O00BTL_000:
            LoadFiles("o00btl_000.nsbtx", "o00btl_000.nsbmd", "o00btl_000_data.bin", 0);
            break;
        case MAP_MODEL_KIND_F00GAT_000_TIMED:
            LoadFiles("f00gat_000.nsbtx", "f00gat_000.nsbmd", "f00gat_000_data.bin", 0);
            break;
        case MAP_MODEL_KIND_F00GAT_000:
            LoadFiles("f00gat_000.nsbtx", "f00gat_000.nsbmd", "f00gat_000_data.bin", 0);
            break;
        case MAP_MODEL_KIND_F00GAT_001:
            LoadFiles("f00gat_001.nsbtx", "f00gat_001.nsbmd", "f00gat_001_data.bin", 0);
            break;
        case MAP_MODEL_KIND_F00GAT_002:
            LoadFiles("f00gat_002.nsbtx", "f00gat_002.nsbmd", "f00gat_002_data.bin", 0);
            break;
        case MAP_MODEL_KIND_F00GAT_003:
            LoadFiles("f00gat_003.nsbtx", "f00gat_003.nsbmd", "f00gat_003_data.bin", 0);
            break;
    }
    ChangeDir("/");
}

void BattleMapModel::DrawPulsing() {
    if (mModel == 0) {
        return;
    }
    u16 color;
    MtxFx43 mtx = gIdentityMtx43;
    MTX_SetTranslation43(&mtx, &mPosition);
    mModel->DetachAllAnims();
    if (GameSystem::Get().mFrame & 0x20) {
        if (mColorLevel > 0xf) {
            mColorLevel--;
        }
    } else {
        if (mColorLevel < 0x1f) {
            mColorLevel++;
        }
    }
    void *data = mModel->mModelFile->mResModel;
    func_020330fc(data, 0, 0x80);
    func_020330fc(data, 0, 0x40);
    func_020330fc(data, 0, 0x400);
    func_020330fc(data, 0, 0x200);
    color = mColorLevel | (mColorLevel << 5) | (mColorLevel << 10);
    func_02032370(color, color, 1);
    func_02032394(color, color, 1);
    mModel->Draw(&mtx, &mScale);
    func_020330fc(data, 1, 0x80);
    func_020330fc(data, 1, 0x40);
    func_020330fc(data, 1, 0x400);
    func_020330fc(data, 1, 0x200);
    mModel->StartAnim(0, 0);
}
