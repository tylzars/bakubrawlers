#include "global.h"
#include <nitro/fx.h>
#include "types.h"
#include "Scene/SceneBase.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Battle/BattleTarget.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Text/TextManager.hpp"
#include "System/SoundManager.hpp"
#include "System/WirelessManager.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Effect.hpp"
#include "UI/SelectWindow.hpp"
#include "UI/MessageWindow.hpp"
#include "Graphics/AntennaIndicator.hpp"
#include "System/FileSystem.hpp"
#include "System/FrameUpdate.hpp"
#include <nitro/math.h>
#include "Game/Random.hpp"
#include "Battle/BattleCpu.hpp"
#include "Battle/GimmickTypes.hpp"
#include "System/TouchManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "Battle/BattleCpu.hpp"
#include "System/InputManager.hpp"
#include "Graphics/Display.hpp"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include "UI/StorytalkWindow.hpp"
#include "Battle/BattleUnk0227532c.hpp"

BOOL SetMessageTypeAndCheckAll(BattlePlayerList *list, u32 id);

struct ShootPacket {
    u8 mUnk_00;
    u8 mUnk_01;
    u16 mUnk_02;
    u16 mUnk_04;
    u16 mUnk_06;
    u16 mUnk_08;
    u16 mUnk_0a;
    u16 mUnk_0c;
    u8 mUnk_0e;
    u8 mUnk_0f;
    u8 mUnk_10;
};

struct BattleShootCell { // 0xc bytes
    /* 00 */ CellAnimation *mCellAnim;
    /* 04 */ s16 mX;
    /* 06 */ s16 mY;
    /* 08 */ u8 mUnk_08;
    /* 09 */ PAD(0x09, 0x0c);
};

struct BattleShootIcon { // 0x10 bytes
    /* 00 */ CellAnimation *mCellAnim;
    /* 04 */ s16 mX;
    /* 06 */ s16 mY;
    /* 08 */ u16 mRotation;
    /* 0a */ s8 mXPerFrame;
    /* 0b */ s8 mYPerFrame;
    /* 0c */ u8 mTimer;
    /* 0d */ PAD(0x0d, 0x10);
};

struct ShootWords7 {
    u32 mUnk_00[7];
};

struct BattleShootMsg { // 0x20 bytes: a message of the shoot minigame, sent to or received from a player
    /* 00 */ u8 mUnk_00;
    /* 01 */ u8 mUnk_01;
    /* 02 */ u8 mUnk_02;
    /* 03 */ u8 mUnk_03;
    union {
        struct {
            /* 04 */ s32 mUnk_04;
            /* 08 */ s32 mUnk_08;
            /* 0c */ u8 mUnk_0c;
        };
        /* 04 */ ShootWords7 mWords;
    };
};

struct ShootWords3 {
    u32 mUnk_00;
    u32 mUnk_04;
    u32 mUnk_08;
};


struct ShootMsg32 {
    u8 mUnk_00;
    u8 mUnk_01;
    u8 mUnk_02;
    u8 mUnk_03;
    u32 mUnk_04[7];
};

// The shoot minigame (vtable 0228a5a8, 0x61b0 bytes).
class BattleShoot : public SceneBase {
public:
    /* 000c */ u8 mBitmap[0x6000]; // 4bpp 256x192 bitmap in tile order, at +0x0c
    /* 600c */ BattleShootCell mCells[7];
    /* 6060 */ BattleShootIcon mIcons[5];
    /* 60b0 */ BattleShootMsg mMsgs[4];
    /* 6130 */ BattleTarget *mBattleTarget;
    /* 6134 */ SelectWindow *mWindow;
    /* 6138 */ MessageWindow *mMessageWindow;
    /* 613c */ void *mGlyphBuffer;
    /* 6140 */ u32 mGlyphCount;
    /* 6144 */ Camera *mCamera;
    /* 6148 */ PAD(0x6148, 0x6154);
    /* 6154 */ u16 mShotRotX;
    /* 6156 */ u16 mShotRotY;
    /* 6158 */ u16 mShotRotZ;
    /* 615a */ PAD(0x615a, 0x615c);
    /* 615c */ u32 mTouchSumX;
    /* 6160 */ u32 mTouchSumY;
    /* 6164 */ u32 mUnk_6164;
    /* 6168 */ u16 mRotYAdjust;
    /* 616a */ u16 mRotZAdjust;
    /* 616c */ s32 mRotYAdjustPerFrame;
    /* 6170 */ s32 mRotZAdjustPerFrame;
    /* 6174 */ u16 mRotYAdjustFrames;
    /* 6176 */ u16 mRotZAdjustFrames;
    /* 6178 */ u16 mUnk_6178;
    /* 617a */ u16 mUnk_617a;
    /* 617c */ u16 mUnk_617c;
    /* 617e */ u16 mUnk_617e;
    /* 6180 */ u16 mUnk_6180;
    /* 6182 */ u16 mUnk_6182;
    /* 6184 */ u16 mMessageId;
    /* 6186 */ u8 mState;
    /* 6187 */ u8 mDelayTimer;
    /* 6188 */ u8 mBlendAlpha;
    /* 6189 */ u8 mTouchFrames;
    /* 618a */ u8 mPrevTouchFrames;
    /* 618b */ u8 mStartTouchX;
    /* 618c */ u8 mStartTouchY;
    /* 618d */ u8 mPrevTouchX;
    /* 618e */ u8 mPrevTouchY;
    /* 618f */ u8 mTouchX;
    /* 6190 */ u8 mTouchY;
    /* 6191 */ u8 mBallXPerFrame;
    /* 6192 */ u8 mBallYPerFrame;
    /* 6193 */ u8 mShotSpeed;
    /* 6194 */ u8 mUnk_6194;
    /* 6195 */ u8 mUnk_6195;
    /* 6196 */ u8 mSpEnabled;
    /* 6197 */ u8 mUnk_6197;
    /* 6198 */ u8 mSpKind;
    /* 6199 */ u8 mStartTouchInCorner;
    /* 619a */ u8 mMessageY;
    /* 619b */ u8 mMessageTargetY;
    /* 619c */ u8 mUnk_619c;
    /* 619d */ u8 mShootTextVisible;
    /* 619e */ u8 mUnk_619e;
    /* 619f */ u8 mTextWidth;
    /* 61a0 */ u8 mUnk_61a0;
    /* 61a1 */ u8 mPlotEnabled;
    /* 61a2 */ u8 mUnk_61a2;
    /* 61a3 */ u8 mSubPlanes;
    /* 61a4 */ u8 mMicVisible;
    /* 61a5 */ u8 mUnk_61a5;
    /* 61a6 */ s8 mMicTargetY;
    /* 61a7 */ u8 mKeyTimer;
    /* 61a8 */ u8 mReturnState;
    /* 61a9 */ u8 mOtherPlayerCount;
    /* 61aa */ PAD(0x61aa, 0x61b0);
    /* 61b0 */

    BattleShoot(Camera *unk1, u8 unk2);
    BattleShoot(Camera *unk1, const u16 *pos, u16 unk3, u16 unk4, u8 unk5, u8 unk6, u8 unk7);
    virtual ~BattleShoot();
    /* 08 */ virtual void Update();
    void Init(Camera *unk1);
    void InitDisplay();
    void Setup();
    void LoadGraphics();
    void ClearBitmap();
    void func_ov007_0226e884();
    void func_ov007_0226e430();
    void func_ov007_0226f0e8(u8 kind, s32 x, s32 z);
    void func_ov007_0226f378();
    void func_ov007_0226dfb8();
    BOOL func_ov007_0226d23c();
    void UpdateRotAdjustKeys();
    BOOL func_ov007_0226d7ac();
    void UpdateTouch(BOOL canStart);
    void DrawMic();
    void UploadBitmap();
    void PlotLine(u8 x0, u8 y0, u8 x1, u8 y1);
    void PlotDot(u8 x, u8 y);
    void PlotPixel(u8 x, u8 y);
    void func_ov007_0226f8a8();
    BOOL TryStartPopup();
    void SetSubPlanes(u8 value);
    void LoadShootText();
    void DrawShootText();
    void func_ov007_0226d690();
    void func_ov007_0226e234();
    void ApplyBlendAlpha();
    BOOL func_ov007_0226d214();
    u16 GetLengthXY(int a, int b);
    u16 func_ov007_0226e390();
};

extern const u16 gFieldRotZAdjust[];
extern const s8 gDotOffsets[12][2];
void ApplySubPlanes(void *arg);

BattleShoot::BattleShoot(Camera *unk1, u8 unk2)
    : SceneBase(0x3e8, 0) {
    Init(unk1);
    mShotRotX = 0;
    mShotRotY = 0x6000;
    mShotRotZ = 0x38e;
    mUnk_619e = 0;
    mUnk_61a2 = unk2;
    mState = 0;
}

BattleShoot::BattleShoot(Camera *unk1, const u16 *pos, u16 unk3, u16 unk4, u8 unk5, u8 unk6, u8 unk7)
    : SceneBase(0x3e8, 0) {
    Init(unk1);
    mShotRotX = pos[0];
    mShotRotY = pos[1];
    mShotRotZ = pos[2];
    mShotSpeed = unk5;
    mRotYAdjust = unk3;
    mRotZAdjust = unk4;
    mSpKind = unk6;
    mUnk_61a2 = unk7;
    mState = 7;
}

