#pragma once

#include "Battle/BattleTarget.hpp"
#include "Battle/Gimmick.hpp"
#include "Game/Random.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/ModelResource.hpp"
#include <nitro/fx.h>

// Functions of other battle files / ITCM used by the gimmicks.
void func_ov007_022719c8(void *buf, u8 id);
GimmickSetData *func_ov007_02271ca8(UnkStruct_020d433c_0c *data, int count, void *buf, VecFx32 *pos, u16 *rot, u8 unk);
void func_ov007_02271d78(UnkStruct_020d433c_0c *data, GimmickSetData *set);
extern MtxFx43 data_020c1cf4;
extern "C" fx32 func_01ff8000(fx32 num, fx32 den); // fixed point num / den (hardware divider)
void func_ov007_02233ed8(VecFx32 *in, u16 angle, VecFx32 *out); // rotates `in` about Y

// Bakugan statue shown for the attribute in data_020d433c.mUnk_95 (pyrus_dorg00, angelo/diablo, Ventus_skyr_0,
// subterra_majin, haos_tig, hydra). Vtable 02286c84.
class GimmickBakugan : public Gimmick {
public:
    /* 45 */ u8 mVariant;
    /* 46 */ PAD(0x46, 0x48);
    /* 48 */ s32 mAnimFrames;
    /* 4c */ s32 mAnimTimer;
    /* 50 */

    GimmickBakugan(u8 a, u8 variant);
    virtual ~GimmickBakugan();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};

// Breakable block, model block_%02d (indexed by data_020d433c.mUnk_95). Vtable 02286e30.
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

// Falling field pieces (fielddestroy_n/b/e models), vtable 02286e90. mKind selects the effect run by func_0222b410.
class GimmickFieldDestroy : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ ModelInstance *mModelE;
    /* 50 */ u8 mKind;
    /* 51 */ u8 mStep;
    /* 52 */ PAD(0x52, 0x54);
    /* 54 */ u32 mTimer;
    /* 58 */

    GimmickFieldDestroy(u8 a);
    virtual ~GimmickFieldDestroy();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void func_ov007_0222accc();
    void func_ov007_0222b410();
    void func_ov007_0222b8b4();
    int func_ov007_0222bc24();
};

// Wall that is thrown away (sutewall_n / sutewall_b), vtable 02286f94.
class GimmickSuteWall : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ u8 mUnk_4c;
    /* 4d */ PAD(0x4d, 0x50);
    /* 50 */

    GimmickSuteWall(u8 a);
    virtual ~GimmickSuteWall();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    int func_ov007_0222c2f0();
};

// Boost pad (dashbase_00), vtable 02287044. mDir is the push direction, rotated by the model angles.
class GimmickDashBase : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ VecFx32 mDir;
    /* 54 */ u8 mUnk_54;
    /* 55 */ u8 mUnk_55;
    /* 56 */ PAD(0x56, 0x58);
    /* 58 */

    GimmickDashBase(u8 a);
    virtual ~GimmickDashBase();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void func_ov007_0222c48c();
    void func_ov007_0222c7f0();
    void func_ov007_0222c824(int reset);
};

// Helpers of the effect manager (data_020d433c.mEffects).
u16 func_0202bd40(EffectManager *manager, u32 a, u32 b, u32 c, u32 d);
void func_0202bf60(EffectManager *manager, u16 handle, MtxFx43 *mtx);
void func_0202bf18(EffectManager *manager, u16 handle, VecFx32 *vec);

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
void func_01ffd088(MtxFx43 *m, u16 angle);                       // rotate about X
extern "C" void func_01ff89c0(MtxFx43 *a, MtxFx43 *b, MtxFx43 *out);        // a * b
extern MtxFx43 data_0220e86c;

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

void func_ov007_0225c960(BattleTarget *target, VecFx32 *dir, int unk);
void func_ov007_0225c9c0(BattleTarget *target, int a, int b, int c, int d);

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

    void func_ov007_0222db58();
};

void func_ov007_0225ca18(BattleTarget *target, int a, int b);
extern "C" void func_01ff84d0(VecFx32 *vec, MtxFx33 *m, VecFx32 *out);
void func_01ffcfd8(MtxFx33 *m, u16 angle);
extern "C" u16 FX_Atan2Idx(fx32 x, fx32 y); // atan2 (16-bit angle)
extern MtxFx33 data_0220e848;

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

    void func_ov007_0222dc3c();
    void func_ov007_0222e08c(int reset);
};

// One slot of the manager's doubly linked list (node 0 is the list head).
struct GimmickNode {
    /* 00 */ GimmickNode *mPrev;
    /* 04 */ GimmickNode *mNext;
    /* 08 */ Gimmick *mGimmick;
    /* 0c */
};

// Object record of a SetData_%02d_%02d.bin file. The records are 0x20 bytes apart and start 4 bytes into
// the file: fields sit at +04 (position), +1c (angles), +22 and +23, counted from the file start.
struct GimmickSetEntry {
    /* 00 */ PAD(0x00, 0x04);
    /* 04 */ VecFx32 mPosition;
    /* 10 */ PAD(0x10, 0x1c);
    /* 1c */ u16 mRotX;
    /* 1e */ u16 mRotY;
    /* 20 */ u16 mRotZ;
    /* 22 */ u8 mParam;
    /* 23 */ u8 mType;
    /* 24 */
};

struct GimmickSetFile {
    /* 00 */ PAD(0x00, 0x02);
    /* 02 */ u8 mCount;
    /* 03 */ PAD(0x03, 0x04);

