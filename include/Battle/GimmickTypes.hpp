#pragma once

#include "Battle/Gimmick.hpp"
#include "Game/UnkStruct_020d433c.hpp"
#include "Graphics/ModelResource.hpp"
#include <nitro/fx.h>

// Functions of other battle files / ITCM used by the gimmicks.
void func_ov007_022719c8(void *buf, u8 id);
GimmickSetData *func_ov007_02271ca8(UnkStruct_020d433c_0c *data, int count, void *buf, VecFx32 *pos, u16 *rot, u8 unk);
void func_ov007_02271d78(UnkStruct_020d433c_0c *data, GimmickSetData *set);
void func_01ff8e70(const VecFx32 *a, const VecFx32 *b, VecFx32 *out); // a - b
void func_01ff8e3c(const VecFx32 *a, const VecFx32 *b, VecFx32 *out); // a + b
void func_01ff8fc4(const VecFx32 *a, VecFx32 *out);                   // normalize
fx32 func_01ff8f64(const VecFx32 *v);                                 // length
extern MtxFx43 data_020c1cf4;

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
void func_01ff8c94(VecFx32 *vec, const MtxFx43 *m, VecFx32 *out); // m * vec
void func_01ffd088(MtxFx43 *m, u16 angle);                       // rotate about X
void func_01ff89c0(MtxFx43 *a, MtxFx43 *b, MtxFx43 *out);        // a * b
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
void func_01ff84d0(VecFx32 *vec, MtxFx33 *m, VecFx32 *out);
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
class GimmickFieldPowerS : public Gimmick { // 0222eb1c
public:
    /* 45 */ PAD(0x45, 0x48);
    GimmickFieldPowerS(u8 id, u8 variant);
    virtual ~GimmickFieldPowerS();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
class GimmickTornado : public Gimmick { // 0222f1e8
public:
    /* 45 */ PAD(0x45, 0x48);
    GimmickTornado(u8 id, u8 variant);
    virtual ~GimmickTornado();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
class GimmickVentKaze : public Gimmick { // 0222f480
public:
    /* 45 */ PAD(0x45, 0x48);
    GimmickVentKaze(u8 id);
    virtual ~GimmickVentKaze();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
class GimmickPyrusEruption : public Gimmick { // 0222f8fc
public:
    /* 45 */ PAD(0x45, 0x50);
    GimmickPyrusEruption(u8 id);
    virtual ~GimmickPyrusEruption();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
class GimmickAbilityField : public Gimmick { // 022300dc
public:
    /* 45 */ PAD(0x45, 0x50);
    GimmickAbilityField(u8 id, u8 param);
    virtual ~GimmickAbilityField();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
class GimmickUnk_022308a8 : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x58);
    GimmickUnk_022308a8(u8 id);
    virtual ~GimmickUnk_022308a8();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
class GimmickUnk_02230cec : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    GimmickUnk_02230cec(u8 id);
    virtual ~GimmickUnk_02230cec();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
class GimmickUnk_02274830 : public Gimmick {
public:
    /* 45 */ PAD(0x45, 0x48);
    GimmickUnk_02274830(u8 id);
    virtual ~GimmickUnk_02274830();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
class GimmickHaosWall : public Gimmick { // 02274d78
public:
    /* 45 */ PAD(0x45, 0x48);
    GimmickHaosWall(u8 id, u8 param);
    virtual ~GimmickHaosWall();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
class GimmickAquosFukidashi : public Gimmick { // 02275de8
public:
    /* 45 */ PAD(0x45, 0x54);
    GimmickAquosFukidashi(u8 id);
    virtual ~GimmickAquosFukidashi();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
class GimmickIkazuchiStart : public Gimmick { // 02278270
public:
    /* 45 */ PAD(0x45, 0x60);
    GimmickIkazuchiStart(u8 id);
    virtual ~GimmickIkazuchiStart();
    virtual void Update();
    virtual void Load();
    virtual void Unload();
    virtual void Finish();
};
