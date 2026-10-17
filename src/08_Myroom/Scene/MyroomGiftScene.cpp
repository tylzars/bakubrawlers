#include "UI/SelectWindow.hpp"
struct GiftPos {
    u8 x;
    u8 y;
};
struct GiftLayout {
    GiftPos pos[3][4];
    u8 order[12];
    u8 ids[12];
    s8 slots[12];
};

#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
const GiftPos sMyroomGiftCellAnimPos = {0x80, 0x60};

#include "Graphics/SpriteManager.hpp"
WindowCellRecord sMyroomGiftEmblemCells[10] = {
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 0, 0, 0, 1, 1, {0, 0, 0, 0}},
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 1, 0, 0, 1, 1, {1, 1, 1, 0}},
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 2, 0, 0, 1, 1, {2, 2, 2, 0}},
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 3, 0, 0, 1, 1, {3, 3, 3, 0}},
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 4, 0, 0, 1, 1, {4, 4, 4, 0}},
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 5, 0, 0, 1, 1, {5, 5, 5, 0}},
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 6, 0, 0, 1, 1, {6, 6, 6, 0}},
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 7, 0, 0, 1, 1, {7, 7, 7, 0}},
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 8, 0, 0, 1, 1, {8, 8, 8, 0}},
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 9, 0, 0, 1, 1, {9, 9, 9, 0}},
};

#include "Text/TextManager.hpp"
const u8 sMyroomGiftTextDimensions[3][2] = {{0x10, 0x12}, {0x40, 0x02}, {0x00, 0x00}};

const u32 sMyroomGiftTextUnitSizes[2] = {0x44, 0x21};

const u8 sMyroomGiftGridUsed[3][4] = {{0x01, 0x01, 0x01, 0x01}, {0x01, 0x01, 0x01, 0x01}, {0x01, 0x01, 0x00, 0x00}};

const u8 data_ov008_02228a70[3][4] = {{0, 0x20, 0, 0x80}, {0, 0, 0, 0}, {0, 0, 0, 0}};

const WindowCellRecord data_ov008_02228ab8[4] = {
    {"Game/Myroom/myroom_gift_b_obj_nce.bin", 10, 0, 0, 0, 1, {10, 10, 10, 0}},
    {"Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 16384, 13, 0, 1, {2, 3, 3, 0}},
    {"Game/2D/union_b/union_b_obj_page_nce.bin", 0, 28672, 14, 0, 1, {0, 1, 1, 0}},
    {"Game/2D/union_b/union_b_obj_page_nce.bin", 3, 28672, 14, 0, 1, {3, 4, 4, 0}},
};

const GiftLayout sMyroomGiftLayout = {
    {{{0x2d, 0x23}, {0x59, 0x23}, {0x85, 0x23}, {0xb1, 0x23}}, {{0x2d, 0x4f}, {0x59, 0x4f}, {0x85, 0x4f}, {0xb1, 0x4f}}, {{0x2d, 0x7b}, {0x59, 0x7b}, {0x00, 0x00}, {0x00, 0x00}}},
    {6, 1, 4, 5, 0, 2, 3, 7, 8, 9, 0, 0},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0},
    {4, 1, 5, 6, 2, 3, 0, 7, 8, 9, 0, 0},
};

