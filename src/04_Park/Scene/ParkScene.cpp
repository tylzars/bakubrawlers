#include "global.h"
#include "types.h"
#include "Scene/ParkScene.hpp"
#include "Park/ParkMenu.hpp"
#include "Park/ParkPopup.hpp"
#include "Game/Random.hpp"
#include "System/FrameTask.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Unknown/UnkStruct_02070608.hpp"
#include "Unknown/UnkStruct_0208ab40.hpp"
#include "Game/UnkStruct_02065c40.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "UI/SelectWindow.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>


// One record of the park character table, filled by func_02089924.
struct ParkEntry {
    /* 00 */ u32 mType;
    /* 04 */ u8 mUnk_04;
    /* 05 */ u8 mUnk_05;
    /* 06 */ u8 mUnk_06;
    /* 07 */ u8 mUnk_07;
    /* 08 */ u8 mUnk_08;
    /* 09 */ u8 mUnk_09;
    /* 0a */ union {
        u16 mUnk_0a_h;
        struct {
            u8 mUnk_0a;
            u8 mUnk_0b;
        };
    };
};

struct ParkRandEntry {
    u16 mMessage;
    u8 mKind;
    u8 mPad;
};
extern const ParkRandEntry data_ov004_0221d910[][6];
extern const u16 data_ov004_0221d7f4;

extern const char data_ov004_0221dd50[];
extern const char data_ov004_0221dd5c[];
extern const char data_ov004_0221dd68[];
extern const char data_ov004_0221dd78[];
extern const char data_ov004_0221dd88[];
extern const char data_ov004_0221dd9c[];
extern const char data_ov004_0221ddb0[];
extern const char data_ov004_0221ddc4[];
extern const char data_ov004_0221ddd8[];
extern const char data_ov004_0221ddec[];
extern const char data_ov004_0221ddfc[];
extern const char data_ov004_0221de10[];
extern const char data_ov004_0221de24[];


ParkScene::ParkScene() :
    SceneBase(1000, 0) {
    mUnk_0c = 0;
    mUnk_0d = 0;
    mUnk_0e = 0;
    mUnk_0f = 0;
    mWindowVisible = 0;
    mUnk_12 = 0;
    mMenu = NULL;
    mWindow = NULL;
    mUnk_2c = NULL;
    mUnk_1c = NULL;
    mGlyphBuffer = NULL;
    mPlayerCount = data_020d433c.mPlayerCount;
    SpriteManager::Get().SetFade(1, -16);
    G2_BlendNone();
}

ParkScene::~ParkScene() {
    Cleanup();
}

