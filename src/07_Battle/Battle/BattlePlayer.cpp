#include "System/InputManager.hpp"
#include "System/WirelessManager.hpp"
#include "Text/TextManager.hpp"
#include "global.h"
#include "types.h"
#include "Battle/BattleCpu.hpp"
#include "Game/GameData.hpp"
#include "Game/SaveData.hpp"
#include "Game/Random.hpp"
#include "Unknown/UnkFunctions.hpp"
#include <nitro/mi.h>
#include "System/FileSystem.hpp"
#include <nitro/os.h>
#include <nitro/math.h>

struct UnkStruct_020c4230 {
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u16 mUnk_20;
};
extern UnkStruct_020c4230 data_020c4230;
extern u8 data_020d43eb[];
extern "C" void MIi_CpuCopy32(const void *src, void *dst, u32 size);
extern "C" void MIi_CpuCopy16(const void *src, void *dst, u32 size);
extern u8 data_020d441a[];
extern u8 data_020d4352[];

void InitPlayerName(BattlePlayer *player);

struct BattlePlayerTemplate {
    /* 00 */ u16 mUnk_00;
    /* 02 */ u16 mUnk_02;
    /* 04 */ u16 mUnk_04;
    /* 06 */ PAD(0x06, 0x08);
    /* 08 */ u8 mBakuganId;
    /* 09 */ u8 mAttribute;
    /* 0a */ PAD(0x0a, 0x0c);
};
struct BattlePlayerTemplateSet {
    /* 00 */ BattlePlayerTemplate mRecords[3];
    /* 24 */ u16 mSlots54Values[3][2]; // [k][0] copied to mSlots54[k].mValue; [k][1] is never read
    /* 30 */ u16 mSlots6cValues[3][2];
    /* 3c */
};
const BattlePlayerTemplateSet sPlayerTemplates[17][3] = {
    {
        {{{0x0, 0x2243, 0x25, {0}, 0x0, 0x0, {0}}, {0x0, 0x8, 0x14, {0}, 0x15, 0x0, {0}}, {0x0, 0x1002, 0x15, {0}, 0x17, 0x0, {0}}}, {{21, 0}, {52, 0}, {72, 0}}, {{126, 0}, {141, 0}, {169, 0}}},
        {{{0x5, 0x224b, 0x37, {0}, 0x0, 0x0, {0}}, {0x3, 0x248, 0x28, {0}, 0x1f, 0x0, {0}}, {0x3, 0x1440, 0x28, {0}, 0x1d, 0x0, {0}}}, {{1, 0}, {40, 0}, {73, 0}}, {{112, 0}, {146, 0}, {181, 0}}},
        {{{0x3, 0x2693, 0x3c, {0}, 0x1, 0x0, {0}}, {0x9, 0x24d2, 0x2e, {0}, 0x1e, 0x0, {0}}, {0x9, 0x188c, 0x2c, {0}, 0x1c, 0x0, {0}}}, {{2, 0}, {55, 0}, {88, 0}}, {{128, 0}, {147, 0}, {183, 0}}},
    },
    {
        {{{0x0, 0x220a, 0x22, {0}, 0x2, 0x3, {0}}, {0x0, 0x200, 0xc, {0}, 0x13, 0x3, {0}}, {0x0, 0x8, 0xb, {0}, 0x15, 0x3, {0}}}, {{19, 0}, {41, 0}, {86, 0}}, {{107, 0}, {140, 0}, {186, 0}}},
        {{{0x2, 0x228c, 0x24, {0}, 0x2, 0x3, {0}}, {0x1, 0x201, 0x1a, {0}, 0x1b, 0x3, {0}}, {0x1, 0x1048, 0x17, {0}, 0x19, 0x3, {0}}}, {{3, 0}, {48, 0}, {95, 0}}, {{115, 0}, {148, 0}, {177, 0}}},
        {{{0x2, 0x268c, 0x38, {0}, 0x3, 0x3, {0}}, {0x9, 0x424c, 0x2b, {0}, 0x1e, 0x3, {0}}, {0x9, 0x911, 0x2d, {0}, 0x23, 0x3, {0}}}, {{4, 0}, {42, 0}, {87, 0}}, {{131, 0}, {149, 0}, {187, 0}}},
    },
    {
        {{{0x0, 0x2d1, 0x22, {0}, 0x4, 0x1, {0}}, {0x0, 0x40, 0xf, {0}, 0x14, 0x1, {0}}, {0x0, 0x200, 0xa, {0}, 0x13, 0x1, {0}}}, {{20, 0}, {56, 0}, {90, 0}}, {{125, 0}, {140, 0}, {186, 0}}},
        {{{0x5, 0x6d4, 0x24, {0}, 0x4, 0x1, {0}}, {0x1, 0x208, 0x17, {0}, 0x19, 0x1, {0}}, {0x1, 0x1008, 0x16, {0}, 0x1c, 0x1, {0}}}, {{5, 0}, {43, 0}, {92, 0}}, {{129, 0}, {150, 0}, {188, 0}}},
        {{{0x5, 0x26d3, 0x35, {0}, 0x5, 0x1, {0}}, {0x8, 0x44a2, 0x35, {0}, 0x4, 0x1, {0}}, {0x9, 0x151a, 0x28, {0}, 0x1e, 0x1, {0}}}, {{6, 0}, {43, 0}, {79, 0}}, {{135, 0}, {166, 0}, {188, 0}}},
    },
    {
        {{{0x0, 0x1099, 0x25, {0}, 0x9, 0x5, {0}}, {0x0, 0x1002, 0x12, {0}, 0x17, 0x5, {0}}, {0x0, 0x8, 0xf, {0}, 0x15, 0x5, {0}}}, {{23, 0}, {63, 0}, {79, 0}}, {{109, 0}, {141, 0}, {170, 0}}},
        {{{0x4, 0x149c, 0x31, {0}, 0x9, 0x5, {0}}, {0x1, 0x248, 0x19, {0}, 0x1f, 0x5, {0}}, {0x1, 0x49, 0x17, {0}, 0x1c, 0x5, {0}}}, {{9, 0}, {52, 0}, {83, 0}}, {{117, 0}, {153, 0}, {176, 0}}},
        {{{0x3, 0x189b, 0x3b, {0}, 0xa, 0x5, {0}}, {0x9, 0x3912, 0x30, {0}, 0x23, 0x5, {0}}, {0x9, 0x24e2, 0x28, {0}, 0x1d, 0x5, {0}}}, {{10, 0}, {46, 0}, {74, 0}}, {{133, 0}, {154, 0}, {182, 0}}},
    },
    {
        {{{0x0, 0x682, 0x26, {0}, 0x7, 0x2, {0}}, {0x0, 0x8, 0xe, {0}, 0x15, 0x2, {0}}, {0x0, 0x1000, 0xe, {0}, 0x18, 0x2, {0}}}, {{21, 0}, {41, 0}, {93, 0}}, {{124, 0}, {139, 0}, {179, 0}}},
        {{{0x2, 0x168a, 0x2f, {0}, 0x7, 0x2, {0}}, {0x1, 0x201, 0x14, {0}, 0x1b, 0x2, {0}}, {0x1, 0x48, 0x17, {0}, 0x1f, 0x2, {0}}}, {{7, 0}, {59, 0}, {77, 0}}, {{127, 0}, {151, 0}, {175, 0}}},
        {{{0x5, 0x2713, 0x3a, {0}, 0x8, 0x2, {0}}, {0x9, 0x424b, 0x28, {0}, 0x1e, 0x2, {0}}, {0x9, 0x14db, 0x27, {0}, 0x1d, 0x2, {0}}}, {{8, 0}, {48, 0}, {76, 0}}, {{114, 0}, {152, 0}, {172, 0}}},
    },
    {
        {{{0x5, 0x1861, 0xf, {0}, 0x7, 0x2, {0}}, {0x5, 0x36db, 0x12, {0}, 0x15, 0x0, {0}}, {0x5, 0x428a, 0xa, {0}, 0x13, 0x1, {0}}}, {{3, 0}, {44, 0}, {77, 0}}, {{107, 0}, {148, 0}, {177, 0}}},
        {{{0x5, 0x1861, 0x19, {0}, 0x7, 0x2, {0}}, {0x5, 0x36db, 0x1c, {0}, 0x15, 0x0, {0}}, {0x5, 0x428a, 0x14, {0}, 0x13, 0x1, {0}}}, {{3, 0}, {44, 0}, {77, 0}}, {{107, 0}, {148, 0}, {177, 0}}},
        {{{0x5, 0x1861, 0x23, {0}, 0x7, 0x2, {0}}, {0x5, 0x36db, 0x26, {0}, 0x15, 0x0, {0}}, {0x5, 0x428a, 0x1e, {0}, 0x13, 0x1, {0}}}, {{3, 0}, {44, 0}, {77, 0}}, {{107, 0}, {148, 0}, {177, 0}}},
    },
    {
        {{{0x2, 0x22d2, 0x25, {0}, 0xd, 0x4, {0}}, {0x0, 0x1002, 0x19, {0}, 0x17, 0x4, {0}}, {0x0, 0x1, 0x1b, {0}, 0x16, 0x4, {0}}}, {{23, 0}, {44, 0}, {75, 0}}, {{127, 0}, {167, 0}, {168, 0}}},
        {{{0x4, 0x151a, 0x37, {0}, 0xb, 0x4, {0}}, {0x1, 0x208, 0x25, {0}, 0x19, 0x4, {0}}, {0x1, 0x1400, 0x26, {0}, 0x1d, 0x4, {0}}}, {{11, 0}, {69, 0}, {77, 0}}, {{116, 0}, {155, 0}, {180, 0}}},
        {{{0x7, 0x38e4, 0x3e, {0}, 0xc, 0x4, {0}}, {0x9, 0x425a, 0x2e, {0}, 0x1e, 0x4, {0}}, {0x9, 0x8cc, 0x32, {0}, 0x1a, 0x4, {0}}}, {{12, 0}, {61, 0}, {77, 0}}, {{135, 0}, {156, 0}, {188, 0}}},
    },
    {
        {{{0x3, 0x4619, 0x3c, {0}, 0x26, 0x4, {0}}, {0x0, 0x2452, 0x24, {0}, 0x14, 0x4, {0}}, {0x0, 0x1041, 0x25, {0}, 0x18, 0x4, {0}}}, {{38, 0}, {70, 0}, {100, 0}}, {{126, 0}, {164, 0}, {180, 0}}},
        {{{0x8, 0x491c, 0x43, {0}, 0x27, 0x4, {0}}, {0x4, 0x3893, 0x34, {0}, 0x1a, 0x4, {0}}, {0x4, 0x36db, 0x35, {0}, 0x1d, 0x4, {0}}}, {{39, 0}, {53, 0}, {78, 0}}, {{116, 0}, {165, 0}, {173, 0}}},
        {{{0x9, 0x491c, 0x52, {0}, 0x27, 0x4, {0}}, {0x9, 0x4923, 0x3b, {0}, 0xd, 0x4, {0}}, {0x9, 0x48e4, 0x35, {0}, 0x1a, 0x4, {0}}}, {{39, 0}, {51, 0}, {92, 0}}, {{132, 0}, {165, 0}, {180, 0}}},
    },
    {
        {{{0x0, 0x2641, 0x28, {0}, 0xe, 0x2, {0}}, {0x0, 0x200, 0xd, {0}, 0x13, 0x2, {0}}, {0x0, 0x1002, 0xc, {0}, 0x17, 0x2, {0}}}, {{19, 0}, {45, 0}, {77, 0}}, {{118, 0}, {136, 0}, {168, 0}}},
        {{{0x4, 0x2693, 0x2d, {0}, 0xe, 0x2, {0}}, {0x1, 0x49, 0x16, {0}, 0x1f, 0x2, {0}}, {0x1, 0x209, 0x16, {0}, 0x19, 0x2, {0}}}, {{14, 0}, {47, 0}, {95, 0}}, {{106, 0}, {157, 0}, {175, 0}}},
        {{{0x9, 0x36db, 0x34, {0}, 0xe, 0x2, {0}}, {0x9, 0x149c, 0x21, {0}, 0x1d, 0x2, {0}}, {0x9, 0x2313, 0x23, {0}, 0x1e, 0x2, {0}}}, {{14, 0}, {41, 0}, {97, 0}}, {{127, 0}, {157, 0}, {172, 0}}},
    },
    {
        {{{0x0, 0x16c8, 0x25, {0}, 0xf, 0x1, {0}}, {0x0, 0x200, 0x14, {0}, 0x15, 0x1, {0}}, {0x0, 0x1, 0x17, {0}, 0x16, 0x1, {0}}}, {{21, 0}, {45, 0}, {90, 0}}, {{126, 0}, {141, 0}, {178, 0}}},
        {{{0x4, 0x26cc, 0x2d, {0}, 0xf, 0x1, {0}}, {0x1, 0x9, 0x1c, {0}, 0x1b, 0x1, {0}}, {0x1, 0x1240, 0x1a, {0}, 0x1f, 0x1, {0}}}, {{15, 0}, {58, 0}, {101, 0}}, {{113, 0}, {158, 0}, {188, 0}}},
        {{{0x9, 0x191c, 0x34, {0}, 0xf, 0x1, {0}}, {0x9, 0x465a, 0x29, {0}, 0x1e, 0x1, {0}}, {0x9, 0x2712, 0x29, {0}, 0x1c, 0x1, {0}}}, {{15, 0}, {64, 0}, {92, 0}}, {{121, 0}, {158, 0}, {174, 0}}},
    },
    {
        {{{0x0, 0x324a, 0x26, {0}, 0x10, 0x0, {0}}, {0x0, 0x1000, 0x10, {0}, 0x18, 0x0, {0}}, {0x0, 0x40, 0x10, {0}, 0x14, 0x0, {0}}}, {{24, 0}, {40, 0}, {72, 0}}, {{110, 0}, {141, 0}, {171, 0}}},
        {{{0x7, 0x36da, 0x2c, {0}, 0x10, 0x0, {0}}, {0x1, 0x1208, 0x18, {0}, 0x1c, 0x0, {0}}, {0x1, 0x1008, 0x19, {0}, 0x1b, 0x0, {0}}}, {{16, 0}, {55, 0}, {81, 0}}, {{134, 0}, {159, 0}, {181, 0}}},
        {{{0x9, 0x36e3, 0x34, {0}, 0x10, 0x0, {0}}, {0x9, 0x38da, 0x28, {0}, 0x23, 0x0, {0}}, {0x9, 0x1514, 0x26, {0}, 0x1d, 0x0, {0}}}, {{16, 0}, {57, 0}, {94, 0}}, {{120, 0}, {159, 0}, {183, 0}}},
    },
    {
        {{{0x0, 0x282, 0x23, {0}, 0x11, 0x3, {0}}, {0x0, 0x1, 0x10, {0}, 0x16, 0x3, {0}}, {0x0, 0x40, 0xf, {0}, 0x15, 0x3, {0}}}, {{22, 0}, {48, 0}, {91, 0}}, {{126, 0}, {137, 0}, {168, 0}}},
        {{{0x5, 0x2512, 0x29, {0}, 0x11, 0x3, {0}}, {0x1, 0x1440, 0x17, {0}, 0x1d, 0x3, {0}}, {0x1, 0x1009, 0x15, {0}, 0x1c, 0x3, {0}}}, {{17, 0}, {51, 0}, {89, 0}}, {{111, 0}, {160, 0}, {177, 0}}},
        {{{0x9, 0x44e2, 0x34, {0}, 0x11, 0x3, {0}}, {0x9, 0x269b, 0x24, {0}, 0x1e, 0x3, {0}}, {0x9, 0x3912, 0x23, {0}, 0x23, 0x3, {0}}}, {{17, 0}, {42, 0}, {96, 0}}, {{125, 0}, {160, 0}, {187, 0}}},
    },
    {
        {{{0x0, 0x101b, 0x24, {0}, 0x12, 0x5, {0}}, {0x0, 0x1000, 0xe, {0}, 0x18, 0x5, {0}}, {0x0, 0x40, 0xd, {0}, 0x14, 0x5, {0}}}, {{24, 0}, {50, 0}, {77, 0}}, {{110, 0}, {141, 0}, {182, 0}}},
        {{{0x2, 0x125b, 0x28, {0}, 0x12, 0x5, {0}}, {0x1, 0x100a, 0x16, {0}, 0x1c, 0x5, {0}}, {0x1, 0x20a, 0x15, {0}, 0x19, 0x5, {0}}}, {{18, 0}, {50, 0}, {77, 0}}, {{117, 0}, {161, 0}, {189, 0}}},
        {{{0x9, 0x389b, 0x34, {0}, 0x12, 0x5, {0}}, {0x9, 0x248c, 0x22, {0}, 0x23, 0x5, {0}}, {0x9, 0x249a, 0x26, {0}, 0x1d, 0x5, {0}}}, {{18, 0}, {71, 0}, {99, 0}}, {{120, 0}, {161, 0}, {176, 0}}},
    },
    {
        {{{0x0, 0x40, 0x14, {0}, 0x1c, 0x2, {0}}, {0x0, 0x8, 0xb, {0}, 0x15, 0x2, {0}}, {0x0, 0x8, 0xa, {0}, 0x16, 0x2, {0}}}, {{21, 0}, {59, 0}, {93, 0}}, {{126, 0}, {140, 0}, {177, 0}}},
        {{{0x4, 0x128a, 0x1e, {0}, 0x1c, 0x0, {0}}, {0x1, 0x1042, 0x16, {0}, 0x17, 0x0, {0}}, {0x1, 0x1040, 0x17, {0}, 0x18, 0x0, {0}}}, {{28, 0}, {55, 0}, {72, 0}}, {{127, 0}, {140, 0}, {181, 0}}},
        {{{0x9, 0x34dc, 0x2a, {0}, 0x1c, 0x4, {0}}, {0x7, 0x168b, 0x22, {0}, 0x1d, 0x4, {0}}, {0x7, 0x42a3, 0x20, {0}, 0x1e, 0x4, {0}}}, {{28, 0}, {70, 0}, {75, 0}}, {{134, 0}, {141, 0}, {196, 0}}},
    },
    {
        {{{0x0, 0x200, 0x12, {0}, 0x13, 0x2, {0}}, {0x0, 0x1000, 0xa, {0}, 0x18, 0x2, {0}}, {0x0, 0x8, 0xa, {0}, 0x16, 0x2, {0}}}, {{24, 0}, {45, 0}, {77, 0}}, {{127, 0}, {140, 0}, {183, 0}}},
        {{{0x2, 0x1201, 0x1c, {0}, 0x1b, 0x2, {0}}, {0x1, 0x1208, 0x14, {0}, 0x19, 0x2, {0}}, {0x1, 0x1042, 0x13, {0}, 0x17, 0x2, {0}}}, {{27, 0}, {59, 0}, {97, 0}}, {{106, 0}, {141, 0}, {172, 0}}},
        {{{0x9, 0x249b, 0x28, {0}, 0x1e, 0x2, {0}}, {0x6, 0x264a, 0x23, {0}, 0x1f, 0x2, {0}}, {0x5, 0x1293, 0x20, {0}, 0x1c, 0x2, {0}}}, {{30, 0}, {67, 0}, {80, 0}}, {{130, 0}, {141, 0}, {194, 0}}},
    },
    {
        {{{0x0, 0x40, 0x16, {0}, 0x1b, 0x2, {0}}, {0x0, 0x8, 0xc, {0}, 0x15, 0x2, {0}}, {0x0, 0x200, 0xa, {0}, 0x13, 0x2, {0}}}, {{21, 0}, {45, 0}, {76, 0}}, {{124, 0}, {140, 0}, {169, 0}}},
        {{{0x3, 0x124a, 0x1b, {0}, 0x1b, 0x2, {0}}, {0x1, 0x248, 0x14, {0}, 0x19, 0x2, {0}}, {0x1, 0x201, 0x16, {0}, 0x1e, 0x2, {0}}}, {{27, 0}, {41, 0}, {77, 0}}, {{126, 0}, {141, 0}, {189, 0}}},
        {{{0x9, 0x268c, 0x29, {0}, 0x1b, 0x2, {0}}, {0x9, 0x3493, 0x22, {0}, 0x1d, 0x2, {0}}, {0x7, 0x149a, 0x22, {0}, 0x1c, 0x2, {0}}}, {{27, 0}, {67, 0}, {80, 0}}, {{106, 0}, {141, 0}, {194, 0}}},
    },
    {
        {{{0x0, 0x40, 0x15, {0}, 0x1d, 0x1, {0}}, {0x0, 0x8, 0xc, {0}, 0x17, 0x1, {0}}, {0x0, 0x200, 0xd, {0}, 0x18, 0x1, {0}}}, {{23, 0}, {43, 0}, {101, 0}}, {{122, 0}, {140, 0}, {175, 0}}},
        {{{0x3, 0x124a, 0x1c, {0}, 0x1d, 0x1, {0}}, {0x1, 0x1200, 0x18, {0}, 0x1b, 0x1, {0}}, {0x1, 0x211, 0x16, {0}, 0x19, 0x1, {0}}}, {{29, 0}, {58, 0}, {82, 0}}, {{113, 0}, {141, 0}, {172, 0}}},
        {{{0x9, 0x36d4, 0x2c, {0}, 0x1d, 0x1, {0}}, {0x7, 0x428a, 0x22, {0}, 0x1e, 0x1, {0}}, {0x8, 0x1494, 0x20, {0}, 0x1c, 0x1, {0}}}, {{29, 0}, {56, 0}, {78, 0}}, {{129, 0}, {141, 0}, {197, 0}}},
    },
};
// Placeholder for an unidentified object. The original's data layout needs one more participant in this file's
// size sort, after the template table: a small weak object that no instruction addresses. This accessor's static
// joins the sort here, its call leaves no code, and the link strips it.
inline u32 *DataLayoutSlot() {
    static u32 sSlot;
    return &sSlot;
}
extern u8 data_020d43df[];
extern u8 data_020d43db[];
extern u8 data_020d43d7[];
extern u8 data_020d43e3[];
s32 GetSlotBakuganId(SaveData *base, u32 unk1, u32 unk2);

