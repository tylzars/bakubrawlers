#include "System/InputManager.hpp"
#include "Scene/MyroomTipsScene.hpp"
#include "Game/UnkStruct_02065c40.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
extern u8 data_020d499c[];
extern u8 data_020d49a1[];
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

extern const s8 data_ov008_02229650[6];
extern const u8 data_ov008_02229660[6];

#define TEST_BIT(flags, index) ((flags)[(index) / 8] & (1 << ((index) % 8)))
#define SET_BIT(flags, index) ((flags)[(index) / 8] |= 1 << ((index) % 8))

// One row per tips category: each entry holds the text id shown in the list and the event message it opens.
struct TipsEntry {
    s8 mText;
    s8 mMessage;
};
extern const TipsEntry data_ov008_022296ac[][10];

extern const u8 data_ov008_02229668[][2];

extern const WindowItemRecord data_ov008_02229674[];
extern const WindowCellRecord data_ov008_02229690[];
extern const WindowItemRecord data_ov008_02229724[];
extern const WindowCellRecord data_ov008_022297b0[];

extern "C" void func_020705f0(UnkStruct_02065c40 *event);
extern "C" void func_0206f88c(UnkStruct_02065c40 *event, u32 category, s32 value, u32 unk4);
extern const s8 data_ov008_02229658[];

extern "C" void func_0206f6f0(UnkStruct_02065c40 *event, const char *name, u32 id, u32 unk3, u32 unk4);

MyroomTipsScene::MyroomTipsScene() :
    SceneBase(2, 0) {
    mUnk_6c = 0;
    mFlags  = 0;
    mScroll = 0;
    mRow    = 0;
    mCount  = 5;
    mUnk_85 = 0;
    MI_CpuFill8(&mContext->mInfo, 0, 5);
    MI_CpuClear16(mUnk_78, 8);
    MI_CpuClear16(mUnk_70, 8);
    MI_CpuFill8(mTimers, 0x3c, 4);
    mWindowA = NULL;
    mWindowB = NULL;
    mEvent   = NULL;
    mCellB   = NULL;
    mMessage = NULL;
    mCellA   = NULL;
    u8 i;
    for (i = 0; i < 6; i++) {
        mCells[i] = NULL;
    }
    for (i = 0; i < 2; i++) {
        mRowCells[i] = NULL;
    }
    for (i = 0; i < 2; i++) {
        mNewCells[i] = NULL;
    }
    mMessages    = NULL;
    mGlyphBuffer = NULL;
    for (i = 0; i < 4; i++) {
        mTextBuffers[i] = NULL;
    }
    for (i = 0; i < 4; i++) {
        mIndices[i] = 0;
    }
    func_ov008_02228464();
}

MyroomTipsScene::~MyroomTipsScene() {
    delete mWindowA;
    delete mWindowB;
    CellAnimation *cellB = mCellB;
    if (cellB != NULL) {
        delete cellB;
    }
    delete mMessage;
    CellAnimation *cellA = mCellA;
    if (cellA != NULL) {
        delete cellA;
    }
    for (u8 i = 0; i < 6; i++) {
        CellAnimation *cell = mCells[i];
        if (cell != NULL) {
            delete cell;
        }
    }
    {
        CellAnimation *cell;
        for (u8 j = 0; j < 2; j++) {
            cell = mRowCells[j];
            if (cell != NULL) {
                delete cell;
            }
        }
    }
    {
        CellAnimation *cell;
        for (u8 j = 0; j < 2; j++) {
            cell = mNewCells[j];
            if (cell != NULL) {
                delete cell;
            }
        }
    }
    if (mMessages != NULL) {
        OS_Free(mMessages);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
    }
    for (u8 i = 0; i < 4; i++) {
        if (mTextBuffers[i] != NULL) {
            OS_Free(mTextBuffers[i]);
        }
    }
}

