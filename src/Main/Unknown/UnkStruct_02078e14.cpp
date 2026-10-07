#include "Unknown/UnkStruct_02078e14.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>

extern u8 data_020d43db[];
void operator delete(void *ptr);

UnkStruct_02078e14::UnkStruct_02078e14(u32 unk1, u32 unk2) :
    SceneBase(0x3e8, 0) {
    mUnk_0c   = NULL;
    mUnk_10   = NULL;
    mUnk_14   = NULL;
    mUnk_18   = NULL;
    mUnk_1c   = NULL;
    mUnk_20   = NULL;
    mUnk_48   = NULL;
    mUnk_4c   = NULL;
    mUnk_50   = NULL;
    mUnk_54   = NULL;
    mUnk_58   = NULL;
    mUnk_c0   = NULL;
    mUnk_c4   = NULL;
    mUnk_e0   = unk1;
    mUnk_e1   = 0;
    mUnk_e2   = 0;
    mUnk_1e0  = 0;
    mUnk_1e4  = 0;
    mUnk_1e8  = NULL;
    mUnk_1ec  = NULL;
    mUnk_1f0  = unk2;
    mUnk_3f4  = 0;
    mUnk_3f8  = 0;
    mUnk_3f9  = 0;
    mUnk_3fa  = 0;
    mUnk_3fc  = 0;
    mUnk_3fe  = 0;
    mUnk_3ff  = 0;
    mUnk_400  = 0;
    mUnk_401  = 0;
    mUnk_404  = -1;
    SpriteManager::Get().SetFade(1, -16);
    mUnk_1f1 = mUnk_e0 == 5 ? data_020d43db[mUnk_1f0] : 0x15;
    for (u8 i = 0; i < 2; i++) {
        mUnk_28[i] = NULL;
        mUnk_30[i] = NULL;
        mUnk_38[i] = NULL;
        mUnk_40[i] = NULL;
    }
    for (u8 i = 0; i < 6; i++) {
        mUnk_5c[i] = NULL;
    }
    for (u8 i = 0; i < 0x13; i++) {
        mUnk_74[i] = NULL;
    }
    mUnk_f4 = 0;
    if (mUnk_1f1 != 0x15) {
        mUnk_f4 = 1;
    }
    mUnk_e4 = 0;
    mUnk_e6 = 0;
    mUnk_e7 = 0;
    mUnk_e8 = mUnk_e6;
    mUnk_e9 = 0;
    mUnk_ea = 0;
    mUnk_ec = 0;
    mUnk_f0 = 1;
    mUnk_f5 = 0;
    mUnk_3f9 = 2;
    mUnk_f8  = 0;
    mUnk_fa  = 0;
    mUnk_fb  = 0;
    mUnk_fc  = 0;
    mUnk_fd  = 0;
    mUnk_100 = 0;
    mUnk_104 = 0;
    for (u8 i = 0; i < 0x40; i++) {
        mUnk_106[i] = 0;
    }
    for (u8 i = 0; i < 13; i++) {
        mUnk_186[i] = 0;
    }
    for (u8 i = 0; i < 3; i++) {
        mUnk_194[i] = 1;
    }
    mUnk_1a0 = 4;
    mUnk_1a1 = 0;
    mUnk_1dc = 0;
    MI_CpuClear16(&mUnk_24, 2);
    mUnk_3f4 = data_020d433c.mUnk_8a_0;
}