BattlePlayer::BattlePlayer(u32 index)
    : SceneBase(2, 0) {
    (void) DataLayoutSlot();
    mPushSumX = 0;
    mPushSumZ = 0;
    mMicCharge = 0;
    mUnk_ee = 0;
    mState = 0;
    mSpGauge = 0;
    mHistoryCount = 0;
    mRecord = 0;
    mAbilityId = 0;
    mFieldPowerSHitCount = 0;
    mFieldPowerGHitCount = 0;
    mUnk_fe = 0;
    MI_CpuFill8((u8 *) this + 0xc, 0, 0x78);
    MI_CpuFill8((u8 *) this + 0x84, 0, 0x24);
    MI_CpuFill8((u8 *) this + 0xa8, 0, 8);
    mIndex = index;
    mPartner = gGameData.mHasPartners != 0 ? data_020d43d7[mIndex] : mIndex;
    mCpu = 0;
    u32 limit = 1;
    WirelessManager::Get();
    if (IsWirelessMode(&data_020c4258)) {
        limit = WirelessManager::Get().GetRecvBlockCount();
    }
    if (limit <= index) {
        mCpu = new BattleCpu(index);
        mState = 2;
    }
    mCharacterLevel = data_020d43df[index];
    mCharacterId = data_020d43db[index];
    SaveData *save = gSaveData;
    mSlots54[0].mValue = save->mSlots54Values[0];
    mSlots54[1].mValue = save->mSlots54Values[1];
    mSlots54[2].mValue = save->mSlots54Values[2];
    mSlots6c[0].mValue = save->mSlots6cValues[0];
    mSlots6c[1].mValue = save->mSlots6cValues[1];
    mSlots6c[2].mValue = save->mSlots6cValues[2];
    if (mCharacterId == 0x15) {
        u8 k = 0;
        do {
            mRecords[k].mAttribute = GetSlotBakuganAttribute(save, 0, k);
            mRecords[k].mBakuganId = GetSlotBakuganId(save, 0, k);
            mRecords[k].mUnk_04 = func_020231f4(save, 0, mRecords[k].mBakuganId, mRecords[k].mAttribute);
            mRecords[k].mUnk_0b = mRecords[k].mUnk_04 / 10;
            mRecords[k].mHead = *(BattleRecordHead *) GetBakuganEntry(save, mRecords[k].mBakuganId, mRecords[k].mAttribute);
            k++;
        } while (k < 3);
    }
    if (mCharacterId <= 0x10) {
        if (mCharacterId > 0x10) {
            mCharacterId = 0;
        }
        u8 k = 0;
        do {
            u32 extra = 0;
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4258)) {
                int n = gGameData.mRecvBlockIndices[index];
                if (data_020d43e3[(u32) n >> 3] & (1 << (n % 8))) {
                    extra = 0x64;
                }
            } else if (gSaveData[0].mCreditsDone) {
                extra = 0x64;
            }
            mRecords[k].mUnk_04 = sPlayerTemplates[mCharacterId][mCharacterLevel].mRecords[k].mUnk_04 * 10 + extra;
            mRecords[k].mUnk_0b = mRecords[k].mUnk_04 / 10;
            mRecords[k].mAttribute = sPlayerTemplates[mCharacterId][mCharacterLevel].mRecords[k].mAttribute;
            mRecords[k].mBakuganId = sPlayerTemplates[mCharacterId][mCharacterLevel].mRecords[k].mBakuganId;
            mRecords[k].mHead = *(BattleRecordHead *) &sPlayerTemplates[mCharacterId][mCharacterLevel].mRecords[k];
            mSlots54[k].mValue = sPlayerTemplates[mCharacterId][mCharacterLevel].mSlots54Values[k][0];
            mSlots6c[k].mValue = sPlayerTemplates[mCharacterId][mCharacterLevel].mSlots6cValues[k][0];
            k++;
        } while (k < 3);
    }
    if (gGameData.mPlayerCount == 2) {
        switch (index) {
            case 0:
                mCorner = 0;
                break;
            case 1:
                mCorner = 2;
                break;
        }
    } else {
        switch (index) {
            case 0:
                mCorner = 0;
                break;
            case 1:
                mCorner = 1;
                break;
            case 2:
                mCorner = 2;
                break;
            case 3:
                mCorner = 3;
                break;
        }
    }
    mSlots54Remaining = gGameData.mSlotsPerPlayer;
    mSlots6cRemaining = gGameData.mSlotsPerPlayer;
    MI_CpuClear32((u8 *) this + 0x100, 0x20);
    MI_CpuClear32((u8 *) this + 0x120, 0x20);
    InitPlayerName(this);
    if (gGameData.mLastBattle != 0) {
        mRecords[1].mStatus = 2;
        mRecords[2].mStatus = 2;
    }
}