void MyroomTipsScene::Update() {
    UnkStruct_SceneContext *context = mContext;
    u8 state                        = context->mInfo.mStep;
    switch (state) {
        case 0:
            InitDisplay();
            func_ov008_022267f4();
            func_ov008_022283a8();
            mEvent = new UnkStruct_02065c40(5, 0, 1);
            GXS_SetVisiblePlane(0x13);
            SpriteManager::Get().StartFade(2);
            context->mInfo.mStep++;
            return;
        case 1:
            if (!SpriteManager::Get().IsFading()) {
                context->mInfo.mStep = 4;
            }
            Draw();
            return;
        case 2:
            if (SpriteManager::Get().sFade[0] <= 1) {
                mEvent->func_020676f8();
                context->mInfo.mStep++;
            }
            Draw();
            return;
        case 3:
            mEvent = new UnkStruct_02065c40(5, 0, 1);
            func_ov008_0222840c(0x2d);
            SpriteManager::Get().sFade[0] = 2;
            context->mInfo.mStep++;
            Draw();
            return;
        case 4:
            if (SpriteManager::Get().sFade[0] <= 1) {
                if (!mEvent->func_020678e8()) {
                    u32 id = 0x33;
                    if (mUnlocked[0] == 0) {
                        id = 0x34;
                    }
                    func_0206f6f0(mEvent, "mes_tips_Dan", id, 10, 0x15);
                    SpriteManager::Get().sFade[0] = 2;
                    context->mInfo.mStep             = 5;
                }
            }
            Draw();
            return;
        case 5:
            if (SpriteManager::Get().sFade[0] <= 1) {
                mWindowA->mUnk_061 = 4;
                mWindowA->func_0205c7c8(-1);
                mFlags = 1;
                context->mInfo.mStep++;
            }
            Draw();
            return;
        case 6:
            GXS_SetVisiblePlane(0x13);
            Draw();
            context->mInfo.mStep++;
            return;
        case 7:
            func_ov008_022274f8();
            Draw();
            return;
        case 8:
            if (!mEvent->func_020678e8()) {
                mWindowB->mUnk_061 = 4;
                mWindowB->func_0205c7c8(-1);
                mWindowB->mCells[0]->SetAnimation(2);
                MI_CpuClear16(mUnk_78, 8);
                MI_CpuClear16(mUnk_70, 8);
                MI_CpuFill8(mTimers, 0x3c, 4);
                func_ov008_0222840c(0x2f);
                mFlags = 2;
                context->mInfo.mStep++;
            }
            Draw();
            return;
        case 9: {
            u8 i = 0;
            mRow    = 0;
            mScroll = 0;
            do {
                mIndices[i] = func_ov008_0222852c((u8) (mScroll + i));
                i++;
            } while (i < 4);
            func_ov008_02228214(-1);
            GXS_SetVisiblePlane(0x11);
            mWindowB->mCursor->SetScroll(mScroll);
            SelectWindow *window  = mWindowB;
            u8 y                  = window->mCursor->mUnk_0b;
            window->mCellPos[1].x = 0xe4;
            window->mCellPos[1].y = y;
            mUnk_85               = 0;
            context->mInfo.mStep++;
            Draw();
            return;
        }
        case 10:
            func_ov008_022285ac();
            func_ov008_022274f8();
            Draw();
            return;
        case 11:
            if (mEvent->mUnk_14f1 == 0) {
                context->mInfo.mStep = state + 1;
                Draw();
            }
        case 12:
            if (!mEvent->func_020678e8()) {
                context->mInfo.mStep = 10;
                MI_CpuClear16(mUnk_78, 8);
                MI_CpuClear16(mUnk_70, 8);
                MI_CpuFill8(mTimers, 0x3c, 4);
                func_ov008_02228214(-1);
            }
            func_ov008_022274f8();
            Draw();
            return;
        case 13:
            context->mInfo.mStep = state + 1;
            SpriteManager::Get().StartFade(3);
            GXS_SetVisiblePlane(0x11);
            return;
        case 14:
            if (!SpriteManager::Get().IsFading()) {
                mEvent->func_020676f8();
                mContext->mUnk_12 = 0;
                context->mInfo.mStep++;
            }
            return;
        case 15:
            if (context->mUnk_12 != 0) {
                context->mInfo.mStep = state + 1;
            }
            return;
        case 16:
            RequestScene(1);
            mFinished = TRUE;
            return;
    }
}

void MyroomTipsScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    GXS_SetVisiblePlane(0x11);
    G2S_SetBlendBrightness(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_BlendNone();
}

