#include "UI/SelectWindow.hpp"

// item table of the page window (positions are patched by func_02055764)
inline WindowItemRecord *PageItems() {
    static WindowItemRecord sItems[4] = {
        { 0, 0x208, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 1 },
        { 1, 0x408, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 1 },
        { 2, 0x8000, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 0 },
        { 3, 0x8000, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 0 },
    };
    return sItems;
}

// tables passed to SpriteScreen::func_02027778
static const u16 data_0209e558[3][6] = {
    {0x2000, 0xc000, 0, 0, 0, 0},
    {0xa000, 0xc000, 0, 0, 0, 0},
    {0x4000, 0x8000, 0, 0, 0, 0},
};

#define PAGE "/Game/2D/union_b/union_b_obj_page_nce.bin"
inline WindowCellRecord *PageCells() {
    static WindowCellRecord sCells[4] = {
        { PAGE, {0}, 0, {0}, {0}, 1, {0, 1, 1, 0} },
        { PAGE, {3}, 0, {0}, {0}, 1, {3, 4, 4, 0} },
        { PAGE, {6}, 0, {0}, {0}, 1, {6, 6, 6, 0} },
        { PAGE, {7}, 0, {0}, {0}, 1, {7, 7, 7, 0} },
    };
    return sCells;
}

#include "Text/TextManager.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/BattleCommandPage.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"

// paths loaded by func_02055c28 and func_020561c4
inline const char **PagePaths() {
    static const char *sPaths[12] = {
        "/Game/Card/Graphic/cardspec_u_obj_00_ncl.bin",
        "/Game/Card/Graphic/cardspec_u_obj_00.nce_ncg1D.bin",
        "/Game/Card/Graphic/cardspec_u_obj_00_nce.bin",
        "/Game/Card/Graphic/icon_cbattle_ncl.bin",
        "/Game/Card/Graphic/icon_cbattle.nce_ncg1D.bin",
        "/Game/Card/Graphic/icon_cbattle_nce.bin",
        "/Game/2D/union_b/union_b_obj_you_ncl.bin",
        "/Game/2D/union_b/union_b_obj_you.nce_ncg1D.bin",
        "/Game/2D/union_b/union_b_obj_you_nce.bin",
        "/Game/Card/Graphic/cardspec_u_bg_00_ncl.bin",
        "/Game/Card/Graphic/cardspec_u_bg_00_ncg.bin",
        "/Game/Card/Graphic/cardspec_u_bg_00_nsc.bin",
    };
    return sPaths;
}

#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "Graphics/Display.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

u16 func_02065c20(u32 id);
u8 GetCardInfo(u32 id, u32 kind);
extern u16 data_020c1a4c[0x10]; // palette scratch (cleared before each text)

