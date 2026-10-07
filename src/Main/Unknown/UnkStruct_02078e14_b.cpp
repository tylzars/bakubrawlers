#include "Unknown/UnkStruct_02078e14.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Graphics/CellAnimation.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include "System/InputManager.hpp"
#include "System/TouchManager.hpp"
#include "UI/SelectWindow.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

void UnkStruct_02078e14::func_0207b3a8() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 0);
    G2_SetBG2Control(0, 1, 4, 8, 0);
    G2_SetBG3Control(0, 1, 6, 0xc, 0);
    G2S_SetBG0Control(0, 1, 0, 4, 0);
    G2S_SetBG1Control(0, 1, 2, 1, 0);
    G2S_SetBG2Control(0, 1, 4, 4, 0);
    G2S_SetBG3Control(0, 1, 6, 4, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0x1e0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(2);
    G2S_SetBG3Priority(1);
    GX_SetVisiblePlane(0x11);
    if (mUnk_e0 == 4) {
        GXS_SetVisiblePlane(0x19);
    } else if (mUnk_e0 == 5) {
        GXS_SetVisiblePlane(0x11);
    } else {
        GXS_SetVisiblePlane(0x1d);
    }
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_SetWnd0InsidePlane(0x1f, FALSE);
    G2_SetWnd1InsidePlane(0x1f, FALSE);
    G2_SetWndOBJInsidePlane(0x1f, FALSE);
    G2_SetWnd0Position(0, 0, 0, 0);
    G2_SetWnd1Position(0, 0, 0, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWnd1InsidePlane(0x1f, FALSE);
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWnd0Position(0, 0, 0, 0);
    G2S_SetWnd1Position(0, 0, 0, 0);
    G2_BlendNone();
    G2S_BlendNone();
}

void UnkStruct_02078e14::func_0207b6e8() {
    switch (mUnk_e0) {
        case 0:
        case 1:
            mUnk_e9 = 6;
            break;
        case 2:
            mUnk_e9 = 6;
            break;
        case 3:
        case 5:
            mUnk_e9 = 10;
            break;
        case 4:
            mUnk_e9 = 13;
            func_020832bc();
            break;
    }
    func_02080210();
    func_02080bdc(1);
}

void UnkStruct_02078e14::func_0207c568() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    ChangeDir("/font/");
    mUnk_0c    = func_02023504("mes_storecomment.mes", 0, 0, 0, 0, 0);
    mUnk_10    = func_02023504("mes_BakuganNameShort.mes", 0, 0, 0, 0, 0);
    mUnk_14    = func_02023504("mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mUnk_18    = func_02023504("mes_CardNameShort.mes", 0, 0, 0, 0, 0);
    mUnk_1c    = func_02023504("mes_DeckSelect.mes", 0, 0, 0, 0, 0);
    mUnk_20    = func_02023504("mes_Result.mes", 0, 0, 0, 0, 0);
    mUnk_38[0] = TextManager::Get().BufferSize(0x18, 0x10);
    mUnk_28[0] = OS_Alloc(mUnk_38[0]);
    MI_CpuClear16(mUnk_28[0], mUnk_38[0]);
    mUnk_40[0] = TextManager::Get().GlyphCount(6);
    mUnk_30[0] = OS_Alloc((s16) mUnk_40[0] * (s16) 0xc);
    TextManager::Get().Init(mUnk_30[0], 1, 0, 0x18, 0x10, 0x44);
    TextManager::Get().SetColor(1, 0, 0xf);
    u8 *alphabet = (u8 *) func_02023504("mes_Alphabet.mes", 0, 0, 0, 0, 0);
    TextManager::Get();
    mUnk_24 = *(u16 *) (alphabet + *(u16 *) (alphabet + 2) + 0xb4);
    OS_Free(alphabet);
    switch (mUnk_e0) {
        case 0:
            mUnk_1ec = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mUnk_1ec->func_02065890(0xb, 0, 1);
            break;
        case 1:
            mUnk_1ec = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mUnk_1ec->func_02065890(0xd, 0, 1);
            break;
        case 2:
            mUnk_1ec = new MessageWindow("mes_storecomment", 1, 0x1b000);
            mUnk_1ec->func_02065890(0xc, 0, 1);
            break;
        case 3:
            mUnk_1ec = new MessageWindow("mes_DeckSelect", 1, 0x1b000);
            mUnk_1ec->func_02065890(6, 0, 1);
            break;
        case 5:
            mUnk_1ec = new MessageWindow("mes_DeckSelect", 1, 0x1b000);
            mUnk_1ec->func_02065890(5, 0, 1);
            break;
        case 4:
            mUnk_1ec = new MessageWindow("mes_myroom", 1, 0x1b000);
            mUnk_1ec->func_02065890(0x30, 0, 1);
            break;
    }
    if (mUnk_1ec != NULL) {
        mUnk_1ec->func_02065bd4(0x14, 4, 0);
        mUnk_1ec->func_02065be4(0xd8);
    }
}

u32 func_020231f4(UnkStruct_020d43f0 *data, u32 unk2, u32 unk3, u32 unk4);

void UnkStruct_02078e14::func_0207ca60() {
    u8 i = 0;
    func_0207cf84();
    switch (mUnk_e0) {
        case 0:
            for (; i < 4; i++) {
                s32 index = mUnk_fd + i;
                if (index >= mUnk_fc) {
                    break;
                }
                u32 a = func_02086548((u8) mUnk_106[index], mUnk_104);
                u32 b = func_02086570((u8) mUnk_106[index], mUnk_104);
                func_02081d5c((u8) mUnk_106[mUnk_fd + i], i, a, b);
            }
            break;
        case 1:
            for (; i < 4; i++) {
                if (mUnk_fd + i >= mUnk_fc) {
                    break;
                }
                UnkStruct_02023248 *entry = func_02023248(data_020d43f0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                u32 v = func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                func_02081d5c((u8) mUnk_106[mUnk_fd + i], i, v, (u8) entry->mUnk_00_0);
            }
            break;
        case 2:
            for (; i < 4; i++) {
                if (mUnk_fd + i >= mUnk_fc) {
                    break;
                }
                func_0208250c(mUnk_106[mUnk_fd + i], i);
            }
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                break;
            }
            if (mUnk_e6 < 4) {
                for (; i < 4; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        break;
                    }
                    UnkStruct_02023248 *entry = func_02023248(data_020d43f0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                    u32 v = func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                    func_02081d5c((u8) mUnk_106[mUnk_fd + i], i, v, (u8) entry->mUnk_00_0);
                }
            } else {
                for (; i < 4; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        break;
                    }
                    func_0208250c(mUnk_106[mUnk_fd + i], i);
                }
            }
            break;
        case 4:
            if (mUnk_e6 == 0) {
                break;
            }
            if (mUnk_e6 < 7) {
                for (; i < 4; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        break;
                    }
                    UnkStruct_02023248 *entry = func_02023248(data_020d43f0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                    u32 v = func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fd + i], mUnk_104);
                    func_02081d5c((u8) mUnk_106[mUnk_fd + i], i, v, (u8) entry->mUnk_00_0);
                }
            } else {
                for (; i < 4; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        break;
                    }
                    func_0208250c(mUnk_106[mUnk_fd + i], i);
                }
            }
            break;
    }
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0, mUnk_28[0], mUnk_38[0], 0);
}

