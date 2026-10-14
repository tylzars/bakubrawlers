#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/BattleCommandList.hpp"
#include "Graphics/UnkStruct_0203d374.hpp"
#include "System/FileSystem.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/Model.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Text/TextManager.hpp"

// per cell values, five columns
static const u8 data_0209e5bc[41][5] = {
    {3, 1, 1, 0, 2},
    {3, 2, 2, 0, 2},
    {2, 1, 0, 1, 2},
    {3, 2, 0, 1, 2},
    {1, 1, 3, 2, 0},
    {0, 1, 3, 2, 2},
    {0, 1, 3, 2, 2},
    {2, 3, 2, 0, 0},
    {3, 3, 2, 0, 0},
    {1, 0, 2, 3, 1},
    {2, 0, 3, 3, 1},
    {2, 2, 0, 3, 1},
    {3, 2, 1, 2, 1},
    {2, 1, 3, 2, 0},
    {1, 3, 1, 0, 2},
    {0, 1, 3, 3, 1},
    {2, 1, 1, 1, 3},
    {2, 2, 3, 0, 0},
    {3, 0, 0, 3, 1},
    {0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0},
    {2, 0, 0, 0, 1},
    {0, 0, 0, 0, 1},
    {2, 1, 0, 1, 0},
    {3, 1, 0, 1, 0},
    {0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1},
    {0, 2, 0, 0, 1},
    {0, 3, 1, 0, 0},
    {0, 0, 1, 1, 0},
    {0, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {0, 1, 2, 1, 1},
    {1, 0, 0, 2, 0},
    {2, 1, 1, 2, 1},
    {2, 1, 2, 3, 1},
    {1, 3, 0, 3, 1},
    {1, 2, 1, 3, 2},
    {3, 1, 1, 0, 3},
};

static const u16 data_0209e588[6] = {0x4000, 0x8000, 0, 0, 0, 0};

// per command, six message indices (rows of six; 0xf8 bytes, which puts the table in a section of its own)
static const u8 data_0209e68c[0xf8] = {
    0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 0, 0,
    0, 0, 0, 3, 0, 0,
    0, 4, 0, 0, 0, 0,
    0, 5, 0, 0, 0, 0,
    0, 5, 0, 0, 0, 0,
    0, 0, 6, 0, 0, 0,
    0, 0, 7, 0, 0, 0,
    0, 0, 0, 0, 0, 8,
    0, 0, 0, 0, 0, 9,
    0, 0, 0, 0, 10, 0,
    0, 0, 0, 0, 11, 0,
    124, 125, 127, 126, 12, 128,
    0, 0, 13, 0, 0, 0,
    0, 14, 0, 0, 0, 0,
    15, 0, 0, 0, 0, 0,
    0, 16, 0, 16, 0, 0,
    0, 0, 0, 0, 0, 17,
    22, 23, 26, 24, 25, 27,
    28, 29, 32, 30, 31, 33,
    34, 35, 38, 36, 37, 39,
    40, 41, 44, 42, 43, 45,
    46, 47, 50, 48, 49, 51,
    52, 53, 56, 54, 55, 57,
    112, 113, 116, 114, 115, 117,
    58, 59, 62, 60, 61, 63,
    64, 65, 68, 66, 67, 69,
    70, 71, 74, 72, 73, 75,
    76, 77, 80, 78, 79, 81,
    82, 83, 86, 84, 85, 87,
    88, 89, 92, 90, 91, 93,
    94, 95, 98, 96, 97, 99,
    100, 101, 104, 102, 103, 105,
    106, 107, 110, 108, 109, 111,
    118, 119, 122, 120, 121, 123,
    18, 18, 18, 18, 18, 18,
    19, 19, 19, 19, 19, 19,
    0, 0, 0, 0, 20, 0,
    0, 0, 0, 0, 21, 0,
    0, 0, 0, 0, 0, 0,
};

// item table of the list window (positions are patched by func_02058404)
inline WindowItemRecord *ListItems() {
    static WindowItemRecord sItems[4] = {
        { 0, 0x208, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 1 },
        { 1, 0x408, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 1 },
        { 2, 0x8000, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 0 },
        { 3, 0x8000, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 0 },
    };
    return sItems;
}

#define PAGE "/Game/2D/union_b/union_b_obj_page_nce.bin"
inline WindowCellRecord *ListCells() {
    static WindowCellRecord sCells[4] = {
        { PAGE, {0}, 0, {0}, {0}, 1, {0, 1, 1, 0} },
        { PAGE, {3}, 0, {0}, {0}, 1, {3, 4, 4, 0} },
        { PAGE, {6}, 0, {0}, {0}, 1, {6, 6, 6, 0} },
        { PAGE, {7}, 0, {0}, {0}, 1, {7, 7, 7, 0} },
    };
    return sCells;
}

#include "Graphics/SpriteManager.hpp"
#include "Graphics/Camera.hpp"
#include "System/SoundManager.hpp"

// paths loaded by func_020589f4
inline const char **ListPaths() {
    static const char *sPaths[14] = {
        "/Game/Card/Graphic/cardspec_u_obj_back_ncl.bin",
        "/Game/Card/Graphic/cardspec_u_obj_back.nce_ncg1D.bin",
        "/Game/Card/Graphic/cardspec_u_obj_00_ncl.bin",
        "/Game/Card/Graphic/cardspec_u_obj_00.nce_ncg1D.bin",
        "/Game/Card/Graphic/cardspec_u_obj_00_nce.bin",
        "/Game/2D/union_b/union_b_obj_gp_ncl.bin",
        "/Game/2D/union_b/union_b_obj_gp.nce_ncg1D.bin",
        "/Game/2D/union_b/union_b_obj_gp_nce.bin",
        "/Game/2D/union_b/union_b_obj_you_ncl.bin",
        "/Game/2D/union_b/union_b_obj_you.nce_ncg1D.bin",
        "/Game/2D/union_b/union_b_obj_you_nce.bin",
        "/Game/Card/Graphic/bakugan_sub_l_ncl.bin",
        "/Game/Card/Graphic/bakugan_sub_l.nce_ncg1D.bin",
        "/Game/Card/Graphic/bakugan_sub_l_nce.bin",
    };
    return sPaths;
}

// light table of the list model
static const u16 data_0209e594[20] = {1, 0x7fff, 0xf666, 0xf5ec, 0xf829};

// table of func_0205aea4
static const u16 data_0209e57c[6] = {0x2000, 0xc000, 0, 0, 0, 0};

extern u16 data_020c1a4c[];

void func_02023a54();

BattleCommandList::BattleCommandList(u8 a, u8 b, s32 c, u8 d) :
    SceneBase(0x3e8, 0) {
    mUnk_78  = NULL;
    mUnk_7c  = NULL;
    mUnk_80  = NULL;
    mUnk_84  = NULL;
    mUnk_88  = NULL;
    mUnk_8c  = NULL;
    mUnk_90  = NULL;
    mUnk_94  = NULL;
    mUnk_14e = a;
    mUnk_14f = a;
    mUnk_150 = b;
    mUnk_151 = b;
    mUnk_152 = c;
    mUnk_154 = 0;
    mUnk_156 = 0;
    mUnk_157 = 0;
    mUnk_158 = 0;
    mUnk_159 = 0;
    mUnk_15a = 0;
    mUnk_15b = 0;
    mUnk_15c = 0;
    mUnk_15d = 0;
    mUnk_15e = 0;
    mUnk_15f = 0;
    mUnk_160 = 0;
    mUnk_161 = d;
    mUnk_162 = 0;
    mUnk_163 = 0;
    mUnk_164 = 0;
    mUnk_165 = 0;
    mUnk_166 = 0;
    MI_CpuClear32(mUnk_a0, 0xc);
    MI_CpuClear32(mUnk_ac, 0xc);
    MI_CpuClear32(mUnk_b8, 0xc);
    MI_CpuClear32(mUnk_c4, 0xc);
    MI_CpuClear32(mUnk_d0, 0x38);
    MI_CpuClear32(mUnk_108, 0x38);
    MI_CpuClear32(mUnk_1c, 0x10);
    MI_CpuClear32(mUnk_2c, 0x10);
    MI_CpuClear32(mCells3c, 0x1c);
    MI_CpuClear32(mCells58, 0x20);
    MI_CpuFill8(mUnk_0c, 0, 0xa);
    MI_CpuFill8(mUnk_16, 0, 5);
    mUnk_98 = 0;
    mUnk_9a = 0;
    mUnk_9c = 0;
    func_0205af8c();
}

BattleCommandList::~BattleCommandList() {
    func_02023a54();
    if (mUnk_78 != NULL) {
        OS_Free(mUnk_78);
    }
    if (mUnk_7c != NULL) {
        OS_Free(mUnk_7c);
    }
    if (mUnk_80 != NULL) {
        OS_Free(mUnk_80);
    }
    if (mUnk_84 != NULL) {
        OS_Free(mUnk_84);
    }
    for (u8 i = 0; i < 3; i++) {
        if (mUnk_a0[i] != NULL) {
            OS_Free(mUnk_a0[i]);
        }
        if (mUnk_ac[i] != NULL) {
            OS_Free(mUnk_ac[i]);
        }
    }
    for (u8 i = 0; i < 4; i++) {
        if (mUnk_1c[i] != NULL) {
            OS_Free(mUnk_1c[i]);
        }
        CellAnimFile *file = mUnk_2c[i];
        if (file != NULL) {
            delete file;
        }
    }
    CellAnimation *cell;
    for (u8 i = 0; i < 7; i++) {
        cell = mCells3c[i];
        if (cell != NULL) {
            delete cell;
        }
    }
    CellAnimation *cell2;
    for (u8 i = 0; i < 8; i++) {
        cell2 = mCells58[i];
        if (cell2 != NULL) {
            delete cell2;
        }
    }
    delete mUnk_94;
    UnkStruct_02029ca8 *lights = mUnk_88;
    if (lights != NULL) {
        delete lights;
    }
    if (mUnk_8c != NULL) {
        delete mUnk_8c;
    }
    UnkStruct_0203d374 *task = mUnk_90;
    if (task != NULL) {
        delete task;
    }
    CameraManager::Get().func_02026504(0, NULL, 0);
    SpriteManager::Get().func_0202740c();
    GX_SetVisibleWnd(0);
    G2_SetBlendBrightness(0, 0);
}

void BattleCommandList::Update() {
    switch (mUnk_159) {
        case 0:
            func_020587b4();
            func_020597d0();
            func_020589e8();
            mUnk_159++;
            break;
        case 1:
            if (func_020589f4()) {
                mUnk_15d = 1;
                mUnk_159++;
            }
            break;
        case 2:
            if (SpriteManager::Get().sFade[0] <= 1) {
                mUnk_159++;
            }
            break;
        case 3: {
            UnkStruct_0203d374 *task = mUnk_90;
            if (task != NULL) {
                delete task;
            }
            mUnk_90 = NULL;
            SpriteManager::Get().func_0202740c();
            func_020589e8();
            mUnk_159++;
            break;
        }
        case 4:
            if (func_02059734()) {
                mUnk_159++;
            }
            break;
        case 5:
            func_02059c50(0);
            func_020585e0();
            mUnk_159++;
            break;
        case 6: {
            u8 i;
            u8 offset = (mUnk_15b == 0) ? 1 : 0;
            u8 *dst   = (u8 *) this + offset;
            for (i = 0; i < 5; i++) {
                dst[0xc + i * 2] = mUnk_16[i];
            }
            if (mUnk_160 == 0) {
                mUnk_15f = 0;
            }
            if (mUnk_15e == 0) {
                SpriteManager::Get().sFade[0] = 2;
            }
            mUnk_159++;
            break;
        }
        case 7: {
            if (mUnk_160 != 0) {
                mUnk_15f = 1;
            }
            u8 next = mUnk_15c;
            if (mUnk_15b != next) {
                mUnk_15b = next;
            }
            if ((mUnk_14f != mUnk_14e) | (mUnk_151 != mUnk_150)) {
                mUnk_14e = mUnk_14f;
                mUnk_150 = mUnk_151;
                mUnk_160 = 0;
                if (mUnk_15b != 0) {
                    mUnk_15b = 1;
                    mUnk_15c = 1;
                }
                mUnk_159 = 3;
            }
            if (mUnk_15e != 0) {
                mUnk_159 = 8;
            }
            break;
        }
        case 8:
            SpriteManager::Get().sFade[0] = 3;
            mUnk_159++;
            break;
        case 9:
            if (SpriteManager::Get().sFade[0] <= 1) {
                mUnk_15d = 0;
                UnkStruct_0203d374 *task = mUnk_90;
                if (task != NULL) {
                    delete task;
                }
                mUnk_90 = NULL;
                mUnk_159++;
            }
            break;
        case 10:
            RequestScene(1);
            mFinished = 1;
            break;
    }
    if (mUnk_15d == 0) {
        return;
    }
    func_0205a804();
    func_0205a6a8();
    if (mUnk_15f == 0 && mUnk_159 >= 5) {
        func_0205a7ac();
    }
    if (mUnk_158 != 0 && mUnk_15f == 0) {
        if (SpriteManager::Get().sBrightness[1] == 0) {
            if (mUnk_159 == 7) {
                func_02058558();
            }
            mUnk_8c->Draw();
        }
    }
    func_0205aea4();
}

void func_0205821c(BattleCommandList *list) {
    list->mUnk_15e = 1;
}

void func_02058228(BattleCommandList *list, u32 a) {
    u8 value        = a;
    list->mUnk_160 = value;
    if (list->mUnk_159 < 3) {
        list->mUnk_15f = value;
    }
}

void BattleCommandList::func_02058240(u32 value) {
    mUnk_162 = value;
}

void func_02058248(BattleCommandList *list, u8 b) {
    func_02058260(list, 0x2b, b, -1);
}

void func_02058260(BattleCommandList *list, u8 a, u8 b, s32 c) {
    if (a >= 0x2b) {
        if ((list->mUnk_15b == 0) & (list->mUnk_14e < 0x2b)) {
            MI_CpuClear16((void *) 0x6400000, 0x2000);
        }
        list->mUnk_15c = 1;
        list->mUnk_15b = 1;
    } else {
        if (list->mUnk_14e == 0x2b) {
            list->mUnk_15c = 0;
            list->mUnk_15b = 0;
        }
    }
    if (c < 0) {
        list->mUnk_152 = -1;
        for (u8 i = 0; i < 5; i++) {
            list->mUnk_16[i] = 0;
        }
    } else {
        list->mUnk_152 = c % 4;
    }
    if (list->mUnk_159 == 1) {
        list->mUnk_14e = a;
        list->mUnk_14f = a;
        list->mUnk_150 = b;
        list->mUnk_151 = b;
        list->mUnk_15f = 0;
        list->mUnk_160 = 0;
        return;
    }
    list->mUnk_14f = a;
    list->mUnk_151 = b;
    list->mUnk_162 = 0;
    if ((list->mUnk_14f == list->mUnk_14e) & (list->mUnk_151 == list->mUnk_150)) {
        if (list->mUnk_15f != 0) {
            list->mUnk_15f = 0;
        }
    }
    list->mUnk_160 = 0;
}

void BattleCommandList::func_0205839c(u32 flag) {
    if (flag == 0) {
        mUnk_15c = (mUnk_15c + 2) % 3;
    } else {
        mUnk_15c = (mUnk_15c + 1) % 3;
    }
    if (mUnk_15c == 0) {
        return;
    }
    func_02059c50(mUnk_15c - 1);
}

BOOL BattleCommandList::func_020583f0() {
    return mUnk_159 < 7;
}

void func_02058404(BattleCommandList *list, u32 a, u32 b, s16 c, s16 d, s16 e, s16 f, u32 g) {
    if (list->mUnk_8c != NULL) {
        delete list->mUnk_8c;
    }
    ListItems()[0].mUnk_04 = 0x208;
    ListItems()[0].mUnk_18 = 1;
    ListItems()[1].mUnk_04 = 0x408;
    ListItems()[1].mUnk_18 = 1;
    ListItems()[2].mUnk_04 = 0x8000;
    ListItems()[2].mUnk_18 = 0;
    ListItems()[3].mUnk_04 = 0x8000;
    ListItems()[3].mUnk_18 = 0;
    ListItems()[0].mX      = c;
    ListItems()[0].mY      = d;
    ListItems()[1].mX      = e;
    ListItems()[1].mY      = f;
    ListItems()[2].mX      = c;
    ListItems()[2].mY      = d;
    ListItems()[3].mX      = e;
    ListItems()[3].mY      = f;
    ListCells()[0].mUnk_08 = a;
    ListCells()[0].mUnk_0c = b;
    ListCells()[1].mUnk_08 = a;
    ListCells()[1].mUnk_0c = b;
    ListCells()[2].mUnk_08 = a;
    ListCells()[2].mUnk_0c = b;
    ListCells()[3].mUnk_08 = a;
    ListCells()[3].mUnk_0c = b;
    u32 count;
    list->mUnk_163 = g;
    if (list->mUnk_163 != 0) {
        count = 4;
    } else {
        count = 2;
    }
    list->mUnk_8c  = new SelectWindow(count, ListItems(), -1);
    list->mUnk_8c->SetCells(count, ListCells());
    list->mUnk_8c->mCursor->mSelection = -1;
    list->mUnk_158                     = 1;
    list->func_020585e0();
}

void BattleCommandList::func_02058530(u32 enable) {
    if (enable != 0) {
        if (mUnk_8c != NULL) {
            mUnk_158 = 1;
        }
    } else {
        mUnk_158 = 0;
    }
}

void BattleCommandList::func_02058558() {
    if (mUnk_8c->Update(FALSE)) {
        SoundManager::Get().func_020246f8(0);
        func_0205839c(*(u8 *) &mUnk_8c->mCursor->mSelection);
    }
    func_020585e0();
}

void BattleCommandList::func_020585e0() {
    if (mUnk_8c == NULL) {
        return;
    }
    u8 selected       = mUnk_15b;
    u8 count          = mUnk_157;
    WindowItem *items = mUnk_8c->mCursor->mItems;
    if (selected <= count) {
        if (func_02058774(1) == 0) {
            mUnk_8c->mCells[1]->SetAnimation(3);
            items[1].mFlags = 0x408;
            if (mUnk_163 != 0) {
                mUnk_8c->mCells[3]->SetAnimation(7);
            }
        }
    } else {
        if (func_02058774(1) != 0) {
            mUnk_8c->mCells[1]->SetAnimation(5);
            items[1].mFlags = 0x8000;
            if (mUnk_163 != 0) {
                mUnk_8c->mCells[3]->SetAnimation(5);
            }
        }
    }
    BOOL hasPage = FALSE;
    if (mUnk_14e == 0x2b) {
        if (mUnk_156 != 0) {
            hasPage = TRUE;
        }
    } else {
        if (mUnk_15b != 0) {
            hasPage = TRUE;
        }
    }
    if (hasPage) {
        if (func_02058774(0) != 0) {
            return;
        }
        mUnk_8c->mCells[0]->SetAnimation(0);
        items[0].mFlags = 0x208;
        if (mUnk_163 == 0) {
            return;
        }
        mUnk_8c->mCells[2]->SetAnimation(6);
    } else {
        if (func_02058774(0) == 0) {
            return;
        }
        mUnk_8c->mCells[0]->SetAnimation(2);
        items[0].mFlags = 0x8000;
        if (mUnk_163 == 0) {
            return;
        }
        mUnk_8c->mCells[2]->SetAnimation(2);
    }
}

BOOL BattleCommandList::func_02058774(u32 right) {
    if (right == 0) {
        if (mUnk_8c->mCells[0]->mUnk_0e == 2) {
            return FALSE;
        }
    } else {
        if (mUnk_8c->mCells[1]->mUnk_0e == 5) {
            return FALSE;
        }
    }
    return TRUE;
}

void BattleCommandList::func_020587b4() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(0, 4);
    GX_SetGraphicsMode(1, 0, 1);
    GX_SetVisiblePlane(0x11);
    G2_SetBG0Priority(2);
    func_020037e8(0);
    G2_SetBlendBrightness(1, -8);
    GX_SetVisibleWnd(1);
    G2_SetWndOutsidePlane(0x11, FALSE);
    G2_SetWnd0InsidePlane(0x11, TRUE);
    G2_SetWnd0Position(0x7c, 0, 0xff, 0xc0);
    mUnk_94 = new Camera();
    mUnk_94->SetUp(0, 0x1000, 0);
    mUnk_94->SetTarget(0, 0, 0);
    mUnk_94->SetClip(0x1000, 0x1388000);
    mUnk_94->func_02026004(0x2aaa);
    mUnk_94->SetPosition(0, 0, mUnk_94->mUnk_34);
    CameraManager::Get().mActive = 0;
    CameraManager::Get().func_02026504(1, mUnk_94, 0);
}