void MyroomTipsScene::func_ov008_022267f4() {
    ChangeDir("/Game/Myroom/");
    func_02025b64(1, 1, 0, "myroom_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "myroom_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "myroom_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "myroom_tips_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "myroom_tips_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "myroom_tips_b_bg_00_ncg.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    sub.func_02027624(3);
    ChangeDir("/Game/Myroom/");
    sub.LoadCharacters("myroom_tips_obj_00.nce_ncg1D.bin", 0x2000, 0);
    sub.LoadPalette("myroom_tips_obj_00_ncl.bin", 0, 0);
    mCellA            = new CellAnimation("myroom_tips_obj_00_nce.bin", 0, 1, 1);
    mCellA->mCharBase = sub.func_020276f4(0x2000);
    mCellA->mUnk_11   = 0;
    for (u8 i = 0; i < 6; i++) {
        mCells[i]            = new CellAnimation("myroom_tips_obj_00_nce.bin", 0, 1, 1);
        mCells[i]->mCharBase = sub.func_020276f4(0x2000);
        mCells[i]->mUnk_11   = 0;
        mCells[i]->SetAnimation(i + 2);
    }
    mCellB            = new CellAnimation("myroom_tips_obj_00_nce.bin", 0, 1, 1);
    mCellB->mCharBase = sub.func_020276f4(0x2000);
    mCellB->mUnk_11   = 0;
    mCellB->SetAnimation(1);
    mNewCells[0]            = new CellAnimation("myroom_tips_obj_00_nce.bin", 8, 1, 1);
    mNewCells[0]->mCharBase = sub.func_020276f4(0x2000);
    mNewCells[0]->mUnk_11   = 0;
    ChangeDir("/Game/Tutorial/obj/");
    sub.LoadCharacters("tutorial_b_obj_00.nce_ncg1D.bin", 0x5000, 0);
    sub.LoadPalette("tutorial_b_obj_00_ncl.bin", 0xa0, 0);
    for (u8 i = 0; i < 2; i++) {
        mRowCells[i]            = new CellAnimation("tutorial_b_obj_00_nce.bin", 0, 1, 1);
        mRowCells[i]->mCharBase = sub.func_020276f4(0x5000);
        mRowCells[i]->mUnk_11   = 5;
    }
    mRowCells[0]->SetAnimation(1);
    mRowCells[1]->SetAnimation(2);
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_scroll.nce_ncg1D.bin", 0x7000, 0);
    sub.LoadPalette("union_b_obj_scroll_ncl.bin", 0x80, 0);
    sub.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0, 0);
    sub.LoadCharacters("union_b_obj_new.nce_ncg1D.bin", 0xc000, 0);
    sub.LoadPalette("union_b_obj_new_ncl.bin", 0x100, 0);
    mNewCells[1]            = new CellAnimation("union_b_obj_new_nce.bin", 0, 1, 1);
    mNewCells[1]->mCharBase = sub.func_020276f4(0xc000);
    mNewCells[1]->mUnk_11   = 8;
    ChangeDir("/");
    mWindowA = new SelectWindow(1, data_ov008_02229674, 0);
    mWindowA->SetCells(1, data_ov008_02229690);
    mWindowA->mCursor->mSelection = -1;
    mWindowA->mUnk_064            = 0;
    mWindowB                      = new SelectWindow(5, data_ov008_02229724, -1);
    mWindowB->SetCells(5, data_ov008_022297b0);
    mWindowB->mCursor->mSelection = -1;
    mWindowB->mCursor->mSelection = mScroll;
    mWindowB->mCursor->func_02030474(0x30, 0x90, 4, mCount);
    mWindowB->mUnk_064 = 0;
    mRow               = 0;
    func_ov008_02226e00();
    mFlags = 1;
}

