
#include "Battle/BattleBase.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/fx.h>

// Fixed-point conversion of a float value, rounded to nearest.

extern const s32 data_ov007_022790c8[][6];
extern const u8 gCircleSlots[];

int PointDistance(int x1, int y1, int x2, int y2);
void NormalizeXY(float x, float y, float *out);

// Placeholder: an enum-typed copy of the entry index (mwcc does not propagate enum locals) gives the original
// register pair for the first entries[] access; the real source form is not known.
enum EntryIndex { ENTRY_INDEX_0 };

BattleSub48::BattleSub48(BattleData *data, u8 mode)
    : SceneBase(2, 0) {
    mState = 0;
    mMoveStep = 0;
    mUnk_0e_0 = 0;
    mCircleUpDownVisible = 0;
    mSideUpDownVisible = 0;
    mNoEmblem = 0;
    mUnk_0e_4 = 0;
    mFieldIndex = 0;
    mStepFrames = 0;
    mAbilitiesY = 0xc0;
    mAbilitiesYPerFrame = 10;
    mLocalSide = mode;
    mData = data;
    BattleEntry *entries = mData->mEntries;
    u8 i = 0;
    do {
        mCellAnims[i] = 0;
        i++;
    } while (i < 15);
    if (data_020d433c.mFieldId != 0 && data_020d433c.mFieldId != 7) {
        mFieldIndex = data_020d433c.mFieldId - 1;
        u8 k = 0;
        do {
            EntryIndex kk = (EntryIndex) k;
            u8 nib = entries[kk].mAttribute;
            s32 value = data_ov007_022790c8[mFieldIndex][nib];
            mSideAttributes[k] = nib;
            if (entries[k].mBakuganId == 5) {
                if (value < data_ov007_022790c8[mFieldIndex][3]) {
                    mSideAttributes[k] = 3;
                }
            } else if (entries[k].mBakuganId == 6) {
                if (value < data_ov007_022790c8[mFieldIndex][0]) {
                    mSideAttributes[k] = 0;
                }
            }
            u8 c = gCircleSlots[mSideAttributes[k]];
            mCircleY = 90;
            int step = (u16) (c * 0x2aaa + 0xc000) >> 4;
            mPos[k][0] = 110.0f + 50.0f * ((float) FX_SinCosTable_[step * 2 + 1] / 4096.0f);
            mPos[k][1] = (float) (u32) mCircleY + 50.0f * ((float) FX_SinCosTable_[step * 2] / 4096.0f);
            k++;
        } while (k < 2);
        if (!entries[0].mNextBattleRequested && !entries[1].mNextBattleRequested) {
            mState = 1;
        } else {
            u32 left = 8;
            u32 right = 0xdc;
            mPos[0][0] = left;
            mPos[0][1] = 5.0f;
            mPos[1][0] = right;
            mPos[1][1] = 5.0f;
            mUnk_0e |= 0x14;
        }
    } else {
        mUnk_0e |= 8;
    }
    LoadGraphics();
}

BattleSub48::~BattleSub48() {
    u8 i = 0;
    do {
        CellAnimation *anim = mCellAnims[i];
        if (anim) {
            delete anim;
        }
        i++;
    } while (i < 15);
}

void BattleSub48::LoadCharactersAndPalettes() {
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.SetObjVramMode(1);
    ChangeDir("/Game/Confrontation/Obj/");
    screen.LoadCharacters("vs_u_obj_ability_32k.nce_ncg1D.bin", 0x1000, 0);
    screen.LoadPalette("vs_u_obj_ability_32k_ncl.bin", 0x20, 0x40);
    screen.LoadCharacters("vs_u_obj_updown_32k.nce_ncg1D.bin", 0x3400, 0);
    screen.LoadPalette("vs_u_obj_updown_32k_ncl.bin", 0x60, 0);
    int emblem = -1;
    switch (data_020d433c.mFieldId) {
        case 1:
            emblem = 0;
            break;
        case 2:
            emblem = 1;
            break;
        case 4:
            emblem = 2;
            break;
        case 5:
            emblem = 3;
            break;
        case 6:
            emblem = 4;
            break;
        case 3:
            emblem = 5;
            break;
    }
    if (emblem != -1) {
        char path[0x80];
        sprintf(path, "vs_emblem_%02d_32k.nce_ncg1D.bin", emblem);
        screen.LoadCharacters(path, 0x6000, 0);
        sprintf(path, "vs_emblem_%02d_32k_ncl.bin", emblem);
        screen.LoadPalette(path, 0x140, 0);
    }
    ChangeDir("/");
}

