#include "Scene/MyroomScene.hpp"
#include "Unknown/UnkStruct_02078e14.hpp"
#include "Scene/CharacterEditScene.hpp"
#include "Scene/MyroomCharaScene.hpp"
#include "Scene/MyroomGiftScene.hpp"
#include "Scene/MyroomRankingScene.hpp"
#include "Scene/MyroomShootScene.hpp"
#include "Scene/MyroomTipsScene.hpp"
#include "Unknown/UnkStruct_02070608.hpp"
#include "Game/UnkStruct_02065c40.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameTask.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>

extern const char data_ov008_02229da0[];
extern const WindowItemRecord data_ov008_02229d14[];
extern const WindowItemRecord data_ov008_02229c88[];
extern const WindowCellRecord data_ov008_0222877c[];
extern const WindowCellRecord data_ov008_02228808[];
extern const WindowItemRecord data_ov008_02228894[];
extern const WindowCellRecord data_ov008_02228974[];

extern const u8 data_ov008_022286f4[];
extern const u8 data_ov008_022286f8[];
extern const u8 data_ov008_02228714[];
extern const u8 data_ov008_02228764[4][8];
extern const u8 data_ov008_022286fc[];
extern const u8 data_ov008_02228704[];
extern const u8 data_ov008_0222870c[];
extern const Vec2s data_ov008_02228758[];

MyroomScene::MyroomScene() :
    SceneBase(2, 0) {
    mMessages       = NULL;
    mTextBuffer     = NULL;
    mGlyphBuffer    = NULL;
    mTextBufferSize = 0;
    mGlyphCount     = 0;
    mCellAnim       = NULL;
    MI_CpuClear8(&mContext->mInfo, sizeof(SceneInfo));
    mMenus[0] = NULL;
    mMenus[1] = NULL;
    mMenus[2] = NULL;
    mMessage  = NULL;
    data_020d433c.mUnk_14_1 = 0;
    FrameTask::Get().func_0203dc34(0, 0x13);
}

MyroomScene::~MyroomScene() {
    Cleanup();
    FrameTask::Get().func_0203dd90();
}

void MyroomScene::Cleanup() {
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
    delete mMenus[0];
    mMenus[0] = NULL;
    delete mMenus[1];
    mMenus[1] = NULL;
    delete mMenus[2];
    mMenus[2] = NULL;
    delete mMessage;
    mMessage = NULL;
    CellAnimation *cell = mCellAnim;
    if (cell != NULL) {
        delete cell;
    }
    mCellAnim = NULL;
}

