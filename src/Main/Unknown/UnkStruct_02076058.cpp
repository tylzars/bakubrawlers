#include "System/TouchManager.hpp"
#include "UI/SelectWindow.hpp"

// Defined between the headers: where they sit among the singletons' objects sets the data layout.
static const u8 sResultCellPositions[12][2] = {
    {0x28, 0x2b}, {0x28, 0x53}, {0x28, 0x7b}, {0x78, 0x33}, {0x78, 0x5b}, {0x78, 0x83},
    {0x78, 0x33}, {0x78, 0x5b}, {0x78, 0x83}, {0x74, 0x35}, {0x74, 0x5d}, {0x74, 0x85},
};
static const WindowItemRecord sResultCockpitItems[1] = {
    {0, 0x5, 201, 170, 52, 20, 1, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord sResultCockpitCells[1] = {
    {"/Game/2D/union_b/union_b_obj_cockpit_nce.bin", 0, 0xa000, 2, 0, 0, {0, 1, 1, 0}},
};

#include "Text/TextManager.hpp"
#include "System/InputManager.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Unknown/UnkStruct_02071d24.hpp"
#include "Game/SaveData.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Scene/SceneBase.hpp"
#include "System/FileSystem.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "UI/SelectWindow.hpp"

#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

// Result screen of a battle: the three players' bakugan with their level and points.
class UnkStruct_02076058 : public SceneBase {
public:
    /* 0c */ MessageFile *mBakuganNameMessages;
    /* 10 */ MessageFile *mResultMessages;
    /* 14 */ CellAnimation *mCellAnims[12];
    /* 44 */ void *mTextBuffer;
    /* 48 */ void *mGlyphBuffer;
    /* 4c */ u32 mTextBufferSize;
    /* 50 */ u32 mGlyphCount;
    /* 54 */ UnkStruct_02023248 *mSaveEntries[3];
    /* 60 */ UnkStruct_02071d24 *mUnk_60;
    /* 64 */ u16 mRemainingGauge[3];
    /* 6a */ u8 mBakuganIds[3];
    /* 6d */ u8 mAttributes[3];
    /* 70 */ u8 mUnk_70[3];
    /* 73 */ u8 mMode;
    /* 74 */ u8 mState;
    /* 75 */ u8 mSlotOffset;
    /* 76 */ u8 mTimer;
    /* 77 */ u8 mUnk_77;
    /* 78 */ u8 mUnk_78;
    /* 79 */ u8 mUnk_79;
    /* 7a */ PAD(0x7a, 0x7c);
    /* 7c */ SelectWindow *mWindow;
    /* 80 */

    UnkStruct_02076058(u8 unk1);
    virtual ~UnkStruct_02076058();
    virtual void Update();
    void InitDisplay();
    void LoadGraphics();
    void AddRemainingGaugeAtOnce();
    u32 func_020771d8(u32 index, u32 unk2);
    void Draw();
    void UpdateGauge();
    void SetBakugan(u32 index, u32 unk2, u32 unk3, u8 unk4);
    void LoadMessages();
    void PrintText();
};

void func_02078250(UnkStruct_02076058 *object);
extern "C" void DC_FlushRange(void *buffer, u32 size);
void func_020781a0(UnkStruct_02076058 *object);

UnkStruct_02076058::UnkStruct_02076058(u8 unk1) :
    SceneBase(0x3e8, 0) {
    mBakuganNameMessages = NULL;
    mResultMessages = NULL;
    mTextBuffer = NULL;
    mGlyphBuffer = NULL;
    mTextBufferSize = 0;
    mGlyphCount = 0;
    mUnk_60 = NULL;
    mMode = unk1;
    mState = 0;
    mSlotOffset = 0;
    mTimer = 0x3c;
    mUnk_77 = 0;
    mUnk_78 = 0;
    mUnk_79 = 0;
    mWindow = NULL;
    SpriteManager::Get();
    SpriteManager::sFade[1] = FADE_MODE_HOLD;
    SpriteManager::sBrightness[1] = -16;
    MI_CpuClear16(mRemainingGauge, 6);
    MI_CpuClear8(mBakuganIds, 3);
    MI_CpuClear8(mAttributes, 3);
    MI_CpuClear8(mUnk_70, 3);
    for (u8 i = 0; i < 12; i++) {
        mCellAnims[i] = NULL;
    }
    u8 i = 0;
    do {
        mSaveEntries[i] = NULL;
        i++;
    } while (i < 3);
    u8 slot = 0;
    do {
        if (mMode == 0 && data_020d433c.mBattlePlayers != NULL) {
            BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9]->mRecords[slot];
            mBakuganIds[slot] = record->mBakuganId;
            if (mBakuganIds[slot] == 6) {
                mBakuganIds[slot] = 5;
            }
            u8 variant = record->mAttribute;
            mAttributes[slot] = variant;
            UnkStruct_02023248 *entry = GetBakuganEntry(gSaveData, mBakuganIds[slot], variant);
            mSaveEntries[slot] = entry;
            u32 base = entry->mGauge;
            mRemainingGauge[slot] = record->mGauge - base;
            mUnk_70[slot] = mSaveEntries[slot]->mUnk_00_0;
        }
        slot++;
    } while (slot < 3 && data_020d433c.mLastBattle == 0);
}

UnkStruct_02076058::~UnkStruct_02076058() {
    SpriteManager::Get();
    SpriteManager::sFade[1] = FADE_MODE_HOLD;
    SpriteManager::sBrightness[1] = -16;
    for (u8 i = 0; i < 12; i++) {
        delete mCellAnims[i];
    }
    delete mWindow;
    if (mBakuganNameMessages != NULL) {
        OS_Free(mBakuganNameMessages);
        mBakuganNameMessages = NULL;
    }
    if (mResultMessages != NULL) {
        OS_Free(mResultMessages);
        mResultMessages = NULL;
    }
    if (mTextBuffer != NULL) {
        func_02078250(this);
        OS_Free(mTextBuffer);
    }
    if (mGlyphBuffer != NULL) {
        OS_Free(mGlyphBuffer);
        mGlyphBuffer = NULL;
    }
}

void UnkStruct_02076058::Update() {
    switch (mState) {
        case 0:
            InitDisplay();
            LoadGraphics();
            LoadMessages();
            SpriteManager::Get();
            SpriteManager::sFade[1] = FADE_MODE_TO_ZERO;
            mState++;
            break;
        case 1:
            SpriteManager::Get();
            if (SpriteManager::sFade[1] <= FADE_MODE_HOLD) {
                mState++;
            }
            Draw();
            break;
        case 2:
            switch (mMode) {
                case 0:
                    if (mUnk_78 != 0) {
                        mState++;
                    }
                    break;
                case 1:
                    mState++;
                    break;
            }
            Draw();
            break;
        case 3:
            Draw();
            UpdateGauge();
            break;
        case 4:
            if (mUnk_77 != 0) {
                mState++;
                SpriteManager::Get();
                SpriteManager::sFade[0] = FADE_MODE_DOWN;
                SpriteManager::sFade[1] = FADE_MODE_DOWN;
            } else {
                switch (mMode) {
                    case 0:
                        if (mWindow != NULL) {
                            mWindow->mEnabled = 1;
                            if (mWindow->Update(1) == 1) {
                                mState = 7;
                            }
                        }
                        break;
                    case 1:
                        mState = 7;
                        break;
                }
            }
            Draw();
            break;
        case 5: {
            Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (mUnk_78 == 0) {
                return;
            }
            mUnk_60 = new UnkStruct_02071d24(mMode);
            u8 count = 0;
            mContext->mRequest = 0;
            for (u8 i = 0; i < 3; i++) {
                if (mSaveEntries[i] != NULL) {
                    u8 gain = mSaveEntries[i]->mUnk_00_0 - mUnk_70[i];
                    if (gain != 0) {
                        mUnk_60->SetBakugan(count, mBakuganIds[i], mAttributes[i], gain);
                        count++;
                    }
                }
            }
            mState++;
            break;
        }
        case 6:
            if (mContext->mRequest == 2) {
                mState = 9;
            }
            break;
        case 7:
            Draw();
            SpriteManager::Get();
            SpriteManager::sFade[0] = FADE_MODE_DOWN;
            SpriteManager::sFade[1] = FADE_MODE_DOWN;
            mState++;
            break;
        case 8:
            Draw();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mState++;
            break;
        case 9:
            RequestScene(3);
            mFinished = 1;
            break;
    }
}

void UnkStruct_02076058::InitDisplay() {
    GX_SetDispSelect(1);
    if (mMode == 0) {
        SpriteManager::Get().SetupMode(0, 1);
        GX_SetGraphicsMode(1, 0, 0);
    }
    G2S_SetBG0Control(0, 1, 0, 1, 0);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG0Priority(3);
    GXS_SetVisiblePlane(0x11);
    G2S_SetBlendBrightness(0x1f, 0);
    GXS_SetVisibleWnd(0);
    G2S_BlendNone();
}


void UnkStruct_02076058::LoadGraphics() {
    ChangeDir("/Game/Battle/Bg/");
    LoadAndQueueUpload(1, UPLOAD_KIND_BG_EXT_PLTT, 0, "result_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_CHAR, 0, "result_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, UPLOAD_KIND_BG0_SCR, 0, "result_b_bg_00_nsc.bin", 0, 0, 0);
    ChangeDir("/Game/Battle/Obj/");
    u8 i;
    SpriteScreen &screen = SpriteManager::Get().mScreens[1];
    screen.SetObjVramMode(3);
    screen.LoadCharacters("bakugan_sub.nce_ncg1D.bin", 0, 0);
    screen.LoadExtPalette("bakugan_sub_ncl.bin", 0, 0);
    screen.LoadCharacters("result_b_obj_00.nce_ncg1D.bin", 0xc000, 0);
    screen.LoadPalette("result_b_obj_00_ncl.bin", 0, 0);
    for (i = 0; i < 12; i++) {
        switch (i) {
            case 0:
            case 1:
            case 2:
                mCellAnims[i] = new CellAnimation("bakugan_sub_nce.bin", 0, 1, 1);
                mCellAnims[i]->mCharBase = screen.GetCharBase(0);
                mCellAnims[i]->mPaletteBase = 0;
                mCellAnims[i]->SetMode(CELL_ANIM_MODE_STOP);
                mCellAnims[i]->mFrameIndex = mAttributes[i];
                break;
            case 6:
            case 7:
            case 8:
                mCellAnims[i] = new CellAnimation("result_b_obj_00_nce.bin", 0, 1, 1);
                mCellAnims[i]->mCharBase = screen.GetCharBase(0xc000);
                mCellAnims[i]->mPaletteBase = 0;
                mCellAnims[i]->SetMode(CELL_ANIM_MODE_STOP);
                if (mSaveEntries[i % 3] != NULL) {
                    s32 level = mSaveEntries[i % 3]->mUnk_00_0;
                    if (level < 9) {
                        mCellAnims[i]->mFrameIndex = level;
                    } else {
                        mCellAnims[i]->mFrameIndex = 8;
                    }
                }
                break;
            case 3:
            case 4:
            case 5: {
                mCellAnims[i] = new CellAnimation("result_b_obj_00_nce.bin", 0, 1, 1);
                mCellAnims[i]->mCharBase = screen.GetCharBase(0xc000);
                mCellAnims[i]->mPaletteBase = 0;
                mCellAnims[i]->SetAnimation(1);
                mCellAnims[i]->SetMode(CELL_ANIM_MODE_STOP);
                mCellAnims[i]->mFrameIndex = 0;
                UnkStruct_02023248 *entry = mSaveEntries[i % 3];
                if (entry != NULL) {
                    int gauge = entry->mGauge;
                    u16 rest = gauge % 300;
                    mCellAnims[i]->mFrameIndex = (u8) ((float) (rest * (mCellAnims[i]->mFrames[mCellAnims[i]->mAnimIndex].mFrameCount - 1)) / 300.0f);
                }
                break;
            }
            case 9:
            case 10:
            case 11:
                mCellAnims[i] = new CellAnimation("result_b_obj_00_nce.bin", 0, 1, 1);
                mCellAnims[i]->mCharBase = screen.GetCharBase(0xc000);
                mCellAnims[i]->mPaletteBase = 0;
                mCellAnims[i]->SetAnimation(5);
                CellAnimation *last = mCellAnims[i];
                last->mFrameIndex = last->mFrames[last->mAnimIndex].mFrameCount - 1;
                break;
        }
    }
    ChangeDir("/Game/2D/union_b/");
    SpriteScreen &window = SpriteManager::Get().mScreens[1];
    window.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0xa000, 0);
    window.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x40, 0);
    mWindow = new SelectWindow(1, sResultCockpitItems, 0);
    mWindow->SetCells(1, sResultCockpitCells);
    mWindow->mCursor->mSelection = -1;
    mWindow->mEnabled = 0;
    ChangeDir("/");
}

