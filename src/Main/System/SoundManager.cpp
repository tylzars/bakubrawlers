#include "System/SoundManager.hpp"

// NitroSystem sound library, addresses in the main module.
extern "C" void InitSnd();
extern "C" void UpdateSnd();
extern "C" void func_020367b0(u32 volume);
extern "C" u32 CreateSndHeap(void *buffer, u32 size);
extern "C" void OpenSndArc(void *arc, const char *path, u32 heap, u32 unk);
extern "C" void InitWavStrms(u32 a, u32 heap);
extern "C" void InitPlayersFromArc(u32 heap);
extern "C" void LoadGroup(s32 id, u32 heap);
extern "C" void RecordSndHeapState(u32 heap);
extern "C" void InitSndHandle(void *handle);
extern "C" void SetPlayerMaxSounds(u32 a, u32 b);
extern "C" void RestoreSndHeapState(u32 heap, u32 a);
extern "C" void StopHandleSound(void *handle, u32 fadeFrames);
extern "C" void func_02039048(void *handle, s32 id);
extern "C" void SetArcReadChunk(u32 a);
extern "C" void func_02039090(void *handle, u32 a, s32 b, s32 c, s32 d, s32 e);
extern "C" void StopPlayerSounds(u32 a, u32 b);
extern "C" void StopSoundsWithIdPair(s32 a, s32 b, u32 c);
extern "C" int CountSoundsWithIdPair(s32 a, s32 b);
extern "C" void SetHandleVolB(void *handle, s32 a);
extern "C" void func_02036b88(void *handle, u32 a, s32 b);

extern u32 gSoundHeapBuffer[];
extern u32 gSeqArcPlayerIndex;
extern SoundEntry gSeqTable[];
extern SoundEntry gSeqArcTable[];

void SoundManager::Init() {
    InitSnd();
    func_020245c4(0x32);
    mHeap = CreateSndHeap(gSoundHeapBuffer, 0x139000);
    OpenSndArc(mArchive, "/Snd/sound_data.sdat", mHeap, 0);
    mSeqTable = gSeqTable;
    mSeqArcTable = gSeqArcTable;
    InitWavStrms(1, mHeap);
    InitPlayersFromArc(mHeap);
    LoadGroup(0, mHeap);
    RecordSndHeapState(mHeap);
    InitSndHandle(&mSeqHandle);
    InitSndHandle(&mSeqArcHandle);
    SetPlayerMaxSounds(mSeqArcPlayerIndex, 8);
    mSeqIndex = -1;
    mSeqArcPlayerIndex = gSeqArcPlayerIndex;
    mSeqArcVolume = 0x7f;
    mSeqVolume = 0x7f;
}

void SoundManager::Update() {
    UpdateSnd();
}

void SoundManager::RestoreHeapState() {
    RestoreSndHeapState(mHeap, 1);
    mSeqIndex = -1;
}

void SoundManager::func_020245c4(u32 unk1) {
    func_020367b0(unk1);
}

void SoundManager::LoadAndStartSeq(u32 unk1) {
    LoadSeqGroup(unk1);
    StartSeq(unk1);
}

void SoundManager::LoadSeqGroup(u32 unk1) {
    s16 id = mSeqTable[unk1].mUnk_0;
    if (id < 0) {
        return;
    }
    if (unk1 == mSeqIndex) {
        return;
    }
    LoadGroup(id, mHeap);
}

void SoundManager::StartSeq(u32 unk1) {
    if (mSeqTable[unk1].mUnk_2 < 0) {
        return;
    }
    if (unk1 == mSeqIndex) {
        return;
    }
    StopHandleSound(&mSeqHandle, 1);
    func_02039048(&mSeqHandle, mSeqTable[unk1].mUnk_2);
    SetSeqVolume(0x7f);
    mSeqIndex = unk1;
}

void SoundManager::StopSeq(u32 fadeFrames) {
    StopHandleSound(&mSeqHandle, fadeFrames);
    mSeqIndex = -1;
}

void SoundManager::SetSeqVolume(s32 unk1) {
    SetHandleVolB(&mSeqHandle, mSeqVolume * unk1 / 0x7f);
}

void SoundManager::LoadSoundGroup(u32 unk1) {
    SetArcReadChunk(0x8000);
    LoadGroup(unk1, mHeap);
}

void SoundManager::StartSeqArc(u32 unk1) {
    SoundEntry *table = mSeqArcTable;
    if (table[unk1].mUnk_2 >= 0 && table[unk1].mUnk_0 >= 0) {
        func_02039090(&mSeqArcHandle, mSeqArcPlayerIndex, -1, -1, table[unk1].mUnk_0, table[unk1].mUnk_2);
        SetSeqArcVolume(0x7f);
    }
}

void SoundManager::StopSeqArcPlayerSounds(u32 unk1) {
    StopPlayerSounds(mSeqArcPlayerIndex, unk1);
}

extern "C" void StopSeqArc(SoundManager *manager, u32 unk1, u32 unk2) {
    SoundEntry *table = manager->mSeqArcTable;
    if (table[unk1].mUnk_2 >= 0 && table[unk1].mUnk_0 >= 0) {
        StopSoundsWithIdPair(table[unk1].mUnk_0, table[unk1].mUnk_2, unk2);
    }
}

extern "C" int CountSeqArcSounds(SoundManager *manager, u32 unk1) {
    SoundEntry *table = manager->mSeqArcTable;
    if (table[unk1].mUnk_2 < 0) {
        return 0;
    }
    if (table[unk1].mUnk_0 < 0) {
        return 0;
    }
    return (u8) CountSoundsWithIdPair(table[unk1].mUnk_0, table[unk1].mUnk_2);
}

void SoundManager::SetSeqArcVolume(s32 unk1) {
    SetHandleVolB(&mSeqArcHandle, mSeqArcVolume * unk1 / 0x7f);
}

extern "C" void func_020247fc(SoundManager *manager, s16 unk1) {
    func_02036b88(&manager->mSeqArcHandle, 0xffff, unk1);
}