void MyroomScene::Update() {
    UnkStruct_SceneContext *ctx = mContext;
    switch (ctx->mInfo.mState) {
        case 0:
            InitDisplay();
            LoadGraphics();
            SetupText();
            mEvent = new UnkStruct_02065c40(1, 0, 0);
            ctx->mInfo.mState++;
            break;
        case 1:
            if (mEvent->func_020678e8()) {
                return;
            }
            mEvent->func_0206ef4c(data_ov008_02229da0, 0, 0xa, 0, 0x15);
            if (mMenus[0] != NULL) {
                mMenus[0]->func_0205c7c8(-1);
                mMenus[0]->mCursor->mSelection = -1;
                mMenus[0]->mCursor->mSelection = *(s8 *) &ctx->mInfo.mUnk_06;
                mMenus[0]->mCells[(u8) (s8) ctx->mInfo.mUnk_06]->SetAnimation(data_ov008_022286fc[(s8) ctx->mInfo.mUnk_06]);
            }
            PrintStatus();
            ctx->mInfo.mState++;
            break;
        case 2:
            if (mEvent->func_020678e8()) {
                return;
            }
            switch (ctx->mInfo.mMenuResult) {
                case 0:
                    mEvent->func_02067998(data_ov008_0222870c[(s8) ctx->mInfo.mUnk_06]);
                    break;
                case 1:
                    mEvent->func_02067998(4);
                    break;
                case 2:
                    mEvent->func_02067998(3);
                    break;
            }
            SpriteManager::Get().StartFade(2);
            ctx->mInfo.mState++;
            break;
        case 3:
            switch (ctx->mInfo.mMenuResult) {
                case 0:
                    mMenus[0]->Draw();
                    break;
                case 1:
                    mMenus[1]->Draw();
                    break;
                case 2:
                    mMenus[2]->Draw();
                    break;
            }
            DrawText();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            ctx->mInfo.mState++;
            break;
        case 4:
            func_ov008_0221c9c8();
            func_ov008_0221c2a0();
            DrawText();
            break;
        case 5:
            if (func_ov008_0221cc6c()) {
                ctx->mInfo.mState = 4;
            }
            DrawText();
            break;
        case 6:
            func_ov008_0221c2a0();
            DrawText();
            SpriteManager::Get().StartSubFade(3);
            switch (ctx->mInfo.mScroll) {
                case 2:
                case 4:
                case 5:
                    mEvent->func_020702e0(0x3c, 8, -606, -26, -4, -656);
                    break;
                case 6:
                    mEvent->func_020702e0(0x76, -52, -619, 0x1f, -64, -669);
                    break;
                case 3:
                    mEvent->func_020702e0(0xe0, 8, -595, 0x92, -39, -657);
                    break;
                case 0:
                case 1:
                case 7:
                    mEvent->func_020702e0(0xb6, 8, -665, 0x60, -4, -715);
                    break;
            }
            ctx->mInfo.mState++;
            break;
        case 7:
            func_ov008_0221c2a0();
            DrawText();
            if (mEvent->func_020678e8()) {
                return;
            }
            SpriteManager::Get().StartFade(3);
            ctx->mInfo.mState++;
            break;
        case 8:
            func_ov008_0221c2a0();
            DrawText();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mEvent->func_020676f8();
            mContext->mUnk_12 = 0;
            ctx->mInfo.mState++;
            break;
        case 9:
            if (ctx->mUnk_12 == 0) {
                return;
            }
            SpriteManager::Get().StartFade(3);
            ctx->mInfo.mState++;
            break;
        case 10:
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            switch (ctx->mInfo.mScroll) {
                case 0:
                    new MyroomGiftScene();
                    break;
                case 1:
                    new MyroomShootScene();
                    break;
                case 2:
                    new MyroomRankingScene();
                    break;
                case 4:
                    new MyroomCharaScene();
                    break;
                case 3:
                    new CharacterEditScene(1);
                    break;
                case 5:
                    new MyroomTipsScene();
                    break;
                case 6:
                    new UnkStruct_02078e14(3, 0);
                    break;
                case 7:
                    new UnkStruct_02078e14(4, 0);
                    break;
            }
            mContext->mUnk_12 = 0;
            Cleanup();
            ctx->mInfo.mState++;
            break;
        case 11:
            if (ctx->mUnk_12 == 0) {
                return;
            }
            ctx->mInfo.mPrevSelection = 0;
            ctx->mInfo.mState         = 0;
            break;
        case 12:
            func_ov008_0221c2a0();
            DrawText();
            mEvent->func_020702e0(0x3a, 0x10, -667, -28, 4, -717);
            ctx->mInfo.mState++;
            break;
        case 13:
            func_ov008_0221c2a0();
            DrawText();
            if (mEvent->func_020678e8()) {
                return;
            }
            new UnkStruct_02070608(1, 1, 0, 0x2000, 2, 0, 0, 0x1e000, 0x1a0);
            mContext->mUnk_12 = 0;
            data_020d433c.mUnk_14_1 = 0;
            ctx->mInfo.mState++;
            break;
        case 14:
            if (ctx->mUnk_12 == 0) {
                return;
            }
            GXS_SetVisiblePlane(0x11);
            ctx->mInfo.mState         = 1;
            ctx->mInfo.mPrevSelection = 0;
            FrameTask::Get().func_0203dc34(0, 0x13);
            break;
        case 15:
            if ((data_020d433c.mUnk_14 & 2) && data_020d433c.mUnk_7f == 4) {
                new UnkStruct_02070608(1, 1, 0, 0x2000, 2, 0, 0, 0x1e000, 0x1a0);
                mContext->mUnk_12 = 0;
                ctx->mInfo.mState++;
            } else {
                ctx->mInfo.mState = 0x11;
                mMenus[0]->Draw();
                DrawText();
            }
            break;
        case 16:
            if (ctx->mUnk_12 != 0) {
                ctx->mInfo.mState++;
            }
            break;
        case 17:
            SpriteManager::Get().StartFade(3);
            ctx->mInfo.mState++;
            mMenus[0]->Draw();
            DrawText();
            break;
        case 18:
            mMenus[0]->Draw();
            DrawText();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mEvent->func_020676f8();
            mContext->mUnk_12 = 0;
            ctx->mInfo.mState++;
            break;
        case 19:
            if (ctx->mUnk_12 != 0) {
                ctx->mInfo.mState++;
            }
            break;
        case 20:
            RequestScene(data_020d433c.mUnk_7f);
            mFinished = 1;
            break;
    }
}

void MyroomScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 7);
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

void MyroomScene::LoadGraphics() {
    ChangeDir("/Game/Myroom/");
    func_02025b64(1, 7, 0, "myroom_b_bg_01_nsc.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "myroom_b_bg_01_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "myroom_b_bg_01_ncl.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.func_02027624(3);
    sub.LoadCharacters("myroom_b_obj_00.nce_ncg1D.bin", 0x5000, 0);
    sub.LoadPalette("myroom_b_obj_00_ncl.bin", 0x40, 0);
    mCellAnim            = new CellAnimation("myroom_b_obj_00_nce.bin", 0, 1, 1);
    mCellAnim->mCharBase = sub.func_020276f4(0x5000);
    mCellAnim->mUnk_11   = 2;
    ChangeDir("/Game/2D/union_b/");
    SpriteScreen &sub2 = SpriteManager::Get().Screen(1);
    sub2.func_02027624(3);
    sub2.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0, 0);
    sub2.LoadPalette("union_b_obj_cockpit_ncl.bin", 0, 0);
    sub2.LoadCharacters("union_b_obj_arrow.nce_ncg1D.bin", 0x4000, 0);
    sub2.LoadPalette("union_b_obj_arrow_ncl.bin", 0x20, 0);
    ChangeDir("/");
    UnkStruct_SceneContext *ctx = mContext;
    s32 selection1              = 0;
    if (ctx->mInfo.mMenuResult == 1) {
        selection1 = (s8) ctx->mInfo.mUnk_06;
    }
    mMenus[1] = new SelectWindow(5, data_ov008_02229d14, selection1);
    mMenus[1]->SetCells(5, data_ov008_0222877c);
    mMenus[1]->mCursor->mSelection = -1;
    mMenus[1]->mCursor->mSelection = selection1;
    s32 selection2                 = 0;
    if (ctx->mInfo.mMenuResult == 2) {
        selection2 = (s8) ctx->mInfo.mUnk_06;
    }
    mMenus[2] = new SelectWindow(5, data_ov008_02229c88, selection2);
    mMenus[2]->SetCells(5, data_ov008_02228808);
    mMenus[2]->mCursor->mSelection = -1;
    mMenus[2]->mCursor->mSelection = selection2;
    SwitchPage(mContext->mInfo.mMenuResult);
}

void MyroomScene::SwitchPage(u8 page) {
    UnkStruct_SceneContext *ctx = mContext;
    if (ctx->mInfo.mMenuResult != page) {
        switch (ctx->mInfo.mMenuResult) {
            case 1:
                ctx->mInfo.mUnk_06 = 0;
                ctx->mInfo.mUnk_0a = 0;
                break;
            case 2:
                ctx->mInfo.mUnk_06 = 4;
                ctx->mInfo.mUnk_0a = 4;
                break;
        }
        ctx->mInfo.mMenuResult = page;
        func_ov008_0221d2c0();
    }
    switch (ctx->mInfo.mMenuResult) {
        case 0:
            delete mMenus[0];
            mMenus[0] = new SelectWindow(8, data_ov008_02228894, (s8) ctx->mInfo.mUnk_06);
            mMenus[0]->SetCells(8, data_ov008_02228974);
            mMenus[0]->mCursor->mSelection = -1;
            ctx->mInfo.mPrevSelection      = 0;
            break;
        case 1:
            delete mMenus[1];
            mMenus[1] = new SelectWindow(5, data_ov008_02229d14, -1);
            mMenus[1]->SetCells(5, data_ov008_0222877c);
            mMenus[1]->mCursor->mSelection = -1;
            mMenus[1]->mCursor->mSelection = *(s8 *) &ctx->mInfo.mUnk_06;
            break;
        case 2:
            delete mMenus[2];
            mMenus[2] = new SelectWindow(5, data_ov008_02229c88, -1);
            mMenus[2]->SetCells(5, data_ov008_02228808);
            mMenus[2]->mCursor->mSelection = -1;
            mMenus[2]->mCursor->mSelection = *(s8 *) &ctx->mInfo.mUnk_06;
            break;
    }
    func_ov008_0221cea0();
}