void UnkStruct_02076058::AddRemainingGaugeAtOnce() {
    u32 changed = 0;
    for (u8 i = 0; i < 3; i++) {
        u8 slot = i + mSlotOffset;
        if (mSaveEntries[slot] != NULL && mRemainingGauge[slot] != 0) {
            mRemainingGauge[slot] += mSaveEntries[slot]->mGauge;
            if (func_020771d8(i, 1)) {
                changed = 1;
            }
            mRemainingGauge[slot] = 0;
        }
    }
    if (changed != 0) {
        SoundManager::Get().StartSeqArc(0x35);
        PrintText();
    }
}

u32 UnkStruct_02076058::func_020771d8(u32 index, u32 unk2) {
    u32 result = 0;
    u32 count = 1;
    if (unk2 != 0) {
        count = 9;
    }
    u8 slot = index + mSlotOffset;
    if (mSaveEntries[slot] != NULL) {
        u8 k = 0;
        do {
            UnkStruct_02023248 *entry = mSaveEntries[slot];
            u16 cost = (entry->mUnk_00_0 + 1) * 300;
            if (unk2 != 0) {
                u16 points = mRemainingGauge[slot];
                if (points >= cost) {
                    mRemainingGauge[slot] = points - cost;
                    if ((s32) mSaveEntries[slot]->mUnk_00_0 < 9) {
                        mSaveEntries[slot]->mUnk_00_0 = mSaveEntries[slot]->mUnk_00_0 + 1;
                    }
                    s32 level = mSaveEntries[slot]->mUnk_00_0;
                    if (level < 9) {
                        mCellAnims[index + 6]->mFrameIndex = level;
                    }
                    mCellAnims[index + 9]->ResetAnimation();
                    mUnk_77 = 1;
                    mUnk_79 = 1;
                    result = 1;
                } else {
                    mSaveEntries[slot]->mGauge = mRemainingGauge[slot];
                    break;
                }
            } else {
                u32 gauge = entry->mGauge;
                if (gauge < cost) {
                    break;
                }
                entry->mGauge = gauge - cost;
                if ((s32) mSaveEntries[slot]->mUnk_00_0 < 9) {
                    mSaveEntries[slot]->mUnk_00_0 = mSaveEntries[slot]->mUnk_00_0 + 1;
                }
                s32 level = mSaveEntries[slot]->mUnk_00_0;
                if (level < 9) {
                    mCellAnims[index + 6]->mFrameIndex = level;
                }
                mCellAnims[index + 9]->ResetAnimation();
                mUnk_77 = 1;
                mUnk_79 = 1;
                result = 1;
            }
            k++;
        } while (k < count);
        CellAnimation *anim = mCellAnims[index + 3];
        int gauge = mSaveEntries[slot]->mGauge;
        u16 rest = gauge % 300;
        anim->mFrameIndex = (u8) ((float) (rest * (anim->mFrames[anim->mAnimIndex].mFrameCount - 1)) / 300.0f);
    }
    return result;
}