void BattleShoot::Init(Camera *unk1) {
    mKeyTimer = 0;
    mBlendAlpha = 0;
    mShotSpeed = 0;
    mTouchFrames = 0;
    mPrevTouchFrames = 0;
    mStartTouchX = 0;
    mStartTouchY = 0;
    mUnk_6164 = 0;
    mTouchX = 0;
    mTouchY = 0;
    mPrevTouchX = 0;
    mPrevTouchY = 0;
    mBallXPerFrame = 0;
    mBallYPerFrame = 0;
    mTouchSumX = 0;
    mTouchSumY = 0;
    mUnk_6194 = 0;
    mRotYAdjust = 0;
    mRotZAdjust = gFieldRotZAdjust[data_020d433c.mFieldId];
    mUnk_6195 = 1;
    mSpEnabled = 0;
    mUnk_6197 = 0;
    mSpKind = 0;
    mStartTouchInCorner = 0;
    mUnk_6182 = 0x122;
    mCamera = unk1;
    mWindow = 0;
    mBattleTarget = 0;
    mMessageWindow = 0;
    mRotYAdjustPerFrame = 0x5b;
    mRotZAdjustPerFrame = 0xb6;
    mRotYAdjustFrames = 0;
    mRotZAdjustFrames = 0;
    mUnk_6178 = 0;
    mUnk_617a = 0;
    mUnk_617c = 0;
    mUnk_617e = 0;
    mUnk_6180 = 0;
    mUnk_61a0 = 1;
    mPlotEnabled = 1;
    mUnk_61a5 = 0;
    mMicVisible = 0;
    mMicTargetY = 0x30;
    mMessageId = 0;
    mMessageY = 0x80;
    mMessageTargetY = 0x80;
    mGlyphBuffer = 0;
    mGlyphCount = 0;
    mUnk_619c = 0;
    mShootTextVisible = 0;
    mTextWidth = 0;
    mUnk_61a2 = 0;
    if (data_020d433c.mBattlePlayers->mTurnPlayer == data_020d433c.mBattlePlayers->mUnk_2a9) {
        mUnk_6197 = 1;
    }
    MI_CpuClear32(mCells, 0x54);
    MI_CpuClear32(mIcons, 0x50);
    MI_CpuClear32(mMsgs, 0x80);
    mOtherPlayerCount = data_020d433c.mPlayerCount - 1;
}

BattleShoot::~BattleShoot() {
    u8 i = 0;
    do {
        CellAnimation *a = mCells[i].mCellAnim;
        if (a) {
            delete a;
        }
        i++;
    } while (i < 7);
    u8 j = 0;
    do {
        CellAnimation *a = mIcons[j].mCellAnim;
        if (a) {
            delete a;
        }
        j++;
    } while (j < 5);
    if (mGlyphBuffer) {
        OS_Free(mGlyphBuffer);
    }
    data_020d433c.mEffects->FreeAllEffects();
    FreeEffectGroup(data_020d433c.mEffects, 0);
    data_020d433c.mBattlePlayers->mBattleTarget = 0;
    if (mWindow) {
        delete mWindow;
    }
    if (mMessageWindow) {
        delete mMessageWindow;
    }
    G2_BlendNone();
    G2S_BlendNone();
}

static inline bool CpuActive(BattleCpu *cpu) {
    bool active = false;
    if (cpu) {
        active = true;
    }
    return active;
}

void BattleShoot::Update() {
    UpdateTouch(mState == 12);
    switch (mState) {
        case 0:
            Setup();
            mDelayTimer = 4;
            mState++;
            break;
        case 1:
            mDelayTimer--;
            if (mDelayTimer == 0) {
                if (data_020d433c.mTutorialId == 3) {
                    mWindow->mEnabled = 0;
                } else {
                    mWindow->mEnabled = 1;
                }
                SpriteManager::Get().StartFade(2);
                u8 mode = data_020d433c.mTutorialId;
                if (mode >= 3 && mode <= 12) {
                    data_020d433c.mBattlePlayers->mUnk_2a4->LoadGraphics();
                    data_020d433c.mBattlePlayers->mUnk_2a4->SetMessage(data_020d433c.mBattlePlayers->mUnk_2af, 0x64, 0, 0);
                    G2S_SetBlendBrightness(0x1f, -10);
                    mState = 2;
                } else {
                    mState = 3;
                }
            }
            break;
        case 2: {
            mCells[0].mX = (mCells[0].mX + 0x80) / 2;
            mCells[0].mY = (mCells[0].mY + 0xa0) / 2;
            if (InputManager::Get().IsHeld(1) || TouchManager::Get().mTouching) {
                data_020d433c.mBattlePlayers->mUnk_2a4->NextPage(1);
            }
            u8 limit;
            if (data_020d433c.mUnk_83 == 3) {
                limit = 2;
            } else if (data_020d433c.mUnk_83 == 2) {
                limit = 2;
            } else {
                limit = 4;
            }
            if (!data_020d433c.mBattlePlayers->mUnk_2a4->func_02087cac()) {
                data_020d433c.mBattlePlayers->mUnk_2af++;
                if (data_020d433c.mBattlePlayers->mUnk_2af > limit) {
                    mState = 3;
                } else {
                    data_020d433c.mBattlePlayers->mUnk_2a4->SetMessage(data_020d433c.mBattlePlayers->mUnk_2af, 0x64, 0, 0);
                }
            }
            break;
        }
        case 3:
            SetSubPlanes(0x17);
            G2S_SetBlendBrightness(0x1f, 0);
            mState++;
            break;
        case 4:
            if (TryStartPopup()) {
                mUnk_61a0 = 0;
                break;
            }
            if (data_020d433c.mTutorialId != 3) {
                mWindow->Draw();
            }
            if (func_ov007_0226d23c()) {
                mWindow->mEnabled = 0;
                func_ov007_0226dfb8();
                mPlotEnabled = 0;
                mState++;
                break;
            }
            if (func_ov007_0226d214()) {
                SpriteManager::Get().StartFade(3);
                mState = 0x10;
                func_ov007_0226f378();
                return;
            }
            break;
        case 5:
            if (mCells[0].mY > -0x40) {
                mCells[0].mX += (s8) mBallXPerFrame;
                mCells[0].mY += (s8) mBallYPerFrame;
            } else {
                SetSubPlanes(0x13);
                WirelessManager::Get();
                if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                    mState = 6;
                } else {
                    mMicVisible = 1;
                    mUnk_619c = 3;
                    mShootTextVisible = 0;
                    if (data_020d433c.mUnk_83 != 0 || mUnk_61a2 != 0) {
                        mShootTextVisible = 1;
                    }
                    mState = 11;
                }
            }
            break;
        case 6: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            ((ShootPacket *) &data_0220f540)->mUnk_04     = mShotRotX;
            ((ShootPacket *) &data_0220f540)->mUnk_06     = mShotRotY;
            ((ShootPacket *) &data_0220f540)->mUnk_08     = mShotRotZ;
            ((ShootPacket *) &data_0220f540)->mUnk_0a     = mRotYAdjust;
            ((ShootPacket *) &data_0220f540)->mUnk_0c     = mRotZAdjust;
            ((ShootPacket *) &data_0220f540)->mUnk_0f     = mShotSpeed;
            ((ShootPacket *) &data_0220f540)->mUnk_10     = mSpKind;
            ((ShootPacket *) &data_0220f540)->mUnk_0e           = list->mPlayers[list->mTurnPlayer]->mRecord;
            if (SetMessageTypeAndCheckAll(list, 8)) {
                mState = 10;
            }
            break;
        }
        case 7:
            Setup();
            SetSubPlanes(0x11);
            mDelayTimer = 4;
            mState++;
            break;
        case 8:
            mDelayTimer--;
            if (mDelayTimer == 0) {
                SpriteManager::Get().StartFade(2);
                mState++;
            }
            break;
        case 9: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            ((ShootPacket *) &data_0220f540)->mUnk_04     = mShotRotX;
            ((ShootPacket *) &data_0220f540)->mUnk_06     = mShotRotY;
            ((ShootPacket *) &data_0220f540)->mUnk_08     = mShotRotZ;
            ((ShootPacket *) &data_0220f540)->mUnk_0a     = mRotYAdjust;
            ((ShootPacket *) &data_0220f540)->mUnk_0c     = mRotZAdjust;
            ((ShootPacket *) &data_0220f540)->mUnk_0f     = mShotSpeed;
            ((ShootPacket *) &data_0220f540)->mUnk_10     = mSpKind;
            ((ShootPacket *) &data_0220f540)->mUnk_0e     = 0;
            if (SetMessageTypeAndCheckAll(list, 8)) {
                mState++;
            }
            break;
        }
        case 10: {
            BattlePlayerList *list = data_020d433c.mBattlePlayers;
            BattleCpu *cpu = list->mPlayers[list->mTurnPlayer]->mCpu;
            if (CpuActive(cpu)) {
                u8 kind = mSpKind;
                if (kind == 1 || kind == 3 || kind == 4) {
                    mRotZAdjust = 0;
                    mRotYAdjust = func_ov007_0226e390();
                    mShotRotY = 0x6000;
                    mShotRotZ = 0;
                    *(u16 *) ((u8 *) mBattleTarget + 0x1b8) = mRotYAdjust;
                    *(u16 *) ((u8 *) mBattleTarget + 0x1ba) = mRotZAdjust;
                } else if (kind == 2) {
                    cpu->StartJumpObjTargets(0x14000);
                }
            }
            u8 idx = data_020d433c.mBattlePlayers->mTurnPlayer;
            data_020d433c.mBattlePlayers->mPlayers[idx]->mUnk_ec = mShotSpeed;
            BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[idx];
            p->mUnk_d4 = 0;
            p->mUnk_d8 = 0;
            p = data_020d433c.mBattlePlayers->mPlayers[idx];
            p->mUnk_cc[0]              = mShotRotX;
            p->mUnk_cc[1]              = mShotRotY;
            p->mUnk_cc[2]              = mShotRotZ;
            mBattleTarget->mSpKind        = mSpKind;
            mBattleTarget->StartShot();
            if (mSpKind == 0) {
                mUnk_6182 = 0x122;
            }
            mUnk_619e = 0x14;
            mMicVisible = 1;
            if (data_020d433c.mTutorialId != 0 && data_020d433c.mBattlePlayers->mTurnPlayer != 0) {
                mMicVisible = 0;
            }
            mPlotEnabled = 0;
            mUnk_619c = 3;
            mShootTextVisible = 0;
            if (data_020d433c.mUnk_83 != 0) {
                mUnk_619c = 0;
            } else if (mUnk_61a2 != 0) {
                mShootTextVisible = 1;
            }
            if (mSpKind == 5) {
                mUnk_619c = 0;
                mShootTextVisible = 0;
            }
            mState++;
            break;
        }
        case 11: {
            BattlePlayerList *l = data_020d433c.mBattlePlayers;
            if (l->mPlayers[l->mTurnPlayer]->mCpu == 0) {
                u8 i = 0;
                do {
                    BattlePlayerList *l3 = data_020d433c.mBattlePlayers;
                    if (l3->mPlayers[i]->mCpu != 0 && i != l3->mTurnPlayer) {
                        l3->mPlayers[i]->mCpu->RandomizeOffsets();
                    }
                    i++;
                } while (i < data_020d433c.mPlayerCount);
            }
            mState++;
            break;
        }
        case 12:
            if (!TryStartPopup()) {
                if (mContext->mRequest != 0) {
                    mUnk_61a0 = 0;
                    mMicTargetY = 0x30;
                    mUnk_6182 = 0x122;
                    mState = 15;
                } else {
                    if (mUnk_61a0 != 0 && mBattleTarget->mUnk_1d7 != 0) {
                        u8 i = 0;
                        do {
                            mIcons[i].mTimer = 0;
                            i++;
                        } while (i < 5);
                        mUnk_61a0 = 0;
                        mMicTargetY = 0x30;
                        mUnk_6182 = 0x122;
                    }
                    if (mBattleTarget->func_ov007_0225d078()) {
                        G2S_SetBG1Priority(3);
                        mMicTargetY = 0x30;
                    }
                    if (mBattleTarget->mUnk_1cb == 0) {
                        BattlePlayerList *l = data_020d433c.mBattlePlayers;
                        if (l->mPlayers[l->mTurnPlayer]->IsActive()) {
                            func_ov007_0226e430();
                        }
                        func_ov007_0226e234();
                        func_ov007_0226e884();
                    }
                    func_ov007_0226d690();
                }
            }
            break;
        case 13:
            new BattleUnk0227532c(3);
            mContext->mRequest = 0;
            mState++;
            break;
        case 14:
            switch (mContext->mRequest) {
                case 1:
                    mState = mReturnState;
                    if (mState == 4) {
                        mUnk_61a0 = 1;
                        mPlotEnabled = 1;
                        mState = 3;
                    } else {
                        SetSubPlanes(0x11);
                    }
                    ClearBitmap();
                    data_020d433c.mUnk_14 &= ~8;
                    mContext->mRequest = 0;
                    break;
                case 2:
                    if (data_020d433c.mWireless != 0) {
                        WirelessManager::Get();
                        data_020c4030.mUnk_22a = 6;
                        mState++;
                    } else {
                        SpriteManager::Get().StartFade(3);
                        mState = 0x10;
                    }
                    break;
            }
            break;
        case 15:
            u16 scene = mContext->mRequest;
            if (scene != 0) {
                RequestScene(scene);
                mFinished = 1;
            }
            break;
        case 16:
            if (!SpriteManager::Get().IsFading()) {
                mBattleTarget->func_ov007_02253fbc();
                mState = 15;
                data_020d433c.mUnk_14 &= ~8;
            }
            break;
    }
    func_ov007_0226f378();
    if (mUnk_61a0 != 0 && mState != 14) {
        func_ov007_0226f8a8();
    }
    if (mMicVisible != 0) {
        DrawMic();
    }
}