BattlePlayer::~BattlePlayer() {
    if (mCpu) {
        delete mCpu;
    }
}

void BattlePlayer::Update() {
    if (gGameData.mFlags & 1) {
        mFinished = 1;
        return;
    }
    switch (mState) {
        case 0:
            mState = 1;
            break;
        case 1:
            WirelessManager::Get();
            if (IsWirelessMode(&data_020c4030.mUnk_228)) {
                WirelessManager::Get();
                if (data_020c4230.mUnk_20 == data_020d43eb[mIndex]) {
                    WirelessManager::Get().SetSendBlock(&gSendMessage);
                    gSendMessage.mType = 0;
                }
                u8 *received = WirelessManager::Get().GetRecvBlock(data_020d43eb[mIndex]);
                if (!received) {
                    mMsg.mType = 0;
                } else {
                    MIi_CpuCopy32(received, &mMsg, 0x20);
                }
                if (mMsg.mType != 0) {
                    MIi_CpuCopy32(&mMsg, (u8 *) this + 0x120, 0x20);
                }
            }
            InputManager::Get();
            break;
        case 2:
            break;
        case 3:
            mFinished = 1;
            break;
    }
}

void func_ov007_02269ee4(BattlePlayer *player, u32 reset) {
    u8 i = 0;
    BOOL hasZero;
    u8 n;
    u8 ones;
    player->mPushSumX = 0;
    player->mPushSumZ = 0;
    n       = gGameData.mSlotsPerPlayer;
    hasZero = 0;
    ones    = 0;
    do {
        u8 state = player->mRecords[i].mStatus;
        if (state == 0) {
            hasZero = 1;
            break;
        }
        if (state == 1) {
            ones++;
        }
        i++;
    } while (i < n);
    gGameData.mBattlePlayers->mUnk_292 = 0;
    if (!hasZero) {
        if (gGameData.mLastBattle != 0) {
            if (ones == 1) {
                gGameData.mBattlePlayers->mUnk_292 = 1;
            } else {
                i = 0;
                do {
                    if (player->mRecords[i].mStatus == 2) {
                        player->mRecords[i].mStatus = 0;
                    }
                    i++;
                } while (i < gGameData.mSlotsPerPlayer);
            }
        } else {
            if (ones == gGameData.mSlotsPerPlayer) {
                gGameData.mBattlePlayers->mUnk_292 = 1;
            } else {
                i = 0;
                do {
                    if (player->mRecords[i].mStatus == 2) {
                        player->mRecords[i].mStatus = 0;
                    }
                    i++;
                } while (i < gGameData.mSlotsPerPlayer);
            }
        }
    }
    if (player->mSlots54Remaining == 0) {
        gGameData.mBattlePlayers->mAddMarkerEnabled = 0;
    }
    if (player->mCpu) {
        player->mCpu->func_ov007_02231038();
    }
    player->mAbilityId = 0;
    if (reset == 0) {
        u32 flagA = 0;
        u8 best  = 0;
        if (gGameData.mHasPartners) {
            BattlePlayerList *list = gGameData.mBattlePlayers;
            u8 mine                = player->mUnk_ee;
            u8 total               = mine + list->mPlayers[player->mPartner]->mUnk_ee;
            u8 sum                 = 0;
            i                      = 0;
            do {
                u8 partner = list->mPlayers[i]->mPartner;
                if (i != player->mIndex && partner != player->mIndex) {
                    sum = gGameData.mBattlePlayers->mPlayers[i]->mUnk_ee + gGameData.mBattlePlayers->mPlayers[partner]->mUnk_ee;
                    break;
                }
                i++;
            } while (i < gGameData.mPlayerCount);
            s8 diff = sum - total;
            if (sum == 2) {
                flagA = 1;
            }
            if (diff > 0) {
                best = (u8) diff;
            }
        } else {
            BattlePlayerList *list = gGameData.mBattlePlayers;
            int top                = gGameData.mSlotsPerPlayer - 1;
            u8 mine                = player->mUnk_ee;
            i                      = 0;
            do {
                u8 level = list->mPlayers[i]->mUnk_ee;
                if (top == level) {
                    if (i != player->mIndex) {
                        flagA = 1;
                    }
                }
                s8 diff = level - mine;
                if (best < diff) {
                    best = (u8) diff;
                }
                i++;
            } while (i < gGameData.mPlayerCount);
        }
        u8 gain = best * 0x28 + 0x1e + flagA * 0x19;
        if (player->mSpGauge + gain < 0xbe) {
            player->mSpGauge += gain;
        } else {
            player->mSpGauge = 0xbe;
        }
    }
    if (gGameData.mLastBattle != 0) {
        player->mRecords[1].mStatus = 2;
        player->mRecords[2].mStatus = 2;
        if (player->mIndex != 0) {
            if (player->mSpGauge + 0xbe < 0xbe) {
                player->mSpGauge += 0xbe;
            } else {
                player->mSpGauge = 0xbe;
            }
        }
        gGameData.mBattlePlayers->mAddMarkerEnabled = 0;
    }
}

