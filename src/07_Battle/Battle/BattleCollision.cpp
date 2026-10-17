#include "Battle/BattleTarget.hpp"
#include "Battle/GimmickTypes.hpp"
#include <nitro/fx.h>
#include <nitro/mi.h>

extern "C" void MTX_ApplyRotX33(MtxFx33 *m, u16 angle);
extern "C" void MTX_ApplyRotZ33(MtxFx33 *m, u16 angle);
void GetNearestPointOnBox(BattleSphere *sphere, BattleHitBox *box, VecFx32 *out);
fx32 GetBoxDistance(BattleSphere *sphere, BattleHitBox *box);
BOOL TestBoxSphere(BattleObstacle *a, BattleObstacle *b);
BOOL TestPosInRadiusXZ(BattleObstacle *a, BattleObstacle *b);

// a's centre within b's x/z radius (b->mHalfX), and b's y range (+-mHalfY) overlapping a's y +- a->mHalfX: a's
// radius counts only on y.
BOOL TestCylinder(BattleObstacle *a, BattleObstacle *b) {
    BattleObstacle tmp = *a;
    if (TestPosInRadiusXZ(&tmp, b)) {
        if (b->mPos.y + b->mHalfY >= tmp.mPos.y - tmp.mHalfX && b->mPos.y - b->mHalfY <= tmp.mPos.y + tmp.mHalfX) {
            return 1;
        }
    }
    return 0;
}

BOOL TestPosInRadiusXZ(BattleObstacle *a, BattleObstacle *b) {
    VecFx32 d;
    VEC_Subtract(&a->mPos, &b->mPos, &d);
    d.y = 0;
    fx32 r = b->mHalfX;
    if (r < 0) {
        r = -r;
    }
    return VEC_Mag(&d) < r;
}

// a's centre strictly inside b's half sizes in x and z of b's own rotated frame (y not tested).
BOOL TestPosInBoxXZ(BattleObstacle *a, BattleObstacle *b) {
    VecFx32 d;
    MtxFx33 mtx;
    VEC_Subtract(&a->mPos, &b->mPos, &d);
    mtx = gIdentityMtx33;
    if (b->mRotY) {
        MTX_ApplyRotY33(&mtx, -b->mRotY);
    }
    if (b->mRotX) {
        MTX_ApplyRotX33(&mtx, -b->mRotX);
    }
    if (b->mRotZ) {
        MTX_ApplyRotZ33(&mtx, -b->mRotZ);
    }
    MTX_MultVec33(&d, &mtx, &d);
    fx32 hz = b->mHalfZ;
    fx32 nz = -hz;
    fx32 hx = b->mHalfX;
    if (-hx >= d.x || hx <= d.x || nz >= d.z || hz <= d.z) {
        return 0;
    }
    return 1;
}

// b's centre within |b->mHalfX| of box a. TestObstacles also calls it when both obstacles are OBSTACLE_TYPE_BOX (b then
// read as a sphere).
BOOL TestBoxSphere(BattleObstacle *a, BattleObstacle *b) {
    BattleHitBox box;
    box.mPos.x = a->mPos.x;
    box.mPos.y = a->mPos.y;
    box.mPos.z = a->mPos.z;
    MI_CpuClear32(box.mAxes, 0x24);
    box.mHalfX = a->mHalfX;
    box.mHalfY = a->mHalfY;
    box.mHalfZ = a->mHalfZ;
    box.mRotX  = a->mRotX;
    box.mRotY  = a->mRotY;
    box.mRotZ  = a->mRotZ;
    fx32 d = GetBoxDistance((BattleSphere *) b, &box);
    fx32 r = b->mHalfX;
    if (r < 0) {
        r = -r;
    }
    if (d < 0) {
        d = -d;
    }
    return d <= r;
}

BOOL TestSphereSphere(BattleObstacle *a, BattleObstacle *b) {
    VecFx32 d;
    VEC_Subtract(&a->mPos, &b->mPos, &d);
    fx32 mag = VEC_Mag(&d);
    fx32 rb  = b->mHalfX;
    fx32 ra  = a->mHalfX;
    if (rb < 0) {
        rb = -rb;
    }
    if (ra < 0) {
        ra = -ra;
    }
    return mag < ra + rb;
}

fx32 GetBoxDistance(BattleSphere *sphere, BattleHitBox *box) {
    VecFx32 p;
    GetNearestPointOnBox(sphere, box, &p);
    VEC_Subtract(&p, &sphere->mPos, &p);
    return VEC_Mag(&p);
}

// Nearest point of the rotated box to the sphere's centre (the centre itself when inside); box->mAxes is scratch.
void GetNearestPointOnBox(BattleSphere *sphere, BattleHitBox *box, VecFx32 *out) {
    VecFx32 d;
    fx32 half[3];
    MtxFx33 mtx;
    VecFx32 axes[3];
    half[0] = box->mHalfX;
    half[1] = box->mHalfY;
    half[2] = box->mHalfZ;
    VEC_Subtract(&sphere->mPos, &box->mPos, &d);
    out->x    = box->mPos.x;
    out->y    = box->mPos.y;
    out->z    = box->mPos.z;
    axes[0].x = 0x1000;
    axes[0].y = 0;
    axes[0].z = 0;
    axes[1].x = 0;
    axes[1].y = 0x1000;
    axes[1].z = 0;
    axes[2].x = 0;
    axes[2].y = 0;
    axes[2].z = 0x1000;
    mtx       = gIdentityMtx33;
    MTX_ApplyRotY33(&mtx, box->mRotY);
    MTX_ApplyRotX33(&mtx, box->mRotX);
    MTX_ApplyRotZ33(&mtx, box->mRotZ);
    MTX_MultVec33(&axes[0], &mtx, &box->mAxes[0]);
    MTX_MultVec33(&axes[1], &mtx, &box->mAxes[1]);
    MTX_MultVec33(&axes[2], &mtx, &box->mAxes[2]);
    for (u8 i = 0; i < 3; i++) {
        fx32 t = VEC_DotProduct(&d, &box->mAxes[i]);
        if (t > half[i]) {
            t = half[i];
        }
        if (t < -half[i]) {
            t = -half[i];
        }
        out->x += FX_Mul(t, box->mAxes[i].x);
        out->y += FX_Mul(t, box->mAxes[i].y);
        out->z += FX_Mul(t, box->mAxes[i].z);
    }
}
