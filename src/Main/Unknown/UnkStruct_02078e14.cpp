#include "Unknown/UnkStruct_02078e14.hpp"
#include "Unknown/UnkStruct_02071d24.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/SoundManager.hpp"
#include "System/WirelessManager.hpp"
#include "UI/SelectWindow.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

extern u8 data_020d43db[];
void operator delete(void *ptr);
extern u8 data_020d4946[];
extern u8 data_020d4404[];

class UnkStruct_02076058 {
public:
    /* 00 */ PAD(0x00, 0x78);
    /* 78 */ u8 mUnk_78;
    /* 79 */ PAD(0x79, 0x80);
    UnkStruct_02076058(u8 unk1);
    void func_02077a30(u32 a, u32 b, u32 c, u8 d);
};
BOOL func_02078270(UnkStruct_02076058 *obj);
u32 func_020779ec(void *obj);


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

#define UPDATE_TAIL()                      \
    func_0207eec4();                       \
    mUnk_74[18]->Update();                 \
    mUnk_74[18]->Draw(1, 0x80, 0x60, 0);   \
    return

void UnkStruct_02078e14::Update() {
    if (mUnk_404 != -1) {
        GXS_SetVisiblePlane(mUnk_404);
        mUnk_404 = -1;
        if (mUnk_e1 == 3) {
            if (mUnk_f0 != 0) {
                mUnk_3f9 = 2;
            } else {
                mUnk_3f9 = 1;
            }
        }
    }
    if (mUnk_3fa != 0) {
        mUnk_3fa--;
        if (mUnk_3fa == 0 && mUnk_4c != NULL) {
            mUnk_4c->mCells[0]->SetAnimation(2);
        }
    }
    if (mUnk_3ff != 0 && mUnk_3fc == 0) {
        SoundManager::Get().func_020246f8(0x66);
        mUnk_3ff = 0;
        u32 one = 1;
        if (mUnk_e6 < 4) {
            mUnk_c8[mUnk_e6 - 1] = mUnk_106[mUnk_fa];
            mUnk_c8[mUnk_e6 + 8] = mUnk_104;
            mUnk_f0              = one;
            mUnk_3f9             = 2;
            mUnk_186[mUnk_e6]    = mUnk_fa;
            mUnk_194[0]          = one;
            mUnk_194[1]          = one;
            mUnk_194[2]          = one;
            func_02081128();
            data_020d433c.mUnk_14 |= 2;
        } else {
            mUnk_c8[mUnk_e6 - 1] = mUnk_106[mUnk_fa];
            mUnk_f0              = one;
            mUnk_3f9             = 2;
            mUnk_186[mUnk_e6]    = mUnk_fa;
            mUnk_194[0]          = one;
            mUnk_194[1]          = one;
            mUnk_194[2]          = one;
            func_02081594();
            data_020d433c.mUnk_14 |= 2;
        }
    }
    if (mUnk_3fc > 0) {
        mUnk_3fc -= mUnk_3fe;
        if (mUnk_3fc <= 0) {
            mUnk_3fc = 0;
        }
    }
    switch (mUnk_e1) {
        case 0:
            func_0207b3a8();
            func_0207b6e8();
            func_0207b760();
            func_0207c568();
            mUnk_e1++;
            if (mUnk_e0 == 5) {
                mUnk_e1 = 7;
                func_020851d0();
                return;
            }
            break;
        case 1:
            mUnk_48->mUnk_064 = 0;
            mUnk_4c->mUnk_064 = 0;
            if (!func_020842a0()) {
                SpriteManager::Get().StartFade(2);
                mUnk_e1++;
            }
            break;
        case 2:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_4c->mUnk_064 = 1;
                mUnk_e1++;
            }
            break;
        case 3:
            func_0207cfa4();
            break;
        case 4:
            if (!func_020842a0()) {
                switch (mUnk_48->Update(1)) {
                    case 1:
                        mUnk_e1 = 5;
                        break;
                    case 2:
                        mUnk_e1 = 6;
                        break;
                }
            }
            break;
        case 5:
            mUnk_48->Update(1);
            if (mUnk_48->mUnk_061 == 4) {
                switch (mUnk_1a1) {
                    case 2:
                        switch (mUnk_e0) {
                            case 0:
                                data_020d43f0[0].mMoney -= mUnk_1dc;
                                func_020834ec(this, 4);
                                break;
                            case 1:
                                data_020d43f0[0].mMoney -= mUnk_1dc;
                                func_020834ec(this, 0);
                                mUnk_186[mUnk_e6] = mUnk_fa;
                                mUnk_e1           = 0x21;
                                break;
                            case 2:
                                data_020d43f0[0].mMoney -= mUnk_1dc;
                                func_020834ec(this, 6);
                                break;
                        }
                        SoundManager::Get().func_020246f8(0x69);
                        break;
                    case 3:
                        func_020834ec(this, 2);
                        func_02081128();
                        break;
                    case 4:
                    case 5:
                        func_0208415c(this);
                        func_020834ec(this, 10);
                        break;
                    case 10:
                        func_020834ec(this, 0);
                        mUnk_e1 = 0x26;
                        break;
                    case 6:
                        SetFlagBit(data_020d4946, mUnk_106[mUnk_fa]);
                        func_020834ec(this, 0);
                        break;
                    case 8:
                        func_02080210();
                        func_020834ec(this, 0);
                        SoundManager::Get().func_020246f8(0x68);
                        break;
                    case 9:
                        func_0208550c(this);
                        data_020d433c.mUnk_14 |= 2;
                        func_020834ec(this, 0);
                        SoundManager::Get().func_020246f8(0x67);
                        break;
                    default:
                        func_020834ec(this, 0);
                        break;
                }
            }
            break;
        case 6:
            mUnk_48->Update(1);
            if (mUnk_48->mUnk_061 == 4) {
                func_020834ec(this, 0);
            }
            break;
        case 7:
            mUnk_48->mUnk_064 = 0;
            mUnk_4c->mUnk_064 = 0;
            mUnk_50->mUnk_064 = 0;
            SpriteManager::Get().StartFade(2);
            mUnk_e1++;
            func_020851d0();
            return;
        case 8:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_50->mUnk_064 = 1;
                mUnk_e1++;
                if (mUnk_3f4 != 0) {
                    func_02085220(this, 0xc);
                }
            }
            func_020851d0();
            return;
        case 9:
            if (!func_020842a0()) {
                if (mUnk_50->Update(1) == 3) {
                    switch (mUnk_50->mCursor->mSelection) {
                        case 0:
                            func_020842e8();
                            func_02085220(this, 0xb);
                            break;
                        case 1:
                            mUnk_e6 = mUnk_f4;
                            mUnk_e7 = mUnk_f4;
                            mUnk_e8 = mUnk_f4;
                            mUnk_ea = mUnk_f4;
                            if (mUnk_1f1 != 0x15) {
                                mUnk_1a0 = 1;
                            }
                            mUnk_e1++;
                            break;
                    }
                }
            }
            func_020851d0();
            return;
        case 10:
            SpriteManager::Get().StartFade(3);
            mUnk_50->mUnk_064 = 0;
            mUnk_e1++;
            func_020851d0();
            return;
        case 11:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_e1++;
            }
            func_020851d0();
            return;
        case 12:
            if (func_020842a0()) {
                return;
            }
            func_020843ac(this, 0);
            mUnk_e1++;
            break;
        case 13:
            if (!func_020842a0()) {
                SpriteManager::Get().StartFade(2);
                mUnk_e1++;
            }
            break;
        case 14:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_4c->mUnk_064 = 1;
                mUnk_e1           = 3;
            }
            break;
        case 15:
            SpriteManager::Get().StartFade(3);
            mUnk_4c->mUnk_064 = 0;
            mUnk_e1++;
            break;
        case 16:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_e1++;
            }
            break;
        case 17:
            if (func_020842a0()) {
                return;
            }
            func_020843ac(this, 1);
            mUnk_e1++;
            func_020851d0();
            return;
        case 18:
            if (!func_020842a0() && mUnk_1a0 == 0) {
                SpriteManager::Get().StartFade(2);
                mUnk_e1++;
            }
            func_020851d0();
            return;
        case 19:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_50->mUnk_064 = 1;
                mUnk_e1           = 9;
            }
            func_020851d0();
            return;
        case 20: {
            switch (mUnk_48->Update(1)) {
                case 1:
                    mUnk_e1 = 0x15;
                    break;
                case 2:
                    mUnk_e1 = 0x16;
                    break;
            }
            func_020851d0();
            return;
        }
        case 21:
            mUnk_48->Update(1);
            if (mUnk_48->mUnk_061 == 4) {
                switch (mUnk_1a1) {
                    case 0xb:
                        SoundManager::Get().func_020246f8(0x6a);
                        func_02085220(this, 0);
                        mUnk_50->mCells[0]->SetAnimation(0);
                        mUnk_e1 = 0x17;
                        WirelessManager::Get();
                        if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                            mUnk_50->mUnk_064 = 0;
                            mUnk_e1           = 0x1b;
                            UPDATE_TAIL();
                        }
                        break;
                    case 0xc:
                        func_02085220(this, 0);
                        break;
                }
            }
            func_020851d0();
            return;
        case 22:
            mUnk_48->Update(1);
            if (mUnk_48->mUnk_061 == 4) {
                func_02085220(this, 0);
            }
            func_020851d0();
            return;
        case 23:
            mUnk_50->mUnk_064 = 0;
            if (mUnk_1f1 == 0x15) {
                func_020848c8(this);
            }
            MI_CpuCopy8(data_020d4404, (u8 *) ((u32 *) data_020d433c.mBattlePlayers)[3] + 0xa8, 8);
            SpriteManager::Get().StartFade(3);
            mUnk_e1++;
            func_020851d0();
            return;
        case 24:
            if (!SpriteManager::Get().IsFading()) {
                func_0207966c(1);
                mUnk_e1++;
                return;
            }
            func_020851d0();
            return;
        case 25:
            if (mContext->mUnk_12 != 0) {
                mUnk_e1++;
            }
            return;
        case 26:
            RequestScene(1);
            mFinished = 1;
            return;
        case 27:
            if (func_02084a10(this)) {
                mUnk_e1++;
            }
            UPDATE_TAIL();
        case 28:
            if (func_02084c38(this)) {
                mUnk_e1++;
            }
            UPDATE_TAIL();
        case 29:
            if (func_0208501c(this)) {
                mUnk_e1++;
            }
            UPDATE_TAIL();
        case 30:
            if (func_02084f40(this)) {
                mUnk_e1++;
            }
            UPDATE_TAIL();
        case 31:
            if (func_02084e64(this)) {
                SpriteManager::Get().StartFade(3);
                mUnk_e1++;
            }
            UPDATE_TAIL();
        case 32:
            if (!SpriteManager::Get().IsFading()) {
                func_0207966c(1);
                mUnk_e1 = 0x19;
                return;
            }
            UPDATE_TAIL();
        case 33:
            SpriteManager::Get().StartSubFade(3);
            mUnk_e1++;
            break;
        case 34:
            SpriteManager::Get();
            if (SpriteManager::sFade.mSub <= 1) {
                if (mUnk_c0 != NULL) {
                    mUnk_c0->func_02058530(0);
                }
                func_0207966c(0);
                mUnk_1e0 = new UnkStruct_02076058(1);
                mUnk_1e0->func_02077a30(1, mUnk_106[mUnk_fa] & 0xff, mUnk_104, 1);
                mContext->mUnk_12 = 0;
                mUnk_e1++;
                return;
            }
            break;
        case 35:
            if (func_02078270(mUnk_1e0)) {
                UnkStruct_02023248 *pair = func_02023248(&data_020d43f0[0], mUnk_106[mUnk_fa] & 0xff, mUnk_104);
                u16 val = func_020231f4(&data_020d43f0[0], 0, mUnk_106[mUnk_fa] & 0xff, mUnk_104);
                func_0205afb4(mUnk_c0, mUnk_104, val, (BattleCellPair *) pair);
            }
            if (func_020779ec(mUnk_1e0) == 0) {
                return;
            }
            if (mUnk_c0 != NULL) {
                func_0205821c(mUnk_c0);
            }
            mUnk_e1++;
            return;
        case 36:
            if (mUnk_c0 != NULL) {
                if (mContext->mUnk_12 == 0) {
                    return;
                }
                mUnk_1e0->mUnk_78 = 1;
                mUnk_c0           = NULL;
            }
            mUnk_e1++;
            return;
        case 37:
            if (mContext->mUnk_12 != 3) {
                return;
            }
            mContext->mUnk_12 = 0;
            mUnk_1e0          = NULL;
            mUnk_e1           = 0;
            mUnk_f0           = 0;
            mUnk_1a0          = 4;
            mUnk_194[0]       = 1;
            mUnk_194[1]       = 1;
            mUnk_194[2]       = 1;
            mUnk_3f9          = 1;
            return;
        case 38:
            SpriteManager::Get().StartFade(3);
            mUnk_e1++;
            break;
        case 39:
            if (!SpriteManager::Get().IsFading()) {
                func_0207966c(1);
                mUnk_e1++;
                return;
            }
            break;
        case 40:
            if (mContext->mUnk_12 == 0) {
                return;
            }
            mUnk_1e4 = new UnkStruct_02071d24(2);
            mUnk_1e4->func_02074148(0, mUnk_106[mUnk_fa] & 0xff, mUnk_104, 1);
            mContext->mUnk_12 = 0;
            mUnk_e1++;
            return;
        case 41:
            if (mContext->mUnk_12 != 2) {
                return;
            }
            mContext->mUnk_12 = 0;
            mUnk_1e4          = NULL;
            mUnk_e1           = 0;
            mUnk_f0           = 1;
            mUnk_1a0          = 4;
            mUnk_194[0]       = 1;
            mUnk_194[1]       = 1;
            mUnk_194[2]       = 1;
            mUnk_3f9          = 2;
            return;
        case 43:
            if (mUnk_e0 == 3 || mUnk_e0 == 5) {
                func_020842e8();
            }
            SpriteManager::Get().StartFade(3);
            mUnk_48->mUnk_064 = 0;
            mUnk_4c->mUnk_064 = 0;
            mUnk_e1++;
            break;
        case 44:
            if (!SpriteManager::Get().IsFading()) {
                func_0207966c(1);
                mUnk_e1++;
                return;
            }
            break;
        case 45:
            if (mContext->mUnk_12 != 0) {
                mUnk_e1++;
            }
            return;
        case 46:
            RequestScene(1);
            mFinished = 1;
            return;
    }
    func_0207ed3c();
    func_0207dea4();
    func_0207e074();
    func_0207e424();
    func_0207ea3c();
    func_0207eec4();
    func_0208403c();
    func_0207ceb0();
    func_02080104();
    func_0207f2f0();
    func_0207f32c();
    func_0207f90c();
}
