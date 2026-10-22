#include "System/SoundManager.hpp"
#include "System/FrameTask.hpp"
#include "Scene/StoreScene.hpp"
#include "Scene/ListScene.hpp"
#include "Game/ScriptPlayer.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/SaveData.hpp"
#include "Game/UnkStruct_020d4414.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Scene/GiftUnlockScene.hpp"

// Scroll directions: the values UpdateMenu stores in mInfo.mScrollDir when an arrow starts a scroll. PREV goes with
// mPage - 1 (arrow item 4 or KEY_LEFT), NEXT with mPage + 1 (arrow item 5 or KEY_RIGHT); ScrollStep negates its x step
// for NEXT and, when the scroll ends, restarts arrow cell 4 for PREV and arrow cell 5 otherwise.
#define STORE_SCROLL_PREV 1
#define STORE_SCROLL_NEXT 2

// Menu entries: the values HandleMenuResult stores in mInfo.mUnk_06 for menu selections 0-2, which Update switches on
// to move the event camera (state 12) and to create the list (state 16), each named after the ListScene mode created
// for it (LIST_MODE_STORE_BAKUGAN / STORE_CARD / STORE_GAUGE).
#define STORE_MENU_BAKUGAN 0
#define STORE_MENU_CARD 1
#define STORE_MENU_GAUGE 2

s32 UnlockEmblem8Or9();
void func_02086d48();

// only ever SetMapExtraAnim's argument (GetAnimObjForSet(1, n))
static const u8 sSelectScriptIds[3]     = {2, 1, 3};
static const u8 sMessageIndices[3]      = {0x12, 0x13, 0x14};
static const u8 sIdleAnimIndices[3]     = {3, 0, 6};
static const u8 sSelectedAnimIndices[3] = {4, 1, 7};
static const u8 sSelectEventIds[3]      = {1, 2, 3};

