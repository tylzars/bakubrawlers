#include "Battle/BattleStatusPanels.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include "Text/TextManager.hpp"
#include "Unknown/UnkStruct_020651bc.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>

BattleStatusPanels::BattleStatusPanels(u32 charBase, u8 palette, u8 mode, u8 flag) {
    mIcons      = 0;
    mAnimC      = 0;
    mAnimD      = 0;
    mCharBase   = charBase;
    mPalette    = palette;
    mUnk_f1     = flag;
    mUnk_f2     = 0;
    mBufferSize = 0;
    mGlyphCount = 0;
    mMode       = mode;
    mUnk_fd     = 0;
    MI_CpuClear32(mPanels, 0xe0);
    u8 i = 0;
    do {
        mPanels[i].mUnk_24 = -1;
        mPanels[i].mUnk_25 = i & 1;
        mPanels[i].mUnk_26 = i & 1;
        mPanels[i].mUnk_27 = (u32) i >> 1;
        if (data_020d433c.mBattlePlayers->mPlayers[i]) {
            mPanels[i].mUnk_28 = data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_ee;
        }
        if (data_020d433c.mUnk_98) {
            if (data_020d433c.mBattlePlayers->mPlayers[i]) {
                mPanels[i].mUnk_28 += data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mUnk_9b[i]]->mUnk_ee;
            }
            mPanels[i].mUnk_29 = 3;
        } else {
            mPanels[i].mUnk_29 = data_020d433c.mUnk_99;
        }
        i++;
    } while (i < 4);
    func_ov007_02234efc();
    func_ov007_02235fe4();
}

BattleStatusPanels::~BattleStatusPanels() {
    u8 i = 0;
    do {
        u8 j = 0;
        do {
            if (mPanels[i].mAnims[j]) {
                delete mPanels[i].mAnims[j];
            }
            j++;
        } while (j < 7);
        if (mPanels[i].mBuffer) {
            OS_Free(mPanels[i].mBuffer);
            mPanels[i].mBuffer = 0;
        }
        if (mPanels[i].mBuffer2) {
            OS_Free(mPanels[i].mBuffer2);
            mPanels[i].mBuffer2 = 0;
        }
        i++;
    } while (i < 4);
    if (mAnimC) {
        delete mAnimC;
    }
    if (mAnimD) {
        delete mAnimD;
    }
    if (mIcons) {
        delete mIcons;
    }
}

// non-matching: 18 words, register naming only.
void BattleStatusPanels::func_ov007_02234efc() {
    mIcons = new UnkStruct_020651bc(mCharBase + 0x1800, (u8) (mPalette + 6), 0);
    ChangeDir("/Game/Battle/Obj/");
    u8 j;
    SpriteScreen &screen = SpriteManager::Get().Screen(0);
    u8 i;
    screen.func_02027624(1);
    screen.LoadCharacters("battle_u.nce_ncg1D.bin", mCharBase, 0);
    screen.LoadPalette("battle_u_ncl.bin", mPalette << 5, 0);
    if (data_020d433c.mUnk_98) {
        screen.LoadCharacters("battle_u_team.nce_ncg1D.bin", mCharBase + 0x4000, 0);
    }
    i = 0;
    do {
        j = 0;
        do {
            mPanels[i].mAnims[j]            = new CellAnimation("battle_u_nce.bin", 0, 1, 1);
            mPanels[i].mAnims[j]->mCharBase = screen.func_020276f4(mCharBase);
            mPanels[i].mAnims[j]->mUnk_11   = mPalette;
            j++;
        } while (j < 6);
        mPanels[i].mAnims[0]->SetAnimation(mPanels[i].mUnk_25 + 6);
        mPanels[i].mAnims[1]->SetAnimation(0);
        mPanels[i].mAnims[2]->SetAnimation(mPanels[i].mUnk_25 + 4);
        mPanels[i].mAnims[3]->SetAnimation(2);
        mPanels[i].mAnims[3]->mUnk_0f = i;
        mPanels[i].mAnims[4]->SetAnimation(i + 0x12);
        mPanels[i].mAnims[5]->SetAnimation(3);
        mPanels[i].mAnims[5]->mUnk_0f = i;
        if (data_020d433c.mUnk_98) {
            mPanels[i].mAnims[6]            = new CellAnimation("battle_u_team_nce.bin", 0, 1, 1);
            mPanels[i].mAnims[6]->mCharBase = screen.func_020276f4(mCharBase + 0x4000);
            mPanels[i].mAnims[6]->mUnk_11   = mPalette;
            mPanels[i].mAnims[6]->SetAnimation(i);
        }
        i++;
    } while (i < 4);
    if (data_020d433c.mUnk_98) {
        mAnimC            = new CellAnimation("battle_u_team_nce.bin", 0, 1, 1);
        mAnimC->mCharBase = screen.func_020276f4(mCharBase + 0x4000);
        mAnimC->mUnk_11   = mPalette;
        mAnimC->SetAnimation(4);
    }
    mAnimD            = new CellAnimation("battle_u_nce.bin", 0, 1, 1);
    mAnimD->mCharBase = screen.func_020276f4(mCharBase);
    mAnimD->mUnk_11   = mPalette;
    mAnimD->SetAnimation(1);
    ChangeDir("/");
}

