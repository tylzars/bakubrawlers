#include "Battle/BattleCommandPage.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/SelectWindow.hpp"
#include "Graphics/Display.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

void func_02023a54();
void func_02023708(const char *path, u32 a, u32 b, u32 c, u32 d, u32 e);
int func_02023874();
void *func_02023864();
u16 func_02065c20(u32 id);
int func_02065c0c(u32 id, u32 kind);
extern u16 data_020c1a4c[0x10]; // palette scratch (cleared before each text)
extern const char *data_020b3070[12]; // paths loaded by func_02055c28 and func_020561c4

extern WindowItemRecord data_020b30d4[4]; // item table of the page window (positions are patched by func_02055764)
extern WindowCellRecord data_020b3144[4];
extern u8 data_0209e558[]; // tables passed to SpriteScreen::func_02027778 (.rodata 0x0209e558-0x0209e57c)
extern u8 data_0209e564[];
extern u8 data_0209e570[];

BattleCommandPage::BattleCommandPage(u32 a, s32 b) :
    SceneBase(0x3e8, 0) {
    mUnk_50 = NULL;
    mUnk_54 = NULL;
    mUnk_58 = NULL;
    mUnk_5c = NULL;
    mUnk_f1 = 0;
    mUnk_f2 = 0;
    mUnk_fa = b;
    mUnk_fb = 0;
    mUnk_fc = 0;
    mUnk_fd = 0;
    mUnk_fe = 0;
    mUnk_100 = 0;
    mUnk_102 = 0;
    mUnk_104 = 0;
    mUnk_105 = 0;
    mUnk_106 = 0;
    mUnk_107 = 0;
    mUnk_108 = 0;
    MI_CpuClear32(mUnk_60, 0xc);
    MI_CpuClear32(mUnk_6c, 0xc);
    MI_CpuClear32(mUnk_78, 0xc);
    MI_CpuClear32(mUnk_84, 0xc);
    MI_CpuClear32(mUnk_90, 0x30);
    MI_CpuClear32(mUnk_c0, 0x30);
    MI_CpuClear32(mUnk_0c, 0xc);
    MI_CpuClear32(mUnk_18, 0xc);
    MI_CpuClear32(mCells, 0x2c);
    mUnk_f4 = func_02055644(a);
    mUnk_f6 = mUnk_f4;
    mUnk_f8 = mUnk_f4;
}

BattleCommandPage::~BattleCommandPage() {
    func_02023a54();
    if (mUnk_54 != NULL) {
        OS_Free(mUnk_54);
    }
    if (mUnk_58 != NULL) {
        OS_Free(mUnk_58);
    }
    if (mUnk_5c != NULL) {
        OS_Free(mUnk_5c);
    }
    for (u8 i = 0; i < 3; i++) {
        if (mUnk_60[i] != NULL) {
            OS_Free(mUnk_60[i]);
        }
        if (mUnk_6c[i] != NULL) {
            OS_Free(mUnk_6c[i]);
        }
    }
    for (u8 i = 0; i < 3; i++) {
        if (mUnk_0c[i] != NULL) {
            OS_Free(mUnk_0c[i]);
        }
        if (mUnk_18[i] != NULL) {
            OS_Free(mUnk_18[i]);
        }
    }
    for (u8 i = 0; i < 11; i++) {
        CellAnimation *cell = mCells[i];
        if (cell != NULL) {
            delete cell;
        }
    }
    if (mUnk_50 != NULL) {
        delete mUnk_50;
    }
}