void BattleCommandList::func_020589e8() {
    mUnk_140 = 0;
}

BOOL BattleCommandList::func_020589f4() {
    if (mUnk_140 != 0) {
        if (func_02023874() != 0) {
            return FALSE;
        }
        mUnk_d0[mUnk_140 - 1] = (u32) func_02023864();
    }
    u32 n = mUnk_140;
    if (n < 14) {
        u32 *out = &mUnk_108[n];
        func_02023708(ListPaths()[n], 0, 0, 0, out, 0);
        mUnk_140++;
        return FALSE;
    }
    SpriteManager::Get().func_0202740c();
    SpriteManager::Get().mScreens[0].func_02027624(3);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 0, 0, (void *) mUnk_d0[0], mUnk_108[0], 1);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x14000, (void *) mUnk_d0[1], mUnk_108[1], 1);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 5, 0x40, (void *) mUnk_d0[2], mUnk_108[2], 1);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x7000, (void *) mUnk_d0[3], mUnk_108[3], 1);
    if (mUnk_1c[0] != NULL) {
        OS_Free(mUnk_1c[0]);
    }
    mUnk_1c[0] = (void *) mUnk_d0[4];
    if (mUnk_1c[0] != NULL) {
        CellAnimFile *file = mUnk_2c[0];
        if (file != NULL) {
            delete file;
        }
        mUnk_2c[0] = new CellAnimFileData(mUnk_1c[0]);
        if (mUnk_2c[0] != NULL) {
            for (u8 i = 0; i < 7; i++) {
                CellAnimation *cell = mCells3c[i];
                if (cell != NULL) {
                    delete cell;
                }
                mCells3c[i]            = new CellAnimation(mUnk_2c[0], 0);
                mCells3c[i]->mCharBase = SpriteManager::Get().Screen(0).func_020276f4(0x7000);
                mCells3c[i]->mUnk_11   = 2;
            }
            mCells3c[0]->SetAnimation(2);
            mCells3c[1]->SetAnimation(3);
            mCells3c[2]->SetAnimation(6);
            mCells3c[4]->SetAnimation(0xa);
            mCells3c[5]->SetAnimation(5);
            mCells3c[3]->SetAnimation(0);
            mCells3c[3]->mUnk_0f = 2;
            mCells3c[6]->SetAnimation(4);
        }
    }
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 5, 0, (void *) mUnk_d0[5], mUnk_108[5], 1);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x4000, (void *) mUnk_d0[6], mUnk_108[6], 1);
    if (mUnk_1c[1] != NULL) {
        OS_Free(mUnk_1c[1]);
    }
    mUnk_1c[1] = (void *) mUnk_d0[7];
    if (mUnk_1c[1] != NULL) {
        CellAnimFile *file = mUnk_2c[1];
        if (file != NULL) {
            delete file;
        }
        mUnk_2c[1] = new CellAnimFileData(mUnk_1c[1]);
        if (mUnk_2c[1] != NULL) {
            for (u8 i = 0; i <= 2; i++) {
                CellAnimation *cell = mCells58[i];
                if (cell != NULL) {
                    delete cell;
                }
                mCells58[i]            = new CellAnimation(mUnk_2c[1], 0);
                mCells58[i]->mCharBase = SpriteManager::Get().Screen(0).func_020276f4(0x4000);
                mCells58[i]->mUnk_11   = 0;
            }
            mCells58[0]->SetAnimation(2);
            mCells58[1]->SetAnimation(1);
            mCells58[2]->SetAnimation(3);
        }
    }
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 5, 0x20, (void *) mUnk_d0[8], mUnk_108[8], 1);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x5000, (void *) mUnk_d0[9], mUnk_108[9], 1);
    if (mUnk_1c[2] != NULL) {
        OS_Free(mUnk_1c[2]);
    }
    mUnk_1c[2] = (void *) mUnk_d0[10];
    if (mUnk_1c[2] != NULL) {
        CellAnimFile *file = mUnk_2c[2];
        if (file != NULL) {
            delete file;
        }
        mUnk_2c[2] = new CellAnimFileData(mUnk_1c[2]);
        if (mUnk_2c[2] != NULL) {
            for (u8 i = 3; i < 7; i++) {
                CellAnimation *cell = mCells58[i];
                if (cell != NULL) {
                    delete cell;
                }
                mCells58[i]            = new CellAnimation(mUnk_2c[2], 0);
                mCells58[i]->mCharBase = SpriteManager::Get().Screen(0).func_020276f4(0x5000);
                mCells58[i]->mUnk_11   = 1;
            }
            mCells58[3]->SetAnimation(1);
            mCells58[4]->SetAnimation(3);
            mCells58[5]->SetAnimation(2);
            mCells58[6]->SetAnimation(4);
        }
    }
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 0, 0x200, (void *) mUnk_d0[11], mUnk_108[11], 1);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0xc000, (void *) mUnk_d0[12], mUnk_108[12], 1);
    if (mUnk_1c[3] != NULL) {
        OS_Free(mUnk_1c[3]);
    }
    mUnk_1c[3] = (void *) mUnk_d0[13];
    if (mUnk_1c[3] != NULL) {
        CellAnimFile *file = mUnk_2c[3];
        if (file != NULL) {
            delete file;
        }
        mUnk_2c[3] = new CellAnimFileData(mUnk_1c[3]);
        if (mUnk_2c[3] != NULL) {
            CellAnimation *cell = mCells58[7];
            if (cell != NULL) {
                delete cell;
            }
            mCells58[7]            = new CellAnimation(mUnk_2c[3], 0);
            mCells58[7]->mCharBase = SpriteManager::Get().Screen(0).func_020276f4(0xc000);
            mCells58[7]->mUnk_11   = 1;
        }
    }
    CameraManager &manager = CameraManager::Get();
    Camera *camera         = manager.mCameras[manager.mActive];
    data_02211374.mCameraPosition = camera->mPosition;
    data_02211374.mCameraUp       = camera->mUp;
    data_02211374.mCameraTarget   = camera->mTarget;
    func_01ff8d34(&camera->mPosition, &camera->mUp, &camera->mTarget, &data_02211374.mView);
    data_02211374.mFlags &= ~0xe8;
    manager.mProjection[manager.mActive] = data_02211374.mProjection;
    manager.mView[manager.mActive]       = data_02211374.mView;
    manager.mMtxA[manager.mActive]       = *func_02032710();
    manager.mMtxB[manager.mActive]       = *func_02032604();
    manager.mMtxC[manager.mActive]       = *func_020323b8();
    mUnk_88 = new UnkStruct_02029ca8(data_0209e594);
    return TRUE;
}

