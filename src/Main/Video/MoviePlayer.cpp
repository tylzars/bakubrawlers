#include "Video/MoviePlayer.hpp"

#include "Graphics/Display.hpp"
#include "Graphics/SpriteManager.hpp"
#include <nitro/mi.h>
#include <nitro/os.h>

extern "C" {
extern "C" void DC_StoreRange(void *buffer, u32 size);
extern "C" void func_0200800c(u32 a, u32 b, u32 c, u32 d);
extern "C" void func_02008034(u32 a, u32 b, u32 c, u32 d);
extern "C" void func_02008098(u32 a, u32 b, u32 c, void (*callback)(void *), void *arg);
extern "C" void func_020080d8(u32 a, u32 b);
extern "C" void func_020080f8(u32 a, u32 b);
extern "C" void func_02008160(u32 a, u32 b, void *data, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h, u32 i);
extern "C" void SND_FlushCommand(u32 a);
extern "C" u32 G2S_GetBG2ScrPtr(u32 a);
extern "C" void GX_DisableBankForBG();
extern "C" void GX_DisableBankForOBJ();
extern "C" void GX_DisableBankForBGExtPltt();
extern "C" void GX_DisableBankForOBJExtPltt();
extern "C" void GX_DisableBankForTex();
extern "C" void GX_DisableBankForTexPltt();
extern "C" void GX_DisableBankForSubBG();
extern "C" void GX_DisableBankForSubOBJ();
extern "C" void GX_DisableBankForSubBGExtPltt();
extern "C" void GX_DisableBankForSubOBJExtPltt();
extern "C" u32 func_02005a68();
extern "C" void func_0200d3e0(u32 a, u32 b, u32 c);
extern "C" void func_0200d904(u32 a);
extern "C" void OS_WaitIrq(u32 a, u32 b);
u32 func_0200682c();
extern "C" void OS_CreateAlarm(void *alarm);
extern "C" void OS_SetPeriodicAlarm(void *alarm, u64 start, u64 period, void (*handler)(void *), void *arg);
extern "C" void OS_CancelAlarm(void *alarm);
extern "C" u32 func_0203bdc8();
extern "C" u32 func_0203bdd4();
extern "C" void func_0203bddc(u32 a, u32 b);
extern "C" void func_0203bdfc(u32 a, u32 b);
}
struct Reader;
u32 MobiclipCreate(Reader *decoder, u32 a);
void MobiclipDelete(Player *video);
u32 func_0203a67c(Player *video);
u32 MobiclipGetAudioChannelCount(Player *video);
u32 MobiclipGetFrameRate(Player *video);
u32 MobiclipGetFrameCount(Player *video);
u32 func_0203a6dc(Player *video);
BOOL func_0203a6f4(Player *video);
void func_0203a718(Player *video);
void func_0203a73c(Player *video, u32 a, u32 b, int c);
void func_0203a760(Player *video);
u32 func_0203a784(Player *video);
void func_0203a79c(Player *video, void *buffer);
void func_0203a7c0(Player *video);
extern "C" u64 OS_GetTick();
extern "C" BOOL func_0200d924();
extern "C" void GX_SetBankForLCDC(u32 a);
extern "C" void GX_SetBankForSubBG(u32 a);
extern "C" void GX_SetBankForSubOBJ(u32 a);
extern "C" void GX_SetBankForSubBGExtPltt(u32 a);
extern "C" void func_02002dd8();
extern "C" void GXS_LoadOAM(const void *data, u32 offset, u32 size);
extern "C" void func_0200a7b4(void *decoder);
extern "C" void func_0200aa94(void *decoder, const char *path);
extern "C" void func_0200aadc(void *decoder);
extern "C" int OS_GetArenaLo(int a);
extern "C" int OS_GetArenaHi(int a);
void QueueFrameCallback(void (*callback)(void *), void *arg);

MoviePlayer::MoviePlayer() {
    mUnk_d4  = 0;
    mUnk_d8  = 0;
    mFrameCount  = 0;
    mFrameIndex  = 0;
    mUnk_e4  = 0;
    mUseBuffer2 = 0;
    mSoundStarted = 0;
    mUnk_10a = 0;
    mUnk_10b = 0;
}

MoviePlayer::~MoviePlayer() {
}

void CountTick(MoviePlayer *player) {
    player->mUnk_00 += 1;
}

