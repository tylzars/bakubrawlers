#include "Scene/TitleScene.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "System/SoundManager.hpp"
#include "UI/MessageWindow.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include <nitro/gx.h>
#include "System/FileSystem.hpp"
#include <stdio.h>
#include "Text/TextManager.hpp"
#include <nitro/fx.h>
#include "System/FrameTask.hpp"
#include "System/FrameUpdate.hpp"
#include "Unknown/UnkStruct_020c17a8.hpp"
#include "Unknown/UnkStruct_02070608.hpp"
#include "Scene/NameEntryScene.hpp"
#include "Unknown/UnkStruct_02071868.hpp"
#include "Game/UnkStruct_020d43f0.hpp"

extern u8 data_ov000_022266fc[];

void operator delete(void *ptr);

extern u8 data_ov000_022266f8[];
extern u8 data_ov000_02227a74[];
extern u8 data_ov000_02227a88[];
extern u8 data_ov000_02226910[];
extern WindowItemRecord data_ov000_02226830[][2];
extern WindowCellRecord data_ov000_022268a0[][2];
extern WindowItemRecord data_ov000_02226788[];
extern WindowCellRecord data_ov000_022267dc[];
extern WindowItemRecord data_ov000_02226718[];
extern WindowCellRecord data_ov000_02226750[];

void func_ov000_02224edc(void *arg);
s32 func_020230dc(UnkStruct_020d43f0 *base, u32 unk1, u32 unk2);
void func_02023134(UnkStruct_020d43f0 *base, u32 a, u32 b, u32 c, u32 d, u32 e);
void func_0202321c(UnkStruct_020d43f0 *base, const Vec2s *src, u32 b, u32 c);
void func_02022db4(UnkStruct_020d43f0 *base);
void func_02022b28();
void func_02022b0c();
s32 func_02087100();
void func_02086d48();
void func_02089278();

class UnkObj_0203ced4 {
public:
    u8 mPad[0x14];
    UnkObj_0203ced4(const void *data, u8 value);
};
class UnkObj_0221d6a8 {
public:
    u8 mPad[0x68];
    UnkObj_0221d6a8();
};
class UnkObj_0205c8f0 {
public:
    u8 mPad[0x1e8];
    UnkObj_0205c8f0(u32 unk);
};
class UnkObj_0221f4fc {
public:
    u8 mPad[0x50];
    UnkObj_0221f4fc();
};
class UnkObj_02224f60 {
public:
    u8 mPad[0x34];
    UnkObj_02224f60();
};
class UnkObj_02089f90 {
public:
    u8 mPad[0x24];
    UnkObj_02089f90(s32 unk);
};

// Loads the save data (vtable 0x020a5494).
class UnkScene_020a5494 : public SceneBase {
public:
    UnkScene_020a5494() :
        SceneBase(0xffdc, 0) {
        ((u8 *) mContext)[0x23] = 0;
    }
    virtual ~UnkScene_020a5494();
    virtual void Update();
};

TitleScene::TitleScene() :
    SceneBase(1000, 0) {
    mCursor = NULL;
    mWindow30 = NULL;
    mMessageWindow = NULL;
    mBuf38 = NULL;
    mBuf3c = NULL;
    mBuf40 = NULL;
    mBuf44 = NULL;
    mUnk_48 = 0;
    mUnk_4c = 0;
    mUnk_c8 = 0;
    mUnk_cb = 0;
    mUnk_cc = 0;
    mUnk_cd = 0;
    mUnk_cf = 0xff;
    mUnk_d0 = 0;
    mUnk_d1 = 0x10;
    mUnk_d4 = 0;
    mUnk_d6 = 0;
    mUnk_d7 = 0;
    mUnk_d8 = 0x60;
    data_020d433c.mUnk_7b = 4;
    data_020d433c.mUnk_7d = 0;
    data_020d433c.mUnk_87 = 0;
    data_020d433c.mUnk_88_4 = 0;
    data_020d433c.mUnk_88_0 = 0;
    data_020d433c.mUnk_7e_0 = 0;
    data_020d433c.mUnk_8a_0 = 0;
    MI_CpuClear32(mCells, sizeof(mCells));
    MI_CpuClear32(mBuffers, sizeof(mBuffers));
    MI_CpuClear32(mUnk_8c, sizeof(mUnk_8c));
    mCursor = new SelectWindowCursor(1, data_ov000_022266fc, 0);
    mUnk_ce = data_020d433c.mUnk_7e_4;
    mUnk_d5 = mUnk_ce;
    SoundManager::Get().func_020245a4();
    SoundManager::Get().func_020246d4(4);
}