// non-matching: 2 words in case 24. The original computes the func_0206fa20 address arguments in the order
// r2, r1, r3; every source form tried gives r1, r2, r3.
void ParkScene::Update() {
    switch (mUnk_0d) {
        case 0: {
            u8 saved                 = data_020d433c.mUnk_7d;
            data_020d433c.mUnk_7d    = 0;
            Init();
            if (mEvent->func_02066c7c()) {
                switch (data_020d433c.mUnk_7c) {
                    case 2:
                    case 3:
                        data_020d433c.mUnk_7d = data_020d433c.mUnk_7c;
                        break;
                    default:
                        data_020d433c.mUnk_7d = 0;
                        break;
                }
                mUnk_0e = 8;
                mUnk_0d++;
            } else {
                data_020d433c.mUnk_7d = saved;
                mUnk_0d               = 10;
            }
            break;
        }
        case 1:
            if (--mUnk_0e != 0) {
                break;
            }
            data_020d433c.mUnk_7c = 1;
            SpriteManager::Get().StartFade(2);
            mUnk_0d++;
            break;
        case 2:
            if (mEvent->func_02067900() | mEvent->func_02067954()) {
                SpriteManager::Get().StartSubFade(3);
                mUnk_0d = 4;
            }
            if (mEvent->func_0206797c()) {
                SpriteManager::Get().StartSubFade(3);
                mUnk_0d = 7;
            }
            if (mEvent->func_020678e8()) {
                break;
            }
            SpriteManager::Get().StartSubFade(3);
            mUnk_0d++;
            break;
        case 3:
            if (SpriteManager::Get().sFade.mSub > 1) {
                break;
            }
            {
                u8 v = data_020d43f0[0].mUnk_42;
                if ((v == 2) | (v == 4)) {
                    if (v == 2) {
                        data_020d433c.mUnk_83 = 5;
                    } else {
                        data_020d433c.mUnk_83 = 6;
                    }
                    data_020d433c.mUnk_84 = 0;
                    data_020d433c.mUnk_82 = 1;
                    RequestScene(10);
                    mFinished = TRUE;
                    return;
                }
            }
            RequestScene(5);
            mFinished = TRUE;
            return;
        case 4:
            if (SpriteManager::Get().sFade.mSub > 1) {
                break;
            }
            LoadWindowGraphics();
            new UnkStruct_02070608(1, 1, 0, 0x2000, 0, 0, 0, 0, 0);
            mContext->mUnk_12 = 0;
            data_020d433c.mUnk_14_1 = 0;
            SpriteManager::Get().StartSubFade(2);
            mUnk_0d++;
            break;
        case 5:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            SpriteManager::Get().StartSubFade(3);
            FrameTask::Get().func_0203dc34(0, 0x14);
            mUnk_0d++;
            break;
        case 6:
            if (SpriteManager::Get().sFade.mSub > 1) {
                break;
            }
            if (mUnk_12 == 0) {
                RequestScene(9);
                mFinished = TRUE;
            } else {
                RequestScene(12);
                mFinished = TRUE;
            }
            return;
        case 7:
            if (SpriteManager::Get().sFade.mSub > 1) {
                break;
            }
            new UnkStruct_0208ab40(mEvent->func_0206797c() - 1);
            mContext->mUnk_12 = 0;
            mUnk_0d++;
            break;
        case 8:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            func_ov004_0221b5b4();
            mEvent->func_02067988();
            mUnk_0d++;
            break;
        case 9:
            mUnk_0d = 2;
            break;
        case 10:
            switch (data_020d433c.mUnk_7c) {
                case 2:
                case 3:
                    mEvent->func_020676f8();
                    mContext->mUnk_12 = 0;
                    mUnk_0d           = 11;
                    break;
                default:
                    FrameTask::Get().func_0203dc34(0, 0x14);
                    mUnk_0d = 0x11;
                    break;
            }
            break;
        case 11: {
            if (mContext->mUnk_12 == 0) {
                break;
            }
            ParkEntry entry;
            func_02089924(data_020d433c.mUnk_8b_0, &entry);
            if (entry.mType == 6) {
                SpriteManager::Get().StartSubFade(3);
                mUnk_0d++;
                break;
            }
            if (data_020d433c.mUnk_98 != 0) {
                BOOL rules = FALSE;
                if (data_020d433c.mUnk_7c == 2) {
                    rules = reinterpret_cast<ParkRules *>(this)->func_ov004_0221bf5c();
                }
                func_02089a9c(data_020d433c.mUnk_8b_0);
                data_020d433c.mUnk_7c = 1;
                if (rules == 0) {
                    Init();
                    FrameTask::Get().func_0203dc34(0, 0x14);
                    mUnk_0d = 0x11;
                } else {
                    mUnk_12 = 1;
                    mUnk_0d = 4;
                }
            } else {
                mEvent = new UnkStruct_02065c40(7, 0, 1);
                FrameTask::Get().func_0203dc34(0, 0x14);
                mUnk_0d = 0xd;
            }
            break;
        }
        case 12:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            data_020d433c.mUnk_7c = 1;
            func_02089a9c(data_020d433c.mUnk_8b_0);
            mUnk_12 = 1;
            mUnk_0d = 4;
            break;
        case 13: {
            ParkEntry entry;
            func_02089924(data_020d433c.mUnk_8b_0, &entry);
            u32 idx = func_02089a64(data_020d433c.mUnk_8b_0);
            u8 pick = MATH_Rand32(&data_020d42e8, 3);
            u8 nib  = data_020d433c.mUnk_8c_4;
            if (nib == 0) {
                pick += 3;
            }
            u8 kind     = data_ov004_0221d910[idx][pick].mKind;
            u16 message = data_ov004_0221d910[idx][pick].mMessage;
            u8 ok;
            if (nib != 0) {
                ok = mEvent->func_0206f9b4(data_ov004_0221dd50, idx, kind, message, 0, 2, 0);
            } else {
                ok = mEvent->func_0206f9b4(data_ov004_0221dd5c, idx, kind, message, 1, 2, 0);
            }
            if (ok == 0) {
                break;
            }
            SpriteManager::Get().StartFade(2);
            mUnk_0d++;
            break;
        }
        case 14:
            if (!mEvent->func_020678e8()) {
                data_020d433c.mUnk_7c = 1;
                SpriteManager::Get().StartSubFade(3);
                mUnk_0d = 0xf;
                break;
            }
            if (mEvent->func_02067938()) {
                func_ov004_0221b5b4();
                mEvent->func_02067944();
            }
            break;
        case 15:
            if (SpriteManager::Get().sFade.mSub > 1) {
                break;
            }
            mEvent->func_020676f8();
            mContext->mUnk_12 = 0;
            mUnk_0d++;
            break;
        case 16: {
            if (mContext->mUnk_12 == 0) {
                break;
            }
            BOOL rules = FALSE;
            if (data_020d433c.mUnk_8c_4 == 0) {
                rules = reinterpret_cast<ParkRules *>(this)->func_ov004_0221bf5c();
            }
            func_02089a9c(data_020d433c.mUnk_8b_0);
            if (rules == 0) {
                Init();
                mUnk_0d = 0x11;
            } else {
                mUnk_12 = 1;
                mUnk_0d = 4;
            }
            break;
        }
        case 17:
            SpriteManager::Get().StartFade(2);
            mUnk_0d++;
            break;
        case 18:
            if (mEvent->func_020678e8()) {
                break;
            }
            data_020d433c.mUnk_7c = 1;
            mEvent->func_0206ef4c(data_ov004_0221dd68, 5, 0xa, 2, 0x15);
            mUnk_0d++;
            break;
        case 19: {
            u8 result = mMenu->mWindow->Update(TRUE);
            switch (result) {
                case 1:
                    mUnk_0f = 1;
                    mUnk_0d = 0x15;
                    break;
                case 2:
                    mUnk_0f = 0;
                    mUnk_0d = 0x15;
                    break;
                default:
                    if (mMenu->UpdateSelection(result)) {
                        u8 target = mMenu->mTarget;
                        if (mUnk_0c != target) {
                            mUnk_0c = target;
                            u32 n   = func_02089a64(target);
                            if (n >= 5) {
                                n = (u8) (n - 1);
                            }
                            mEvent->func_0206f564(n + 6, 10, -1);
                        }
                        mUnk_0d = 0x14;
                    }
                    break;
            }
            break;
        }
        case 20:
            if (mMenu->mWindow->mUnk_061 != 1) {
                mMenu->mWindow->Update(TRUE);
            }
            if (mMenu->Slide()) {
                mUnk_0d = 0x13;
            }
            break;
        case 21:
            SpriteManager::Get().StartFade(3);
            mUnk_0d++;
            break;
        case 22:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->func_020676f8();
            mContext->mUnk_12 = 0;
            mUnk_0d++;
            break;
        case 23:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            Cleanup();
            if (mUnk_0f != 0) {
                mUnk_0d++;
                mEvent = new UnkStruct_02065c40(7, 0, 1);
            } else {
                mUnk_0d = 0x29;
            }
            break;
        case 24: {
            u32 shown;
            u32 idx = func_02089a64(mUnk_0c);
            shown = idx;
            ParkEntry entry;
            func_02089924(mUnk_0c, &entry);
            if (idx >= 5) {
                shown = (u16) (idx - 1);
            }
            u32 kind = 2;
            char name[0x20];
            switch (entry.mType) {
                case 0:
                    sprintf(name, data_ov004_0221dd78);
                    kind = 3;
                    break;
                case 1:
                    sprintf(name, data_ov004_0221dd88);
                    kind = 4;
                    break;
                case 2:
                    sprintf(name, data_ov004_0221dd9c);
                    kind = 5;
                    break;
                case 3:
                    sprintf(name, data_ov004_0221ddb0);
                    kind = 6;
                    break;
                case 4:
                    sprintf(name, data_ov004_0221ddc4);
                    kind = 7;
                    break;
                case 5:
                    sprintf(name, data_ov004_0221ddd8);
                    kind = 8;
                    break;
                case 6:
                    sprintf(name, data_ov004_0221ddec);
                    shown = entry.mUnk_0a_h;
                    if (shown == 0 && (data_020d43f0[0].mUnk_5be & 8)) {
                        shown = 1;
                    }
                    kind = 9;
                    break;
            }
            u16 halves[2] = {data_ov004_0221d7f4, idx};
            u8 bytes[2]   = {entry.mUnk_04, entry.mUnk_05};
            if (!mEvent->func_0206fa20(name, halves, bytes, 2, shown, kind, 2, 1, 1)) {
                break;
            }
            GXS_SetVisiblePlane(0x11);
            SpriteManager::Get().StartFade(2);
            mContext->mUnk_12 = 0;
            mUnk_0d++;
            break;
        }
        case 25:
            if (mEvent->mUnk_14e7 != 0) {
                ParkEntry entry;
                func_02089924(mUnk_0c, &entry);
                switch (entry.mType) {
                    case 0:
                    case 1:
                    case 2:
                        mEvent->mUnk_14f1 = 0;
                        if (entry.mUnk_06 != 0xff) {
                            mUnk_0d = 0x1c;
                        } else {
                            mUnk_0d = 0x1f;
                        }
                        break;
                    case 6:
                        mEvent->mUnk_14f1 = 0;
                        mUnk_0d           = 0x24;
                        break;
                    default:
                        SpriteManager::Get().StartFade(3);
                        mUnk_0d++;
                        break;
                }
            } else if (mEvent->func_02067938()) {
                func_ov004_0221b5b4();
                mEvent->func_02067944();
            }
            break;
        case 26:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->func_020676f8();
            mContext->mUnk_12 = 0;
            mUnk_0d++;
            break;
        case 27:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            func_02089a9c(mUnk_0c);
            Init();
            mUnk_0d = 0x11;
            break;
        case 28: {
            ParkEntry entry;
            func_02089924(mUnk_0c, &entry);
            reinterpret_cast<ParkPopup *>(this)->Open(data_ov004_0221ddfc, (u8) (entry.mUnk_06 + 5), 1);
            ShowWindow();
            mUnk_0d++;
            break;
        }
        case 29:
            mWindow->mCursor->mSelection = -1;
            mWindow->mUnk_061            = 4;
            mWindow->func_0205c7c8(0);
            mUnk_0d++;
            break;
        case 30:
            if ((u8) mWindow->Update(TRUE) != 1) {
                break;
            }
            HideWindow();
            mUnk_0d++;
            break;
        case 31:
            reinterpret_cast<ParkPopup *>(this)->Open(data_ov004_0221de10, 2, 0);
            ShowWindow();
            mUnk_0d++;
            break;
        case 32:
            mWindow->mCursor->mSelection = -1;
            mWindow->mUnk_061            = 4;
            mWindow->func_0205c7c8(1);
            mUnk_0d++;
            break;
        case 33: {
            if ((u8) mWindow->Update(TRUE) != 1) {
                break;
            }
            HideWindow();
            if (mWindow->mCursor->mSelection == 0) {
                u32 idx = func_02089a64(mUnk_0c);
                ParkEntry entry;
                func_02089924(mUnk_0c, &entry);
                switch (entry.mType) {
                    case 0:
                        data_020d433c.mPlayerCount = 2;
                        data_020d433c.mUnk_99      = 3;
                        data_020d433c.mUnk_95      = func_ov004_0221b908(entry.mUnk_08);
                        data_020d433c.mUnk_96      = entry.mUnk_09;
                        data_020d433c.mUnk_98      = 0;
                        data_020d433c.mUnk_9f[0]  = 0x15;
                        data_020d433c.mUnk_a3[0]      = 0;
                        data_020d433c.mUnk_9f[1]      = idx;
                        data_020d433c.mUnk_a3[1]      = func_ov004_0221b8c4(idx);
                        break;
                    case 1:
                        data_020d433c.mPlayerCount = 4;
                        data_020d433c.mUnk_99      = 3;
                        data_020d433c.mUnk_95      = func_ov004_0221b908(entry.mUnk_08);
                        data_020d433c.mUnk_96      = entry.mUnk_09;
                        data_020d433c.mUnk_98      = 1;
                        data_020d433c.mUnk_9f[0]  = 0x15;
                        data_020d433c.mUnk_a3[0]      = 0;
                        data_020d433c.mUnk_9f[1]      = idx;
                        data_020d433c.mUnk_a3[1]      = func_ov004_0221b8c4(idx);
                        data_020d433c.mUnk_9f[2]      = entry.mUnk_0a;
                        data_020d433c.mUnk_a3[2]      = func_ov004_0221b8c4(entry.mUnk_0a);
                        data_020d433c.mUnk_9f[3]      = entry.mUnk_0b;
                        data_020d433c.mUnk_a3[3]      = func_ov004_0221b8c4(entry.mUnk_0b);
                        data_020d433c.mUnk_9b[0]      = 1;
                        data_020d433c.mUnk_9b[1]      = 0;
                        data_020d433c.mUnk_9b[2]      = 3;
                        data_020d433c.mUnk_9b[3]      = 2;
                        break;
                    case 2:
                        data_020d433c.mPlayerCount = 4;
                        data_020d433c.mUnk_99      = 3;
                        data_020d433c.mUnk_95      = func_ov004_0221b908(entry.mUnk_08);
                        data_020d433c.mUnk_96      = entry.mUnk_09;
                        data_020d433c.mUnk_98      = 0;
                        data_020d433c.mUnk_9f[0]  = 0x15;
                        data_020d433c.mUnk_a3[0]      = 0;
                        data_020d433c.mUnk_9f[1]      = idx;
                        data_020d433c.mUnk_a3[1]      = func_ov004_0221b8c4(idx);
                        data_020d433c.mUnk_9f[2]      = entry.mUnk_0a;
                        data_020d433c.mUnk_a3[2]      = func_ov004_0221b8c4(entry.mUnk_0a);
                        data_020d433c.mUnk_9f[3]      = entry.mUnk_0b;
                        data_020d433c.mUnk_a3[3]      = func_ov004_0221b8c4(entry.mUnk_0b);
                        break;
                }
                data_020d433c.mUnk_9a = 0x11;
                data_020d433c.mUnk_9a_0 = data_020d43f0[0].mUnk_28_6;
                if (data_020d43f0[0].mUnk_5bf_0) {
                    if (entry.mUnk_06 == 0x19 && idx == 7) {
                        data_020d433c.mUnk_a3[1] = 2;
                    }
                }
                data_020d433c.mUnk_8b_0 = mUnk_0c;
                data_020d433c.mUnk_8b_4 = data_020d43f0[0].mUnk_5b8;
                mUnk_0d++;
            } else {
                mUnk_0f = 0;
                mUnk_0d++;
            }
            SpriteManager::Get().StartFade(3);
            break;
        }
        case 34:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->func_020676f8();
            mContext->mUnk_12 = 0;
            mUnk_0d++;
            break;
        case 35:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            if (mUnk_0f != 0) {
                mUnk_0d = 0x29;
                break;
            }
            Init();
            mUnk_0d = 0x11;
            break;
        case 36:
            reinterpret_cast<ParkPopup *>(this)->Open(data_ov004_0221de24, 4, 0);
            ShowWindow();
            mUnk_0d++;
            break;
        case 37:
            mWindow->mCursor->mSelection = -1;
            mWindow->mUnk_061            = 4;
            mWindow->func_0205c7c8(1);
            mUnk_0d++;
            break;
        case 38: {
            if ((u8) mWindow->Update(TRUE) != 1) {
                break;
            }
            HideWindow();
            if (mWindow->mCursor->mSelection == 0) {
                ParkEntry entry;
                func_02089924(mUnk_0c, &entry);
                if (entry.mUnk_0a_h == 0) {
                    if (data_020d43f0[0].mUnk_5be & 8) {
                        data_020d433c.mUnk_83 = 4;
                    } else {
                        data_020d433c.mUnk_83 = 3;
                    }
                } else {
                    data_020d433c.mUnk_83 = 4;
                }
                data_020d433c.mUnk_84   = 0;
                data_020d433c.mUnk_82   = 1;
                data_020d433c.mUnk_8b_0 = mUnk_0c;
                mUnk_0f                 = 0xff;
                mUnk_0d++;
            } else {
                mUnk_0f = 0;
                mUnk_0d++;
            }
            SpriteManager::Get().StartFade(3);
            break;
        }
        case 39:
            if (SpriteManager::Get().IsFading()) {
                break;
            }
            mEvent->func_020676f8();
            mContext->mUnk_12 = 0;
            mUnk_0d++;
            break;
        case 40:
            if (mContext->mUnk_12 == 0) {
                break;
            }
            if (mUnk_0f != 0) {
                mUnk_0d = 0x29;
                break;
            }
            Init();
            mUnk_0d = 0x11;
            break;
        case 41:
            if (mUnk_0f != 0) {
                data_020d433c.mUnk_7b = 0xc;
                data_020d433c.mUnk_7c = 1;
                if (mUnk_0f == 0xff) {
                    RequestScene(10);
                    mFinished = TRUE;
                    return;
                }
                RequestScene(5);
                mFinished = TRUE;
                return;
            }
            RequestScene(9);
            mFinished = TRUE;
            return;
    }
    if (mUnk_0d >= 0x13) {
        Draw();
    }
}