void UnkStruct_02078e14::func_0207ceb0() {
    if (mUnk_1a1 != 0) {
        return;
    }
    if (mUnk_1ec != NULL) {
        mUnk_1ec->func_020658d0();
    }
    SpriteManager::Get().Screen(1).func_02027778(mUnk_30[0], (u8) mUnk_40[0], 0x41, (s16) (mUnk_f8 + 0x48), 2, 0, 0xf, 0);
}

void UnkStruct_02078e14::func_0207cf84() {
    if (mUnk_28[0] != NULL) {
        MI_CpuClear16(mUnk_28[0], mUnk_38[0]);
    }
}

void UnkStruct_02078e14::func_0207dea4() {
    switch (mUnk_1a0) {
        case 1:
            mUnk_4c->mUnk_064 = 0;
            if (mUnk_c0 != NULL) {
                func_0205821c(mUnk_c0);
                mContext->mUnk_12 = 0;
                mUnk_400          = 1;
                mUnk_1a0          = 3;
            } else if (mUnk_c4 == NULL) {
                mUnk_1a0 = 2;
            } else {
                func_02055624(mUnk_c4, 1);
                mContext->mUnk_12 = 0;
                mUnk_401          = 1;
                mUnk_1a0          = 3;
            }
            SpriteManager::Get().sFade.mMain = 3;
            break;
        case 2:
            if (SpriteManager::Get().sFade.mMain <= 1) {
                func_02082660();
                mUnk_1a0 = 4;
            }
            break;
        case 3:
            if (mContext->mUnk_12 != 0) {
                if (mUnk_c0 != NULL) {
                    mUnk_c0 = NULL;
                }
                if (mUnk_c4 != NULL) {
                    mUnk_c4 = NULL;
                }
                mUnk_1a0 = 4;
            }
            break;
        case 4:
            func_0208038c();
            mUnk_1a0        = 0;
            mUnk_4c->mUnk_064 = 1;
            break;
    }
}

void UnkStruct_02078e14::func_0207e074() {
    if (mUnk_194[0] != 0) {
        mUnk_194[0] = 0;
        if (mUnk_e8 > mUnk_f4 && mUnk_f0 != 0) {
            func_02081994(0, 1, 8);
            mUnk_4c->mCells[1]->func_02029fc0();
        } else {
            func_02081994(0, 1, 0x8000);
        }
        if (mUnk_e8 < mUnk_e9 - 1 && mUnk_f0 != 0) {
            func_02081994(0, 2, 8);
            mUnk_4c->mCells[2]->func_02029fc0();
        } else {
            func_02081994(0, 2, 0x8000);
        }
        for (u8 i = 0; i <= 4; i++) {
            if (mUnk_e8 + i >= mUnk_e9 || mUnk_f0 == 0) {
                func_02081994(2, i, 0x8000);
            } else {
                func_02081994(2, i, 8);
            }
        }
        for (u8 i = 5; i <= 7; i++) {
            if (i - 5 >= mUnk_fc || mUnk_f0 != 0) {
                func_02081994(2, i, 0x8000);
            } else {
                func_02081994(2, i, 8);
            }
        }
        func_02081994(0, 0, 8);
        if (mUnk_fc < 2 || mUnk_f0 != 0) {
            func_02081994(0, 5, 0x8000);
            func_02081994(0, 6, 0x8000);
        } else {
            func_02081994(0, 5, 8);
            mUnk_4c->mCells[5]->func_02029fc0();
            func_02081994(0, 6, 8);
            mUnk_4c->mCells[6]->func_02029fc0();
        }
        switch (mUnk_e0) {
            case 0:
            case 1:
            case 2:
            case 4:
                func_02081994(0, 3, 0x8000);
                func_02081994(0, 4, 0x8000);
                break;
            case 3:
            case 5:
                if (mUnk_e6 != 0 && mUnk_e6 < 4 && mUnk_f0 == 0) {
                    func_02081994(0, 3, 8);
                    func_02081994(0, 4, 8);
                    mUnk_74[7]->SetAnimation((mUnk_104 + 5) % 6 * 2 + 0x10);
                    mUnk_74[8]->SetAnimation((mUnk_104 + 1) % 6 * 2 + 0x10);
                } else {
                    func_02081994(0, 3, 0x8000);
                    func_02081994(0, 4, 0x8000);
                }
                break;
        }
        if (mUnk_f0 != 0) {
            mUnk_54->mSelection = 0;
        } else {
            mUnk_54->mSelection = mUnk_fa - mUnk_fd + 5;
        }
    }
    mUnk_e2 = mUnk_4c->Update(0);
}

void UnkStruct_02078e14::func_0207e424() {
    switch (mUnk_f5) {
        case 2:
            if (InputManager::Get().IsDown(0x20) == 0 && mUnk_ec == 0) {
                func_0207e7e4();
            }
            break;
        case 4:
            if (InputManager::Get().IsDown(0x10) == 0 && mUnk_ec == 0) {
                func_0207e7e4();
            }
            break;
        case 5:
            if (TouchManager::Get().mTouching != 0) {
                mUnk_f5 = 0;
            }
            break;
        case 6:
            if (TouchManager::Get().mTouching != 0 && mUnk_ec == 0) {
                func_0207e7e4();
            }
            break;
        case 7:
            if (TouchManager::Get().mTouching != 0) {
                mUnk_f5 = 0;
            }
            break;
        case 8:
            if (TouchManager::Get().mTouching != 0 && mUnk_ec == 0) {
                func_0207e7e4();
            }
            break;
        case 9:
            if (TouchManager::Get().mTouching != 0) {
                mUnk_f5 = 0;
            }
            break;
    }
    if (mUnk_ec != 0 || mUnk_e7 != mUnk_e8) {
        if (mUnk_ec == 0) {
            mUnk_ec = 1;
            if (mUnk_e7 <= mUnk_e8) {
                mUnk_e4 = -0x2c;
                mUnk_ea--;
                mUnk_e8--;
            }
        }
        if (mUnk_e7 > mUnk_e8) {
            mUnk_e4 -= 0xb;
        } else {
            mUnk_e4 += 0xb;
        }
        if (mUnk_e4 >= 0 || mUnk_e4 <= -0x2c) {
            mUnk_ec = 0;
            if (mUnk_e7 > mUnk_e8) {
                mUnk_ea++;
                mUnk_e8++;
            }
            mUnk_e4 = 0;
            if (mUnk_e7 == mUnk_e8) {
                mUnk_194[0] = 1;
                if (func_020862d0()) {
                    func_0207e7e4();
                }
            }
        }
    }
    if (mUnk_194[1] != 0) {
        if (func_020842a0() == 0) {
            func_020807cc();
            mUnk_194[1] = 0;
        }
    }
}

