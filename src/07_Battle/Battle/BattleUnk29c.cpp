#include "global.h"
#include <nitro/fx.h>
#include "types.h"
#include "Scene/SceneBase.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/BattleTarget.hpp"
#include "Battle/BattleMapTypes.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/UnkStruct_0203d374.hpp"


BattleUnk29c::BattleUnk29c() {
    u8 i;
    for (i = 0; i < 12; i++) {
        mUnk_00[i] = 0;
        mUnk_30[i] = 0;
    }
    for (i = 0; i < 9; i++) {
        mUnk_64[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        mUnk_9b[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        mUnk_98[i] = 0;
    }
    mUnk_60 = 0;
    mUnk_9e = 0;
    mUnk_9f = 0;
    mUnk_88.x = 0;
    mUnk_88.y = 0;
    mUnk_88.z = 0;
    mUnk_96 = 0;
    mUnk_94 = 0;
    mUnk_95 = 0;
    mUnk_a0 = 0;
}

BattleUnk29c::~BattleUnk29c() {
    if (mUnk_60) {
        delete mUnk_60;
    }
    mUnk_60 = 0;
}

UnkStruct_0203d374 *BattleUnk29c::func_ov007_0226fc4c(u8 index) {
    if (mUnk_00[index] == 0) {
        return 0;
    }
    return mUnk_00[index];
}

void BattleUnk29c::func_ov007_0226fc5c(u32 index) {
    UnkStruct_0203d374 *pose = mUnk_00[index];
    if (pose) {
        delete pose;
    }
    mUnk_00[index] = 0;
    delete mUnk_30[index];
    mUnk_30[index] = 0;
}

void BattleUnk29c::func_ov007_0226fca8() {
    u32 i = 0;
    do {
        if (mUnk_64[i] == 0) {
            mUnk_64[i] = new BattleFontObj((u8) i);
        }
        i = (u8) (i + 1);
    } while (i <= 6);
    u8 type = data_020d433c.mBattlePlayers->mUnk_2a9;
    if (mUnk_64[7] == 0) {
        mUnk_64[7] = new BattleFontObj((u8) (type + 7));
    }
    type = data_020d433c.mBattlePlayers->mPlayers[type]->mUnk_f2;
    if (mUnk_64[8] == 0) {
        mUnk_64[8] = new BattleFontObj((u8) (type + 11));
    }
}

void BattleUnk29c::func_ov007_0226fd78() {
    u8 i = 0;
    do {
        if (mUnk_00[i]) {
            delete mUnk_00[i];
        }
        mUnk_00[i] = 0;
        delete mUnk_30[i];
        mUnk_30[i] = 0;
        i++;
    } while (i < 12);
    if (mUnk_60) {
        delete mUnk_60;
    }
    mUnk_60 = 0;
    i = 0;
    do {
        if (mUnk_64[i]) {
            delete mUnk_64[i];
        }
        mUnk_64[i] = 0;
        i++;
    } while (i < 9);
    mUnk_94 = 0;
    mUnk_95 = 0;
    mUnk_96 = 0;
    mUnk_88.x = 0;
    mUnk_88.y = 0;
    mUnk_88.z = 0;
}

void BattleUnk29c::func_ov007_0226fe4c() {
    u8 i = 0;
    do {
        if (mUnk_00[i]) {
            switch (mUnk_30[i]->mMode) {
                case 2:
                    func_ov007_02270dbc(i);
                    break;
                case 3:
                    func_ov007_02270f1c(i);
                    break;
                case 4:
                    func_ov007_02271054(i);
                    break;
                case 5:
                    func_ov007_0227118c(i);
                    break;
            }
        }
        i++;
    } while (i < 12);
    if (mUnk_a0 == 0) {
        func_ov007_0226ff08(1);
    } else {
        func_ov007_0226ff08(0);
    }
}

void BattleUnk29c::func_ov007_0226ff08(u32 flag) {
    if (flag == 0 && mUnk_9e == 0) {
        return;
    }
    s32 far[3]  = {0, 0, 0};
    s32 near[3] = {-1, -1, -1};
    VecFx32 diff;
    if (data_020d433c.mBattlePlayers->mUnk_298 == 0) {
        return;
    }
    u8 i;
    for (i = 0; i < 3; i++) {
        mUnk_9b[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        mUnk_98[i] = 0;
    }
    BattleTarget *target = data_020d433c.mBattlePlayers->mUnk_298;
    UnkStruct_0203d374 *tp = target->mUnk_1a4;
    u8 n = 0;
    do {
        if (tp && mUnk_00[n] && mUnk_30[n] && mUnk_30[n]->mMode == 1) {
            BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[n];
            VEC_Subtract(&mUnk_00[n]->mPosition, &tp->mPosition, &diff);
            s32 dist = VEC_Mag(&diff);
            if (unit->mUnk_10 == 2 || unit->mUnk_10 == 3 || unit->mUnk_12 != 0) {
                u8 j = 0;
                do {
                    if (near[j] == -1 || far[j] > dist) {
                        if (j + 1 < 3) {
                            near[j + 1]   = near[j];
                            mUnk_98[j + 1] = mUnk_98[j];
                        }
                        near[j]   = dist;
                        mUnk_98[j] = n + 1;
                        break;
                    }
                    j++;
                } while (j < 3);
            }
            if (dist < 0x64000) {
                u8 j = 0;
                do {
                    if (far[j] == 0 || far[j] > dist) {
                        if (j + 1 < 3) {
                            far[j + 1]     = far[j];
                            mUnk_9b[j + 1] = mUnk_9b[j];
                        }
                        far[j]     = dist;
                        mUnk_9b[j] = n + 1;
                        break;
                    }
                    j++;
                } while (j < 3);
            }
        }
        n++;
    } while (n < 12);
}

void BattleUnk29c::func_ov007_02270120(u32 index, VecFx32 *pos, u16 *rot, u8 a, u8 b) {
    if (mUnk_00[index] == 0 && mUnk_30[index] == 0) {
        mUnk_00[index] = new UnkStruct_0203d374();
        if (data_020d433c.mBattlePlayers->mUnk_295 < 8) {
            mUnk_00[index]->func_0203d4dc(a, b, 1);
        } else {
            mUnk_00[index]->func_0203d4dc(a, b, 2);
        }
        mUnk_00[index]->SetPosition(pos);
        mUnk_00[index]->SetRotation(rot);
        BattleMoveState *m = new BattleMoveState;
        mUnk_30[index]     = m;
        m->mVelocity.x     = 0;
        m->mVelocity.y     = 0;
        m->mVelocity.z     = 0;
        BattleMoveState *r = mUnk_30[index];
        r->mRot[0]         = 0;
        r->mRot[1]         = 0;
        r->mRot[2]         = 0;
        mUnk_30[index]->mMode = 1;
    }
    if (mUnk_60 == 0) {
        mUnk_60 = new BattleMapPiece(2);
    }
}

static inline void CopyVec(VecFx32 *dst, const VecFx32 *src) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

void BattleUnk29c::func_ov007_02270248() {
    u8 i = 0;
    do {
        if (mUnk_00[i] && mUnk_30[i] && mUnk_30[i]->mMode) {
            mUnk_00[i]->func_0203dab0();
            UnkStruct_0203d374 *pose = mUnk_00[i];
            VecFx32 pos              = {0, 0x3800, 0};
            pos.x                    = pose->mPosition.x;
            pos.z                    = pose->mPosition.z;
            VecFx32 sc               = {0x1000, 0x1000, 0x1000};
            if (pose->mUnk_4a && mUnk_60) {
                mUnk_60->mScale    = sc;
                mUnk_60->mPosition = pos;
                mUnk_60->func_020526bc();
            }
        }
        i++;
    } while (i < 12);
    func_ov007_02270350();
    func_ov007_0227068c();
    func_ov007_0227044c();
}

void BattleUnk29c::func_ov007_02270350() {
    if (mUnk_9e == 0) {
        return;
    }
    u8 i = 0;
    do {
        u8 k = mUnk_9b[i];
        if (k && mUnk_00[k - 1]) {
            VecFx32 pos = mUnk_00[k - 1]->mPosition;
            VecFx32 sc  = {0x4cd, 0x4cd, 0x4cd};
            u8 sel      = data_020d433c.mBattlePlayers->mUnits[(u8) (k - 1)].mUnk_0f;
            pos.y += 0x14000;
            if (mUnk_64[sel]) {
                mUnk_64[sel]->mPosition = pos;
                mUnk_64[sel]->mScale    = sc;
                mUnk_64[sel]->func_020526bc();
            }
        }
        i++;
    } while (i < 3);
}

void BattleUnk29c::func_ov007_0227044c() {
    if (mUnk_a0 != 1) {
        return;
    }
    if (mUnk_9e == 0) {
        return;
    }
    u8 i = 0;
    do {
        if (mUnk_9b[i] && mUnk_00[mUnk_9b[i] - 1]) {
            VecFx32 pos = mUnk_00[mUnk_9b[i] - 1]->mPosition;
            VecFx32 sc  = {0x4cd, 0x4cd, 0x4cd};
            u8 idx = mUnk_9b[i] - 1;
            u8 sel = data_020d433c.mBattlePlayers->mUnits[idx].mUnk_0f;
            u8 cur = data_020d433c.mBattlePlayers->mUnk_2a9;
            VecFx32 off = {-0x5000, 0, 0};
            MtxFx43 mtx = CameraManager::Get().mMtxC[0];
            mtx._[9]    = 0;
            mtx._[10]   = 0;
            mtx._[11]   = 0;
            MTX_MultVec43(&off, &mtx, &off);
            VEC_Add(&off, &pos, &pos);
            pos.y += 0x17000;
            if (sel == cur) {
                if (mUnk_64[7]) {
                    mUnk_64[7]->mPosition = pos;
                    mUnk_64[7]->mScale    = sc;
                    mUnk_64[7]->func_020526bc();
                }
            } else if (sel == data_020d433c.mBattlePlayers->mPlayers[cur]->mUnk_f2) {
                if (mUnk_64[8]) {
                    mUnk_64[8]->mPosition = pos;
                    mUnk_64[8]->mScale    = sc;
                    mUnk_64[8]->func_020526bc();
                }
            }
        }
        i++;
    } while (i < 3);
}

void BattleUnk29c::func_ov007_0227068c() {
    u8 i = 0;
    if (mUnk_a0 == 0) {
        do {
            func_ov007_02270784(i + 1);
            i++;
        } while (i < 12);
    } else if (mUnk_9e) {
        do {
            if (mUnk_98[i]) {
                func_ov007_02270784(mUnk_98[i]);
            }
            i++;
        } while (i < 3);
    }
    if (mUnk_94) {
        VecFx32 pos = mUnk_88;
        pos.y += 0xc000;
        mUnk_64[mUnk_96]->mPosition  = pos;
        BattleFontObj *f = mUnk_64[mUnk_96];
        f->mScale.x      = 0x1000;
        f->mScale.y      = 0x1000;
        f->mScale.z      = 0x1000;
        mUnk_64[mUnk_96]->func_020526bc();
    }
}

void BattleUnk29c::func_ov007_02270784(u8 index) {
    s32 scale;
    u32 i = index - 1;
    BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[(u8) i];
    if (unit->mUnk_10 == 0 || mUnk_00[i] == 0) {
        return;
    }
    VecFx32 pos = mUnk_00[i]->mPosition;
    u8 slot     = 0xff;
    if (mUnk_a0 == 0) {
        pos.y += 0xc000;
        scale = 0x1666;
    } else {
        pos.y += 0xa000;
        scale = 0x800;
    }
    if (unit->mUnk_12 != 0) {
        if (mUnk_64[4]) {
            if (unit->mUnk_10 != 1) {
                VecFx32 off = {-0x5000, 0, 0};
                MtxFx43 mtx = CameraManager::Get().mMtxC[0];
                mtx._[9]    = 0;
                mtx._[10]   = 0;
                mtx._[11]   = 0;
                MTX_MultVec43(&off, &mtx, &off);
                VEC_Add(&off, &pos, &off);
                mUnk_64[4]->mPosition = off;
            } else {
                mUnk_64[4]->mPosition = pos;
            }
            BattleFontObj *f = mUnk_64[4];
            f->mScale.x      = scale;
            f->mScale.y      = scale;
            f->mScale.z      = scale;
            mUnk_64[4]->func_020526bc();
        }
        slot = 0xff;
    }
    switch (unit->mUnk_10) {
        case 2:
            slot = 6;
            break;
        case 3:
            slot = 5;
            break;
    }
    if (slot == 0xff || mUnk_64[slot] == 0) {
        return;
    }
    if (unit->mUnk_12 != 0) {
        VecFx32 off = {0x5000, 0, 0};
        MtxFx43 mtx = CameraManager::Get().mMtxC[0];
        mtx._[9]    = 0;
        mtx._[10]   = 0;
        mtx._[11]   = 0;
        MTX_MultVec43(&off, &mtx, &off);
        VEC_Add(&off, &pos, &off);
        mUnk_64[slot]->mPosition = off;
    } else {
        mUnk_64[slot]->mPosition = pos;
    }
    BattleFontObj *g = mUnk_64[slot];
    g->mScale.x      = scale;
    g->mScale.y      = scale;
    g->mScale.z      = scale;
    mUnk_64[slot]->func_020526bc();
}

void BattleUnk29c::func_ov007_02270a7c(u8 index, u32 type) {
    BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[index];
    mUnk_94          = index + 1;
    mUnk_95          = type;
    mUnk_88.x        = unit->mPos.x;
    mUnk_88.y        = unit->mPos.y;
    mUnk_88.z        = unit->mPos.z;
    if (type != 0) {
        if (type == 1) {
            mUnk_96 = 6;
        }
    } else {
        mUnk_96 = 5;
    }
}

void BattleUnk29c::func_ov007_02270ae0(u8 index, BattleMoveState *state) {
    BattleMoveState *m;
    if (mUnk_00[index] && (m = mUnk_30[index]) && m->mMode) {
        *m = *state;
        m  = mUnk_30[index];
        switch (m->mMode) {
            case 2:
                m->mVelocity.x = FX_Mul(m->mVelocity.x, 0xccd);
                m->mVelocity.y = FX_Mul(m->mVelocity.y, 0xccd);
                m->mVelocity.z = FX_Mul(m->mVelocity.z, 0xccd);
                break;
            case 3:
                VEC_Normalize(&m->mVelocity, &m->mVelocity);
                m = mUnk_30[index];
                m->mVelocity.x = FX_Mul(m->mVelocity.x, 0xa000);
                m->mVelocity.y = FX_Mul(m->mVelocity.y, 0xa000);
                m->mVelocity.z = FX_Mul(m->mVelocity.z, 0xa000);
                mUnk_30[index]->mVelocity.y = 0x8000;
                mUnk_30[index]->mRot[1]     = 0x1c71;
                break;
            case 4:
                VEC_Normalize(&m->mVelocity, &m->mVelocity);
                m = mUnk_30[index];
                m->mVelocity.x = FX_Mul(m->mVelocity.x, 0x5000);
                m->mVelocity.y = FX_Mul(m->mVelocity.y, 0x5000);
                m->mVelocity.z = FX_Mul(m->mVelocity.z, 0x5000);
                mUnk_30[index]->mVelocity.y = 0x5800;
                mUnk_30[index]->mRot[0]     = 0x1c71;
                break;
            case 5:
                VEC_Normalize(&m->mVelocity, &m->mVelocity);
                m = mUnk_30[index];
                m->mVelocity.x = FX_Mul(m->mVelocity.x, 0xa000);
                m->mVelocity.y = FX_Mul(m->mVelocity.y, 0xa000);
                m->mVelocity.z = FX_Mul(m->mVelocity.z, 0xa000);
                mUnk_30[index]->mVelocity.y = 0x8000;
                mUnk_30[index]->mRot[0]     = 0x1c71;
                break;
        }
    }
}

// non-matching (rotation tail, shared by four functions): AddRot makes mwcc reload all six halfwords as the original does; what is left is
// register colouring (the original has m in r1 and the loads in r5, r4, r3, r2, r1).
static inline void AddRot(UnkStruct_0203d374 *p, const u16 *r) {
    u16 rx = p->mRotX + r[0];
    u16 ry = p->mRotY + r[1];
    u16 rz = p->mRotZ + r[2];
    p->mRotX = rx;
    p->mRotY = ry;
    p->mRotZ = rz;
}
void BattleUnk29c::func_ov007_02270dbc(u32 index) {
    BattleMoveState *m = mUnk_30[index];
    if (m->mVelocity.x != 0 || m->mVelocity.y != 0 || m->mVelocity.z != 0) {
        UnkStruct_0203d374 *q = mUnk_00[index];
        VecFx32 pos;
        pos.x = q->mPosition.x;
        pos.y = q->mPosition.y;
        pos.z = q->mPosition.z;
        VEC_Add(&pos, &mUnk_30[index]->mVelocity, &pos);
        mUnk_00[index]->mPosition = pos;
        m = mUnk_30[index];
        m->mVelocity.x = FX_Mul(m->mVelocity.x, 0xf0a);
        m->mVelocity.y = FX_Mul(m->mVelocity.y, 0xf0a);
        m->mVelocity.z = FX_Mul(m->mVelocity.z, 0xf0a);
    }
    m = mUnk_30[index];
    if (m->mRot[0] != 0 || m->mRot[1] != 0 || m->mRot[2] != 0) {
        AddRot(mUnk_00[index], m->mRot);
    }
}

// non-matching (rotation tail, shared by four functions): AddRot makes mwcc reload all six halfwords as the original does; what is left is
// register colouring (the original has m in r1 and the loads in r5, r4, r3, r2, r1).
void BattleUnk29c::func_ov007_02270f1c(u32 index) {
    BattleMoveState *m = mUnk_30[index];
    if (m->mVelocity.x != 0 || m->mVelocity.y != 0 || m->mVelocity.z != 0) {
        UnkStruct_0203d374 *q = mUnk_00[index];
        VecFx32 pos;
        pos.x = q->mPosition.x;
        pos.y = q->mPosition.y;
        pos.z = q->mPosition.z;
        VEC_Add(&pos, &mUnk_30[index]->mVelocity, &pos);
        mUnk_00[index]->mPosition = pos;
        mUnk_30[index]->mVelocity.y -= 0x800;
        if (pos.y < 0x899a) {
            mUnk_00[index]->mUnk_4a = 0;
            m = mUnk_30[index];
            m->mVelocity.x = 0;
            m->mVelocity.y = 0;
            m->mVelocity.z = 0;
            m = mUnk_30[index];
            m->mRot[0] = 0;
            m->mRot[1] = 0;
            m->mRot[2] = 0;
        }
    }
    m = mUnk_30[index];
    if (m->mRot[0] != 0 || m->mRot[1] != 0 || m->mRot[2] != 0) {
        AddRot(mUnk_00[index], m->mRot);
    }
}

// non-matching (rotation tail, shared by four functions): AddRot makes mwcc reload all six halfwords as the original does; what is left is
// register colouring (the original has m in r1 and the loads in r5, r4, r3, r2, r1).
void BattleUnk29c::func_ov007_02271054(u32 index) {
    BattleMoveState *m = mUnk_30[index];
    if (m->mVelocity.x != 0 || m->mVelocity.y != 0 || m->mVelocity.z != 0) {
        UnkStruct_0203d374 *q = mUnk_00[index];
        VecFx32 pos;
        pos.x = q->mPosition.x;
        pos.y = q->mPosition.y;
        pos.z = q->mPosition.z;
        VEC_Add(&pos, &mUnk_30[index]->mVelocity, &pos);
        mUnk_00[index]->mPosition = pos;
        mUnk_30[index]->mVelocity.y -= 0x800;
        if (pos.y < 0x899a) {
            mUnk_00[index]->mUnk_4a = 0;
            m = mUnk_30[index];
            m->mVelocity.x = 0;
            m->mVelocity.y = 0;
            m->mVelocity.z = 0;
            m = mUnk_30[index];
            m->mRot[0] = 0;
            m->mRot[1] = 0;
            m->mRot[2] = 0;
        }
    }
    m = mUnk_30[index];
    if (m->mRot[0] != 0 || m->mRot[1] != 0 || m->mRot[2] != 0) {
        AddRot(mUnk_00[index], m->mRot);
    }
}

// non-matching (rotation tail, shared by four functions): AddRot makes mwcc reload all six halfwords as the original does; what is left is
// register colouring (the original has m in r1 and the loads in r5, r4, r3, r2, r1).
void BattleUnk29c::func_ov007_0227118c(u32 index) {
    BattleMoveState *m = mUnk_30[index];
    if (m->mVelocity.x != 0 || m->mVelocity.y != 0 || m->mVelocity.z != 0) {
        UnkStruct_0203d374 *q = mUnk_00[index];
        VecFx32 pos;
        pos.x = q->mPosition.x;
        pos.y = q->mPosition.y;
        pos.z = q->mPosition.z;
        VEC_Add(&pos, &mUnk_30[index]->mVelocity, &pos);
        mUnk_00[index]->mPosition = pos;
        mUnk_30[index]->mVelocity.y -= 0x800;
        if (pos.y < 0x899a) {
            mUnk_00[index]->mUnk_4a = 0;
            m = mUnk_30[index];
            m->mVelocity.x = 0;
            m->mVelocity.y = 0;
            m->mVelocity.z = 0;
            m = mUnk_30[index];
            m->mRot[0] = 0;
            m->mRot[1] = 0;
            m->mRot[2] = 0;
        }
    }
    m = mUnk_30[index];
    if (m->mRot[0] != 0 || m->mRot[1] != 0 || m->mRot[2] != 0) {
        AddRot(mUnk_00[index], m->mRot);
    }
}

void BattleUnk29c::func_ov007_022712c4(u8 index, VecFx32 *pos) {
    VecFx32 scale = {0x1000, 0x1000, 0x1000};
    mUnk_64[index]->mPosition = *pos;
    mUnk_64[index]->mScale    = scale;
    mUnk_64[index]->func_020526bc();
}

void BattleUnk29c::func_ov007_02271338(VecFx32 *pos) {
    VecFx32 scale = {0x1000, 0x1000, 0x1000};
    mUnk_64[7]->mPosition = *pos;
    mUnk_64[7]->mScale    = scale;
    mUnk_64[7]->func_020526bc();
}

void BattleUnk29c::func_ov007_022713a4(VecFx32 *pos) {
    VecFx32 scale = {0x1000, 0x1000, 0x1000};
    mUnk_64[8]->mPosition = *pos;
    mUnk_64[8]->mScale    = scale;
    mUnk_64[8]->func_020526bc();
}