void BattleStatusPanels::func_ov007_02235280(u32 index, s32 character, u32 team, u8 side) {
    if (index >= 4) {
        return;
    }
    mPanels[index].mUnk_24     = character;
    if (team == 4) {
        mPanels[index].mUnk_26 = index & 1;
        mPanels[index].mUnk_27 = index >> 1;
        team       = index;
    } else {
        mPanels[index].mUnk_26 = team & 1;
        mPanels[index].mUnk_27 = team >> 1;
    }
    if (side == 2) {
        mPanels[index].mUnk_25 = team & 1;
    } else {
        mPanels[index].mUnk_25 = side & 1;
    }
    mPanels[index].mUnk_1c[0] = 0;
    mPanels[index].mUnk_1c[1] = 0;
    mPanels[index].mUnk_1c[2] = 0;
    mPanels[index].mUnk_1c[3] = 0;
    BOOL c        = FALSE;
    if (mUnk_f1 == 1) {
        c = TRUE;
    }
    if (c & (index == data_020d433c.mBattlePlayers->mUnk_28a)) {
        mPanels[index].mAnims[0]->SetAnimation(index * 2 + 10 + mPanels[index].mUnk_25);
    } else {
        mPanels[index].mAnims[0]->SetAnimation(mPanels[index].mUnk_25 + 6);
    }
    mPanels[index].mAnims[2]->SetAnimation(mPanels[index].mUnk_25 + 4);
    u8 level = func_ov007_02236458(index);
    if (level >= 0x13) {
        u8 cur  = data_020d433c.mBattlePlayers->mUnk_28a;
        u8 cur2 = data_020d433c.mBattlePlayers->mUnk_2a9;
        if ((index == cur) & (cur == cur2)) {
            mPanels[index].mAnims[2]->SetAnimation(mPanels[index].mUnk_25 + 0x16);
        } else {
            mPanels[index].mAnims[2]->mUnk_0f = 0x12;
        }
    }
    mPanels[index].mUnk_2b = level;
    mIcons->func_02065204(index, mPanels[index].mUnk_24, data_020d433c.mBattlePlayers->mPlayers[index]->NameInfo(), 0);
    func_ov007_022361c4(index);
    BattlePlayer *pl = data_020d433c.mBattlePlayers->mPlayers[index];
    if (pl->mUnk_fe != 0) {
        pl->mUnk_fe = 0;
        mPanels[index].mUnk_36  = 1;
        mUnk_fd     = 1;
    }
}