void BattleShoot::InitDisplay() {
    reg_GX_POWCNT |= 0x8000;
    SpriteManager::Get().SetupMode(1, 7);
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(3);
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG1Control(0, 1, 2, 1, 0);
    G2S_SetBG2Control(0, 0, 4, 6, 0);
    G2S_SetBG3Control(1, 0, 6, 6, 0);
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2_SetBG0Priority(2);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(2);
    G2S_SetBG2Priority(1);
    G2S_SetBG3Priority(1);
    GX_SetVisiblePlane(0x11);
    GXS_SetVisiblePlane(0x17);
    G2_SetBlendBrightness(0x1f, 0);
    G2S_SetBlendBrightness(0x1f, 0);
    GX_SetVisibleWnd(0);
    G2_SetWndOutsidePlane(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_SetWndOutsidePlane(0x1f, 0);
    G2_BlendNone();
    G2S_BlendNone();
    ApplyBlendAlpha();
}

static const WindowCellRecord sShootCells[1] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 2, 0x6000, 5, 0, 1, {2, 3, 3, 0}},
};

static const WindowItemRecord sShootItems[22] = { // only the first entry is used
    {0, 2, 225, 170, 30, 20, 0, {-1, -1, -1, -1}, 0, 1},
};

void BattleShoot::Setup() {
    InitDisplay();
    LoadGraphics();
    LoadShootText();
    u8 player = data_020d433c.mBattlePlayers->mTurnPlayer;
    u8 record = data_020d433c.mBattlePlayers->mPlayers[player]->mRecord;
    delete mBattleTarget;
    mBattleTarget                              = new BattleTarget(mCamera, player, record);
    data_020d433c.mBattlePlayers->mBattleTarget = mBattleTarget;
    mContext->mRequest                      = 0;
    mBattleTarget->mRotYAdjust                    = mRotYAdjust;
    mBattleTarget->mRotZAdjust                    = mRotZAdjust;
}

