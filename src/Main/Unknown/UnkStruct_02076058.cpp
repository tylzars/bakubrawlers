#include "System/TouchManager.hpp"
#include "UI/SelectWindow.hpp"

// Defined between the headers: where they sit among the singletons' objects sets the data layout.
static const u8 data_020a308c[12][2] = {
    {0x28, 0x2b}, {0x28, 0x53}, {0x28, 0x7b}, {0x78, 0x33}, {0x78, 0x5b}, {0x78, 0x83},
    {0x78, 0x33}, {0x78, 0x5b}, {0x78, 0x83}, {0x74, 0x35}, {0x74, 0x5d}, {0x74, 0x85},
};
static const WindowItemRecord data_020a30a4[1] = {
    {0, 0x5, 201, 170, 52, 20, 1, {-1, -1, -1, -1}, 0, 1},
};
static const WindowCellRecord data_020a30c0[1] = {
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
    /* 0c */ MessageFile *mUnk_0c;
    /* 10 */ MessageFile *mUnk_10;
    /* 14 */ CellAnimation *mUnk_14[12];
    /* 44 */ void *mUnk_44;
    /* 48 */ void *mUnk_48;
    /* 4c */ u32 mUnk_4c;
    /* 50 */ u32 mUnk_50;
    /* 54 */ UnkStruct_02023248 *mUnk_54[3];
    /* 60 */ UnkStruct_02071d24 *mUnk_60;
    /* 64 */ u16 mUnk_64[3];
    /* 6a */ u8 mUnk_6a[3];
    /* 6d */ u8 mUnk_6d[3];
    /* 70 */ u8 mUnk_70[3];
    /* 73 */ u8 mUnk_73;
    /* 74 */ u8 mUnk_74;
    /* 75 */ u8 mUnk_75;
    /* 76 */ u8 mUnk_76;
    /* 77 */ u8 mUnk_77;
    /* 78 */ u8 mUnk_78;
    /* 79 */ u8 mUnk_79;
    /* 7a */ PAD(0x7a, 0x7c);
    /* 7c */ SelectWindow *mUnk_7c;
    /* 80 */

    UnkStruct_02076058(u8 unk1);
    virtual ~UnkStruct_02076058();
    virtual void Update();
    void func_02076a28();
    void func_02076b34();
    void func_020770f4();
    u32 func_020771d8(u32 index, u32 unk2);
    void func_02077458();
    void func_02077770();
    void func_02077a30(u32 index, u32 unk2, u32 unk3, u8 unk4);
    void func_02077ae8();
    void func_02077d2c();
};

void func_02078250(UnkStruct_02076058 *object);
extern "C" void DC_FlushRange(void *buffer, u32 size);
void func_020781a0(UnkStruct_02076058 *object);

UnkStruct_02076058::UnkStruct_02076058(u8 unk1) :
    SceneBase(0x3e8, 0) {
    mUnk_0c = NULL;
    mUnk_10 = NULL;
    mUnk_44 = NULL;
    mUnk_48 = NULL;
    mUnk_4c = 0;
    mUnk_50 = 0;
    mUnk_60 = NULL;
    mUnk_73 = unk1;
    mUnk_74 = 0;
    mUnk_75 = 0;
    mUnk_76 = 0x3c;
    mUnk_77 = 0;
    mUnk_78 = 0;
    mUnk_79 = 0;
    mUnk_7c = NULL;
    SpriteManager::Get();
    SpriteManager::sFade[1] = 1;
    SpriteManager::sBrightness[1] = -16;
    MI_CpuClear16(mUnk_64, 6);
    MI_CpuClear8(mUnk_6a, 3);
    MI_CpuClear8(mUnk_6d, 3);
    MI_CpuClear8(mUnk_70, 3);
    for (u8 i = 0; i < 12; i++) {
        mUnk_14[i] = NULL;
    }
    u8 i = 0;
    do {
        mUnk_54[i] = NULL;
        i++;
    } while (i < 3);
    u8 slot = 0;
    do {
        if (mUnk_73 == 0 && data_020d433c.mBattlePlayers != NULL) {
            BattleRecord *record = &data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9]->mRecords[slot];
            mUnk_6a[slot] = record->mBakuganId;
            if (mUnk_6a[slot] == 6) {
                mUnk_6a[slot] = 5;
            }
            u8 variant = record->mAttribute;
            mUnk_6d[slot] = variant;
            UnkStruct_02023248 *entry = func_02023248(gSaveData, mUnk_6a[slot], variant);
            mUnk_54[slot] = entry;
            u32 base = entry->mPad;
            mUnk_64[slot] = record->mGauge - base;
            mUnk_70[slot] = mUnk_54[slot]->mUnk_00_0;
        }
        slot++;
    } while (slot < 3 && data_020d433c.mUnk_8a_0 == 0);
}