void BattleCommandPage::Update() {
    switch (mUnk_f1) {
        case 0:
            func_02055af4();
            func_02056414();
            func_02055c1c();
            mUnk_f1++;
            break;
        case 1:
            if (func_02055c28()) {
                mUnk_104 = 1;
                mUnk_f1++;
            }
            break;
        case 2:
            func_02055c1c();
            mUnk_f1++;
            break;
        case 3:
            if (func_020561c4()) {
                func_02056814(0);
                if ((s32) mUnk_100 > 0xf0) {
                    mUnk_102 = 0x100;
                }
                func_02055940();
                if (mUnk_107 == 0) {
                    mUnk_106 = 0;
                }
                mUnk_f8  = mUnk_f4;
                mUnk_109 = mUnk_10a;
                if (mUnk_105 == 0) {
                    SpriteManager::Get().sFade.mMain = 2;
                }
                mUnk_f1++;
            }
            break;
        case 4:
            if (mUnk_107 != 0) {
                mUnk_106 = 1;
            }
            if (mUnk_f6 != mUnk_f4) {
                mUnk_f4  = mUnk_f6;
                mUnk_107 = 0;
                mUnk_fb  = 0;
                mUnk_f1  = 2;
            }
            if (mUnk_105 != 0) {
                mUnk_f1 = 5;
            }
            break;
        case 5:
            switch (mUnk_105) {
                case 1:
                    SpriteManager::Get().sFade.mMain = 3;
                    break;
                case 2:
                    SpriteManager::Get();
                    SpriteManager::sFade.mMain = 4;
                    SpriteManager::sFade.mSub  = 4;
                    break;
                case 3:
                    SpriteManager::Get();
                    SpriteManager::sFade.mMain = 3;
                    SpriteManager::sFade.mSub  = 3;
                    break;
            }
            mUnk_f1++;
            break;
        case 6:
            if (SpriteManager::Get().sFade.mMain <= 1) {
                mUnk_104 = 0;
                mUnk_f1++;
            }
            break;
        case 7:
            RequestScene(1);
            mFinished = 1;
            break;
    }
    if (mUnk_104 == 0) {
        return;
    }
    func_020571a8();
    func_02056d38();
    func_02056f70();
    if (mUnk_fd == 0) {
        return;
    }
    if (SpriteManager::Get().sBrightness.mSub != 0) {
        return;
    }
    if (mUnk_fc == 0) {
        return;
    }
    if (mUnk_f1 == 4) {
        func_020558b8();
    }
    mUnk_50->Draw();
}

void func_02055624(BattleCommandPage *page, u32 a) {
    page->mUnk_105 = a;
}

void func_0205562c(BattleCommandPage *page, u32 a) {
    u8 value         = a;
    page->mUnk_107 = value;
    if (page->mUnk_f1 < 2) {
        page->mUnk_106 = value;
    }
}

u32 BattleCommandPage::func_02055644(u32 id) {
    if (id == 0xd6) {
        mUnk_10a = 1;
        return 0;
    }
    mUnk_10a = 0;
    return id;
}

void func_02055668(BattleCommandPage *page, u32 a, s32 b) {
    page->mUnk_fa = b;
    if (page->mUnk_f1 == 0) {
        u32 value      = page->func_02055644(a);
        page->mUnk_f4  = value;
        page->mUnk_f6  = value;
        page->mUnk_106 = 0;
        page->mUnk_107 = 0;
        return;
    }
    page->mUnk_f6 = page->func_02055644(a);
    if (page->mUnk_f6 == page->mUnk_f4) {
        if (page->mUnk_106 != 0) {
            page->mUnk_106 = 0;
        }
    }
    page->mUnk_107 = 0;
}

void BattleCommandPage::func_020556d0(u32 flag) {
    if (mUnk_fc == 0) {
        return;
    }
    u8 count = mUnk_fc + 1;
    if (flag == 0) {
        mUnk_fb = (mUnk_fb + count - 1) % count;
    } else {
        mUnk_fb = (mUnk_fb + 1) % count;
    }
    func_02056814(mUnk_fb);
}

BOOL BattleCommandPage::func_02055724() {
    return mUnk_f1 < 4;
}

void func_02055738(BattleCommandPage *page, u32 a, u32 b, u32 c) {
    func_02055764(page, a, b, 0xc0, 0, 0xe0, 0, c);
}

