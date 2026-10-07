#include "Unknown/UnkStruct_02071868.hpp"
#include "Unknown/UnkStruct_02071d24.hpp"

#include "Battle/BattleCommandList.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/FrameClock.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

extern const u8 data_020a2e3c[];
extern const WindowItemRecord data_020a2e48[];
extern const WindowCellRecord data_020a2f0c[];
extern const u32 data_020a2dac[];
void func_02005e9c(void *buffer, u32 size);
void func_02072614();
extern u16 data_020a596c;

UnkStruct_02071868::UnkStruct_02071868(u32 unk1) {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(1);
    func_02001eec((vu16 *) 0x400006c, -16);
    func_02001eec((vu16 *) 0x400106c, 0);
    G2_BlendNone();
    G2S_BlendNone();
    SpriteManager::Get().SetFade(1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 1);
    MessageFile *file;
    if (unk1 != 2) {
        file = (MessageFile *) func_02023504("/font/mes_system.mes", 0, 0, 0, 0, 0);
    } else {
        file = (MessageFile *) func_02023504("/font/mes_title.mes", 0, 0, 0, 0, 0);
    }
    TextManager::Get();
    void *glyphs = OS_Alloc(0xc000);
    MIi_CpuClear16(0, glyphs, 0xc000);
    TextManager::Get();
    void *map = OS_Alloc(0x600);
    MIi_CpuClear16(0, map, 0x600);
    u32 index;
    switch (unk1) {
        case 0:
            index = 5;
            break;
        case 1:
            index = 4;
            break;
        case 2:
            index = 8;
            break;
    }
    TextManager &text = TextManager::Get();
    s16 width;
    s16 height;
    height = 0x40;
    width  = 0x20;
    text.Print(file->GetText(index), file->GetLength(index), glyphs, 0x20, 0x18, &width, &height, NULL, 1, 2, 0, 0x10, 0,
               0, 0, 1, 1);
    OS_Free(file);
    TextManager::Get().func_0202cd48(1, (u16 *) map, 0, 0, 0x20, 0x18, 0x20);
    func_02003ea4(glyphs, 0, 0xc000);
    OS_Free(glyphs);
    func_02003ba4(map, 0, 0x600);
    OS_Free(map);
    func_02004394();
    func_020043ac(&data_020a596c, 0x4000, 0x20);
    func_02004410();
    G2S_SetBG0Offset(0x10, 0);
    if (unk1 != 2) {
        UnkStruct_020c1884::Get().mUnk_14 = 1;
    }
}

UnkStruct_02071868::~UnkStruct_02071868() {
}

UnkStruct_02071d24::UnkStruct_02071d24(u32 unk1) :
    SceneBase(2, 0) {
    mUnk_0c = NULL;
    mUnk_10 = NULL;
    mUnk_14 = NULL;
    mUnk_18 = NULL;
    mUnk_20 = NULL;
    mUnk_4c = unk1;
    mUnk_4d = 0;
    mUnk_4e = 0;
    mUnk_4f = 0;
    mUnk_50 = 0;
    mUnk_51 = 0;
    mUnk_52 = 0;
    mUnk_53 = 0;
    mUnk_54 = -1;
    mUnk_1c = NULL;
    mUnk_24 = NULL;
    mUnk_28 = NULL;
    mUnk_2c = 0;
    mUnk_30 = 0;
    MI_CpuClear8(mUnk_58, 6);
    MI_CpuClear8(mUnk_5e, 6);
    MI_CpuClear8(mUnk_64, 6);
    for (u8 i = 0; i < 6; i++) {
        mUnk_34[i] = NULL;
    }
}

UnkStruct_02071d24::~UnkStruct_02071d24() {
    delete mUnk_1c;
    delete mUnk_20;
    if (mUnk_0c != NULL) {
        OS_Free(mUnk_0c);
        mUnk_0c = NULL;
    }
    if (mUnk_10 != NULL) {
        OS_Free(mUnk_10);
        mUnk_10 = NULL;
    }
    delete mUnk_14;
    if (mUnk_24 != NULL) {
        func_02074128();
        OS_Free(mUnk_24);
    }
    if (mUnk_28 != NULL) {
        OS_Free(mUnk_28);
        mUnk_28 = NULL;
    }
}

