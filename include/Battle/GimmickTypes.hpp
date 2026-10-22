#pragma once

#include "Battle/BattleTarget.hpp"
#include "Battle/Gimmick.hpp"
#include "Game/Random.hpp"
#include "Game/GameData.hpp"
#include "Graphics/ModelResource.hpp"
#include <nitro/fx.h>

// Functions of other battle files / ITCM used by the gimmicks.
void LoadCollisionFile(void *buf, u8 id);
GimmickSetData *AddCollisionSet(GimmickCollision *data, int mode, void *buf, VecFx32 *pos, u16 *rot, u8 unk);
void RemoveCollisionSet(GimmickCollision *data, GimmickSetData *set);
extern MtxFx43 data_020c1cf4;
extern "C" fx32 FX_Div(fx32 num, fx32 den); // fixed point num / den (hardware divider)
void RotateVecY(VecFx32 *in, u16 angle, VecFx32 *out); // rotates `in` about Y

// Bakugan statue loaded per field id gGameData.mFieldId (pyrus_dorg00, angelo/diablo, Ventus_skyr_0,
// subterra_majin, haos_tig, hydra). Vtable 02286c84.
class GimmickBakugan : public Gimmick {
public:
    /* 45 */ u8 mVariant;
    /* 46 */ PAD(0x46, 0x48);
    /* 48 */ s32 mPauseFrameCount;
    /* 4c */ s32 mPauseTimer;
    /* 50 */

    GimmickBakugan(u8 a, u8 variant);
    virtual ~GimmickBakugan();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};

// Breakable block, model block_%02d (indexed by gGameData.mFieldId). Vtable 02286e30.
class GimmickBlock : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */

    GimmickBlock(u8 a);
    virtual ~GimmickBlock();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};

// Falling field pieces (fielddestroy_n/b/e models), vtable 02286e90. mKind selects UpdateKindSequence's effect.
class GimmickFieldDestroy : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ ModelInstance *mModelE;
    /* 50 */ u8 mKind;
    /* 51 */ u8 mStep;
    /* 52 */ PAD(0x52, 0x54);
    /* 54 */ u32 mSequenceFrames;
    /* 58 */

    GimmickFieldDestroy(u8 a);
    virtual ~GimmickFieldDestroy();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void func_ov007_0222accc();
    u8 PickKind();
    // While mSequenceFrames runs, one step every sKindStepIntervals[mKind] frames, sKindStepCounts[mKind] steps: seq
    // 0x34 / 0x35 / 0x36 by kind; kind 1 also func_ov007_0226a380(p, p->mRecord, 0x1e) and effect 3, kind 2 effect 0xe
    // and + 0x28 to the turn player's record mGauge (below 0xfd7).
    void UpdateKindSequence();
    void func_ov007_0222b8b4();
    int UpdateModelBAndE();
};

// Wall that is thrown away (sutewall_n / sutewall_b), vtable 02286f94.
class GimmickSuteWall : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ u8 mMarkerNear;
    /* 4d */ PAD(0x4d, 0x50);
    /* 50 */

    GimmickSuteWall(u8 a);
    virtual ~GimmickSuteWall();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    int UpdateModelBThenRemove();
};

// Boost pad (dashbase_00), vtable 02287044. mDir is the push direction, rotated by the model angles.
class GimmickDashBase : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ VecFx32 mDir;
    /* 54 */ u8 mUnk_54;
    /* 55 */ u8 mMarkerNear;
    /* 56 */ PAD(0x56, 0x58);
    /* 58 */

    GimmickDashBase(u8 a);
    virtual ~GimmickDashBase();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void Init();
    void DrawModel();
    void RotateDir(int reset); // its only call passes 0: the reset branch does not run in this build
};

// Helpers of the effect manager (gGameData.mEffects).
u16 StartEffect(EffectManager *manager, u32 a, u32 b, u32 c, u32 d);
void SetEffectMatrix(EffectManager *manager, u16 handle, MtxFx43 *mtx);
void SetEffectScale(EffectManager *manager, u16 handle, VecFx32 *vec);

// Exit marker (exepoint_00), vtable 022870bc.
class GimmickExePoint : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */

    GimmickExePoint(u8 a);
    virtual ~GimmickExePoint();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};

void func_ov007_0226a380(BattlePlayer *player, u8 record, int unk);
extern "C" void MTX_ApplyRotX43(MtxFx43 *m, u16 angle);                       // rotate about X
extern "C" void MTX_Concat43(MtxFx43 *a, MtxFx43 *b, MtxFx43 *out);        // a * b
extern MtxFx43 gIdentityMtx43;

// Power field (fieldpower_g_0), vtable 0228711c.
class GimmickFieldPowerG : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */

    GimmickFieldPowerG(u8 a);
    virtual ~GimmickFieldPowerG();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};

void SetTargetVelocity(BattleTarget *target, VecFx32 *dir, int unk);
void SetTargetVelocityXyz(BattleTarget *target, int a, int b, int c, int d);

