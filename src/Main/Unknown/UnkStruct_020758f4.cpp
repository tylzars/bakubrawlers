#include "Unknown/UnkStruct_020758f4.hpp"

#include "Game/UnkStruct_020d43f0.hpp"

#include <nitro/fx.h>

extern const u8 data_020a3070[][13];
extern const u8 data_020a305c[4];
extern const u32 data_020a3060[4];
extern const u16 data_020b94fc[2][12];
extern const u16 data_020b952c[2][12];
extern const u16 data_020b955c[2][12];
extern const u32 data_020b958c[2][12];
extern const u32 data_020b95ec[2][12];

UnkStruct_020758f4::UnkStruct_020758f4() {
    mUnk_8c = 0x15;
    mUnk_8e = 0x3e7;
    mUnk_90 = data_020d43f0[0].mUnk_20;
    mUnk_94 = 0;
    u32 flag = 0;
    u8 mode = data_020d43f0[0].mUnk_42;
    if (mode == 6) {
        if (data_020d43f0[0].mUnk_43 > 2) {
            flag = 1;
        }
    }
    if (((mode > 6) | flag | data_020d43f0[0].mUnk_5bf_0) != 0) {
        mUnk_94 = 1;
    }
    func_02075a0c();
}

UnkStruct_020758f4::~UnkStruct_020758f4() {
}

UnkStruct_020758f4_Record *UnkStruct_020758f4::func_02075998(u32 index) {
    u8 i = 0;
    do {
        if (index == mUnk_04[i].mUnk_0) {
            break;
        }
        i++;
    } while (i < 13);
    if (i >= 13) {
        i = 0;
        do {
            if (index == mUnk_6c[i].mUnk_0) {
                break;
            }
            i++;
        } while (i < 4);
        return (UnkStruct_020758f4_Record *) &mUnk_6c[i];
    }
    return (UnkStruct_020758f4_Record *) &mUnk_04[i];
}

UnkStruct_020758f4_Entry *UnkStruct_020758f4::func_02075a00(u32 index) {
    return &mUnk_04[index];
}