void UnkStruct_02078e14::func_0207e7e4() {
    if (mUnk_f5 != 5 && mUnk_f5 != 7) {
        mUnk_f5 = 0;
    }
    mUnk_e6      = mUnk_e7;
    mUnk_194[2] = 1;
    switch (mUnk_e0) {
        case 0:
        case 1:
        case 2:
            mUnk_104 = mUnk_e6;
            func_02080bdc(1);
            func_02081128();
            func_02081594();
            break;
        case 3:
        case 5:
            func_02085460(-1);
            if (mUnk_e6 == 0) {
                func_02080bdc(1);
                if (mUnk_1a0 == 0) {
                    mUnk_1a0 = 1;
                }
            } else if (mUnk_e6 < 4) {
                mUnk_104 = mUnk_c8[mUnk_e6 + 8];
                func_02080bdc(1);
                if (mUnk_c0 == NULL || mUnk_400 != 0) {
                    if (mUnk_1a0 == 0) {
                        mUnk_1a0 = 1;
                    }
                } else {
                    func_02081128();
                }
            } else {
                mUnk_104 = mUnk_e6 - 4;
                func_02080bdc(1);
                if (mUnk_c4 == NULL || mUnk_401 != 0) {
                    if (mUnk_1a0 == 0) {
                        mUnk_1a0 = 1;
                    }
                } else {
                    func_02081594();
                }
            }
            break;
        case 4:
            if (mUnk_e6 == 0) {
                func_02080bdc(1);
                if (mUnk_1a0 == 0) {
                    mUnk_1a0 = 1;
                }
                GXS_SetVisiblePlane(0x19);
            } else if (mUnk_e6 < 7) {
                mUnk_104 = mUnk_e6 - 1;
                func_02080bdc(1);
                if (mUnk_c0 == NULL || mUnk_400 != 0) {
                    if (mUnk_1a0 == 0) {
                        mUnk_1a0 = 1;
                    }
                } else {
                    func_02081128();
                }
                GXS_SetVisiblePlane(0x1d);
            } else {
                mUnk_104 = mUnk_e6 - 7;
                func_02080bdc(1);
                if (mUnk_c4 == NULL || mUnk_401 != 0) {
                    if (mUnk_1a0 == 0) {
                        mUnk_1a0 = 1;
                    }
                } else {
                    func_02081594();
                }
                GXS_SetVisiblePlane(0x1d);
            }
            break;
    }
}

void func_02081a4c(void *arg);

// non-matching: 2 words, the original materialises `moved = 1` with `mov r4, #1` where the compiler reuses r2 (`mov r4, r2`)
void UnkStruct_02078e14::func_0207ea3c() {
    if (mUnk_100 != 0 || mUnk_fb != mUnk_fa) {
        if (mUnk_100 == 0 && mUnk_fd <= mUnk_fb && mUnk_fb < mUnk_fd + 3) {
            mUnk_fa = mUnk_fb;
            func_02081128();
            func_02081594();
            func_02085460(-1);
            mUnk_194[0] = 1;
        } else if (func_020842a0() == 0) {
            u32 moved = 0;
            if (mUnk_100 == 0) {
                mUnk_100 = 1;
                if (mUnk_fb > mUnk_fa) {
                    if (mUnk_fb - mUnk_fa > 1) {
                        mUnk_fa = mUnk_fb;
                        moved   = 1;
                        mUnk_fd = mUnk_fb;
                        if (mUnk_fd >= mUnk_fc - 3) {
                            mUnk_fd = mUnk_fc - 3;
                        }
                    } else {
                        func_02081c2c((u8) mUnk_106[mUnk_fd + 3], (u8) ((mUnk_fd + 3) % 4), 2);
                        func_020823d8(mUnk_106[mUnk_fd + 3], (u8) ((mUnk_fd + 3) % 4), 2);
                    }
                } else {
                    if (mUnk_fa - mUnk_fb > 1) {
                        mUnk_fa = mUnk_fb;
                        moved   = 1;
                        mUnk_fd = mUnk_fb;
                        if (mUnk_fd >= mUnk_fc - 3) {
                            mUnk_fd = mUnk_fc - 3;
                        }
                    } else {
                        mUnk_f8 = -0x20;
                        mUnk_fd--;
                        mUnk_fa--;
                        func_02081c2c((u8) mUnk_106[mUnk_fd], (u8) (mUnk_fd % 4), 2);
                        func_020823d8(mUnk_106[mUnk_fd], (u8) (mUnk_fd % 4), 2);
                        func_0207ca60();
                    }
                }
            }
            if (moved == 0) {
                if (mUnk_fb > mUnk_fa) {
                    mUnk_f8 -= 8;
                } else {
                    mUnk_f8 += 8;
                }
                if (mUnk_f8 >= 0 || mUnk_f8 <= -0x20) {
                    mUnk_100 = 0;
                    if (mUnk_fb > mUnk_fa) {
                        mUnk_fd++;
                        mUnk_fa++;
                        func_0207ca60();
                    }
                    mUnk_f8 = 0;
                    func_02081128();
                    func_02081594();
                    mUnk_194[0] = 1;
                }
                func_020254f0(func_02081a4c, this);
            } else {
                mUnk_100 = 0;
                func_02081128();
                func_02081594();
                mUnk_194[0] = 1;
                mUnk_194[2] = 1;
            }
        }
    }
    if (mUnk_194[2] != 0) {
        if (func_020842a0() == 0) {
            func_02080898();
            mUnk_194[2] = 0;
        }
        func_0207ca60();
    }
}

void UnkStruct_02078e14::func_0207ed3c() {
    if (mUnk_3f8 == mUnk_3f9) {
        return;
    }
    switch (mUnk_3f9) {
        case 0:
            GXS_SetVisibleWnd(0);
            G2S_SetWndOutsidePlane(0x1f, FALSE);
            G2S_SetWnd0InsidePlane(0x1f, FALSE);
            G2S_SetWnd1InsidePlane(0x1f, FALSE);
            G2S_SetBlendBrightness(0x1f, 0);
            break;
        case 1:
            GXS_SetVisibleWnd(3);
            G2S_SetWnd0Position(0x18, 0x21, 0x38, 0x3f);
            G2S_SetWnd1Position(1, 0x21, 0xff, 0x3f);
            G2S_SetWndOutsidePlane(0x1f, FALSE);
            G2S_SetWnd0InsidePlane(0x1f, FALSE);
            G2S_SetWnd1InsidePlane(0x3f, FALSE);
            G2S_SetBlendBrightness(0x1f, -8);
            break;
        case 2:
            GXS_SetVisibleWnd(3);
            G2S_SetWnd0Position(0, 0x41, 0x80, 0xa8);
            G2S_SetWnd1Position(0x80, 0x41, 0, 0xa8);
            G2S_SetWndOutsidePlane(0x1f, FALSE);
            G2S_SetWnd0InsidePlane(0x3f, FALSE);
            G2S_SetWnd1InsidePlane(0x3f, FALSE);
            G2S_SetBlendBrightness(0x1f, -8);
            break;
    }
    mUnk_3f8 = mUnk_3f9;
}

