// Mobiclip video wrapper, main 0x0203a52c-0x0203afb0
#include <nitro/mi.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

// byte source (vtable data_020a5950 over base data_020a592c)
struct ReaderBase {
    ReaderBase() { mUnk_4 = 0; mPos = 0; }
    virtual ~ReaderBase() {}
    virtual int Seek(void *addr);
    virtual int Read(void *dst, u32 size);
    virtual int v4();
    virtual int v5();
    virtual void Close();
    s32 mUnk_4;
    s32 mPos;   // 08
};
struct Reader : ReaderBase {
    static void *operator new(unsigned long size);
    virtual ~Reader();
    virtual int Seek(void *addr);
    virtual int Read(void *dst, u32 size);
    virtual int v4();
    virtual int v5();
    virtual void Close();
    s32 mEnd;   // 0c
    s32 mUnk_10;
};

struct Ctx {
    u32 mUnk_0;
    u32 mWidth;  // 04
    u32 mHeight; // 08
    u8 mPad[0x30];
    u32 mUnk_3c;
    u32 mUnk_40;
    u32 mUnk_44;
};

struct AudioCh {
    u8 mPad[0x8c0];
    u8 mData[0xc34];
    u32 mMode;
};

struct Player {
    Reader *mReader;     // 00
    char mMagic[6];      // 04
    u16 mVersion;        // 0a
    u32 mUnk_c;          // 0c
    u32 mWidth;          // 10
    u32 mHeight;         // 14
    u32 mRate;           // 18
    u16 mAudioType;      // 1c
    u16 mChannels;       // 1e
    u32 mFrames;         // 20
    u32 mUnk_24;         // 24
    u32 mUnk_28;         // 28
    u32 mUnk_2c;         // 2c
    u32 mUnk_30;         // 30
    Ctx *mCtx;           // 34
    u32 mUnk_38;         // 38
    u8 mPad3c[0x10];     // 3c
    u32 mUnk_4c;         // 4c
    u32 mUnk_50;         // 50
    u32 mUnk_54;         // 54
    u8 *mAudio;          // 58
    u8 **mPlaneY;        // 5c
    u8 **mPlaneC;        // 60
    u32 *mUnk_64;        // 64
    void *mUnk_68;       // 68
    void *mUnk_6c;       // 6c
    u8 *mBuf[2];         // 70
    u32 mUnk_78;         // 78
    u32 mUnk_7c;         // 7c
    u32 mUnk_80;         // 80
    u32 mUnk_84;         // 84
    u32 mUnk_88;         // 88
    u32 mUnk_8c;         // 8c
    u32 mUnk_90;         // 90
    u8 *mUnk_94;         // 94
    u32 mUnk_98;         // 98
    u32 mUnk_9c;         // 9c
    u32 mUnk_a0;         // a0
    u32 mUnk_a4;         // a4
    u32 mNumPlanes;      // a8
    u32 mUnk_ac[6];      // ac
    u32 mUnk_c4;         // c4
    u32 mUnk_c8[3];      // c8
    u32 mBase;           // d4
    static void *operator new(unsigned long size);
    static void operator delete(void *p);
    Player();
    ~Player();
};

extern "C" void func_02000b9c(u32 a);
extern "C" void *func_0203b640(u32 size);
extern "C" int func_0203b658(Reader *r, u32 a);
extern "C" void func_0203b64c(Reader *r);
extern "C" void *func_0203c15c(u32 size);
extern "C" void func_0203c174(void *p);
extern "C" u32 func_0203be24();
extern "C" u32 func_0203bea0();
extern "C" u32 func_0203bf10();
extern "C" u32 func_0203bf80();
extern "C" int func_0203afb0(Player *p);
extern "C" int func_0203b0e8(Player *p);
extern "C" int func_0203b1cc(Player *p, u32 a, u32 b, int c);
extern "C" int func_0203b350(Player *p);
extern "C" int func_0203b390(Player *p);
extern "C" int func_0203b3a0(Player *p, void *buffer);
extern "C" int func_0203b550(Player *p);
int func_0203a544(Player *p, Reader *r, u32 a);
int func_0203a858(Player *p, Reader *r, u32 a);
void func_0203ae04(Player *p);
void _ZdlPv(void *p);
extern u32 data_020a592c, data_020a5950;
void func_02092890();

int func_0203a544(Player *p, Reader *r, u32 a) {
    func_02000b9c(0x2000bdc);
    return func_0203a858(p, r, a);
}

Player *func_0203a574(Reader *src, u32 a) {
    Reader *r;
    Player *p;
    r = new Reader;
    if (r == 0) {
        return 0;
    }
    if (func_0203b658(r, (u32) src) == 0) {
        delete r;
        return 0;
    }
    p = new Player;
    if (func_0203a544(p, r, a) == 0) {
        delete p;
        return 0;
    }
    return p;
}

Reader *func_0203a648(Reader *r) {
    func_0203b64c(r);
    return r;
}