const WindowItemRecord sMyroomGiftWindowItems[14] = {
    {0, 0x8, 46, 80, 32, 32, 0x0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8, 90, 36, 32, 32, 0x0, {-1, -1, -1, -1}, 0, 1},
    {2, 0x8, 90, 80, 32, 32, 0x0, {-1, -1, -1, -1}, 0, 1},
    {3, 0x8, 134, 80, 32, 32, 0x0, {-1, -1, -1, -1}, 0, 1},
    {4, 0x8, 134, 36, 32, 32, 0x0, {-1, -1, -1, -1}, 0, 1},
    {5, 0x8, 178, 36, 32, 32, 0x0, {-1, -1, -1, -1}, 0, 1},
    {6, 0x8, 46, 36, 32, 32, 0x0, {-1, -1, -1, -1}, 0, 1},
    {7, 0x8, 178, 80, 32, 32, 0x0, {-1, -1, -1, -1}, 0, 1},
    {8, 0x8, 46, 124, 32, 32, 0x0, {-1, -1, -1, -1}, 0, 1},
    {9, 0x8, 90, 124, 32, 32, 0x0, {-1, -1, -1, -1}, 0, 1},
    {10, 0x8000, 46, 36, 0, 0, 0x0, {-1, -1, -1, -1}, 0, 0},
    {11, 0x2, 225, 170, 30, 20, 0x0, {-1, -1, -1, -1}, 0, 1},
    {12, 0x208, 192, 20, 32, 16, 0x0, {-1, -1, -1, -1}, 0, 1},
    {13, 0x408, 224, 20, 32, 16, 0x0, {-1, -1, -1, -1}, 0, 1},
};

#include "Scene/MyroomGiftScene.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "System/FileSystem.hpp"
#include "UI/MessageWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

MyroomGiftScene::MyroomGiftScene() :
    SceneBase(1000, 0) {
    mMessages = NULL;
    mMessageWindow  = NULL;
    mWindow   = NULL;
    mResult   = 0;
    mTextWidth   = 0;
    mTextX   = 0x100;
    mTextEndY   = 0;
    mEmblem   = -1;
    mCol   = 0;
    mRow   = 0;
    mState    = 0;
    mTimer    = 0;
    mTextPage   = 0;
    mCellAnim = NULL;
    for (u8 i = 0; i < 2; i++) {
        mTextBuffers[i]    = NULL;
        mGlyphBuffers[i]   = NULL;
        mTextBufferSize[i] = 0;
        mGlyphCount[i]     = 0;
    }
    InitEmblems();
}