void UnkStruct_02076058::Draw() {
    for (u8 i = 0; i < 3; i++) {
        u8 slot = i + 9;
        if (mCellAnims[slot] != NULL) {
            mCellAnims[slot]->Update();
            mCellAnims[slot]->Draw(1, sResultCellPositions[slot][0], sResultCellPositions[slot][1], 1);
        }
    }
    for (u32 i = 0; i < 3; i = (u8) (i + 1)) {
        u8 slot = i;
        if (mSaveEntries[i + mSlotOffset] != NULL && mCellAnims[slot] != NULL) {
            mCellAnims[slot]->Update();
            mCellAnims[slot]->DrawTransformed(1, sResultCellPositions[slot][0], sResultCellPositions[slot][1], 1, 0x800, 0x800, 0);
        }
    }
    for (u32 i = 0; i < 3; i = (u8) (i + 1)) {
        u8 slot = i + 3;
        UnkStruct_02023248 *entry = mSaveEntries[i + mSlotOffset];
        if (entry != NULL && (s32) entry->mUnk_00_0 < 9 && mCellAnims[slot] != NULL) {
            u32 last = mCellAnims[slot]->mFrames[mCellAnims[slot]->mAnimIndex].mFrameCount - 1;
            u8 count = entry->mGauge / 300;
            u8 n = 0;
            if (count != 0) {
                u8 x = sResultCellPositions[slot][0];
                u8 y = sResultCellPositions[slot][1];
                do {
                    u8 cellX = x + n % 3 * 32;
                    u8 cellY = n / 3 * 6 + y;
                    mCellAnims[slot]->DrawFrame(1, (u8) last, cellX, cellY, 1);
                    n++;
                } while (n < count);
            }
            u8 drawX = sResultCellPositions[slot][0] + n % 3 * 32;
            u8 drawY = sResultCellPositions[slot][1] + n / 3 * 6;
            mCellAnims[slot]->Draw(1, drawX, drawY, 1);
        }
    }
    for (u8 i = 0; i < 3; i++) {
        u8 slot = i + 6;
        if (mSaveEntries[i + mSlotOffset] != NULL && (s32) mSaveEntries[i + mSlotOffset]->mUnk_00_0 < 9 && mCellAnims[slot] != NULL) {
            mCellAnims[slot]->Draw(1, sResultCellPositions[slot][0], sResultCellPositions[slot][1], 1);
        }
    }
    func_020781a0(this);
    if (mWindow != NULL) {
        mWindow->Draw();
    }
}