// Haos ball (Haosball), vtable 02287184. Pulls the camera towards itself for mTimer frames.
class GimmickHaosBall : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ s32 mTimer;
    /* 4c */

    GimmickHaosBall(u8 a);
    virtual ~GimmickHaosBall();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void DrawModel();
};

void func_ov007_0225ca18(BattleTarget *target, int a, int b);
extern "C" void MTX_MultVec33(VecFx32 *vec, MtxFx33 *m, VecFx32 *out);
extern "C" void MTX_ApplyRotY33(MtxFx33 *m, u16 angle);
extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y); // atan2 (16-bit angle)
extern MtxFx33 gIdentityMtx33;

// Jump pad (jumpobj_00), vtable 022871f0. mDir is the launch direction (0, 0x3000, 0) turned by a random angle.
class GimmickJumpObj : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ VecFx32 mDir;
    /* 54 */ s32 mTimer;
    /* 58 */

    GimmickJumpObj(u8 a);
    virtual ~GimmickJumpObj();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void Init();
    void RotateDir(int reset);
};

// One slot of the manager's doubly linked list (node 0 is the list head).
struct GimmickNode {
    /* 00 */ GimmickNode *mPrev;
    /* 04 */ GimmickNode *mNext;
    /* 08 */ Gimmick *mGimmick;
    /* 0c */
};

// Object record of a SetData_%02d_%02d.bin file as loaded by LoadGimmickSetFile (0x20 bytes).
struct GimmickSetEntry {
    /* 00 */ VecFx32 mPosition;
    /* 0c */ VecFx32 mScale; // half the stored value
    /* 18 */ u16 mRotX;
    /* 1a */ u16 mRotY;
    /* 1c */ u16 mRotZ;
    /* 1e */ u8 mParam;
    /* 1f */ u8 mType;
    /* 20 */
};

struct GimmickSetFile {
    /* 000 */ PAD(0x000, 0x002);
    /* 002 */ u8 mCount;
    /* 003 */ PAD(0x003, 0x004);
    /* 004 */ GimmickSetEntry mEntries[64];
    /* 804 */
};

// Owns up to 63 gimmick objects (ids come from a ring of free ids).
class GimmickManager {
public:
    /* 00 */ u8 *mIds;
    /* 04 */ GimmickNode *mNodes;
    /* 08 */ u8 mTop;
    /* 09 */ u8 mState;
    /* 0a */ PAD(0x0a, 0x0c);
    /* 0c */

    GimmickManager();
    ~GimmickManager();
    void LoadAll();   // Load() of every object
    void UnloadAll(); // Unload() of every object
    void CountTurnAll();
    void CreateFromFile(GimmickSetFile *file);
    void LoadSetData(int a, int b);
    int Add(Gimmick *gimmick); // links the gimmick into a node; returns that node's id from the mIds ring
    void Remove(int id);
};

// Gimmick classes of later rows of the overlay, declared with the right size and constructor so that
// GimmickManager::CreateFromFile can create them.
class GimmickFieldPowerS : public Gimmick { // 0222eb1c, vtable 022872c0, model fieldpower_s_<mVariant>
public:
    /* 45 */ u8 mVariant;
    /* 46 */ PAD(0x46, 0x48);
    /* 48 */

    GimmickFieldPowerS(u8 id, u8 variant);
    virtual ~GimmickFieldPowerS();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void LoadVariantCollision();
};
class GimmickTornado : public Gimmick { // 0222f1e8, vtable 02287358, model vne_tornado
public:
    /* 45 */ PAD(0x45, 0x46);
    /* 46 */ u16 mAngle;
    /* 48 */

    GimmickTornado(u8 id, u8 variant);
    virtual ~GimmickTornado();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};
class GimmickVentKaze : public Gimmick { // 0222f480, vtable 022873d0, model ventkaze0
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */

    GimmickVentKaze(u8 id);
    virtual ~GimmickVentKaze();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};
class GimmickPyrusEruption : public Gimmick { // 0222f8fc, vtable 0228743c, models Pyruseruption_n / Pyruseruption_b
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ u16 mTimer;
    /* 4e */ u8 mPushed; // set as state 3 is left, cleared before state 4 returns to 1: the state-3 test never sees it
    /* 4f */ u8 mMarkerNear;
    /* 50 */

    GimmickPyrusEruption(u8 id);
    virtual ~GimmickPyrusEruption();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    // Advances mModelB's animation only while bit 3 of gGameData.mFlags is clear; its `&= 8` first clears the other
    // bits (as in CountDownToState).
    void DrawModelB();
    void DrawModels(int drawB);
    // Steps mTimer only while bit 3 of gGameData.mFlags is clear; its `&= 8` first clears the other bits.
    int CountDownToState(int frames, int state);
};
// One row of the ability pick table at 02279568: `mCount` entries, picked by weight, mapped through `mIds`.
struct GimmickAbilityRow {
    /* 00 */ u8 mCount;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ const u8 *mIds;
    /* 08 */ const u8 *mWeights;
    /* 0c */
};
extern const GimmickAbilityRow gAbilityRows[];
extern "C" void func_020331a4(void *nsbmd, u32 value, u32 mask); // sets the bits of `mask` of every material's polygon attribute