void func_0203a65c(Player *p) {
    if (p != 0) {
        delete p;
    }
}


int func_0203af6c(Player *p);
int func_0203af74(Player *p);
int func_0203af7c(Player *p);
int func_0203af84(Player *p);
int func_0203af8c(Player *p);

int func_0203a67c(Player *p) {
    if (p == 0) return 0;
    return func_0203af6c(p);
}
int func_0203a694(Player *p) {
    if (p == 0) return 0;
    return func_0203af74(p);
}
int func_0203a6ac(Player *p) {
    if (p == 0) return 0;
    return func_0203af7c(p);
}
int func_0203a6c4(Player *p) {
    if (p == 0) return 0;
    return func_0203af84(p);
}
int func_0203a6dc(Player *p) {
    if (p == 0) return 0;
    return func_0203af8c(p);
}
int func_0203a6f4(Player *p) {
    if (p == 0) return 0;
    return func_0203afb0(p) == 1;
}
int func_0203a718(Player *p) {
    if (p == 0) return 0;
    return func_0203b0e8(p) == 1;
}
int func_0203a73c(Player *p, u32 a, u32 b, int c) {
    if (p == 0) return 0;
    return func_0203b1cc(p, a, b, c) == 1;
}
int func_0203a760(Player *p) {
    if (p == 0) return 0;
    return func_0203b350(p) == 1;
}
int func_0203a784(Player *p) {
    if (p == 0) return 0;
    return func_0203b390(p);
}
int func_0203a79c(Player *p, void *buffer) {
    if (p == 0) return 0;
    return func_0203b3a0(p, buffer) == 1;
}
int func_0203a7c0(Player *p) {
    if (p == 0) return 0;
    return func_0203b550(p) == 1;
}
void func_0203a7e4() {}
void *func_0203a7e8(void *p) {
    _ZdlPv(p);
    return p;
}
void *Player::operator new(unsigned long size) { return func_0203c15c(size); }
void Player::operator delete(void *p) { func_0203c174(p); }

Player::Player() {
    mReader = 0;
    mPlaneY = 0;
    mPlaneC = 0;
    mUnk_68 = 0;
    mUnk_6c = 0;
    mBuf[0] = 0;
    mBuf[1] = 0;
    mUnk_64 = 0;
    mAudio = 0;
    mUnk_94 = 0;
}

Player::~Player() {
    func_0203ae04(this);
}

int func_0203af6c(Player *p) { return p->mFrames; }
int func_0203af74(Player *p) { return p->mChannels; }
int func_0203af7c(Player *p) { return p->mRate; }
int func_0203af84(Player *p) { return p->mUnk_c; }
int func_0203af8c(Player *p) {
    if (p->mFrames == 0) return 0;
    return p->mUnk_28 != 0 ? 0x80 : 0x100;
}

void func_0203ae04(Player *p) {
    if (p->mReader != 0) {
        p->mReader->Close();
        if (p->mReader != 0) {
            delete p->mReader;
        }
    }
    func_0203c174(p->mCtx);
    if (p->mPlaneY != 0) {
        for (u32 i = 0; i < p->mNumPlanes; i++) {
            func_0203c174(p->mPlaneY[i]);
        }
        func_0203c174(p->mPlaneY);
    }
    if (p->mPlaneC != 0) {
        for (u32 i = 0; i < p->mNumPlanes; i++) {
            func_0203c174(p->mPlaneC[i]);
        }
        func_0203c174(p->mPlaneC);
    }
    if (p->mUnk_64 != 0) {
        func_0203c174(p->mUnk_64);
    }
    if (p->mUnk_68 != 0) {
        func_0203c174(p->mUnk_68);
    }
    if (p->mUnk_6c != 0) {
        func_0203c174(p->mUnk_6c);
    }
    for (int i = 0; i < 2; i++) {
        if (p->mBuf[i] != 0) {
            func_0203c174(p->mBuf[i]);
        }
        p->mBuf[i] = 0;
    }
    if (p->mUnk_94 != 0) {
        func_0203c174(p->mUnk_94);
    }
    if (p->mAudio != 0) {
        func_0203c174(p->mAudio);
    }
    p->mReader = 0;
    p->mPlaneY = 0;
    p->mPlaneC = 0;
    p->mUnk_68 = 0;
    p->mUnk_6c = 0;
    p->mUnk_64 = 0;
    p->mAudio = 0;
    p->mUnk_94 = 0;
}