void InitPlayerName(BattlePlayer *player) {
    if (player->mCharacterId == 0x15) {
        WirelessManager::Get();
        const u16 *name;
        if (!IsWirelessMode(&data_020c4030.mUnk_228)) {
            name = (const u16 *) data_020d441a;
        } else {
            name = (const u16 *) (data_020d4352 + data_020d43eb[player->mIndex] * 0x18);
        }
        u8 length = 0;
        do {
            if (name[length] == 0) {
                break;
            }
            length++;
        } while (length < 12);
        if (length == 0) {
            length = 1;
        }
        MIi_CpuCopy16(name, (u8 *) player + 0xb0, length * sizeof(u16));
        player->mNameLength = length * 2;
    } else {
        MessageFile *file = (MessageFile *) LoadFile("/font/mes_StoryCharaName.mes", 0, 0, 0, 0, 0);
        player->mNameLength   = file->GetLength(player->mCharacterId);
        TextManager::Get();
        MIi_CpuCopy16(file->GetText(player->mCharacterId), (u8 *) player + 0xb0, player->mNameLength);
        OS_Free(file);
    }
}

struct BattleRecordWord {
    u16 mValue;
    u8 mPad[10];
};

void func_ov007_0226a380(BattlePlayer *player, u8 record, int amount) {
    if (amount > 0) {
        BattleRecordWord *words = (BattleRecordWord *) ((u8 *) player + 0x12);
        BattleRecordWord *own = (BattleRecordWord *) ((u8 *) player + 0x10);
        u16 value = words[record].mValue;
        u16 base  = own[record].mValue;
        words[record].mValue = value + amount;
        u16 limit = 0x3de - base;
        if (words[record].mValue > limit) {
            words[record].mValue = limit;
        }
    } else {
        BattleRecordWord *words = (BattleRecordWord *) ((u8 *) player + 0x12);
        u16 value = words[record].mValue;
        if ((s16) ((s16) value + amount) <= 0) {
            words[record].mValue = 0;
        } else {
            words[record].mValue = value + amount;
        }
    }
}