MyroomGiftScene::~MyroomGiftScene() {
    delete mWindow;
    delete mMessageWindow;
    CellAnimation *cell = mCellAnim;
    if (cell != NULL) {
        delete cell;
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    ClearTextBuffers(0);
    ClearTextBuffers(1);
    for (u8 i = 0; i < 2; i++) {
        OS_Free(mTextBuffers[i]);
        if (mGlyphBuffers[i] != NULL) {
            OS_Free(mGlyphBuffers[i]);
        }
    }
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    SpriteManager::Get().ResetSlots();
}

void MyroomGiftScene::Update() {
    BOOL draw = TRUE;
    switch (mState) {
        case 0:
            draw = FALSE;
            InitDisplay();
            LoadGraphics();
            mTimer = 10;
            mState++;
            break;
        case 1:
            draw = FALSE;
            mTimer--;
            if (mTimer > 0) {
                break;
            }
            SpriteManager::Get().StartFade(2);
            mState++;
            break;
        case 2:
            func_ov008_0221f194();
            break;
        case 3:
            if (!SpriteManager::Get().IsFading()) {
                mState = 4;
            }
            break;
        case 4:
            draw = FALSE;
            RequestScene(1);
            mFinished = TRUE;
            break;
    }
    if (draw) {
        mResult = mWindow->Update(FALSE);
        Draw();
    }
}

void MyroomGiftScene::InitDisplay() {
    MI_CpuClearFast((void *) 0x6000000, 0x80000);
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(0, 1);
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(3);
    G2S_SetBG0Priority(3);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x11);
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

void MyroomGiftScene::LoadGraphics() {
    ChangeDir("/Game/Card/Graphic/");
    LoadAndQueueUpload(0, 7, 0, "cardspec_u_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 11, 0, "cardspec_u_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(0, 1, 0, "cardspec_u_bg_00_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/Myroom/");
    LoadAndQueueUpload(1, 7, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    SpriteScreen &main = SpriteManager::Get().Screen(0);
    main.SetObjVramMode(3);
    LoadEmblem(6);
    ChangeDir("/Game/Myroom/");
    main.LoadCharacters("myroom_gift_u_obj_00.nce_ncg1D.bin", 0x3000, 0);
    main.LoadPalette("myroom_gift_u_obj_00_ncl.bin", 0, 0);
    mCellAnim            = new CellAnimation("myroom_gift_u_obj_00_nce.bin", 0, 1, 1);
    mCellAnim->mCharBase = main.GetCharBase(0x3000);
    mCellAnim->mPaletteBase   = 0;
    mCellAnim->SetAnimation(0);
    ChangeDir("/");
    mWindow = NULL;
    mWindow = new SelectWindow(0xe, sMyroomGiftWindowItems, -1);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.SetObjVramMode(3);
    ChangeDir("/");
    sub.LoadCharacters("Game/Myroom/myroom_gift_b_obj.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("Game/Myroom/myroom_gift_b_obj_ncl.bin", 0, 0);
    sub.LoadCharacters("Game/2D/union_b/union_b_obj_cockpit.nce_ncg1D.bin", 0x4000, 0);
    sub.LoadPalette("Game/2D/union_b/union_b_obj_cockpit_ncl.bin", 0x1a0, 0);
    sub.LoadCharacters("Game/2D/union_b/union_b_obj_page.nce_ncg1D.bin", 0x7000, 0);
    sub.LoadPalette("Game/2D/union_b/union_b_obj_page_ncl.bin", 0x1c0, 0);
    for (u8 i = 0; i < 10; i++) {
        if (mHasEmblem[i] != 0) {
            const u8 *ids = sMyroomGiftLayout.ids;
            ids += i;
            u8 value = *ids;
            *(u8 *) &sMyroomGiftEmblemCells[i].mAnimIndex = value;
            sMyroomGiftEmblemCells[i].mAnimIndices[0] = value;
            sMyroomGiftEmblemCells[i].mAnimIndices[1] = value;
            sMyroomGiftEmblemCells[i].mAnimIndices[2] = value;
        } else {
            *(u8 *) &sMyroomGiftEmblemCells[i].mAnimIndex = 0xb;
            sMyroomGiftEmblemCells[i].mAnimIndices[0] = 0xb;
            sMyroomGiftEmblemCells[i].mAnimIndices[1] = 0xb;
            sMyroomGiftEmblemCells[i].mAnimIndices[2] = 0xb;
        }
    }
    mWindow->SetCells(10, sMyroomGiftEmblemCells);
    mWindow->SetCells(4, data_ov008_02228ab8);
    ChangeDir("/font/");
    if (mMessages == NULL) {
        mMessages = (MessageFile *) LoadFile("mes_myroom.mes", 0, 0, 0, 0, 0);
    }
    for (u8 i = 0; i < 2; i++) {
        mTextBufferSize[i] = (u16) TextManager::Get().BufferSize((s16) sMyroomGiftTextDimensions[i][0], (s16) sMyroomGiftTextDimensions[i][1]);
        mGlyphCount[i]     = TextManager::Get().GlyphCount(sMyroomGiftTextDimensions[i][0], sMyroomGiftTextDimensions[i][1], sMyroomGiftTextUnitSizes[i]);
        mTextBuffers[i]    = OS_Alloc(mTextBufferSize[i]);
        ClearTextBuffers(i);
        mGlyphBuffers[i] = OS_Alloc(mGlyphCount[i] * 0xc);
        TextManager::Get().Init(mGlyphBuffers[i], 0, 0, sMyroomGiftTextDimensions[i][0], sMyroomGiftTextDimensions[i][1], sMyroomGiftTextUnitSizes[i]);
    }
    TextManager::Get().SetColor(0, 0, 0xf);
    mMessageWindow = new MessageWindow("mes_myroom", 1, 0x6000);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(0x22, 0, 1);
    PrintEmblemText();
    ChangeDir("/");
}

void MyroomGiftScene::InitEmblems() {
    MI_CpuFill8(mHasEmblem, 0, sizeof(mHasEmblem));
    for (u8 i = 0; i < 10; i++) {
        if (TestFlagBit(data_020d49a6, i)) {
            mHasEmblem[i] = 1;
        }
    }
}

void MyroomGiftScene::LoadEmblem(u8 index) {
    char name[64];
    if (mEmblem == index) {
        return;
    }
    mEmblem = index;
    if (mHasEmblem[mEmblem] == 0) {
        return;
    }
    ChangeDir("/Game/Myroom/");
    sprintf(name, "myroom_gift_u_obj_emblem_%02d.nce_ncg1D.bin", index + 1);
    LoadAndQueueUpload(0, 4, 0, name, 0, 0, 0);
    sprintf(name, "myroom_gift_u_obj_emblem_%02d_ncl.bin", index + 1);
    LoadAndQueueUpload(0, 0, 0, name, 0, 0, 0);
    ChangeDir("/");
    mTextPage = 0;
    mTextX = 0x100;
}

void MyroomGiftScene::Draw() {
    DrawMainScreen();
    DrawWindows();
}

void MyroomGiftScene::DrawMainScreen() {
    if (mCellAnim != NULL) {
        mCellAnim->Update();
        mCellAnim->Draw(0, sMyroomGiftCellAnimPos.x, sMyroomGiftCellAnimPos.y, 0);
    }
    if (mHasEmblem[mEmblem] == 0) {
        return;
    }
    u8 j;
    u8 i = 0;
    do {
        int y = i * 32 + 0x24;
        j     = 0;
        do {
            SpriteManager::Get().Screen(0).AddOam((void *) data_ov008_02228a70, 1, j * 32 + 0xe, y, 0, (u16) (i * 0x18 + j * 8), 0, 0);
            j++;
        } while (j < 3);
        i++;
    } while (i < 4);
    SpriteManager::Get().Screen(0).AddOam(mGlyphBuffers[0], 4, 0x70, 0x1e, 0, 0xa0, 0xf, 0);
    s16 x;
    u8 first;
    u8 last;
    x = 0x80 - mTextWidth / 2;
    if ((s32) mTextWidth > 0xf0) {
        x       = mTextX;
        mTextX = x - 1;
        if (mTextX <= -mTextWidth) {
            mTextX = 0x100;
        }
    }
    first = 0;
    if (x < 0) {
        first = (-x) / 32;
    }
    last = first + 9 - x / 32;
    if (last > 0x10) {
        last = 0x10;
    }
    j = first;
    do {
        SpriteManager::Get().Screen(0).AddOam((u8 *) mGlyphBuffers[1] + j * 0xc, 1, x, 8, 0, 0x100, 0xf, 0);
        j++;
    } while (j < last);
}

void MyroomGiftScene::DrawWindows() {
    mMessageWindow->Update();
    mWindow->Draw();
}

void MyroomGiftScene::ClearTextBuffers(u8 index) {
    MI_CpuClear16(mTextBuffers[index], mTextBufferSize[index]);
}

void MyroomGiftScene::PrintEmblemText() {
    ClearTextBuffers(0);
    TextManager &text = TextManager::Get();
    s16 width;
    s16 height;
    height            = 2 - mTextPage * 0x7e;
    width             = 0x10;
    u16 index         = mEmblem;
    MessageFile *file = mMessages;
    text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[0], 0x10, 0x12, &width, &height, 0, 0, 2, 0x44,
               0xe, 0, 0, 0, 1, 1);
    if (&mTextEndY != NULL) {
        mTextEndY = height;
    }
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x5000, mTextBuffers[0], mTextBufferSize[0], 0);
    ClearTextBuffers(1);
    TextManager::Get().mUse4bppFont = 1;
    TextManager &text2         = TextManager::Get();
    s16 height2;
    s16 width2;
    width2     = 0;
    height2    = 0;
    u16 index2 = mEmblem + 11;
    file       = mMessages;
    text2.Print(file->GetText(index2), file->GetLength(index2), mTextBuffers[1], 0x40, 2, &height2, &width2, &mTextWidth, 0, 2,
                0x21, 0xe, 0, 0, 0, 1, 1);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(0).mBgIndex, 4, 0x8000, mTextBuffers[1], mTextBufferSize[1], 0);
    if (mHasEmblem[mEmblem] == 0) {
        mTextEndY = 0;
    }
}

s32 MyroomGiftScene::UpdateInput() {
    s32 result = -1;
    s8 previous = mTextPage;
    if (mResult == 3) {
        s8 id = mWindow->GetSelectedId();
        if (id >= 0 && id <= 9) {
            result  = sMyroomGiftLayout.slots[id];
            mCol = result % 4;
            mRow = result / 4;
            u8 emblem = sMyroomGiftLayout.order[result];
            if (emblem != mEmblem) {
                SoundManager::Get().StartSeqArc(0);
            }
        } else {
            switch (id) {
                case 0xc:
                    if (previous > 0) {
                        mTextPage--;
                        SoundManager::Get().StartSeqArc(0);
                    }
                    break;
                case 0xd:
                    if (mTextEndY > 0x80) {
                        mTextPage++;
                        SoundManager::Get().StartSeqArc(0);
                    }
                    break;
            }
        }
    }
    if (previous != mTextPage) {
        PrintEmblemText();
    } else if (result == -1) {
        result = UpdateKeySelection();
    }
    return result;
}

s8 MyroomGiftScene::UpdateKeySelection() {
    s8 result  = -1;
    BOOL moved = FALSE;
    if (InputManager::Get().IsTrigger(KEY_UP)) {
        moved = TRUE;
        if (mRow != 0) {
            mRow--;
        } else {
            mRow = 2;
            if (sMyroomGiftGridUsed[mRow][mCol] == 0) {
                mRow--;
            }
        }
    } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
        moved = TRUE;
        mRow++;
        if (mRow >= 3) {
            mRow = 0;
        } else if (sMyroomGiftGridUsed[mRow][mCol] == 0) {
            mRow = 0;
        }
    }
    if (InputManager::Get().IsTrigger(KEY_LEFT)) {
        moved = TRUE;
        if (mCol != 0) {
            mCol--;
        } else {
            mCol = 3;
            if (sMyroomGiftGridUsed[mRow][mCol] == 0) {
                do {
                    mCol--;
                } while (mCol != 0 && sMyroomGiftGridUsed[mRow][mCol] == 0);
            }
        }
    } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
        moved = TRUE;
        mCol++;
        if (mCol >= 4) {
            mCol = 0;
        } else if (sMyroomGiftGridUsed[mRow][mCol] == 0) {
            mCol = 0;
        }
    }
    if (moved) {
        result = mCol + mRow * 4;
        SoundManager::Get().StartSeqArc(0);
    }
    return result;
}