void func_02055764(BattleCommandPage *page, u32 a, u32 b, s16 c, s16 d, s16 e, s16 f, u32 g) {
    if (page->mUnk_50 != NULL) {
        delete page->mUnk_50;
    }
    data_020b30d4[0].mUnk_04 = 0x208;
    data_020b30d4[0].mUnk_18 = 1;
    data_020b30d4[1].mUnk_04 = 0x408;
    data_020b30d4[1].mUnk_18 = 1;
    data_020b30d4[2].mUnk_04 = 0x8000;
    data_020b30d4[2].mUnk_18 = 0;
    data_020b30d4[3].mUnk_04 = 0x8000;
    data_020b30d4[3].mUnk_18 = 0;
    data_020b30d4[0].mX      = c;
    data_020b30d4[0].mY      = d;
    data_020b30d4[1].mX      = e;
    data_020b30d4[1].mY      = f;
    data_020b30d4[2].mX      = c;
    data_020b30d4[2].mY      = d;
    data_020b30d4[3].mX      = e;
    data_020b30d4[3].mY      = f;
    data_020b3144[0].mUnk_08 = a;
    data_020b3144[0].mUnk_0c = b;
    data_020b3144[1].mUnk_08 = a;
    data_020b3144[1].mUnk_0c = b;
    data_020b3144[2].mUnk_08 = a;
    data_020b3144[2].mUnk_0c = b;
    data_020b3144[3].mUnk_08 = a;
    data_020b3144[3].mUnk_0c = b;
    u32 count;
    if (g & 0xff) {
        count = 4;
    } else {
        count = 2;
    }
    page->mUnk_fe = g;
    page->mUnk_50 = new SelectWindow(count, data_020b30d4, -1);
    page->mUnk_50->SetCells(count, data_020b3144);
    page->mUnk_50->mCursor->mSelection = -1;
    page->mUnk_fd                      = 1;
    page->func_02055940();
}

void BattleCommandPage::func_02055890(u32 enable) {
    if (enable != 0) {
        if (mUnk_50 != NULL) {
            mUnk_fd = 1;
        }
    } else {
        mUnk_fd = 0;
    }
}

void BattleCommandPage::func_020558b8() {
    if (mUnk_50->Update(FALSE)) {
        SoundManager::Get().func_020246f8(0);
        func_020556d0(*(u8 *) &mUnk_50->mCursor->mSelection);
    }
    func_02055940();
}

void BattleCommandPage::func_02055940() {
    if (mUnk_50 == NULL) {
        return;
    }
    u8 selected = mUnk_fb;
    u8 count    = mUnk_fc;
    WindowItem *items = mUnk_50->mCursor->mItems;
    if (selected < count) {
        if (func_02055ab4(1) == 0) {
            mUnk_50->mCells[1]->SetAnimation(3);
            items[1].mFlags = 0x408;
            if (mUnk_fe != 0) {
                mUnk_50->mCells[3]->SetAnimation(7);
            }
        }
    } else {
        if (func_02055ab4(1) != 0) {
            mUnk_50->mCells[1]->SetAnimation(5);
            items[1].mFlags = 0x8000;
            if (mUnk_fe != 0) {
                mUnk_50->mCells[3]->SetAnimation(5);
            }
        }
    }
    if (mUnk_fb != 0) {
        if (func_02055ab4(0) != 0) {
            return;
        }
        mUnk_50->mCells[0]->SetAnimation(0);
        items[0].mFlags = 0x208;
        if (mUnk_fe == 0) {
            return;
        }
        mUnk_50->mCells[2]->SetAnimation(6);
    } else {
        if (func_02055ab4(0) == 0) {
            return;
        }
        mUnk_50->mCells[0]->SetAnimation(2);
        items[0].mFlags = 0x8000;
        if (mUnk_fe == 0) {
            return;
        }
        mUnk_50->mCells[2]->SetAnimation(2);
    }
}

BOOL BattleCommandPage::func_02055ab4(u32 right) {
    if (right == 0) {
        if (mUnk_50->mCells[0]->mUnk_0e == 2) {
            return FALSE;
        }
    } else {
        if (mUnk_50->mCells[1]->mUnk_0e == 5) {
            return FALSE;
        }
    }
    return TRUE;
}

void BattleCommandPage::func_02055af4() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(0, 1);
    func_02001e68(1, 0, 0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG0Priority(3);
    GX_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6400000, 0x40000);
}

void BattleCommandPage::func_02055c1c() {
    mUnk_f0 = 0;
}

void BattleCommandPage::func_02056d38() {
    if (mUnk_f4 == 0) {
        return;
    }
    if (mUnk_106 != 0) {
        return;
    }
    SpriteManager::Get().mScreens[0].func_02027778(mUnk_6c[mUnk_108], mUnk_84[mUnk_108], 0x73, 0x20, 3, 0, 0xf, 0);
    if ((s32) mUnk_100 > 0xf0) {
        mUnk_102--;
        if (mUnk_102 < -(s32) mUnk_100) {
            mUnk_102 = 0x100;
        }
    } else {
        mUnk_102 = 0x80 - ((u32) mUnk_100 >> 1);
    }
    u8 i = 0;
    do {
        s16 x = mUnk_102 + (i << 5);
        if ((x < 0x120) & (x > -0x20)) {
            SpriteManager::Get().mScreens[0].func_02027778(data_0209e570, 1, x, 8, 1, (u16) (0x80 + i * 2), 0xf, 0);
        }
        i++;
    } while (i < (s32) ((u32) mUnk_100 >> 5) + 1);
}