int func_0203a858(Player *p, Reader *r, u32 n) {
    p->mReader = r;
    p->mBase = r->mPos;
    if (!p->mReader->Read(p->mMagic, 0x30)) {
        return 0;
    }
    if (p->mMagic[0] != 'M' || p->mMagic[1] != 'O' || p->mMagic[2] != 'D' || p->mMagic[3] != 'S' ||
        p->mMagic[4] != 'N' || (p->mMagic[5] != '2' && p->mMagic[5] != '3') || p->mVersion != 10) {
        return 0;
    }
    if (p->mMagic[4] == 'N' && p->mMagic[5] == '3') {
        char tag[2];
        u16 off;
        do {
            if (!p->mReader->Read(tag, 2)) {
                return 0;
            }
            if (!p->mReader->Read(&off, 2)) {
                return 0;
            }
            if (!p->mReader->Seek((void *) ((s32) (off << 2) + p->mReader->mPos))) {
                return 0;
            }
        } while (!(tag[0] == 'H' && tag[1] == 'E'));
    }
    if (p->mAudioType != 0 && p->mAudioType != 1 && p->mAudioType != 2 && p->mAudioType != 3 &&
        p->mAudioType != 4) {
        return 0;
    }
    p->mCtx = (Ctx *) func_0203c15c(0x454);
    MI_CpuClearFast(p->mCtx, 0x454);
    p->mUnk_38 = func_0203be24();
    p->mCtx->mUnk_3c = func_0203bea0();
    p->mCtx->mUnk_40 = (u32) func_02092890;
    p->mCtx->mUnk_44 = func_0203bf80() + 0x40;
    p->mUnk_4c = p->mWidth;
    p->mUnk_50 = p->mHeight;
    p->mUnk_54 = func_0203bf10();
    p->mCtx->mWidth = p->mWidth;
    p->mCtx->mHeight = p->mHeight;
    if (p->mWidth > 0x100) {
        return 0;
    }
    if (n < 6) {
        return 0;
    }
    p->mNumPlanes = n;
    p->mPlaneY = (u8 **) func_0203c15c(n * 4);
    if (p->mPlaneY == 0) {
        return 0;
    }
    p->mPlaneC = (u8 **) func_0203c15c(p->mNumPlanes * 4);
    if (p->mPlaneC == 0) {
        return 0;
    }
    for (u32 i = 0; i < p->mNumPlanes; i++) {
        p->mPlaneY[i] = (u8 *) func_0203c15c(p->mHeight << 8);
        if (p->mPlaneY[i] == 0) {
            return 0;
        }
        p->mPlaneC[i] = (u8 *) func_0203c15c((p->mHeight >> 1) << 8);
        if (p->mPlaneC[i] == 0) {
            return 0;
        }
        MI_CpuClearFast(p->mPlaneY[i], p->mHeight << 8);
        MI_CpuFill8(p->mPlaneC[i], 0x80, (p->mHeight >> 1) << 8);
    }
    p->mUnk_64 = (u32 *) func_0203c15c(p->mNumPlanes * 4);
    if (p->mUnk_64 == 0) {
        return 0;
    }
    p->mUnk_98 = 0;
    p->mUnk_ac[0] = 0;
    for (int i = 1; i < 6; i++) {
        p->mUnk_ac[i] = p->mNumPlanes - i;
    }
    p->mUnk_c4 = 0;
    p->mUnk_a0 = 0;
    p->mUnk_9c = 0;
    for (int i = 0; i < 2; i++) {
        p->mBuf[i] = (u8 *) func_0203c15c(p->mUnk_24 + 0x400);
        if (p->mBuf[i] == 0) {
            return 0;
        }
    }
    if (p->mAudioType != 0) {
        p->mAudio = (u8 *) func_0203c15c(p->mChannels * 0x14f8);
        if (p->mAudio == 0) {
            return 0;
        }
        MI_CpuClearFast(p->mAudio, p->mChannels * 0x14f8);
    }
    int saved = p->mReader->mPos;
    p->mReader->Seek((void *) (p->mUnk_2c + p->mBase));
    p->mUnk_94 = (u8 *) func_0203c15c(p->mUnk_30 << 3);
    if (!p->mReader->Read(p->mUnk_94, p->mUnk_30 << 3)) {
        return 0;
    }
    p->mReader->Seek((void *) saved);
    if (p->mAudioType == 1) {
        int i;
        int saved2 = p->mReader->mPos;
        p->mReader->Seek((void *) (p->mUnk_28 + p->mBase));
        for (i = 0; i < p->mChannels; i++) {
            if (!p->mReader->Read(p->mAudio + i * 0x14f8 + 0x8c0, 0xc34)) {
                return 0;
            }
            *(u32 *) (p->mAudio + i * 0x14f8 + 0x14f4) = 2;
        }
        p->mReader->Seek((void *) saved2);
    } else if (p->mAudioType == 2) {
        for (int i = 0; i < p->mChannels; i++) {
            *(u32 *) (p->mAudio + i * 0x14f8 + 0x1cc) = 2;
        }
    }
    u32 w;
    p->mReader->Read(&w, 4);
    p->mUnk_78 = w >> 14;
    p->mUnk_88 = w & 0x3fff;
    p->mUnk_80 = 0;
    p->mReader->Read(p->mBuf[0], p->mUnk_78 + 4);
    p->mUnk_90 = 1;
    return 1;
}