void MoviePlayer::StartSound(u32 rate) {
    u32 v      = 0xffb0ff / rate;
    u32 size   = mUnk_f8 * mUnk_fc;
    u32 total  = (mUnk_fc >> 6) * (v << 1);
    DC_StoreRange(mBuffer, size << 1);
    if (mUseBuffer2 == 1) {
        DC_StoreRange(mBuffer2, mUnk_f8 * mUnk_fc << 1);
    }
    func_020080d8(3, 0);
    if (mUseBuffer2 == 1) {
        func_02008160(0, 1, mBuffer, 1, 0, (mUnk_f8 * mUnk_fc) >> 1, 0x7f, 0, v, 0);
        func_02008160(1, 1, mBuffer2, 1, 0, (mUnk_f8 * mUnk_fc) >> 1, 0x7f, 0, v, 0x7f);
    } else {
        func_02008160(0, 1, mBuffer, 1, 0, (mUnk_f8 * mUnk_fc) >> 1, 0x7f, 0, v, 0x40);
    }
    func_02008098(0, total, total, (void (*)(void *)) CountTick, this);
    func_0200800c(3, 0, 1, 0);
    SND_FlushCommand(1);
}

void MoviePlayer::StopSound() {
    func_02008034(3, 0, 1, 0);
    func_020080f8(3, 0);
    SND_FlushCommand(1);
}

u32 MoviePlayer::GetVideoTarget() {
    return mVramPage == 0 ? 0x6820000 : 0x6800000;
}

u32 MoviePlayer::GetVideo2Target() {
    u32 result = 0;
    if (mUseVideo2) {
        if (mVramPage == 0) {
            result = 0x6600000;
        } else {
            result = G2S_GetBG2ScrPtr(mVramPage);
        }
    }
    return result;
}

void MoviePlayer::DrawFrame() {
    while (mFlipDone == 0) {
    }
    while (mFrameDue != 1) {
    }
    mFrameDue = 0;
    func_0203a73c(mVideo, GetVideoTarget(), 0x100, 0);
    if (mUseVideo2) {
        func_0203a73c(mVideo2, GetVideo2Target(), 0x100, 0);
    }
    mFlipDone = 0;
}

void SetFrameDue(MoviePlayer *player) {
    player->mFrameDue = 1;
}

void FlipVramPage(MoviePlayer *player) {
    if (player->mFlipDone != 0) {
        return;
    }
    player->mFlipDone = 1;
    if (player->mVramPage == 0) {
        GX_SetGraphicsMode(6, 0, 0);
        if (player->mUseVideo2) {
            *(vu32 *) 0x4001000 = (*(vu32 *) 0x4001000 & ~0x60) | 0x20;
            SpriteOamEntry *oam = SpriteManager::Get().mScreens[1].mOam;
            for (int i = 0; i < 12; i++) {
                int row = i / 4;
                int col = i % 4;
                SpriteOamEntry *e = oam++;
                e->mAttr01 = ((row * 64) & 0xff) | 0xc0000c00 | ((col & 7) << 22);
                e->mAttr2  = ((col << 3) + (row << 8)) | 0xf000;
            }
            GXS_LoadOAM(oam, 0, 0x400);
        }
    } else {
        GX_SetGraphicsMode(2, 0, 0);
        if (player->mUseVideo2) {
            *(vu32 *) 0x4001000 = (*(vu32 *) 0x4001000 & ~0x1f00) | 0x400;
        }
    }
    player->mVramPage ^= 1;
}