void UnkStruct_02071d24::Update() {
    switch (mUnk_4d) {
        case 0:
            func_02072614();
            func_02072734();
            func_02073750();
            func_02073040();
            func_02072c40();
            mUnk_4d++;
            break;
        case 1:
            if (mUnk_18->func_020583f0() != 0) {
                break;
            }
            SpriteManager::Get();
            SpriteManager::sFade.mSub = 2;
            mUnk_4d++;
            break;
        case 2:
            if (!SpriteManager::Get().IsFading()) {
                mUnk_4d++;
            }
            func_02072e7c();
            break;
        case 3:
            if (func_02072d20()) {
                SoundManager::Get().func_020246f8(0x34);
                SetFlagBit(data_020d49b0, mUnk_5e[mUnk_4e]);
            }
            mUnk_4d++;
            if (mUnk_4c != 2) {
                func_02072b60();
            }
            func_02072e7c();
            break;
        case 4: {
            WindowItem *items;
            if (mUnk_18->func_0205b0c4() != 0) {
                mUnk_52 = 1;
            } else {
                items = mUnk_20->mCursor->mItems;
                mUnk_52 = 0;
                if (mUnk_50 == 3) {
                    if ((u8) mUnk_20->GetSelectedId() == 6) {
                        if (mUnk_53 == 1) {
                            mUnk_50 = 0;
                            mUnk_53 = 2;
                            func_020739e4();
                        } else {
                            mUnk_54 = 0x11;
                            mUnk_20->mButtons[0].mEnabled = 1;
                            mUnk_20->mButtons[1].mEnabled = 1;
                            mUnk_20->mButtons[2].mEnabled = 1;
                            mUnk_20->mButtons[3].mEnabled = 1;
                            mUnk_20->mButtons[4].mEnabled = 1;
                            items[0].mFlags = 8;
                            items[1].mFlags = 8;
                            items[2].mFlags = 8;
                            items[3].mFlags = 8;
                            items[4].mFlags = 8;
                            mUnk_20->mButtons[5].mEnabled = 0;
                            mUnk_20->mButtons[6].mEnabled = 0;
                            items[6].mFlags = 0x8000;
                            items[6].mY     = 0xc0;
                            items[4].mY     = 0x90;
                            mUnk_20->Update(1);
                            mUnk_20->func_0205c7c8(-1);
                            mUnk_50 = 0;
                            func_02073040();
                            mUnk_53 = 0;
                            func_020739e4();
                            mUnk_4d++;
                            mUnk_52 = 0x1e;
                        }
                    }
                }
            }
            func_02072e7c();
            break;
        }
        case 5: {
            s32 plane = mUnk_54;
            if (plane != -1) {
                GXS_SetVisiblePlane(plane);
                mUnk_54 = -1;
            }
            mUnk_52--;
            if (mUnk_52 == 0) {
                mUnk_4d++;
            }
            func_02072e7c();
            break;
        }
        case 6:
            if (mUnk_18->func_0205b0c4() != 0) {
                mUnk_52 = 1;
            } else {
                mUnk_52 = 0;
                if (mUnk_51 != 0) {
                    if (InputManager::Get().IsHeld(1) | TouchManager::Get().mUnk_57) {
                        mUnk_51 = 0;
                    } else {
                        mUnk_51--;
                    }
                } else {
                    func_020733d4();
                }
            }
            func_02072e7c();
            break;
        case 7:
            func_02072dfc();
            if (mUnk_18 != NULL) {
                func_0205821c(mUnk_18);
                mContext->mUnk_12 = 0;
            }
            mUnk_4d++;
            SpriteManager::Get().StartFade(3);
            break;
        case 8:
            func_02072dfc();
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            if (mUnk_18 != NULL) {
                if (mContext->mUnk_12 != 0) {
                    mUnk_18 = NULL;
                } else {
                    return;
                }
            }
            mUnk_4d++;
            break;
        case 9:
            RequestScene(2);
            mFinished = 1;
            break;
    }
}