void MyroomGiftScene::func_ov008_0221f194() {
    if (mResult == 2) {
        SpriteManager::Get().StartFade(3);
        SoundManager::Get().StartSeqArc(1);
        mState = 3;
    } else {
        s32 selection = UpdateInput();
        if (selection >= 0) {
            LoadEmblem(sMyroomGiftLayout.order[selection]);
            PrintEmblemText();
            if (mWindow->mState != 1) {
                mWindow->mState = 4;
                mWindow->Reset(-1);
            }
        }
    }
    WindowItem *items = mWindow->mCursor->mItems;
    if (mTextPage != 0) {
        if (mWindow->mCells[12]->mAnimIndex == 2) {
            mWindow->mCells[12]->SetAnimation(0);
            items[12].mFlags = 0x208;
        }
    } else {
        if (mWindow->mCells[12]->mAnimIndex != 2) {
            mWindow->mCells[12]->SetAnimation(2);
            items[12].mFlags = 0x8000;
        }
    }
    if (mTextEndY > 0x80) {
        if (mWindow->mCells[13]->mAnimIndex == 5) {
            mWindow->mCells[13]->SetAnimation(3);
            items[13].mFlags = 0x408;
        }
    } else {
        if (mWindow->mCells[13]->mAnimIndex != 5) {
            mWindow->mCells[13]->SetAnimation(5);
            items[13].mFlags = 0x8000;
        }
    }
    Vec2s pos = {sMyroomGiftLayout.pos[mRow][mCol].x, sMyroomGiftLayout.pos[mRow][mCol].y};
    mWindow->mCellPos[10] = pos;
}