void BattleSub48::LoadGraphics() {
    LoadCharactersAndPalettes();
    BattleEntry *entries = mData->Entries();
    int kind;
    int other;
    u8 i;
    u8 j;
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    screen.SetObjVramMode(1);
    ChangeDir("/Game/Confrontation/Obj/");
    if (mLocalSide < 2) {
        if (entries[mLocalSide].mCardId != 0) {
            kind = GetCardInfo(entries[(u8) mLocalSide].mCardId, 3);
            other = func_ov007_02222764((u8) mLocalSide, entries);
            if (kind != 5 && other != 5) {
                i = 0;
                do {
                    mCellAnims[i] = new CellAnimation("vs_u_obj_ability_32k_nce.bin", 0, 1, 1);
                    mCellAnims[i]->mCharBase = screen.GetCharBase(0x1000);
                    mCellAnims[i]->mPaletteBase = 1;
                    i++;
                } while (i < 4);
                mCellAnims[0]->SetAnimation(9);
                mCellAnims[1]->SetAnimation(8);
                mUnk_0e_0 = 0;
                switch (kind) {
                    case 0:
                        mCellAnims[3]->SetAnimation(0);
                        break;
                    case 1:
                        mCellAnims[3]->SetAnimation(1);
                        break;
                    case 2:
                        mCellAnims[3]->SetAnimation(2);
                        break;
                    case 3:
                        mCellAnims[3]->SetAnimation(3);
                        break;
                    case 4:
                        mUnk_0e_0 = 1;
                        mCellAnims[2]->SetAnimation(10);
                        switch (other) {
                            case 0:
                                mCellAnims[3]->SetAnimation(5);
                                break;
                            case 1:
                                mCellAnims[3]->SetAnimation(4);
                                break;
                            case 2:
                                mCellAnims[3]->SetAnimation(6);
                                break;
                            case 3:
                                mCellAnims[3]->SetAnimation(7);
                                break;
                        }
                        break;
                }
                if (data_020d433c.mUnk_9a_2) {
                    mCellAnims[2]->SetAnimation(10);
                    mUnk_0e_0 = 1;
                }
            }
        }
    }
    i = 4;
    do {
        mCellAnims[i] = new CellAnimation("vs_u_obj_ability_32k_nce.bin", 0, 1, 1);
        mCellAnims[i]->mCharBase = screen.GetCharBase(0x1000);
        mCellAnims[i]->mPaletteBase = 1;
        i++;
    } while (i < 7);
    mCellAnims[4]->SetAnimation(11);
    i = 0;
    j = 5;
    do {
        if (entries[i].mKind3CardRemoved) {
            mCellAnims[j]->SetAnimation(3);
        } else {
            int k = GetCardInfo(entries[i].mCardId, 3);
            if (k == 1) {
                mCellAnims[j]->SetAnimation(1);
            } else if (k == 3) {
                mCellAnims[j]->SetAnimation(3);
            }
        }
        j++;
        i++;
    } while (i < 2);
    if (!mNoEmblem) {
        i = 0;
        j = 7;
        do {
            s32 value = data_ov007_022790c8[mFieldIndex][i];
            mCellAnims[j] = new CellAnimation("vs_u_obj_updown_32k_nce.bin", 0, 1, 1);
            mCellAnims[j]->mCharBase = screen.GetCharBase(0x3400);
            mCellAnims[j]->mPaletteBase = 3;
            if (value > 0x1000) {
                mCellAnims[j]->SetAnimation(0);
            } else if (value < 0x1000) {
                mCellAnims[j]->SetAnimation(1);
            } else {
                mCellAnims[j]->SetAnimation(2);
            }
            j++;
            i++;
        } while (i < 6);
        mCellAnims[13] = new CellAnimation("vs_emblem_00_32k_nce.bin", 0, 1, 1);
        mCellAnims[13]->mCharBase = screen.GetCharBase(0x6000);
        mCellAnims[13]->mPaletteBase = 10;
        mCellAnims[13]->SetAnimation(0);
        mCellAnims[14] = new CellAnimation("vs_u_obj_updown_32k_nce.bin", 0, 1, 1);
        mCellAnims[14]->mCharBase = screen.GetCharBase(0x3400);
        mCellAnims[14]->mPaletteBase = 3;
        mCellAnims[14]->SetAnimation(3);
    }
    ChangeDir("/");
}