void UnkStruct_02078e14::func_0207eec4() {
    if (mUnk_c0 != NULL || mUnk_c4 != NULL) {
        return;
    }
    if (mUnk_1a0 > 2) {
        return;
    }
    u8 i = 0;
    switch (mUnk_e0) {
        case 3:
        case 5:
            if (mUnk_5c[0] != NULL) {
                mUnk_5c[0]->Draw(0, 0x80, 0x60, 0);
            }
            if (mUnk_5c[3] != NULL) {
                mUnk_5c[3]->func_0202a3ec(0, 0, 0x80, 0x60, 0);
            }
            for (; i < 9; i++) {
                if (i < 3) {
                    if (mUnk_3f4 == 0 || i == 0) {
                        func_02081a74((u8) mUnk_c8[9 + i], i, 0, i * 24 + 0x15, 0x57);
                    }
                } else {
                    func_02082264(i - 3, i - 3, 0, (i + 1) * 24, 0x55);
                }
            }
            {
                s32 n = func_0208644c();
                if ((u32) n < 1000) {
                    mUnk_5c[5]->func_0202a3ec(0, n / 100, 0x6e, 0x94, 0);
                    mUnk_5c[5]->func_0202a3ec(0, n / 10 % 10, 0x7a, 0x94, 0);
                    mUnk_5c[5]->func_0202a3ec(0, n % 10, 0x86, 0x94, 0);
                } else {
                    mUnk_5c[5]->func_0202a3ec(0, n / 1000, 0x68, 0x94, 0);
                    mUnk_5c[5]->func_0202a3ec(0, n / 100 % 10, 0x74, 0x94, 0);
                    mUnk_5c[5]->func_0202a3ec(0, n / 10 % 10, 0x80, 0x94, 0);
                    mUnk_5c[5]->func_0202a3ec(0, n % 10, 0x8c, 0x94, 0);
                }
            }
            break;
        case 4:
            if (mUnk_5c[1] != NULL) {
                mUnk_5c[1]->Draw(i, 0x80, 0x60, i);
            }
            if (mUnk_5c[3] != NULL) {
                mUnk_5c[3]->func_0202a3ec(0, 1, 0x80, 0x60, 0);
            }
            func_020831f4();
            if (mUnk_5c[2] == NULL) {
                return;
            }
            for (; i < 14; i++) {
                s16 x = i / 7 * 0x78 + 0x24;
                s16 y = i % 7 * 0x14 + 0x2c;
                if (i % 7 == 6) {
                    y += 6;
                }
                mUnk_5c[2]->func_0202a3ec(0, func_0208349c(i), x, y, 0);
            }
            break;
    }
}

void UnkStruct_02078e14::func_0207f2f0() {
    if (mUnk_4c != NULL) {
        mUnk_4c->Draw();
    }
    if (mUnk_48 != NULL) {
        mUnk_48->Draw();
    }
    if (mUnk_50 != NULL) {
        mUnk_50->Draw();
    }
}

void UnkStruct_02078e14::func_0207f32c() {
    if (mUnk_3ff != 0) {
        if (mUnk_e6 < 4) {
            func_02081a74(mUnk_104, (u8) (mUnk_fa % 4), 4, 0x1d, mUnk_3fc + 0x25);
        } else {
            func_02082264(mUnk_104, (u8) (mUnk_fa % 4), 4, 0x20, mUnk_3fc + 0x24);
        }
    }
    u8 limit = 6;
    u8 i     = 0;
    if (mUnk_ec == 0) {
        limit--;
    }
    do {
        if (mUnk_ea + i >= mUnk_e9) {
            break;
        }
        switch (mUnk_e0) {
            case 0:
            case 1:
                mUnk_74[4]->func_0202a3ec(1, (u8) (mUnk_ea + i), mUnk_e4 + 0x1e + i * 0x2c, 0x26, 1);
                break;
            case 2:
                mUnk_74[5]->func_0202a3ec(1, (u8) (mUnk_ea + i), mUnk_e4 + 0x20 + i * 0x2c, 0x24, 1);
                break;
            case 3:
            case 5:
                if ((mUnk_ea + i) == 0) {
                    mUnk_74[10]->SetAnimation(0x1d);
                    mUnk_74[10]->Draw(1, (s16) (mUnk_e4 + 0x19 + i * 0x2c), 0x21, 1);
                } else if ((mUnk_ea + i) < 4) {
                    if (mUnk_3f4 == 0 || (mUnk_ea + i) == 1) {
                        func_02081a74((u8) mUnk_c8[(mUnk_ea + i) + 8], (u8) ((mUnk_ea + i) - 1), 1, mUnk_e4 + 0x1d + i * 0x2c, 0x25);
                    }
                } else {
                    func_02082264((mUnk_ea + i) - 4, (mUnk_ea + i) - 4, 1, mUnk_e4 + 0x20 + i * 0x2c, 0x24);
                }
                break;
            case 4:
                if ((mUnk_ea + i) == 0) {
                    mUnk_74[10]->SetAnimation(0x20);
                    mUnk_74[10]->Draw(1, (s16) (mUnk_e4 + 0x19 + i * 0x2c), 0x21, 1);
                } else if ((mUnk_ea + i) < 7) {
                    mUnk_74[4]->func_0202a3ec(1, (u8) ((mUnk_ea + i) - 1), mUnk_e4 + 0x1e + i * 0x2c, 0x26, 1);
                } else {
                    mUnk_74[5]->func_0202a3ec(1, (u8) ((mUnk_ea + i) - 7), mUnk_e4 + 0x20 + i * 0x2c, 0x24, 1);
                }
                break;
        }
        i++;
    } while (i < limit);
    mUnk_74[0]->Draw(1, 0x80, 0x60, 1);
    switch (mUnk_e0) {
        case 0:
        case 1:
            mUnk_74[1]->func_0202a3ec(1, 1, mUnk_e4 + 0x14 - mUnk_ea * 0x2c, 0x22, 1);
            break;
        case 2:
            mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x14 - mUnk_ea * 0x2c, 0x22, 1);
            mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x98 - mUnk_ea * 0x2c, 0x22, 1);
            break;
        case 3:
        case 5:
            if (mUnk_ea < 6) {
                mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x40 - mUnk_ea * 0x2c, 0x22, 1);
            }
            mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0xc4 - mUnk_ea * 0x2c, 0x22, 1);
            if (mUnk_ea != 0) {
                mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x148 - mUnk_ea * 0x2c, 0x22, 1);
            }
            break;
        case 4:
            if (mUnk_ea < 8) {
                mUnk_74[1]->func_0202a3ec(1, 1, mUnk_e4 + 0x40 - mUnk_ea * 0x2c, 0x22, 1);
            }
            if (mUnk_ea != 0) {
                mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x148 - mUnk_ea * 0x2c, 0x22, 1);
            }
            if (mUnk_ea > 3) {
                mUnk_74[1]->func_0202a3ec(1, 0, mUnk_e4 + 0x1cc - mUnk_ea * 0x2c, 0x22, 1);
            }
            break;
    }
}