UnkStruct_02078e14::~UnkStruct_02078e14() {
    u8 i;
    for (i = 0; i < 6; i++) {
        delete mUnk_5c[i];
    }
    for (i = 0; i < 0x13; i++) {
        delete mUnk_74[i];
    }
    delete mUnk_4c;
    delete mUnk_48;
    delete mUnk_50;
    operator delete(mUnk_54);
    operator delete(mUnk_58);
    if (mUnk_0c != NULL) {
        OS_Free(mUnk_0c);
        mUnk_0c = NULL;
    }
    if (mUnk_10 != NULL) {
        OS_Free(mUnk_10);
        mUnk_10 = NULL;
    }
    if (mUnk_14 != NULL) {
        OS_Free(mUnk_14);
        mUnk_14 = NULL;
    }
    if (mUnk_18 != NULL) {
        OS_Free(mUnk_18);
        mUnk_18 = NULL;
    }
    if (mUnk_1c != NULL) {
        OS_Free(mUnk_1c);
        mUnk_1c = NULL;
    }
    if (mUnk_20 != NULL) {
        OS_Free(mUnk_20);
        mUnk_20 = NULL;
    }
    for (u8 i = 0; i < 2; i++) {
        if (mUnk_28[i] != NULL) {
            OS_Free(mUnk_28[i]);
            mUnk_28[i] = NULL;
        }
        if (mUnk_30[i] != NULL) {
            OS_Free(mUnk_30[i]);
            mUnk_30[i] = NULL;
        }
    }
    SpriteManager::Get().SetFade(1, -16);
    if (data_020d433c.mUnk_a8 != 0) {
        mUnk_1e8->mUnk_04 = 1;
    }
    delete mUnk_1ec;
    mUnk_3f9 = 0;
    func_0207ed3c();
}

void UnkStruct_02078e14::func_0207966c(u32 unk1) {
    for (u8 i = 0; i < 6; i++) {
        delete mUnk_5c[i];
        mUnk_5c[i] = NULL;
    }
    for (u8 i = 0; i < 0x13; i++) {
        delete mUnk_74[i];
        mUnk_74[i] = NULL;
    }
    delete mUnk_4c;
    mUnk_4c = NULL;
    delete mUnk_48;
    mUnk_48 = NULL;
    delete mUnk_50;
    mUnk_50 = NULL;
    operator delete(mUnk_54);
    mUnk_54 = NULL;
    operator delete(mUnk_58);
    mUnk_58 = NULL;
    if (mUnk_0c != NULL) {
        OS_Free(mUnk_0c);
        mUnk_0c = NULL;
    }
    if (mUnk_10 != NULL) {
        OS_Free(mUnk_10);
        mUnk_10 = NULL;
    }
    if (mUnk_14 != NULL) {
        OS_Free(mUnk_14);
        mUnk_14 = NULL;
    }
    if (mUnk_18 != NULL) {
        OS_Free(mUnk_18);
        mUnk_18 = NULL;
    }
    if (mUnk_1c != NULL) {
        OS_Free(mUnk_1c);
        mUnk_1c = NULL;
    }
    if (mUnk_20 != NULL) {
        OS_Free(mUnk_20);
        mUnk_20 = NULL;
    }
    for (u8 i = 0; i < 2; i++) {
        if (mUnk_28[i] != NULL) {
            OS_Free(mUnk_28[i]);
            mUnk_28[i] = NULL;
        }
        if (mUnk_30[i] != NULL) {
            OS_Free(mUnk_30[i]);
            mUnk_30[i] = NULL;
        }
    }
    if (data_020d433c.mUnk_a8 != 0) {
        mUnk_1e8->mUnk_04 = 1;
    }
    delete mUnk_1ec;
    mUnk_1ec = NULL;
    if (unk1 != 0) {
        if (mUnk_400 == 0 && mUnk_401 == 0) {
            mContext->mUnk_12 = 1;
        }
        if (mUnk_c0 != NULL && mUnk_400 == 0) {
            func_0205821c(mUnk_c0);
            mUnk_c0           = NULL;
            mContext->mUnk_12 = 0;
        }
        if (mUnk_c4 != NULL && mUnk_401 == 0) {
            func_02055624(mUnk_c4, 1);
            mUnk_c4           = NULL;
            mContext->mUnk_12 = 0;
        }
    }
    mUnk_3f9 = 0;
    func_0207ed3c();
}
