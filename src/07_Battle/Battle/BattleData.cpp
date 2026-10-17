#include "Battle/BattleBase.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/FileSystem.hpp"
#include "System/SoundManager.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>
#include <stdio.h>

u8 GetCardAttributeValue(u32 id, u32 index);

BattleData::BattleData() {
    mMarkerCardId = 0;
    mMarkerPlayer = 0;
    mUnk_07 = 0;
    mUnk_0b = 0;
    mMessageWindow = 0;
    mCharacterIcons = 0;
    u8 index0         = data_020d433c.mBattlePlayers->mUnitA;
    u8 index1         = data_020d433c.mBattlePlayers->mUnitB;
    BattleUnit *unit0 = &data_020d433c.mBattlePlayers->mUnits[index0];
    BattleUnit *unit1 = &data_020d433c.mBattlePlayers->mUnits[index1];
    mEntries[0].mUnk_0d_4 = unit0->mPlayer;
    mEntries[1].mUnk_0d_4 = unit1->mPlayer;
    BattlePlayer *player0 = data_020d433c.mBattlePlayers->mPlayers[mEntries[0].mUnk_0d_4];
    BattlePlayer *player1 = data_020d433c.mBattlePlayers->mPlayers[mEntries[1].mUnk_0d_4];
    BattleRecord *rec0    = &player0->mRecords[unit0->mRecord];
    BattleRecord *rec1    = &player1->mRecords[unit1->mRecord];
    mEntries[0].mBakuganId   = rec0->mBakuganId;
    mEntries[1].mBakuganId   = rec1->mBakuganId;
    mEntries[0].mUnk_0d_0 = rec0->mAttribute;
    mEntries[1].mUnk_0d_0 = rec1->mAttribute;
    mEntries[0].mUnk_00   = rec0->mUnk_04 + rec0->mUnk_06;
    mEntries[1].mUnk_00   = rec1->mUnk_04 + rec1->mUnk_06;
    mEntries[0].mUnk_04   = mEntries[0].mUnk_00;
    mEntries[1].mUnk_04   = mEntries[1].mUnk_00;
    mEntries[0].mUnk_06   = 0;
    mEntries[1].mUnk_06   = 0;
    mEntries[0].mUnk_0a   = 0;
    mEntries[1].mUnk_0a   = 0;
    mEntries[0].mCharacterId   = data_020d433c.mBattlePlayers->mPlayers[mEntries[0].mUnk_0d_4]->mCharacterId;
    mEntries[1].mCharacterId   = data_020d433c.mBattlePlayers->mPlayers[mEntries[1].mUnk_0d_4]->mCharacterId;
    mEntries[0].mUnk_10   = 0;
    mEntries[1].mUnk_10   = 0;
    mEntries[0].mSlot6cIndex   = 0;
    mEntries[1].mSlot6cIndex   = 0;
    mEntries[0].mCardId   = 0;
    mEntries[1].mCardId   = 0;
    BattleMarker *marker = &data_020d433c.mBattlePlayers->mMarkers[data_020d433c.mBattlePlayers->mUnits[index1].mMarker];
    u8 slot  = marker->mSlot;
    u8 owner = marker->mPlayer;
    mMarkerPlayer  = owner;
    mMarkerCardId  = data_020d433c.mBattlePlayers->mPlayers[owner]->Slot54(slot);
    u8 i = 0;
    do {
        if (mEntries[i].mBakuganId == 5) {
            u32 a = GetCardAttributeValue(mMarkerCardId, 1);
            u32 b = GetCardAttributeValue(mMarkerCardId, 3);
            if (a > b) {
                mEntries[i].mUnk_06 = a * 10;
            } else {
                mEntries[i].mUnk_06 = b * 10;
            }
        } else if (mEntries[i].mBakuganId == 6) {
            u32 a = GetCardAttributeValue(mMarkerCardId, 1);
            u32 b = GetCardAttributeValue(mMarkerCardId, 0);
            if (a > b) {
                mEntries[i].mUnk_06 = a * 10;
            } else {
                mEntries[i].mUnk_06 = b * 10;
            }
        } else {
            mEntries[i].mUnk_06 = GetCardAttributeValue(mMarkerCardId, mEntries[i].mUnk_0d_0) * 10;
        }
        mEntries[i].mUnk_02 = mEntries[i].mUnk_00 + mEntries[i].mUnk_06;
        i++;
    } while (i < 2);
    u8 j = 0;
    do {
        mEntries[j].mUnk_08   = 0;
        mEntries[j].mUnk_0f_0 = 0;
        mEntries[j].mUnk_0f_1 = 0;
        mEntries[j].mUnk_0f_2 = 0;
        mEntries[j].mUnk_0f_3 = 0;
        mEntries[j].mUnk_0f_4 = 0;
        mEntries[j].mUnk_0f_5 = 0;
        mEntries[j].mUnk_0f_6 = 0;
        j++;
    } while (j < 2);
    mUnk_08[0] = data_020d433c.mBattlePlayers->mPlayers[mEntries[0].mUnk_0d_4]->mUnk_ee;
    mUnk_08[1] = data_020d433c.mBattlePlayers->mPlayers[mEntries[1].mUnk_0d_4]->mUnk_ee;
    if (data_020d433c.mHasPartners == 0) {
        mUnk_0a = 3;
    } else {
        mUnk_0a = 3;
        mUnk_08[0] += data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mPlayers[mEntries[0].mUnk_0d_4]->mPartner]->mUnk_ee;
        mUnk_08[1] += data_020d433c.mBattlePlayers->mPlayers[data_020d433c.mBattlePlayers->mPlayers[mEntries[1].mUnk_0d_4]->mPartner]->mUnk_ee;
    }
    MI_CpuClearFast(mCellAnims, 0x38);
}