void func_02072614() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(0, 1);
    func_02001e68(1, 0, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    GXS_SetVisiblePlane(0x11);
    G2S_SetBlendBrightness(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_BlendNone();
}

// non-matching: 6 words. For the last func_02025b64 call (kind 1, offset 0x2000) the original keeps the zero stack arguments in
// r5 and sets r0 after the stores; the compiler uses r0 for them. Everything else is identical.
void UnkStruct_02071d24::func_02072734() {
    ChangeDir("/Game/Battle/Bg/");
    func_02025b64(1, 7, 0, "result_b_bg_01_nsc.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "result_b_bg_01_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "result_b_bg_01_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/2d/union_b/");
    func_02025b64(1, 12, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0xf000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0x160, 0);
    screen.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x11000, 0);
    screen.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x1a0, 0);
    ChangeDir("/Game/Battle/Obj/");
    SpriteScreen &screen2 = SpriteManager::Get().Screen(1);
    screen2.func_02027624(3);
    screen2.LoadCharacters("bakugan_sub.nce_ncg1D.bin", 0x9000, 0);
    screen2.func_02027fec("bakugan_sub_ncl.bin", 0, 0);
    screen2.LoadCharacters("result_b_obj_01.nce_ncg1D.bin", 0x4000, 0);
    screen2.LoadPalette("result_b_obj_01_ncl.bin", 0, 0);
    mUnk_1c           = new CellAnimation("bakugan_sub_nce.bin", 0, 1, 1);
    mUnk_1c->mCharBase = screen2.func_020276f4(0x9000);
    mUnk_1c->mUnk_11   = 0;
    mUnk_1c->func_02029fb8(5);
    mUnk_1c->mUnk_0f = mUnk_5e[mUnk_4e];
    mUnk_20          = new SelectWindow(7, data_020a2e48, 0);
    mUnk_20->SetCells(7, data_020a2f0c);
    mUnk_20->Update(1);
    mUnk_18 = new BattleCommandList(mUnk_58[mUnk_4e], mUnk_5e[mUnk_4e], -1, 0);
    if (mUnk_34[mUnk_4e] != NULL) {
        func_02072b14();
    } else {
        func_02058228(mUnk_18, 1);
    }
}

void UnkStruct_02071d24::func_02072b14() {
    int value = func_020231f4(data_020d43f0, 0, mUnk_58[mUnk_4e], mUnk_5e[mUnk_4e]);
    func_0205afb4(mUnk_18, mUnk_5e[mUnk_4e], value, mUnk_34[mUnk_4e]);
}

// non-matching: 1 word. The original computes the u16 end value before the func_0205b048 call and keeps it in r6 (extra mov
// r2, r6 before func_020231c0); the compiler sinks the computation behind the call. Also the table pointer is loaded after the
// shift in the original.
void UnkStruct_02071d24::func_02072b60() {
    u16 total = 0;
    BattleCellPair *pair = mUnk_34[mUnk_4e];
    u8 count = mUnk_64[mUnk_4e];
    u8 i = 0;
    do {
        total += data_020a2e3c[pair->mUnk_00_0 - i];
        i++;
    } while (i < count);
    int current = func_0202317c(data_020d43f0, 0, mUnk_58[mUnk_4e], mUnk_5e[mUnk_4e]);
    int base    = func_020231f4(data_020d43f0, 0, mUnk_58[mUnk_4e], mUnk_5e[mUnk_4e]);
    if (base + total > 0x3de) {
        total = 0x3de - base;
    }
    u16 end = base + total;
    mUnk_18->func_0205b048(total);
    func_020231c0(data_020d43f0, current, end);
}

// non-matching: 2 words. The original reloads mUnk_20 into lr (not r0) for the buttons[3] store.
void UnkStruct_02071d24::func_02072c40() {
    WindowItem *items = mUnk_20->mCursor->mItems;
    mUnk_53 = func_02072d20() ? 1 : 2;
    GXS_SetVisiblePlane(0x13);
    mUnk_20->mButtons[0].mEnabled = 0;
    mUnk_20->mButtons[1].mEnabled = 0;
    mUnk_20->mButtons[2].mEnabled = 0;
    mUnk_20->mButtons[3].mEnabled = 0;
    mUnk_20->mButtons[4].mEnabled = 0;
    items[0].mFlags = 0x8000;
    items[1].mFlags = 0x8000;
    items[2].mFlags = 0x8000;
    items[3].mFlags = 0x8000;
    items[4].mFlags = 0x8000;
    mUnk_20->mButtons[5].mEnabled = 1;
    mUnk_20->mButtons[6].mEnabled = 1;
    items[6].mFlags = 8;
    items[4].mY     = 0xc0;
    items[6].mY     = 0x98;
    mUnk_20->Update(1);
    mUnk_20->func_0205c7c8(-1);
    mUnk_20->mCursor->mSelection = 6;
    func_020739e4();
}