void BattleCommandPage::func_02056f38() {
    for (u8 i = 0; i < 3; i++) {
        MI_CpuClear16(mUnk_60[i], mUnk_78[i]);
    }
}

void BattleCommandPage::func_02056f70() {
    if (mUnk_106 != 0) {
        return;
    }
    SpriteManager::Get().mScreens[0].func_02027778(data_0209e564, 1, 0x54, 0x25, 3, 0xa0, 0, 0);
    SpriteManager::Get().mScreens[0].func_02027778(data_0209e564, 1, 0x54, 0x65, 3, 0xb0, 0, 0);
    SpriteManager::Get().mScreens[0].func_02027778(data_0209e558, 1, 0x14, 0x25, 3, 0xc0, 0, 0);
    SpriteManager::Get().mScreens[0].func_02027778(data_0209e558, 1, 0x14, 0x65, 3, 0xe0, 0, 0);
}

BOOL BattleCommandPage::func_02055c28() {
    if (mUnk_f0 != 0) {
        if (func_02023874() != 0) {
            return FALSE;
        }
        mUnk_90[mUnk_f0 - 1] = func_02023864();
    }
    if (mUnk_f0 < 12) {
        func_02023708(data_020b3070[mUnk_f0], 0, 0, 0, (u32) &mUnk_c0[mUnk_f0], 0);
        mUnk_f0++;
        return FALSE;
    }
    func_02025c1c(0, 1, 0, mUnk_90[9], mUnk_c0[9], 1);
    func_02025c1c(0, 0xb, 0, mUnk_90[10], mUnk_c0[10], 1);
    func_02025c1c(0, 7, 0, mUnk_90[11], mUnk_c0[11], 1);
    SpriteScreen &screen = SpriteManager::Get().mScreens[0];
    screen.func_02027624(3);
    func_02025c1c(screen.mBgIndex, 5, 0, mUnk_90[0], mUnk_c0[0], 1);
    func_02025c1c(screen.mBgIndex, 4, 0x8000, mUnk_90[1], mUnk_c0[1], 1);
    if (mUnk_0c[0] != NULL) {
        OS_Free(mUnk_0c[0]);
    }
    mUnk_0c[0] = mUnk_90[2];
    if (mUnk_0c[0] != NULL) {
        CellAnimFile *file = mUnk_18[0];
        if (file != NULL) {
            delete file;
        }
        mUnk_18[0] = NULL;
        mUnk_18[0] = new CellAnimFileData(mUnk_0c[0]);
        if (mUnk_18[0] != NULL) {
            for (u8 i = 0; i < 6; i++) {
                CellAnimation *cell = mCells[i];
                if (cell != NULL) {
                    delete cell;
                }
                mCells[i]            = new CellAnimation(mUnk_18[0], 0);
                mCells[i]->mCharBase = screen.func_020276f4(0x8000);
                mCells[i]->mUnk_11   = 0;
            }
            mCells[0]->SetAnimation(0);
            mCells[1]->SetAnimation(1);
            mCells[2]->SetAnimation(3);
            mCells[3]->SetAnimation(7);
            mCells[4]->SetAnimation(8);
            mCells[5]->SetAnimation(9);
        }
    }
    func_02025c1c(screen.mBgIndex, 5, 0x140, mUnk_90[3], mUnk_c0[3], 1);
    func_02025c1c(screen.mBgIndex, 4, 0x14000, mUnk_90[4], mUnk_c0[4], 1);
    if (mUnk_0c[1] != NULL) {
        OS_Free(mUnk_0c[1]);
    }
    mUnk_0c[1] = mUnk_90[5];
    if (mUnk_0c[1] != NULL) {
        CellAnimFile *file = mUnk_18[1];
        if (file != NULL) {
            delete file;
        }
        mUnk_18[1] = NULL;
        mUnk_18[1] = new CellAnimFileData(mUnk_0c[1]);
        if (mUnk_18[1] != NULL) {
            CellAnimation *cell = mCells[6];
            if (cell != NULL) {
                delete cell;
            }
            mCells[6]            = new CellAnimation(mUnk_18[1], 0);
            mCells[6]->mCharBase = screen.func_020276f4(0x14000);
            mCells[6]->mUnk_11   = 10;
        }
    }
    func_02025c1c(screen.mBgIndex, 5, 0x100, mUnk_90[6], mUnk_c0[6], 1);
    func_02025c1c(screen.mBgIndex, 4, 0x12000, mUnk_90[7], mUnk_c0[7], 1);
    if (mUnk_0c[2] != NULL) {
        OS_Free(mUnk_0c[2]);
    }
    mUnk_0c[2] = mUnk_90[8];
    if (mUnk_0c[2] != NULL) {
        CellAnimFile *file = mUnk_18[2];
        if (file != NULL) {
            delete file;
        }
        mUnk_18[2] = NULL;
        mUnk_18[2] = new CellAnimFileData(mUnk_0c[2]);
        if (mUnk_18[2] != NULL) {
            for (u8 i = 7; i < 11; i++) {
                CellAnimation *cell = mCells[i];
                if (cell != NULL) {
                    delete cell;
                }
                mCells[i]            = new CellAnimation(mUnk_18[2], 0);
                mCells[i]->mCharBase = screen.func_020276f4(0x12000);
                mCells[i]->mUnk_11   = 8;
            }
            mCells[7]->SetAnimation(1);
            mCells[8]->SetAnimation(3);
            mCells[9]->SetAnimation(2);
            mCells[10]->SetAnimation(4);
        }
    }
    return TRUE;
}