void BattleShoot::LoadGraphics() {
    char buffer[0x80];
    u8 player;
    u8 record;
    LoadEffectGroup(data_020d433c.mEffects, 0, "jammer");
    player = data_020d433c.mBattlePlayers->mTurnPlayer;
    record = data_020d433c.mBattlePlayers->mPlayers[player]->RecordIndex();
    ChangeDir("/Game/Battle/Bg/");
    LoadAndQueueUpload(1, 1, 0, "battle_shoot_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 0xb, 0, "battle_shoot_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 7, 0, "battle_shoot_b_bg_00_b_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 1, 0x2000, "battle_shoot_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 8, 0, "battle_shoot_b_bg_00_a_nsc.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 6, 0, "pentest_pen_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 9, 0, "pentest_pen_nsc.bin", 0, 0, 0);
    ClearBitmap();
    u8 id = data_020d433c.mBattlePlayers->mPlayers[player]->mRecords[record].mAttribute;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Battle/Obj/");
    sprintf(buffer, "battle_shoot_b_ball_%02d.nce_ncg1D.bin", id);
    screen.LoadCharacters(buffer, 0, 0);
    sprintf(buffer, "battle_shoot_b_ball_%02d_ncl.bin", id);
    screen.LoadExtPalette(buffer, 0, 0);
    sprintf(buffer, "battle_shoot_b_ball_%02d.nce_ncg1D.bin", id);
    screen.LoadCharacters("battle_shoot_b_ball_sp.nce_ncg1D.bin", 0x2000, 0);
    void *palette = LoadFile("battle_shoot_b_ball_sp_ncl.bin", 0, id << 5, 0x20, 0, 0);
    QueueUpload(screen.mBgIndex, 5, 0, palette, 0x20, 1);
    mCells[0].mCellAnim = new CellAnimation("battle_shoot_b_ball_00_nce.bin", 1, 1, 1);
    mCells[0].mCellAnim->mCharBase = screen.GetCharBase(0);
    mCells[0].mCellAnim->mPaletteBase   = 0;
    mCells[0].mX               = 0x80;
    mCells[0].mY               = -0x40;
    mCells[0].mUnk_08          = 1;
    mCells[1].mCellAnim = new CellAnimation("battle_shoot_b_ball_sp_nce.bin", 0, 1, 1);
    mCells[1].mCellAnim->mCharBase = screen.GetCharBase(0x2000);
    mCells[1].mCellAnim->mPaletteBase   = 0;
    mCells[1].mX               = 0x80;
    mCells[1].mY               = -0x40;
    mCells[1].mUnk_08          = 1;
    ChangeDir("/Game/2D/union_b/");
    screen.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0x6000, 0);
    screen.LoadPalette("union_b_obj_cockpit_ncl.bin", 0xa0, 0);
    screen.LoadCharacters("union_b_obj_mic.nce_ncg1D.bin", 0x7000, 0);
    screen.LoadPalette("union_b_obj_mic_ncl.bin", 0xc0, 0);
    mCells[4].mCellAnim = new CellAnimation("union_b_obj_mic_nce.bin", 1, 1, 1);
    mCells[4].mCellAnim->mCharBase = screen.GetCharBase(0x7000);
    mCells[4].mCellAnim->mPaletteBase   = 6;
    mCells[4].mX               = 0x80;
    mCells[4].mY               = 0x30;
    mCells[4].mUnk_08          = 2;
    if (data_020d433c.mUnk_9a_0 && data_020d433c.mUnk_83 == 0) {
        mCells[4].mCellAnim->SetAnimation(0);
    }
    mMicTargetY = 0x60;
    ChangeDir("/Game/Battle/Obj/");
    screen.LoadCharacters("battle_shoot_b_effect.nce_ncg1D.bin", 0xf000, 0);
    screen.LoadPalette("battle_shoot_b_effect_ncl.bin", 0x140, 0);
    u8 i = 0;
    do {
        mIcons[i].mCellAnim = new CellAnimation("battle_shoot_b_effect_nce.bin", 0, 1, 1);
        mIcons[i].mCellAnim->mCharBase = screen.GetCharBase(0xf000);
        mIcons[i].mCellAnim->mPaletteBase   = 10;
        mIcons[i].mTimer          = 0;
        i++;
    } while (i < 5);
    mCells[5].mCellAnim = new CellAnimation("battle_shoot_b_effect_nce.bin", 1, 1, 1);
    mCells[5].mCellAnim->mCharBase = screen.GetCharBase(0xf000);
    mCells[5].mCellAnim->mPaletteBase   = 10;
    mCells[6].mCellAnim = new CellAnimation("battle_shoot_b_effect_nce.bin", 2, 1, 1);
    mCells[6].mCellAnim->mCharBase = screen.GetCharBase(0xf000);
    mCells[6].mCellAnim->mPaletteBase   = 10;
    sprintf(buffer, "battle_shoot_b_obj_%02d.nce_ncg1D.bin", id);
    screen.LoadCharacters(buffer, 0x4000, 0);
    sprintf(buffer, "battle_shoot_b_obj_%02d_ncl.bin", id);
    screen.LoadPalette(buffer, 0x20, 0);
    sprintf(buffer, "battle_shoot_b_obj_%02d_nce.bin", id);
    mCells[2].mCellAnim = new CellAnimation(buffer, 0, 1, 1);
    mCells[2].mCellAnim->mCharBase = screen.GetCharBase(0x4000);
    mCells[2].mCellAnim->mPaletteBase   = 1;
    mCells[2].mX               = 0x122;
    mCells[2].mY               = 0x18;
    mCells[2].mUnk_08          = 1;
    bool flag = false;
    if (data_020d433c.mBattlePlayers->Current()->mUnk_f3 >= 0xbe) {
        u8 kind = data_020d433c.mBattlePlayers->Current()->CurrentRecord()->mBakuganId;
        if ((kind == 0x24) | (kind == 0x25)) {
            flag = true;
        } else if ((u8) data_020d433c.mBattlePlayers->Current()->CurrentRecord()->mUnk_00_0 >= 2) {
            flag = true;
        }
    }
    if (!flag) {
        mSpEnabled = 0;
    } else {
        mCells[2].mCellAnim->SetAnimation(2);
        mSpEnabled = 1;
        mUnk_6182 = 0xdc;
    }
    ChangeDir("/Game/Confrontation/Obj/");
    sprintf(buffer, "vs_b_obj_pen.nce_ncg1D.bin", id);
    screen.LoadCharacters(buffer, 0xe000, 0);
    sprintf(buffer, "vs_cutin_perfect_ncl.bin", id);
    screen.LoadPalette(buffer, 0x120, 0);
    sprintf(buffer, "vs_b_obj_pen_nce.bin", id);
    mCells[3].mCellAnim = new CellAnimation(buffer, 4, 1, 1);
    mCells[3].mCellAnim->mCharBase = screen.GetCharBase(0xe000);
    mCells[3].mCellAnim->mPaletteBase   = 9;
    mCells[3].mX               = 0x80;
    mCells[3].mY               = 0x60;
    mCells[3].mUnk_08          = 1;
    ChangeDir("/");
    mWindow = new SelectWindow(1, sShootItems, 0);
    mWindow->SetCells(1, sShootCells);
    mWindow->mCursor->mSelection = -1;
    mWindow->Update(1);
    mMessageWindow = new MessageWindow("mes_CardName", 1, 0x9000);
    mMessageWindow->func_02065bd4(0x20, 0xa2, 2);
    mMessageWindow->SetWidth(0xe0);
    mMessageWindow->StartMessage(0, 0, 1);
    if (data_020d433c.mUnk_10) {
        data_020d433c.mUnk_10->SetupLayout(9);
    }
}

void BattleShoot::LoadShootText() {
    MessageFile *file = (MessageFile *) LoadFile("/font/mes_shoot.mes", 0, 0, 0, 0, 0);
    u32 size          = TextManager::Get().BufferSize(0x18, 0xa);
    void *buffer      = OS_Alloc(size);
    MI_CpuClear16(buffer, size);
    mGlyphCount = TextManager::Get().GlyphCount(0x1e);
    mGlyphBuffer = OS_Alloc(mGlyphCount * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 0xa, 0x21);
    u16 pixelWidth = 0;
    TextManager::Get().mUse4bppFont = 1;
    TextManager &text = TextManager::Get();
    s16 height;
    s16 width;
    width  = 0;
    height = 0;
    text.Print(file->GetText(0), file->GetLength(0), buffer, 0x18, 0xa, &height, &width, &pixelWidth, 0, 2, 0x21, 0x10, 0, 0, 0, 1, 1);
    TextManager::Get().mUse4bppFont = 0;
    mTextWidth = pixelWidth;
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x14000, buffer, size, 0);
    if (file) {
        OS_Free(file);
    }
    if (buffer) {
        OS_Free(buffer);
    }
}

static inline u16 KeysReleased(InputManager &input, u16 keys) {
    return input.mReleased & keys;
}

void BattleShoot::DrawShootText() {
    SpriteManager::Get().Screen(1).AddOam(mGlyphBuffer, mGlyphCount, (s16) (0x80 - ((u32) mTextWidth >> 1)), 0x50, *(u8 *) ((u8 *) this + 0x6038), 0x280, 0xf, 0);
}

void BattleShoot::UpdateTouch(BOOL canStart) {
    mPrevTouchFrames = mTouchFrames;
    if (canStart) {
        if (mKeyTimer != 0) {
            mKeyTimer--;
        }
    } else {
        mKeyTimer = 0;
    }
    if (mTouchFrames != 0) {
        if (TouchManager::Get().mDown != 0) {
            mPrevTouchX = mTouchX;
            mPrevTouchY = mTouchY;
            mTouchX = TouchManager::Get().mX;
            mTouchY = TouchManager::Get().mY;
            BOOL draw = FALSE;
            if (mTouchFrames < 0x1e) {
                mTouchFrames++;
            }
            if (mPrevTouchFrames != 0) {
                if (mStartTouchInCorner == 0) {
                    draw = TRUE;
                }
            }
            if (draw) {
                PlotLine(mTouchX, mTouchY, mPrevTouchX, mPrevTouchY);
            }
        } else {
            mTouchFrames = 0;
            ClearBitmap();
        }
    } else {
        mStartTouchInCorner = 0;
        if (TouchManager::Get().mDown != 0) {
            if (TouchManager::Get().mPressed != 0) {
                mTouchX = TouchManager::Get().mX;
                mTouchY = TouchManager::Get().mY;
            }
            if ((mTouchX != TouchManager::Get().mX) | (mTouchY != TouchManager::Get().mY)) {
                mTouchX = TouchManager::Get().mX;
                mTouchY = TouchManager::Get().mY;
                mPrevTouchX = mTouchX;
                mPrevTouchY = mTouchY;
                mStartTouchX = mTouchX;
                mStartTouchY = mTouchY;
                mTouchFrames++;
                if (mSpEnabled != 0) {
                    if ((mTouchX > 0xc8) & (mTouchY < 0x30)) {
                        mStartTouchInCorner = 1;
                    }
                }
                if ((mTouchY > 0xa0) & (mTouchX > 0xc8)) {
                    mStartTouchInCorner = 1;
                }
            }
        } else if (canStart) {
            if (mKeyTimer == 0) {
                mStartTouchX = mPrevTouchX = 0x80;
                mStartTouchY = mPrevTouchY = 0x60;
                mTouchX = 0x80;
                mTouchY = 0x60;
                if (InputManager::Get().IsDown(KEY_LEFT | 0x800)) {
                    mStartTouchX = mPrevTouchX = 0xa8;
                    mTouchX = 0x58;
                    mPrevTouchFrames = mTouchFrames = 1;
                    mKeyTimer = 0xf;
                } else if (InputManager::Get().IsDown(KEY_RIGHT | 1)) {
                    mStartTouchX = mPrevTouchX = 0x58;
                    mTouchX = 0xa8;
                    mPrevTouchFrames = mTouchFrames = 1;
                    mKeyTimer = 0xf;
                }
                if (InputManager::Get().IsDown(KEY_UP | 0x400)) {
                    mStartTouchY = mPrevTouchY = 0x88;
                    mTouchY = 0x38;
                    mPrevTouchFrames = mTouchFrames = 1;
                    mKeyTimer = 0xf;
                } else if (InputManager::Get().IsDown(KEY_DOWN | 2)) {
                    mStartTouchY = mPrevTouchY = 0x38;
                    mTouchY = 0x88;
                    mPrevTouchFrames = mTouchFrames = 1;
                    mKeyTimer = 0xf;
                }
            }
        }
    }
}

