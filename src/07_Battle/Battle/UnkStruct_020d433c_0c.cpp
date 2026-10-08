#include "Battle/Gimmick.hpp"
#include "Battle/BattleTarget.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/GimmickTypes.hpp"

void *operator new[](size_t size);
void operator delete[](void *p);
void func_ov007_02271b24(GimmickSetData *set, int count);

struct ObstacleRaw {
    u32 w[8];
};

UnkStruct_020d433c_0c::UnkStruct_020d433c_0c()
    : SceneBase(0x3e8, 0) {
    mUnk_0c = (GimmickSetData *) new u8[0x41c00];
    mUnk_0c->mUnk_822 = 0;
    mUnk_10[0]        = 0;
    for (u32 i = 0; i < 0x80; i++) {
        mUnk_0c[i].mUnk_822 = i;
        mUnk_10[i]          = &mUnk_0c[i];
    }
    mUnk_210       = 0;
    mUnk_211       = 1;
    mUnk_0c->mPrev = 0;
    mUnk_0c->mNext = 0;
    mUnk_0c->mUnk_08 = 0;
    mUnk_212       = 0;
}

UnkStruct_020d433c_0c::~UnkStruct_020d433c_0c() {
    delete[] mUnk_0c;
}

// non-matching: 2 instructions: the two halfword loads from buf and the strh into +0c/+0e come out as (r1=[2], r0=[0]; strh +e, +c) or loads swapped, never the original (r1=[0], r0=[2]; strh +c, +e)
GimmickSetData *func_ov007_02271ca8(UnkStruct_020d433c_0c *data, int count, void *buf, VecFx32 *pos, u16 *rot, u8 unk) {
    GimmickSetData *set = data->mUnk_10[data->mUnk_211];
    data->mUnk_211      = (data->mUnk_211 + 1) & 0x7f;
    set->mNext          = data->mUnk_0c->mNext;
    set->mPrev          = data->mUnk_0c;
    data->mUnk_0c->mNext = set;
    if (set->mNext) {
        set->mNext->mPrev = set;
    }
    func_ov007_02271b24(set, count);
    u16 hi       = ((u16 *) buf)[1];
    u16 lo       = ((u16 *) buf)[0];
    set->mUnk_0e = hi;
    set->mUnk_0c = lo;
    set->mUnk_10 = *(GimmickSetBlock *) ((u8 *) buf + 4);
    set->mUnk_810 = *pos;
    set->mRotX    = rot[0];
    set->mRotY    = rot[1];
    set->mRotZ    = rot[2];
    set->mUnk_823 = 0;
    set->mUnk_834 = unk;
    return set;
}

void func_ov007_02271d78(UnkStruct_020d433c_0c *data, GimmickSetData *set) {
    set->mPrev->mNext = set->mNext;
    if (set->mNext) {
        set->mNext->mPrev = set->mPrev;
    }
    data->mUnk_10[data->mUnk_210] = set;
    data->mUnk_210 = (data->mUnk_210 + 1) & 0x7f;
}

void UnkStruct_020d433c_0c::Update() {
    if (data_020d433c.mUnk_14 & 1) {
        mFinished = 1;
        return;
    }
    GimmickSetData *n = mUnk_0c;
    do {
        n->mUnk_823 = 0;
        n = n->mNext;
    } while (n);
    if (!mUnk_212) {
        return;
    }
    GimmickSetData *b;
    GimmickSetData *a = mUnk_0c;
    do {
        if (a->mUnk_08 != 0) {
            for (b = a->mNext; b; b = b->mNext) {
                if (b->mUnk_08 != 0) {
                    func_ov007_02271e48(a, b);
                }
            }
        }
        a = a->mNext;
    } while (a);
}

BOOL func_ov007_0227226c(BattleObstacle *a, BattleObstacle *b);
BOOL func_ov007_02272308(BattleObstacle *a, BattleObstacle *b);
BOOL func_ov007_02272170(BattleObstacle *a, BattleObstacle *b);
BOOL func_ov007_0227212c(BattleObstacle *a, BattleObstacle *b);
BOOL func_ov007_022720ac(BattleObstacle *a, BattleObstacle *b);

// Tests indexed by the type of the second object minus one (the first one is a ball); the last two entries are empty.
static BOOL (*sTests[6])(BattleObstacle *, BattleObstacle *) = {
    func_ov007_02272308, func_ov007_02272170, func_ov007_0227212c, func_ov007_022720ac, 0, 0,
};

// non-matching: same instructions and size (0x1f4) but the registers differ (a/b in r6/r5 instead of r7/r6, j r4 instead of r5, fileB fp instead of r4), the zero for i/j is not shared and hoisted before the loops, and the spill slots of i/fileA/entry differ (~87 instructions)
void UnkStruct_020d433c_0c::func_ov007_02271e48(GimmickSetData *a, GimmickSetData *b) {
    BattleObstacle shapeA;
    BattleObstacle shapeB;
    u8 i;
    u8 j;
    if (!((a->mUnk_0a & b->mUnk_08) | (b->mUnk_0a & a->mUnk_08))) {
        return;
    }
    GimmickSetFile *fileA = (GimmickSetFile *) &a->mUnk_0c;
    GimmickSetFile *fileB = (GimmickSetFile *) &b->mUnk_0c;
    for (i = 0; i < fileA->mCount; i++) {
        for (j = 0; j < fileB->mCount; j++) {
            *(ObstacleRaw *) &shapeA = *(ObstacleRaw *) ((u8 *) fileA + i * 0x20 + 4);
            *(ObstacleRaw *) &shapeB = *(ObstacleRaw *) ((u8 *) fileB + j * 0x20 + 4);
            VEC_Add(&shapeA.mPos, &a->mUnk_810, &shapeA.mPos);
            shapeA.mRotX += a->mRotX;
            shapeA.mRotY += a->mRotY;
            shapeA.mRotZ += a->mRotZ;
            VEC_Add(&shapeB.mPos, &b->mUnk_810, &shapeB.mPos);
            shapeB.mRotX += b->mRotX;
            shapeB.mRotY += b->mRotY;
            shapeB.mRotZ += b->mRotZ;
            if (!func_ov007_0227203c(&shapeA, &shapeB)) {
                continue;
            }
            if (a->mUnk_823 < 8) {
                a->mUnk_824[a->mUnk_823] = b->mUnk_822;
                a->mUnk_82c[a->mUnk_823] = j;
                a->mUnk_823++;
            }
            if (b->mUnk_823 < 8) {
                b->mUnk_824[b->mUnk_823] = a->mUnk_822;
                b->mUnk_82c[b->mUnk_823] = j;
                b->mUnk_823++;
            }
        }
    }
}



BOOL UnkStruct_020d433c_0c::func_ov007_0227203c(BattleObstacle *a, BattleObstacle *b) {
    BOOL result = 0;
    u8 type     = a->mType;
    if (type == 0) {
        if (b->mType == 0) {
            result = func_ov007_0227226c(a, b);
        }
    } else if (type == 1) {
        type = b->mType;
        if (type == 0) {
            result = func_ov007_0227226c(b, a);
        } else {
            result = sTests[type - 1](a, b);
        }
    }
    return result;
}