void ParkScene::Init() {
    InitDisplay();
    LoadBackground();
    mEvent = new UnkStruct_02065c40(2, 0, 0);
}

void ParkScene::InitDisplay() {
    GX_SetDispSelect(1);
    SpriteManager::Get().func_02026dec(1, 7);
    func_02001e68(1, 0, 1);
    func_02001ed0(0);
    G2_SetBG0Control(0, 1, 0, 1, 1);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    G2S_SetBG2Control(0, 1, 4, 5, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2_SetBG0Priority(3);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(0);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x15);
    G2_SetBlendBrightness(0x1f, -16);
    G2S_SetBlendBrightness(0x1f, -16);
    GX_SetVisibleWnd(0);
    GXS_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    G2S_BlendNone();
}

void ParkScene::LoadWindowGraphics() {
    MI_CpuClearFast((void *) 0x6200000, 0x20000);
    MI_CpuClearFast((void *) 0x6600000, 0x20000);
    G2S_SetBG0Control(0, 0, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 4, 0);
    GXS_SetVisiblePlane(0x13);
    ChangeDir("/Game/2D/union_b/");
    func_02025b64(1, 1, 0, "union_b_bg_window_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "union_b_bg_window_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "union_b_bg_window_b_nsc.bin", 0, 0, 0);
    SpriteScreen &sub = SpriteManager::Get().Screen(1);
    ChangeDir("/Game/2D/union_b/");
    sub.LoadCharacters("union_b_obj_window.nce_ncg1D.bin", 0, 0);
    sub.LoadPalette("union_b_obj_window_ncl.bin", 0, 0);
    ChangeDir("/");
}