void MoviePlayer::Open(void *a, void *b, u8 c, u32 d) {
    mUnk_10c = a != 0;
    mUseVideo2 = a != 0 && b != 0;
    GX_DisableBankForBG();
    GX_DisableBankForBGExtPltt();
    GX_DisableBankForOBJ();
    GX_DisableBankForOBJExtPltt();
    GX_DisableBankForTex();
    GX_DisableBankForTexPltt();
    GX_DisableBankForSubBG();
    GX_DisableBankForSubBGExtPltt();
    GX_DisableBankForSubOBJ();
    GX_DisableBankForSubOBJExtPltt();
    MIi_CpuClearFast(0, (void *) 0x5000000, 0x200);
    MIi_CpuClearFast(0, (void *) 0x5000400, 0x200);
    GX_SetBankForLCDC(0x1ff);
    MIi_CpuClearFast(0, (void *) 0x6800000, 0xa4000);
    func_02002dd8();
    GX_SetBankForLCDC(3);
    GX_SetGraphicsMode(2, 0, 0);
    if (mUseVideo2) {
        GX_SetBankForSubOBJ(8);
        GX_SetBankForSubBG(4);
        GXS_SetGraphicsMode(5);
        vu32 *dispcnt = (vu32 *) 0x4001000;
        *dispcnt      = (*dispcnt & ~0x1f00) | 0x400;
        vu16 *bgcnt   = (vu16 *) 0x400100c;
        *bgcnt        = *bgcnt & ~3;
        *bgcnt        = ((*bgcnt & 0x43) | 0x84) | 0x4000;
        *dispcnt      = (*dispcnt & ~0x60) | 0x20;
    }
    vu16 *powcnt = (vu16 *) 0x4000304;
    if (mUseVideo2) {
        *powcnt = *powcnt | 0x8000;
    } else if (mUnk_10c) {
        *powcnt = *powcnt | 0x8000;
    } else {
        a       = b;
        *powcnt = *powcnt & ~0x8000;
    }
    u32 startA = OS_GetArenaLo(3);
    u32 sizeA  = OS_GetArenaHi(3) - startA;
    if (sizeA >= func_0203bdc8()) {
        sizeA = func_0203bdc8();
    }
    u32 sizeB = OS_GetArenaLo(4);
    sizeB     = func_0200682c() + 0x4000 - sizeB;
    if (sizeB >= func_0203bdd4()) {
        sizeB = func_0203bdd4();
    }
    func_0203bddc(OS_GetArenaLo(3), sizeA);
    func_0203bdfc(OS_GetArenaLo(4), sizeB);
    func_0200a7b4(mDecoder);
    func_0200aa94(mDecoder, (const char *) a);
    mUnk_d4 = 6;
    mVideo  = (Player *) MobiclipCreate((Reader *) mDecoder, mUnk_d4);
    mFrameCount = MobiclipGetFrameCount(mVideo);
    mUnk_fc = func_0203a6dc(mVideo);
    if (mUseVideo2) {
        func_0200a7b4(mDecoder2);
        func_0200aa94(mDecoder2, (const char *) b);
        mVideo2 = (Player *) MobiclipCreate((Reader *) mDecoder2, mUnk_d4);
    }
    mUseBuffer2 = c;
    u32 frames = func_0203a67c(mVideo);
    u32 rate   = MobiclipGetFrameRate(mVideo);
    mUnk_d8    = (u32) (((u64) frames << 24) / ((u64) mUnk_fc * rate)) + 1;
    mUnk_f8    = mUnk_d8 * (mUnk_d4 + 1);
    mBuffer    = (u16 *) OS_Alloc(mUnk_f8 * mUnk_fc * 2);
    MIi_CpuClearFast(0, mBuffer, mUnk_f8 * mUnk_fc * 2);
    if (mUseBuffer2 == 1) {
        mBuffer2 = (u16 *) OS_Alloc(mUnk_f8 * mUnk_fc * 2);
        MIi_CpuClearFast(0, mBuffer2, mUnk_f8 * mUnk_fc * 2);
    }
    mUnk_00 = 0;
    mWriteOffset = 0;
    mFrames = 0;
    mVramPage = 0;
    mFlipDone = 1;
    OS_CreateAlarm(mAlarm);
    u64 tick   = OS_GetTick();
    u64 period = (((((u64) 1000000 << 24) / MobiclipGetFrameRate(mVideo)) * 0x82ea) >> 6) / 1000;
    OS_SetPeriodicAlarm(mAlarm, tick + 5, (u32) period, (void (*)(void *)) SetFrameDue, this);
    mUnk_10f = MobiclipGetAudioChannelCount(mVideo);
    int n    = mUseBuffer2 + 1;
    int q    = mUnk_10f / n;
    if (*(u8 *) &d >= q) {
        *(u8 *) &d = q - 1;
    }
    mUnk_110 = *(u8 *) &d * n;
}