UnkStruct_02076058::~UnkStruct_02076058() {
    SpriteManager::Get();
    SpriteManager::sFade[1] = 1;
    SpriteManager::sBrightness[1] = -16;
    for (u8 i = 0; i < 12; i++) {
        delete mUnk_14[i];
    }
    delete mUnk_7c;
    if (mUnk_0c != NULL) {
        OS_Free(mUnk_0c);
        mUnk_0c = NULL;
    }
    if (mUnk_10 != NULL) {
        OS_Free(mUnk_10);
        mUnk_10 = NULL;
    }
    if (mUnk_44 != NULL) {
        func_02078250(this);
        OS_Free(mUnk_44);
    }
    if (mUnk_48 != NULL) {
        OS_Free(mUnk_48);
        mUnk_48 = NULL;
    }
}

void UnkStruct_02076058::Update() {
    switch (mUnk_74) {
        case 0:
            func_02076a28();
            func_02076b34();
            func_02077ae8();
            SpriteManager::Get();
            SpriteManager::sFade[1] = 2;
            mUnk_74++;
            break;
        case 1:
            SpriteManager::Get();
            if (SpriteManager::sFade[1] <= 1) {
                mUnk_74++;
            }
            func_02077458();
            break;
        case 2:
            switch (mUnk_73) {
                case 0:
                    if (mUnk_78 != 0) {
                        mUnk_74++;
                    }
                    break;
                case 1:
                    mUnk_74++;
                    break;
            }
            func_02077458();
            break;
        case 3:
            func_02077458();
            func_02077770();
            break;
        case 4:
            if (mUnk_77 != 0) {
                mUnk_74++;
                SpriteManager::Get();
                SpriteManager::sFade[0] = 3;
                SpriteManager::sFade[1] = 3;
            } else {
                switch (mUnk_73) {
                    case 0:
                        if (mUnk_7c != NULL) {
                            mUnk_7c->mEnabled = 1;
                            if (mUnk_7c->Update(1) == 1) {
                                mUnk_74 = 7;
                            }
                        }
                        break;
                    case 1:
                        mUnk_74 = 7;
                        break;
                }
            }
            func_02077458();
            break;
        case 5: {
            func_02077458();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            if (mUnk_78 == 0) {
                return;
            }
            mUnk_60 = new UnkStruct_02071d24(mUnk_73);
            u8 count = 0;
            mContext->mRequest = 0;
            for (u8 i = 0; i < 3; i++) {
                if (mUnk_54[i] != NULL) {
                    u8 gain = mUnk_54[i]->mUnk_00_0 - mUnk_70[i];
                    if (gain != 0) {
                        mUnk_60->SetBakugan(count, mUnk_6a[i], mUnk_6d[i], gain);
                        count++;
                    }
                }
            }
            mUnk_74++;
            break;
        }
        case 6:
            if (mContext->mRequest == 2) {
                mUnk_74 = 9;
            }
            break;
        case 7:
            func_02077458();
            SpriteManager::Get();
            SpriteManager::sFade[0] = 3;
            SpriteManager::sFade[1] = 3;
            mUnk_74++;
            break;
        case 8:
            func_02077458();
            if (SpriteManager::Get().IsFading()) {
                return;
            }
            mUnk_74++;
            break;
        case 9:
            RequestScene(3);
            mFinished = 1;
            break;
    }
}

