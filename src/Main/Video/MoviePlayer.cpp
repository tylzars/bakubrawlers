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
u32 func_0203a574(void *decoder, u32 a);
void func_0203a65c(MobiclipPlayer *video);
u32 func_0203a67c(MobiclipPlayer *video);
u32 func_0203a694(MobiclipPlayer *video);
u32 func_0203a6ac(MobiclipPlayer *video);
u32 func_0203a6c4(MobiclipPlayer *video);
u32 func_0203a6dc(MobiclipPlayer *video);
BOOL func_0203a6f4(MobiclipPlayer *video);
void func_0203a718(MobiclipPlayer *video);
void func_0203a73c(MobiclipPlayer *video, u32 a, u32 b, u32 c);
void func_0203a760(MobiclipPlayer *video);
u32 func_0203a784(MobiclipPlayer *video);
void func_0203a79c(MobiclipPlayer *video, void *buffer);
void func_0203a7c0(MobiclipPlayer *video);
u32 func_0203bdc8();
u32 func_0203bdd4();
void func_0203bddc(u32 a, u32 b);
void func_0203bdfc(u32 a, u32 b);
}
extern "C" u64 OS_GetTick();
extern "C" BOOL func_0200d924();
extern "C" void GX_SetBankForLCDC(u32 a);
extern "C" void GX_SetBankForSubBG(u32 a);
extern "C" void GX_SetBankForSubOBJ(u32 a);
extern "C" void GX_SetBankForSubBGExtPltt(u32 a);
void func_02002dd8();
extern "C" void GXS_LoadOAM(const void *data, u32 offset, u32 size);
extern "C" void func_0200a7b4(void *decoder);
extern "C" void func_0200aa94(void *decoder, const char *path);
extern "C" void func_0200aadc(void *decoder);
extern "C" int OS_GetArenaLo(int a);
extern "C" int OS_GetArenaHi(int a);
void func_020254f0(void (*callback)(void *), void *arg);

MoviePlayer::MoviePlayer() {
    mUnk_d4  = 0;
    mUnk_d8  = 0;
    mUnk_dc  = 0;
    mUnk_e0  = 0;
    mUnk_e4  = 0;
    mUnk_108 = 0;
    mUnk_109 = 0;
    mUnk_10a = 0;
    mUnk_10b = 0;
}

MoviePlayer::~MoviePlayer() {
}

void func_0203c1bc(MoviePlayer *player) {
    player->mUnk_00 += 1;
}

void MoviePlayer::func_0203c1d8(u32 rate) {
    u32 v      = 0xffb0ff / rate;
    u32 size   = mUnk_f8 * mUnk_fc;
    u32 total  = (mUnk_fc >> 6) * (v << 1);
    DC_StoreRange(mBuffer, size << 1);
    if (mUnk_108 == 1) {
        DC_StoreRange(mBuffer2, mUnk_f8 * mUnk_fc << 1);
    }
    func_020080d8(3, 0);
    if (mUnk_108 == 1) {
        func_02008160(0, 1, mBuffer, 1, 0, (mUnk_f8 * mUnk_fc) >> 1, 0x7f, 0, v, 0);
        func_02008160(1, 1, mBuffer2, 1, 0, (mUnk_f8 * mUnk_fc) >> 1, 0x7f, 0, v, 0x7f);
    } else {
        func_02008160(0, 1, mBuffer, 1, 0, (mUnk_f8 * mUnk_fc) >> 1, 0x7f, 0, v, 0x40);
    }
    func_02008098(0, total, total, (void (*)(void *)) func_0203c1bc, this);
    func_0200800c(3, 0, 1, 0);
    SND_FlushCommand(1);
}

void MoviePlayer::func_0203c354() {
    func_02008034(3, 0, 1, 0);
    func_020080f8(3, 0);
    SND_FlushCommand(1);
}

u32 MoviePlayer::func_0203c384() {
    return mUnk_ec == 0 ? 0x6820000 : 0x6800000;
}

u32 MoviePlayer::func_0203c39c() {
    u32 result = 0;
    if (mUnk_10e) {
        if (mUnk_ec == 0) {
            result = 0x6600000;
        } else {
            result = G2S_GetBG2ScrPtr(mUnk_ec);
        }
    }
    return result;
}