// non-matching: 0x438 vs 0x434. The original keeps `this` in r7 (loop counter r6, zero r4, table r5) where ours puts it in r4,
// and it adds the 9000 of the score > 1000000 case to a register holding 10000 (ours loads the folded 0x4a38).
void UnkStruct_020758f4::func_02075a0c() {
    s8 i;
    s8 j;
    s8 k;
    for (i = 0; i < 13; i++) {
        u8 id = data_020a3070[mUnk_94][i];
        mUnk_04[i].mUnk_0 = id;
        if (id == 0x15) {
            mUnk_04[i].mUnk_4 = 0;
        } else {
            mUnk_04[i].mUnk_4 = func_02075e40((u8) i);
        }
    }
    for (i = 1; i < 13; i++) {
        j = i;
        k = i - 1;
        do {
            u32 scoreA = mUnk_04[k].mUnk_4;
            u32 scoreB = mUnk_04[j].mUnk_4;
            if (scoreB > scoreA) {
                mUnk_04[k].mUnk_4 = scoreB;
                mUnk_04[j].mUnk_4 = scoreA;
                u8 idA = mUnk_04[k].mUnk_0;
                u8 idB = mUnk_04[j].mUnk_0;
                mUnk_04[k].mUnk_0 = idB;
                mUnk_04[j].mUnk_0 = idA;
                j = k;
            }
            k--;
        } while (k >= 0);
    }
    u8 rank = 1;
    mUnk_04[0].mUnk_2 = 1;
    for (i = 1; i < 13; i++) {
        if (mUnk_04[i].mUnk_4 < mUnk_04[i - 1].mUnk_4) {
            rank = i + 1;
        }
        mUnk_04[i].mUnk_2 = rank;
    }
    for (i = 0; (u32) i < 10; i++) {
        if (mUnk_90 >= mUnk_04[i].mUnk_4) {
            s8 j = 12;
            do {
                mUnk_04[j].mUnk_2 = mUnk_04[j - 1].mUnk_2;
                if (mUnk_90 > mUnk_04[j - 1].mUnk_4) {
                    mUnk_04[j].mUnk_2++;
                }
                mUnk_04[j].mUnk_0 = mUnk_04[j - 1].mUnk_0;
                mUnk_04[j].mUnk_4 = mUnk_04[j - 1].mUnk_4;
                j--;
            } while (i < j);
            mUnk_04[i].mUnk_4 = mUnk_90;
            mUnk_04[i].mUnk_0 = mUnk_8c;
            mUnk_8e = mUnk_04[i].mUnk_2;
            break;
        }
    }
    if (mUnk_8e > 10) {
        if (mUnk_90 > 0x30bb0) {
            mUnk_8e = 11;
        } else {
            mUnk_8e = func_02075ff4(mUnk_90);
        }
    }
    u32 base;
    u32 score = mUnk_90;
    if (score <= 100000) {
        base = score * 10 / 100;
    } else {
        base = 10000;
        if (score > 1000000) {
            base += 9000;
        } else {
            base += (score - 100000) / 100;
        }
    }
    for (i = 0; i < 4; i++) {
        u32 value = (base + data_020a3060[i]) / 10 * 10;
        mUnk_6c[i].mUnk_0 = data_020a305c[i];
        mUnk_6c[i].mUnk_4 = value;
        mUnk_6c[i].mUnk_2 = func_02075ff4(value);
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i != j && mUnk_6c[i].mUnk_2 == mUnk_6c[j].mUnk_2) {
                if (mUnk_6c[i].mUnk_4 <= mUnk_6c[j].mUnk_4) {
                    mUnk_6c[j].mUnk_2--;
                } else {
                    mUnk_6c[i].mUnk_2--;
                }
            }
        }
    }
    if (mUnk_8e > 10) {
        for (k = 12; k > 10; k--) {
            if (mUnk_04[k - 1].mUnk_4 != 0) {
                if (mUnk_04[k - 1].mUnk_4 <= mUnk_90) {
                    mUnk_8e = mUnk_04[k - 1].mUnk_2;
                    mUnk_04[k].mUnk_0 = mUnk_04[k - 1].mUnk_0;
                    mUnk_04[k].mUnk_1 = mUnk_04[k - 1].mUnk_1;
                    mUnk_04[k].mUnk_2 = mUnk_04[k - 1].mUnk_2;
                    mUnk_04[k].mUnk_4 = mUnk_04[k - 1].mUnk_4;
                    if (mUnk_04[k - 1].mUnk_4 < mUnk_90) {
                        mUnk_04[k].mUnk_2++;
                    }
                    mUnk_04[k - 1].mUnk_0 = mUnk_8c;
                    mUnk_04[k - 1].mUnk_1 = mUnk_8d;
                    mUnk_04[k - 1].mUnk_2 = mUnk_8e;
                    mUnk_04[k - 1].mUnk_4 = mUnk_90;
                } else {
                    u16 prevRank = mUnk_04[k - 1].mUnk_2;
                    if (prevRank >= mUnk_8e) {
                        mUnk_8e = prevRank + 1;
                    }
                    break;
                }
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (mUnk_8e == mUnk_6c[i].mUnk_2) {
            if (mUnk_6c[i].mUnk_4 > mUnk_90) {
                mUnk_6c[i].mUnk_2--;
            } else {
                mUnk_6c[i].mUnk_2++;
            }
        }
    }
}

u32 UnkStruct_020758f4::func_02075e40(u8 index) {
    u32 base = mUnk_90 / 2000 * 2000;
    u8 mode = mUnk_94;
    s32 x = (base + data_020b955c[mode][index]) * data_020b952c[mode][index];
    double angle = (double) x - (double) (u32) ((double) x / 6.283185308) * 6.283185308;
    s32 step = (u32) (angle * 10430.378349108529);
    s32 sinv = FX_SinCosTable_[(step >> 4) * 2];
    u32 adj;
    if (sinv >= 0) {
        adj = (sinv * (data_020b95ec[mode][index] / 100) >> 12) * 100;
    } else {
        adj = -((-sinv * (data_020b95ec[mode][index] / 100)) >> 12) * 100;
    }
    u32 sub = base * data_020b94fc[mode][index];
    return (base + data_020b958c[mode][index] + adj - sub / 100) / 10 * 10;
}

u16 UnkStruct_020758f4::func_02075ff4(u32 score) {
    u32 t = (0x30bb0 - score) / 1000;
    u32 cube = t * t * t;
    u16 rank = cube / 7968 + 11;
    if (rank <= 10) {
        rank = 11;
    }
    if (rank > 999) {
        rank = 999;
    }
    return rank;
}