// non-matching: size 0x61c vs 0x610: register allocation of first/last/count/window differs (r4/r7/r8/r9 vs sl/r4/r7/r8)
void MyroomScene::func_ov008_0221c2a0() {
    UnkStruct_SceneContext *ctx = mContext;
    u8 first                    = 0;
    u8 last                     = 0;
    u8 count                    = 0;
    SelectWindow *window        = NULL;
    ctx->mInfo.mSelection       = *(s8 *) &ctx->mInfo.mUnk_06;
    switch (ctx->mInfo.mMenuResult) {
        case 0:
            first  = 6;
            window = mMenus[0];
            last   = 7;
            count  = 5;
            break;
        case 1:
            count  = 3;
            window = mMenus[1];
            last   = 2;
            break;
        case 2:
            count  = 3;
            window = mMenus[2];
            last   = 2;
            break;
    }
    if (window == NULL) {
        return;
    }
    if (ctx->mInfo.mState == 4) {
        u8 hit    = window->Update(TRUE);
        s32 delta = 0;
        if ((u8) ctx->mInfo.mPrevSelection != 2) {
            ctx->mInfo.mPrevSelection = hit;
        }
        switch (ctx->mInfo.mMenuResult) {
            case 0:
                switch ((u8) ctx->mInfo.mPrevSelection) {
                    case 1:
                    case 2:
                        break;
                    case 3: {
                        u8 id = window->GetSelectedId();
                        if (id == first) {
                            delta = -1;
                        } else if (id == last) {
                            delta = 1;
                        }
                    }
                    // fall through
                    default:
                        if (delta == 0 && window->mUnk_061 == 1) {
                            if (InputManager::Get().IsTrigger(0x20)) {
                                if ((s8) ctx->mInfo.mUnk_06 != 0) {
                                    window->func_0205c844(first, 3);
                                    SoundManager::Get().func_020246f8(0);
                                }
                            } else if (InputManager::Get().IsTrigger(0x10)) {
                                if ((s8) ctx->mInfo.mUnk_06 < count - 1) {
                                    window->func_0205c844(last, 3);
                                    SoundManager::Get().func_020246f8(0);
                                }
                            }
                        }
                        break;
                }
                if (delta != 0) {
                    ctx->mInfo.mPage = delta > 0 ? 2 : 1;
                    ctx->mInfo.mUnk_0a += delta;
                    func_ov008_0221c8b0();
                    ctx->mInfo.mPrevSelection = 0;
                }
                break;
            case 1:
            case 2:
                switch ((u8) ctx->mInfo.mPrevSelection) {
                    case 1:
                    case 2:
                        break;
                    case 3: {
                        u8 id = window->GetSelectedId();
                        if (id >= first && id <= last) {
                            delta = (s8) (id - (s8) ctx->mInfo.mUnk_06 - first);
                        }
                    }
                    // fall through
                    default:
                        if (delta == 0) {
                            u8 n = last - first + 1;
                            u8 k = 0;
                            do {
                                if (TouchManager::Get().func_02022860(data_ov008_02228758[k].x, data_ov008_02228758[k].y, 0x38,
                                                                      0x30)) {
                                    s8 selection = (s8) ctx->mInfo.mUnk_06;
                                    if (k != selection) {
                                        delta = (s8) (k - selection - first);
                                    } else if (window->mUnk_061 == 1) {
                                        window->func_0205c844((u8) selection, 3);
                                        SoundManager::Get().func_020246f8(0);
                                    }
                                }
                                k++;
                            } while (k < n);
                            if (window->mUnk_061 == 1) {
                                if (InputManager::Get().IsTrigger(0x20)) {
                                    s8 selection = (s8) ctx->mInfo.mUnk_06;
                                    if (selection > first) {
                                        delta = -1;
                                    } else {
                                        delta = (s8) (last - selection - first);
                                    }
                                } else if (InputManager::Get().IsTrigger(0x10)) {
                                    s8 selection = (s8) ctx->mInfo.mUnk_06;
                                    if (selection < last) {
                                        delta = 1;
                                    } else {
                                        delta = (s8) -selection;
                                    }
                                }
                            }
                        }
                        break;
                }
                if (delta != 0) {
                    ctx->mInfo.mUnk_06 = (s8) ctx->mInfo.mUnk_06 + delta;
                    window->mCursor->mSelection = (s8) ctx->mInfo.mUnk_06 + first;
                    PrintStatus();
                    ctx->mInfo.mPrevSelection = 0;
                    SoundManager::Get().func_020246f8(0);
                }
                break;
        }
    }
    window->Draw();
    switch (ctx->mInfo.mMenuResult) {
        case 1:
        case 2:
            mCellAnim->Update();
            s8 cur = ctx->mInfo.mUnk_06;
            mCellAnim->Draw(1, data_ov008_02228758[cur].x, data_ov008_02228758[cur].y, 0);
            break;
    }
}

