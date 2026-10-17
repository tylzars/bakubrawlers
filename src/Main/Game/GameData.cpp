#include "System/SoundManager.hpp"
#include "System/WirelessManager.hpp"
#include "Game/SaveData.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d48f0.hpp"
#include "System/FrameTask.hpp"
#include "System/SoundManager.hpp"
#include "Unknown/UnkFunctions.hpp"
#include "System/WirelessManager.hpp"

#include <nitro/mi.h>

// Volume tables (0x020906f0-0x02090778), indexed by the 3-bit settings in gSaveData.
extern const u8 gSeqVolumes[6] = {0, 20, 40, 60, 85, 110};
extern const u8 gSeqArcVolumes[6] = {0, 25, 50, 75, 100, 127};
extern const float gFrameTaskVolumes1[6] = {0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f};
extern const float gFrameTaskVolumes0[6] = {0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 0.9f};
extern const float gFrameTaskVolumes2[6] = {0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f};
extern const u16 data_02090754[3][6] = {
    {0x0000, 0x0016, 0x0000, 0x00be, 0x0013, 0x007e},
    {0x0000, 0x0017, 0x0005, 0x00d2, 0x0028, 0x008c},
    {0x0000, 0x0013, 0x0001, 0x00c8, 0x0048, 0x00ab},
};
// npdata keeps the code addressing these through their own symbols (data_020d433c + 0x7a, not .bss + 0x9e).
#pragma section npdata begin
UnkStruct_020d433c data_020d433c;
SaveData gSaveData[1];
#pragma section npdata end
extern u16 data_020c4250;
int func_0205b1c4(u32 a, u32 b);

void ResetOptions(SaveData *d);
void ApplyVolumeOptions(SaveData *d);

void func_02022b0c() {
    MI_CpuFill8(&data_020d433c, 0, 0xb4);
}

void ResetSaveData() {
    MI_CpuFill8(gSaveData, 0, 0xff4);
    ResetOptions(gSaveData);
    gSaveData[0].mMoney = 3000;
    UnlockStoreItems();
    UnlockCharacters();
    UnlockFields();
    // data_020d48f0 lies inside gSaveData (+0x500); going through the member keeps the stores ordered before
    // the bit-field updates below. The `mUnk_460 + 0xa0` cast is a placeholder for the real member at +0x500.
    ((UnkStruct_020d48f0 *) (gSaveData[0].mUnk_460 + 0xa0))->mPrevRank = 999;
    ((UnkStruct_020d48f0 *) (gSaveData[0].mUnk_460 + 0xa0))->mRank = 999;
    u8 i = 0;
    gSaveData[0].mUnk_1a_0 = 1;
    gSaveData[0].mUnk_14_1 = 0;
    gSaveData[0].mUnk_14_4 = 0;
    gSaveData[0].mUnk_14_7 = 0;
    gSaveData[0].mUnk_14_10 = 0;
    gSaveData[0].mUnk_16_12 = 0;
    gSaveData[0].mUnk_18_0 = 2;
    gSaveData[0].mUnk_18_3 = 0;
    gSaveData[0].mUnk_18_8 = 2;
    gSaveData[0].mUnk_18_14 = 0;
    gSaveData[0].mUnk_18_11 = 1;
    gSaveData[0].mUnk_27_5 = 0;
    gSaveData[0].mRules |= 0x1c;
    data_020d433c.mRules = gSaveData[0].mRules;
    do {
        SetSlotEntryIndex(gSaveData, 0, i, 0xffff);
        i++;
    } while (i < 3);
    u8 n = 0;
    const u16 (*tab)[6] = data_02090754;
    SaveData *base = gSaveData;
    do {
        const u16 *rec = tab[n];
        int idx = GetBakuganEntryIndex(base, tab[n][0] & 0xff, rec[1] & 0xff, rec[2] & 0xff);
        SetSlotEntryIndex(base, 0, n, idx);
        func_020231c0(base, idx, rec[3]);
        func_0205b1c4(rec[1] & 0xff, rec[2] & 0xff);
        base->mSlots54Values[n] = rec[4];
        SetFlagBit((u8 *) gSaveData[0].mUnk_556, rec[4]);
        base->mSlots6cValues[n] = rec[5];
        SetFlagBit((u8 *) gSaveData[0].mUnk_556, rec[5]);
        n++;
    } while (n < 3);
}

void ResetOptions(SaveData *d) {
    SaveData *p = gSaveData;
    p->mUnk_28_5 = 1;
    p->mMicEnabled = 1;
    p->mVolume0Index = 5;
    p->mVolume1Index = 5;
    p->mVolume2Index = 5;
    p->mUnk_27_0 = p->mMicEnabled;
    ApplyVolumeOptions(d);
}

