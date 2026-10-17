#include "global.h"
#include <nitro/fx.h>
#include "types.h"
#include "Scene/SceneBase.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Battle/BattleTarget.hpp"
#include "Battle/BattleMapTypes.hpp"
#include "Graphics/BakuganModel.hpp"


BattleUnk29c::BattleUnk29c() {
    u8 i;
    for (i = 0; i < 12; i++) {
        mBakugans[i] = 0;
        mMoveStates[i] = 0;
    }
    for (i = 0; i < 9; i++) {
        mFontObjs[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        mUnk_9b[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        mUnk_98[i] = 0;
    }
    mShadow = 0;
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
    if (mShadow) {
        delete mShadow;
    }
    mShadow = 0;
}

BakuganModel *BattleUnk29c::GetBakugan(u8 index) {
    if (mBakugans[index] == 0) {
        return 0;
    }
    return mBakugans[index];
}

void BattleUnk29c::func_ov007_0226fc5c(u32 index) {
    BakuganModel *pose = mBakugans[index];
    if (pose) {
        delete pose;
    }
    mBakugans[index] = 0;
    delete mMoveStates[index];
    mMoveStates[index] = 0;
}

void BattleUnk29c::CreateFontObjs() {
    u32 i = 0;
    do {
        if (mFontObjs[i] == 0) {
            mFontObjs[i] = new BattleFontObj((u8) i);
        }
        i = (u8) (i + 1);
    } while (i <= 6);
    u8 type = data_020d433c.mBattlePlayers->mUnk_2a9;
    if (mFontObjs[7] == 0) {
        mFontObjs[7] = new BattleFontObj((u8) (type + 7));
    }
    type = data_020d433c.mBattlePlayers->mPlayers[type]->mPartner;
    if (mFontObjs[8] == 0) {
        mFontObjs[8] = new BattleFontObj((u8) (type + 11));
    }
}

void BattleUnk29c::Release() {
    u8 i = 0;
    do {
        if (mBakugans[i]) {
            delete mBakugans[i];
        }
        mBakugans[i] = 0;
        delete mMoveStates[i];
        mMoveStates[i] = 0;
        i++;
    } while (i < 12);
    if (mShadow) {
        delete mShadow;
    }
    mShadow = 0;
    i = 0;
    do {
        if (mFontObjs[i]) {
            delete mFontObjs[i];
        }
        mFontObjs[i] = 0;
        i++;
    } while (i < 9);
    mUnk_94 = 0;
    mUnk_95 = 0;
    mUnk_96 = 0;
    mUnk_88.x = 0;
    mUnk_88.y = 0;
    mUnk_88.z = 0;
}

void BattleUnk29c::Update() {
    u8 i = 0;
    do {
        if (mBakugans[i]) {
            switch (mMoveStates[i]->mMode) {
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
    if (data_020d433c.mBattlePlayers->mBattleTarget == 0) {
        return;
    }
    u8 i;
    for (i = 0; i < 3; i++) {
        mUnk_9b[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        mUnk_98[i] = 0;
    }
    BattleTarget *target = data_020d433c.mBattlePlayers->mBattleTarget;
    BakuganModel *tp = target->mBakugan;
    u8 n = 0;
    do {
        if (tp && mBakugans[n] && mMoveStates[n] && mMoveStates[n]->mMode == 1) {
            BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[n];
            VEC_Subtract(&mBakugans[n]->mPosition, &tp->mPosition, &diff);
            s32 dist = VEC_Mag(&diff);
            if (unit->mStatus == 2 || unit->mStatus == 3 || unit->mUnk_12 != 0) {
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

void BattleUnk29c::CreateBakugan(u32 index, VecFx32 *pos, u16 *rot, u8 a, u8 b) {
    if (mBakugans[index] == 0 && mMoveStates[index] == 0) {
        mBakugans[index] = new BakuganModel();
        if (data_020d433c.mBattlePlayers->mUnitCount < 8) {
            mBakugans[index]->Setup(a, b, 1);
        } else {
            mBakugans[index]->Setup(a, b, 2);
        }
        mBakugans[index]->SetPosition(pos);
        mBakugans[index]->SetRotation(rot);
        BattleMoveState *m = new BattleMoveState;
        mMoveStates[index]     = m;
        m->mVelocity.x     = 0;
        m->mVelocity.y     = 0;
        m->mVelocity.z     = 0;
        BattleMoveState *r = mMoveStates[index];
        r->mRot[0]         = 0;
        r->mRot[1]         = 0;
        r->mRot[2]         = 0;
        mMoveStates[index]->mMode = 1;
    }
    if (mShadow == 0) {
        mShadow = new BattleMapPiece(2);
    }
}

static inline void CopyVec(VecFx32 *dst, const VecFx32 *src) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

void BattleUnk29c::Draw() {
    u8 i = 0;
    do {
        if (mBakugans[i] && mMoveStates[i] && mMoveStates[i]->mMode) {
            mBakugans[i]->DrawModel();
            BakuganModel *pose = mBakugans[i];
            VecFx32 pos              = {0, 0x3800, 0};
            pos.x                    = pose->mPosition.x;
            pos.z                    = pose->mPosition.z;
            VecFx32 sc               = {0x1000, 0x1000, 0x1000};
            if (pose->mVisible && mShadow) {
                mShadow->mScale    = sc;
                mShadow->mPosition = pos;
                mShadow->DrawInstance();
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
        if (k && mBakugans[k - 1]) {
            VecFx32 pos = mBakugans[k - 1]->mPosition;
            VecFx32 sc  = {0x4cd, 0x4cd, 0x4cd};
            u8 sel      = data_020d433c.mBattlePlayers->mUnits[(u8) (k - 1)].mPlayer;
            pos.y += 0x14000;
            if (mFontObjs[sel]) {
                mFontObjs[sel]->mPosition = pos;
                mFontObjs[sel]->mScale    = sc;
                mFontObjs[sel]->DrawInstance();
            }
        }
        i++;
    } while (i < 3);
}

// func_ov007_0227044c's scale and offset are file statics defined before the camera header is first parsed: the
// size sort needs them ahead of CameraManager's guard, node and instance to give the original .rodata order.
static const VecFx32 sMarkerScale  = {0x4cd, 0x4cd, 0x4cd};
static const VecFx32 sMarkerOffset = {-0x5000, 0, 0};

#include "Graphics/Camera.hpp"

void BattleUnk29c::func_ov007_0227044c() {
    if (mUnk_a0 != 1) {
        return;
    }
    if (mUnk_9e == 0) {
        return;
    }
    u8 i = 0;
    do {
        if (mUnk_9b[i] && mBakugans[mUnk_9b[i] - 1]) {
            VecFx32 pos = mBakugans[mUnk_9b[i] - 1]->mPosition;
            VecFx32 sc  = sMarkerScale;
            u8 idx = mUnk_9b[i] - 1;
            u8 sel = data_020d433c.mBattlePlayers->mUnits[idx].mPlayer;
            u8 cur = data_020d433c.mBattlePlayers->mUnk_2a9;
            VecFx32 off = sMarkerOffset;
            MtxFx43 mtx = CameraManager::Get().mMtxC[0];
            mtx._[9]    = 0;
            mtx._[10]   = 0;
            mtx._[11]   = 0;
            MTX_MultVec43(&off, &mtx, &off);
            VEC_Add(&off, &pos, &pos);
            pos.y += 0x17000;
            if (sel == cur) {
                if (mFontObjs[7]) {
                    mFontObjs[7]->mPosition = pos;
                    mFontObjs[7]->mScale    = sc;
                    mFontObjs[7]->DrawInstance();
                }
            } else if (sel == data_020d433c.mBattlePlayers->mPlayers[cur]->mPartner) {
                if (mFontObjs[8]) {
                    mFontObjs[8]->mPosition = pos;
                    mFontObjs[8]->mScale    = sc;
                    mFontObjs[8]->DrawInstance();
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
        mFontObjs[mUnk_96]->mPosition  = pos;
        BattleFontObj *f = mFontObjs[mUnk_96];
        f->mScale.x      = 0x1000;
        f->mScale.y      = 0x1000;
        f->mScale.z      = 0x1000;
        mFontObjs[mUnk_96]->DrawInstance();
    }
}

void BattleUnk29c::func_ov007_02270784(u8 index) {
    s32 scale;
    u32 i = index - 1;
    BattleUnit *unit = &data_020d433c.mBattlePlayers->mUnits[(u8) i];
    if (unit->mStatus == 0 || mBakugans[i] == 0) {
        return;
    }
    VecFx32 pos = mBakugans[i]->mPosition;
    u8 slot     = 0xff;
    if (mUnk_a0 == 0) {
        pos.y += 0xc000;
        scale = 0x1666;
    } else {
        pos.y += 0xa000;
        scale = 0x800;
    }
    if (unit->mUnk_12 != 0) {
        if (mFontObjs[4]) {
            if (unit->mStatus != 1) {
                VecFx32 off = {-0x5000, 0, 0};
                MtxFx43 mtx = CameraManager::Get().mMtxC[0];
                mtx._[9]    = 0;
                mtx._[10]   = 0;
                mtx._[11]   = 0;
                MTX_MultVec43(&off, &mtx, &off);
                VEC_Add(&off, &pos, &off);
                mFontObjs[4]->mPosition = off;
            } else {
                mFontObjs[4]->mPosition = pos;
            }
            BattleFontObj *f = mFontObjs[4];
            f->mScale.x      = scale;
            f->mScale.y      = scale;
            f->mScale.z      = scale;
            mFontObjs[4]->DrawInstance();
        }
        slot = 0xff;
    }
    switch (unit->mStatus) {
        case 2:
            slot = 6;
            break;
        case 3:
            slot = 5;
            break;
    }
    if (slot == 0xff || mFontObjs[slot] == 0) {
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
        mFontObjs[slot]->mPosition = off;
    } else {
        mFontObjs[slot]->mPosition = pos;
    }
    BattleFontObj *g = mFontObjs[slot];
    g->mScale.x      = scale;
    g->mScale.y      = scale;
    g->mScale.z      = scale;
    mFontObjs[slot]->DrawInstance();
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

void BattleUnk29c::StartMove(u8 index, BattleMoveState *state) {
    BattleMoveState *m;
    if (mBakugans[index] && (m = mMoveStates[index]) && m->mMode) {
        *m = *state;
        m  = mMoveStates[index];
        switch (m->mMode) {
            case 2:
                m->mVelocity.x = FX_Mul(m->mVelocity.x, 0xccd);
                m->mVelocity.y = FX_Mul(m->mVelocity.y, 0xccd);
                m->mVelocity.z = FX_Mul(m->mVelocity.z, 0xccd);
                break;
            case 3:
                VEC_Normalize(&m->mVelocity, &m->mVelocity);
                m = mMoveStates[index];
                m->mVelocity.x = FX_Mul(m->mVelocity.x, 0xa000);
                m->mVelocity.y = FX_Mul(m->mVelocity.y, 0xa000);
                m->mVelocity.z = FX_Mul(m->mVelocity.z, 0xa000);
                mMoveStates[index]->mVelocity.y = 0x8000;
                mMoveStates[index]->mRot[1]     = 0x1c71;
                break;
            case 4:
                VEC_Normalize(&m->mVelocity, &m->mVelocity);
                m = mMoveStates[index];
                m->mVelocity.x = FX_Mul(m->mVelocity.x, 0x5000);
                m->mVelocity.y = FX_Mul(m->mVelocity.y, 0x5000);
                m->mVelocity.z = FX_Mul(m->mVelocity.z, 0x5000);
                mMoveStates[index]->mVelocity.y = 0x5800;
                mMoveStates[index]->mRot[0]     = 0x1c71;
                break;
            case 5:
                VEC_Normalize(&m->mVelocity, &m->mVelocity);
                m = mMoveStates[index];
                m->mVelocity.x = FX_Mul(m->mVelocity.x, 0xa000);
                m->mVelocity.y = FX_Mul(m->mVelocity.y, 0xa000);
                m->mVelocity.z = FX_Mul(m->mVelocity.z, 0xa000);
                mMoveStates[index]->mVelocity.y = 0x8000;
                mMoveStates[index]->mRot[0]     = 0x1c71;
                break;
        }
    }
}

// The x angle is read first and added last (`rx += r[0]`): that loads p->mRotX first and r[0] last, which gives the
// original's registers (a plain `p->mRotX + r[0]` is substituted into the store and loaded after the y and z sums).
static inline void AddRot(BakuganModel *p, const u16 *r) {
    u16 rx = p->mRotX;
    u16 ry = p->mRotY + r[1];
    u16 rz = p->mRotZ + r[2];
    rx += r[0];
    p->mRotX = rx;
    p->mRotY = ry;
    p->mRotZ = rz;
}
void BattleUnk29c::func_ov007_02270dbc(u32 index) {
    BattleMoveState *m = mMoveStates[index];
    if (m->mVelocity.x != 0 || m->mVelocity.y != 0 || m->mVelocity.z != 0) {
        BakuganModel *q = mBakugans[index];
        VecFx32 pos;
        pos.x = q->mPosition.x;
        pos.y = q->mPosition.y;
        pos.z = q->mPosition.z;
        VEC_Add(&pos, &mMoveStates[index]->mVelocity, &pos);
        mBakugans[index]->mPosition = pos;
        m = mMoveStates[index];
        m->mVelocity.x = FX_Mul(m->mVelocity.x, 0xf0a);
        m->mVelocity.y = FX_Mul(m->mVelocity.y, 0xf0a);
        m->mVelocity.z = FX_Mul(m->mVelocity.z, 0xf0a);
    }
    m = mMoveStates[index];
    if (m->mRot[0] != 0 || m->mRot[1] != 0 || m->mRot[2] != 0) {
        AddRot(mBakugans[index], m->mRot);
    }
}

void BattleUnk29c::func_ov007_02270f1c(u32 index) {
    BattleMoveState *m = mMoveStates[index];
    if (m->mVelocity.x != 0 || m->mVelocity.y != 0 || m->mVelocity.z != 0) {
        BakuganModel *q = mBakugans[index];
        VecFx32 pos;
        pos.x = q->mPosition.x;
        pos.y = q->mPosition.y;
        pos.z = q->mPosition.z;
        VEC_Add(&pos, &mMoveStates[index]->mVelocity, &pos);
        mBakugans[index]->mPosition = pos;
        mMoveStates[index]->mVelocity.y -= 0x800;
        if (pos.y < 0x899a) {
            mBakugans[index]->mVisible = 0;
            m = mMoveStates[index];
            m->mVelocity.x = 0;
            m->mVelocity.y = 0;
            m->mVelocity.z = 0;
            m = mMoveStates[index];
            m->mRot[0] = 0;
            m->mRot[1] = 0;
            m->mRot[2] = 0;
        }
    }
    m = mMoveStates[index];
    if (m->mRot[0] != 0 || m->mRot[1] != 0 || m->mRot[2] != 0) {
        AddRot(mBakugans[index], m->mRot);
    }
}

void BattleUnk29c::func_ov007_02271054(u32 index) {
    BattleMoveState *m = mMoveStates[index];
    if (m->mVelocity.x != 0 || m->mVelocity.y != 0 || m->mVelocity.z != 0) {
        BakuganModel *q = mBakugans[index];
        VecFx32 pos;
        pos.x = q->mPosition.x;
        pos.y = q->mPosition.y;
        pos.z = q->mPosition.z;
        VEC_Add(&pos, &mMoveStates[index]->mVelocity, &pos);
        mBakugans[index]->mPosition = pos;
        mMoveStates[index]->mVelocity.y -= 0x800;
        if (pos.y < 0x899a) {
            mBakugans[index]->mVisible = 0;
            m = mMoveStates[index];
            m->mVelocity.x = 0;
            m->mVelocity.y = 0;
            m->mVelocity.z = 0;
            m = mMoveStates[index];
            m->mRot[0] = 0;
            m->mRot[1] = 0;
            m->mRot[2] = 0;
        }
    }
    m = mMoveStates[index];
    if (m->mRot[0] != 0 || m->mRot[1] != 0 || m->mRot[2] != 0) {
        AddRot(mBakugans[index], m->mRot);
    }
}

void BattleUnk29c::func_ov007_0227118c(u32 index) {
    BattleMoveState *m = mMoveStates[index];
    if (m->mVelocity.x != 0 || m->mVelocity.y != 0 || m->mVelocity.z != 0) {
        BakuganModel *q = mBakugans[index];
        VecFx32 pos;
        pos.x = q->mPosition.x;
        pos.y = q->mPosition.y;
        pos.z = q->mPosition.z;
        VEC_Add(&pos, &mMoveStates[index]->mVelocity, &pos);
        mBakugans[index]->mPosition = pos;
        mMoveStates[index]->mVelocity.y -= 0x800;
        if (pos.y < 0x899a) {
            mBakugans[index]->mVisible = 0;
            m = mMoveStates[index];
            m->mVelocity.x = 0;
            m->mVelocity.y = 0;
            m->mVelocity.z = 0;
            m = mMoveStates[index];
            m->mRot[0] = 0;
            m->mRot[1] = 0;
            m->mRot[2] = 0;
        }
    }
    m = mMoveStates[index];
    if (m->mRot[0] != 0 || m->mRot[1] != 0 || m->mRot[2] != 0) {
        AddRot(mBakugans[index], m->mRot);
    }
}

void BattleUnk29c::func_ov007_022712c4(u8 index, VecFx32 *pos) {
    VecFx32 scale = {0x1000, 0x1000, 0x1000};
    mFontObjs[index]->mPosition = *pos;
    mFontObjs[index]->mScale    = scale;
    mFontObjs[index]->DrawInstance();
}

void BattleUnk29c::func_ov007_02271338(VecFx32 *pos) {
    VecFx32 scale = {0x1000, 0x1000, 0x1000};
    mFontObjs[7]->mPosition = *pos;
    mFontObjs[7]->mScale    = scale;
    mFontObjs[7]->DrawInstance();
}

void BattleUnk29c::func_ov007_022713a4(VecFx32 *pos) {
    VecFx32 scale = {0x1000, 0x1000, 0x1000};
    mFontObjs[8]->mPosition = *pos;
    mFontObjs[8]->mScale    = scale;
    mFontObjs[8]->DrawInstance();
}