void MyroomScene::func_ov008_0221c8b0() {
    UnkStruct_SceneContext *ctx = mContext;
    ctx->mInfo.mState           = 5;
    ctx->mInfo.mScrollDir       = 0x62;
    if (ctx->mInfo.mMenuResult != 0) {
        return;
    }
    SelectWindow *window = mMenus[0];
    u8 i;
    WindowItem *items = window->mCursor->mItems;
    for (i = 0; i < 5; i++) {
        if (ctx->mInfo.mUnk_0a == i) {
            window->mCells[i]->func_02029f6c();
            window->mCells[i]->SetAnimation(data_ov008_022286fc[i]);
        } else if ((s8) ctx->mInfo.mUnk_06 == i) {
            window->mCells[i]->func_02029fb8(2);
        }
        items[i].mFlags = 0x8000;
    }
    if (ctx->mInfo.mUnk_0a == 0) {
        window->mButtons[6].mEnabled = 0;
        items[6].mFlags              = 0x8000;
    }
    if (ctx->mInfo.mUnk_0a == 4) {
        window->mButtons[7].mEnabled = 0;
        items[7].mFlags              = 0x8000;
    }
    if (ctx->mInfo.mMenuResult != 0) {
        return;
    }
    if ((s32) ctx->mInfo.mUnk_0a >= 3) {
        window->mButtons[0].mEnabled = 0;
    } else {
        window->mButtons[0].mEnabled = 1;
    }
    if (ctx->mInfo.mUnk_0a >= 2) {
        window->mButtons[4].mEnabled = 1;
    } else {
        window->mButtons[4].mEnabled = 0;
    }
}

void MyroomScene::func_ov008_0221c9c8() {
    UnkStruct_SceneContext *ctx = mContext;
    switch ((u8) ctx->mInfo.mPrevSelection) {
        case 2:
            switch (ctx->mInfo.mMenuResult) {
                case 0:
                    ctx->mInfo.mState = 0xf;
                    break;
                case 1:
                    if (mMenus[1]->mCells[3]->mUnk_12 == 0) {
                        return;
                    }
                    SwitchPage(0);
                    mMenus[1]->func_0205c7c8(0);
                    break;
                case 2:
                    if (mMenus[2]->mCells[3]->mUnk_12 == 0) {
                        return;
                    }
                    SwitchPage(0);
                    mMenus[2]->func_0205c7c8(0);
                    break;
            }
            break;
        case 1:
        case 3: {
            s8 selection = (s8) ctx->mInfo.mUnk_06;
            if (selection != 0 && ctx->mInfo.mSelection != selection) {
                return;
            }
            switch (ctx->mInfo.mMenuResult) {
                case 0:
                    switch (selection) {
                        case 0:
                            ctx->mInfo.mUnk_06 = 0;
                            ctx->mInfo.mUnk_0a = 0;
                            SwitchPage(1);
                            mMenus[1]->mCells[4]->func_02029f6c();
                            break;
                        case 1:
                            ctx->mInfo.mState = 0xc;
                            break;
                        case 2:
                            ctx->mInfo.mScroll = 6;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 3:
                            ctx->mInfo.mScroll = 3;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 4:
                            ctx->mInfo.mUnk_06 = 0;
                            ctx->mInfo.mUnk_0a = 0;
                            SwitchPage(2);
                            mMenus[2]->mCells[4]->func_02029f6c();
                            break;
                        case 5:
                            ctx->mInfo.mState = 0xf;
                            break;
                    }
                    break;
                case 1:
                    switch (selection) {
                        case 0:
                            ctx->mInfo.mScroll = 5;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 1:
                            ctx->mInfo.mScroll = 2;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 2:
                            ctx->mInfo.mScroll = 4;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 3:
                            if (mMenus[1]->mCells[3]->mUnk_12 == 0) {
                                return;
                            }
                            SwitchPage(0);
                            mMenus[1]->func_0205c7c8(0);
                            break;
                    }
                    break;
                case 2:
                    switch (selection) {
                        case 0:
                            ctx->mInfo.mScroll = 7;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 1:
                            ctx->mInfo.mScroll = 1;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 2:
                            ctx->mInfo.mScroll = 0;
                            ctx->mInfo.mState  = 6;
                            break;
                        case 3:
                            if (mMenus[2]->mCells[3]->mUnk_12 == 0) {
                                return;
                            }
                            SwitchPage(0);
                            mMenus[2]->func_0205c7c8(0);
                            break;
                    }
                    break;
            }
            break;
        }
    }
}