void MyroomTipsScene::func_ov008_02226e00() {
    ChangeDir("/font/");
    mMessages = (MessageFile *) func_02023504("mes_TIPS_sys.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x30, 2);
    mGlyphCount     = TextManager::Get().GlyphCount(0x30, 2, 0x11);
    mUnk_6c         = mGlyphCount / 2;
    for (u8 i = 0; i < 4; i++) {
        mTextBuffers[i] = OS_Alloc(mTextBufferSize);
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
    }
    mGlyphBuffer = OS_Alloc(mGlyphCount * 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x30, 2, 0x11);
    TextManager::Get().SetColor(1, 0, 0xf);
    func_ov008_02228214(-1);
}

// non-matching: in the first loop's Draw call the two data_ov008_02229668 byte loads come x then y; the original loads y
// first (2 words; same source shape gives either order in func_ov008_022271f8)
void MyroomTipsScene::Draw() {
    UnkStruct_SceneContext *context = mContext;
    func_ov008_022271f8();
    func_ov008_022273f0();
    DrawMessage();
    if (mFlags & 1) {
        mNewCells[0]->Update();
        BOOL show = TRUE;
        if (!data_020d43f0[0].mUnk_5bf_0 && data_020d43f0[0].mUnk_42 == 6) {
            show = FALSE;
        }
        BOOL first = (show == FALSE);
        u8 i       = first;
        do {
            if (mNew[i] != 0) {
                mNewCells[0]->Draw(1, data_ov008_02229668[i][0], data_ov008_02229668[i][1], 1);
            }
            i++;
        } while (i < 6);
    }
    if (!(mFlags & 2)) {
        return;
    }
    mNewCells[1]->Update();
    u8 j = 0;
    do {
        SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, mUnk_6c, 0x26, (s16) (j * 32 + 0x29), 1,
                                                     (u16) ((j * 3 * 0x400 + 0x8000) >> 7), 0xf, 0);
        if (!TEST_BIT(data_020d49a1, data_ov008_02229660[(s8) context->mInfo.mMenuResult] + mIndices[j])) {
            mNewCells[1]->Draw(1, 0x20, (s16) (j * 32 + 0x20), 1);
        }
        j++;
    } while (j < 4);
}

void MyroomTipsScene::func_ov008_022271f8() {
    UnkStruct_SceneContext *context = mContext;
    if (mFlags & 1) {
        BOOL drawn = FALSE;
        mCellA->Update();
        if (!data_020d43f0[0].mUnk_5bf_0 && data_020d43f0[0].mUnk_42 == 6 && mUnlocked[0] != 0) {
            mCellA->Draw(1, 0x14, 0x22, 2);
            drawn = TRUE;
        }
        u8 i = 0;
        if (drawn) {
            i = 1;
        }
        do {
            if (mUnlocked[i] == 0) {
                mCellA->Draw(1, data_ov008_02229668[i][0], data_ov008_02229668[i][1], 2);
            } else {
                mCells[i]->Update();
                mCells[i]->Draw(1, data_ov008_02229668[i][0], data_ov008_02229668[i][1], 2);
            }
            i++;
        } while (i < 6);
        if (mWindowA != NULL) {
            if (!(InputManager::Get().IsDown(1))) {
                context->mInfo.mPrevSelection = mWindowA->Update(TRUE);
            }
            mWindowA->Draw();
        }
    }
    if (!(mFlags & 2)) {
        return;
    }
    if (mUnk_85 == 0) {
        if (!(InputManager::Get().IsDown(1))) {
            context->mInfo.mPrevSelection = mWindowB->Update(TRUE);
        }
    }
    mWindowB->Draw();
}

void MyroomTipsScene::func_ov008_022273f0() {
    UnkStruct_SceneContext *context = mContext;
    if (mFlags & 1) {
        func_ov008_022274a4(data_ov008_02229668[0], context->mInfo.mMenuResult);
    }
    if (!(mFlags & 2)) {
        return;
    }
    mRowCells[0]->Update();
    mRowCells[1]->Update();
    for (u8 i = 0; i < 4; i++) {
        u8 cell = 0;
        if (i == mRow) {
            cell = 1;
        }
        mRowCells[cell]->Draw(1, 0x20, (s16) (i * 32 + 0x20), 2);
    }
}

void MyroomTipsScene::func_ov008_022274a4(const u8 *pos, u32 index) {
    if (mCellB == NULL) {
        return;
    }
    mCellB->Update();
    mCellB->Draw(1, pos[index * 2], pos[index * 2 + 1], 2);
}