void UnkStruct_02076058::func_02076a28() {
    GX_SetDispSelect(1);
    if (mUnk_73 == 0) {
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


void UnkStruct_02076058::func_02076b34() {
    ChangeDir("/Game/Battle/Bg/");
    LoadAndQueueUpload(1, 1, 0, "result_b_bg_00_ncl.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 11, 0, "result_b_bg_00_ncg.bin", 0, 0, 0);
    LoadAndQueueUpload(1, 7, 0, "result_b_bg_00_nsc.bin", 0, 0, 0);
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
                mUnk_14[i] = new CellAnimation("bakugan_sub_nce.bin", 0, 1, 1);
                mUnk_14[i]->mCharBase = screen.GetCharBase(0);
                mUnk_14[i]->mPaletteBase = 0;
                mUnk_14[i]->SetMode(5);
                mUnk_14[i]->mFrameIndex = mUnk_6d[i];
                break;
            case 6:
            case 7:
            case 8:
                mUnk_14[i] = new CellAnimation("result_b_obj_00_nce.bin", 0, 1, 1);
                mUnk_14[i]->mCharBase = screen.GetCharBase(0xc000);
                mUnk_14[i]->mPaletteBase = 0;
                mUnk_14[i]->SetMode(5);
                if (mUnk_54[i % 3] != NULL) {
                    s32 level = mUnk_54[i % 3]->mUnk_00_0;
                    if (level < 9) {
                        mUnk_14[i]->mFrameIndex = level;
                    } else {
                        mUnk_14[i]->mFrameIndex = 8;
                    }
                }
                break;
            case 3:
            case 4:
            case 5: {
                mUnk_14[i] = new CellAnimation("result_b_obj_00_nce.bin", 0, 1, 1);
                mUnk_14[i]->mCharBase = screen.GetCharBase(0xc000);
                mUnk_14[i]->mPaletteBase = 0;
                mUnk_14[i]->SetAnimation(1);
                mUnk_14[i]->SetMode(5);
                mUnk_14[i]->mFrameIndex = 0;
                UnkStruct_02023248 *entry = mUnk_54[i % 3];
                if (entry != NULL) {
                    int gauge = entry->mPad;
                    u16 rest = gauge % 300;
                    mUnk_14[i]->mFrameIndex = (u8) ((float) (rest * (mUnk_14[i]->mFrames[mUnk_14[i]->mAnimIndex].mFrameCount - 1)) / 300.0f);
                }
                break;
            }
            case 9:
            case 10:
            case 11:
                mUnk_14[i] = new CellAnimation("result_b_obj_00_nce.bin", 0, 1, 1);
                mUnk_14[i]->mCharBase = screen.GetCharBase(0xc000);
                mUnk_14[i]->mPaletteBase = 0;
                mUnk_14[i]->SetAnimation(5);
                CellAnimation *last = mUnk_14[i];
                last->mFrameIndex = last->mFrames[last->mAnimIndex].mFrameCount - 1;
                break;
        }
    }
    ChangeDir("/Game/2D/union_b/");
    SpriteScreen &window = SpriteManager::Get().mScreens[1];
    window.LoadCharacters("union_b_obj_cockpit.nce_ncg1D.bin", 0xa000, 0);
    window.LoadPalette("union_b_obj_cockpit_ncl.bin", 0x40, 0);
    mUnk_7c = new SelectWindow(1, data_020a30a4, 0);
    mUnk_7c->SetCells(1, data_020a30c0);
    mUnk_7c->mCursor->mSelection = -1;
    mUnk_7c->mEnabled = 0;
    ChangeDir("/");
}

void UnkStruct_02076058::func_020770f4() {
    u32 changed = 0;
    for (u8 i = 0; i < 3; i++) {
        u8 slot = i + mUnk_75;
        if (mUnk_54[slot] != NULL && mUnk_64[slot] != 0) {
            mUnk_64[slot] += mUnk_54[slot]->mPad;
            if (func_020771d8(i, 1)) {
                changed = 1;
            }
            mUnk_64[slot] = 0;
        }
    }
    if (changed != 0) {
        SoundManager::Get().StartSeqArc(0x35);
        func_02077d2c();
    }
}

u32 UnkStruct_02076058::func_020771d8(u32 index, u32 unk2) {
    u32 result = 0;
    u32 count = 1;
    if (unk2 != 0) {
        count = 9;
    }
    u8 slot = index + mUnk_75;
    if (mUnk_54[slot] != NULL) {
        u8 k = 0;
        do {
            UnkStruct_02023248 *entry = mUnk_54[slot];
            u16 cost = (entry->mUnk_00_0 + 1) * 300;
            if (unk2 != 0) {
                u16 points = mUnk_64[slot];
                if (points >= cost) {
                    mUnk_64[slot] = points - cost;
                    if ((s32) mUnk_54[slot]->mUnk_00_0 < 9) {
                        mUnk_54[slot]->mUnk_00_0 = mUnk_54[slot]->mUnk_00_0 + 1;
                    }
                    s32 level = mUnk_54[slot]->mUnk_00_0;
                    if (level < 9) {
                        mUnk_14[index + 6]->mFrameIndex = level;
                    }
                    mUnk_14[index + 9]->ResetAnimation();
                    mUnk_77 = 1;
                    mUnk_79 = 1;
                    result = 1;
                } else {
                    mUnk_54[slot]->mPad = mUnk_64[slot];
                    break;
                }
            } else {
                u32 gauge = entry->mPad;
                if (gauge < cost) {
                    break;
                }
                entry->mPad = gauge - cost;
                if ((s32) mUnk_54[slot]->mUnk_00_0 < 9) {
                    mUnk_54[slot]->mUnk_00_0 = mUnk_54[slot]->mUnk_00_0 + 1;
                }
                s32 level = mUnk_54[slot]->mUnk_00_0;
                if (level < 9) {
                    mUnk_14[index + 6]->mFrameIndex = level;
                }
                mUnk_14[index + 9]->ResetAnimation();
                mUnk_77 = 1;
                mUnk_79 = 1;
                result = 1;
            }
            k++;
        } while (k < count);
        CellAnimation *anim = mUnk_14[index + 3];
        int gauge = mUnk_54[slot]->mPad;
        u16 rest = gauge % 300;
        anim->mFrameIndex = (u8) ((float) (rest * (anim->mFrames[anim->mAnimIndex].mFrameCount - 1)) / 300.0f);
    }
    return result;
}

void UnkStruct_02076058::func_02077458() {
    for (u8 i = 0; i < 3; i++) {
        u8 slot = i + 9;
        if (mUnk_14[slot] != NULL) {
            mUnk_14[slot]->Update();
            mUnk_14[slot]->Draw(1, data_020a308c[slot][0], data_020a308c[slot][1], 1);
        }
    }
    for (u32 i = 0; i < 3; i = (u8) (i + 1)) {
        u8 slot = i;
        if (mUnk_54[i + mUnk_75] != NULL && mUnk_14[slot] != NULL) {
            mUnk_14[slot]->Update();
            mUnk_14[slot]->DrawTransformed(1, data_020a308c[slot][0], data_020a308c[slot][1], 1, 0x800, 0x800, 0);
        }
    }
    for (u32 i = 0; i < 3; i = (u8) (i + 1)) {
        u8 slot = i + 3;
        UnkStruct_02023248 *entry = mUnk_54[i + mUnk_75];
        if (entry != NULL && (s32) entry->mUnk_00_0 < 9 && mUnk_14[slot] != NULL) {
            u32 last = mUnk_14[slot]->mFrames[mUnk_14[slot]->mAnimIndex].mFrameCount - 1;
            u8 count = entry->mPad / 300;
            u8 n = 0;
            if (count != 0) {
                u8 x = data_020a308c[slot][0];
                u8 y = data_020a308c[slot][1];
                do {
                    u8 cellX = x + n % 3 * 32;
                    u8 cellY = n / 3 * 6 + y;
                    mUnk_14[slot]->DrawFrame(1, (u8) last, cellX, cellY, 1);
                    n++;
                } while (n < count);
            }
            u8 drawX = data_020a308c[slot][0] + n % 3 * 32;
            u8 drawY = data_020a308c[slot][1] + n / 3 * 6;
            mUnk_14[slot]->Draw(1, drawX, drawY, 1);
        }
    }
    for (u8 i = 0; i < 3; i++) {
        u8 slot = i + 6;
        if (mUnk_54[i + mUnk_75] != NULL && (s32) mUnk_54[i + mUnk_75]->mUnk_00_0 < 9 && mUnk_14[slot] != NULL) {
            mUnk_14[slot]->Draw(1, data_020a308c[slot][0], data_020a308c[slot][1], 1);
        }
    }
    func_020781a0(this);
    if (mUnk_7c != NULL) {
        mUnk_7c->Draw();
    }
}

void UnkStruct_02076058::func_02077770() {
    if (InputManager::Get().IsHeld(1) | TouchManager::Get().mPressed) {
        func_020770f4();
    }
    u32 changed = 0;
    for (u8 i = 0; i < 3; i++) {
        u8 slot = i + mUnk_75;
        UnkStruct_02023248 *entry = mUnk_54[slot];
        if (entry != NULL) {
            if ((s32) entry->mUnk_00_0 >= 9) {
                entry->mPad = 0;
                mUnk_64[slot] = 0;
            } else if (mUnk_14[i + 9]->mFinished != 0 && mUnk_64[slot] != 0) {
                u32 step = 3;
                s32 points = mUnk_64[slot];
                if (points < 3) {
                    step = (u8) points;
                }
                mUnk_64[slot] -= step;
                mUnk_54[slot]->mPad = mUnk_54[slot]->mPad + step;
                if (func_020771d8(i, 0)) {
                    changed = 1;
                }
            }
        }
    }
    if (changed != 0) {
        SoundManager::Get().StartSeqArc(0x35);
        func_02077d2c();
    }
    u32 done = 1;
    for (u8 i = 0; i < 3; i++) {
        if (mUnk_64[i] != 0) {
            done = 0;
            break;
        }
    }
    for (u8 i = 0; i < 3; i++) {
        if (mUnk_14[i + 9]->mFinished == 0) {
            done = 0;
        }
    }
    if (done != 0) {
        if (mUnk_76 != 0) {
            mUnk_76--;
        } else {
            mUnk_74++;
        }
    }
}

u32 func_020779ec(void *unk) {
    UnkStruct_02076058 *object = (UnkStruct_02076058 *) unk;
    switch (object->mUnk_73) {
        case 0:
            if (object->mUnk_60 != NULL) {
                return 1;
            }
            break;
        case 1:
            if (object->mUnk_74 >= 5) {
                return 1;
            }
            break;
    }
    return 0;
}

void UnkStruct_02076058::func_02077a30(u32 index, u32 unk2, u32 unk3, u8 unk4) {
    if (!(index < 3 && unk2 < 0x29 && unk3 < 6)) {
        return;
    }
    mUnk_6a[index] = unk2;
    mUnk_6d[index] = unk3;
    mUnk_54[index] = func_02023248(gSaveData, unk2, unk3);
    mUnk_70[index] = mUnk_54[index]->mUnk_00_0;
    if (unk4 != 0) {
        func_02023248(gSaveData, unk2, unk3);
        u16 cost = (mUnk_54[index]->mUnk_00_0 + 1) * 300;
        mUnk_64[index] = cost - mUnk_54[index]->mPad;
    }
}

void UnkStruct_02076058::func_02077ae8() {
    SpriteManager::Get().Screen(1).SetObjVramMode(3);
    ChangeDir("/font/");
    mUnk_0c = (MessageFile *) LoadFile("mes_BakuganName.mes", 0, 0, 0, 0, 0);
    mUnk_10 = (MessageFile *) LoadFile("mes_Result.mes", 0, 0, 0, 0, 0);
    mUnk_4c = TextManager::Get().BufferSize(0x18, 0x10);
    mUnk_44 = OS_Alloc(mUnk_4c);
    MI_CpuClear16(mUnk_44, mUnk_4c);
    mUnk_50 = TextManager::Get().GlyphCount(0x30);
    mUnk_48 = OS_Alloc((s16) mUnk_50 * (s16) 0xc);
    TextManager::Get().Init(mUnk_48, 1, 0, 0x18, 0x10, 0x21);
    TextManager::Get().SetColor(1, 0, 0xf);
    func_02077d2c();
}

void UnkStruct_02076058::func_02077d2c() {
    func_02078250(this);
    TextManager::Get().mUse4bppFont = 1;
    for (u8 i = 0; i < 3; i++) {
        if (mUnk_54[i + mUnk_75] != NULL) {
            s16 x = 0;
            s16 y = i * 0x28;
            u16 pixelWidth;
            u8 id = (&mUnk_6a[i])[mUnk_75];
            TextManager &text = TextManager::Get();
            s16 nameX;
            s16 nameY;
            nameY = y;
            nameX = x;
            MessageFile *names = mUnk_0c;
            text.Print(names->GetText(id), names->GetLength(id), mUnk_44, 0x18, 0x10, &nameX, &nameY, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
            y += 12;
            TextManager &text2 = TextManager::Get();
            s16 labelX;
            s16 labelY;
            labelY = y;
            labelX = x;
            MessageFile *labels = mUnk_10;
            text2.Print(labels->GetText(5), labels->GetLength(5), mUnk_44, 0x18, 0x10, &labelX, &labelY, &pixelWidth, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
            x += pixelWidth;
            u8 level = mUnk_54[i + mUnk_75]->mUnk_00_0;
            if (level < 9) {
                TextManager::Get().PrintNumber(level + 1, 1, 0, mUnk_44, 0x18, 0x10, &x, &y, 0, 2, 0x21, 0, 0, 1, 1);
            } else {
                TextManager &text3 = TextManager::Get();
                s16 maxX;
                s16 maxY;
                maxY = y;
                maxX = x;
                MessageFile *labels2 = mUnk_10;
                text3.Print(labels2->GetText(6), labels2->GetLength(6), mUnk_44, 0x18, 0x10, &maxX, &maxY, 0, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
            }
        }
    }
    DC_FlushRange(mUnk_44, mUnk_4c);
    TextManager::Get().mUse4bppFont = 0;
    QueueUpload(SpriteManager::Get().Screen(1).mBgIndex, 4, 0x6000, mUnk_44, mUnk_4c, 0);
}

void func_020781a0(UnkStruct_02076058 *object) {
    SpriteManager::Get().Screen(1).AddOam(object->mUnk_48, object->mUnk_50, 0x46, 0x29, 1, 0xc0, 0xf, 0);
}

void func_02078250(UnkStruct_02076058 *object) {
    if (object->mUnk_44 != NULL) {
        MI_CpuClear16(object->mUnk_44, object->mUnk_4c);
    }
}

BOOL func_02078270(UnkStruct_02076058 *object) {
    if (object->mUnk_79 != 0) {
        object->mUnk_79 = 0;
        return TRUE;
    }
    return FALSE;
}
