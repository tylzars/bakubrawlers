#pragma once

#include "global.h"
#include "types.h"

// One entry of a sound table: two ids, negative when unused.
struct SoundEntry {
    /* 0 */ s16 mUnk_0;
    /* 2 */ s16 mUnk_2;
    /* 4 */
};

// Global sound player, constructed on first use and destroyed at exit.
class SoundManager {
public:
    /* 00 */ SoundEntry *mSeqTable;
    /* 04 */ SoundEntry *mSeqArcTable;
    /* 08 */ u8 mArchive[0x94]; // archive state (NNSSndArc)
    /* 9c */ u32 mHeap;
    /* a0 */ u32 mSeqHandle; // sound handle
    /* a4 */ u32 mSeqArcHandle; // sound handle
    /* a8 */ s32 mSeqIndex;
    /* ac */ u8 mSeqArcPlayerIndex;
    /* ad */ u8 mSeqArcVolume;
    /* ae */ u8 mSeqVolume;
    /* af */ PAD(0xaf, 0xb0);
    /* b0 */

#ifdef GAME_SINGLETON_CTORS
    // As Game.cpp saw the class: an empty constructor, so its copies of the singleton objects join guard first.
    SoundManager() {}
#endif
    ~SoundManager();

    static SoundManager &Get() {
        static SoundManager instance;
        return instance;
    }

    void Init();
    void Update();
    void RestoreHeapState();
    void func_020245c4(u32 unk1);
    void LoadAndStartSeq(u32 unk1);
    void LoadSeqGroup(u32 unk1);
    void StartSeq(u32 unk1);
    void StopSeq(u32 fadeFrames);
    void SetSeqVolume(s32 unk1);
    void LoadSoundGroup(u32 unk1);
    void StartSeqArc(u32 unk1);
    void StopSeqArcPlayerSounds(u32 unk1);
    void SetSeqArcVolume(s32 unk1);
};

// Sound helpers with C linkage that take the manager explicitly.
extern "C" void StopSeqArc(SoundManager *manager, u32 unk1, u32 unk2);
extern "C" int CountSeqArcSounds(SoundManager *manager, u32 unk1);
extern "C" void func_020247fc(SoundManager *manager, s16 unk1);