void MoviePlayer::func_0203c3d0() {
    while (mUnk_f0 == 0) {
    }
    while (mUnk_e8 != 1) {
    }
    mUnk_e8 = 0;
    func_0203a73c(mVideo, func_0203c384(), 0x100, 0);
    if (mUnk_10e) {
        func_0203a73c(mVideo2, func_0203c39c(), 0x100, 0);
    }
    mUnk_f0 = 0;
}

void func_0203c448(MoviePlayer *player) {
    player->mUnk_e8 = 1;
}

// non-matching: 7 instructions, register allocation of the OAM fill loop (original keeps row in r6 and col in r5, computes
// row << 6 before col, and has one more `mov` for the post-incremented pointer; same size 0x160 vs 0x15c)
void func_0203c454(MoviePlayer *player) {
    if (player->mUnk_f0 != 0) {
        return;
    }
    player->mUnk_f0 = 1;
    if (player->mUnk_ec == 0) {
        GX_SetGraphicsMode(6, 0, 0);
        if (player->mUnk_10e) {
            *(vu32 *) 0x4001000 = (*(vu32 *) 0x4001000 & ~0x60) | 0x20;
            SpriteOamEntry *oam = SpriteManager::Get().mScreens[1].mOam;
            for (int i = 0; i < 12; i++) {
                int row = i / 4;
                int col = i % 4;
                SpriteOamEntry *e = oam++;
                e->mAttr01 = ((row * 64) & 0xff) | 0xc0000c00 | ((col & 7) << 22);
                e->mAttr2  = ((row << 8) + (col << 3)) | 0xf000;
            }
            GXS_LoadOAM(oam, 0, 0x400);
        }
    } else {
        GX_SetGraphicsMode(2, 0, 0);
        if (player->mUnk_10e) {
            *(vu32 *) 0x4001000 = (*(vu32 *) 0x4001000 & ~0x1f00) | 0x400;
        }
    }
    player->mUnk_ec ^= 1;
}