BattleData::~BattleData() {
    u8 i = 0;
    do {
        CellAnimation *a = mCellAnims[i];
        if (a) {
            delete a;
        }
        i++;
    } while (i < 14);
    if (mMessageWindow) {
        delete mMessageWindow;
    }
    if (mCharacterIcons) {
        delete mCharacterIcons;
    }
}

extern const u8 gBakuganNumbers[];

void BattleData::Load() {
    char path[0x80];
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.SetObjVramMode(3);
    ChangeDir("/Game/Bakugan/Card/");
    u8 i = 0;
    do {
        u8 id = gBakuganNumbers[mEntries[i].mBakuganId];
        u32 size;
        sprintf(path, "bakugan_l_%03d.nce_ncg1D.bin", id);
        screen.LoadCharacters(path, i << 13, 0);
        sprintf(path, "bakugan_l_%03d_ncl.bin", id);
        u8 *palette = (u8 *) LoadFile(path, 0, 0, 0, &size, 0);
        QueueUpload(1, UPLOAD_KIND_OBJ_EXT_PLTT, i << 9, palette + (mEntries[i].mUnk_0d_0 << 9), 0x200, 0);
        OS_Free(palette);
        sprintf(path, "bakugan_l_%03d_nce.bin", id);
        mCellAnims[i] = new CellAnimation(path, 0, 1, 1);
        mCellAnims[i]->mCharBase = screen.GetCharBase(i << 13);
        mCellAnims[i]->mPaletteBase   = i;
        mCellAnims[i]->mFrameIndex   = 0;
        mCellAnims[i + 5] = new CellAnimation("bakugan_l_attribute_nce.bin", 0, 1, 1);
        mCellAnims[i + 5]->mCharBase = screen.GetCharBase(0x1800);
        mCellAnims[i + 5]->mPaletteBase   = 0;
        mCellAnims[i + 5]->mFrameIndex   = mEntries[i].mUnk_0d_0;
        i++;
    } while (i < 2);
    screen.LoadCharacters("bakugan_l_attribute.nce_ncg1D.bin", 0x1800, 0);
    ChangeDir("/Game/Confrontation/Obj");
    sprintf(path, "vs_b_obj_num.nce_ncg1D.bin");
    screen.LoadCharacters(path, 0x4000, 0);
    sprintf(path, "vs_b_obj_num_ncl.bin");
    screen.LoadPalette(path, 0, 0);
    for (u8 k = 2; k < 5; k++) {
        mCellAnims[k] = new CellAnimation("vs_b_obj_num_nce.bin", 0, 1, 1);
        mCellAnims[k]->mCharBase = screen.GetCharBase(0x4000);
        mCellAnims[k]->mPaletteBase   = 0;
    }
    mCellAnims[2]->SetAnimation(0);
    mCellAnims[3]->SetAnimation(1);
    mCellAnims[4]->SetAnimation(2);
    ChangeDir("/Game/Character/Icon");
    sprintf(path, "icon_ch_base.nce_ncg1D.bin");
    screen.LoadCharacters(path, 0x5000, 0);
    sprintf(path, "icon_ch_base_ncl.bin");
    screen.LoadPalette(path, 0x20, 0);
    for (u8 m = 7; m < 14; m++) {
        mCellAnims[m] = new CellAnimation("icon_ch_base_nce.bin", 0, 1, 1);
        mCellAnims[m]->mCharBase = screen.GetCharBase(0x5000);
        mCellAnims[m]->mPaletteBase   = 1;
    }
    mCellAnims[7]->SetAnimation(mEntries[0].mUnk_0d_4 + 7);
    mCellAnims[8]->SetAnimation(mEntries[1].mUnk_0d_4 + 7);
    mCellAnims[9]->SetAnimation(0);
    mCellAnims[10]->SetAnimation(1);
    u8 current = data_020d433c.mBattlePlayers->mUnk_2a9;
    u8 n       = 0;
    do {
        if (current == mEntries[n].mUnk_0d_4) {
            mCellAnims[11]->SetAnimation(mEntries[n].mUnk_0d_4 + 2);
            break;
        }
        n++;
    } while (n < 2);
    mCellAnims[12]->SetAnimation(0xb);
    mCellAnims[13]->SetAnimation(6);
    mCharacterIcons = new CharacterIcons(0x6800, 3, 1);
    mCharacterIcons->SetCharacter(0, *(s8 *) &mEntries[0].mCharacterId, data_020d433c.mBattlePlayers->mPlayers[mEntries[0].mUnk_0d_4]->NameInfo(), 0);
    mCharacterIcons->SetCharacter(1, *(s8 *) &mEntries[1].mCharacterId, data_020d433c.mBattlePlayers->mPlayers[mEntries[1].mUnk_0d_4]->NameInfo(), 0);
    ChangeDir("/");
    mMessageWindow = new MessageWindow("mes_CardName", 1, 0x1f800);
    mMessageWindow->func_02065bd4(0x14, 4, 0);
    mMessageWindow->SetWidth(0xd8);
}