// Also keeps mSlots54Remaining (the slots with flag 0) in step.
void SetSlot54Flag(BattlePlayer *player, u32 index, u32 flag) {
    if (player->mSlots54[index].mFlag == 0) {
        if (flag != 0 && player->mSlots54Remaining != 0) {
            player->mSlots54Remaining--;
        }
    } else {
        if (flag == 0) {
            player->mSlots54Remaining++;
        }
    }
    player->mSlots54[index].mFlag = flag;
}

// Also keeps mSlots6cRemaining (the slots with flag 0) in step.
void SetSlot6cFlag(BattlePlayer *player, u8 index, int flag) {
    if (player->mSlots6c[index].mFlag == 0) {
        if (flag != 0 && player->mSlots6cRemaining != 0) {
            player->mSlots6cRemaining--;
        }
    } else {
        if (flag == 0) {
            player->mSlots6cRemaining++;
        }
    }
    player->mSlots6c[index].mFlag = flag;
}


// Every caller is in SetupTutorial's mTutorialId cases 7, 8, 10 and 14, so the mTutorialId == 1 branch does not run.
void SetupTutorialPlayer(BattlePlayer *player) {
    if (gGameData.mTutorialId != 0xe) {
        u8 two = 2;
        player->mSlots6c[0].mFlag = two;
        player->mSlots6c[1].mFlag = two;
        player->mSlots6c[2].mFlag = two;
        if (gGameData.mTutorialId == 1) {
            player->mSlots54[0].mFlag = 0;
            player->mSlots54[1].mFlag = two;
            player->mSlots54[2].mFlag = two;
        } else {
            player->mSlots54[0].mFlag = two;
            player->mSlots54[1].mFlag = two;
            player->mSlots54[2].mFlag = two;
        }
        if (player->mIndex != 0) {
            player->mRecords[0].mUnk_04 = 0xe6;
            player->mRecords[0].mUnk_0b = 0x17;
        } else {
            player->mRecords[0].mBakuganId = 0x16;
            player->mRecords[0].mUnk_04 = 0xbe;
            player->mRecords[0].mUnk_0b = 0x13;
            player->mRecords[0].mAttribute = 0;
        }
        player->mRecords[1].mStatus = 2;
        player->mRecords[2].mStatus = 2;
        player->mSlots54Remaining = 0;
    } else if (player->mIndex == 0) {
        player->mRecords[0].mBakuganId = 0x24;
        int v = func_020231f4(gSaveData, 0, 0x24, (u8) gSaveData[0].mAttribute);
        if (v == 0) {
            v = 300;
        }
        player->mRecords[0].mUnk_04 = v;
        player->mRecords[0].mUnk_0b = v / 10;
        player->mRecords[0].mAttribute = gSaveData[0].mAttribute;
        player->mSlots6c[0].mValue  = gSaveData[0].mAttribute + 0x68;
        player->mSlots6c[1].mValue  = 0x8c;
        player->mSlots6c[2].mValue  = 0xac;
        player->mSlots6c[1].mFlag   = 2;
        player->mSlots6c[2].mFlag   = 2;
    } else {
        player->mSlots54[0].mValue = 0x16;
        player->mSlots6c[0].mFlag  = 2;
        player->mSlots6c[1].mFlag  = 2;
        player->mSlots6c[2].mFlag  = 2;
    }
}