void MoviePlayer::Close() {
    if ((func_02005a68() & 0x40000000) || (func_02005a68() & 0x80000000)) {
        if (mSoundStarted == 1) {
            StopSound();
        }
    }
    OS_CancelAlarm(mAlarm);
    if (mUseBuffer2 == 1) {
        OS_Free(mBuffer2);
    }
    OS_Free(mBuffer);
    MobiclipDelete(mVideo);
    func_0200aadc(mDecoder);
    if (!mUseVideo2) {
        return;
    }
    MobiclipDelete(mVideo2);
    func_0200aadc(mDecoder2);
}

void MoviePlayer::SeekToEnd() {
    mUnk_10b = 1;
    mFrameIndex  = mFrameCount;
}

BOOL MoviePlayer::Update() {
    int i;
    u64 count;
    u8 j;
    BOOL result = FALSE;
    if (func_0203a6f4(mVideo)) {
        func_0203a718(mVideo);
        if (mUseVideo2) {
            func_0203a6f4(mVideo2);
            func_0203a718(mVideo2);
        }
        mUnk_e4++;
        if ((func_02005a68() & 0x40000000) || (func_02005a68() & 0x80000000)) {
            if (func_0203a67c(mVideo)) {
                count = func_0203a784(mVideo);
                i = 0;
                j = 0;
                while (i < count) {
                    if (mSoundStarted) {
                        while ((u64) mUnk_f8 - (u64) (mFrames - mUnk_00) == 0) {
                        }
                    }
                    j = 0;
                    do {
                        if (j == mUnk_110) {
                            func_0203a79c(mVideo, mBuffer + mWriteOffset);
                            if (mUseBuffer2 == 1) {
                                func_0203a79c(mVideo, mBuffer2 + mWriteOffset);
                            }
                            DC_StoreRange(mBuffer + mWriteOffset, mUnk_fc * 2);
                            if (mUseBuffer2 == 1) {
                                DC_StoreRange(mBuffer2 + mWriteOffset, mUnk_fc * 2);
                            }
                            mWriteOffset += mUnk_fc;
                            if (mWriteOffset == mUnk_f8 * mUnk_fc) {
                                mWriteOffset = 0;
                            }
                            mFrames += 1;
                        } else {
                            func_0203a7c0(mVideo);
                            if (mUseBuffer2 == 1) {
                                func_0203a7c0(mVideo);
                            }
                        }
                        j = j + (mUseBuffer2 + 1);
                    } while (j < mUnk_10f);
                    i++;
                }
                if (!mSoundStarted) {
                    if (mUnk_e4 >= mUnk_d4) {
                        StartSound(func_0203a67c(mVideo));
                        mSoundStarted = 1;
                    }
                }
            }
        }
        if (mUnk_e4 >= mUnk_d4 || func_0203a67c(mVideo) == 0) {
            if ((s32) (mFrames - mUnk_00) <= (s32) (mUnk_d8 * 3) && !mUnk_10a && func_0203a67c(mVideo) != 0) {
                func_0203a760(mVideo);
                if (mUseVideo2) {
                    func_0203a760(mVideo2);
                }
                mUnk_10a = 1;
            } else {
                DrawFrame();
                mUnk_10a = 0;
            }
        }
        if (((*(vu16 *) 0x27fffa8 & 0x8000) >> 15) == 1) {
            if (mSoundStarted == 1) {
                StopSound();
            }
            func_0200d904(0);
            func_0200d3e0(0xc, 0, 0);
            while ((*(vu16 *) 0x27fffa8 & 0x8000) >> 15) {
                OS_WaitIrq(0, 1);
            }
            if (!func_0200d924()) {
                do {
                    func_0200d904(1);
                    OS_WaitIrq(0, 1);
                } while (!func_0200d924());
            }
            if (mSoundStarted == 1) {
                MIi_CpuClearFast(0, mBuffer, mUnk_f8 * mUnk_fc * 2);
                if (mUseBuffer2 == 1) {
                    MIi_CpuClearFast(0, mBuffer2, mUnk_f8 * mUnk_fc * 2);
                }
                mFrames -= mUnk_00;
                mUnk_00 = 0;
                mWriteOffset = (u32) mFrames * mUnk_fc;
                if (mWriteOffset == mUnk_f8 * mUnk_fc) {
                    mWriteOffset = 0;
                }
                StartSound(func_0203a67c(mVideo));
            }
        }
        mFrameIndex++;
        if (mFrameIndex >= mFrameCount) {
            result = TRUE;
        } else {
            QueueFrameCallback((void (*)(void *)) FlipVramPage, this);
        }
    }
    return result;
}
