#include "Graphics/Model.hpp"
#include "Graphics/ModelInternal.hpp"

ModelLight::ModelLight(u32 index, u32 enabled, u16 color, const s16 *direction) {
    mEnabled      = enabled;
    mIndex        = index;
    mColor        = color;
    mDirection[0] = direction[0];
    mDirection[1] = direction[1];
    mDirection[2] = direction[2];
    if ((mDirection[0] | mDirection[1] | mDirection[2]) != 0) {
        func_01ff90dc(mDirection, mDirection);
        if (mDirection[0] >= 0x1000) {
            mDirection[0] = 0xfff;
        }
        if (mDirection[1] >= 0x1000) {
            mDirection[1] = 0xfff;
        }
        if (mDirection[2] >= 0x1000) {
            mDirection[2] = 0xfff;
        }
    }
}

ModelLight::~ModelLight() {}

void ModelLight::Apply() {
    if (mEnabled == 0) {
        return;
    }
    func_0203235c(mIndex, mColor);
    func_02032320(mIndex, mDirection[0], mDirection[1], mDirection[2]);
}

void ModelLight::SetDirection(const s16 *direction) {
    mDirection[0] = direction[0];
    mDirection[1] = direction[1];
    mDirection[2] = direction[2];
    func_01ff90dc(mDirection, mDirection);
    if (mDirection[0] >= 0x1000) {
        mDirection[0] = 0xfff;
    }
    if (mDirection[1] >= 0x1000) {
        mDirection[1] = 0xfff;
    }
    if (mDirection[2] >= 0x1000) {
        mDirection[2] = 0xfff;
    }
}

ModelLightGroup::ModelLightGroup(const void *data) {
    for (u8 i = 0; i < 4; i++) {
        const u8 *table   = (const u8 *) data;
        ModelLight *light = new ModelLight(i, table[i * 10], *(const u16 *) (table + i * 10 + 2),
                                           (const s16 *) (table + i * 10 + 4));
        mLights[i] = light;
        light->Apply();
    }
}

ModelLightGroup::~ModelLightGroup() {
    for (u8 i = 0; i < 4; i++) {
        if (mLights[i] != NULL) {
            delete mLights[i];
        }
    }
}

void ModelLightGroup::ApplyAll() {
    for (u8 i = 0; i < 4; i++) {
        mLights[i]->Apply();
    }
}
