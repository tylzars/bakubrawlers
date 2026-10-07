#include "Scene/StoreScene.hpp"
#include "Game/UnkStruct_02065c40.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Game/UnkStruct_020d4414.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameTask.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

// Other overlay-3 objects that are only ever constructed here.
class UnkStruct_02078e14 {
public:
    /* 000 */ PAD(0x000, 0x408);

    UnkStruct_02078e14(u32 unk1, u32 unk2);
};

class UnkStruct_02089f90 {
public:
    /* 00 */ PAD(0x00, 0x24);

    UnkStruct_02089f90(s32 unk1);
};

s32 func_02087468();
void func_02086d48();

static const u8 sIdleFrames[3]      = {3, 0, 6};
static const u8 sSelectEventIds[3]  = {1, 2, 3};
static const u8 sSelectedFrames[3]  = {4, 1, 7};
static const u8 sSelectScriptIds[3] = {2, 1, 3};
static const u8 sMessageIndices[3]  = {0x12, 0x13, 0x14};

static const WindowItemRecord sMenuItems[6] = {
    {0, 0x8000, 88, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {1, 0x8000, 186, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {2, 0x8000, 284, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, 1},
    {3, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
    {4, 0x8, 0, 72, 80, 48, 0, {-1, -1, -1, -1}, 0, 1},
    {5, 0x8, 176, 72, 80, 48, 0, {-1, -1, -1, -1}, 0, 1},
};

static const WindowCellRecord sMenuCells[6] = {
    {"/Game/Store/shop_b_obj_00_nce.bin", 3, 0x5000, 2, 2, 1, {4, 5, 4, 0}},
    {"/Game/Store/shop_b_obj_00_nce.bin", 0, 0x5000, 2, 2, 1, {1, 2, 1, 0}},
    {"/Game/Store/shop_b_obj_00_nce.bin", 6, 0x5000, 2, 2, 1, {7, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x0, 0, 2, 1, {2, 3, 3, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 0, 0x4000, 1, 2, 1, {0, 2, 0, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 1, 0x4000, 1, 2, 1, {1, 3, 1, 0}},
};

StoreScene::StoreScene() :
    SceneBase(2, 0) {
    mMessages       = NULL;
    mTextBuffer     = NULL;
    mGlyphBuffer    = NULL;
    mTextBufferSize = 0;
    mGlyphCount     = 0;
    mUnk_2c         = 9;
    MI_CpuClear8(&mContext->mInfo, sizeof(SceneInfo));
    mMenu    = NULL;
    mMessage = NULL;
    for (u8 i = 0; i < 2; i++) {
        mCellAnims[i] = NULL;
    }
    data_020d43f0[0].mUnk_5b9_0 = 0;
    FrameTask::Get().func_0203dc34(0, 0x16);
}

StoreScene::~StoreScene() {
    Cleanup();
    FrameTask::Get().func_0203dd90();
}

// non-matching: the cases' fall-through order differs (register swap on the data_020d4414 write in case 0)
void StoreScene::Update() {
    UnkStruct_SceneContext *ctx = mContext;
    switch (ctx->mInfo.mState) {
        case 0:
            if (data_020d43f0[0].mUnk_24 & 1) {
                ctx->mInfo.mState = 6;
                break;
            }
            data_020d4414.mUnk_00 |= 1;
            ctx->mInfo.mState++;
            break;
        case 1:
            InitDisplay();
            mEvent = new UnkStruct_02065c40(4, 0, 1);
            ctx->mInfo.mState++;
            break;
        case 2:
            if (mEvent->func_020678e8()) {
                break;
            }
            mEvent->func_0206eb10("cmd_ST_sev_Ali.bin");
            SpriteManager::Get().StartFade(2);
            ctx->mInfo.mState++;
            break;
        case 3:
            if (mEvent->func_020678e8()) {
                break;
            }
            SpriteManager::Get().StartFade(3);
            ctx->mInfo.mState++;
            break;
        case 4:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->func_020676f8();
            mContext->mUnk_12 = 0;
            ctx->mInfo.mState++;
            break;
        case 5:
            if (ctx->mUnk_12 != 0) {
                ctx->mInfo.mState++;
            }
            break;
        case 6:
            InitDisplay();
            LoadGraphics();
            SetupText();
            mEvent = new UnkStruct_02065c40(4, 0, 0);
            ctx->mInfo.mState++;
            break;
        case 7:
            if (mEvent->func_020678e8()) {
                break;
            }
            mEvent->func_0206ef4c("mes_sm_button_a", 0, 0, 1, 5);
            PrintStatus();
            ctx->mInfo.mState++;
            break;
        case 8:
            if (mEvent->func_020678e8()) {
                break;
            }
            mEvent->func_02067998(sSelectScriptIds[ctx->mInfo.mSelection]);
            SpriteManager::Get().StartFade(2);
            ctx->mInfo.mState++;
            break;
        case 9:
            UpdateMenu();
            DrawText();
            DrawMoney();
            if (!SpriteManager::Get().IsFading()) {
                ctx->mInfo.mState++;
            }
            break;
        case 10:
            func_ov003_0221ac5c();
            UpdateMenu();
            DrawText();
            DrawMoney();
            break;
        case 11:
            if (mMenu->mUnk_061 != 1) {
                mMenu->Update(TRUE);
            }
            if (ScrollStep()) {
                ctx->mInfo.mState = 10;
            }
            mMenu->Draw();
            DrawText();
            DrawMoney();
            break;
        case 12:
            if (mMenu != NULL) {
                mMenu->Draw();
            }
            DrawText();
            SpriteManager::Get().StartSubFade(3);
            switch (ctx->mInfo.mUnk_06) {
                case 0:
                    mEvent->func_020702e0(0x4c, 0x2c, 0x3c, 0xa6, 2, 0x11);
                    break;
                case 1:
                    mEvent->func_020702e0(0x53, -8, 0x39, 0xad, -0x26, 0xe);
                    break;
                case 2:
                    mEvent->func_020702e0(0x8e, 0x44, -0x33, 0xe9, 0x40, -0x5e);
                    break;
            }
            ctx->mInfo.mState++;
            break;
        case 13:
            if (mMenu != NULL) {
                mMenu->Draw();
            }
            DrawText();
            if (mEvent->func_020678e8()) {
                break;
            }
            SpriteManager::Get().StartFade(3);
            ctx->mInfo.mState++;
            break;
        case 14:
            if (mMenu != NULL) {
                mMenu->Draw();
            }
            DrawText();
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->func_020676f8();
            mContext->mUnk_12 = 0;
            ctx->mInfo.mState++;
            break;
        case 15:
            if (ctx->mUnk_12 == 0) {
                break;
            }
            SpriteManager::Get().StartFade(3);
            ctx->mInfo.mState++;
            break;
        case 16:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            switch (ctx->mInfo.mUnk_06) {
                case 0:
                    new UnkStruct_02078e14(0, 0);
                    break;
                case 1:
                    new UnkStruct_02078e14(2, 0);
                    break;
                case 2:
                    new UnkStruct_02078e14(1, 0);
                    break;
            }
            mContext->mUnk_12 = 0;
            Cleanup();
            ctx->mInfo.mState++;
            break;
        case 17:
            if (ctx->mUnk_12 == 0) {
                break;
            }
            ctx->mInfo.mMenuResult = 0;
            ctx->mInfo.mState      = 6;
            break;
        case 18:
            ctx->mInfo.mUnk_0a = 0;
            ctx->mInfo.mState++;
            mMenu->Draw();
            break;
        case 19:
            if (++ctx->mInfo.mUnk_0a >= 8) {
                SpriteManager::Get().StartFade(3);
                ctx->mInfo.mState++;
            }
            mMenu->Draw();
            break;
        case 20:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->func_020676f8();
            mContext->mUnk_12 = 0;
            ctx->mInfo.mState++;
            break;
        case 21:
            if (ctx->mUnk_12 != 0) {
                ctx->mInfo.mState++;
            }
            break;
        case 22: {
            s32 result = func_02087468();
            if (result >= 0) {
                new UnkStruct_02089f90(result);
                mContext->mUnk_12 = 0;
                ctx->mInfo.mState++;
            } else {
                func_02086d48();
                ctx->mInfo.mState += 2;
            }
            break;
        }
        case 23:
            if (ctx->mUnk_12 != 0) {
                ctx->mInfo.mState--;
            }
            break;
        case 24:
            ctx->mList->mHead->mRequest->mNextScene = mUnk_2c;
            mFinished                               = TRUE;
            break;
    }
}

void StoreScene::Cleanup() {
    ClearTextBuffer();
    if (mMessages != NULL) {
        OS_Free(mMessages);
        mMessages = NULL;
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
        mTextBuffer = NULL;
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
    delete mMenu;
    mMenu = NULL;
    delete mMessage;
    mMessage = NULL;
    for (u8 i = 0; i < 2; i++) {
        delete mCellAnims[i];
        mCellAnims[i] = NULL;
    }
}

void StoreScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 7);
    func_02001e68(1, 0, 1);
    func_02001ed0(0);
    G2_SetBG0Control(1, 1, 0, 1, 1);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2ControlText(0, 0, 4, 6);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2S_SetBG2Priority(3);
    G2S_SetBG3Priority(3);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
    MI_CpuClearFast((void *) 0x06000000, 0x80000);
    MI_CpuClearFast((void *) 0x06400000, 0x40000);
    MI_CpuClearFast((void *) 0x06200000, 0x20000);
}

void StoreScene::LoadGraphics() {
    ChangeDir("/Game/Store/");
    func_02025b64(1, 7, 0, "shop_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "shop_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "shop_b_bg_00_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/Store/");
    SpriteScreen &shop = SpriteManager::Get().Screen(1);
    shop.func_02027624(3);
    shop.LoadCharacters("shop_b_obj_00.nce_ncg1D.bin", 0x5000, 0);
    shop.LoadPalette("shop_b_obj_00_ncl.bin", 0x40, 0);
    ChangeDir("/Game/2D/union_b/");
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("union_b_obj_cockpit_ncl.bin", 0, 0);
    sub.LoadCharacters("union_b_obj_arrow.nce_ncg1D.bin", 0x4000, 0);
    sub.LoadPalette("union_b_obj_arrow_ncl.bin", 0x20, 0);
    sub.LoadCharacters("union_b_obj_point.nce_ncg1D.bin", 0x10000, 0);
    sub.LoadPalette("union_b_obj_point_ncl.bin", 0x160, 0);
    sub.LoadCharacters("union_b_obj_number.nce_ncg1D.bin", 0x11000, 0);
    sub.LoadPalette("union_b_obj_number_ncl.bin", 0x160, 0);
    mCellAnims[0]            = new CellAnimation("union_b_obj_point_nce.bin", 0, 1, 1);
    mCellAnims[0]->mCharBase = sub.func_020276f4(0x10000);
    mCellAnims[0]->mUnk_11   = 11;
    mCellAnims[0]->SetAnimation(0);
    mCellAnims[1]            = new CellAnimation("union_b_obj_number_nce.bin", 0, 1, 1);
    mCellAnims[1]->mCharBase = sub.func_020276f4(0x11000);
    mCellAnims[1]->mUnk_11   = 11;
    mCellAnims[1]->SetAnimation(0);
    CreateMenu();
}

void StoreScene::CreateMenu() {
    SceneInfo *info = &mContext->mInfo;
    delete mMenu;
    mMenu = new SelectWindow(6, sMenuItems, info->mSelection);
    mMenu->SetCells(6, sMenuCells);
    mMenu->Update(TRUE);
    UpdateSlotLayout();
}

void StoreScene::UpdateMenu() {
    SceneInfo *info      = &mContext->mInfo;
    info->mPrevSelection = info->mSelection;
    if (mMenu != NULL) {
        if (info->mState == 10) {
            info->mMenuResult = mMenu->Update(TRUE);
        }
        s32 delta = 0;
        switch (info->mMenuResult) {
            case 1:
                break;
            case 2:
                info->mSelection = 3;
                info->mState     = 18;
                mMenu->Draw();
                return;
            case 3: {
                u8 id = mMenu->GetSelectedId();
                switch (id) {
                    case 4:
                        delta = -1;
                        break;
                    case 5:
                        delta = 1;
                        break;
                }
                break;
            }
            default:
                if (mMenu->mUnk_061 == 1) {
                    if (InputManager::Get().IsTrigger(0x20)) {
                        if (info->mSelection != 0) {
                            mMenu->func_0205c844(4, 3);
                            SoundManager::Get().func_020246f8(0);
                        }
                    } else if (InputManager::Get().IsTrigger(0x10)) {
                        if (info->mSelection < 2) {
                            mMenu->func_0205c844(5, 3);
                            SoundManager::Get().func_020246f8(0);
                        }
                    }
                }
                break;
        }
        if (delta != 0) {
            info->mScrollDir = delta > 0 ? 2 : 1;
            info->mPage += delta;
            func_ov003_0221ab38();
            info->mMenuResult = 0;
        }
        mMenu->Draw();
    }
    if (info->mPrevSelection != info->mSelection && info->mSelection >= 0) {
        BOOL valid = TRUE;
        if (info->mSelection >= 3) {
            valid = FALSE;
        }
        if (valid) {
            PrintStatus();
            mEvent->func_0206f564(sSelectEventIds[info->mSelection], 10, -1);
        }
    }
}

void StoreScene::func_ov003_0221ab38() {
    SceneInfo *info = &mContext->mInfo;
    info->mState    = 11;
    info->mScroll   = 0x62;
    u8 i;
    WindowItem *items = mMenu->mCursor->mItems;
    for (i = 0; i < 3; i++) {
        if (info->mPage == i) {
            mMenu->mCells[i]->func_02029f6c();
            CellAnimation *cell = mMenu->mCells[i];
            cell->mUnk_0e       = sSelectedFrames[i];
            cell->func_02029f6c();
        } else if (info->mSelection == i) {
            mMenu->mCells[i]->func_02029fb8(2);
        }
        items[i].mFlags = 0x8000;
    }
    if (info->mPage >= 2) {
        mMenu->mButtons[0].mEnabled = 0;
    } else {
        mMenu->mButtons[0].mEnabled = 1;
    }
    mMenu->mButtons[2].mEnabled = info->mPage >= 1 ? 1 : 0;
    if (info->mPage == 0) {
        mMenu->mButtons[4].mEnabled = 0;
        items[4].mFlags             = 0x8000;
    }
    if (info->mPage == 2) {
        mMenu->mButtons[5].mEnabled = 0;
        items[5].mFlags             = 0x8000;
    }
}

void StoreScene::func_ov003_0221ac5c() {
    SceneInfo *info = &mContext->mInfo;
    switch (info->mMenuResult) {
        case 2:
            info->mState = 18;
            return;
        case 1:
        case 3:
            break;
        default:
            return;
    }
    if (info->mSelection != 0 && info->mPrevSelection != info->mSelection) {
        return;
    }
    switch (info->mSelection) {
        case 0:
            info->mUnk_06 = 0;
            info->mState  = 12;
            break;
        case 1:
            info->mUnk_06 = 1;
            info->mState  = 12;
            break;
        case 2:
            info->mUnk_06 = 2;
            info->mState  = 12;
            break;
        case 3:
            info->mState = 18;
            break;
    }
}

BOOL StoreScene::ScrollStep() {
    SceneInfo *info = &mContext->mInfo;
    s16 step        = 14;
    BOOL done       = FALSE;
    if (info->mScroll < 14) {
        step = info->mScroll;
    }
    info->mScroll -= step;
    if (info->mScrollDir == 2) {
        step = -step;
    }
    WindowItem *items = mMenu->mCursor->mItems;
    for (u8 i = 0; i < 3; i++) {
        Vec2s pos = mMenu->GetCellPos(i);
        pos.x += step;
        mMenu->SetCellPos(i, pos);
        items[i].mX = step + items[i].mX;
    }
    if (info->mScroll == 0) {
        CellAnimation *sel = mMenu->mCells[(u8) info->mSelection];
        sel->mUnk_0e       = sIdleFrames[info->mSelection];
        sel->func_02029f6c();
        items[info->mPage].mFlags      = 8;
        items[info->mSelection].mFlags = 0x8000;
        mMenu->mCursor->mSelection     = info->mPage;
        info->mSelection               = info->mPage;
        mEvent->func_02067998(sSelectScriptIds[info->mSelection]);
        mEvent->func_0206f564(sSelectEventIds[info->mSelection], 10, -1);
        if (info->mSelection != 0) {
            mMenu->mButtons[4].mEnabled = 1;
            items[4].mFlags             = 8;
        }
        if (info->mSelection != 2) {
            mMenu->mButtons[5].mEnabled = 1;
            items[5].mFlags             = 8;
        }
        mMenu->mCells[4]->func_02029fc0();
        mMenu->mCells[5]->func_02029fc0();
        if (info->mScrollDir == 1) {
            CellAnimation *cell = mMenu->mCells[4];
            cell->mUnk_0e       = 0;
            cell->func_02029f6c();
        } else {
            CellAnimation *cell = mMenu->mCells[5];
            cell->mUnk_0e       = 1;
            cell->func_02029f6c();
        }
        PrintStatus();
        done = TRUE;
    }
    return done;
}

void StoreScene::UpdateSlotLayout() {
    SceneInfo *info   = &mContext->mInfo;
    WindowItem *items = mMenu->mCursor->mItems;
    s16 offset        = info->mSelection * 98;
    for (u8 i = 0; i < 3; i++) {
        Vec2s pos = mMenu->GetCellPos(i);
        pos.x     = i * 98 + 0x58 - offset;
        mMenu->SetCellPos(i, pos);
        items[i].mX = pos.x;
        if (info->mSelection == i) {
            CellAnimation *cell = mMenu->mCells[i];
            cell->mUnk_0e       = sSelectedFrames[i];
            cell->func_02029f6c();
            items[i].mFlags = 8;
        } else {
            CellAnimation *cell = mMenu->mCells[i];
            cell->mUnk_0e       = sIdleFrames[i];
            cell->func_02029f6c();
            items[i].mFlags = 0x8000;
        }
        mMenu->mCells[i]->func_02029f6c();
    }
    u32 flags;
    if (info->mSelection != 0) {
        mMenu->mButtons[4].mEnabled = 1;
        flags                       = 8;
    } else {
        mMenu->mButtons[4].mEnabled = 0;
        flags                       = 0x8000;
    }
    items[4].mFlags = flags;
    if (info->mSelection == 2) {
        mMenu->mButtons[5].mEnabled = 0;
        flags                       = 0x8000;
    } else {
        mMenu->mButtons[5].mEnabled = 1;
        flags                       = 8;
    }
    items[5].mFlags = flags;
    if (info->mPage >= 2) {
        mMenu->mButtons[0].mEnabled = 0;
    } else {
        mMenu->mButtons[0].mEnabled = 1;
    }
    mMenu->mButtons[2].mEnabled = info->mPage >= 1 ? 1 : 0;
}

void StoreScene::SetupText() {
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    mMessages       = (MessageFile *) func_02023504("/font/mes_storecomment.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x10, 2);
    mGlyphCount     = TextManager::Get().GlyphCount(4);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    ClearTextBuffer();
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x10, 2, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    mMessage = new MessageWindow("mes_storecomment", 1, 0x1800);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(0xa, 0, 1);
}

void StoreScene::DrawText() {
    UnkStruct_SceneContext *ctx = mContext;
    if (mGlyphBuffer != NULL) {
        SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, mGlyphCount,
                                                     (s16) (0x80 - ((u32) ctx->mInfo.mUnk_00 >> 1)), 0x90, 0, 0x60,
                                                     0xf, 0);
    }
    if (mMessage != NULL) {
        mMessage->func_020658d0();
    }
}

// non-matching: the guard loads around the TextManager::mUnk_5b stores are scheduled differently and one stm
// differs
void StoreScene::PrintStatus() {
    SceneInfo *info = &mContext->mInfo;
    u8 index        = sMessageIndices[info->mSelection];
    ClearTextBuffer();
    TextManager::Get().mUnk_5b = 1;
    TextManager &text          = TextManager::Get();
    s16 height;
    s16 width;
    width              = 0;
    height             = 0;
    MessageFile *table = mMessages;
    text.Print(table->GetText(index), table->GetLength(index), mTextBuffer, 0x10, 2, &height, &width,
               &info->mUnk_00, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x3000, mTextBuffer, mTextBufferSize, 0);
}

void StoreScene::ClearTextBuffer() {
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}

void StoreScene::DrawMoney() {
    u8 j      = 0;
    u32 money = data_020d43f0[0].mMoney;
    s8 digits[7] = {-1, -1, -1, -1, -1, -1, -1};
    s8 i      = 6;
    do {
        digits[i] = money % 10;
        money /= 10;
        i--;
    } while (i >= 0 && money != 0);
    for (; j < 7; j++) {
        if (digits[j] >= 0 && mCellAnims[1] != NULL) {
            mCellAnims[1]->func_0202a3ec(1, digits[j], j * 7 + 0x2e, 0x17, 1);
        }
    }
    if (mCellAnims[0] != NULL) {
        mCellAnims[0]->Draw(1, 3, 0x14, 1);
    }
}
