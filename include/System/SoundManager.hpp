#pragma once

#include "global.h"
#include "types.h"

// One entry of a sound table: two ids, negative when unused.
struct UnkStruct_SoundEntry {
    /* 0 */ s16 mUnk_0;
    /* 2 */ s16 mUnk_2;
    /* 4 */
};

// Global sound player, constructed on first use and destroyed at exit.
class SoundManager {
public:
    /* 00 */ UnkStruct_SoundEntry *mUnk_00;
    /* 04 */ UnkStruct_SoundEntry *mUnk_04;
    /* 08 */ u8 mUnk_08[0x94]; // archive state (NNSSndArc)
    /* 9c */ u32 mUnk_9c;
    /* a0 */ u32 mUnk_a0; // sound handle
    /* a4 */ u32 mUnk_a4; // sound handle
    /* a8 */ s32 mUnk_a8;
    /* ac */ u8 mUnk_ac;
    /* ad */ u8 mUnk_ad;
    /* ae */ u8 mUnk_ae;
    /* af */ PAD(0xaf, 0xb0);
    /* b0 */

    ~SoundManager();

    static SoundManager &Get() {
        static SoundManager instance;
        return instance;
    }

    void func_020244cc();
    void func_020245a4();
    void func_020245c4(u32 unk1);
    void func_020245d4(u32 unk1);
    void func_020245f4(u32 unk1);
    void func_02024628(u32 unk1);
    void func_02024688();
    void func_020246a4(s32 unk1);
    void func_020246d4(u32 unk1);
    void func_020246f8(u32 unk1);
    void func_02024754();
    void func_020247cc(s32 unk1);
};

// Sound helpers with C linkage that take the manager explicitly.
extern "C" void func_02024764(SoundManager *manager, u32 unk1, u32 unk2);
extern "C" int func_02024790(SoundManager *manager, u32 unk1);
extern "C" void func_020247fc(SoundManager *manager, s16 unk1);