TitleScene::~TitleScene() {
    for (u8 i = 0; i < 8; i++) {
        if (mCells[i] != NULL) {
            delete mCells[i];
        }
    }
    operator delete(mCursor);
    delete mWindow30;
    delete mMessageWindow;
    if (mBuf38 != NULL) {
        OS_Free(mBuf38);
    }
    if (mBuf3c != NULL) {
        OS_Free(mBuf3c);
    }
    if (mBuf40 != NULL) {
        OS_Free(mBuf40);
    }
    if (mBuf44 != NULL) {
        OS_Free(mBuf44);
    }
    for (u8 i = 0; i < 5; i++) {
        for (u8 j = 0; j < 3; j++) {
            if (mBuffers[i][j] != NULL) {
                OS_Free(mBuffers[i][j]);
            }
        }
    }
}

// non-matching: register allocation in the state 12 / case 4 save-fixup block (data_020d43f0 pointer, buf pointer, 0x22/0 constants and loop counters land in different registers; 31 differing instructions, same size)
void TitleScene::Update() {
    switch (mUnk_cb) {
        case 0:
            if (data_020d433c.mUnk_ad) {
                mUnk_d3 = 0;
                mUnk_cb = 0x2a;
                break;
            }
            if (data_020d433c.mUnk_80) {
                mUnk_cb = 3;
                break;
            }
            FrameTask::Get().func_0203dc28();
            mUnk_cb++;
            break;
        case 1:
            new UnkObj_0203ced4(data_ov000_02227a74, data_020d433c.mUnk_79);
            mUnk_cb++;
            break;
        case 2:
            if (mContext->mUnk_12 == 1) {
                FrameTask::Get().func_0203dbc4();
                mUnk_cb++;
            }
            break;
        case 3:
            func_ov000_02223aa4();
            LoadGraphics();
            LoadSystemText();
            SpriteManager::Get().StartFade(2);
            if (data_020d433c.mUnk_76) {
                FrameTask::Get().func_0203dc34(0, 0x10);
                mUnk_d7 = 1;
            } else {
                FrameTask::Get().func_0203dc34(0, 0x17);
            }
            mUnk_cb++;
            break;
        case 4:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            SetWindowMode(1);
            mUnk_cb++;
            break;
        case 5:
            if (IsFadeDone(0)) {
                mUnk_c8 = 0;
                if (data_020d433c.mUnk_76) {
                    mUnk_cb = 0x20;
                    break;
                }
                mUnk_cb++;
            }
            break;
        case 6:
            func_ov000_022245e8();
            mUnk_c8++;
            if (mUnk_c8 > 0xe10) {
                mUnk_c8 = 0;
                mUnk_cb = 7;
                break;
            }
            if (mCursor->func_0202f468(1) == 1) {
                mUnk_cc = 0;
                mUnk_cb = 9;
            }
            break;
        case 7:
            func_ov000_022245e8();
            if (IsFadeDone(0)) {
                SetWindowMode(0);
                SpriteManager::Get().StartFade(3);
                mUnk_cb++;
            }
            break;
        case 8:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            FrameTask::Get().func_0203dc28();
            mUnk_cb = 1;
            return;
        case 9: {
            func_02022b28();
            u8 a = data_020d433c.mUnk_81;
            u8 b = data_020d433c.mUnk_79;
            u8 c = data_020d433c.mUnk_77;
            func_02022b0c();
            data_020d433c.mUnk_81 = a;
            data_020d433c.mUnk_79 = b;
            data_020d433c.mUnk_77 = c;
            GXS_SetVisiblePlane(0x11);
            u32 idx = 1;
            u8 count = 2;
            switch (data_020d433c.mUnk_77) {
                case 0:
                    data_020d433c.mUnk_77 = idx;
                    if (UnkStruct_020c17a8::Get().func_02023d30(0) == 1) {
                        new UnkStruct_02071868(1);
                    }
                case 1:
                    count--;
                    idx = 0;
                    break;
            }
            delete mWindow30;
            mWindow30 = new SelectWindow(count, data_ov000_02226830[idx], -1);
            mWindow30->SetCells(count, data_ov000_022268a0[idx]);
            mWindow30->mCursor->mSelection = -1;
            if (idx == 0) {
                mWindow30->mCursor->mSelection = 0;
                SetMessage(0);
            } else if (mUnk_d0) {
                mWindow30->mCursor->mSelection = 0;
                mWindow30->mCells[0]->SetAnimation(1);
            } else {
                mWindow30->mCursor->mSelection = 1;
                mWindow30->mCells[1]->SetAnimation(4);
            }
            mUnk_cb++;
            break;
        }
        case 10: {
            u8 result = mWindow30->Update(1);
            u8 sel = mWindow30->mCursor->mSelection;
            if (result == 1) {
                if (sel == 0) {
                    switch (data_020d433c.mUnk_77) {
                        case 0:
                        case 1:
                            mUnk_cb = 0x13;
                            break;
                        default:
                            mUnk_cb = 0xd;
                            break;
                    }
                } else {
                    FrameTask::Get().func_0203dd90();
                    mUnk_cb++;
                }
            }
            mWindow30->Draw();
            break;
        }
        case 11:
            if (func_0203dd0c(&FrameTask::Get(), 0) != 0) {
                break;
            }
            mContext->mUnk_12 = 0;
            UnkStruct_020c17a8::Get();
            new UnkScene_020a5494();
            mUnk_cb++;
            break;
        case 12:
            switch (mContext->mUnk_12) {
                case 1:
                case 2:
                case 5:
                    new UnkStruct_02071868(0);
                    break;
                case 3:
                    mUnk_cb = 0xf;
                    break;
                case 4: {
                    u8 buf[3];
                    buf[0] = data_ov000_022266f8[0];
                    buf[1] = data_ov000_022266f8[1];
                    buf[2] = data_ov000_022266f8[2];
                    u8 changed = 0;
                    for (u8 i = 0; i < 3; i++) {
                        if (func_020230dc(data_020d43f0, 0, i) == 0x28) {
                            u8 v = buf[i];
                            func_02023134(data_020d43f0, 0, i, 0, 0x22, v);
                            changed = 1;
                        }
                    }
                    for (u8 i = 0; i < 6; i++) {
                        if (func_020231f4(data_020d43f0, 0, 0x28, i)) {
                            int r = func_0202317c(data_020d43f0, 0, 0x28, i);
                            func_020231c0(data_020d43f0, r, 0);
                            changed = 1;
                        }
                    }
Vec2s tmp = {0, 0};
                    if (changed) {
                        for (u8 i = 0; i < 3; i++) {
                            u8 v = buf[i];
                            int r = func_0202317c(data_020d43f0, 0, 0x22, v);
                            func_020231c0(data_020d43f0, r, 0xc8);
                            func_0202321c(data_020d43f0, &tmp, 0x22, v);
                        }
                    }
                    func_02022db4(data_020d43f0);
                    FrameTask::Get().func_0203dc34(0, 0x10);
                    data_020d433c.mUnk_76 = 1;
                    mUnk_d7 = 1;
                    mUnk_d8 = 0x48;
                    mUnk_cb = 0x20;
                    break;
                }
            }
            break;
        case 13:
            delete mWindow30;
            mWindow30 = new SelectWindow(3, data_ov000_02226788, -1);
            mWindow30->SetCells(3, data_ov000_022267dc);
            mWindow30->mCursor->mSelection = -1;
            mWindow30->mCursor->mSelection = 1;
            PrintText((MessageFile *) mBuf38, 8);
            GXS_SetVisiblePlane(0x19);
            mUnk_d0 = 1;
            mUnk_cb++;
            break;
        case 14:
            if ((u8) mWindow30->Update(1) == 1) {
                if ((u8) mWindow30->mCursor->mSelection == 0) {
                    mUnk_cb = 0x13;
                } else {
                    mUnk_cb = 9;
                }
            }
            DrawText();
            mWindow30->Draw();
            break;
        case 15:
            delete mWindow30;
            mWindow30 = new SelectWindow(3, data_ov000_02226788, -1);
            mWindow30->SetCells(3, data_ov000_022267dc);
            mWindow30->mCursor->mSelection = -1;
            mWindow30->mCursor->mSelection = 1;
            PrintText((MessageFile *) mBuf38, 7);
            GXS_SetVisiblePlane(0x19);
            mUnk_cb++;
            break;
        case 16:
            if ((u8) mWindow30->Update(1) == 1) {
                if ((u8) mWindow30->mCursor->mSelection == 0) {
                    data_020d433c.mUnk_77 = 1;
                    if (UnkStruct_020c17a8::Get().func_02023d30(0) == 1) {
                        new UnkStruct_02071868(1);
                    }
                    mUnk_cb++;
                } else {
                    mUnk_cb = 9;
                }
            }
            DrawText();
            mWindow30->Draw();
            break;
        case 17:
            delete mWindow30;
            mWindow30 = new SelectWindow(2, data_ov000_02226718, -1);
            mWindow30->SetCells(2, data_ov000_02226750);
            mWindow30->mCursor->mSelection = -1;
            mWindow30->mCursor->mSelection = 0;
            PrintText((MessageFile *) mBuf38, 3);
            GXS_SetVisiblePlane(0x19);
            mUnk_cb++;
            break;
        case 18:
            if ((u8) mWindow30->Update(1) == 1) {
                mUnk_cb = 9;
            }
            DrawText();
            mWindow30->Draw();
            break;
        case 19:
            if (IsFadeDone(1)) {
                SetWindowMode(0);
                data_020d433c.mUnk_76 = 1;
                SpriteManager::Get().StartFade(3);
                FrameTask::Get().func_0203dc28();
                mUnk_cb++;
            }
            break;
        case 20:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            new UnkObj_0203ced4(data_ov000_02227a88, data_020d433c.mUnk_79);
            mUnk_cb++;
            break;
        case 21:
            if (mContext->mUnk_12 == 1) {
                FrameTask::Get().func_0203dbc4();
                FrameTask::Get().func_0203dc34(0, 0x17);
                mUnk_cb++;
            }
            break;
        case 22:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            new NameEntryScene(0);
            mContext->mUnk_12 = 0;
            mUnk_cb++;
            break;
        case 23:
            if (mContext->mUnk_12 != 0) {
                data_020d43f0[0].mUnk_1a_8 = 0;
                mUnk_cb++;
            }
            break;
        case 24:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            new UnkObj_0221d6a8();
            mContext->mUnk_12 = 0;
            mUnk_cb++;
            break;
        case 25:
            switch (mContext->mUnk_12) {
                case 1:
                    mUnk_cb = 0x16;
                    break;
                case 2:
                    mUnk_cb++;
                    break;
            }
            break;
        case 26:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            new UnkObj_0205c8f0(0);
            mContext->mUnk_12 = 0;
            mUnk_cb++;
            break;
        case 27:
            switch (mContext->mUnk_12) {
                case 1:
                    mUnk_cb++;
                    break;
                case 2:
                    mUnk_cb = 0x18;
                    break;
            }
            break;
        case 28:
            func_ov000_02223cdc();
            new UnkStruct_02070608(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mUnk_12 = 0;
            data_020d433c.mUnk_14_1 = 0;
            SpriteManager::Get().StartFade(2);
            mUnk_cb++;
            break;
        case 29:
            if (mContext->mUnk_12 != 0) {
                func_ov000_02223aa4();
                LoadGraphics();
                G2_BlendNone();
                SpriteManager::Get().StartFade(2);
                mUnk_d7 = 1;
                FrameTask::Get().func_0203dc34(0, 0x10);
                mUnk_cb++;
            }
            break;
        case 30:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            SetWindowMode(1);
            mUnk_cb++;
            break;
        case 31:
            if (IsFadeDone(0)) {
                mUnk_d6 = 1;
                mUnk_cb++;
            }
            break;
        case 32:
            data_020d433c.mUnk_80 = 1;
            SelectWindowCursor *cursor = mCursor;
            cursor->mItems = (WindowItem *) data_ov000_02226910;
            cursor->mCount = 5;
            mCursor->mSelection = mUnk_ce;
            UploadSubBg(mUnk_ce);
            GXS_SetVisiblePlane(0x17);
            func_020254f0(func_ov000_02224edc, this);
            mUnk_da = -0xc0;
            mUnk_cc = 0x13;
            mUnk_cb++;
            break;
        case 33:
            mUnk_cc--;
            u8 cc = mUnk_cc;
            if (cc != 0) {
                func_020254f0(func_ov000_02224edc, this);
                s8 d = cc - 3;
                mUnk_da = -((s16) ((d * d - 9) * 0xc0 / 0x100));
                break;
            }
            func_020254f0(func_ov000_02224edc, this);
            mUnk_da = 0;
            mUnk_cd = 1;
            func_ov000_02224678();
            SetMessage((u8) (mUnk_ce + 2));
            mUnk_cc = 0;
            if (mUnk_d6) {
                mUnk_d6 = 0;
                delete mWindow30;
                mWindow30 = new SelectWindow(2, data_ov000_02226718, -1);
                mWindow30->SetCells(2, data_ov000_02226750);
                mWindow30->mCursor->mSelection = -1;
                mWindow30->mCursor->mSelection = 0;
                PrintText((MessageFile *) mBuf3c, 7);
                GXS_SetVisiblePlane(0x1f);
                mUnk_cb++;
            } else {
                mUnk_cb = 0x24;
            }
            break;
        case 34:
            if (mWindow30->Update(1)) {
                mUnk_cb++;
            }
            DrawText();
            mWindow30->Draw();
            break;
        case 35:
            GXS_SetVisiblePlane(0x17);
            mUnk_cb++;
            break;
        case 36: {
            u8 changed = 0;
            u8 prev = mUnk_ce;
            u8 event = mCursor->func_0202f468(1);
            if (prev != (u8) mCursor->mSelection) {
                mUnk_ce = mCursor->mSelection;
                func_ov000_02224678();
                SetMessage((u8) (mUnk_ce + 2));
            }
            if (event == 1 || event == 3) {
                changed = 1;
            }
            if (changed) {
                mCells[mUnk_ce + 3]->SetAnimation(mUnk_ce * 3 + 2);
                mUnk_cb++;
            }
            UpdateCursorCell();
            break;
        }
        case 37:
            UpdateCursorCell();
            if (IsFadeDone(0)) {
                SetWindowMode(0);
                SpriteManager::Get().StartFade(3);
                mUnk_cb++;
            }
            break;
        case 38:
            if (UpdateCursorCell()) {
                mUnk_cb++;
            }
            break;
        case 39:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mUnk_cd = 0;
            mUnk_d7 = 0;
            mUnk_cb = 0x28;
            break;
        case 40: {
            data_020d433c.mUnk_7e_4 = mUnk_ce;
            u32 scene;
            switch (mUnk_ce) {
                case 0:
                    scene = 9;
                    if (data_020d43f0[0].mUnk_5bf_0) {
                        func_02089278();
                    }
                    break;
                case 1:
                    data_020d433c.mUnk_89 = 0;
                    scene = 8;
                    break;
                case 2:
                    data_020d433c.mUnk_83 = 0;
                    data_020d433c.mUnk_84 = 0;
                    data_020d433c.mUnk_82 = 0;
                    scene = 10;
                    break;
                case 3:
                    data_020d433c.mUnk_7f = 4;
                    scene = 7;
                    break;
                case 4:
                    mContext->mUnk_12 = 0;
                    new UnkObj_0221f4fc();
                    mUnk_cb = 0x29;
                    return;
            }
            RequestScene(scene);
            mFinished = 1;
            break;
        }
        case 41:
            if (mContext->mUnk_12 != 0) {
                RequestScene(4);
                mFinished = 1;
                return;
            }
            break;
        case 42:
            new UnkObj_02224f60();
            mContext->mUnk_12 = 0;
            mUnk_cb++;
            break;
        case 43:
            if (mContext->mUnk_12 != 0) {
                mUnk_cb++;
            }
            break;
        case 44: {
            data_020d43f0[0].mUnk_5bf_0 = 1;
            s32 id = func_02087100();
            if (id >= 0) {
                new UnkObj_02089f90(id);
                mContext->mUnk_12 = 0;
                mUnk_cb--;
            } else {
                func_02086d48();
                mUnk_cb++;
            }
            break;
        }
        case 45:
            func_ov000_02223cdc();
            new UnkStruct_02070608(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mUnk_12 = 0;
            SpriteManager::Get().StartFade(2);
            mUnk_cb++;
            break;
        case 46:
            if (mContext->mUnk_12 != 0) {
                mUnk_cb++;
            }
            break;
        case 47:
            data_020d433c.mUnk_ad = 0;
            RequestScene(2);
            mFinished = 1;
            return;
    }
    UpdateFade();
    if (mUnk_cd) {
        func_ov000_02224620();
    }
    DrawLogo();
}

void TitleScene::func_ov000_02223aa4() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 0);
    G2_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(2, 1, 2, 4, 0);
    G2S_SetBG2Control(2, 1, 4, 6, 0);
    G2S_SetBG3Control(0, 1, 6, 7, 0);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0x140);
    G2S_SetBG2Offset(0, 0x140);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(1);
    G2S_SetBG2Priority(2);
    G2S_SetBG3Priority(0);
    GX_SetVisiblePlane(2);
    GXS_SetVisiblePlane(0x11);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2_BlendNone();
    G2S_BlendNone();
}

