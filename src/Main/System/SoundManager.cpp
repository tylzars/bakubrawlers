#include "System/SoundManager.hpp"

// NitroSystem sound library (NNS_Snd*), addresses in the main module.
extern "C" void func_020366f8();
extern "C" void func_02036780();
extern "C" void func_020367b0(u32 volume);
extern u32 func_0203824c(void *buffer, u32 size);
extern void func_02037af8(void *arc, const char *path, u32 heap, u32 unk);
extern void func_020392fc(u32 a, u32 heap);
extern void func_02038fac(u32 heap);
extern void func_020385b8(s32 id, u32 heap);
extern void func_020383f8(u32 heap);
extern "C" void func_02036ab4(void *handle);
extern "C" void func_02036918(u32 a, u32 b);
extern void func_02038444(u32 heap, u32 a);
extern "C" void func_020369e4(void *handle, u32 fadeFrames);
extern void func_02039048(void *handle, s32 id);
extern void func_02038214(u32 a);
extern void func_02039090(void *handle, u32 a, s32 b, s32 c, s32 d, s32 e);
extern "C" void func_020369f4(u32 a, u32 b);
extern "C" void func_02036a50(s32 a, s32 b, u32 c);
extern "C" int func_02036ae0(s32 a, s32 b);
extern "C" void func_02036b40(void *handle, s32 a);
extern "C" void func_02036b88(void *handle, u32 a, s32 b);

extern u32 data_020d5520[];
extern u32 data_02090778;
extern UnkStruct_SoundEntry data_02090780[];
extern UnkStruct_SoundEntry data_02090788[];

void SoundManager::func_020244cc() {
    func_020366f8();
    func_020245c4(0x32);
    mUnk_9c = func_0203824c(data_020d5520, 0x139000);
    func_02037af8(mUnk_08, "/Snd/sound_data.sdat", mUnk_9c, 0);
    mUnk_00 = data_02090780;
    mUnk_04 = data_02090788;
    func_020392fc(1, mUnk_9c);
    func_02038fac(mUnk_9c);
    func_020385b8(0, mUnk_9c);
    func_020383f8(mUnk_9c);
    func_02036ab4(&mUnk_a0);
    func_02036ab4(&mUnk_a4);
    func_02036918(mUnk_ac, 8);
    mUnk_a8 = -1;
    mUnk_ac = data_02090778;
    mUnk_ad = 0x7f;
    mUnk_ae = 0x7f;
}

void func_02024598() {
    func_02036780();
}

void SoundManager::func_020245a4() {
    func_02038444(mUnk_9c, 1);
    mUnk_a8 = -1;
}

void SoundManager::func_020245c4(u32 unk1) {
    func_020367b0(unk1);
}

void SoundManager::func_020245d4(u32 unk1) {
    func_020245f4(unk1);
    func_02024628(unk1);
}

void SoundManager::func_020245f4(u32 unk1) {
    s16 id = mUnk_00[unk1].mUnk_0;
    if (id < 0) {
        return;
    }
    if (unk1 == mUnk_a8) {
        return;
    }
    func_020385b8(id, mUnk_9c);
}

void SoundManager::func_02024628(u32 unk1) {
    if (mUnk_00[unk1].mUnk_2 < 0) {
        return;
    }
    if (unk1 == mUnk_a8) {
        return;
    }
    func_020369e4(&mUnk_a0, 1);
    func_02039048(&mUnk_a0, mUnk_00[unk1].mUnk_2);
    func_020246a4(0x7f);
    mUnk_a8 = unk1;
}

void SoundManager::func_02024688(u32 fadeFrames) {
    func_020369e4(&mUnk_a0, fadeFrames);
    mUnk_a8 = -1;
}

void SoundManager::func_020246a4(s32 unk1) {
    func_02036b40(&mUnk_a0, mUnk_ae * unk1 / 0x7f);
}

void SoundManager::func_020246d4(u32 unk1) {
    func_02038214(0x8000);
    func_020385b8(unk1, mUnk_9c);
}

void SoundManager::func_020246f8(u32 unk1) {
    UnkStruct_SoundEntry *table = mUnk_04;
    if (table[unk1].mUnk_2 >= 0 && table[unk1].mUnk_0 >= 0) {
        func_02039090(&mUnk_a4, mUnk_ac, -1, -1, table[unk1].mUnk_0, table[unk1].mUnk_2);
        func_020247cc(0x7f);
    }
}

void SoundManager::func_02024754(u32 unk1) {
    func_020369f4(mUnk_ac, unk1);
}

extern "C" void func_02024764(SoundManager *manager, u32 unk1, u32 unk2) {
    UnkStruct_SoundEntry *table = manager->mUnk_04;
    if (table[unk1].mUnk_2 >= 0 && table[unk1].mUnk_0 >= 0) {
        func_02036a50(table[unk1].mUnk_0, table[unk1].mUnk_2, unk2);
    }
}

extern "C" int func_02024790(SoundManager *manager, u32 unk1) {
    UnkStruct_SoundEntry *table = manager->mUnk_04;
    if (table[unk1].mUnk_2 < 0) {
        return 0;
    }
    if (table[unk1].mUnk_0 < 0) {
        return 0;
    }
    return (u8) func_02036ae0(table[unk1].mUnk_0, table[unk1].mUnk_2);
}

void SoundManager::func_020247cc(s32 unk1) {
    func_02036b40(&mUnk_a4, mUnk_ad * unk1 / 0x7f);
}

extern "C" void func_020247fc(SoundManager *manager, s16 unk1) {
    func_02036b88(&manager->mUnk_a4, 0xffff, unk1);
}