u8 PickUnflaggedSlot54(BattlePlayer *player) {
    BattlePlayerList *list = gGameData.mBattlePlayers;
    u8 first               = 0xff;
    u8 i                   = 0;
    u8 count               = 0;
    u8 j;
    BattlePlayer *owner    = list->mPlayers[player->mIndex];
    u8 n                   = gGameData.mSlotsPerPlayer;
    do {
        if (owner->mSlots54[i].mFlag == 0) {
            count++;
        }
        i++;
    } while (i < n);
    j = 0;
    do {
        if (owner->mSlots54[j].mFlag == 0) {
            first = j;
            break;
        }
        j++;
    } while (j < n);
    if (count != 0) {
        u8 pick              = MATH_Rand32(&gRandom, count);
        u8 k                 = 0;
        BattlePlayer *owner2 = list->mPlayers[player->mIndex];
        i                    = 0;
        do {
            if (owner2->mSlots54[i].mFlag == 0) {
                if (pick == k) {
                    first = i;
                    break;
                }
                k++;
            }
            i++;
        } while (i < n);
    }
    return first;
}

// Random index (below gGameData.mSlotsPerPlayer) of an mSlots6c entry with mFlag 0 whose bit in mask is clear; 0xff for
// none. Slot 1 with an mValue of 0x92..0xa7 is returned at once (or, at random, slot 0 when that one is free too).
// The pick gives 0xff instead for mValue 0x8c when x > y and for 0x8e..0x91 when y >= 0.
u8 PickUnflaggedSlot6c(BattlePlayer *player, u8 mask, s16 x, s16 y) {
    BattlePlayerList *list = gGameData.mBattlePlayers;
    u8 i = 0;
    u8 count = 0;
    BattlePlayer *owner = list->mPlayers[player->mIndex];
    u8 n = gGameData.mSlotsPerPlayer;
    u8 result = 0xff;
    do {
        if (owner->mSlots6c[i].mFlag == 0) {
            if (!TestFlagBit(&mask, i)) {
                count++;
            }
        }
        i++;
    } while (i < n);
    if (!(mask & 2) && owner->mSlots6c[1].mFlag == 0) {
        u16 value = owner->mSlots6c[1].mValue;
        if (value >= 0x92 && value <= 0xa7) {
            u8 ret = 1;
            if (!((mask & 1) || owner->mSlots6c[0].mFlag != 0)) {
                if (MATH_Rand32(&gRandom, 2) != 0) {
                    ret = 0;
                }
            }
            return ret;
        }
    }
    if (count != 0) {
        u8 pick = MATH_Rand32(&gRandom, count);
        owner = list->mPlayers[player->mIndex];
        u8 k = 0;
        i = 0;
        do {
            BattlePlayerSlot *slot = &owner->mSlots6c[i];
            if (slot->mFlag == 0 && !TestFlagBit(&mask, i)) {
                if (pick == k) {
                    u16 value = slot->mValue;
                    if (value == 0x8c) {
                        if (x > y) {
                            return 0xff;
                        }
                    }
                    if (value >= 0x8e && value <= 0x91) {
                        if (y >= 0) {
                            return 0xff;
                        }
                    }
                    result = i;
                    break;
                }
                k++;
            }
            i++;
        } while (i < n);
    }
    return result;
}