void ApplyVolumeOptions(SaveData *d) {
    SoundManager::Get().mSeqVolume = gSeqVolumes[d->mVolume0Index];
    SoundManager::Get().mSeqArcVolume = gSeqArcVolumes[d->mVolume1Index];
    FrameTask::Get().mVolume0 = gFrameTaskVolumes0[d->mVolume0Index];
    FrameTask::Get().mVolume1 = gFrameTaskVolumes1[d->mVolume1Index];
    FrameTask::Get().mVolume2 = gFrameTaskVolumes2[d->mVolume2Index];
    SoundManager::Get().SetSeqVolume(0x7f);
    SoundManager::Get().SetSeqArcVolume(0x7f);
    FrameTask::Get().SetChannelVolume(0, 4);
    FrameTask::Get().SetChannelVolume(1, 4);
}

s32 GetSlotBakuganAttribute(SaveData *d, u32 a, u32 b) {
    u16 *rec = (u16 *) ((u8 *) d + a * 0x14);
    return (u8) (rec[b] % 6);
}

u8 GetSlotBakuganId(SaveData *d, u32 a, u32 b) {
    u16 *rec = (u16 *) ((u8 *) d + a * 0x14);
    u32 v = rec[b];
    if (v >= 0xf6) {
        return 0xff;
    }
    u16 q = v / 6;
    return q % 41;
}

void SetSlotBakugan(SaveData *d, u32 a, u32 b, u32 c, u32 e, u32 f) {
    u16 *rec = (u16 *) ((u8 *) d + a * 0x14);
    rec[b] = GetBakuganEntryIndex(d, c, *(u8 *) &e, *(u8 *) &f);
}

void SetSlotEntryIndex(SaveData *d, u32 a, u32 b, int c) {
    u16 *rec = (u16 *) ((u8 *) d + a * 0x14);
    rec[b] = c;
}

int GetBakuganEntryIndex(SaveData *d, u32 a, u32 b, u32 c) {
    if (a >= 1) {
        return 0;
    }
    if (b >= 0x29) {
        return 0;
    }
    if (c >= 6) {
        return 0;
    }
    return (u16) (c + (b * 6 + a * 0xf6));
}

void func_020231c0(SaveData *d, int idx, u32 v) {
    if (v >= 10) {
        v = (u16) ((int) v / 10);
    }
    d->mUnk_460[idx] = v;
}

u16 func_020231f4(SaveData *d, u32 a, u8 b, u8 c) {
    int idx = GetBakuganEntryIndex(d, a, b, c);
    return d->mUnk_460[idx] * 10;
}

void SetBakuganEntry(SaveData *d, const u16 *src, u32 b, u32 c) {
    int idx = GetBakuganEntryIndex(d, 0, b, c);
    d->mBakuganEntries[idx][0] = src[0];
    d->mBakuganEntries[idx][1] = src[1];
}

UnkStruct_02023248 *GetBakuganEntry(SaveData *d, u32 a, u32 b) {
    int idx = GetBakuganEntryIndex(d, 0, a, b);
    return (UnkStruct_02023248 *) &d->mBakuganEntries[idx];
}

BOOL func_0202326c(SaveData *d) {
    BOOL found = FALSE;
    u8 pairs[6][2] = {{1, 2}, {3, 3}, {6, 2}, {7, 4}, {8, 3}, {9, 6}};
    u8 i = 0;
    u8 flag = data_020d433c.mUnk_7a;
    do {
        if (flag && d->mUnk_42 == pairs[i][0] && d->mUnk_43 == pairs[i][1]) {
            found = TRUE;
        }
        i++;
    } while (i < 6);
    return found;
}

void func_0202330c(UnkStruct_020d433c *d) {
    MI_CpuFill8(d->mUnk_af, 0, 4);
    u8 i;
    u8 n = 0;
    i = 0;
    do {
        WirelessManager::Get();
        if (i == data_020c4250) {
            d->mUnk_ae = n;
        }
        if (WirelessManager::Get().func_0202f0cc(i)) {
            d->mUnk_af[n++] = i;
        }
        i++;
    } while (i < 4);
}

void func_020233f8(UnkStruct_020d433c *d) {
    u8 n = 0;
    u8 i = 0;
    do {
        WirelessManager::Get();
        if (i == data_020c4250) {
            d->mUnk_ae = n;
        }
        if (WirelessManager::Get().func_0202f0cc(i)) {
            n++;
        }
        i++;
    } while (i < 4);
}