void UnkStruct_02076058::UpdateGauge() {
    if (InputManager::Get().IsPressed(1) | TouchManager::Get().mPressed) {
        AddRemainingGaugeAtOnce();
    }
    u32 changed = 0;
    for (u8 i = 0; i < 3; i++) {
        u8 slot = i + mSlotOffset;
        UnkStruct_02023248 *entry = mSaveEntries[slot];
        if (entry != NULL) {
            if ((s32) entry->mUnk_00_0 >= 9) {
                entry->mGauge = 0;
                mRemainingGauge[slot] = 0;
            } else if (mCellAnims[i + 9]->mFinished != 0 && mRemainingGauge[slot] != 0) {
                u32 step = 3;
                s32 points = mRemainingGauge[slot];
                if (points < 3) {
                    step = (u8) points;
                }
                mRemainingGauge[slot] -= step;
                mSaveEntries[slot]->mGauge = mSaveEntries[slot]->mGauge + step;
                if (func_020771d8(i, 0)) {
                    changed = 1;
                }
            }
        }
    }
    if (changed != 0) {
        SoundManager::Get().StartSeqArc(0x35);
        PrintText();
    }
    u32 done = 1;
    for (u8 i = 0; i < 3; i++) {
        if (mRemainingGauge[i] != 0) {
            done = 0;
            break;
        }
    }
    for (u8 i = 0; i < 3; i++) {
        if (mCellAnims[i + 9]->mFinished == 0) {
            done = 0;
        }
    }
    if (done != 0) {
        if (mTimer != 0) {
            mTimer--;
        } else {
            mState++;
        }
    }
}

