#include "UI/SelectWindow.hpp"

// item table of the page window (positions are patched by CreateCommandPageWindow)
inline WindowItemRecord *PageItems() {
    static WindowItemRecord sItems[4] = {
        { 0, 0x208, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 1 },
        { 1, 0x408, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 1 },
        { 2, 0x8000, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 0 },
        { 3, 0x8000, 0, 0, 0x20, 0x10, {0}, {-1, -1, -1, -1}, 0, 0 },
    };
    return sItems;
}

// tables passed to SpriteScreen::AddOam
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

// paths loaded by LoadGraphics and LoadLargeCard
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

u16 GetCardNumber(u32 id);
u8 GetCardInfo(u32 id, u32 kind);
extern u16 data_020c1a4c[0x10]; // palette scratch (cleared before each text)

BattleCommandPage::BattleCommandPage(u16 a, s32 b) :
    SceneBase(0x3e8, 0) {
    mWindow = NULL;
    mCardMessages = NULL;
    mCardTextMessages = NULL;
    mCardNameMessages = NULL;
    mState = 0;
    mUnk_f2 = 0;
    mPlayer = b;
    mTextPage = 0;
    mLastTextPage = 0;
    mWindowVisible = 0;
    mFourItems = 0;
    mNameWidth = 0;
    mNameX = 0;
    mVisible = 0;
    mCloseFadeKind = 0;
    mUnk_106 = 0;
    mUnk_107 = 0;
    mUnk_108 = 0;
    MI_CpuClear32(mTextBuffers, 0xc);
    MI_CpuClear32(mGlyphBuffers, 0xc);
    MI_CpuClear32(mTextBufferSizes, 0xc);
    MI_CpuClear32(mGlyphCounts, 0xc);
    MI_CpuClear32(mFiles, 0x30);
    MI_CpuClear32(mFileSizes, 0x30);
    MI_CpuClear32(mNceData, 0xc);
    MI_CpuClear32(mCellFiles, 0xc);
    MI_CpuClear32(mCellAnims, 0x2c);
    mCardId = func_02055644(a);
    mNextCardId = mCardId;
    mDrawnCardId = mCardId;
}

BattleCommandPage::~BattleCommandPage() {
    StopAsyncLoad();
    if (mCardMessages != NULL) {
        OS_Free(mCardMessages);
    }
    if (mCardTextMessages != NULL) {
        OS_Free(mCardTextMessages);
    }
    if (mCardNameMessages != NULL) {
        OS_Free(mCardNameMessages);
    }
    for (u8 i = 0; i < 3; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
        }
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
        }
    }
    for (u8 i = 0; i < 3; i++) {
        if (mNceData[i] != NULL) {
            OS_Free(mNceData[i]);
        }
        if (mCellFiles[i] != NULL) {
            OS_Free(mCellFiles[i]);
        }
    }
    for (u8 i = 0; i < 11; i++) {
        CellAnimation *cell = mCellAnims[i];
        if (cell != NULL) {
            delete cell;
        }
    }
    if (mWindow != NULL) {
        delete mWindow;
    }
}