void BattleSub48::Update() {
    switch (mState) {
        case 1:
            DrawCircleAndSides();
            break;
        case 2:
            mAbilitiesY = mAbilitiesY - mAbilitiesYPerFrame;
            mAbilitiesYPerFrame = mAbilitiesYPerFrame - 2;
            if (mAbilitiesYPerFrame < 2) {
                mAbilitiesYPerFrame = 2;
            }
            if (mAbilitiesY <= 0x8c) {
                mAbilitiesY = 0x8c;
                mState++;
            }
            DrawAbilitiesAndSides();
            break;
        case 3:
            DrawAbilitiesAndSides();
            break;
        case 4:
            mFinished = 1;
            break;
    }
}

extern const u8 gSideTargetX[];

u8 BattleSub48::UpdateCircleAndSidesMove() {
    if (mState == 0) {
        return 1;
    }
    u8 i;
    u8 ret = 0;
    switch (mMoveStep) {
        case 0: {
            i = 0;
            do {
                mVel[i][0] = ((float) (u32) gSideTargetX[i] - mPos[i][0]) / 60.0f;
                mVel[i][1] = (5.0f - mPos[i][1]) / 60.0f;
                i++;
            } while (i < 2);
            mUnk_0e |= 2;
            mStepFrames = 0;
            mMoveStep++;
            break;
        }
        case 1:
            mStepFrames++;
            if (mStepFrames > 0x78) {
                mStepFrames = 0;
                mUnk_0e &= ~2;
                mMoveStep++;
            }
            break;
        case 2:
            mCircleY += 3;
            if (mCircleY >= 250) {
                mMoveStep++;
            }
            break;
        case 3:
            mPos[0][0] = mPos[0][0] + mVel[0][0];
            mPos[0][1] = mPos[0][1] + mVel[0][1];
            mPos[1][0] = mPos[1][0] + mVel[1][0];
            mPos[1][1] = mPos[1][1] + mVel[1][1];
            mStepFrames++;
            if (mStepFrames >= 0x3c) {
                mStepFrames = 0;
                i = 0;
                do {
                    s32 value = data_ov007_022790c8[mFieldIndex][mSideAttributes[i]];
                    u8 slot = mSideAttributes[i] + 7;
                    if (value > 0x1000) {
                        mCellAnims[slot]->SetAnimation(0);
                    } else if (value < 0x1000) {
                        mCellAnims[slot]->SetAnimation(1);
                    } else {
                        mCellAnims[slot]->SetAnimation(2);
                    }
                    i++;
                } while (i < 2);
                mUnk_0e |= 4;
                ret = 1;
            }
            break;
    }
    return ret;
}

void BattleSub48::DrawCircleAndSides() {
    mCellAnims[13]->Draw(0, 0x6e, (s16) mCircleY, 1);
    u16 x;
    u16 y;
    int step;
    u8 i = 0;
    do {
        u8 c = gCircleSlots[i];
        u16 angle = c * 0x2aaa + 0xc000;
        if (mCircleUpDownVisible) {
            u8 slot = i + 7;
            if (mCellAnims[slot]->mAnimIndex != 2) {
                step = angle >> 4;
                x = 110.0f + 70.0f * ((float) FX_SinCosTable_[step * 2 + 1] / 4096.0f);
                y = (float) (u32) mCircleY + 70.0f * ((float) FX_SinCosTable_[step * 2] / 4096.0f);
                mCellAnims[slot]->Update();
                mCellAnims[slot]->Draw(0, (s16) x, (s16) y, 1);
            }
        }
        if (gCircleSlots[mSideAttributes[0]] != c && gCircleSlots[mSideAttributes[1]] != c) {
            step = angle >> 4;
            x = 110.0f + 50.0f * ((float) FX_SinCosTable_[step * 2 + 1] / 4096.0f);
            y = (float) (u32) mCircleY + 50.0f * ((float) FX_SinCosTable_[step * 2] / 4096.0f);
            mCellAnims[14]->DrawFrame(0, i, x, y, 1);
        }
        i++;
    } while (i < 6);
    DrawSides();
}