extern u16 data_020a39fc[];

void UnkStruct_02078e14::func_0207f90c() {
    if (mUnk_e0 == 4 && mUnk_e6 == 0) {
        return;
    }
    mUnk_74[15]->Update();
    u8 count = 4;
    u8 i     = 0;
    if (mUnk_100 == 0) {
        count--;
    }
    do {
        if (mUnk_fd + i >= mUnk_fc) {
            if (mUnk_fc == 0 && i == 0) {
                mUnk_74[12]->SetAnimation(0x21);
                mUnk_74[12]->Draw(1, 0x71, (s16) (mUnk_f8 + 0x4b + i * 32), 2);
            }
        } else {
            switch (mUnk_e0) {
                case 0:
                    if (func_020231f4(data_020d43f0, 0, (u8) mUnk_106[mUnk_fd + i], mUnk_104) == 0) {
                        mUnk_74[15]->Draw(1, 0x19, (s16) (mUnk_f8 + 0x5b + i * 32), 2);
                    }
                    func_02081a74(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                    func_020816ec(func_020864ec((u8) mUnk_106[mUnk_fd + i], mUnk_104), i);
                    break;
                case 1:
                    func_02081a74(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                    func_020816ec(func_02086224((u8) mUnk_106[mUnk_fd + i], mUnk_104), i);
                    if (func_02082d00(1, (u8) mUnk_106[mUnk_fd + i], mUnk_104) != 0) {
                        mUnk_74[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                    }
                    break;
                case 2:
                    func_02082264(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x20, mUnk_f8 + 0x48 + i * 32);
                    func_020816ec(data_020a39fc[mUnk_106[mUnk_fd + i]], i);
                    break;
                case 3:
                case 5:
                    if (mUnk_e6 == 0) {
                        mUnk_74[11]->func_0202a3ec(1, (u8) (i + 1), 0x19, mUnk_f8 + 0x45 + i * 32, 2);
                        mUnk_74[12]->SetAnimation(0x1f);
                        mUnk_74[12]->func_0202a3ec(1, (u8) (i + 1), 0x71, mUnk_f8 + 0x4b + i * 32, 2);
                    } else if (mUnk_e6 < 4) {
                        func_02081a74(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                        if (func_02082d00(1, (u8) mUnk_106[mUnk_fd + i], mUnk_104) != 0) {
                            mUnk_74[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    } else {
                        func_02082264(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x20, mUnk_f8 + 0x48 + i * 32);
                        if (func_02082d00(0, (u8) mUnk_106[mUnk_fd + i], 0) != 0) {
                            mUnk_74[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    }
                    break;
                case 4:
                    if (mUnk_e6 < 7) {
                        func_02081a74(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x1d, mUnk_f8 + 0x49 + i * 32);
                        if (func_02082d00(1, (u8) mUnk_106[mUnk_fd + i], mUnk_104) != 0) {
                            mUnk_74[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    } else {
                        func_02082264(mUnk_104, (u8) ((mUnk_fd + i) % 4), 2, 0x20, mUnk_f8 + 0x48 + i * 32);
                        if (func_02082d00(0, (u8) mUnk_106[mUnk_fd + i], 0) != 0) {
                            mUnk_74[9]->Draw(1, 0x80, (s16) (mUnk_f8 + 0x54 + i * 32), 2);
                        }
                    }
                    break;
            }
        }
        i++;
    } while (i < count);
    if ((mUnk_e0 == 3 || mUnk_e0 == 5) && mUnk_e6 == 0) {
        return;
    }
    func_0207ff54();
}

void UnkStruct_02078e14::func_0207ff54() {
    s16 x = 0xb7;
    u8 a  = mUnk_fa + 1;
    if (mUnk_fc == 0) {
        a = 0;
    }
    mUnk_74[13]->SetAnimation(0);
    if (a >= 10) {
        mUnk_74[13]->func_0202a3ec(1, (u8) (a / 10), 0xb7, 0xaf, 1);
    }
    x += 6;
    mUnk_74[13]->func_0202a3ec(1, (u8) (a % 10), x, 0xaf, 1);
    mUnk_74[13]->SetAnimation(2);
    x += 6;
    mUnk_74[13]->Draw(1, x, 0xaf, 1);
    mUnk_74[13]->SetAnimation(0);
    x += 6;
    if (mUnk_fc >= 10) {
        mUnk_74[13]->func_0202a3ec(1, (u8) (mUnk_fc / 10), x, 0xaf, 1);
    }
    mUnk_74[13]->func_0202a3ec(1, (u8) (mUnk_fc % 10), x + 6, 0xaf, 1);
    mUnk_74[2]->Draw(1, 0xaf, 0xaa, 1);
}

void UnkStruct_02078e14::func_02080104() {
    switch (mUnk_e0) {
        case 0:
        case 1:
        case 2:
            func_020816ec(data_020d43f0[0].mMoney, 4);
            break;
        case 3:
        case 5:
            if (mUnk_4c->mButtons[3].mEnabled != 0) {
                mUnk_74[7]->Update();
                mUnk_74[7]->Draw(1, 5, 0x6c, 1);
            }
            if (mUnk_4c->mButtons[4].mEnabled != 0) {
                mUnk_74[8]->Update();
                mUnk_74[8]->Draw(1, 0xed, 0x6c, 1);
            }
            break;
    }
    mUnk_74[3]->Update();
    if (mUnk_f0 != 0) {
        mUnk_74[3]->Draw(1, 0x19, 0x21, 1);
    } else {
        mUnk_74[3]->Draw(1, 0x19, (s16) ((mUnk_fa - mUnk_fd) * 32 + 0x45), 1);
    }
}

u8 func_020230dc(UnkStruct_020d43f0 *d, u32 a, u32 b);

void UnkStruct_02078e14::func_02080210() {
    if (mUnk_1f1 != 0x15) {
        for (u8 i = 0; i < 3; i++) {
            u8 *entry = (u8 *) ((u32 *) data_020d433c.mBattlePlayers)[mUnk_1f0 + 3] + 0xc + i * 12;
            mUnk_c8[i] = entry[8];
            mUnk_c8[9 + i] = entry[9];
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[3 + i] = *(u16 *) ((u8 *) ((u32 *) data_020d433c.mBattlePlayers)[mUnk_1f0 + 3] + i * 4 + 0x54);
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[6 + i] = *(u16 *) ((u8 *) ((u32 *) data_020d433c.mBattlePlayers)[mUnk_1f0 + 3] + i * 4 + 0x6c);
        }
    } else {
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[i]     = func_020230dc(data_020d43f0, 0, i);
            mUnk_c8[9 + i] = func_020230a8(data_020d43f0, 0, i);
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[3 + i] = data_020d43f0[0].mUnk_06[i];
        }
        for (u8 i = 0; i < 3; i++) {
            mUnk_c8[6 + i] = data_020d43f0[0].mUnk_0c[i];
        }
    }
}

void UnkStruct_02078e14::func_0208038c() {
    switch (mUnk_e0) {
        case 0:
        case 1:
            if (mUnk_c0 != NULL) {
                return;
            }
            mUnk_c0 = new BattleCommandList(0, 0, -1, 0);
            func_02058404(mUnk_c0, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_400 = 0;
            func_02081128();
            break;
        case 2:
            if (mUnk_c4 != NULL) {
                return;
            }
            mUnk_c4 = new BattleCommandPage(0, -1);
            func_02055764(mUnk_c4, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_401 = 0;
            func_02081594();
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                func_020826ec();
                for (u8 i = 0; i < 9; i++) {
                    if (i < 3) {
                        func_02081c2c((u8) mUnk_c8[i], i, 0);
                    } else {
                        func_020823d8(mUnk_c8[i], (u8) (i - 3), 0);
                    }
                }
                SpriteManager::Get().sFade.mMain = 2;
                return;
            }
            if (mUnk_e6 < 4) {
                if (mUnk_c0 != NULL) {
                    return;
                }
                mUnk_c0 = new BattleCommandList(0, 0, -1, 0);
                func_02058404(mUnk_c0, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
                mUnk_400 = 0;
                func_02081128();
                return;
            }
            if (mUnk_c4 != NULL) {
                return;
            }
            mUnk_c4 = new BattleCommandPage(0, -1);
            func_02055764(mUnk_c4, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_401 = 0;
            func_02081594();
            return;
        case 4:
            if (mUnk_e6 == 0) {
                func_020826ec();
                SpriteManager::Get().sFade.mMain = 2;
                return;
            }
            if (mUnk_e6 < 7) {
                if (mUnk_c0 != NULL) {
                    return;
                }
                mUnk_c0 = new BattleCommandList(0, 0, -1, 0);
                func_02058404(mUnk_c0, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
                mUnk_400 = 0;
                func_02081128();
                return;
            }
            if (mUnk_c4 != NULL) {
                return;
            }
            mUnk_c4 = new BattleCommandPage(0, -1);
            func_02055764(mUnk_c4, 0x1d000, 0, 0xc0, 0x14, 0xe0, 0x14, 0);
            mUnk_401 = 0;
            func_02081594();
            break;
    }
}

void UnkStruct_02078e14::func_020807cc() {
    SpriteManager::Get();
    if (mUnk_e0 != 3 && mUnk_e0 != 5) {
        return;
    }
    for (u8 i = 0; i < 9; i++) {
        if (i < 3) {
            func_02081c2c((u8) mUnk_c8[i], i, 1);
        } else {
            func_020823d8(mUnk_c8[i], (u8) (i - 3), 1);
        }
    }
}

BOOL UnkStruct_02078e14::func_02080b48(u32 unk1, u32 unk2) {
    BOOL result = FALSE;
    if (unk1 == 0x22) {
        if (unk2 == 0) {
            if (data_020d43f0[0].mUnk_5d2 != 0) {
                result = TRUE;
            }
        } else {
            result = TRUE;
        }
    } else if ((unk1 != 0x20) & (unk1 != 0x21)) {
        if ((unk1 == 0x24) | (unk1 == 0x25)) {
            if (unk2 == data_020d43f0[0].mUnk_1a_8) {
                result = TRUE;
            }
        } else {
            result = TRUE;
        }
    }
    return result;
}

void func_02081a4c(void *arg) {
    UnkStruct_02078e14 *self = (UnkStruct_02078e14 *) arg;
    reg_G2S_DB_BG2OFS = (u32) ((-0x20 - self->mUnk_f8) << 16) & G2_BGOFS_V_MASK;
}

void UnkStruct_02078e14::func_02081994(u32 unk1, u32 unk2, u32 unk3) {
    WindowItem *items;
    switch (unk1) {
        case 0:
            items = mUnk_4c->mCursor->mItems;
            mUnk_4c->mButtons[unk2].mEnabled = unk3 != 0x8000 ? 1 : 0;
            break;
        case 1:
            items = mUnk_48->mCursor->mItems;
            mUnk_48->mButtons[unk2].mEnabled = unk3 != 0x8000 ? 1 : 0;
            break;
        case 2:
            items = mUnk_54->mItems;
            break;
        case 3:
            items = mUnk_50->mCursor->mItems;
            mUnk_50->mButtons[unk2].mEnabled = unk3 != 0x8000 ? 1 : 0;
            break;
        default:
            return;
    }
    items[unk2].mFlags = unk3;
}

void UnkStruct_02078e14::func_02082660() {
    for (u8 i = 0; i < 6; i++) {
        delete mUnk_5c[i];
        mUnk_5c[i] = NULL;
    }
    if (mUnk_28[1] != NULL) {
        OS_Free(mUnk_28[1]);
        mUnk_28[1] = NULL;
    }
    if (mUnk_30[1] != NULL) {
        OS_Free(mUnk_30[1]);
        mUnk_30[1] = NULL;
    }
}

void UnkStruct_02078e14::func_02081594() {
    if (mUnk_c4 == NULL) {
        return;
    }
    if (mUnk_401 != 0) {
        return;
    }
    switch (mUnk_e0) {
        case 2:
            if (mUnk_f0 != 0) {
                func_02055668(mUnk_c4, (u16) (mUnk_e6 + 0xd7), -1);
                return;
            }
            if (mUnk_fc == 0) {
                if (mUnk_e6 < 3) {
                    func_02055668(mUnk_c4, 0, -1);
                } else {
                    func_02055668(mUnk_c4, 0xd6, -1);
                }
            } else {
                func_02055668(mUnk_c4, mUnk_106[mUnk_fa], -1);
            }
            break;
        case 3:
        case 5:
            if (mUnk_e6 < 4) {
                return;
            }
            {
                u16 v;
                if (mUnk_f0 != 0) {
                    v = mUnk_c8[mUnk_e6 - 1];
                } else {
                    v = mUnk_106[mUnk_fa];
                }
                func_02055668(mUnk_c4, (u8) v, -1);
            }
            break;
        case 4:
            if (mUnk_e6 < 7) {
                return;
            }
            if (mUnk_f0 != 0) {
                func_02055668(mUnk_c4, (u16) (mUnk_e6 + 0xd0), -1);
            } else {
                func_02055668(mUnk_c4, mUnk_106[mUnk_fa], -1);
            }
            break;
    }
}

void UnkStruct_02078e14::func_02080898() {
    SpriteManager::Get();
    u8 i = 0;
    switch (mUnk_e0) {
        case 0:
        case 1:
            for (; i < 3; i++) {
                if (mUnk_fd + i >= mUnk_fc) {
                    return;
                }
                func_02081c2c((u8) mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
            }
            break;
        case 2:
            for (; i < 3; i++) {
                if (mUnk_fd + i >= mUnk_fc) {
                    return;
                }
                func_020823d8(mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
            }
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                return;
            }
            if (mUnk_e6 < 4) {
                for (; i < 3; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        return;
                    }
                    func_02081c2c((u8) mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
                }
            } else {
                for (; i < 3; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        return;
                    }
                    func_020823d8(mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
                }
            }
            break;
        case 4:
            if (mUnk_e6 == 0) {
                return;
            }
            if (mUnk_e6 < 7) {
                for (; i < 3; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        return;
                    }
                    func_02081c2c((u8) mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
                }
            } else {
                for (; i < 3; i++) {
                    if (mUnk_fd + i >= mUnk_fc) {
                        return;
                    }
                    func_020823d8(mUnk_106[mUnk_fd + i], (u8) ((mUnk_fd + i) % 4), 2);
                }
            }
            break;
    }
}

extern u8 data_020a3194[];

// non-matching: 2 words, movne/moveq order when computing `second = unk3 != 0` (original emits moveq r4,#0 first)
void UnkStruct_02078e14::func_02081a74(u8 unk1, u32 unk2, u32 unk3, s16 unk4, s16 unk5) {
    BOOL second = unk3 == 0 ? 0 : 1;
    u16 pos     = 0;
    u32 priority = 0;
    switch (unk3) {
        case 0:
            pos = unk2 * 8 + 0xa0;
            break;
        case 1:
            pos      = unk2 * 8 + 0x280;
            priority = 1;
            break;
        case 2:
            pos      = unk2 * 8 + 0x2a0;
            priority = 2;
            break;
        case 3:
            pos = unk2 * 8 + 0x320;
            break;
        case 4:
            pos = unk2 * 8 + 0x2a0;
            break;
    }
    SpriteManager::Get().Screen(second).func_02027778(data_020a3194, 1, (s16) (unk4 + 3), (s16) (unk5 + 3), priority, pos, 1, 0);
    if (unk3 == 0) {
        if (mUnk_5c[4] == NULL) {
            return;
        }
        mUnk_5c[4]->func_0202a3ec(second, unk1, unk4, unk5, priority);
    } else {
        mUnk_74[14]->func_0202a3ec(second, unk1, unk4, unk5, priority);
    }
}

extern u8 data_020a3188[];
extern u8 data_020a1074[];

// non-matching: 2 words, movne/moveq order when computing `second = unk3 != 0` (original emits moveq r4,#0 first)
void UnkStruct_02078e14::func_02082264(u8 unk1, u8 unk2, u32 unk3, s16 unk4, s16 unk5) {
    BOOL second = unk3 == 0 ? 0 : 1;
    u16 pos      = 0;
    u32 priority = 0;
    switch (unk3) {
        case 0:
            pos = unk2 * 4 + 0xc0;
            break;
        case 1:
            pos      = unk2 * 4 + 0x2c0;
            priority = 1;
            break;
        case 2:
            pos      = unk2 * 4 + 0x300;
            priority = 2;
            break;
        case 3:
            pos = unk2 * 4 + 0x340;
            break;
        case 4:
            pos = unk2 * 4 + 0x300;
            break;
    }
    SpriteManager::Get().Screen(second).func_02027778(data_020a3188, 1, unk4, unk5, priority, pos, 2, 0);
    if (unk3 == 0) {
        return;
    }
    mUnk_74[6]->func_0202a3ec(second, unk1, unk4, unk5, priority);
}

// non-matching: 2 words, movne/moveq order when computing `second = unk3 != 0` (original emits moveq r4,#0 first)
void UnkStruct_02078e14::func_02081c2c(u32 unk1, u32 unk2, u32 unk3) {
    switch (mUnk_e0) {
        case 2:
            return;
        case 3:
        case 5:
            if (mUnk_e6 >= 4) {
                return;
            }
            break;
        case 4:
            if (mUnk_e6 == 0) {
                return;
            }
            if (mUnk_e6 >= 7) {
                return;
            }
            break;
    }
    BOOL second = unk3 == 0 ? 0 : 1;
    ChangeDir("/Game/Bakugan/mark");
    char path[0x20];
    sprintf(path, "bakugan_s_%03d_ncg.bin", data_020a1074[unk1]);
    u32 offset = 0;
    switch (unk3) {
        case 0:
            offset = unk2 * 0x400 + 0x5000;
            break;
        case 1:
            offset = unk2 * 0x400 + 0x14000;
            break;
        case 2:
            offset = unk2 * 0x400 + 0x15000;
            break;
        case 3:
            offset = unk2 * 0x400 + 0x19000;
            break;
    }
    func_02025b64(second, 4, offset, path, 0, 0, 0);
}

// non-matching: 2 words, movne/moveq order when computing `second = unk3 != 0` (original emits moveq r4,#0 first)
void UnkStruct_02078e14::func_020823d8(u32 unk1, u32 unk2, u32 unk3) {
    switch (mUnk_e0) {
        case 0:
        case 1:
            return;
        case 3:
        case 5:
            if (mUnk_e6 != 0 && mUnk_e6 < 4 && unk3 != 1) {
                return;
            }
            break;
        case 4:
            if (mUnk_e6 < 7) {
                return;
            }
            break;
    }
    BOOL second = unk3 == 0 ? 0 : 1;
    ChangeDir("/Game/Card/Graphic/Small");
    char path[0x20];
    sprintf(path, "card_s_%03d_ncg.bin", func_02065c20(unk1));
    u32 offset = 0;
    switch (unk3) {
        case 0:
            offset = unk2 * 0x200 + 0x6000;
            break;
        case 1:
            offset = unk2 * 0x200 + 0x16000;
            break;
        case 2:
            offset = unk2 * 0x200 + 0x18000;
            break;
        case 3:
            offset = unk2 * 0x200 + 0x1a000;
            break;
    }
    func_02025b64(second, 4, offset, path, 0, 0, 0);
}

static inline void PrintMessage(TextManager &tm, const u8 *data, u32 offset, u32 length, void *buffer, s16 x, s16 y) {
    tm.Print(data + offset, length, buffer, 0x18, 0x10, &y, &x, NULL, 0, 2, 0x44, 0x10, 0, 0, 0, 1, 1);
}

// non-matching: guard load of TextManager::Get() is ahead of the preceding mUnk_5b store in the original (known open problem), plus 4 bytes of schedule
void UnkStruct_02078e14::func_0208250c(u32 unk1, u32 unk2) {
    TextManager::Get().mUnk_5b = 1;
    u16 *entry = (u16 *) ((u8 *) mUnk_18 + unk1 * 4);
    PrintMessage(TextManager::Get(), (u8 *) mUnk_18, entry[1], entry[2], mUnk_28[0], (s16) (unk2 << 5), 0);
    TextManager::Get().mUnk_5b = 0;
}

BOOL UnkStruct_02078e14::func_02082d00(u32 unk1, u32 unk2, u32 unk3) {
    if (unk1 != 0) {
        for (u8 i = 0; i <= 2; i++) {
            if (unk2 == mUnk_c8[i] && unk3 == mUnk_c8[9 + i]) {
                return TRUE;
            }
        }
    } else {
        for (u8 i = 3; i <= 8; i++) {
            if (unk2 == mUnk_c8[i]) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void UnkStruct_02078e14::func_0208329c() {
    if (mUnk_28[1] != NULL) {
        MI_CpuClear16(mUnk_28[1], mUnk_38[1]);
    }
}

u8 UnkStruct_02078e14::func_0208349c(u32 unk1) {
    s32 a = mUnk_1a2[unk1].mUnk_a;
    if (a == 0) {
        return 0;
    }
    s32 b = mUnk_1a2[unk1].mUnk_b;
    if (b == 0) {
        return 0;
    }
    u8 v = a * 0x28 / b;
    if (v == 0) {
        return 1;
    }
    if (v >= 0x28) {
        v = 0x28;
    }
    return v;
}

void UnkStruct_02078e14::func_020831f4() {
    SpriteManager::Get().Screen(0).func_02027778(mUnk_30[1], (u8) mUnk_40[1], 0x40, 0x22, 0, 0x200, 0xf, 0);
}

BOOL UnkStruct_02078e14::func_020842a0() {
    if (mUnk_c0 != NULL && mUnk_c0->func_020583f0()) {
        return TRUE;
    }
    if (mUnk_c4 != NULL && mUnk_c4->func_02055724()) {
        return TRUE;
    }
    return FALSE;
}

void UnkStruct_02078e14::func_02082d70() {
    ChangeDir("/font/");
    mUnk_38[1] = TextManager::Get().BufferSize(0x18, 0x14);
    mUnk_28[1] = OS_Alloc(mUnk_38[1]);
    MI_CpuClear16(mUnk_28[1], mUnk_38[1]);
    mUnk_40[1] = TextManager::Get().GlyphCount(0xf);
    mUnk_30[1] = OS_Alloc((s16) mUnk_40[1] * (s16) 0xc);
    TextManager::Get().Init(mUnk_30[1], 0, 0, 0x18, 0x14, 0x42);
    TextManager::Get().SetColor(0, 0, 0xf);
}

extern u8 data_020a3768[0x29][6];
extern u8 data_020d4946[];

// non-matching: 8 words, register assignment of the loop variables (original: i in r7, j in r8, row pointer in r9)
void UnkStruct_02078e14::func_020832bc() {
    MI_CpuClear16(&mUnk_1a2[0], 0x38);
    mUnk_1a2[0].mUnk_b = 0x11;
    mUnk_1a2[1].mUnk_b = 0x11;
    mUnk_1a2[2].mUnk_b = 0x11;
    mUnk_1a2[3].mUnk_b = 0x11;
    mUnk_1a2[4].mUnk_b = 0x12;
    mUnk_1a2[5].mUnk_b = 0x11;
    mUnk_1a2[6].mUnk_b = 0x67;
    mUnk_1a2[data_020d43f0[0].mUnk_1a_8].mUnk_b += 2;
    mUnk_1a2[6].mUnk_b += 2;
    if (data_020d43f0[0].mUnk_5d2 != 0) {
        mUnk_1a2[0].mUnk_b += 1;
        mUnk_1a2[6].mUnk_b += 1;
    }
    u8 i;
    const u8 *row;
    u8 j;
    for (i = 0; i < 0x29; i++) {
        row = data_020a3768[i];
        for (j = 0; j < 6; j++) {
            if (row[j] != 0) {
                if (func_020231f4(data_020d43f0, 0, i, j) != 0) {
                    mUnk_1a2[j].mUnk_a += 1;
                    mUnk_1a2[6].mUnk_a += 1;
                }
            }
        }
    }
    for (u16 k = 1; k < 0xd5; k++) {
        if (func_02065c20(k) < 0x2bc) {
            u8 sel = func_02065c0c(k, 1) + 7;
            mUnk_1a2[sel].mUnk_b += 1;
            mUnk_1a2[13].mUnk_b += 1;
            if (TestFlagBit(data_020d4946, k)) {
                mUnk_1a2[sel].mUnk_a += 1;
                mUnk_1a2[13].mUnk_a += 1;
            }
        }
    }
}

void UnkStruct_02078e14::func_02085460(s32 unk1) {
    if (unk1 >= 0) {
        mUnk_1ec->func_02065890(unk1, 0, 1);
        return;
    }
    switch (mUnk_e0) {
        case 0:
        case 1:
        case 2:
        case 4:
            break;
        case 3:
        case 5:
            if (mUnk_e6 == 0) {
                if (mUnk_f0 != 0) {
                    mUnk_1ec->func_02065890(6, 0, 1);
                } else {
                    mUnk_1ec->func_02065890((s16) (mUnk_fa + 7), 0, 1);
                }
            } else {
                mUnk_1ec->func_02065890((s16) (mUnk_e6 + 8), 0, 1);
            }
            break;
    }
}

void UnkStruct_02078e14::func_020851d0() {
    func_0207dea4();
    func_0207eec4();
    func_0208403c();
    if (mUnk_1ec != NULL && mUnk_1a1 == 0) {
        mUnk_1ec->func_020658d0();
    }
    if (mUnk_e1 < 0x17) {
        func_0207f2f0();
    }
}

void func_02023134(UnkStruct_020d43f0 *d, u32 a, u32 b, u32 c, u32 e, u32 f);

void UnkStruct_02078e14::func_0208403c() {
    switch (mUnk_1a1) {
        case 0:
            return;
        case 4:
        case 5:
            func_02081a74(mUnk_104, 0, 3, 0x23, 0x5a);
            break;
        case 6:
            func_02082264(mUnk_104, 0, 3, 0x23, 0x5a);
            break;
    }
    SpriteManager::Get().Screen(1).func_02027778(mUnk_30[0], (u8) mUnk_40[0], 0x20, 0x28, 0, 0, 0xf, 0);
}

void UnkStruct_02078e14::func_020842e8() {
    if (mUnk_1f1 != 0x15) {
        return;
    }
    for (u8 i = 0; i < 3; i++) {
        func_02023134(data_020d43f0, 0, i, 0, (u8) mUnk_c8[i], (u8) mUnk_c8[9 + i]);
    }
    for (u8 i = 0; i < 3; i++) {
        data_020d43f0[0].mUnk_06[i] = mUnk_c8[3 + i];
    }
    for (u8 i = 0; i < 3; i++) {
        data_020d43f0[0].mUnk_0c[i] = mUnk_c8[6 + i];
    }
}