BOOL BattleCommandList::func_02059734() {
    mUnk_14e = mUnk_14f;
    mUnk_150 = mUnk_151;
    if (mUnk_14e >= 0x2b) {
        mUnk_90 = NULL;
        return TRUE;
    }
    mUnk_90 = new UnkStruct_0203d374();
    mUnk_90->func_0203d4dc(mUnk_14e, mUnk_150, 6);
    VecFx32 pos;
    pos.x = -0xd000;
    pos.y = pos.x + 0xb000;
    pos.z = 0xb8500;
    mUnk_90->func_0203db70(&pos);
    return TRUE;
}

void BattleCommandList::func_020597d0() {
    SpriteManager::Get().mScreens[0].func_02027624(3);
    mUnk_7c    = func_02023504("/font/mes_BakuganDisplay.mes", 0, 0, 0, 0, 0);
    mUnk_78    = func_02023504("/font/mes_Bakugan.mes", 0, 0, 0, 0, 0);
    mUnk_b8[0] = TextManager::Get().BufferSize(0x10, 0x12);
    mUnk_a0[0] = OS_Alloc(mUnk_b8[0]);
    MI_CpuClear16(mUnk_a0[0], mUnk_b8[0]);
    mUnk_c4[0] = TextManager::Get().GlyphCount(0x24);
    mUnk_ac[0] = OS_Alloc((s16) mUnk_c4[0] * (s16) 0xc);
    TextManager::Get().Init(mUnk_ac[0], 0, 0, 0x10, 0x12, 0x21);
    mUnk_80    = func_02023504("/font/mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mUnk_b8[1] = TextManager::Get().BufferSize(0x10, 8);
    mUnk_a0[1] = OS_Alloc(mUnk_b8[1]);
    MI_CpuClear16(mUnk_a0[1], mUnk_b8[1]);
    mUnk_c4[1] = TextManager::Get().GlyphCount(0x10);
    mUnk_ac[1] = OS_Alloc((s16) mUnk_c4[1] * (s16) 0xc);
    TextManager::Get().Init(mUnk_ac[1], 0, 0, 0x40, 2, 0x21);
    mUnk_84    = func_02023504("/font/mes_Result.mes", 0, 0, 0, 0, 0);
    mUnk_b8[2] = TextManager::Get().BufferSize(0x10, 2);
    mUnk_a0[2] = OS_Alloc(mUnk_b8[2]);
    MI_CpuClear16(mUnk_a0[2], mUnk_b8[2]);
    mUnk_c4[2] = TextManager::Get().GlyphCount(4);
    mUnk_ac[2] = OS_Alloc((s16) mUnk_c4[2] * (s16) 0xc);
    TextManager::Get().Init(mUnk_ac[2], 0, 0, 0x10, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    MI_CpuClearFast((void *) 0x6400000, 0x3800);
    func_0205a774();
}

void BattleCommandList::func_02059c50(u8 index) {
    mUnk_156 = index;
    func_0205a774();
    u16 width;
    s16 numY;
    s16 numX;
    u8 cmd = mUnk_14e;
    if (cmd >= 0x2b) {
        if (mUnk_156 == 0) {
            TextManager &text = TextManager::Get();
            s16 y;
            u16 x;
            x = 2 - (mUnk_156 << 7);
            y = 0x10;
            u8 sel            = mUnk_150;
            MessageFile *file;
            void *buffer;
            buffer = mUnk_a0[0];
            file   = (MessageFile *) mUnk_78;
            MI_CpuClear16(data_020c1a4c, 0x20);
            text.mUnk_58      = 1;
            text.Print(file->GetText(sel), file->GetLength(sel), buffer, 0x10, 0x12, &y, (s16 *) &x, 0, 0, 2, 0x21,
                       0xe, 0, 1, 0, 1, 1);
            u16 w = x;
            for (u8 i = 1; i < 0x10; i++) {
                if (text.mPalette[i] == 0) {
                    text.mPalette[i] = text.mUnk_36[i];
                }
            }
            mUnk_157 = (w - 0xe) / 0x80;
        }
        {
            TextManager &text = TextManager::Get();
            s16 y;
            u16 x;
            x = 2 - (mUnk_156 << 7);
            y = 0x10;
            u8 sel            = mUnk_150;
            MessageFile *file = (MessageFile *) mUnk_78;
            text.Print(file->GetText(sel), file->GetLength(sel), mUnk_a0[0], 0x10, 0x12, &y, (s16 *) &x,
                                     0, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
        }
        mUnk_154 = 0;
        TextManager::Get().mUnk_5b = 1;
        {
            TextManager &text = TextManager::Get();
            s16 y;
            s16 x;
            x = 0;
            y = 0;
            MessageFile *file = (MessageFile *) mUnk_78;
            text.Print(file->GetText((u16) (mUnk_150 + 6)), file->GetLength((u16) (mUnk_150 + 6)), mUnk_a0[1], 0x40, 2, &y, &x,
                                     &mUnk_154, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
        }
        TextManager::Get().mUnk_5b = 0;
    } else {
        if (mUnk_156 == 0) {
            TextManager &text = TextManager::Get();
            s16 y;
            u16 x;
            x = 2 - (mUnk_156 << 7);
            y = 0x10;
            u8 row = ((const u8(*)[6]) data_0209e68c)[cmd][mUnk_150];
            MessageFile *file;
            void *buffer;
            buffer = mUnk_a0[0];
            file   = (MessageFile *) mUnk_7c;
            MI_CpuClear16(data_020c1a4c, 0x20);
            text.mUnk_58      = 1;
            text.Print(file->GetText(row), file->GetLength(row), buffer, 0x10, 0x12, &y, (s16 *) &x, 0, 0, 2, 0x21,
                       0xe, 0, 1, 0, 1, 1);
            u16 w = x;
            for (u8 i = 1; i < 0x10; i++) {
                if (text.mPalette[i] == 0) {
                    text.mPalette[i] = text.mUnk_36[i];
                }
            }
            mUnk_157 = (w - 0xe) / 0x80;
        }
        {
            TextManager &text = TextManager::Get();
            s16 y;
            u16 x;
            x = 2 - (mUnk_156 << 7);
            y = 0x10;
            u8 row = ((const u8(*)[6]) data_0209e68c)[cmd][mUnk_150];
            MessageFile *file = (MessageFile *) mUnk_7c;
            text.Print(file->GetText(row), file->GetLength(row), mUnk_a0[0], 0x10, 0x12, &y, (s16 *) &x,
                                     0, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
        }
        if (mUnk_161 != 0) {
            if ((cmd == 5) | (cmd == 6)) {
                cmd = cmd + 0x24;
            }
        }
        mUnk_154 = 0;
        TextManager::Get().mUnk_5b = 1;
        {
            TextManager &text = TextManager::Get();
            s16 y;
            s16 x;
            x = 0;
            y = 0;
            MessageFile *file = (MessageFile *) mUnk_80;
            text.Print(file->GetText(cmd), file->GetLength(cmd), mUnk_a0[1], 0x40, 2, &y, &x,
                       &mUnk_154, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
        }
        width = 0;
        {
            TextManager &text = TextManager::Get();
            s16 y;
            s16 x;
            x = 0;
            y = 0;
            MessageFile *file = (MessageFile *) mUnk_84;
            text.Print(file->GetText(5), file->GetLength(5), mUnk_a0[2], 0x10, 2, &y, &x, &width, 0, 2,
                       0x21, 0xe, 0, 0, 0, 1, 1);
        }
        if (mUnk_164 >= 9) {
            TextManager &text = TextManager::Get();
            s16 y;
            s16 x;
            x = 0;
            y = width + 6;
            MessageFile *file = (MessageFile *) mUnk_84;
            text.Print(file->GetText(6), file->GetLength(6), mUnk_a0[2], 0x10, 2, &y, &x, 0, 0, 2, 0x21,
                       0xe, 0, 0, 0, 1, 1);
        } else {
            numY = width + 10;
            numX = 0;
            TextManager::Get().PrintNumber(mUnk_164 + 1, 1, 0, mUnk_a0[2], 0x10, 2, &numY, &numX, 0, 2, 0x21, 0xe, 0, 1, 1);
        }
        TextManager::Get().mUnk_5b = 0;
    }
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0, mUnk_a0[0], 0x2000, 0);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x2000, mUnk_a0[1], mUnk_b8[1], 0);
    func_02025c1c(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x3000, mUnk_a0[2], mUnk_b8[2], 0);
}

void BattleCommandList::func_0205a6a8() {
    if (mUnk_15f != 0) {
        return;
    }
    SpriteManager::Get().mScreens[0].func_02027778(mUnk_ac[1], mUnk_c4[1], (s16) (0x80 - ((u32) mUnk_154 >> 1)), 8, 0,
                                                   0x40, 0xf, 0);
}

void BattleCommandList::func_0205a774() {
    for (u8 i = 0; i < 3; i++) {
        MI_CpuClear16(mUnk_a0[i], mUnk_b8[i]);
    }
}

void BattleCommandList::func_0205a7ac() {
    if (mUnk_14e < 0x2b) {
        mUnk_90->func_0203d988();
        mUnk_90->func_0203dab0();
        return;
    }
    mCells58[7]->func_0202a3ec(0, mUnk_150, 0x22, 0x4c, 0);
}

extern u8 data_020d43d7[];

void BattleCommandList::func_0205a804() {
    bool late = mUnk_14e >= 0x2b;
    if (mUnk_15f != 0) {
        mCells3c[2]->Draw(0, 0x80, 0x60, 1);
        return;
    }
    if (!late) {
        s16 x;
        u32 count;
        u32 max;
        count = 0;
        max = 0;
        u8 list[6];
        for (u8 i = 0; i < 6; i++) {
            u32 v = mUnk_142[i];
            if (v != 0) {
                list[count] = i;
                count = (u8) (count + 1);
                if (max < v) {
                    max = v;
                }
            }
        }
        if (mUnk_165 != 0) {
            mUnk_165--;
        } else if (mUnk_166 != 0) {
            mUnk_166--;
            mUnk_142[list[0]]++;
            if (mUnk_166 % 4 == 0) {
                SoundManager::Get().func_020246f8(8);
            }
        }
        if (count != 0) {
            if (max != 0) {
                x = 0x38;
                if ((s32) max < 10) {
                    x -= 7;
                }
                mCells58[1]->Draw(0, (s16) (x + 0x5a), 0xae, 0);
                do {
                    u8 digit = (s32) max % 10;
                    x -= 10;
                    mCells58[0]->func_0202a3ec(0, digit, x + 0x58, 0xae, 0);
                    max /= 10;
                } while (max != 0);
            }
            u8 i = 0;
            do {
                mCells3c[0]->func_0202a3ec(0, list[i], 0x58 - (count - i - 1) * 0x18, 0xa8, 0);
                i++;
            } while (i < count);
        }
        s16 x3;
        u32 val = mUnk_166;
        if (val != 0) {
            x3 = 0xe6;
            for (u8 i = 0; i < 3; i++) {
                u8 digit = (s32) val % 10;
                x3 -= 10;
                mCells58[0]->func_0202a3ec(0, digit, x3, 0xae, 0);
                val /= 10;
                if (val == 0) {
                    break;
                }
            }
            mCells58[2]->Draw(0, 0xc0, 0xae, 0);
        }
    }
    if (mUnk_152 > -1) {
        u8 cur = data_020d433c.mBattlePlayers->mUnk_2a9;
        BOOL has;
        if (mUnk_152 == cur) {
            mCells58[4]->func_0202a3ec(0, mUnk_152, 0x24, 0x18, 0);
        } else {
            has = 0;
            if (data_020d433c.mBattlePlayers->mPlayers[(u8) mUnk_152]->mUnk_c8 != NULL) {
                has = 1;
            }
            if (has == 1) {
                mCells58[5]->func_0202a3ec(0, mUnk_152, 0x15, 0x12, 0);
            }
            if ((data_020d433c.mUnk_98 == 1) & (mUnk_152 == data_020d43d7[cur])) {
                mCells58[6]->func_0202a3ec(0, mUnk_152, 0x24, 0x18, 0);
            }
        }
        mCells58[3]->func_0202a3ec(0, mUnk_152, 0, 0x16, 0);
    }
    mCells3c[3]->Draw(0, 0x80, 0x60, 1);
    if (!late && mUnk_15b == 0) {
        for (u8 i = 0; i < 5; i++) {
            u8 target = mUnk_0c[i].mTarget;
            u8 cur    = mUnk_0c[i].mCur;
            if (cur != target) {
                if (cur < target) {
                    mUnk_0c[i].mCur = cur + 1;
                } else {
                    mUnk_0c[i].mCur = cur - 1;
                }
            }
            mCells3c[5]->func_0202a3ec(0, mUnk_0c[i].mCur + 1, 0x84, i * 0x1a + 0x2a, 1);
            mCells3c[6]->func_0202a3ec(0, mUnk_0c[i].mCur + 1, 0xe1, i * 0x1a + 0x1e, 1);
        }
        mCells3c[2]->Draw(0, 0x80, 0x60, 1);
    } else {
        for (u8 i = 0; i < 0x20; i++) {
            SpriteManager::Get().Screen(0).func_02027778((void *) data_0209e588, 1, (s16) ((i % 4) * 0x20 + 0x73),
                                                         (s16) ((i / 4) * 0x10 + 0x20), 3, (u16) (i << 1), 0xf, 0);
        }
    }
    if (late) {
        return;
    }
    SpriteManager::Get().Screen(0).func_02027778(mUnk_ac[2], mUnk_c4[2], 0x2a, 0x8e, 0, 0x60, 0xf, 0);
    if (mUnk_162 != 0) {
        mCells3c[4]->Draw(0, 0x80, 0x60, 1);
    }
}

void BattleCommandList::func_0205aea4() {
    for (u8 i = 0; i < 12; i++) {
        SpriteManager::Get().Screen(0).func_02027778((void *) data_0209e57c, 1, (s16) ((i % 4) << 6), (s16) ((i / 4) << 6), 3,
                                                     (u16) (0x280 + i * 0x20), 0, 0);
    }
}

void BattleCommandList::func_0205af8c() {
    for (u8 i = 0; i < 6; i++) {
        mUnk_142[i] = 0;
    }
}

void func_0205afb4(BattleCommandList *list, u32 a, u32 b, BattleCellPair *pair) {
    list->func_0205af8c();
    list->func_0205b034(a, b);
    if (list->mUnk_161 != 0) {
        switch (list->mUnk_14f) {
            case 5:
                list->func_0205b034(3, b);
                break;
            case 6:
                list->func_0205b034(0, b);
                break;
        }
    }
    list->func_0205b0dc(pair);
}

void BattleCommandList::func_0205b034(u32 index, u32 value) {
    if (index < 6) {
        mUnk_142[index] = value;
    }
}

void BattleCommandList::func_0205b048(u32 value) {
    if (mUnk_166 != 0) {
        return;
    }
    SoundManager::Get().func_020246f8(8);
    mUnk_165 = 0x3c;
    mUnk_166 = value;
}

BOOL BattleCommandList::func_0205b0c4() {
    return mUnk_166 != 0 ? 1 : 0;
}

void BattleCommandList::func_0205b0dc(const BattleCellPair *pair) {
    if (pair == NULL) {
        for (u8 i = 0; i < 5; i++) {
            mUnk_16[i] = data_0209e5bc[mUnk_14f][i];
        }
        mUnk_164 = 0;
    } else {
        mUnk_16[0] = pair->mUnk_02_0;
        mUnk_16[2] = pair->mUnk_02_3;
        mUnk_16[3] = pair->mUnk_02_6;
        mUnk_16[4] = pair->mUnk_02_9;
        mUnk_16[1] = pair->mUnk_02_12;
        mUnk_164   = pair->mUnk_00_0;
    }
    for (u8 i = 0; i < 5; i++) {
        mUnk_0c[i].mTarget = mUnk_16[i];
    }
    if (mUnk_7c != NULL) {
        func_02059c50(mUnk_156);
    }
}

   // 0x20 bytes cleared before the first text of a page

void func_0205b1c4(u32 index, u32 unk2) {
    UnkStruct_02023248 *entry = func_02023248(data_020d43f0, index, unk2);
    entry->mUnk_0  = data_0209e5bc[index][0];
    entry->mUnk_3  = data_0209e5bc[index][2];
    entry->mUnk_6  = data_0209e5bc[index][3];
    entry->mUnk_9  = data_0209e5bc[index][4];
    entry->mUnk_12 = data_0209e5bc[index][1];
}