// The second px (the icons' x) reuses the first one's variable, and `iy` is assigned in two steps: both give the
// original registers. The `frame` local gives the loop's `mov r2, #0; movcs r2, #1` (passing `k >= mUnk_08[side]`
// gives movcs/movcc).
void BattleData::DrawCharacterIcons(u8 side, s16 x, s16 y) {
    int offset;
    int top;
    s16 px;
    s16 iy;
    offset = side == 0 ? 0 : 0x24;
    px     = x + offset;
    top    = y - 4;
    mCellAnims[side + 7]->Draw(1, (s16) (px - 6), (s16) top, 2);
    if (data_020d433c.mBattlePlayers->mPlayers[mEntries[side].mUnk_0d_4]->IsCpu()) {
        mCellAnims[12]->DrawFrame(1, mEntries[side].mUnk_0d_4, px - 6, top, 2);
    }
    mCellAnims[9]->Draw(1, (s16) (px - 2), (s16) (y - 2), 3);
    mCharacterIcons->SetDrawArgs(side, px, y, 2);
    BattlePlayerList *list = data_020d433c.mBattlePlayers;
    u8 owner               = mEntries[side].mUnk_0d_4;
    u8 current             = list->mUnk_2a9;
    if (current == owner) {
        int shift = side == 0 ? 0x20 : 0;
        mCellAnims[11]->Draw(1, (s16) (x + shift), y, 3);
    } else if (data_020d433c.mHasPartners && current == list->mPlayers[owner]->mPartner) {
        int shift = side == 0 ? 0x20 : 0;
        mCellAnims[13]->DrawFrame(1, owner, x + shift, y, 3);
    }
    u8 k = 0;
    px   = x + (side == 0 ? 0x22 : 0x18);
    iy   = y;
    iy += 0x10;
    do {
        u8 frame = 0;
        if (k >= mUnk_08[side]) {
            frame = 1;
        }
        mCellAnims[10]->DrawFrame(1, frame, px, iy, 3);
        px = px + (side == 0 ? 6 : -6);
        k++;
    } while (k < mUnk_0a);
}