BOOL MyroomScene::func_ov008_0221cc6c() {
    UnkStruct_SceneContext *ctx = mContext;
    BOOL done;
    SelectWindow *window;
    WindowItem *items;
    s32 step;
    step = 0xe;
    done = FALSE;
    if (ctx->mInfo.mScrollDir < 0xe) {
        step = ctx->mInfo.mScrollDir;
    }
    ctx->mInfo.mScrollDir -= step;
    if (ctx->mInfo.mPage == 2) {
        step = (s16) -step;
    }
    switch (ctx->mInfo.mMenuResult) {
        case 0:
            window = mMenus[0];
            break;
        default:
            return FALSE;
    }
    if (window->mUnk_061 != 1) {
        window->Update(TRUE);
    }
    items = window->mCursor->mItems;
    for (u8 i = 0; i < 5; i++) {
        Vec2s pos = window->GetCellPos(i);
        pos.x += step;
        window->SetCellPos(i, pos);
        items[i].mX = step + items[i].mX;
    }
    if (ctx->mInfo.mScrollDir == 0) {
        window->mCells[(u8) (s8) ctx->mInfo.mUnk_06]->SetAnimation(data_ov008_02228704[(s8) ctx->mInfo.mUnk_06]);
        items[ctx->mInfo.mUnk_0a].mFlags     = 8;
        items[(s8) ctx->mInfo.mUnk_06].mFlags     = 0x8000;
        window->mCursor->mSelection          = ctx->mInfo.mUnk_0a;
        ctx->mInfo.mUnk_06                   = ctx->mInfo.mUnk_0a;
        mEvent->func_0206f564(data_ov008_02228764[ctx->mInfo.mMenuResult][(s8) ctx->mInfo.mUnk_06], 0xa, -1);
        if (ctx->mInfo.mMenuResult == 0) {
            mEvent->func_02067998(data_ov008_0222870c[(s8) ctx->mInfo.mUnk_06]);
        }
        if ((s8) ctx->mInfo.mUnk_06 != 0) {
            window->mButtons[6].mEnabled = 1;
            items[6].mFlags              = 8;
        }
        if ((s8) ctx->mInfo.mUnk_06 != 4) {
            window->mButtons[7].mEnabled = 1;
            items[7].mFlags              = 8;
        }
        window->mCells[6]->func_02029fc0();
        window->mCells[7]->func_02029fc0();
        if (ctx->mInfo.mPage == 1) {
            window->mCells[6]->SetAnimation(0);
        } else {
            window->mCells[7]->SetAnimation(1);
        }
        PrintStatus();
        done = TRUE;
    }
    window->Draw();
    return done;
}