static const WindowItemRecord sMenuItems[6] = {
    {0, WINDOW_ITEM_CURSOR_SKIP, 88, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {1, WINDOW_ITEM_CURSOR_SKIP, 186, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {2, WINDOW_ITEM_CURSOR_SKIP, 284, 56, 80, 80, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {3, 0x2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {4, WINDOW_ITEM_NO_SLIDE_OUT, 0, 72, 80, 48, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
    {5, WINDOW_ITEM_NO_SLIDE_OUT, 176, 72, 80, 48, 0, {-1, -1, -1, -1}, 0, WINDOW_TOUCH_RELEASE},
};

static const WindowCellRecord sMenuCells[6] = {
    {"/Game/Store/shop_b_obj_00_nce.bin", 3, 0x5000, 2, 2, 1, {4, 5, 4, 0}},
    {"/Game/Store/shop_b_obj_00_nce.bin", 0, 0x5000, 2, 2, 1, {1, 2, 1, 0}},
    {"/Game/Store/shop_b_obj_00_nce.bin", 6, 0x5000, 2, 2, 1, {7, 8, 7, 0}},
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x0, 0, 2, 1, {2, 3, 3, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 0, 0x4000, 1, 2, 1, {0, 2, 0, 0}},
    {"/Game/2D/union_b/union_b_obj_arrow_nce.bin", 1, 0x4000, 1, 2, 1, {1, 3, 1, 0}},
};

// Event camera positions per shop counter (read with constant indices, so the code holds them as immediates).
static const s16 sEventPositions[3][6] = {
    {0x53, -8, 0x39, 0xad, -0x26, 0xe},
    {0x8e, 0x44, -0x33, 0xe9, 0x40, -0x5e},
    {0x4c, 0x2c, 0x3c, 0xa6, 2, 0x11},
};

StoreScene::StoreScene() :
    SceneBase(2, 0) {
    mMessages       = NULL;
    mTextBuffer     = NULL;
    mGlyphBuffer    = NULL;
    mTextBufferSize = 0;
    mGlyphCount     = 0;
    mNextScene         = SCENE_MAP;
    MI_CpuClear8(&mContext->mInfo, sizeof(SceneInfo));
    mWindow    = NULL;
    mMessageWindow = NULL;
    for (u8 i = 0; i < 2; i++) {
        mCellAnims[i] = NULL;
    }
    gSaveData[0].mStoreItemsUnlocked = 0;
    FrameTask::Get().StartChannel(0, 0x16);
}

StoreScene::~StoreScene() {
    Cleanup();
    FrameTask::Get().StopAllChannels();
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
    delete mWindow;
    mWindow = NULL;
    delete mMessageWindow;
    mMessageWindow = NULL;
    for (u8 i = 0; i < 2; i++) {
        delete mCellAnims[i];
        mCellAnims[i] = NULL;
    }
}

void StoreScene::Update() {
    SceneContext *ctx = mContext;
    switch (ctx->mInfo.mState) {
        case 0:
            if (gSaveData[0].mUnk_24 & 1) {
                ctx->mInfo.mState = 6;
                break;
            }
            *(u8 *) &gSaveData[0].mUnk_24 |= 1;
            ctx->mInfo.mState++;
            break;
        case 1:
            InitDisplay();
            mEvent = new ScriptPlayer(SCRIPT_KIND_STORE, 0, 1);
            ctx->mInfo.mState++;
            break;
        case 2:
            if (mEvent->IsRunning()) {
                break;
            }
            mEvent->StartScript("cmd_ST_sev_Ali.bin");
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
            ctx->mInfo.mState++;
            break;
        case 3:
            if (mEvent->IsRunning()) {
                break;
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            ctx->mInfo.mState++;
            break;
        case 4:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            ctx->mInfo.mState++;
            break;
        case 5:
            if (ctx->mRequest != 0) {
                ctx->mInfo.mState++;
            }
            break;
        case 6:
            InitDisplay();
            LoadGraphics();
            SetupText();
            mEvent = new ScriptPlayer(SCRIPT_KIND_STORE, 0, 0);
            ctx->mInfo.mState++;
            break;
        case 7:
            if (mEvent->IsRunning()) {
                break;
            }
            mEvent->func_0206ef4c("mes_sm_button_a", 0, 0, 1, 5);
            PrintStatus();
            ctx->mInfo.mState++;
            break;
        case 8:
            if (mEvent->IsRunning()) {
                break;
            }
            mEvent->SetMapExtraAnim(sSelectScriptIds[ctx->mInfo.mSelection]);
            SpriteManager::Get().StartFade(FADE_MODE_TO_ZERO);
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
            HandleMenuResult();
            UpdateMenu();
            DrawText();
            DrawMoney();
            break;
        case 11:
            if (mWindow->mState != 1) {
                mWindow->Update(TRUE);
            }
            if (ScrollStep()) {
                ctx->mInfo.mState = 10;
            }
            mWindow->Draw();
            DrawText();
            DrawMoney();
            break;
        case 12:
            if (mWindow != NULL) {
                mWindow->Draw();
            }
            DrawText();
            SpriteManager::Get().StartSubFade(FADE_MODE_DOWN);
            switch (ctx->mInfo.mUnk_06) {
                case STORE_MENU_BAKUGAN:
                    mEvent->func_020702e0(sEventPositions[2][0], sEventPositions[2][1], sEventPositions[2][2],
                                          sEventPositions[2][3], sEventPositions[2][4], sEventPositions[2][5]);
                    break;
                case STORE_MENU_CARD:
                    mEvent->func_020702e0(sEventPositions[0][0], sEventPositions[0][1], sEventPositions[0][2],
                                          sEventPositions[0][3], sEventPositions[0][4], sEventPositions[0][5]);
                    break;
                case STORE_MENU_GAUGE:
                    mEvent->func_020702e0(sEventPositions[1][0], sEventPositions[1][1], sEventPositions[1][2],
                                          sEventPositions[1][3], sEventPositions[1][4], sEventPositions[1][5]);
                    break;
            }
            ctx->mInfo.mState++;
            break;
        case 13:
            if (mWindow != NULL) {
                mWindow->Draw();
            }
            DrawText();
            if (mEvent->IsRunning()) {
                break;
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            ctx->mInfo.mState++;
            break;
        case 14:
            if (mWindow != NULL) {
                mWindow->Draw();
            }
            DrawText();
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            ctx->mInfo.mState++;
            break;
        case 15:
            if (ctx->mRequest == 0) {
                break;
            }
            SpriteManager::Get().StartFade(FADE_MODE_DOWN);
            ctx->mInfo.mState++;
            break;
        case 16:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            switch (ctx->mInfo.mUnk_06) {
                case STORE_MENU_BAKUGAN:
                    new ListScene(LIST_MODE_STORE_BAKUGAN, 0);
                    break;
                case STORE_MENU_CARD:
                    new ListScene(LIST_MODE_STORE_CARD, 0);
                    break;
                case STORE_MENU_GAUGE:
                    new ListScene(LIST_MODE_STORE_GAUGE, 0);
                    break;
            }
            mContext->mRequest = 0;
            Cleanup();
            ctx->mInfo.mState++;
            break;
        case 17:
            if (ctx->mRequest == 0) {
                break;
            }
            ctx->mInfo.mMenuResult = 0;
            ctx->mInfo.mState      = 6;
            break;
        case 18:
            ctx->mInfo.mUnk_0a = 0;
            ctx->mInfo.mState++;
            mWindow->Draw();
            break;
        case 19:
            if (++ctx->mInfo.mUnk_0a >= 8) {
                SpriteManager::Get().StartFade(FADE_MODE_DOWN);
                ctx->mInfo.mState++;
            }
            mWindow->Draw();
            break;
        case 20:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->RequestFinish();
            mContext->mRequest = 0;
            ctx->mInfo.mState++;
            break;
        case 21:
            if (ctx->mRequest != 0) {
                ctx->mInfo.mState++;
            }
            break;
        case 22: {
            s32 result = UnlockEmblem8Or9();
            if (result >= 0) {
                new GiftUnlockScene(result);
                mContext->mRequest = 0;
                ctx->mInfo.mState++;
            } else {
                func_02086d48();
                ctx->mInfo.mState += 2;
            }
            break;
        }
        case 23:
            if (ctx->mRequest != 0) {
                ctx->mInfo.mState--;
            }
            break;
        case 24:
            ctx->mList->mHead->mRequest->mRequest = mNextScene;
            mFinished                               = TRUE;
            break;
    }
}

void StoreScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().SetupMode(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
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
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "shop_b_bg_00_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "shop_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "shop_b_bg_00_ncl.bin", 0, 0, 0);
    ChangeDir("/Game/Store/");
    SpriteScreen &shop = SpriteManager::Get().Screen(1);
    shop.SetObjVramMode(3);
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
    mCellAnims[0]->mCharBase = sub.GetCharBase(0x10000);
    mCellAnims[0]->mPaletteBase   = 11;
    mCellAnims[0]->SetAnimation(0);
    mCellAnims[1]            = new CellAnimation("union_b_obj_number_nce.bin", 0, 1, 1);
    mCellAnims[1]->mCharBase = sub.GetCharBase(0x11000);
    mCellAnims[1]->mPaletteBase   = 11;
    mCellAnims[1]->SetAnimation(0);
    CreateMenu();
}

void StoreScene::CreateMenu() {
    SceneInfo *info = &mContext->mInfo;
    delete mWindow;
    mWindow = new SelectWindow(6, sMenuItems, info->mSelection);
    mWindow->SetCells(6, sMenuCells);
    mWindow->Update(TRUE);
    UpdateSlotLayout();
}

void StoreScene::UpdateMenu() {
    SceneInfo *info      = &mContext->mInfo;
    info->mPrevSelection = info->mSelection;
    if (mWindow != NULL) {
        if (info->mState == 10) {
            info->mMenuResult = mWindow->Update(TRUE);
        }
        s32 delta = 0;
        switch (info->mMenuResult) {
            case 1:
                break;
            case 2:
                info->mSelection = 3;
                info->mState     = 18;
                mWindow->Draw();
                return;
            case 3: {
                u8 id = mWindow->GetSelectedId();
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
                if (mWindow->mState == 1) {
                    if (InputManager::Get().IsTrigger(KEY_LEFT)) {
                        if (info->mSelection != 0) {
                            mWindow->SetResultItem(4, 3);
                            SoundManager::Get().StartSeqArc(0);
                        }
                    } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
                        if (info->mSelection < 2) {
                            mWindow->SetResultItem(5, 3);
                            SoundManager::Get().StartSeqArc(0);
                        }
                    }
                }
                break;
        }
        if (delta != 0) {
            info->mScrollDir = delta > 0 ? STORE_SCROLL_NEXT : STORE_SCROLL_PREV;
            info->mPage += delta;
            StartScroll();
            info->mMenuResult = 0;
        }
        mWindow->Draw();
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

void StoreScene::StartScroll() {
    SceneInfo *info        = &mContext->mInfo;
    info->mState           = 11;
    info->mScrollRemaining = 0x62;
    u8 i;
    WindowItem *items = mWindow->mCursor->mItems;
    for (i = 0; i < 3; i++) {
        if (info->mPage == i) {
            mWindow->mCellAnims[i]->ResetAnimation();
            CellAnimation *cell = mWindow->mCellAnims[i];
            cell->mAnimIndex       = sSelectedAnimIndices[i];
            cell->ResetAnimation();
        } else if (info->mSelection == i) {
            mWindow->mCellAnims[i]->SetMode(CELL_ANIM_MODE_BACKWARD_ONCE);
        }
        items[i].mFlags = WINDOW_ITEM_CURSOR_SKIP;
    }
    if (info->mPage >= 2) {
        mWindow->mButtons[0].mEnabled = 0;
    } else {
        mWindow->mButtons[0].mEnabled = 1;
    }
    mWindow->mButtons[2].mEnabled = info->mPage >= 1 ? 1 : 0;
    if (info->mPage == 0) {
        mWindow->mButtons[4].mEnabled = 0;
        items[4].mFlags             = WINDOW_ITEM_CURSOR_SKIP;
    }
    if (info->mPage == 2) {
        mWindow->mButtons[5].mEnabled = 0;
        items[5].mFlags             = WINDOW_ITEM_CURSOR_SKIP;
    }
}

void StoreScene::HandleMenuResult() {
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
            info->mUnk_06 = STORE_MENU_BAKUGAN;
            info->mState  = 12;
            break;
        case 1:
            info->mUnk_06 = STORE_MENU_CARD;
            info->mState  = 12;
            break;
        case 2:
            info->mUnk_06 = STORE_MENU_GAUGE;
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
    if (info->mScrollRemaining < 14) {
        step = info->mScrollRemaining;
    }
    info->mScrollRemaining -= step;
    if (info->mScrollDir == STORE_SCROLL_NEXT) {
        step = -step;
    }
    WindowItem *items = mWindow->mCursor->mItems;
    for (u8 i = 0; i < 3; i++) {
        Vec2s pos = mWindow->GetCellPos(i);
        pos.x += step;
        mWindow->SetCellPos(i, pos);
        items[i].mX = step + items[i].mX;
    }
    if (info->mScrollRemaining == 0) {
        CellAnimation *sel = mWindow->mCellAnims[(u8) info->mSelection];
        sel->mAnimIndex       = sIdleAnimIndices[info->mSelection];
        sel->ResetAnimation();
        items[info->mPage].mFlags      = WINDOW_ITEM_NO_SLIDE_OUT;
        items[info->mSelection].mFlags = WINDOW_ITEM_CURSOR_SKIP;
        mWindow->mCursor->mSelection     = info->mPage;
        info->mSelection               = info->mPage;
        mEvent->SetMapExtraAnim(sSelectScriptIds[info->mSelection]);
        mEvent->func_0206f564(sSelectEventIds[info->mSelection], 10, -1);
        if (info->mSelection != 0) {
            mWindow->mButtons[4].mEnabled = 1;
            items[4].mFlags             = WINDOW_ITEM_NO_SLIDE_OUT;
        }
        if (info->mSelection != 2) {
            mWindow->mButtons[5].mEnabled = 1;
            items[5].mFlags             = WINDOW_ITEM_NO_SLIDE_OUT;
        }
        mWindow->mCellAnims[4]->Rewind();
        mWindow->mCellAnims[5]->Rewind();
        if (info->mScrollDir == STORE_SCROLL_PREV) {
            CellAnimation *cell = mWindow->mCellAnims[4];
            cell->mAnimIndex       = 0;
            cell->ResetAnimation();
        } else {
            CellAnimation *cell = mWindow->mCellAnims[5];
            cell->mAnimIndex       = 1;
            cell->ResetAnimation();
        }
        PrintStatus();
        done = TRUE;
    }
    return done;
}

void StoreScene::UpdateSlotLayout() {
    SceneInfo *info   = &mContext->mInfo;
    WindowItem *items = mWindow->mCursor->mItems;
    s16 offset        = info->mSelection * 98;
    for (u8 i = 0; i < 3; i++) {
        Vec2s pos = mWindow->GetCellPos(i);
        pos.x     = i * 98 + 0x58 - offset;
        mWindow->SetCellPos(i, pos);
        items[i].mX = pos.x;
        if (info->mSelection == i) {
            CellAnimation *cell = mWindow->mCellAnims[i];
            cell->mAnimIndex       = sSelectedAnimIndices[i];
            cell->ResetAnimation();
            items[i].mFlags = WINDOW_ITEM_NO_SLIDE_OUT;
        } else {
            CellAnimation *cell = mWindow->mCellAnims[i];
            cell->mAnimIndex       = sIdleAnimIndices[i];
            cell->ResetAnimation();
            items[i].mFlags = WINDOW_ITEM_CURSOR_SKIP;
        }
        mWindow->mCellAnims[i]->ResetAnimation();
    }
    u32 flags;
    if (info->mSelection != 0) {
        mWindow->mButtons[4].mEnabled = 1;
        flags                       = WINDOW_ITEM_NO_SLIDE_OUT;
    } else {
        mWindow->mButtons[4].mEnabled = 0;
        flags                       = WINDOW_ITEM_CURSOR_SKIP;
    }
    items[4].mFlags = flags;
    if (info->mSelection == 2) {
        mWindow->mButtons[5].mEnabled = 0;
        flags                       = WINDOW_ITEM_CURSOR_SKIP;
    } else {
        mWindow->mButtons[5].mEnabled = 1;
        flags                       = WINDOW_ITEM_NO_SLIDE_OUT;
    }
    items[5].mFlags = flags;
    if (info->mPage >= 2) {
        mWindow->mButtons[0].mEnabled = 0;
    } else {
        mWindow->mButtons[0].mEnabled = 1;
    }
    mWindow->mButtons[2].mEnabled = info->mPage >= 1 ? 1 : 0;
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
    mMessages       = (MessageFile *) LoadFile("/font/mes_storecomment.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x10, 2);
    mGlyphCount     = TextManager::Get().GlyphCount(4);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    ClearTextBuffer();
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x10, 2, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    mMessageWindow = new MessageWindow("mes_storecomment", 1, 0x1800);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
    mMessageWindow->StartMessage(0xa, 0, 1);
}

void StoreScene::DrawText() {
    SceneContext *ctx = mContext;
    if (mGlyphBuffer != NULL) {
        SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mGlyphCount,
                                                     (s16) (0x80 - ((u32) ctx->mInfo.mUnk_00 >> 1)), 0x90, 0, 0x60,
                                                     0xf, 0);
    }
    if (mMessageWindow != NULL) {
        mMessageWindow->Update();
    }
}

void StoreScene::PrintStatus() {
    u8 index;
    SceneInfo *info = &mContext->mInfo;
    index           = sMessageIndices[info->mSelection];
    ClearTextBuffer();
    TextManager::Get().mUse4bppFont = 1;
    TextManager &text          = TextManager::Get();
    s16 height;
    s16 width;
    width              = 0;
    height             = 0;
    MessageFile *table = mMessages;
    text.Print(table->GetText(index), table->GetLength(index), mTextBuffer, 0x10, 2, &height, &width,
               &info->mUnk_00, 0, 2, 0x21, 0xe, 0, 0, 0, 1, 1);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0x3000, mTextBuffer, mTextBufferSize, 0);
}

void StoreScene::ClearTextBuffer() {
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}

void StoreScene::DrawMoney() {
    u8 j      = 0;
    u32 money = gSaveData[0].mMoney;
    s8 digits[7] = {-1, -1, -1, -1, -1, -1, -1};
    s8 i      = 6;
    do {
        digits[i] = money % 10;
        money /= 10;
        i--;
    } while (i >= 0 && money != 0);
    for (; j < 7; j++) {
        if (digits[j] >= 0 && mCellAnims[1] != NULL) {
            mCellAnims[1]->DrawFrame(1, digits[j], j * 7 + 0x2e, 0x17, 1);
        }
    }
    if (mCellAnims[0] != NULL) {
        mCellAnims[0]->Draw(1, 3, 0x14, 1);
    }
}