BOOL BattleShoot::func_ov007_0226d214() {
    return (u8) mWindow->Update(1) == 2;
}

struct BattleShootCurve { // pairs at gShootCurves
    u8 mNumerator;
    u8 mDenominator;
};
extern const BattleShootCurve gShootCurves[5];

BOOL BattleShoot::func_ov007_0226d23c() {
    BOOL moving = FALSE;
    if (mPrevTouchFrames != 0 && mTouchFrames != 0) {
        if (mStartTouchInCorner == 0) {
            moving = TRUE;
        }
    }
    if (moving) {
        mCells[0].mX = (mCells[0].mX + mTouchX) / 2;
        mCells[0].mY = (mCells[0].mY + mTouchY) / 2;
    } else {
        mCells[0].mX = (mCells[0].mX + 0x80) / 2;
        mCells[0].mY = (mCells[0].mY + 0xa0) / 2;
    }
    if (func_ov007_0226d7ac()) {
        return 0;
    }
    if (mPrevTouchFrames != 0) {
        if (mTouchFrames != 0) {
            if (mPrevTouchY <= mTouchY) {
                mTouchFrames = 1;
                mStartTouchX = mTouchX;
                mStartTouchY = mTouchY;
                ClearBitmap();
            }
        } else {
          if (mStartTouchInCorner != 0) {
            if (((mTouchX > 0xc8) & (mTouchY < 0x30)) && mSpEnabled != 0) {
                u8 record = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mTurnPlayer]->mRecords[data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mTurnPlayer]->mRecord].mAttribute;
                if (mSpKind == 0) {
                    mCells[2].mCellAnim->SetAnimation(0);
                    mCells[0].mCellAnim->SetAnimation(2);
                    mSpKind = record + 1;
                    SoundManager::Get().StartSeqArc(0);
                } else {
                    mCells[2].mCellAnim->SetAnimation(2);
                    mCells[0].mCellAnim->SetAnimation(1);
                    mSpKind = 0;
                    SoundManager::Get().StartSeqArc(1);
                }
                mBattleTarget->mSpKind = mSpKind;
                mTouchFrames = 0;
                return 0;
            }
          } else {
            s32 dist = GetLengthXY((s16) (mStartTouchX - mTouchX), (s16) (mStartTouchY - mTouchY));
            if (dist > 0x14 && mPrevTouchY >= mTouchY) {
                s32 dx = (mTouchX - mStartTouchX) / 8;
                if (dx > 8) {
                    dx = 8;
                } else if (dx < -8) {
                    dx = -8;
                }
                s32 dx2 = mTouchX - mPrevTouchX;
                if (dx2 > 8) {
                    dx2 = 8;
                } else if (dx2 < -8) {
                    dx2 = -8;
                }
                mBallXPerFrame = (dx2 + dx) / 2;
                s32 dy = (mTouchY - mStartTouchY) / 8;
                if (dy > -8) {
                    dy = -8;
                } else if (dy < -16) {
                    dy = -16;
                }
                s32 dy2 = mTouchY - mPrevTouchY;
                if (dy2 > -8) {
                    dy2 = -8;
                } else if (dy2 < -16) {
                    dy2 = -16;
                }
                mBallYPerFrame = (dy2 + dy) / 2;
                return 1;
            }
          }
          mTouchFrames = 0;
        }
    } else {
        UpdateRotAdjustKeys();
        mBattleTarget->mRotYAdjust = mRotYAdjust;
        mBattleTarget->mRotZAdjust = mRotZAdjust;
    }
    if (InputManager::Get().IsHeld(0x400)) {
        mUnk_6195 = (mUnk_6195 + 1) & 1;
        mBattleTarget->func_ov007_02254928(mUnk_6195);
    }
    return 0;
}

void BattleShoot::func_ov007_0226d690() {
    if (mBattleTarget->mUnk_1e5 != 0) {
        mUnk_6180               = 0;
        mBattleTarget->mUnk_1e5 = 0;
    }
    if (InputManager::Get().IsDown(0x200) | InputManager::Get().IsDown(0x100)) {
        if (mUnk_6180 < 0x1e) {
            mUnk_6180++;
        }
    } else {
        if (mUnk_6180 != 0) {
            mUnk_6180--;
        }
    }
    u16 value            = mUnk_6180;
    s32 spread           = value * 0x1800;
    BattleTarget *target = mBattleTarget;
    target->mUnk_188.x   = spread;
    target->mUnk_188.y   = value << 12;
    target->mUnk_188.z   = spread;
}

extern "C" void MTX_ApplyRotZ43(MtxFx43 *mtx, s32 angle);
extern const u8 gPlayerEffectIds[];
extern const VecFx32 data_ov007_0227bcac;

BOOL BattleShoot::func_ov007_0226d7ac() {
    if (InputManager::Get().IsDown(0x200) | InputManager::Get().IsDown(0x100)) {
        if (mUnk_617e < 0x14) {
            mUnk_617e++;
        }
    } else {
        if (mUnk_617e != 0) {
            mUnk_617e--;
        }
    }
    if (mUnk_617e != 0) {
        mRotZAdjustFrames = 0;
        mRotYAdjustFrames = 0;
        mBattleTarget->mCameraMode = 1;
        if (InputManager::Get().IsDown(KEY_LEFT)) {
            mUnk_6178 -= mUnk_617a;
            mUnk_617c++;
            if (mUnk_617c == 0) {
                mUnk_617c = 0xffff;
            }
        }
        if (InputManager::Get().IsDown(KEY_RIGHT)) {
            mUnk_6178 += mUnk_617a;
            mUnk_617c++;
            if (mUnk_617c == 0) {
                mUnk_617c = 0xffff;
            }
        }
        if (KeysReleased(InputManager::Get(), KEY_LEFT) | KeysReleased(InputManager::Get(), KEY_RIGHT)) {
            mUnk_617c = 0;
        }
        if (mUnk_617c >= 0x19) {
            mUnk_617a = (mUnk_617c - 0x19) / 2 * 0x12 + 0x5b;
            if (mUnk_617a > 0x12 + 0xff) {
                mUnk_617a = 0x12 + 0xff;
            }
        } else {
            mUnk_617a = 0x5b;
        }
        mBattleTarget->mUnk_1b0 = mUnk_6178;
        mBattleTarget->mUnk_17c = mUnk_617e - 1;
        ClearBitmap();
        return 1;
    }
    return 0;
}

extern const s32 data_ov007_0227bcc4[3];
struct BattleShootRange { // at data_ov007_0227bcf8
    u32 mLow;
    u32 mHigh;
};
extern const BattleShootRange data_ov007_0227bcf8[3];
extern MtxFx33 data_0220e848;
extern "C" void MTX_ApplyRotY33(MtxFx33 *m, u16 angle);
extern "C" void MTX_MultVec33(VecFx32 *v, MtxFx33 *m, VecFx32 *out);