void MyroomTipsScene::func_ov008_022274f8() {
    if (mEvent->func_020678e8()) {
        mWindowA->mUnk_064 = 0;
        mWindowB->mUnk_064 = 0;
        return;
    }
    UnkStruct_SceneContext *context = mContext;
    if (mFlags & 1) {
        mWindowA->mUnk_064 = 1;
        mWindowB->mUnk_064 = 0;
        if (mWindowA != NULL) {
            switch (*(u8 *) &context->mInfo.mPrevSelection) {
                case 2:
                    context->mInfo.mStep = 0xd;
                    break;
                case 3:
                    break;
            }
        }
        if (context->mInfo.mState == 0) {
            s32 selection = func_ov008_02227a08(data_ov008_02229668[0], 6);
            if (selection >= 0) {
                s8 category = context->mInfo.mMenuResult;
                if (category != selection) {
                    context->mInfo.mMenuResult = selection;
                    SoundManager::Get().func_020246f8(5);
                } else if (mUnlocked[category] != 0) {
                    BOOL available = TRUE;
                    if (category == 0) {
                        if (!data_020d43f0[0].mUnk_5bf_0 && data_020d43f0[0].mUnk_42 == 6) {
                            available = FALSE;
                        }
                    }
                    if (available) {
                        SoundManager::Get().func_020246f8(0);
                        s8 current = context->mInfo.mMenuResult;
                        func_0206f88c(mEvent, (u8) current, data_ov008_02229658[current], 10);
                        mCount = mUnlocked[(s8) context->mInfo.mMenuResult];
                        mWindowB->mCursor->func_02030474(0x30, 0x90, 4, mCount);
                        if (mCount <= 4) {
                            mWindowB->mButtons[1].mEnabled = 0;
                        } else {
                            mWindowB->mButtons[1].mEnabled = 1;
                        }
                        context->mInfo.mStep = 8;
                    }
                }
            }
        }
    }
    u32 second = mFlags & 2;
    if (second == 0) {
        return;
    }
    if (mUnk_85 == 0) {
        mWindowA->mUnk_064 = 0;
        mWindowB->mUnk_064 = 1;
        BOOL changed       = FALSE;
        switch (*(u8 *) &context->mInfo.mPrevSelection) {
            case 1:
                mWindowB->mUnk_061 = 4;
                changed            = TRUE;
                mWindowB->func_0205c7c8(0);
                break;
            case 2:
                SpriteManager::Get().sFade[0] = 3;
                context->mInfo.mStep             = 2;
                break;
            case 0:
                if (func_ov008_02227dd4() == 1) {
                    changed = TRUE;
                }
                break;
        }
        if (!changed) {
            return;
        }
        SoundManager::Get().func_020246f8(0);
        mRowCells[1]->SetAnimation(3);
        mWindowB->mCursor->SetScroll(mScroll);
        SelectWindow *window = mWindowB;
        u8 y                 = window->mCursor->mUnk_0b;
        window->mCellPos[1].x = 0xe4;
        window->mCellPos[1].y = y;
        mUnk_85                 = 1;
    } else {
        if (second == 0) {
            return;
        }
        CellAnimation *cell = mRowCells[1];
        if (cell->mUnk_0e != 3) {
            return;
        }
        if (cell->mUnk_12 == 0) {
            return;
        }
        cell->SetAnimation(2);
        mUnk_85 = 0;
        s32 category = (s8) context->mInfo.mMenuResult;
        u8 entry     = mIndices[mRow];
        u16 message  = data_ov008_022296ac[category][entry].mMessage;
        if (category == 4 && data_ov008_022296ac[category][entry].mText == 0x20) {
            if (data_020d43f0[0].mUnk_5bf_0) {
                message = 0x2b;
            } else {
                u8 level = data_020d43f0[0].mUnk_42;
                if (level <= 1) {
                    message = 0x25;
                } else if (level <= 3) {
                    message = 0x26;
                } else if (level <= 6) {
                    message = 0x27;
                } else if (level <= 7) {
                    message = 0x28;
                } else if (level <= 8) {
                    message = 0x29;
                } else if (level <= 9) {
                    message = 0x2a;
                }
            }
        }
        func_020705f0(mEvent);
        mEvent->func_0206f564(message, 11, -1);
        context->mInfo.mStep = 0xb;
        int flag             = data_ov008_02229660[(s8) context->mInfo.mMenuResult] + mIndices[mRow];
        if (!TEST_BIT(data_020d49a1, flag)) {
            SET_BIT(data_020d49a1, flag);
            mNew[(s8) context->mInfo.mMenuResult]--;
        }
    }
}