// non-matching: ~42 words. Head of the loop body is scheduled differently (the data_020d433c pointer load and the
// x/y/dir temporaries use other registers), the args of func_02027778 get other registers (0xf, 0 and the VRAM offset),
// and one add has its operands swapped (k + mUnk_25).
void BattleStatusPanels::func_ov007_02235480() {
    int shown1 = 0;
    int shown2 = 0;
    u8 i        = 0;
    do {
        if (mPanels[i].mUnk_24 > -1) {
            if (mPanels[i].mUnk_1c[0] != mPanels[i].mUnk_1c[2]) {
                if (mPanels[i].mUnk_1c[0] < mPanels[i].mUnk_1c[2]) {
                    mPanels[i].mUnk_1c[0]++;
                } else {
                    mPanels[i].mUnk_1c[0]--;
                }
            }
            if (mPanels[i].mUnk_1c[1] != mPanels[i].mUnk_1c[3]) {
                if (mPanels[i].mUnk_1c[1] < mPanels[i].mUnk_1c[3]) {
                    mPanels[i].mUnk_1c[1]++;
                } else {
                    mPanels[i].mUnk_1c[1]--;
                }
            }
            s16 baseX = mPanels[i].mUnk_26 * 0xd8 + 6;
            s16 baseY = mPanels[i].mUnk_27 * 0x90 + 8;
            s16 x     = mPanels[i].mUnk_1c[0] + baseX;
            s16 y     = mPanels[i].mUnk_1c[1] + baseY;
            s8 dir    = 1 - mPanels[i].mUnk_25 * 2;
            s16 q     = x + 0xe;
            if (i == data_020d433c.mBattlePlayers->mUnk_2a9) {
                mPanels[i].mAnims[4]->Draw(0, (s16) (dir * (mPanels[i].mUnk_25 * 0x24 - 0x10) + q), (s16) (y + 0x22), mUnk_f2);
            } else if (data_020d433c.mUnk_98 &&
                       i == data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mUnk_2a9]->mUnk_f2) {
                mPanels[i].mAnims[6]->Draw(0, (s16) (dir * (mPanels[i].mUnk_25 * 0x24 - 0x10) + q), (s16) (y + 0x22), mUnk_f2);
            }
            if (data_020d433c.mUnk_98 && mMode == 0) {
                if (i == 0 || i == data_020d433c.mBattlePlayers->mPlayers[0]->mUnk_f2) {
                    BOOL c = TRUE;
                    if (mPanels[data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_f2].mUnk_36 != 0) {
                        c = FALSE;
                    }
                    if (!shown1 && c) {
                        shown1 = true;
                        if (mPanels[i].mUnk_29 != 0) {
                            if (mPanels[i].mUnk_36 != 0) {
                                if (SpriteManager::Get().sFade.mMain <= 1) {
                                    if (mUnk_fd) {
                                        mUnk_fd = 0;
                                        SoundManager::Get().func_020246f8(6);
                                    }
                                    mAnimD->Update();
                                }
                                mAnimD->Draw(0, (s16) ((mPanels[i].mUnk_28 - 1) * 9 + 0x74), 0x10, mUnk_f2);
                            }
                            u8 k = 0;
                            do {
                                u8 f;
                                if (k < mPanels[i].mUnk_28) {
                                    f = 0;
                                    if (mPanels[i].mUnk_36 != 0) {
                                        if (k == mPanels[i].mUnk_28 - 1) {
                                            f = 1;
                                        }
                                    }
                                } else {
                                    f = 1;
                                }
                                mPanels[i].mAnims[1]->func_0202a3ec(0, f, k * 9 + 0x74, 0x10, mUnk_f2);
                                k++;
                            } while (k < mPanels[i].mUnk_29);
                        }
                    }
                } else {
                    BOOL c = TRUE;
                    if (mPanels[data_020d433c.mBattlePlayers->mPlayers[i]->mUnk_f2].mUnk_36 != 0) {
                        c = FALSE;
                    }
                    if (!shown2 && c) {
                        shown2 = true;
                        if (mPanels[i].mUnk_29 != 0) {
                            if (mPanels[i].mUnk_36 != 0) {
                                if (SpriteManager::Get().sFade.mMain <= 1) {
                                    if (mUnk_fd) {
                                        mUnk_fd = 0;
                                        SoundManager::Get().func_020246f8(6);
                                    }
                                    mAnimD->Update();
                                }
                                mAnimD->Draw(0, (s16) ((mPanels[i].mUnk_28 - 1) * 9 + 0x74), 0xa8, mUnk_f2);
                            }
                            u8 k = 0;
                            do {
                                u8 f;
                                if (k < mPanels[i].mUnk_28) {
                                    f = 0;
                                    if (mPanels[i].mUnk_36 != 0) {
                                        if (k == mPanels[i].mUnk_28 - 1) {
                                            f = 1;
                                        }
                                    }
                                } else {
                                    f = 1;
                                }
                                mPanels[i].mAnims[1]->func_0202a3ec(0, f, k * 9 + 0x74, 0xa8, mUnk_f2);
                                k++;
                            } while (k < mPanels[i].mUnk_29);
                        }
                    }
                }
            } else if (mPanels[i].mUnk_29 != 0) {
                if (mPanels[i].mUnk_36 != 0) {
                    if (SpriteManager::Get().sFade.mMain <= 1) {
                        if (mUnk_fd) {
                            mUnk_fd = 0;
                            SoundManager::Get().func_020246f8(6);
                        }
                        mAnimD->Update();
                    }
                    mAnimD->Draw(0, (s16) (dir * ((mPanels[i].mUnk_25 + (mPanels[i].mUnk_28 - 1)) * 9 + 0x14) + q), (s16) (y + 0xc), mUnk_f2);
                }
                u8 k = 0;
                do {
                    u8 f;
                    if (k < mPanels[i].mUnk_28) {
                        f = 0;
                        if (mPanels[i].mUnk_36 != 0) {
                            if (k == mPanels[i].mUnk_28 - 1) {
                                f = 1;
                            }
                        }
                    } else {
                        f = 1;
                    }
                    u8 n25 = mPanels[i].mUnk_25;
                    mPanels[i].mAnims[1]->func_0202a3ec(0, f, dir * ((n25 + k) * 9 + 0x14) + q, y + 0xc, mUnk_f2);
                    k++;
                } while (k < mPanels[i].mUnk_29);
            }
            u8 level = func_ov007_02236458(i);
            if (level != 0) {
                if (mPanels[i].mUnk_2b >= 0x13) {
                    mPanels[i].mAnims[2]->Update();
                    mPanels[i].mAnims[2]->Draw(0, (s16) (x - 2), (s16) (y - 2), mUnk_f2);
                } else {
                    mPanels[i].mAnims[2]->func_0202a3ec(0, level - 1, x - 2, y - 2, mUnk_f2);
                    if (level >= 0x13) {
                        u8 cur  = data_020d433c.mBattlePlayers->mUnk_28a;
                        u8 cur2 = data_020d433c.mBattlePlayers->mUnk_2a9;
                        if ((i == cur) & (cur == cur2)) {
                            mPanels[i].mAnims[2]->SetAnimation(mPanels[i].mUnk_25 + 0x18);
                        } else {
                            mPanels[i].mAnims[2]->mUnk_0f = 0x12;
                        }
                    }
                }
            }
            mPanels[i].mUnk_2b = level;
                        mPanels[i].mAnims[3]->Draw(0, (s16) (q + dir * mPanels[i].mUnk_25 * 16 + dir * -0x12), (s16) (y - 8), mUnk_f2);
            if (data_020d433c.mBattlePlayers->mPlayers[i]->IsActive()) {
                mPanels[i].mAnims[5]->Draw(0, (s16) (dir * (mPanels[i].mUnk_25 * 0x18 - 2) + q), (s16) (y - 8), mUnk_f2);
            }
            s16 px = dir * (mPanels[i].mUnk_34 * mPanels[i].mUnk_25 + 0xe) + q;
            if (dir < 0) {
                if (px < 0x82) {
                    px = 0x82;
                }
            } else {
                int e = px + mPanels[i].mUnk_34;
                if (e >= 0x7e) {
                    e -= 0x7e;
                    px = px - e;
                }
            }
            switch (mMode) {
                case 0:
                    SpriteManager::Get().Screen(0).func_02027778(mPanels[i].mBuffer2, (u8) mGlyphCount, px, (s16) (y - 1), mUnk_f2,
                                                                 ((mCharBase + 0x2000 + (i << 11)) << 11) >> 16, 0xf,
                                                                 0);
                    break;
                case 1:
                    SpriteManager::Get().Screen(0).func_02027778(mPanels[i].mBuffer2, (u8) mGlyphCount, px, (s16) (y - 1), mUnk_f2,
                                                                 ((mCharBase + 0x2000) << 11) >> 16, 0xf, 0);
                    break;
            }
            mIcons->func_0206547c(i, x, y, mUnk_f2);
        }
        i++;
    } while (i < 4);
    mIcons->func_020654a0();
    u8 j = 0;
    do {
        s16 baseX      = mPanels[j].mUnk_26 * 0xd8 + 6;
        s16 baseY      = mPanels[j].mUnk_27 * 0x90 + 8;
        s8 character   = mPanels[j].mUnk_24;
        s16 x          = mPanels[j].mUnk_1c[0] + baseX;
        s16 y          = mPanels[j].mUnk_1c[1] + baseY;
        if (character > -1) {
            mPanels[j].mAnims[0]->Update();
            mPanels[j].mAnims[0]->Draw(0, (s16) (x - 2), (s16) (y - 2), mUnk_f2);
        }
        j++;
    } while (j < 4);
    if (!data_020d433c.mUnk_98) {
        return;
    }
    if (mMode != 0) {
        return;
    }
    mAnimC->Draw(0, 0x80, 0x60, mUnk_f2);
}