void BattleShoot::UpdateRotAdjustKeys() {
    mUnk_617c = 0;
    mBattleTarget->mCameraMode = 0;
    if (InputManager::Get().IsDown(KEY_LEFT)) {
        if ((u16) (mRotYAdjust + 0x8000 - mRotYAdjustPerFrame) > 0x6aab) {
            mRotYAdjust = mRotYAdjust - mRotYAdjustPerFrame;
            mRotYAdjustFrames++;
            if (mRotYAdjustFrames == 0) {
                mRotYAdjustFrames = 0xffff;
            }
        } else {
            mRotYAdjust = 0xeaab;
        }
    } else if (InputManager::Get().IsDown(KEY_RIGHT)) {
        if ((u16) (mRotYAdjustPerFrame + (mRotYAdjust + 0x8000)) < 0x9555) {
            mRotYAdjust += mRotYAdjustPerFrame;
            mRotYAdjustFrames++;
            if (mRotYAdjustFrames == 0) {
                mRotYAdjustFrames = 0xffff;
            }
        } else {
            mRotYAdjust = 0x1555;
        }
    }
    if (KeysReleased(InputManager::Get(), KEY_LEFT) | KeysReleased(InputManager::Get(), KEY_RIGHT)) {
        mRotYAdjustFrames = 0;
    }
    if (InputManager::Get().IsDown(KEY_UP)) {
        if ((u16) (mRotZAdjust + 0x8000 - mRotZAdjustPerFrame) > 0x8000) {
            mRotZAdjust = mRotZAdjust - mRotZAdjustPerFrame;
            mRotZAdjustFrames++;
            if (mRotZAdjustFrames == 0) {
                mRotZAdjustFrames = 0xffff;
            }
        } else {
            mRotZAdjust = 0;
        }
    } else if (InputManager::Get().IsDown(KEY_DOWN)) {
        if ((u16) (mRotZAdjustPerFrame + (mRotZAdjust + 0x8000)) < 0xaaaa) {
            mRotZAdjust += mRotZAdjustPerFrame;
            mRotZAdjustFrames++;
            if (mRotZAdjustFrames == 0) {
                mRotZAdjustFrames = 0xffff;
            }
        } else {
            mRotZAdjust = 0x2aaa;
        }
    }
    if (KeysReleased(InputManager::Get(), KEY_UP) | KeysReleased(InputManager::Get(), KEY_DOWN)) {
        mRotZAdjustFrames = 0;
    }
    if (mRotYAdjustFrames >= 10) {
        mRotYAdjustPerFrame = (mRotYAdjustFrames - 10) / 2 * 0x24 + 0x5b;
        if (mRotYAdjustPerFrame > 0x1c7) {
            mRotYAdjustPerFrame = 0x1c7;
        }
    } else {
        mRotYAdjustPerFrame = 0x5b;
    }
    if (mRotZAdjustFrames >= 10) {
        mRotZAdjustPerFrame = (mRotZAdjustFrames - 10) * 0x24 + 0xb6;
        if (mRotZAdjustPerFrame > 0x222) {
            mRotZAdjustPerFrame = 0x222;
        }
    } else {
        mRotZAdjustPerFrame = 0xb6;
    }
}

class MicManager { // the microphone singleton of Game.cpp and BattlePlayerList.cpp (instance 0x020c189c)
public:
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u8 mActive;
    /* 21 */ PAD(0x21, 0x24);
    /* 24 */ u8 mEvents;
    /* 25 */ PAD(0x25, 0x28);

    MicManager() {
        mActive = 0;
    }
    ~MicManager();

    static MicManager &Get() {
        static MicManager instance;
        return instance;
    }
};

extern const s32 data_ov007_0227bcb8[3];

#define INT_TO_FX(n) ((fx32) ((n) > 0 ? 0.5f + (float) ((n) << 12) : (float) ((n) << 12) - 0.5f))

u16 BattleShoot::GetLengthXY(int a, int b) {
    VecFx32 v;
    v.x = INT_TO_FX(a);
    v.y = INT_TO_FX(b);
    v.z = 0;
    return VEC_Mag(&v) >> 12;
}

void BattleShoot::func_ov007_0226dfb8() {
    s32 diff = mStartTouchX - mTouchX;
    if (diff < 0) {
        diff = -diff;
    }
    u16 value = diff << 6;
    if (value > 0x31c7) {
        value = 0x31c7;
    }
    int angle;
    if (mStartTouchX < mTouchX) {
        angle = (s16) -value;
    } else {
        angle = (s16) value;
    }
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 curve = list->mPlayers[list->mTurnPlayer]->mRecords[list->mPlayers[list->mTurnPlayer]->mRecord].mUnk_02_3;
    mShotRotY = angle * gShootCurves[curve].mNumerator / gShootCurves[curve].mDenominator + 0x6000;
    mShotRotZ = 0x38e;
    u8 power = mPrevTouchFrames;
    if (power > 0x1e) {
        power = 0x1e;
    } else if (power < 0xa) {
        power = 0xa;
    }
    fx32 distance = GetLengthXY((s16) (mStartTouchX - mTouchX), (s16) (mStartTouchY - mTouchY)) << 12;
    u8 limit = 0x50 - (power - 10) * 50 / 20;
    u32 result;
    int speed = (u16) (FX_Mul(distance, 0xa66) / 4096);
    if (limit < speed) {
        result = limit;
    } else {
        result = speed < 0x1e ? 0x1e : speed;
    }
    mShotSpeed = result;
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228)) {
        return;
    }
    u8 index = data_020d433c.mBattlePlayers->mTurnPlayer;
    data_020d433c.mBattlePlayers->mPlayers[index]->mUnk_ec = mShotSpeed;
    BattlePlayer *player = data_020d433c.mBattlePlayers->mPlayers[index];
    player->mUnk_d4 = 0;
    player->mUnk_d8 = 0;
    player = data_020d433c.mBattlePlayers->mPlayers[index];
    player->mUnk_cc[0] = mShotRotX;
    player->mUnk_cc[1] = mShotRotY;
    player->mUnk_cc[2] = mShotRotZ;
    mBattleTarget->StartShot();
    if (mSpKind == 0) {
        mUnk_6182 = 0x122;
    }
}

void BattleShoot::func_ov007_0226e234() {
    if (!mIcons[0].mCellAnim) {
        return;
    }
    if (mBlendAlpha < 0x10) {
        mBlendAlpha++;
        ApplyBlendAlpha();
    }
    if (mUnk_6194 != 0) {
        if (mTouchFrames == 0) {
            mUnk_6194 = 0;
        }
        return;
    }
    if (mTouchFrames == 0) {
        return;
    }
    s32 dx = mStartTouchY - mTouchY;
    if (dx < 0) {
        dx = -dx;
    }
    s32 dy = mStartTouchX - mTouchX;
    if (dy < 0) {
        dy = -dy;
    }
    u8 i;
    int distance = (u16) (dy + dx);
    if (distance >= 0x20) {
        i = 0;
        do {
            if (mIcons[i].mTimer == 0) {
                mIcons[i].mCellAnim->SetAnimation(0);
                mIcons[i].mX = mTouchX;
                mIcons[i].mY = mTouchY;
                mIcons[i].mXPerFrame = 0;
                mIcons[i].mYPerFrame = 0;
                mIcons[i].mRotation = FX_Atan2Idx(-(mStartTouchX - mTouchX), mStartTouchY - mTouchY);
                mIcons[i].mTimer = distance > 0x20 ? 0x20 : distance;
                mUnk_6194         = 1;
                break;
            }
            i++;
        } while (i < 5);
        if (mUnk_619c != 0) {
            mUnk_619c--;
        }
    }
}

u16 BattleShoot::func_ov007_0226e390() {
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 index               = list->mTurnPlayer;
    VecFx32 cpuPos         = list->mPlayers[index]->mCpu->mPos;
    VecFx32 *pos = &mBattleTarget->mBakugan->mPosition;
    VecFx32 delta;
    VecFx32 offset = {0, 0, 0};
    VEC_Subtract(&offset, pos, &offset);
    VEC_Subtract(&cpuPos, pos, &delta);
    u16 first = FX_Atan2Idx(offset.z, offset.x);
    u16 second = FX_Atan2Idx(delta.z, delta.x);
    return second - first;
}

void BattleShoot::func_ov007_0226e430() {
    VecFx32 v;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    BattleCpu *cpu         = list->mPlayers[list->mTurnPlayer]->mCpu;
    if (cpu == 0) {
        return;
    }
    if (!(cpu->mUnk_77 == 0 || cpu->mUnk_77 == 1 || cpu->mUnk_77 == 2)) {
        return;
    }
    if (cpu->mUnk_58 != 0) {
        if (cpu->mUnk_58 <= mBattleTarget->mMoveFrames) {
            return;
        }
    }
    if (cpu->mTimer != 0) {
        cpu->mTimer--;
        return;
    }
    if (cpu->mUsingTargets == 1) {
        BattleTarget *bt = mBattleTarget;
        if (bt->mUnk_1dd == 4 && bt->mUnk_38 == 0 && bt->mUnk_40 == 0) {
            cpu->NextTarget();
        }
        if (mBattleTarget->mMoveFrames == cpu->mUnk_54) {
            cpu->StopJumpObjTargets();
        }
    }
    BattleTarget *target = mBattleTarget;
    if (target->mUnk_1d0 == 0 || target->mUnk_1dd == 4) {
        return;
    }
    if (target->mMoveFrames % mUnk_619e != 0) {
        return;
    }
    BakuganModel *pose = target->mBakugan;
    VecFx32 pos2             = cpu->mPos;
    VEC_Subtract(&pos2, &pose->mPosition, &v);
    VEC_Normalize(&v, &v);
    if (data_020d433c.mTutorialId != 0) {
        v.x = 0;
        v.y = 0;
        v.z = 0;
    } else {
        s32 k = data_ov007_0227bcc4[cpu->mUnk_77];
        v.x   = FX_Mul(v.x, k);
        v.y   = FX_Mul(v.y, k);
        v.z   = FX_Mul(v.z, k);
    }
    if (mBattleTarget->mUnk_1e3 != 0) {
        MtxFx33 m = data_0220e848;
        MTX_ApplyRotY33(&m, 0x5555);
        MTX_MultVec33(&v, &m, &v);
        v.x = FX_Mul(v.x, 0xa000);
        v.y = FX_Mul(v.y, 0xa000);
        v.z = FX_Mul(v.z, 0xa000);
        mBattleTarget->mUnk_1e3 = 0;
    }
    if (mBattleTarget->mHitGimmickType == GIMMICK_TYPE_BLOCK) {
        if (VEC_Mag(&mBattleTarget->mVelocity) > 0) {
            BattleTarget *t = mBattleTarget;
            v.x             = t->mVelocity.x;
            v.y             = t->mVelocity.y;
            v.z             = t->mVelocity.z;
            VEC_Normalize(&v, &v);
            s32 k = data_ov007_0227bcc4[cpu->mUnk_77];
            v.x   = FX_Mul(v.x, k);
            v.y   = FX_Mul(v.y, k);
            v.z   = FX_Mul(v.z, k);
        }
    }
    mBattleTarget->func_ov007_0225c2e4(&v);
    if (mUnk_6164 % 5 == 0) {
        s32 out[2];
        mBattleTarget->func_ov007_02255c04(&v, out);
        func_ov007_0226f0e8(data_020d433c.mBattlePlayers->mTurnPlayer, out[0], out[1]);
    }
    u8 roll = MATH_Rand32(&gRandom, (u8) (data_ov007_0227bcf8[cpu->mUnk_77].mHigh - data_ov007_0227bcf8[cpu->mUnk_77].mLow));
    mUnk_619e = roll + data_ov007_0227bcf8[cpu->mUnk_77].mLow;
    mUnk_6164++;
}

extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y);

struct Bytes2 { u8 b[2]; };

void BattleShoot::func_ov007_0226e884() {
    s32 animState;
    s32 sumX;
    s32 sumZ;
    s32 power;
    s32 spread;
    u8 charged;
    u8 sent;
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 self                = list->mTurnPlayer;
    data_0220f540.mUnk_00  = 9;
    data_0220f540.mUnk_04  = 0;
    *(s32 *) &data_0220f540.mUnk_08 = 0;
    data_0220f540.mUnk_0c  = 0;
    if (MicManager::Get().mEvents != 0) {
        if (data_020d433c.mUnk_9a_0) {
            data_0220f540.mUnk_0c = 1;
        }
    } else {
        data_0220f540.mUnk_0c = 0;
    }
    if (mPrevTouchFrames != 0) {
        if (mTouchFrames == 0) {
            s32 dy = mStartTouchY - mTouchY;
            if (dy < 0) {
                dy = -dy;
            }
            s32 dx = mStartTouchX - mTouchX;
            if (dx < 0) {
                dx = -dx;
            }
            s32 distance = (u16) (dx + dy);
            if (distance >= 0x20) {
                data_0220f540.mUnk_00 = 9;
                s32 idx               = FX_Atan2Idx(-mTouchSumX, mTouchSumY) >> 4;
                data_0220f540.mUnk_04 = -FX_Mul(FX_SinCosTable_[idx * 2], 0x4cd);
                *(s32 *) &data_0220f540.mUnk_08 = -FX_Mul(FX_SinCosTable_[idx * 2 + 1], 0x4cd);
            }
            mTouchSumX = 0;
            mTouchSumY = 0;
        } else {
            u8 prevX;
            u8 prevY;
            u8 curY;
            u8 curX;
            curX  = mTouchX;
            prevX = mPrevTouchX;
            curY  = mTouchY;
            prevY = mPrevTouchY;
            if ((prevX != curX) | (prevY != curY)) {
                mTouchSumX += (s16) (curX - prevX);
                mTouchSumY += (s16) (curY - prevY);
            }
        }
    }
    const u8 *mode = &data_020c4258;
    u8 i;
    animState = 0;
    i = 0;
    do {
        BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[i];
        if (i == data_020d433c.mBattlePlayers->mUnk_2a9) {
            WirelessManager::Get();
            if (!IsWirelessMode(mode)) {
                mMsgs[i].mUnk_00 = ((u8 *) &data_0220f540)[0];
                mMsgs[i].mUnk_01 = ((u8 *) &data_0220f540)[1];
                *(Bytes2 *) &mMsgs[i].mUnk_02 = *(Bytes2 *) (((u8 *) &data_0220f540) + 2);
                mMsgs[i].mWords = *(ShootWords7 *) ((u8 *) &data_0220f540 + 4);
            } else {
                mMsgs[i].mUnk_00 = ((u8 *) p + 0x100)[0];
                mMsgs[i].mUnk_01 = ((u8 *) p + 0x100)[1];
                *(Bytes2 *) &mMsgs[i].mUnk_02 = *(Bytes2 *) (((u8 *) p + 0x100) + 2);
                mMsgs[i].mWords = *(ShootWords7 *) ((u8 *) p + 0x100 + 4);
            }
        } else {
            mMsgs[i].mUnk_00 = ((u8 *) p + 0x100)[0];
                mMsgs[i].mUnk_01 = ((u8 *) p + 0x100)[1];
                *(Bytes2 *) &mMsgs[i].mUnk_02 = *(Bytes2 *) (((u8 *) p + 0x100) + 2);
                mMsgs[i].mWords = *(ShootWords7 *) ((u8 *) p + 0x100 + 4);
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    i = 0;
    do {
        BattlePlayer *p = data_020d433c.mBattlePlayers->mPlayers[i];
        BattleCpu *cpu  = p->mCpu;
        if (cpu != 0 && self != i && self != p->mPartner) {
            s32 outX = 0;
            s32 outZ = 0;
            if (mBattleTarget->mMoveFrames <= cpu->mUnk_5c) {
                cpu->GetOffset(&outX, &outZ);
                mMsgs[i].mUnk_00        = 9;
                mMsgs[i].mUnk_04        = outX;
                mMsgs[i].mUnk_08        = outZ;
                mMsgs[i].mUnk_0c        = 0;
                u16 limit = cpu->mOffsetFrameCounts[cpu->mOffsetEntryIndex];
                cpu->mOffsetFrames++;
                if (cpu->mOffsetFrames >= limit) {
                    cpu->mOffsetFrames = 0;
                    cpu->mOffsetEntryIndex = (cpu->mOffsetEntryIndex + 1) % 8;
                    cpu->SetOffset();
                }
            }
        }
        i++;
    } while (i < data_020d433c.mPlayerCount);
    BattlePlayer *me = data_020d433c.mBattlePlayers->mPlayers[self];
    if (me->mCpu != 0) {
        i = 0;
        do {
            if (data_020d433c.mBattlePlayers->mPlayers[i]->mCpu == 0) {
                if (mMsgs[i].mUnk_04 != 0) {
                    mMsgs[i].mUnk_04 = FX_Mul(mMsgs[i].mUnk_04, data_ov007_0227bcb8[me->mUnk_f7]);
                }
                if (mMsgs[i].mUnk_08 != 0) {
                    mMsgs[i].mUnk_08 = FX_Mul(mMsgs[i].mUnk_08, data_ov007_0227bcb8[me->mUnk_f7]);
                }
            }
            i++;
        } while (i < data_020d433c.mPlayerCount);
    }
    sent    = 0;
    sumX    = 0;
    sumZ    = 0;
    charged = 0;
    power   = data_020d433c.mBattlePlayers->mPlayers[self]->mUnk_dc;
    spread  = data_020d433c.mBattlePlayers->mPlayers[self]->mUnk_e4;
    if (!(mBattleTarget->mUnk_1dd == 4 && mBattleTarget->mUnk_38 <= 0x1e)) {
        i = 0;
        do {
            s32 msgX;
            s32 msgZ;
            BattleShootMsg *m = &mMsgs[i];
            if (m->mUnk_00 == 9) {
                msgX = m->mUnk_04;
                msgZ = m->mUnk_08;
                if (self == i) {
                    sumX += msgX;
                    sumZ += msgZ;
                    if (!(m->mUnk_0c != 0 && mBattleTarget->mUnk_1cc != 1)) {
                    } else {
                        animState = 1;
                        charged   = 1;
                        power += 0x19a;
                    }
                } else if (!(mBattleTarget->mSpKind == 5 || mBattleTarget->mUnk_1cc == 1)) {
                    u8 count = mOtherPlayerCount;
                    sumX += msgX * 30 / 100 / count;
                    sumZ += msgZ * 30 / 100 / count;
                    if (m->mUnk_0c != 0) {
                        animState = 1;
                        spread += 0x7b / count;
                        charged = 1;
                    }
                }
                sent = 1;
                if (self == i || !(mBattleTarget->mSpKind == 5 || mBattleTarget->mUnk_1cc == 1)) {
                    func_ov007_0226f0e8(i, msgX, msgZ);
                }
            }
            i++;
        } while (i < data_020d433c.mPlayerCount);
    }
    if (power > 0x1333) {
        power = 0x1333;
    }
    if (spread > 0x1333) {
        spread = 0x1333;
    }
    if (charged == 0) {
        if (power > 0) {
            power -= 0x800;
            if (power < 0) {
                power = 0;
            }
        }
        if (spread > 0) {
            spread -= 0x800;
            if (spread < 0) {
                spread = 0;
            }
        }
    }
    data_020d433c.mBattlePlayers->mPlayers[self]->mUnk_dc = power;
    data_020d433c.mBattlePlayers->mPlayers[self]->mUnk_e4 = spread;
    if (sent == 1) {
        BattlePlayer *pl = data_020d433c.mBattlePlayers->mPlayers[self];
        pl->mUnk_d4 = sumX;
        pl->mUnk_d8 = sumZ;
        mBattleTarget->func_ov007_0225bf38();
    }
    if (data_020d433c.mUnk_9a_0 && data_020d433c.mUnk_83 == 0) {
        if (mUnk_61a5 != animState) {
            mUnk_61a5 = animState;
            if (mUnk_61a5) {
                mCells[4].mCellAnim->SetAnimation(2);
            } else {
                mCells[4].mCellAnim->SetAnimation(0);
            }
        }
    }
}

extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y);

void BattleShoot::func_ov007_0226f0e8(u8 kind, s32 x, s32 z) {
    if ((x | z) == 0) {
        return;
    }
    u8 handle = StartEffect(data_020d433c.mEffects, 0, gPlayerEffectIds[kind], 0, 1);
    if (handle == 0) {
        return;
    }
    SoundManager::Get().StartSeqArc(0x43);
    VecFx32 pos = mBattleTarget->mBakugan->mPosition;
    VecFx32 off;
    VecFx32 dir = {0, 0, 0};
    VecFx32 ref = data_ov007_0227bcac;
    dir.x       = x;
    dir.z       = z;
    VEC_Normalize(&dir, &dir);
    fx32 dot    = VEC_DotProduct(&dir, &ref);
    off.x       = 0;
    off.y       = 0;
    off.z       = 0xa000;
    s32 angle   = z >= 0 ? 0xc000 : 0x4000;
    if ((dot < 0x800) & (dot > -0x800)) {
        if (x > 0) {
            off.x = -0x5000;
            angle = 0x8000;
        } else if (x < 0) {
            off.x = 0x5000;
            angle = 0;
        }
    }
    CameraManager::Get();
    MtxFx43 mtx = data_020c1cf4;
    mtx._[9]  = 0;
    mtx._[10] = 0;
    mtx._[11] = 0;
    MTX_MultVec43(&off, &mtx, &off);
    VEC_Add(&pos, &off, &pos);
    if (angle != 0) {
        MtxFx43 rot = gIdentityMtx43;
        MTX_ApplyRotZ43(&rot, angle);
        MTX_Concat43(&rot, &mtx, &mtx);
    }
    MTX_SetTranslation43(&mtx, &pos);
    SetEffectMatrix(data_020d433c.mEffects, handle, &mtx);
}

void BattleShoot::func_ov007_0226f378() {
    if (mMessageWindow) {
        u16 id = data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mTurnPlayer]->mUnk_fa;
        if (mMessageId != id) {
            if (mMessageY == 0x80) {
                mMessageId = id;
                mMessageWindow->StartMessage((s16) id, 0, 0);
                mMessageTargetY = 0x60;
            } else if (mMessageY == 0x60) {
                mMessageTargetY = 0x80;
            }
        }
        if (mMessageY != mMessageTargetY) {
            if (mMessageY < mMessageTargetY) {
                mMessageY += 4;
            } else {
                mMessageY -= 4;
            }
        }
        if (mMessageId != 0) {
            mCells[6].mCellAnim->Draw(1, 0x80, mMessageY, 2);
            mMessageWindow->func_02065bd4(0x20, (s16) (mMessageY + 0x42), 0);
            mMessageWindow->Update();
            mCells[5].mCellAnim->Draw(1, 0x80, mMessageY, 2);
        }
    }
    if (mCells[0].mCellAnim) {
        mCells[0].mCellAnim->Update();
        mCells[0].mCellAnim->Draw(1, (s16) (mCells[0].mX + 0x10), (s16) (mCells[0].mY + 0x10), mCells[0].mUnk_08);
        if (mUnk_6197 != 0 && mSpEnabled != 0) {
            mCells[2].mX = (mUnk_6182 + mCells[2].mX) / 2;
            mCells[2].mCellAnim->Update();
            mCells[2].mCellAnim->Draw(1, mCells[2].mX, mCells[2].mY, mCells[2].mUnk_08);
            if (mSpKind != 0) {
                mCells[1].mCellAnim->Update();
                mCells[1].mCellAnim->Draw(1, mCells[0].mX, mCells[0].mY, mCells[0].mUnk_08);
            }
        }
    }
}
void BattleShoot::DrawMic() {
    mCells[4].mCellAnim->Update();
    mCells[4].mCellAnim->Draw(1, mCells[4].mX, mCells[4].mY, mCells[4].mUnk_08);
    mCells[4].mY = (mCells[4].mY * 3 + mMicTargetY) / 4;
}