void MoviePlayer::func_0203c5b4(void *a, void *b, u8 c, u32 d) {
    mUnk_10c = a != 0;
    mUnk_10e = a != 0 && b != 0;
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
    if (mUnk_10e) {
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
    if (mUnk_10e) {
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
    mVideo  = (MobiclipPlayer *) func_0203a574(mDecoder, mUnk_d4);
    mUnk_dc = func_0203a6c4(mVideo);
    mUnk_fc = func_0203a6dc(mVideo);
    if (mUnk_10e) {
        func_0200a7b4(mDecoder2);
        func_0200aa94(mDecoder2, (const char *) b);
        mVideo2 = (MobiclipPlayer *) func_0203a574(mDecoder2, mUnk_d4);
    }
    mUnk_108 = c;
    u32 frames = func_0203a67c(mVideo);
    u32 rate   = func_0203a6ac(mVideo);
    mUnk_d8    = (u32) (((u64) frames << 24) / ((u64) mUnk_fc * rate)) + 1;
    mUnk_f8    = mUnk_d8 * (mUnk_d4 + 1);
    mBuffer    = (u16 *) OS_Alloc(mUnk_f8 * mUnk_fc * 2);
    MIi_CpuClearFast(0, mBuffer, mUnk_f8 * mUnk_fc * 2);
    if (mUnk_108 == 1) {
        mBuffer2 = (u16 *) OS_Alloc(mUnk_f8 * mUnk_fc * 2);
        MIi_CpuClearFast(0, mBuffer2, mUnk_f8 * mUnk_fc * 2);
    }
    mUnk_00 = 0;
    mUnk_f4 = 0;
    mFrames = 0;
    mUnk_ec = 0;
    mUnk_f0 = 1;
    OS_CreateAlarm(mAlarm);
    u64 tick   = OS_GetTick();
    u64 period = (((((u64) 1000000 << 24) / func_0203a6ac(mVideo)) * 0x82ea) >> 6) / 1000;
    OS_SetPeriodicAlarm(mAlarm, tick + 5, (u32) period, (void (*)(void *)) func_0203c448, this);
    mUnk_10f = func_0203a694(mVideo);
    int n    = mUnk_108 + 1;
    int q    = mUnk_10f / n;
    if (*(u8 *) &d >= q) {
        *(u8 *) &d = q - 1;
    }
    mUnk_110 = *(u8 *) &d * n;
}

void MoviePlayer::func_0203c9c4() {
    if ((func_02005a68() & 0x40000000) || (func_02005a68() & 0x80000000)) {
        if (mUnk_109 == 1) {
            func_0203c354();
        }
    }
    OS_CancelAlarm(mAlarm);
    if (mUnk_108 == 1) {
        OS_Free(mBuffer2);
    }
    OS_Free(mBuffer);
    func_0203a65c(mVideo);
    func_0200aadc(mDecoder);
    if (!mUnk_10e) {
        return;
    }
    func_0203a65c(mVideo2);
    func_0200aadc(mDecoder2);
}

void MoviePlayer::func_0203ca5c() {
    mUnk_10b = 1;
    mUnk_e0  = mUnk_dc;
}

BOOL MoviePlayer::func_0203ca70() {
    int i;
    u64 count;
    u8 j;
    BOOL result = FALSE;
    if (func_0203a6f4(mVideo)) {
        func_0203a718(mVideo);
        if (mUnk_10e) {
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
                    if (mUnk_109) {
                        while ((u64) mUnk_f8 - (u64) (mFrames - mUnk_00) == 0) {
                        }
                    }
                    j = 0;
                    do {
                        if (j == mUnk_110) {
                            func_0203a79c(mVideo, mBuffer + mUnk_f4);
                            if (mUnk_108 == 1) {
                                func_0203a79c(mVideo, mBuffer2 + mUnk_f4);
                            }
                            DC_StoreRange(mBuffer + mUnk_f4, mUnk_fc * 2);
                            if (mUnk_108 == 1) {
                                DC_StoreRange(mBuffer2 + mUnk_f4, mUnk_fc * 2);
                            }
                            mUnk_f4 += mUnk_fc;
                            if (mUnk_f4 == mUnk_f8 * mUnk_fc) {
                                mUnk_f4 = 0;
                            }
                            mFrames += 1;
                        } else {
                            func_0203a7c0(mVideo);
                            if (mUnk_108 == 1) {
                                func_0203a7c0(mVideo);
                            }
                        }
                        j = j + (mUnk_108 + 1);
                    } while (j < mUnk_10f);
                    i++;
                }
                if (!mUnk_109) {
                    if (mUnk_e4 >= mUnk_d4) {
                        func_0203c1d8(func_0203a67c(mVideo));
                        mUnk_109 = 1;
                    }
                }
            }
        }
        if (mUnk_e4 >= mUnk_d4 || func_0203a67c(mVideo) == 0) {
            if ((s32) (mFrames - mUnk_00) <= (s32) (mUnk_d8 * 3) && !mUnk_10a && func_0203a67c(mVideo) != 0) {
                func_0203a760(mVideo);
                if (mUnk_10e) {
                    func_0203a760(mVideo2);
                }
                mUnk_10a = 1;
            } else {
                func_0203c3d0();
                mUnk_10a = 0;
            }
        }
        if (((*(vu16 *) 0x27fffa8 & 0x8000) >> 15) == 1) {
            if (mUnk_109 == 1) {
                func_0203c354();
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
            if (mUnk_109 == 1) {
                MIi_CpuClearFast(0, mBuffer, mUnk_f8 * mUnk_fc * 2);
                if (mUnk_108 == 1) {
                    MIi_CpuClearFast(0, mBuffer2, mUnk_f8 * mUnk_fc * 2);
                }
                mFrames -= mUnk_00;
                mUnk_00 = 0;
                mUnk_f4 = (u32) mFrames * mUnk_fc;
                if (mUnk_f4 == mUnk_f8 * mUnk_fc) {
                    mUnk_f4 = 0;
                }
                func_0203c1d8(func_0203a67c(mVideo));
            }
        }
        mUnk_e0++;
        if (mUnk_e0 >= mUnk_dc) {
            result = TRUE;
        } else {
            func_020254f0((void (*)(void *)) func_0203c454, this);
        }
    }
    return result;
}