void BattleSub48::DrawSides() {
    u8 i = 0;
    do {
        u16 x = mPos[i][0];
        u16 y = mPos[i][1];
        if (mSideUpDownVisible) {
            u8 slot = mSideAttributes[i] + 7;
            mCellAnims[slot]->Update();
            mCellAnims[slot]->Draw(0, (s16) (x + (i == 0 ? 30 : -30)), (s16) y, 1);
        }
        mCellAnims[14]->DrawFrame(0, mSideAttributes[i], x, y, 1);
        i++;
    } while (i < 2);
}

// The original reloads xs[i] after the if/else instead of reusing the value both arms loaded: the reload indexes
// through an enum-typed copy of i, which mwcc does not identify with i.
void BattleSub48::DrawAbilitiesAndSides() {
    s16 y;
    s16 xs[2];
    BattleEntry *entries;
    s16 x;
    u8 i;
    y = mAbilitiesY;
    xs[0] = 0x24;
    xs[1] = 0xdc;
    entries = mData->mEntries;
    i = 0;
    if (y >= 0xc0) {
        return;
    }
    do {
        if (entries[i].mCardId != 0) {
            if (mCellAnims[3] == 0) {
                break;
            }
            if ((i == 0 && mLocalSide == 0) || (i == 1 && mLocalSide == 1)) {
                u8 other = i ^ 1;
                if (!entries[other].mUnk_0f_3) {
                    if (!mUnk_0e_0) {
                        mCellAnims[1]->Update();
                        mCellAnims[1]->Draw(0, xs[i], y, 1);
                    } else {
                        mCellAnims[2]->Draw(0, xs[i], y, 1);
                    }
                    EntryIndex k = (EntryIndex) i;
                    x            = xs[k];
                    mCellAnims[3]->Draw(0, x, y, 1);
                    mCellAnims[0]->Draw(0, x, y, 1);
                    if (i == 0) {
                        xs[0] += 0x41;
                    } else {
                        xs[1] -= 0x41;
                    }
                }
            }
        }
        i++;
    } while (i < 2);
    i = 0;
    do {
        if (entries[i].mCardId == 0) {
            if (entries[i].mKind3CardRemoved) {
                s16 x = xs[i];
                u8 slot = i + 5;
                mCellAnims[slot]->Draw(0, x, y, 1);
                mCellAnims[4]->Update();
                mCellAnims[4]->Draw(0, x, y, 1);
                if (i == 0) {
                    xs[0] += 0x41;
                } else {
                    xs[1] -= 0x41;
                }
            } else if (entries[i].mUnk_0f_3) {
                u8 j = i ^ 1;
                s16 x = xs[j];
                u8 slot = i + 5;
                mCellAnims[slot]->Draw(0, x, y, 1);
                mCellAnims[4]->Update();
                mCellAnims[4]->Draw(0, x, y, 1);
                if (j == 0) {
                    xs[0] -= 0x41;
                } else {
                    xs[1] -= 0x41;
                }
            }
        }
        i++;
    } while (i < 2);
    if (mNoEmblem) {
        return;
    }
    DrawSides();
}

void StartAbilitiesSlide(BattleSub48 *obj) {
    obj->mAbilitiesY = 0xc0;
    obj->mAbilitiesYPerFrame = 10;
    obj->mState = 2;
}

void ClearSub48State(BattleSub48 *obj) {
    obj->mState = 0;
}

int PointDistance(int x1, int y1, int x2, int y2) {
    s16 dy = y2 - y1;
    s16 dx = x2 - x1;
    int sq = dx * dx + dy * dy;
    return (int) ((float) FX_Sqrt(FX32_CONST((float) sq)) / 4096.0f);
}

// out is left unchanged for a zero vector.
void NormalizeXY(float x, float y, float *out) {
    float len = (float) FX_Sqrt(FX32_CONST(x * x + y * y)) / 4096.0f;
    if (len == 0.0f) {
        return;
    }
    out[0] = x / len;
    out[1] = y / len;
}
