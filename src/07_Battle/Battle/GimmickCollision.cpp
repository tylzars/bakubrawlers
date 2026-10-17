#include "Battle/Gimmick.hpp"
#include "Battle/BattleTarget.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/GimmickTypes.hpp"

void *operator new[](size_t size);
void operator delete[](void *p);
struct GimmickSetDataHead;
void SetCollisionMode(GimmickSetDataHead *data, int mode);

struct ObstacleRaw {
    u32 w[8];
};

GimmickCollision::GimmickCollision()
    : SceneBase(0x3e8, 0) {
    mSets = (GimmickSetData *) new u8[0x41c00];
    mSets->mSlot = 0;
    mSetRing[0]        = 0;
    for (u32 i = 0; i < 0x80; i++) {
        mSets[i].mSlot = i;
        mSetRing[i]          = &mSets[i];
    }
    mRingWriteIndex       = 0;
    mRingReadIndex       = 1;
    mSets->mPrev = 0;
    mSets->mNext = 0;
    mSets->mMode = 0;
    mEnabled       = 0;
}

GimmickCollision::~GimmickCollision() {
    delete[] mSets;
}

// The first 4 bytes of a set file, copied as one object.
struct GimmickSetHead {
    u16 mData[2];
};

GimmickSetData *AddCollisionSet(GimmickCollision *data, int count, void *buf, VecFx32 *pos, u16 *rot, u8 unk) {
    GimmickSetData *set = data->mSetRing[data->mRingReadIndex];
    data->mRingReadIndex      = (data->mRingReadIndex + 1) & 0x7f;
    set->mNext          = data->mSets->mNext;
    set->mPrev          = data->mSets;
    data->mSets->mNext = set;
    if (set->mNext) {
        set->mNext->mPrev = set;
    }
    SetCollisionMode((GimmickSetDataHead *) set, count);
    *(GimmickSetHead *) &set->mUnk_0c = *(GimmickSetHead *) buf;
    set->mObstacles = *(GimmickSetBlock *) ((u8 *) buf + 4);
    set->mPosition = *pos;
    set->mRotX    = rot[0];
    set->mRotY    = rot[1];
    set->mRotZ    = rot[2];
    set->mHitCount = 0;
    set->mTag = unk;
    return set;
}

void RemoveCollisionSet(GimmickCollision *data, GimmickSetData *set) {
    set->mPrev->mNext = set->mNext;
    if (set->mNext) {
        set->mNext->mPrev = set->mPrev;
    }
    data->mSetRing[data->mRingWriteIndex] = set;
    data->mRingWriteIndex = (data->mRingWriteIndex + 1) & 0x7f;
}

void GimmickCollision::Update() {
    if (data_020d433c.mUnk_14 & 1) {
        mFinished = 1;
        return;
    }
    GimmickSetData *n = mSets;
    do {
        n->mHitCount = 0;
        n = n->mNext;
    } while (n);
    if (!mEnabled) {
        return;
    }
    GimmickSetData *b;
    GimmickSetData *a = mSets;
    do {
        if (a->mMode != 0) {
            for (b = a->mNext; b; b = b->mNext) {
                if (b->mMode != 0) {
                    TestSets(a, b);
                }
            }
        }
        a = a->mNext;
    } while (a);
}

BOOL TestBoxSphere(BattleObstacle *a, BattleObstacle *b);
BOOL TestSphereSphere(BattleObstacle *a, BattleObstacle *b);
BOOL TestPosInBoxXZ(BattleObstacle *a, BattleObstacle *b);
BOOL TestPosInRadiusXZ(BattleObstacle *a, BattleObstacle *b);
BOOL TestCylinder(BattleObstacle *a, BattleObstacle *b);

// Tests indexed by the type of the second object minus one (the first one is a ball). Four entries: the vtable follows
// the table in .data (its size puts it before the vtable in the layout).
static BOOL (*sTests[4])(BattleObstacle *, BattleObstacle *) = {
    TestSphereSphere, TestPosInBoxXZ, TestPosInRadiusXZ, TestCylinder,
};

void GimmickCollision::TestSets(GimmickSetData *a, GimmickSetData *b) {
    BattleObstacle shapeA;
    BattleObstacle shapeB;
    u16 ab = a->mModeMask & b->mMode;
    u16 ba = b->mModeMask & a->mMode;
    if (!(ab | ba)) {
        return;
    }
    u8 *eA;
    GimmickSetFile *fileA = (GimmickSetFile *) &a->mUnk_0c;
    GimmickSetFile *fileB = (GimmickSetFile *) &b->mUnk_0c;
    u8 i = 0;
    u8 j = 0;
    do {
        if (fileA->mCount != 0) {
            eA = (u8 *) fileA + i * 0x20;
            do {
                if (fileB->mCount != 0) {
                    *(ObstacleRaw *) &shapeA = *(ObstacleRaw *) (eA + 4);
                    *(ObstacleRaw *) &shapeB = *(ObstacleRaw *) ((u8 *) fileB + j * 0x20 + 4);
                    VEC_Add(&shapeA.mPos, &a->mPosition, &shapeA.mPos);
                    u16 rxA = shapeA.mRotX + a->mRotX;
                    u16 ryA = shapeA.mRotY + a->mRotY;
                    u16 rzA = shapeA.mRotZ + a->mRotZ;
                    shapeA.mRotX = rxA;
                    shapeA.mRotY = ryA;
                    shapeA.mRotZ = rzA;
                    VEC_Add(&shapeB.mPos, &b->mPosition, &shapeB.mPos);
                    u16 rxB = shapeB.mRotX + b->mRotX;
                    u16 ryB = shapeB.mRotY + b->mRotY;
                    u16 rzB = shapeB.mRotZ + b->mRotZ;
                    shapeB.mRotX = rxB;
                    shapeB.mRotY = ryB;
                    shapeB.mRotZ = rzB;
                    if (!TestObstacles(&shapeA, &shapeB)) {
                        continue;
                    }
                    if (a->mHitCount < 8) {
                        a->mHitSlots[a->mHitCount] = b->mSlot;
                        a->mHitObstacleIndices[a->mHitCount] = j;
                        a->mHitCount++;
                    }
                    if (b->mHitCount < 8) {
                        b->mHitSlots[b->mHitCount] = a->mSlot;
                        b->mHitObstacleIndices[b->mHitCount] = j;
                        b->mHitCount++;
                    }
                }
            } while (++j < fileB->mCount);
        }
    } while (++i < fileA->mCount);
}

BOOL GimmickCollision::TestObstacles(BattleObstacle *a, BattleObstacle *b) {
    BOOL result = 0;
    u8 type     = a->mType;
    if (type == 0) {
        if (b->mType == 0) {
            result = TestBoxSphere(a, b);
        }
    } else if (type == 1) {
        type = b->mType;
        if (type == 0) {
            result = TestBoxSphere(b, a);
        } else {
            result = sTests[type - 1](a, b);
        }
    }
    return result;
}