void BattleStatusPanels::func_ov007_02235fe4() {
    mBufferSize = TextManager::Get().BufferSize(0x10, 2);
    u8 i        = 0;
    do {
        mPanels[i].mBuffer = OS_Alloc(mBufferSize);
        MI_CpuClear16(mPanels[i].mBuffer, mBufferSize);
        i++;
    } while (i < 4);
    mGlyphCount = TextManager::Get().GlyphCount(0x10, 2, 0x21);
    i           = 0;
    do {
        mPanels[i].mBuffer2 = OS_Alloc(mGlyphCount * 0xc);
        TextManager::Get().Init(mPanels[i].mBuffer2, 0, 0, 0x10, 2, 0x21);
        i++;
    } while (i < 4);
    TextManager::Get().SetColor(0, 0, 0xf);
}

void BattleStatusPanels::func_ov007_0223619c(u32 index) {
    if (mPanels[index].mBuffer) {
        MI_CpuClear16(mPanels[index].mBuffer, mBufferSize);
    }
}

void BattleStatusPanels::func_ov007_022361c4(u32 index) {
    s16 height;
    s16 width;
    height                 = 0;
    width                  = 0;
    mPanels[index].mUnk_34 = 0;
    func_ov007_0223619c(index);
    TextManager::Get().mUnk_5b = 1;
    TextManager::Get().Print(data_020d433c.mBattlePlayers->mPlayers[index]->NameText(),
                             data_020d433c.mBattlePlayers->mPlayers[index]->mUnk_ed, mPanels[index].mBuffer, 0x10, 2,
                             &height, &width, &mPanels[index].mUnk_34, 0, 2, 0x21, 0, 0, 0, 0, 1, 1);
    TextManager::Get().mUnk_5b = 0;
    switch (mMode) {
        case 0:
            func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, mCharBase + 0x2000 + (index << 11),
                          mPanels[index].mBuffer, mBufferSize, 0);
            break;
        case 1:
            func_02025c1c(SpriteManager::Get().Screen(0).mBgIndex, 4, mCharBase + 0x2000, mPanels[index].mBuffer,
                          mBufferSize, 0);
            break;
    }
}

u8 BattleStatusPanels::func_ov007_02236458(u32 index) {
    int level = data_020d433c.mBattlePlayers->mPlayers[index]->mUnk_f3 / 10;
    if (level > 0x13) {
        level = 0x13;
    }
    return level;
}