BOOL UnkStruct_02071d24::func_02072d20() {
    u8 index = mUnk_4e;
    BattleCellPair *pair = mUnk_34[index];
    if (pair == NULL) {
        return FALSE;
    }
    s32 count = pair->mUnk_00_0;
    if (count >= 2) {
        if (count - mUnk_64[index] < 2) {
            return TRUE;
        }
    }
    if (count >= 2) {
        if (mUnk_4c == 2) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL UnkStruct_02071d24::func_02072d84() {
    BattleCellPair *pair = mUnk_34[mUnk_4e];
    if ((s32) pair->mUnk_02_0 < 4) {
        return FALSE;
    }
    if ((s32) pair->mUnk_02_12 < 4) {
        return FALSE;
    }
    if ((s32) pair->mUnk_02_3 < 4) {
        return FALSE;
    }
    if ((s32) pair->mUnk_02_6 < 4) {
        return FALSE;
    }
    return (s32) pair->mUnk_02_9 >= 4;
}

void UnkStruct_02071d24::func_02072dfc() {
    if (mUnk_34[mUnk_4e] == NULL) {
        return;
    }
    if (mUnk_1c != NULL) {
        mUnk_1c->Update();
        mUnk_1c->func_0202a494(1, 0x20, 0x3c, 1, 0x800, 0x800, 0);
    }
    if (mUnk_20 != NULL) {
        mUnk_20->Draw();
    }
    func_02073fb0();
}

void UnkStruct_02071d24::func_02072e7c() {
    if (mUnk_34[mUnk_4e] == NULL) {
        return;
    }
    if (mUnk_1c != NULL) {
        mUnk_1c->Update();
        mUnk_1c->func_0202a494(1, 0x20, 0x3c, 1, 0x800, 0x800, 0);
    }
    if (mUnk_20 != NULL) {
        BOOL enabled = TRUE;
        if ((func_02072d84() && (InputManager::Get().IsDown(0x40) | InputManager::Get().IsDown(0x80))) || mUnk_51 != 0) {
            enabled = FALSE;
        }
        if (mUnk_52 != 0) {
            enabled = FALSE;
        }
        if (mUnk_4d != 6 && mUnk_4d != 4) {
            enabled = FALSE;
        }
        if (enabled) {
            s8 previous = mUnk_20->mCursor->mSelection;
            u8 result   = mUnk_20->Update(1);
            s8 current  = mUnk_20->mCursor->mSelection;
            if (current < 5 && previous != current) {
                mUnk_14->func_02065890((s16) (current + 0x10), 0, 1);
            }
            if (mUnk_50 != 3) {
                mUnk_50 = result;
            }
        }
        mUnk_20->Draw();
    }
    func_02073fb0();
}

// non-matching: 15 words, register allocation only (the original keeps cells in r5, the constant 8 in r6 and r11, items in r8,
// the loop counter in r7; in the second loop the index temporaries are r10/r5).
void UnkStruct_02071d24::func_02073040() {
    if (mUnk_34[mUnk_4e] == NULL) {
        return;
    }
    const WindowCellRecord *cells = data_020a2f0c;
    u8 i;
    WindowItem *items = mUnk_20->mCursor->mItems;
    for (i = 0; i < 5; i++) {
        switch (i) {
            case 0:
                if ((s32) mUnk_34[mUnk_4e]->mUnk_02_0 < 4) {
                    items[i].mFlags = 8;
                    mUnk_20->mCells[i]->SetAnimation(cells[i].mUnk_18[0]);
                } else {
                    items[i].mFlags = 0x8000;
                    mUnk_20->mCells[i]->SetAnimation(0xf);
                }
                break;
            case 1:
                if ((s32) mUnk_34[mUnk_4e]->mUnk_02_12 < 4) {
                    items[i].mFlags = 8;
                    mUnk_20->mCells[i]->SetAnimation(cells[i].mUnk_18[0]);
                } else {
                    items[i].mFlags = 0x8000;
                    mUnk_20->mCells[i]->SetAnimation(0x10);
                }
                break;
            case 2:
                if ((s32) mUnk_34[mUnk_4e]->mUnk_02_3 < 4) {
                    items[i].mFlags = 8;
                    mUnk_20->mCells[i]->SetAnimation(cells[i].mUnk_18[0]);
                } else {
                    items[i].mFlags = 0x8000;
                    mUnk_20->mCells[i]->SetAnimation(0x11);
                }
                break;
            case 3:
                if ((s32) mUnk_34[mUnk_4e]->mUnk_02_6 < 4) {
                    items[i].mFlags = 8;
                    mUnk_20->mCells[i]->SetAnimation(cells[i].mUnk_18[0]);
                } else {
                    items[i].mFlags = 0x8000;
                    mUnk_20->mCells[i]->SetAnimation(0x12);
                }
                break;
            case 4:
                if ((s32) mUnk_34[mUnk_4e]->mUnk_02_9 < 4) {
                    items[i].mFlags = 8;
                    mUnk_20->mCells[i]->SetAnimation(cells[i].mUnk_18[0]);
                } else {
                    items[i].mFlags = 0x8000;
                    mUnk_20->mCells[i]->SetAnimation(0x13);
                }
                break;
        }
    }
    u8 j;
    WindowItem *selectItems = mUnk_20->mCursor->mItems;
    for (j = 0; j < 5; j++) {
        u8 index = (u8) (j + mUnk_4f) % 5;
        if (selectItems[index].mFlags == 8) {
            mUnk_14->func_02065890((s16) (index + 0x10), 0, 1);
            mUnk_20->mCursor->mSelection = index;
            mUnk_20->mUnk_063            = index;
            mUnk_20->mCells[index]->SetAnimation(data_020a2f0c[index].mUnk_18[2]);
            return;
        }
    }
}

void UnkStruct_02071d24::func_020733d4() {
    u8 index = mUnk_4e;
    if (mUnk_64[index] == 0) {
        u8 next;
        next = mUnk_4e = index + 1;
        if (next >= 6) {
            mUnk_4d++;
            return;
        }
        if (mUnk_64[next] != 0) {
            if (mUnk_18 != NULL) {
                func_02058260(mUnk_18, mUnk_58[next], mUnk_5e[next], -1);
            }
            mUnk_4f = 0;
            mUnk_1c->mUnk_0f = mUnk_5e[mUnk_4e];
            func_02072b14();
            func_02073040();
            func_02072c40();
            mUnk_4d = 3;
        } else {
            mUnk_4d++;
            mUnk_4e--;
        }
        return;
    }
    if (mUnk_20 == NULL || mUnk_34[index] == NULL) {
        return;
    }
    if (func_02072d84()) {
        if (InputManager::Get().IsHeld(1) | TouchManager::Get().mUnk_57) {
            mUnk_50 = 3;
        }
    }
    mUnk_4f = mUnk_20->GetSelectedId();
    if (mUnk_50 != 3) {
        return;
    }
    mUnk_50 = 0;
    switch (mUnk_4f) {
        case 0:
            if ((s32) mUnk_34[mUnk_4e]->mUnk_02_0 < 4) {
                mUnk_34[mUnk_4e]->mUnk_02_0++;
            }
            break;
        case 1:
            if ((s32) mUnk_34[mUnk_4e]->mUnk_02_12 < 4) {
                mUnk_34[mUnk_4e]->mUnk_02_12++;
            }
            break;
        case 2:
            if ((s32) mUnk_34[mUnk_4e]->mUnk_02_3 < 4) {
                mUnk_34[mUnk_4e]->mUnk_02_3++;
            }
            break;
        case 3:
            if ((s32) mUnk_34[mUnk_4e]->mUnk_02_6 < 4) {
                mUnk_34[mUnk_4e]->mUnk_02_6++;
            }
            break;
        case 4:
            if ((s32) mUnk_34[mUnk_4e]->mUnk_02_9 < 4) {
                mUnk_34[mUnk_4e]->mUnk_02_9++;
            }
            break;
    }
    func_02072b14();
    if (mUnk_64[mUnk_4e] == 0) {
        return;
    }
    mUnk_64[mUnk_4e]--;
    mUnk_51 = 0x78;
    if (mUnk_64[mUnk_4e] == 0) {
        return;
    }
    func_02073040();
}

void UnkStruct_02071d24::func_02073750() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    ChangeDir("/font/");
    mUnk_0c = (MessageFile *) func_02023504("mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mUnk_10 = (MessageFile *) func_02023504("mes_Result.mes", 0, 0, 0, 0, 0);
    mUnk_2c = TextManager::Get().BufferSize(0x18, 0xc);
    mUnk_24 = OS_Alloc(mUnk_2c);
    MI_CpuClear16(mUnk_24, mUnk_2c);
    mUnk_14 = new MessageWindow("mes_Result", 1, 0x2800);
    mUnk_14->func_02065bd4(0x14, 4, 0);
    mUnk_14->func_02065be4(0xd8);
    mUnk_14->func_02065890(0x10, 0, 1);
    mUnk_30 = TextManager::Get().GlyphCount(0x24);
    mUnk_28 = OS_Alloc((s16) mUnk_30 * (s16) 0xc);
    TextManager::Get().Init(mUnk_28, 1, 0, 0x18, 0xc, 0x21);
    TextManager::Get().SetColor(1, 0, 0xe);
}

static inline void PrintResultText(TextManager &text, MessageFile *file, u32 index, void *buffer, s16 w, s16 h,
                                   u16 *pixelWidth) {
    text.Print(file->GetText(index), file->GetLength(index), buffer, 0x18, 0xc, &w, &h, pixelWidth, 0, 2, 0x21, 0, 0, 0, 0, 1,
               1);
}

// non-matching: 6 words. In the original the guard word of the second TextManager::Get() is loaded before the preceding
// `mUnk_5b = 1` store (the guard-load-before-store problem of docs/decompiling.md); everything else is identical.
void UnkStruct_02071d24::func_020739e4() {
    s16 curW;
    s16 curH;
    u16 pixelWidth;
    s16 width1;
    s16 height1;
    s16 width2;
    s16 height2;
    func_02074128();
    if (mUnk_34[mUnk_4e] == NULL) {
        return;
    }
    if (mUnk_53 == 1) {
        TextManager &text = TextManager::Get();
        height1 = 0;
        width1  = 0;
        MessageFile *file = mUnk_10;
        text.Print(file->GetText(1), file->GetLength(1), mUnk_24, 0x18, 0xc, &width1, &height1, NULL, 0, 2, 0x21, 0x10, 0,
                   0, 1, 1, 1);
    } else if (mUnk_53 == 2) {
        TextManager &text = TextManager::Get();
        height2 = 0;
        width2  = 0;
        MessageFile *file = mUnk_10;
        text.Print(file->GetText(0), file->GetLength(0), mUnk_24, 0x18, 0xc, &width2, &height2, NULL, 0, 2, 0x21, 0x10, 0,
                   0, 1, 1, 1);
    } else {
        TextManager::Get().mUnk_5b = 1;
        curW = 0;
        curH = 0;
        PrintResultText(TextManager::Get(), mUnk_0c, mUnk_58[mUnk_4e], mUnk_24, curW, curH, NULL);
        curH += 0x48;
        PrintResultText(TextManager::Get(), mUnk_10, 5, mUnk_24, curW, curH, &pixelWidth);
        curW += pixelWidth;
        u8 count = mUnk_34[mUnk_4e]->mUnk_00_0;
        if (count < 9) {
            curW += 7;
            TextManager::Get().PrintNumber(count + 1, 1, 0, mUnk_24, 0x18, 0xc, &curW, &curH, 0, 2, 0x21, 0, 0, 1, 1);
        } else {
            PrintResultText(TextManager::Get(), mUnk_10, 6, mUnk_24, curW, curH, NULL);
        }
        TextManager::Get().mUnk_5b = 0;
    }
    func_02005e9c(mUnk_24, mUnk_2c);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0, mUnk_24, mUnk_2c, 0);
}

void UnkStruct_02071d24::func_02073fb0() {
    BOOL show = FALSE;
    switch (mUnk_4d) {
        case 2:
        case 3:
            show = TRUE;
            break;
        case 4:
            show = TRUE;
            break;
    }
    if (show) {
        SpriteManager::Get().Screen(1).func_02027778(mUnk_28, (u8) mUnk_30, 0x20, 0x28, 0, 0, 0xe, 0);
    } else {
        SpriteManager::Get().Screen(1).func_02027778(mUnk_28, (u8) mUnk_30, 0x14, 0x1c, 0, 0, 0xe, 0);
    }
    if (mUnk_14 == NULL) {
        return;
    }
    if (show) {
        return;
    }
    mUnk_14->func_020658d0();
}

void UnkStruct_02071d24::func_02074128() {
    if (mUnk_24 != NULL) {
        MI_CpuClear16(mUnk_24, mUnk_2c);
    }
}

void UnkStruct_02071d24::func_02074148(u32 index, u32 unk2, u32 unk3, u8 unk4) {
    if (index >= 6 || unk2 >= 0x29 || unk3 >= 6) {
        return;
    }
    mUnk_4e = 0;
    mUnk_58[index] = unk2;
    mUnk_5e[index] = unk3;
    mUnk_34[index] = (BattleCellPair *) func_02023248(data_020d43f0, unk2, unk3);
    if (unk4 > 9) {
        unk4 = 9;
    }
    mUnk_64[index] = unk4;
}