s8 MyroomTipsScene::func_ov008_02227a08(const u8 *pos, u8 count) {
    UnkStruct_SceneContext *context = mContext;
    u8 saved                        = context->mInfo.mMenuResult;
    if (TouchManager::Get().mUnk_57 != 0) {
        u8 i = 0;
        do {
            if (TouchManager::Get().func_02022860((s16) (pos[i * 2] - 9), (s16) (pos[i * 2 + 1] - 10), 0x4b, 0x42)) {
                return i;
            }
            i++;
        } while (i < count);
    }
    if (InputManager::Get().IsTrigger(0x20)) {
        s8 current = context->mInfo.mMenuResult;
        if (current < 3) {
            context->mInfo.mMenuResult = current + (count - 1);
            context->mInfo.mMenuResult = (s8) context->mInfo.mMenuResult % 3;
        } else {
            context->mInfo.mMenuResult = current + (count - 1);
            context->mInfo.mMenuResult = (s8) context->mInfo.mMenuResult % 3 + 3;
        }
    } else if (InputManager::Get().IsTrigger(0x10)) {
        s8 current = context->mInfo.mMenuResult;
        if (current < 3) {
            context->mInfo.mMenuResult = current + 1;
            context->mInfo.mMenuResult = (s8) context->mInfo.mMenuResult % 3;
        } else {
            context->mInfo.mMenuResult = current + 1;
            context->mInfo.mMenuResult = (s8) context->mInfo.mMenuResult % 3 + 3;
        }
    }
    if (InputManager::Get().IsTrigger(0x80)) {
        context->mInfo.mMenuResult = (s8) context->mInfo.mMenuResult + 3;
        context->mInfo.mMenuResult = (s8) context->mInfo.mMenuResult % count;
    } else if (InputManager::Get().IsTrigger(0x40)) {
        context->mInfo.mMenuResult = (s8) context->mInfo.mMenuResult + (count - 3);
        context->mInfo.mMenuResult = (s8) context->mInfo.mMenuResult % count;
    }
    if ((s8) context->mInfo.mMenuResult != saved) {
        SoundManager::Get().func_020246f8(5);
    }
    if (InputManager::Get().IsHeld(1)) {
        return context->mInfo.mMenuResult;
    }
    return -1;
}

BOOL MyroomTipsScene::func_ov008_02227dd4() {
    u8 confirm = FALSE;
    u8 row       = mRow;
    u8 scroll    = mScroll;
    mScroll      = mWindowB->mCursor->mScroll;
    if (InputManager::Get().IsTrigger(0x40)) {
        if (mRow != 0) {
            mRow--;
        } else if (mScroll != 0) {
            mScroll--;
        }
    } else if (InputManager::Get().IsTrigger(0x80)) {
        if (mRow < 3) {
            mRow++;
        } else if (mScroll + 4 < mCount) {
            mScroll++;
        }
    }
    if (TouchManager::Get().mTouching != 0) {
        u8 i = 0;
        do {
            if (TouchManager::Get().func_020228c0(0x20, (s16) (i * 32 + 0x20), 0xc0, 0x1c)) {
                if (mRow == i) {
                    confirm = TRUE;
                } else {
                    mRow = i;
                }
                break;
            }
            i++;
        } while (i < 4);
    }
    if (InputManager::Get().IsHeld(1)) {
        confirm = TRUE;
    }
    if ((row != mRow) | (scroll != mScroll)) {
        for (u8 j = 0; j < 4; j++) {
            mIndices[j] = func_ov008_0222852c((u8) (mScroll + j));
        }
        if (scroll != mScroll) {
            if (mScroll > scroll) {
                u8 delta = mScroll - scroll;
                for (s8 k = 0; k < 4; k++) {
                    if (k < 4 - delta) {
                        mUnk_70[k] = mUnk_70[k + delta];
                        mUnk_78[k] = mUnk_78[k + delta];
                        mTimers[k] = mTimers[k + delta];
                    } else {
                        mUnk_70[k] = 0;
                        mUnk_78[k] = 0;
                        mTimers[k] = 0x3c;
                    }
                }
            } else {
                u8 delta = scroll - mScroll;
                for (s8 k = 3; k >= 0; k--) {
                    if (k >= delta) {
                        mUnk_70[k] = mUnk_70[k - delta];
                        mUnk_78[k] = mUnk_78[k - delta];
                        mTimers[k] = mTimers[k - delta];
                    } else {
                        mUnk_70[k] = 0;
                        mUnk_78[k] = 0;
                        mTimers[k] = 0x3c;
                    }
                }
            }
        }
        func_ov008_02228214(-1);
        mWindowB->mCursor->SetScroll(mScroll);
        SelectWindow *window  = mWindowB;
        u8 y                  = window->mCursor->mUnk_0b;
        window->mCellPos[1].x = 0xe4;
        window->mCellPos[1].y = y;
        SoundManager::Get().func_020246f8(5);
        mRowCells[1]->func_02029fc0();
    }
    return confirm;
}