void MyroomScene::func_ov008_0221cea0() {
    UnkStruct_SceneContext *ctx = mContext;
    s16 offset                  = (s8) ctx->mInfo.mUnk_06 * 0x62;
    if (ctx->mInfo.mMenuResult != 0) {
        return;
    }
    SelectWindow *window  = mMenus[0];
    WindowItem *items     = window->mCursor->mItems;
    for (u8 i = 0; i < 5; i++) {
        Vec2s pos = window->mCellPos[i];
        pos.x = 0x62 * i + 0x58 - offset;
        window->SetCellPos(i, pos);
        items[i].mX = pos.x;
        if ((s8) ctx->mInfo.mUnk_06 == i) {
            window->mCells[i]->SetAnimation(data_ov008_022286fc[i]);
            items[i].mFlags = 8;
        } else {
            window->mCells[i]->SetAnimation(data_ov008_02228704[i]);
            items[i].mFlags = 0x8000;
        }
        window->mCells[i]->func_02029f6c();
    }
    if ((s8) ctx->mInfo.mUnk_06 != 0) {
        window->mButtons[6].mEnabled = 1;
        items[6].mFlags              = 8;
    } else {
        window->mButtons[6].mEnabled = 0;
        items[6].mFlags              = 0x8000;
    }
    if ((s8) ctx->mInfo.mUnk_06 == 4) {
        window->mButtons[7].mEnabled = 0;
        items[7].mFlags              = 0x8000;
    } else {
        window->mButtons[7].mEnabled = 1;
        items[7].mFlags              = 8;
    }
    if (ctx->mInfo.mMenuResult == 0) {
        if ((s32) ctx->mInfo.mUnk_0a >= 3) {
            window->mButtons[0].mEnabled = 0;
        } else {
            window->mButtons[0].mEnabled = 1;
        }
        if (ctx->mInfo.mUnk_0a >= 2) {
            window->mButtons[4].mEnabled = 1;
        } else {
            window->mButtons[4].mEnabled = 0;
        }
    }
    window->mCursor->mSelection = *(s8 *) &ctx->mInfo.mUnk_06;
}

void MyroomScene::SetupText() {
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mTextBuffer != NULL) {
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    mMessages       = (MessageFile *) func_02023504("/font/mes_myroom.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x20, 2);
    mGlyphCount     = TextManager::Get().GlyphCount(8);
    mTextBuffer     = OS_Alloc(mTextBufferSize);
    ClearTextBuffer();
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x20, 2, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    mMessage = new MessageWindow("mes_myroom", 1, 0x1800);
    mMessage->func_02065bd4(0x14, 4, 0);
    mMessage->func_02065be4(0xd8);
    switch (mContext->mInfo.mMenuResult) {
        case 0:
            mMessage->func_02065890(0x24, 0, 1);
            break;
        case 1:
            mMessage->func_02065890(0x25, 0, 1);
            break;
        case 2:
            mMessage->func_02065890(0x26, 0, 1);
            break;
    }
}

void MyroomScene::func_ov008_0221d2c0() {
    func_ov008_0221d450();
    UnkStruct_SceneContext *ctx = mContext;
    switch (ctx->mInfo.mMenuResult) {
        case 0:
            mMessage->func_02065890(0x24, 0, 1);
            break;
        case 1:
            mMessage->func_02065890(0x25, 0, 1);
            break;
        case 2:
            mMessage->func_02065890(0x26, 0, 1);
            break;
    }
    PrintStatus();
    s8 selection = (s8) ctx->mInfo.mUnk_06;
    if (selection < 0) {
        return;
    }
    mEvent->func_0206f564(data_ov008_02228764[ctx->mInfo.mMenuResult][selection], 0xa, -1);
}

void MyroomScene::DrawText() {
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

void MyroomScene::func_ov008_0221d450() {
    if (mMessage != NULL) {
        mMessage->func_02065878();
    }
}

void MyroomScene::PrintStatus() {
    u8 index = 0;
    SceneInfo *info = &mContext->mInfo;
    switch (info->mMenuResult) {
        case 0:
            index = data_ov008_02228714[(s8) info->mUnk_06];
            break;
        case 1:
            index = data_ov008_022286f4[(s8) info->mUnk_06];
            break;
        case 2:
            index = data_ov008_022286f8[(s8) info->mUnk_06];
            break;
    }
    ClearTextBuffer();
    TextManager::Get().mUnk_5b = 1;
    TextManager &text          = TextManager::Get();
    s16 height;
    s16 width;
    width              = 0;
    height             = 0;
    MessageFile *table = mMessages;
    text.Print(table->GetText(index), table->GetLength(index), mTextBuffer, 0x20, 2, &height, &width, &info->mUnk_00, 0, 2,
               0x21, 0xe, 0, 0, 0, 1, 1);
    TextManager::Get().mUnk_5b = 0;
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x3000, mTextBuffer, mTextBufferSize, 0);
}

void MyroomScene::ClearTextBuffer() {
    if (mTextBuffer != NULL) {
        MI_CpuClear16(mTextBuffer, mTextBufferSize);
    }
}