BOOL BattleCommandPage::func_020561c4() {
    char file[0x7f];
    char path[0x7f];
    char name[0x10];
    if (mUnk_f0 != 0) {
        if (func_02023874() != 0) {
            return FALSE;
        }
        mUnk_90[mUnk_f0 - 1] = func_02023864();
    }
    if (mUnk_f0 < 2) {
        if (mUnk_f4 < 0xd5) {
            sprintf(file, "/Game/Card/Graphic/Large/card_l_%03d", func_02065c20(mUnk_f4));
        } else {
            switch (mUnk_f4 - 0xd7) {
                case 0:
                    sprintf(name, "gold");
                    break;
                case 1:
                    sprintf(name, "silver");
                    break;
                case 2:
                    sprintf(name, "copper");
                    break;
                case 3:
                    sprintf(name, "red");
                    break;
                case 4:
                    sprintf(name, "green");
                    break;
                default:
                    sprintf(name, "blue");
                    break;
            }
            sprintf(file, "/Game/Card/Graphic/Large/card_l_%s", name);
        }
        if (mUnk_f0 == 0) {
            sprintf(path, "%s_ncl.bin", file);
        } else if (mUnk_f0 == 1) {
            sprintf(path, "%s.nce_ncg1D.bin", file);
        }
        func_02023708(path, 0, 0, 0, (u32) &mUnk_c0[mUnk_f0], 0);
        mUnk_f0++;
        return FALSE;
    }
    SpriteScreen &screen = SpriteManager::Get().mScreens[0];
    func_02025c1c(screen.mBgIndex, 0, 0, mUnk_90[0], mUnk_c0[0], 1);
    func_02025c1c(0, 4, 0x5000, mUnk_90[1], mUnk_c0[1], 1);
    return TRUE;
}