BattleCommandPage::BattleCommandPage(u16 a, s32 b) :
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
                    SpriteManager::Get().sFade[0] = 2;
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
                    SpriteManager::Get().sFade[0] = 3;
                    break;
                case 2:
                    SpriteManager::Get();
                    SpriteManager::sFade[0] = 4;
                    SpriteManager::sFade[1]  = 4;
                    break;
                case 3:
                    SpriteManager::Get();
                    SpriteManager::sFade[0] = 3;
                    SpriteManager::sFade[1]  = 3;
                    break;
            }
            mUnk_f1++;
            break;
        case 6:
            if (SpriteManager::Get().sFade[0] <= 1) {
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
    if (SpriteManager::Get().sBrightness[1] != 0) {
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

u16 BattleCommandPage::func_02055644(u16 id) {
    if (id == 0xd6) {
        mUnk_10a = 1;
        return 0;
    }
    mUnk_10a = 0;
    return id;
}

void func_02055668(BattleCommandPage *page, u16 a, s32 b) {
    page->mUnk_fa = b;
    if (page->mUnk_f1 == 0) {
        u16 value      = page->func_02055644(a);
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
    PageItems()[0].mUnk_04 = 0x208;
    PageItems()[0].mUnk_18 = 1;
    PageItems()[1].mUnk_04 = 0x408;
    PageItems()[1].mUnk_18 = 1;
    PageItems()[2].mUnk_04 = 0x8000;
    PageItems()[2].mUnk_18 = 0;
    PageItems()[3].mUnk_04 = 0x8000;
    PageItems()[3].mUnk_18 = 0;
    PageItems()[0].mX      = c;
    PageItems()[0].mY      = d;
    PageItems()[1].mX      = e;
    PageItems()[1].mY      = f;
    PageItems()[2].mX      = c;
    PageItems()[2].mY      = d;
    PageItems()[3].mX      = e;
    PageItems()[3].mY      = f;
    PageCells()[0].mUnk_08 = a;
    PageCells()[0].mUnk_0c = b;
    PageCells()[1].mUnk_08 = a;
    PageCells()[1].mUnk_0c = b;
    PageCells()[2].mUnk_08 = a;
    PageCells()[2].mUnk_0c = b;
    PageCells()[3].mUnk_08 = a;
    PageCells()[3].mUnk_0c = b;
    u32 count;
    page->mUnk_fe = g;
    if (page->mUnk_fe != 0) {
        count = 4;
    } else {
        count = 2;
    }
    page->mUnk_50 = new SelectWindow(count, PageItems(), -1);
    page->mUnk_50->SetCells(count, PageCells());
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
    GX_SetGraphicsMode(1, 0, 0);
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

BOOL BattleCommandPage::func_02055c28() {
    if (mUnk_f0 != 0) {
        if (func_02023874() != 0) {
            return FALSE;
        }
        mUnk_90[mUnk_f0 - 1] = func_02023864();
    }
    if (mUnk_f0 < 12) {
        func_02023708(PagePaths()[mUnk_f0], 0, 0, 0, &mUnk_c0[mUnk_f0], 0);
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
        func_02023708(path, 0, 0, 0, &mUnk_c0[mUnk_f0], 0);
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
    u16 height;
    int total;
    if (id < 0xd5) {
        id2      = id;
        textFile = (MessageFile **) &mUnk_58;
        nameFile = (MessageFile **) &mUnk_5c;
        if (GetCardInfo(id, 0) == 0) {
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
    u8 rem  = total % 14;
    u8 base = (total / 14) * 14;
    if (mUnk_fb == 0) {
        u16 w1;
        u16 h1;
        TextManager &text = TextManager::Get();
        h1 = rem - base * mUnk_fb;
        w1 = 0x10;
        void *buffer = mUnk_60[1];
        MessageFile *file = *textFile;
        MI_CpuClear16(data_020c1a4c, 0x20);
        text.mUnk_58 = 1;
        text.Print(file->GetText(id), file->GetLength(id), buffer, 0x10, height, (s16 *) &w1, (s16 *) &h1, NULL, 0, 2, 0x21, 0xe, 0, 1, 0,
                   1, 1);
        u16 hh = h1;
        for (u8 i = 1; i < 0x10; i++) {
            if (text.mUnk_36[i + 1] == 0) {
                text.mUnk_36[i + 1] = text.mUnk_36[i];
            }
        }
        mUnk_fc = (hh - 0xe) / base;
    }
    {
        u16 w2;
        u16 h2;
        TextManager &text = TextManager::Get();
        h2 = rem - base * mUnk_fb;
        w2 = 0x10;
        MessageFile *file = *textFile;
        text.Print(file->GetText(id), file->GetLength(id), mUnk_60[1], 0x10, height, (s16 *) &w2, (s16 *) &h2, NULL, 0, 2, 0x21, 0xe, 0, 0,
                   0, 1, 1);
    }
    TextManager::Get().mUnk_5b = 1;
    {
        u16 w3;
        u16 h3;
        TextManager &text = TextManager::Get();
        h3 = 0;
        w3 = 0;
        MessageFile *file = *nameFile;
        text.Print(file->GetText(id2), file->GetLength(id2), mUnk_60[2], 0x40, 2, (s16 *) &w3, (s16 *) &h3, &mUnk_100, 0, 2,
                                 0x21, 0xe, 0, 0, 0, 1, 1);
    }
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0, mUnk_60[1], mUnk_78[1], 0);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x4000, mUnk_60[2], mUnk_78[2], 0);
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
            SpriteManager::Get().mScreens[0].func_02027778((void *) data_0209e558[2], 1, x, 8, 1, (u16) (0x80 + i * 2), 0xf, 0);
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
    SpriteManager::Get().mScreens[0].func_02027778((void *) data_0209e558[1], 1, 0x54, 0x25, 3, 0xa0, 0, 0);
    SpriteManager::Get().mScreens[0].func_02027778((void *) data_0209e558[1], 1, 0x54, 0x65, 3, 0xb0, 0, 0);
    SpriteManager::Get().mScreens[0].func_02027778((void *) data_0209e558[0], 1, 0x14, 0x25, 3, 0xc0, 0, 0);
    SpriteManager::Get().mScreens[0].func_02027778((void *) data_0209e558[0], 1, 0x14, 0x65, 3, 0xe0, 0, 0);
}

u8 func_02065bf4(u32 id, u32 index);
extern u8 data_020d43d7[];

void BattleCommandPage::func_020571a8() {
    u8 big;
    if (mUnk_109 != 0) {
        big = 1;
    } else if (mUnk_f8 < 0xd5) {
        big = GetCardInfo(mUnk_f8, 0);
    } else if (mUnk_f8 < 0xda) {
        big = 0;
    } else {
        big = 1;
    }
    mCells[0]->func_0202a3ec(0, big, 0x80, 0x60, 0);
    if (mUnk_106 != 0) {
        return;
    }
    if (mUnk_f8 >= 0xd5) {
        return;
    }
    if (mUnk_fa > -1) {
        u8 cur   = data_020d433c.mBattlePlayers->mUnk_2a9;
        BOOL has;
        if (mUnk_fa == cur) {
            mCells[8]->func_0202a3ec(0, mUnk_fa, 0x24, 0x18, 0);
        } else {
            has = 0;
            if (data_020d433c.mBattlePlayers->mPlayers[(u8)mUnk_fa]->mUnk_c8 != NULL) {
                has = 1;
            }
            if (has == 1) {
                mCells[9]->func_0202a3ec(0, mUnk_fa, 0x15, 0x12, 0);
            }
            if ((data_020d433c.mUnk_98 == 1) & (mUnk_fa == data_020d43d7[cur])) {
                mCells[10]->func_0202a3ec(0, mUnk_fa, 0x24, 0x18, 0);
            }
        }
        mCells[7]->func_0202a3ec(0, mUnk_fa, 0, 0x16, 0);
    }
    if (mUnk_f4 == 0) {
        return;
    }
    s16 a;
    u8 k;
    s16 n;
    s32 d;
    s32 b;
    s16 c;
    if (big) {
        a = 0x32;
        b = -4;
        c = 0x17;
        d = 3;
    } else {
        a = 0x29;
        b = -3;
        c = 0x18;
        d = 4;
    }
    for (k = 0; k < 6; k++) {
        s16 y = k * 0x28 + 0x14;
        n = func_02065bf4(mUnk_f8, k);
        if (n != 0) {
            u8 cnt;
            s32 half;
            u32 i;
            u32 v;
            s32 left;
            s32 ty;
            v = n;
            cnt = 3;
            if (n < 10) {
                cnt--;
                y -= 3;
            }
            v *= 10;
            half = (3 - cnt) * 4 / 2;
            ty = a;
            ty += d;
            left = c;
            left += b;
            i    = 0;
            do {
                mCells[2]->func_0202a3ec(0, (s32) v % 10, y + (3 - i) * 7, 0xb0, 0);
                mCells[5]->func_0202a3ec(0, (s32) v % 10, left + (cnt - i) * 4 + half, ty, 0);
                v /= 10;
                i = (u8) (i + 1);
            } while (i < cnt);
        }
        u8 idx = 3;
        if (big) {
            idx = 4;
        }
        if (n != 0) {
            mCells[1]->func_0202a3ec(0, k, 0x80, 0x60, 0);
            mCells[idx]->func_0202a3ec(0, k, c, a, 0);
        }
        if (big) {
            a += 0xd;
        } else {
            a += 0x11;
        }
    }
    if (big) {
        return;
    }
    if (mUnk_f8 == 0) {
        return;
    }
    mCells[6]->func_0202a3ec(0, GetCardInfo(mUnk_f8, 2), 0x80, 0x84, 0);
}