    GimmickSetEntry *Entry(int i) {
        return (GimmickSetEntry *)((u8 *)this + i * 0x20);
    }
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
    void func_ov007_0222e25c();
    void func_ov007_0222e290(GimmickSetFile *file);
    void func_ov007_0222e75c(int a, int b);
    int func_ov007_0222e7f4(Gimmick *gimmick);
    void func_ov007_0222e860(int id);
};

// Gimmick classes of later rows of the overlay, declared with the right size and constructor so that
// GimmickManager::func_ov007_0222e290 can create them.
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

    void func_ov007_0222f1ac();
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
    /* 4c */ u16 mCounter;
    /* 4e */ u8 mUnk_4e;
    /* 4f */ u8 mUnk_4f;
    /* 50 */

    GimmickPyrusEruption(u8 id);
    virtual ~GimmickPyrusEruption();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void func_ov007_0222ffb0();
    void func_ov007_02230040(int drawB);
    int func_ov007_0223008c(int frames, int state);
};
// One row of the ability pick table at 02279568: `mCount` entries, picked by weight, mapped through `mIds`.
struct GimmickAbilityRow {
    /* 00 */ u8 mCount;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ const u8 *mIds;
    /* 08 */ const u8 *mWeights;
    /* 0c */
};
extern const GimmickAbilityRow data_ov007_02279568[];
extern "C" void func_020331a4(void *nsbmd, u32 value, u32 mask); // sets the bits of `mask` of every material's polygon attribute

class GimmickAbilityField : public Gimmick { // 022300dc, vtable 0228750c, models ability_filed0 / ability_filed1
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ u8 mUnk_4c;
    /* 4d */ u8 mUnk_4d;
    /* 4e */ PAD(0x4e, 0x50);
    /* 50 */

    GimmickAbilityField(u8 id, u8 param);
    virtual ~GimmickAbilityField();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void func_ov007_022303f4();
};
// Model-less push field (no Load/Unload work): while the target is inside, pushes it away from mPosition.
class GimmickUnk_022308a8 : public Gimmick { // vtable 022875bc
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ VecFx32 mUnk_48;
    /* 54 */ u8 mUnk_54;
    /* 55 */ PAD(0x55, 0x58);
    /* 58 */

    GimmickUnk_022308a8(u8 id);
    virtual ~GimmickUnk_022308a8();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};
// Model-less trigger (id 0x3d): hands the target's velocity vector to func_ov007_0225c960.
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
// Model-less hit field (id 0x1f): knocks the target up, drains the current record's gauge and waits mUnk_46 frames.
class GimmickUnk_02274830 : public Gimmick { // vtable 0228af78
public:
    /* 45 */ PAD(0x45, 0x46);
    /* 46 */ u16 mUnk_46;
    /* 48 */

    GimmickUnk_02274830(u8 id);
    virtual ~GimmickUnk_02274830();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();
};
// Wall pair of the Haos field (haos_wall): mUnk_46 is the manager id of the partner wall (node mUnk_46 + 1), the
// target is pulled from the partner's side and the partner gets a mUnk_45 frame cooldown. Vtable 0228af9c.
class GimmickHaosWall : public Gimmick { // 02274d78
public:
    /* 45 */ u8 mUnk_45;
    /* 46 */ u8 mUnk_46;
    /* 47 */ PAD(0x47, 0x48);
    /* 48 */

    GimmickHaosWall(u8 id, u8 param);
    virtual ~GimmickHaosWall();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void func_ov007_02274fac(u8 cooldown); // starts the cooldown (state 2)
};
// Speech bubble of the Aquos field (aqousfukidasi_n / _b): pushes the target up while it is near and counts the
// near bubbles in a static counter for the looping sound 0x63. Vtable 0228b16c.
class GimmickAquosFukidashi : public Gimmick { // 02275de8
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ u16 mCounter;
    /* 4e */ u16 mUnk_4e;
    /* 50 */ u8 mUnk_50;
    /* 51 */ u8 mUnk_51;
    /* 52 */ u8 mUnk_52;
    /* 53 */ PAD(0x53, 0x54);
    /* 54 */

    GimmickAquosFukidashi(u8 id);
    virtual ~GimmickAquosFukidashi();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    void func_ov007_02276634();
    void func_ov007_022766c4(int drawB);
    int func_ov007_02276710(int frames, int state);
    int func_ov007_02276760(); // TRUE when the target is within 0x96000 of the bubble
};
// Lightning strikes (ikazuchistart / ikaz00): at most two strike at a time (static counter), each waits one of
// the random mDelays between strikes and hits the target when it is in range. Vtable 0228b528.
class GimmickIkazuchiStart : public Gimmick { // 02278270
public:
    /* 45 */ PAD(0x45, 0x48);
    /* 48 */ ModelInstance *mModelB;
    /* 4c */ u16 mCounter;
    /* 4e */ u16 mDelays[8];
    /* 5e */ u8 mUnk_5e;
    /* 5f */ u8 mUnk_5f;
    /* 60 */

    GimmickIkazuchiStart(u8 id);
    virtual ~GimmickIkazuchiStart();
    /* 08 */ virtual void Update();
    /* 0c */ virtual void Load();
    /* 10 */ virtual void Unload();
    /* 14 */ virtual void Finish();

    int func_ov007_022789e0(int count); // draws; with `count` also counts mCounter down, TRUE when it hits 0
};