void BattleCommandPage::func_02056414() {
    SpriteManager::Get().mScreens[0].func_02027624(3);
    mUnk_58 = func_02023504("/font/mes_CardText.mes", 0, 0, 0, 0, 0);
    mUnk_54 = func_02023504("/font/mes_Card.mes", 0, 0, 0, 0, 0);
    mUnk_84[0] = TextManager::Get().GlyphCount(0x18);
    mUnk_6c[0] = OS_Alloc((s16) mUnk_84[0] * (s16) 0xc);
    TextManager::Get().Init(mUnk_6c[0], 0, 0, 0x10, 0xc, 0x21);
    mUnk_78[1] = TextManager::Get().BufferSize(0x10, 0x10);
    mUnk_60[1] = OS_Alloc(mUnk_78[1]);
    MI_CpuClear16(mUnk_60[1], mUnk_78[1]);
    mUnk_84[1] = TextManager::Get().GlyphCount(0x20);
    mUnk_6c[1] = OS_Alloc((s16) mUnk_84[1] * (s16) 0xc);
    TextManager::Get().Init(mUnk_6c[1], 0, 0, 0x10, 0x10, 0x21);
    mUnk_5c = func_02023504("/font/mes_CardName.mes", 0, 0, 0, 0, 0);
    mUnk_78[2] = TextManager::Get().BufferSize(0x10, 8);
    mUnk_60[2] = OS_Alloc(mUnk_78[2]);
    MI_CpuClear16(mUnk_60[2], mUnk_78[2]);
    mUnk_84[2] = TextManager::Get().GlyphCount(0x10);
    mUnk_6c[2] = OS_Alloc((s16) mUnk_84[2] * (s16) 0xc);
    TextManager::Get().Init(mUnk_6c[2], 0, 0, 0x40, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    MI_CpuClearFast((void *) 0x6400000, 0x5000);
    func_02056f38();
}

void BattleCommandPage::func_02056814(u32 index) {
    mUnk_fb = index;
    func_02056f38();
    MessageFile **textFile;
    u16 id = mUnk_f4;
    if (id == 0) {
        return;
    }
    MessageFile **nameFile;
    u16 id2;
    u32 height;
    int total;
    if (id < 0xd5) {
        id2      = id;
        textFile = (MessageFile **) &mUnk_58;
        nameFile = (MessageFile **) &mUnk_5c;
        if (func_02065c0c(id, 0) == 0) {
            mUnk_108 = 0;
            total    = 0x58;
            height   = 0xc;
        } else {
            mUnk_108 = 1;
            total    = 0x80;
            height   = 0x10;
        }
    } else {
        textFile = (MessageFile **) &mUnk_54;
        s32 t    = id - 0xd7;
        nameFile = textFile;
        u32 clamped;
        if (t > 5) {
            clamped = 5;
        } else if (t < 0) {
            clamped = 0;
        } else {
            clamped = t;
        }
        id       = clamped;
        id2      = t % 6 + 6;
        mUnk_108 = 1;
        total    = 0x80;
        height   = 0x10;
    }
    u8 base = (total / 14) * 14;
    u8 rem  = total % 14;
    if (mUnk_fb == 0) {
        u16 w1;
        u16 h1;
        void *buffer = mUnk_60[1];
        TextManager &text = TextManager::Get();
        MessageFile *file = *textFile;
        h1 = rem - base * mUnk_fb;
        w1 = 0x10;
        MI_CpuClear16(data_020c1a4c, 0x20);
        text.mUnk_58 = 1;
        text.Print(file->GetText(id), file->GetLength(id), buffer, 0x10, height, (s16 *) &w1, (s16 *) &h1, NULL, 0, 2, 0x21, 0xe, 0, 1, 0,
                   1, 1);
        for (u8 i = 1; i < 0x10; i++) {
            if (text.mUnk_36[i + 1] == 0) {
                text.mUnk_36[i + 1] = text.mUnk_36[i];
            }
        }
        mUnk_fc = (h1 - 0xe) / base;
    }
    {
        u16 w2;
        u16 h2;
        TextManager &text = TextManager::Get();
        MessageFile *file = *textFile;
        h2 = rem - base * mUnk_fb;
        w2 = 0x10;
        text.Print(file->GetText(id), file->GetLength(id), mUnk_60[1], 0x10, height, (s16 *) &w2, (s16 *) &h2, NULL, 0, 2, 0x21, 0xe, 0, 0,
                   0, 1, 1);
    }
    TextManager::Get().mUnk_5b = 1;
    {
        u16 w3 = 0;
        u16 h3 = 0;
        MessageFile *file = *nameFile;
        TextManager::Get().Print(file->GetText(id2), file->GetLength(id2), mUnk_60[2], 0x40, 2, (s16 *) &w3, (s16 *) &h3, &mUnk_100, 0, 2,
                                 0x21, 0xe, 0, 0, 0, 1, 1);
    }
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0, mUnk_60[1], mUnk_78[1], 0);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x4000, mUnk_60[2], mUnk_78[2], 0);
}