void BattleCommandPage::Update() {
    switch (mState) {
        case 0:
            InitDisplay();
            LoadText();
            ResetLoadStep();
            mState++;
            break;
        case 1:
            if (LoadGraphics()) {
                mVisible = 1;
                mState++;
            }
            break;
        case 2:
            ResetLoadStep();
            mState++;
            break;
        case 3:
            if (LoadLargeCard()) {
                PrintCardText(0);
                if ((s32) mNameWidth > 0xf0) {
                    mNameX = 0x100;
                }
                UpdatePageItems();
                if (mUnk_107 == 0) {
                    mUnk_106 = 0;
                }
                mDrawnCardId  = mCardId;
                mUnk_109 = mUnk_10a;
                if (mCloseFadeKind == 0) {
                    SpriteManager::Get().sFade[0] = 2;
                }
                mState++;
            }
            break;
        case 4:
            if (mUnk_107 != 0) {
                mUnk_106 = 1;
            }
            if (mNextCardId != mCardId) {
                mCardId  = mNextCardId;
                mUnk_107 = 0;
                mTextPage  = 0;
                mState  = 2;
            }
            if (mCloseFadeKind != 0) {
                mState = 5;
            }
            break;
        case 5:
            switch (mCloseFadeKind) {
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
            mState++;
            break;
        case 6:
            if (SpriteManager::Get().sFade[0] <= 1) {
                mVisible = 0;
                mState++;
            }
            break;
        case 7:
            RequestScene(1);
            mFinished = 1;
            break;
    }
    if (mVisible == 0) {
        return;
    }
    DrawCard();
    DrawText();
    func_02056f70();
    if (mWindowVisible == 0) {
        return;
    }
    if (SpriteManager::Get().sBrightness[1] != 0) {
        return;
    }
    if (mLastTextPage == 0) {
        return;
    }
    if (mState == 4) {
        UpdateWindow();
    }
    mWindow->Draw();
}

void CloseCommandPage(BattleCommandPage *page, u32 a) {
    page->mCloseFadeKind = a;
}

void func_0205562c(BattleCommandPage *page, u32 a) {
    u8 value         = a;
    page->mUnk_107 = value;
    if (page->mState < 2) {
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

void SetCommandPageCard(BattleCommandPage *page, u16 a, s32 b) {
    page->mPlayer = b;
    if (page->mState == 0) {
        u16 value      = page->func_02055644(a);
        page->mCardId  = value;
        page->mNextCardId  = value;
        page->mUnk_106 = 0;
        page->mUnk_107 = 0;
        return;
    }
    page->mNextCardId = page->func_02055644(a);
    if (page->mNextCardId == page->mCardId) {
        if (page->mUnk_106 != 0) {
            page->mUnk_106 = 0;
        }
    }
    page->mUnk_107 = 0;
}

void BattleCommandPage::CycleTextPage(u32 flag) {
    if (mLastTextPage == 0) {
        return;
    }
    u8 count = mLastTextPage + 1;
    if (flag == 0) {
        mTextPage = (mTextPage + count - 1) % count;
    } else {
        mTextPage = (mTextPage + 1) % count;
    }
    PrintCardText(mTextPage);
}

BOOL BattleCommandPage::IsSettingUp() {
    return mState < 4;
}

void func_02055738(BattleCommandPage *page, u32 a, u32 b, u32 c) {
    CreateCommandPageWindow(page, a, b, 0xc0, 0, 0xe0, 0, c);
}

void CreateCommandPageWindow(BattleCommandPage *page, u32 a, u32 b, s16 c, s16 d, s16 e, s16 f, u32 g) {
    if (page->mWindow != NULL) {
        delete page->mWindow;
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
    PageCells()[0].mCharOffset = a;
    PageCells()[0].mPaletteBase = b;
    PageCells()[1].mCharOffset = a;
    PageCells()[1].mPaletteBase = b;
    PageCells()[2].mCharOffset = a;
    PageCells()[2].mPaletteBase = b;
    PageCells()[3].mCharOffset = a;
    PageCells()[3].mPaletteBase = b;
    u32 count;
    page->mFourItems = g;
    if (page->mFourItems != 0) {
        count = 4;
    } else {
        count = 2;
    }
    page->mWindow = new SelectWindow(count, PageItems(), -1);
    page->mWindow->SetCells(count, PageCells());
    page->mWindow->mCursor->mSelection = -1;
    page->mWindowVisible                      = 1;
    page->UpdatePageItems();
}

void BattleCommandPage::SetWindowVisible(u32 enable) {
    if (enable != 0) {
        if (mWindow != NULL) {
            mWindowVisible = 1;
        }
    } else {
        mWindowVisible = 0;
    }
}

void BattleCommandPage::UpdateWindow() {
    if (mWindow->Update(FALSE)) {
        SoundManager::Get().StartSeqArc(0);
        CycleTextPage(*(u8 *) &mWindow->mCursor->mSelection);
    }
    UpdatePageItems();
}

void BattleCommandPage::UpdatePageItems() {
    if (mWindow == NULL) {
        return;
    }
    u8 selected = mTextPage;
    u8 count    = mLastTextPage;
    WindowItem *items = mWindow->mCursor->mItems;
    if (selected < count) {
        if (IsPageItemEnabled(1) == 0) {
            mWindow->mCellAnims[1]->SetAnimation(3);
            items[1].mFlags = 0x408;
            if (mFourItems != 0) {
                mWindow->mCellAnims[3]->SetAnimation(7);
            }
        }
    } else {
        if (IsPageItemEnabled(1) != 0) {
            mWindow->mCellAnims[1]->SetAnimation(5);
            items[1].mFlags = 0x8000;
            if (mFourItems != 0) {
                mWindow->mCellAnims[3]->SetAnimation(5);
            }
        }
    }
    if (mTextPage != 0) {
        if (IsPageItemEnabled(0) != 0) {
            return;
        }
        mWindow->mCellAnims[0]->SetAnimation(0);
        items[0].mFlags = 0x208;
        if (mFourItems == 0) {
            return;
        }
        mWindow->mCellAnims[2]->SetAnimation(6);
    } else {
        if (IsPageItemEnabled(0) == 0) {
            return;
        }
        mWindow->mCellAnims[0]->SetAnimation(2);
        items[0].mFlags = 0x8000;
        if (mFourItems == 0) {
            return;
        }
        mWindow->mCellAnims[2]->SetAnimation(2);
    }
}

BOOL BattleCommandPage::IsPageItemEnabled(u32 right) {
    if (right == 0) {
        if (mWindow->mCellAnims[0]->mAnimIndex == 2) {
            return FALSE;
        }
    } else {
        if (mWindow->mCellAnims[1]->mAnimIndex == 5) {
            return FALSE;
        }
    }
    return TRUE;
}

void BattleCommandPage::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(0, 1);
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

void BattleCommandPage::ResetLoadStep() {
    mLoadStep = 0;
}

BOOL BattleCommandPage::LoadGraphics() {
    if (mLoadStep != 0) {
        if (IsAsyncLoadActive() != 0) {
            return FALSE;
        }
        mFiles[mLoadStep - 1] = GetAsyncLoadBuffer();
    }
    if (mLoadStep < 12) {
        LoadFileAsync(PagePaths()[mLoadStep], 0, 0, 0, &mFileSizes[mLoadStep], 0);
        mLoadStep++;
        return FALSE;
    }
    QueueUpload(0, 1, 0, mFiles[9], mFileSizes[9], 1);
    QueueUpload(0, 0xb, 0, mFiles[10], mFileSizes[10], 1);
    QueueUpload(0, 7, 0, mFiles[11], mFileSizes[11], 1);
    SpriteScreen &screen = SpriteManager::Get().mScreens[0];
    screen.SetObjVramMode(3);
    QueueUpload(screen.mBgIndex, 5, 0, mFiles[0], mFileSizes[0], 1);
    QueueUpload(screen.mBgIndex, 4, 0x8000, mFiles[1], mFileSizes[1], 1);
    if (mNceData[0] != NULL) {
        OS_Free(mNceData[0]);
    }
    mNceData[0] = mFiles[2];
    if (mNceData[0] != NULL) {
        CellAnimFile *file = mCellFiles[0];
        if (file != NULL) {
            delete file;
        }
        mCellFiles[0] = NULL;
        mCellFiles[0] = new CellAnimFileData(mNceData[0]);
        if (mCellFiles[0] != NULL) {
            for (u8 i = 0; i < 6; i++) {
                CellAnimation *cell = mCellAnims[i];
                if (cell != NULL) {
                    delete cell;
                }
                mCellAnims[i]            = new CellAnimation(mCellFiles[0], 0);
                mCellAnims[i]->mCharBase = screen.GetCharBase(0x8000);
                mCellAnims[i]->mPaletteBase   = 0;
            }
            mCellAnims[0]->SetAnimation(0);
            mCellAnims[1]->SetAnimation(1);
            mCellAnims[2]->SetAnimation(3);
            mCellAnims[3]->SetAnimation(7);
            mCellAnims[4]->SetAnimation(8);
            mCellAnims[5]->SetAnimation(9);
        }
    }
    QueueUpload(screen.mBgIndex, 5, 0x140, mFiles[3], mFileSizes[3], 1);
    QueueUpload(screen.mBgIndex, 4, 0x14000, mFiles[4], mFileSizes[4], 1);
    if (mNceData[1] != NULL) {
        OS_Free(mNceData[1]);
    }
    mNceData[1] = mFiles[5];
    if (mNceData[1] != NULL) {
        CellAnimFile *file = mCellFiles[1];
        if (file != NULL) {
            delete file;
        }
        mCellFiles[1] = NULL;
        mCellFiles[1] = new CellAnimFileData(mNceData[1]);
        if (mCellFiles[1] != NULL) {
            CellAnimation *cell = mCellAnims[6];
            if (cell != NULL) {
                delete cell;
            }
            mCellAnims[6]            = new CellAnimation(mCellFiles[1], 0);
            mCellAnims[6]->mCharBase = screen.GetCharBase(0x14000);
            mCellAnims[6]->mPaletteBase   = 10;
        }
    }
    QueueUpload(screen.mBgIndex, 5, 0x100, mFiles[6], mFileSizes[6], 1);
    QueueUpload(screen.mBgIndex, 4, 0x12000, mFiles[7], mFileSizes[7], 1);
    if (mNceData[2] != NULL) {
        OS_Free(mNceData[2]);
    }
    mNceData[2] = mFiles[8];
    if (mNceData[2] != NULL) {
        CellAnimFile *file = mCellFiles[2];
        if (file != NULL) {
            delete file;
        }
        mCellFiles[2] = NULL;
        mCellFiles[2] = new CellAnimFileData(mNceData[2]);
        if (mCellFiles[2] != NULL) {
            for (u8 i = 7; i < 11; i++) {
                CellAnimation *cell = mCellAnims[i];
                if (cell != NULL) {
                    delete cell;
                }
                mCellAnims[i]            = new CellAnimation(mCellFiles[2], 0);
                mCellAnims[i]->mCharBase = screen.GetCharBase(0x12000);
                mCellAnims[i]->mPaletteBase   = 8;
            }
            mCellAnims[7]->SetAnimation(1);
            mCellAnims[8]->SetAnimation(3);
            mCellAnims[9]->SetAnimation(2);
            mCellAnims[10]->SetAnimation(4);
        }
    }
    return TRUE;
}

BOOL BattleCommandPage::LoadLargeCard() {
    char file[0x7f];
    char path[0x7f];
    char name[0x10];
    if (mLoadStep != 0) {
        if (IsAsyncLoadActive() != 0) {
            return FALSE;
        }
        mFiles[mLoadStep - 1] = GetAsyncLoadBuffer();
    }
    if (mLoadStep < 2) {
        if (mCardId < 0xd5) {
            sprintf(file, "/Game/Card/Graphic/Large/card_l_%03d", GetCardNumber(mCardId));
        } else {
            switch (mCardId - 0xd7) {
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
        if (mLoadStep == 0) {
            sprintf(path, "%s_ncl.bin", file);
        } else if (mLoadStep == 1) {
            sprintf(path, "%s.nce_ncg1D.bin", file);
        }
        LoadFileAsync(path, 0, 0, 0, &mFileSizes[mLoadStep], 0);
        mLoadStep++;
        return FALSE;
    }
    SpriteScreen &screen = SpriteManager::Get().mScreens[0];
    QueueUpload(screen.mBgIndex, 0, 0, mFiles[0], mFileSizes[0], 1);
    QueueUpload(0, 4, 0x5000, mFiles[1], mFileSizes[1], 1);
    return TRUE;
}

void BattleCommandPage::LoadText() {
    SpriteManager::Get().mScreens[0].SetObjVramMode(3);
    mCardTextMessages = LoadFile("/font/mes_CardText.mes", 0, 0, 0, 0, 0);
    mCardMessages = LoadFile("/font/mes_Card.mes", 0, 0, 0, 0, 0);
    mGlyphCounts[0] = TextManager::Get().GlyphCount(0x18);
    mGlyphBuffers[0] = OS_Alloc((s16) mGlyphCounts[0] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[0], 0, 0, 0x10, 0xc, 0x21);
    mTextBufferSizes[1] = TextManager::Get().BufferSize(0x10, 0x10);
    mTextBuffers[1] = OS_Alloc(mTextBufferSizes[1]);
    MI_CpuClear16(mTextBuffers[1], mTextBufferSizes[1]);
    mGlyphCounts[1] = TextManager::Get().GlyphCount(0x20);
    mGlyphBuffers[1] = OS_Alloc((s16) mGlyphCounts[1] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[1], 0, 0, 0x10, 0x10, 0x21);
    mCardNameMessages = LoadFile("/font/mes_CardName.mes", 0, 0, 0, 0, 0);
    mTextBufferSizes[2] = TextManager::Get().BufferSize(0x10, 8);
    mTextBuffers[2] = OS_Alloc(mTextBufferSizes[2]);
    MI_CpuClear16(mTextBuffers[2], mTextBufferSizes[2]);
    mGlyphCounts[2] = TextManager::Get().GlyphCount(0x10);
    mGlyphBuffers[2] = OS_Alloc((s16) mGlyphCounts[2] * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffers[2], 0, 0, 0x40, 2, 0x21);
    TextManager::Get().SetColor(0, 0, 0xf);
    MI_CpuClearFast((void *) 0x6400000, 0x5000);
    ClearText();
}

void BattleCommandPage::PrintCardText(u32 index) {
    mTextPage = index;
    ClearText();
    MessageFile **textFile;
    u16 id = mCardId;
    if (id == 0) {
        return;
    }
    MessageFile **nameFile;
    u16 id2;
    u16 height;
    int total;
    if (id < 0xd5) {
        id2      = id;
        textFile = (MessageFile **) &mCardTextMessages;
        nameFile = (MessageFile **) &mCardNameMessages;
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
        textFile = (MessageFile **) &mCardMessages;
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
    if (mTextPage == 0) {
        u16 w1;
        u16 h1;
        TextManager &text = TextManager::Get();
        h1 = rem - base * mTextPage;
        w1 = 0x10;
        void *buffer = mTextBuffers[1];
        MessageFile *file = *textFile;
        MI_CpuClear16(data_020c1a4c, 0x20);
        text.mLineStartCount = 1;
        text.Print(file->GetText(id), file->GetLength(id), buffer, 0x10, height, (s16 *) &w1, (s16 *) &h1, NULL, 0, 2, 0x21, 0xe, 0, 1, 0,
                   1, 1);
        u16 hh = h1;
        for (u8 i = 1; i < 0x10; i++) {
            if (text.mUnk_36[i + 1] == 0) {
                text.mUnk_36[i + 1] = text.mUnk_36[i];
            }
        }
        mLastTextPage = (hh - 0xe) / base;
    }
    {
        u16 w2;
        u16 h2;
        TextManager &text = TextManager::Get();
        h2 = rem - base * mTextPage;
        w2 = 0x10;
        MessageFile *file = *textFile;
        text.Print(file->GetText(id), file->GetLength(id), mTextBuffers[1], 0x10, height, (s16 *) &w2, (s16 *) &h2, NULL, 0, 2, 0x21, 0xe, 0, 0,
                   0, 1, 1);
    }
    TextManager::Get().mUse4bppFont = 1;
    {
        u16 w3;
        u16 h3;
        TextManager &text = TextManager::Get();
        h3 = 0;
        w3 = 0;
        MessageFile *file = *nameFile;
        text.Print(file->GetText(id2), file->GetLength(id2), mTextBuffers[2], 0x40, 2, (s16 *) &w3, (s16 *) &h3, &mNameWidth, 0, 2,
                                 0x21, 0xe, 0, 0, 0, 1, 1);
    }
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0, mTextBuffers[1], mTextBufferSizes[1], 0);
    QueueUpload(SpriteManager::Get().mScreens[0].mBgIndex, 4, 0x4000, mTextBuffers[2], mTextBufferSizes[2], 0);
}

void BattleCommandPage::DrawText() {
    if (mCardId == 0) {
        return;
    }
    if (mUnk_106 != 0) {
        return;
    }
    SpriteManager::Get().mScreens[0].AddOam(mGlyphBuffers[mUnk_108], mGlyphCounts[mUnk_108], 0x73, 0x20, 3, 0, 0xf, 0);
    if ((s32) mNameWidth > 0xf0) {
        mNameX--;
        if (mNameX < -(s32) mNameWidth) {
            mNameX = 0x100;
        }
    } else {
        mNameX = 0x80 - ((u32) mNameWidth >> 1);
    }
    u8 i = 0;
    do {
        s16 x = mNameX + (i << 5);
        if ((x < 0x120) & (x > -0x20)) {
            SpriteManager::Get().mScreens[0].AddOam((void *) data_0209e558[2], 1, x, 8, 1, (u16) (0x80 + i * 2), 0xf, 0);
        }
        i++;
    } while (i < (s32) ((u32) mNameWidth >> 5) + 1);
}

void BattleCommandPage::ClearText() {
    for (u8 i = 0; i < 3; i++) {
        MI_CpuClear16(mTextBuffers[i], mTextBufferSizes[i]);
    }
}

void BattleCommandPage::func_02056f70() {
    if (mUnk_106 != 0) {
        return;
    }
    SpriteManager::Get().mScreens[0].AddOam((void *) data_0209e558[1], 1, 0x54, 0x25, 3, 0xa0, 0, 0);
    SpriteManager::Get().mScreens[0].AddOam((void *) data_0209e558[1], 1, 0x54, 0x65, 3, 0xb0, 0, 0);
    SpriteManager::Get().mScreens[0].AddOam((void *) data_0209e558[0], 1, 0x14, 0x25, 3, 0xc0, 0, 0);
    SpriteManager::Get().mScreens[0].AddOam((void *) data_0209e558[0], 1, 0x14, 0x65, 3, 0xe0, 0, 0);
}

u8 GetCardAttributeValue(u32 id, u32 index);
extern u8 data_020d43d7[];

void BattleCommandPage::DrawCard() {
    u8 big;
    if (mUnk_109 != 0) {
        big = 1;
    } else if (mDrawnCardId < 0xd5) {
        big = GetCardInfo(mDrawnCardId, 0);
    } else if (mDrawnCardId < 0xda) {
        big = 0;
    } else {
        big = 1;
    }
    mCellAnims[0]->DrawFrame(0, big, 0x80, 0x60, 0);
    if (mUnk_106 != 0) {
        return;
    }
    if (mDrawnCardId >= 0xd5) {
        return;
    }
    if (mPlayer > -1) {
        u8 cur   = data_020d433c.mBattlePlayers->mUnk_2a9;
        BOOL has;
        if (mPlayer == cur) {
            mCellAnims[8]->DrawFrame(0, mPlayer, 0x24, 0x18, 0);
        } else {
            has = 0;
            if (data_020d433c.mBattlePlayers->mPlayers[(u8)mPlayer]->mCpu != NULL) {
                has = 1;
            }
            if (has == 1) {
                mCellAnims[9]->DrawFrame(0, mPlayer, 0x15, 0x12, 0);
            }
            if ((data_020d433c.mHasPartners == 1) & (mPlayer == data_020d43d7[cur])) {
                mCellAnims[10]->DrawFrame(0, mPlayer, 0x24, 0x18, 0);
            }
        }
        mCellAnims[7]->DrawFrame(0, mPlayer, 0, 0x16, 0);
    }
    if (mCardId == 0) {
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
        n = GetCardAttributeValue(mDrawnCardId, k);
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
                mCellAnims[2]->DrawFrame(0, (s32) v % 10, y + (3 - i) * 7, 0xb0, 0);
                mCellAnims[5]->DrawFrame(0, (s32) v % 10, left + (cnt - i) * 4 + half, ty, 0);
                v /= 10;
                i = (u8) (i + 1);
            } while (i < cnt);
        }
        u8 idx = 3;
        if (big) {
            idx = 4;
        }
        if (n != 0) {
            mCellAnims[1]->DrawFrame(0, k, 0x80, 0x60, 0);
            mCellAnims[idx]->DrawFrame(0, k, c, a, 0);
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
    if (mDrawnCardId == 0) {
        return;
    }
    mCellAnims[6]->DrawFrame(0, GetCardInfo(mDrawnCardId, 2), 0x80, 0x84, 0);
}