void MyroomTipsScene::func_ov008_02228214(s32 row) {
    s16 *widths;
    UnkStruct_SceneContext *context = mContext;
    u8 i   = 0;
    u8 end = 4;
    if (row >= 0) {
        i   = row;
        end = i;
    }
    widths = mUnk_78;
    do {
        MI_CpuClear16(mTextBuffers[i], mTextBufferSize);
        TextManager &text = TextManager::Get();
        u16 *pixelWidth = (mUnk_70[i] == 0) ? (u16 *) &widths[i] : NULL;
        s16 width;
        s16 height;
        height = 0;
        width = mUnk_70[i];
        s8 menu = context->mInfo.mMenuResult;
        u16 index = data_ov008_022296ac[menu][mIndices[i]].mText;
        MessageFile *file = mMessages;
        text.Print(file->GetText(index), file->GetLength(index), mTextBuffers[i], 0x30, 2, &width, &height, pixelWidth, 0, 2,
                   0x11, 0x10, 0, 0, 1, 1, 1);
        func_02025c1c(1, 4, 0x8000 + i * 0xc00, mTextBuffers[i], mTextBufferSize, 0);
        i++;
    } while (i < end);
}

void MyroomTipsScene::func_ov008_022283a8() {
    mMessage = new MessageWindow("mes_myroom", 1, 0x3800);
    mMessage->func_02065bd4(0x14, 4, 1);
    mMessage->func_02065be4(0xd8);
    mMessage->func_02065890(0x2d, 0, 1);
}

void MyroomTipsScene::func_ov008_0222840c(u32 message) {
    func_ov008_0222844c();
    mMessage->func_02065890(message, 0, 1);
}

void MyroomTipsScene::DrawMessage() {
    if (mMessage != NULL) {
        mMessage->func_020658d0();
    }
}

void MyroomTipsScene::func_ov008_0222844c() {
    if (mMessage != NULL) {
        mMessage->func_02065878();
    }
}

void MyroomTipsScene::func_ov008_02228464() {
    for (u8 i = 0; i < 6; i++) {
        mUnlocked[i] = 0;
        mNew[i]      = 0;
        s8 count     = data_ov008_02229650[i];
        u8 start     = data_ov008_02229660[i];
        u8 j         = 0;
        do {
            if (TEST_BIT(data_020d499c, start + j)) {
                mUnlocked[i]++;
                if (!TEST_BIT(data_020d49a1, data_ov008_02229660[i] + j)) {
                    mNew[i]++;
                }
            }
            j++;
        } while (j < count);
    }
}

u32 MyroomTipsScene::func_ov008_0222852c(s16 index) {
    UnkStruct_SceneContext *context = mContext;
    u8 i        = 0;
    do {
        if (TEST_BIT(data_020d499c, data_ov008_02229660[(s8) context->mInfo.mMenuResult] + i)) {
            index--;
            if (index < 0) {
                return i;
            }
        }
        i++;
    } while (i < data_ov008_02229650[(s8) context->mInfo.mMenuResult]);
    return 0;
}

void MyroomTipsScene::func_ov008_022285ac() {
    for (u8 i = 0; i < 4; i++) {
        if (mTimers[i] != 0) {
            mTimers[i]--;
        } else if (mUnk_78[i] > 0xc0) {
            if (mUnk_78[i] <= -mUnk_70[i]) {
                mUnk_70[i] = 0xc0;
            } else {
                mUnk_70[i]--;
                if (mUnk_70[i] == 0) {
                    mTimers[i] = 0x3c;
                }
            }
            func_ov008_02228214((s8) i);
        }
    }
}