void TitleScene::func_ov000_02223cdc() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 1);
    func_02001e68(1, 0, 0);
    func_02001ed0(0);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(0);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x13);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_BlendNone();
    G2S_BlendNone();
}

void TitleScene::LoadGraphics() {
    char path[0x80];
    ChangeDir("/Game/Title/");
    func_02025b64(0, 1, 0, "title_u_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(0, 11, 0, "title_u_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(0, 7, 0, "title_u_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(0, 1, 0x2000, "title_u_bg_01_ncl.bin", 0, 0, 0);
    func_02025b64(0, 12, 0, "title_u_bg_01_ncg.bin", 0, 0, 0);
    func_02025b64(0, 8, 0, "title_u_bg_01_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0, "title_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "title_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "title_b_bg_00_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x2000, "title_b_bg_02_ncl.bin", 0, 0, 0);
    func_02025b64(1, 12, 0, "title_b_bg_02_ncg.bin", 0, 0, 0);
    func_02025b64(1, 8, 0, "title_b_bg_02_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 1, 0x6000, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 14, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 10, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Title/");
    if (mBuffers[0][1] == NULL) {
        for (u8 i = 0; i < 5; i++) {
            sprintf(path, "title_b_bg_sub_%02d_ncl.bin", i);
            mBuffers[i][1] = func_02023504(path, 0, 0, 0, &mUnk_8c[i][1], 0);
            sprintf(path, "title_b_bg_sub_%02d_ncg.bin", i);
            mBuffers[i][0] = func_02023504(path, 0, 0, 0, &mUnk_8c[i][0], 0);
            sprintf(path, "title_b_bg_sub_%02d_nsc.bin", i);
            mBuffers[i][2] = func_02023504(path, 0, 0, 0, &mUnk_8c[i][2], 0);
        }
    }
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.LoadCharacters("title_b_obj_00.nce_ncg1D.bin", 0, 0);
    screen.func_02027fec("title_b_obj_00_ncl.bin", 0, 0);
    for (u8 i = 0; i < 8; i++) {
        if (mCells[i] != NULL) {
            delete mCells[i];
        }
        mCells[i] = new CellAnimation("title_b_obj_00_nce.bin", 0, 1, 1);
        mCells[i]->mCharBase = screen.func_020276f4(0);
        mCells[i]->mUnk_11 = 0;
    }
    mCells[3]->SetAnimation(0);
    mCells[4]->SetAnimation(3);
    mCells[5]->SetAnimation(6);
    mCells[6]->SetAnimation(9);
    mCells[7]->SetAnimation(12);
    screen.LoadCharacters("title_b_obj_03.nce_ncg1D.bin", 0xb000, 0);
    screen.LoadPalette("title_b_obj_03_ncl.bin", 0x40, 0);
    if (mCells[0] != NULL) {
        delete mCells[0];
    }
    mCells[0] = new CellAnimation("title_b_obj_03_nce.bin", 0, 1, 1);
    mCells[0]->mCharBase = screen.func_020276f4(0xb000);
    mCells[0]->mUnk_11 = 2;
    screen.LoadCharacters("title_b_obj_02.nce_ncg1D.bin", 0xc000, 0);
    screen.func_02027fec("title_b_obj_02_ncl.bin", 0x400, 0);
    if (mCells[1] != NULL) {
        delete mCells[1];
    }
    mCells[1] = new CellAnimation("title_b_obj_02_nce.bin", 1, 1, 1);
    mCells[1]->mCharBase = screen.func_020276f4(0xc000);
    mCells[1]->mUnk_11 = 2;
    screen.LoadCharacters("title_b_obj_04.nce_ncg1D.bin", 0x1d000, 0);
    screen.LoadPalette("title_b_obj_04_ncl.bin", 0xe0, 0);
    if (mCells[2] != NULL) {
        delete mCells[2];
    }
    mCells[2] = new CellAnimation("title_b_obj_04_nce.bin", 0, 1, 1);
    mCells[2]->mCharBase = screen.func_020276f4(0x1d000);
    mCells[2]->mUnk_11 = 7;
    screen.LoadCharacters("title_b_obj_01.nce_ncg1D.bin", 0x13000, 0);
    screen.func_02027fec("title_b_obj_01_ncl.bin", 0x200, 0);
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0x1b000, 0);
    screen.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
    delete mMessageWindow;
    mMessageWindow = new MessageWindow("mes_title", 1, 0x1f000);
    mMessageWindow->func_02065bd4(0x14, 4, 1);
    mMessageWindow->func_02065be4(0xd8);
}

void TitleScene::func_ov000_022245e8() {
    mCells[0]->Update();
    mCells[0]->Draw(1, 0x80, 0x60, 0);
}

void TitleScene::func_ov000_02224620() {
    for (u8 i = 0; i < 5; i++) {
        mCells[3 + i]->Update();
        mCells[3 + i]->Draw(1, 0x80, 0x60, 3);
    }
}

void TitleScene::func_ov000_02224678() {
    for (u8 i = 0; i < 5; i++) {
        u8 anim = i * 3;
        if (i == mUnk_ce) {
            anim = anim + 1;
        }
        mCells[3 + i]->SetAnimation(anim);
    }
}

void TitleScene::LoadSystemText() {
    SpriteManager::Get().Screen(1).func_02027624(3);
    if (mBuf38 == NULL) {
        ChangeDir("/");
        mBuf38 = func_02023504("font/mes_system.mes", 0, 0, 0, 0, 0);
        mBuf3c = func_02023504("font/mes_title.mes", 0, 0, 0, 0, 0);
        mUnk_48 = TextManager::Get().BufferSize(0x18, 0xa);
        mBuf40 = OS_Alloc(mUnk_48);
        mUnk_4c = TextManager::Get().GlyphCount(0x3c);
        mBuf44 = OS_Alloc(mUnk_4c * 0xc);
        TextManager::Get().Init(mBuf44, 1, 0, 0x18, 0xa, 0x11);
    }
    MI_CpuClear16(mBuf40, mUnk_48);
    TextManager::Get().SetColor(1, 0, 0xf);
}

void TitleScene::PrintText(MessageFile *file, u32 index) {
    ClearText();
    TextManager &text = TextManager::Get();
    s16 outWidth;
    s16 outHeight;
    outHeight = 0;
    outWidth = 0;
    text.Print(file->GetText(index), file->GetLength(index), mBuf40, 0x18, 0xa, &outWidth, &outHeight, NULL, 0, 2,
               0x11, 0xe, 0, 0, 1, 1, 1);
    func_02025c1c(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x19000, mBuf40, mUnk_48, 0);
}

void TitleScene::DrawText() {
    SpriteManager::Get().Screen(1).func_02027778(mBuf44, (u8) mUnk_4c, 0x20, 0x28, 0, 0x320, 0xf, 0);
}

void TitleScene::ClearText() {
    if (mBuf40 != NULL) {
        MI_CpuClear16(mBuf40, mUnk_48);
    }
}

void TitleScene::SetMessage(u32 index) {
    if ((index <= 6) & (mUnk_cf != index)) {
        mUnk_cf = index;
        mMessageWindow->func_02065890(mUnk_cf, 0, 1);
    }
}

void TitleScene::SetWindowMode(u8 mode) {
    mUnk_d3 = mode;
    if (mode) {
        GX_SetVisiblePlane(3);
        G2_SetWndOutsidePlane(0x1f, TRUE);
        mUnk_d2 = 0x2d;
        mUnk_d1 = 0x10;
    } else {
        GX_SetVisiblePlane(1);
        G2_BlendNone();
        mUnk_d2 = 0;
    }
}

// Same helper as FieldSelectScene.cpp defines locally; belongs in nitro/gx.h next to G2S_SetBlendAlpha.
void TitleScene::UpdateFade() {
    if (!mUnk_d3) {
        return;
    }
    u16 angle = (mUnk_d2 << 16) / 180;
    s16 sin = FX_SinCosTable_[(angle >> 4) * 2];
    s8 level = (mUnk_d1 * sin) / 4096;
    if (level < 0) {
        level = 0;
    }
    int ev2 = 0x10 - level;
    G2_SetBlendAlpha(2, 1, level, ev2);
    if (mUnk_d2 != 0) {
        mUnk_d2--;
        return;
    }
    mUnk_d1 = 6;
    mUnk_d2 = 0xb4;
}

BOOL TitleScene::IsFadeDone(BOOL tick) {
    if (!mUnk_d3) {
        return TRUE;
    }
    if (mUnk_d2 < 0x5a) {
        if (tick != 0 && mUnk_d2 != 0) {
            mUnk_d2--;
        }
        return FALSE;
    }
    return TRUE;
}

void TitleScene::UploadSubBg(u32 index) {
    func_02025c1c(1, 1, 0x4000, mBuffers[index][1], mUnk_8c[index][1], 0);
    func_02025c1c(1, 13, 0, mBuffers[index][0], mUnk_8c[index][0], 0);
    func_02025c1c(1, 9, 0, mBuffers[index][2], mUnk_8c[index][2], 0);
}

BOOL TitleScene::UpdateCursorCell() {
    mCells[1]->Update();
    mCells[1]->Draw(1, 0x80, 0x58, 2);
    if (mUnk_d4 == 0) {
        if (mUnk_d5 == mUnk_ce) {
            return TRUE;
        }
        mUnk_d4 = 1;
        mCells[1]->SetAnimation(2);
        SoundManager::Get().func_020246f8(0xd);
    }
    switch (mUnk_d4) {
        case 1:
            if (mCells[1]->mUnk_12 != 0) {
                mUnk_d4++;
            }
            break;
        case 2: {
            u8 cur = mUnk_ce;
            mUnk_d5 = cur;
            UploadSubBg(cur);
            mCells[1]->SetAnimation(3);
            SoundManager::Get().func_020246f8(0xc);
            mUnk_d4++;
            break;
        }
        case 3:
            if (mCells[1]->mUnk_12 != 0) {
                mUnk_d4 = 0;
            }
            break;
    }
    return FALSE;
}

void TitleScene::ApplyScroll() {
    G2S_SetBG1Offset(0, mUnk_da);
    G2S_SetBG2Offset(0, mUnk_da);
}

void TitleScene::DrawLogo() {
    if (!mUnk_d7) {
        return;
    }
    if (mUnk_d8 < 0x60) {
        mUnk_d8++;
    } else {
        mMessageWindow->func_020658d0();
    }
    mCells[2]->Draw(1, 0x80, mUnk_d8, 1);
}