BOOL BattleData::func_ov007_0223dd6c() {
    BOOL result = TRUE;
    u8 i        = 0;
    do {
        if (!func_ov007_0223ddac(i)) {
            result = FALSE;
        }
        i++;
    } while (i < 2);
    return result;
}

BOOL BattleData::func_ov007_0223ddac(u8 side) {
    BOOL done = TRUE;
    if (mEntries[side].mUnk_00 < mEntries[side].mUnk_02) {
        mEntries[side].mUnk_00 += 3;
        if (mEntries[side].mUnk_00 >= mEntries[side].mUnk_02) {
            mEntries[side].mUnk_00 = mEntries[side].mUnk_02;
        } else {
            done = FALSE;
        }
    } else if (mEntries[side].mUnk_00 > mEntries[side].mUnk_02) {
        if ((s32) mEntries[side].mUnk_00 <= 3) {
            mEntries[side].mUnk_00 = mEntries[side].mUnk_02;
        } else {
            mEntries[side].mUnk_00 -= 3;
            done = FALSE;
        }
    }
    if (!done) {
        mUnk_0b++;
        if (mUnk_0b >= 10) {
            mUnk_0b = 0;
            SoundManager::Get().StartSeqArc(8);
        }
    }
    return done;
}

void BattleData::DrawBakuganAndNumber(u8 side, s16 x, s16 y, u8 flag, u16 value) {
    if (value == 0xffff) {
        value = mEntries[side].mUnk_00;
    }
    u8 count = 4;
    if (value < 1000) {
        count--;
        if (value < 100) {
            count--;
            if ((s32) value < 10) {
                count--;
            }
        }
    }
    u32 n = value;
    u8 i  = 0;
    do {
        mCellAnims[2]->DrawFrame(1, (s32) n % 10, x + 0x2f - i * 12, y + 0x4d, flag);
        n /= 10;
        i++;
    } while (i < count);
    mCellAnims[side + 5]->Draw(1, x, y, flag);
    mCellAnims[side]->Draw(1, x, y, flag);
}

void BattleData::DrawEntryNumber(u8 side, u8 kind, s32 x, s16 y, u8 flag, u32 mode, u16 a, u16 b) {
    int delta;
    s16 sdelta;
    int shift;
    u32 value = 0;
    switch (kind) {
        case 0:
            value = mEntries[side].mUnk_06;
            break;
        case 1:
            value = mEntries[side].mUnk_08;
            break;
        case 2:
            value = mEntries[side].mUnk_0a;
            break;
    }
    if (value == 0) {
        return;
    }
    u8 count = 3;
    if (value < 100) {
        count--;
        if ((s32) value < 10) {
            count--;
        }
    }
    delta    = a - (b * 2) / 3;
    int base = x + 0x30;
    u8 offset = 0x20;
    u8 i      = 0;
    do {
        if (mode == 0) {
            sdelta = delta;
            shift  = sdelta * sdelta / 8;
            offset = shift <= 0xff ? shift : 0xff;
            if (offset > 0x20) {
                if (sdelta >= 0) {
                    goto next;
                }
                offset = 0x20;
            }
        }
        s16 px = base - i * 12;
        s16 py = y + offset;
        if (i == count) {
            mCellAnims[4]->Draw(1, px, py, flag);
        } else {
            u8 digit = (s32) value % 10;
            value /= 10;
            mCellAnims[3]->DrawFrame(1, digit, px, py, flag);
        }
    next:
        i++;
    } while (i <= count);
}

void BattleData::func_ov007_0223e180() {
    u16 average         = (mEntries[0].mUnk_00 + mEntries[1].mUnk_00) / 2;
    mEntries[0].mUnk_00 = average;
    mEntries[1].mUnk_00 = average;
}

void BattleData::func_ov007_0223e1a4(u32 unk, u8 side) {
    mUnk_07 = unk;
    if (side == 0xff) {
        return;
    }
    mEntries[side].mUnk_0f_5 = 1;
}

void BattleData::func_ov007_0223e1cc(u8 side, u8 kind) {
    if (kind == 3) {
        mEntries[side].mUnk_0f_2 = 1;
    }
    mEntries[side].mUnk_0f_1 = 1;
    mEntries[side].mCardId   = 0;
}

void func_ov007_0223e214(BattleData *data, int side, u8 bit) {
    data->mEntries[side].mUnk_0f_3 = bit;
}