class GimmickAbilityField : public Gimmick { // 022300dc, vtable 0228750c, models ability_filed0 / ability_filed1
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ u8 mAbilityRowIndex;
    /* 4d */ u8 mInstanceIndex; // sResources[0]->AddRef's count before this instance's reference, capped at 3
    /* 4e */ PAD(0x4e, 0x50);
    /* 50 */

    GimmickAbilityField(u8 id, u8 param);
    virtual ~GimmickAbilityField();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void DrawModelB(); // also sets data_02211374.mUnk_9c from mInstanceIndex before the draw
};
// Model-less pull field (no Load/Unload work): while the target is inside, adds mPullPerFrame to its mBaseVelocity,
// towards a point 0x8000 nearer mPosition turned by 0x71c.
class GimmickPull : public Gimmick { // vtable 022875bc
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ VecFx32 mPullPerFrame; // recomputed every step; the 0x71c turn gives it a sideways part
    /* 54 */ u8 mSoundStarted;
    /* 55 */ PAD(0x55, 0x58);
    /* 58 */

    GimmickPull(u8 id);
    virtual ~GimmickPull();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};
// Model-less trigger (id 0x3d): hands the target's velocity vector to SetTargetVelocity.
class GimmickUnk_02230cec : public Gimmick { // vtable 022875e0
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */

    GimmickUnk_02230cec(u8 id);
    virtual ~GimmickUnk_02230cec();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};
// Model-less hit field (id 0x1f): knocks the target up, drains the current record's gauge and waits mTimer frames.
class GimmickUnk_02274830 : public Gimmick { // vtable 0228af78
public:
    /* 45 */ PAD(0x45, 0x46);
    /* 46 */ u16 mTimer;
    /* 48 */

    GimmickUnk_02274830(u8 id);
    virtual ~GimmickUnk_02274830();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};
// Wall pair of the Haos field (haos_wall): mOtherWallEntryIndex is the set-file entry index of the partner wall (its manager node is the index + 1), the
// target is pulled from the partner's side and the partner gets a mTimer frame cooldown. Vtable 0228af9c.
class GimmickHaosWall : public Gimmick { // 02274d78
public:
    /* 45 */ u8 mTimer; // counts down only on frames without a hit
    /* 46 */ u8 mOtherWallEntryIndex;
    /* 47 */ PAD(0x47, 0x48);
    /* 48 */

    GimmickHaosWall(u8 id, u8 param);
    virtual ~GimmickHaosWall();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void StartTimer(u8 cooldown); // starts the cooldown (state 2)
};
// Speech bubble of the Aquos field (aqousfukidasi_n / _b): pushes the target up while it is near and counts the
// near bubbles in a static counter for the looping sound 0x63. Vtable 0228b16c.
class GimmickAquosFukidashi : public Gimmick { // 02275de8
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ u16 mTimer; // counts down; Load reads 0x104 / 0x140 - mTimer and Update 0x104 - mTimer as elapsed frames
    /* 4e */ u16 mPushTimer;
    /* 50 */ u8 mTargetNear; // last IsTargetNear result, compared with the new one to start / stop the sound
    /* 51 */ u8 mMarkerNear;
    /* 52 */ u8 mPushed;
    /* 53 */ PAD(0x53, 0x54);
    /* 54 */

    GimmickAquosFukidashi(u8 id);
    virtual ~GimmickAquosFukidashi();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    // Advances mModelB's animation only while bit 3 of gGameData.mFlags is clear; its `&= 8` first clears the other
    // bits (as in CountDownToState).
    void DrawModelB();
    void DrawModels(int drawB);
    // Steps mTimer only while bit 3 of gGameData.mFlags is clear; its `&= 8` first clears the other bits.
    int CountDownToState(int frames, int state);
    int IsTargetNear(); // TRUE when the target's bakugan is visible and within 0x96000 of the bubble
};
// Lightning strikes (ikazuchistart / ikaz00): at most two strike at a time (static counter), each waits one of
// the random mDelays between strikes and hits the target when it is in range. Vtable 0228b528.
class GimmickIkazuchiStart : public Gimmick { // 02278270
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ u16 mTimer;
    /* 4e */ u16 mDelays[8];
    /* 5e */ u8 mDelayIndex;
    /* 5f */ u8 mMarkerNear;
    /* 60 */

    GimmickIkazuchiStart(u8 id);
    virtual ~GimmickIkazuchiStart();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    int func_ov007_022789e0(int count); // draws; with `count` also counts mTimer down, TRUE when it hits 0
};