void ParkScene::func_ov004_0221b5b4() {
    GXS_SetVisiblePlane(0x11);
    GXS_SetVisibleWnd(0);
    G2S_BlendNone();
    LoadBackground();
}

void ParkScene::LoadBackground() {
    ChangeDir("/Game/Park/");
    func_02025b64(1, 1, 0, "park_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 11, 0, "park_b_bg_01_ncg.bin", 0, 0, 0);
    func_02025b64(1, 7, 0, "park_b_bg_01_nsc.bin", 0, 0, 0);
    func_02025b64(1, 1, 0x4000, "park_b_bg_00_ncl.bin", 0, 0, 0);
    func_02025b64(1, 13, 0, "park_b_bg_00_ncg.bin", 0, 0, 0);
    func_02025b64(1, 9, 0, "park_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/");
    delete mMenu;
    mMenu = new ParkMenu(mUnk_0c);
}

void ParkScene::Cleanup() {
    delete mMenu;
    mMenu = NULL;
    delete mWindow;
    mWindow = NULL;
    if (mUnk_2c != NULL) {
        OS_Free(mUnk_2c);
        mUnk_2c = NULL;
    }
    if (mUnk_1c != NULL) {
        OS_Free(mUnk_1c);
        mUnk_1c = NULL;
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
}

void ParkScene::Draw() {
    if (mMenu != NULL) {
        mMenu->Draw();
    }
    if (mWindowVisible == 0) {
        return;
    }
    mWindow->Draw();
    SpriteManager::Get().Screen(1).func_02027778(mGlyphBuffer, 9, 0x20, 0x28, 0, 0x360, 0xf, 0);
}

u8 ParkScene::func_ov004_0221b8c4(int index) {
    if (TestFlagBit(data_020d4970, index)) {
        return 2;
    }
    return TestFlagBit(data_020d496d, index) ? 1 : 0;
}

int ParkScene::func_ov004_0221b908(int index) {
    if (!TestFlagBit(data_020d4968, index)) {
        index = 2;
        if (!(data_020d43f0[0].mUnk_578 & 4)) {
            index = 0;
        }
    }
    return index;
}

void ParkScene::ShowWindow() {
    GXS_SetVisiblePlane(0x13);
    mWindow->mUnk_064 = 1;
    mWindowVisible = 1;
    G2S_SetWnd0Position(0x10, 0x10, 0xf0, 0xb0);
    GXS_SetVisibleWnd(1);
    G2S_SetWnd0InsidePlane(0x13, FALSE);
    G2S_SetWndOutsidePlane(0x33, FALSE);
    G2S_SetBlendBrightness(0x11, -8);
}


void ParkScene::HideWindow() {
    GXS_SetVisiblePlane(0x11);
    mWindow->mUnk_064 = 0;
    mWindowVisible = 0;
    G2S_SetBlendBrightness(0x11, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWnd0InsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x3f, FALSE);
}