void BattleShoot::ClearBitmap() {
    if (data_020d433c.mBattlePlayers->mTurnPlayer == data_020d433c.mBattlePlayers->mUnk_2a9) {
        MI_CpuClearFast(mBitmap, 0x6000);
        UploadBitmap();
    }
}

void BattleShoot::UploadBitmap() {
    if (mPlotEnabled != 0) {
        DC_FlushRange(mBitmap, 0x6000);
        QueueUpload(1, 0xd, 0, mBitmap, 0x6000, 0);
    }
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

void BattleShoot::PlotLine(u8 x0, u8 y0, u8 x1, u8 y1) {
    u8 steps;
    u8 i;
    if (mPlotEnabled == 0) {
        return;
    }
    if (data_020d433c.mBattlePlayers->mTurnPlayer != data_020d433c.mBattlePlayers->mUnk_2a9) {
        return;
    }
    steps = ABS(x0 - x1) >= ABS(y0 - y1) ? ABS(x0 - x1) : ABS(y0 - y1);
    i = 0;
    do {
        s32 a = (i * (x0 - x1)) / steps;
        s32 b = (i * (y0 - y1)) / steps;
        PlotDot(x0 - a, y0 - b);
        i++;
    } while (i < steps);
    UploadBitmap();
}

void BattleShoot::PlotDot(u8 x, u8 y) {
    s16 px;
    s16 py;
    u8 i = 0;
    do {
        px = x + gDotOffsets[i][0];
        py = y + gDotOffsets[i][1];
        BOOL nonNegative = (px >= 0) & (py >= 0);
        if (nonNegative & ((u32) px < 0x100) & ((u32) py < 0xc0)) {
            PlotPixel(px, py);
        }
        i++;
    } while (i < 12);
}

void BattleShoot::PlotPixel(u8 x, u8 y) {
    if (y >= 0xc0) {
        return;
    }
    u32 index = (x / 8) * 64 + x % 8 + ((y / 8) * 2048 + (y % 8) * 8);
    mBitmap[index >> 1] |= (index & 1) ? 0x20 : 2;
}

void BattleShoot::ApplyBlendAlpha() {
    u8 value = mBlendAlpha;
    G2S_SetBlendAlpha(2, 0x31, 0x10 - value, value);
}

void BattleShoot::func_ov007_0226f8a8() {
    if (!mIcons[0].mCellAnim) {
        return;
    }
    if (mUnk_619c != 0) {
        mCells[3].mCellAnim->Update();
        mCells[3].mCellAnim->Draw(1, mCells[3].mX, mCells[3].mY, mCells[3].mUnk_08);
        if (mShootTextVisible != 0) {
            DrawShootText();
        }
    }
    u8 i = 0;
    do {
        if (mIcons[i].mTimer != 0) {
            mIcons[i].mTimer--;
            mIcons[i].mCellAnim->Update();
            mIcons[i].mCellAnim->DrawTransformed(1, mIcons[i].mX, mIcons[i].mY, 2, 0x1000, 0x1000, mIcons[i].mRotation);
            if (mIcons[i].mCellAnim->mFinished != 0) {
                mIcons[i].mTimer = 0;
            } else {
                mIcons[i].mX += mIcons[i].mXPerFrame;
                mIcons[i].mY += mIcons[i].mYPerFrame;
            }
        }
        i++;
    } while (i < 5);
}

BOOL BattleShoot::TryStartPopup() {
    u8 ok = 1;
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4030.mUnk_228) || data_020d433c.mUnk_83 != 0) {
        ok = 0;
    }
    if (mBattleTarget->mPopupEnabled == 0) {
        ok = 0;
    }
    if (ok) {
        if (InputManager::Get().IsHeld(8)) {
            mReturnState = mState;
            mState = 0xd;
            data_020d433c.mUnk_14 |= 8;
            mPlotEnabled = 0;
            return 1;
        }
    }
    return 0;
}

void BattleShoot::SetSubPlanes(u8 value) {
    mSubPlanes = value;
    QueueFrameCallback(ApplySubPlanes, this);
}

void ApplySubPlanes(void *arg) {
    GXS_SetVisiblePlane(((BattleShoot *) arg)->mSubPlanes);
}

