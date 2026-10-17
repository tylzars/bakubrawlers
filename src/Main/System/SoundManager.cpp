#include "System/SoundManager.hpp"

// NitroSystem sound library (NNS_Snd*), addresses in the main module.
extern "C" void func_020366f8();
extern "C" void func_02036780();
extern "C" void func_020367b0(u32 volume);
extern "C" u32 func_0203824c(void *buffer, u32 size);
extern "C" void func_02037af8(void *arc, const char *path, u32 heap, u32 unk);
extern "C" void func_020392fc(u32 a, u32 heap);
extern "C" void func_02038fac(u32 heap);
extern "C" void func_020385b8(s32 id, u32 heap);
extern "C" void func_020383f8(u32 heap);
extern "C" void func_02036ab4(void *handle);
extern "C" void func_02036918(u32 a, u32 b);
extern "C" void func_02038444(u32 heap, u32 a);
extern "C" void func_020369e4(void *handle, u32 fadeFrames);
extern "C" void func_02039048(void *handle, s32 id);
extern "C" void func_02038214(u32 a);
extern "C" void func_02039090(void *handle, u32 a, s32 b, s32 c, s32 d, s32 e);
extern "C" void func_020369f4(u32 a, u32 b);
extern "C" void func_02036a50(s32 a, s32 b, u32 c);
extern "C" int func_02036ae0(s32 a, s32 b);
extern "C" void func_02036b40(void *handle, s32 a);
extern "C" void func_02036b88(void *handle, u32 a, s32 b);

extern u32 data_020d5520[];
extern u32 data_02090778;
extern SoundEntry data_02090780[];
extern SoundEntry data_02090788[];

void SoundManager::Init() {
    func_020366f8();
    func_020245c4(0x32);
    mHeap = func_0203824c(data_020d5520, 0x139000);
    func_02037af8(mArchive, "/Snd/sound_data.sdat", mHeap, 0);
    mSeqTable = data_02090780;
    mSeqArcTable = data_02090788;
    func_020392fc(1, mHeap);
    func_02038fac(mHeap);
    func_020385b8(0, mHeap);
    func_020383f8(mHeap);
    func_02036ab4(&mSeqHandle);
    func_02036ab4(&mSeqArcHandle);
    func_02036918(mUnk_ac, 8);
    mSeqIndex = -1;
    mUnk_ac = data_02090778;
    mSeqArcVolume = 0x7f;
    mSeqVolume = 0x7f;
}

void SoundManager::Update() {
    func_02036780();
}

void SoundManager::func_020245a4() {
    func_02038444(mHeap, 1);
    mSeqIndex = -1;
}

void SoundManager::func_020245c4(u32 unk1) {
    func_020367b0(unk1);
}

void SoundManager::LoadAndStartSeq(u32 unk1) {
    func_020245f4(unk1);
    StartSeq(unk1);
}

void SoundManager::func_020245f4(u32 unk1) {
    s16 id = mSeqTable[unk1].mUnk_0;
    if (id < 0) {
        return;
    }
    if (unk1 == mSeqIndex) {
        return;
    }
    func_020385b8(id, mHeap);
}

void SoundManager::StartSeq(u32 unk1) {
    if (mSeqTable[unk1].mUnk_2 < 0) {
        return;
    }
    if (unk1 == mSeqIndex) {
        return;
    }
    func_020369e4(&mSeqHandle, 1);
    func_02039048(&mSeqHandle, mSeqTable[unk1].mUnk_2);
    SetSeqVolume(0x7f);
    mSeqIndex = unk1;
}

void SoundManager::StopSeq(u32 fadeFrames) {
    func_020369e4(&mSeqHandle, fadeFrames);
    mSeqIndex = -1;
}

void SoundManager::SetSeqVolume(s32 unk1) {
    func_02036b40(&mSeqHandle, mSeqVolume * unk1 / 0x7f);
}

void SoundManager::func_020246d4(u32 unk1) {
    func_02038214(0x8000);
    func_020385b8(unk1, mHeap);
}

void SoundManager::StartSeqArc(u32 unk1) {
    SoundEntry *table = mSeqArcTable;
    if (table[unk1].mUnk_2 >= 0 && table[unk1].mUnk_0 >= 0) {
        func_02039090(&mSeqArcHandle, mUnk_ac, -1, -1, table[unk1].mUnk_0, table[unk1].mUnk_2);
        SetSeqArcVolume(0x7f);
    }
}

void SoundManager::func_02024754(u32 unk1) {
    func_020369f4(mUnk_ac, unk1);
}

extern "C" void func_02024764(SoundManager *manager, u32 unk1, u32 unk2) {
    SoundEntry *table = manager->mSeqArcTable;
    if (table[unk1].mUnk_2 >= 0 && table[unk1].mUnk_0 >= 0) {
        func_02036a50(table[unk1].mUnk_0, table[unk1].mUnk_2, unk2);
    }
}

extern "C" int func_02024790(SoundManager *manager, u32 unk1) {
    SoundEntry *table = manager->mSeqArcTable;
    if (table[unk1].mUnk_2 < 0) {
        return 0;
    }
    if (table[unk1].mUnk_0 < 0) {
        return 0;
    }
    return (u8) func_02036ae0(table[unk1].mUnk_0, table[unk1].mUnk_2);
}

void SoundManager::SetSeqArcVolume(s32 unk1) {
    func_02036b40(&mSeqArcHandle, mSeqArcVolume * unk1 / 0x7f);
}

extern "C" void func_020247fc(SoundManager *manager, s16 unk1) {
    func_02036b88(&manager->mSeqArcHandle, 0xffff, unk1);
}
