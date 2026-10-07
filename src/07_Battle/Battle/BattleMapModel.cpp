#include "Battle/BattleMapTypes.hpp"
#include "System/FrameClock.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Model.hpp"
#include "Graphics/ModelTransform.hpp"
#include "System/FileSystem.hpp"

extern "C" void func_020330fc(void *model, u32 a, u32 b);
extern "C" void func_02032370(u32 a, u32 b, u32 c);
extern "C" void func_02032394(u32 a, u32 b, u32 c);

BattleMapModel::BattleMapModel(u32 unk1) {
    mUnk_2c = unk1;
    func_ov007_02248cf4();
    mUnk_2d = 0x1f;
}

BattleMapModel::~BattleMapModel() {
    ModelInstance *model = mModel;
    if (model) {
        model->mUnk_68 = 0xf;
        delete model;
        mModel = 0;
    }
}

void BattleMapModel::func_ov007_02248cf4() {
    ChangeDir("/Game/Battle/3d/MapObj");
    switch (mUnk_2c) {
        case 0:
            func_020525c0("card_00_00.nsbtx", "card_00_00.nsbmd", "card_00_00_data.bin", 0);
            break;
        case 1:
            func_020526a8("card_futi.nsbtx", "card_futi.nsbmd");
            break;
        case 2:
            func_020525c0("o00btl_000.nsbtx", "o00btl_000.nsbmd", "o00btl_000_data.bin", 0);
            break;
        case 3:
            func_020525c0("f00gat_000.nsbtx", "f00gat_000.nsbmd", "f00gat_000_data.bin", 0);
            break;
        case 4:
            func_020525c0("f00gat_000.nsbtx", "f00gat_000.nsbmd", "f00gat_000_data.bin", 0);
            break;
        case 5:
            func_020525c0("f00gat_001.nsbtx", "f00gat_001.nsbmd", "f00gat_001_data.bin", 0);
            break;
        case 6:
            func_020525c0("f00gat_002.nsbtx", "f00gat_002.nsbmd", "f00gat_002_data.bin", 0);
            break;
        case 7:
            func_020525c0("f00gat_003.nsbtx", "f00gat_003.nsbmd", "f00gat_003_data.bin", 0);
            break;
    }
    ChangeDir("/");
}

// non-matching: 14 words, registers only (data and color swapped).
void BattleMapModel::func_ov007_02248eb8() {
    if (mModel == 0) {
        return;
    }
    MtxFx43 mtx = data_0220e86c;
    func_01ffcf64(&mtx, &mPosition);
    mModel->func_02028ef0();
    if (UnkStruct_020c1884::Get().mFrame & 0x20) {
        if (mUnk_2d > 0xf) {
            mUnk_2d--;
        }
    } else {
        if (mUnk_2d < 0x1f) {
            mUnk_2d++;
        }
    }
    void *data = mModel->mUnk_58->mUnk_04;
    func_020330fc(data, 0, 0x80);
    func_020330fc(data, 0, 0x40);
    func_020330fc(data, 0, 0x400);
    func_020330fc(data, 0, 0x200);
    u16 color = mUnk_2d | (mUnk_2d << 5) | (mUnk_2d << 10);
    func_02032370(color, color, 1);
    func_02032394(color, color, 1);
    mModel->func_02029174(&mtx, &mScale);
    func_020330fc(data, 1, 0x80);
    func_020330fc(data, 1, 0x40);
    func_020330fc(data, 1, 0x400);
    func_020330fc(data, 1, 0x200);
    mModel->func_02028de8(0, 0);
}