u32 func_020779ec(void *unk) {
    UnkStruct_02076058 *object = (UnkStruct_02076058 *) unk;
    switch (object->mMode) {
        case 0:
            if (object->mUnk_60 != NULL) {
                return 1;
            }
            break;
        case 1:
            if (object->mState >= 5) {
                return 1;
            }
            break;
    }
    return 0;
}

void UnkStruct_02076058::SetBakugan(u32 index, u32 unk2, u32 unk3, u8 unk4) {
    if (!(index < 3 && unk2 < 0x29 && unk3 < 6)) {
        return;
    }
    mBakuganIds[index] = unk2;
    mAttributes[index] = unk3;
    mSaveEntries[index] = GetBakuganEntry(gSaveData, unk2, unk3);
    mUnk_70[index] = mSaveEntries[index]->mUnk_00_0;
    if (unk4 != 0) {
        GetBakuganEntry(gSaveData, unk2, unk3);
        u16 cost = (mSaveEntries[index]->mUnk_00_0 + 1) * 300;
        mRemainingGauge[index] = cost - mSaveEntries[index]->mGauge;
    }
}

void UnkStruct_02076058::LoadMessages() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    ChangeDir("/font/");
    mBakuganNameMessages = (MessageFile *) LoadFile("mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mResultMessages = (MessageFile *) LoadFile("mes_Result.mes", 0, 0, 0, 0, 0);
    mTextBufferSize = TextManager::Get().BufferSize(0x18, 0x10);
    mTextBuffer = OS_Alloc(mTextBufferSize);
    MI_CpuClear16(mTextBuffer, mTextBufferSize);
    mGlyphCount = TextManager::Get().GlyphCount(0x30);
    mGlyphBuffer = OS_Alloc((s16) mGlyphCount * (s16) 0xc);
    TextManager::Get().Init(mGlyphBuffer, 1, 0, 0x18, 0x10, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    PrintText();
}

void UnkStruct_02076058::PrintText() {
    func_02078250(this);
    TextManager::Get().mUse4bppFont = 1;
    for (u8 i = 0; i < 3; i++) {
        if (mSaveEntries[i + mSlotOffset] != NULL) {
            s16 x = 0;
            s16 y = i * 0x28;
            u16 pixelWidth;
            u8 id = (&mBakuganIds[i])[mSlotOffset];
            TextManager &text = TextManager::Get();
            s16 nameX;
            s16 nameY;
            nameY = y;
            nameX = x;
            MessageFile *names = mBakuganNameMessages;
            text.Print(names->GetText(id), names->GetLength(id), mTextBuffer, 0x18, 0x10, &nameX, &nameY, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
            y += 12;
            TextManager &text2 = TextManager::Get();
            s16 labelX;
            s16 labelY;
            labelY = y;
            labelX = x;
            MessageFile *labels = mResultMessages;
            text2.Print(labels->GetText(5), labels->GetLength(5), mTextBuffer, 0x18, 0x10, &labelX, &labelY, &pixelWidth, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
            x += pixelWidth;
            u8 level = mSaveEntries[i + mSlotOffset]->mUnk_00_0;
            if (level < 9) {
                TextManager::Get().PrintNumber(level + 1, 1, 0, mTextBuffer, 0x18, 0x10, &x, &y, 0, 2, 0x21, 0, 0, 1, 1);
            } else {
                TextManager &text3 = TextManager::Get();
                s16 maxX;
                s16 maxY;
                maxY = y;
                maxX = x;
                MessageFile *labels2 = mResultMessages;
                text3.Print(labels2->GetText(6), labels2->GetLength(6), mTextBuffer, 0x18, 0x10, &maxX, &maxY, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
            }
        }
    }
    DC_FlushRange(mTextBuffer, mTextBufferSize);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, UPLOAD_KIND_OBJ, 0x6000, mTextBuffer, mTextBufferSize, 0);
}

void func_020781a0(UnkStruct_02076058 *object) {
    SpriteManager::Get().Screen(1).AddOam(object->mGlyphBuffer, object->mGlyphCount, 0x46, 0x29, 1, 0xc0, 0xf, 0);
}

void func_02078250(UnkStruct_02076058 *object) {
    if (object->mTextBuffer != NULL) {
        MI_CpuClear16(object->mTextBuffer, object->mTextBufferSize);
    }
}

BOOL func_02078270(UnkStruct_02076058 *object) {
    if (object->mUnk_79 != 0) {
        object->mUnk_79 = 0;
        return TRUE;
    }
    return FALSE;
}
