// Mobiclip video wrapper, main 0x0203a52c-0x0203afb0
#include <nitro/mi.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

// byte source (vtables _ZTV6Reader 0x020a5948 over _ZTV10ReaderBase 0x020a5924, whose virtuals are pure)
struct ReaderBase {
    ReaderBase() { mSize = 0; mPos = 0; }
    virtual ~ReaderBase();
    virtual int Seek(void *addr) = 0;
    virtual int Read(void *dst, u32 size) = 0;
    virtual int v4() = 0;
    virtual int v5() = 0;
    virtual void Close() = 0;
    s32 mSize;
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
    s32 mFile;   // 0c: the FSFile (MobiclipFile.cpp ReaderInit 'f->file = file')
    s32 mAsyncReadPending; // 10: set by ReaderStartAlignedRead
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
    u32 mFrameCount;         // 0c
    u32 mWidth;          // 10
    u32 mHeight;         // 14
    u32 mRate;           // 18
    u16 mAudioType;      // 1c
    u16 mChannels;       // 1e
    u32 mAudioRate;         // 20
    u32 mUnk_24;         // 24
    u32 mUnk_28;         // 28
    u32 mUnk_2c;         // 2c
    u32 mUnk_30;         // 30
    Ctx *mCtx;           // 34
    u32 mDecodeFunc;         // 38
    u8 mPad3c[0x10];     // 3c
    u32 mConvertWidth;         // 4c
    u32 mConvertHeight;         // 50
    u32 mConvertTable;         // 54
    u8 *mAudio;          // 58
    u8 **mPlaneY;        // 5c
    u8 **mPlaneC;        // 60
    u32 *mUnk_64;        // 64
    void *mUnk_68;       // 68
    void *mUnk_6c;       // 6c
    u8 *mBuf[2];         // 70
    u32 mFrameSize0;         // 78
    u32 mFrameSize1;         // 7c
    u32 mFrameOffset0;         // 80
    u32 mFrameOffset1;         // 84
    u32 mAudioBlockCount0;         // 88
    u32 mAudioBlockCount1;         // 8c
    u32 mBufIndex;         // 90
    u8 *mUnk_94;         // 94
    u32 mUnk_98;         // 98
    u32 mUnk_9c;         // 9c
    u32 mDecodedFrames;         // a0
    u32 mUnk_a4;         // a4
    u32 mNumPlanes;      // a8
    u32 mPlaneIndices[6];      // ac
    u32 mConvertPlaneIndex;         // c4
    u32 mUnk_c8[3];      // c8
    u32 mBase;           // d4
    static void *operator new(unsigned long size);
    static void operator delete(void *p);
    Player();
    ~Player();
};

extern "C" void func_02000b9c(u32 a);
extern "C" int ReaderInit(Reader *r, u32 a);
extern "C" void func_0203b64c(Reader *r);
extern "C" void *MobiclipAlloc(u32 size);
extern "C" void MobiclipFree(void *p);
extern "C" u32 GetDecodeFunc();
extern "C" u32 func_0203bea0();
extern "C" u32 GetConvertTable();
extern "C" u32 func_0203bf80();
extern "C" int PlayerNextFrame(Player *p);
extern "C" int PlayerDecodeFrame(Player *p);
extern "C" int PlayerConvertFrame(Player *p, u32 a, u32 b, int c);
extern "C" int PlayerSkipFrame(Player *p);
extern "C" int PlayerGetAudioBlockCount(Player *p);
extern "C" int PlayerReadAudioBlock(Player *p, void *buffer);
extern "C" int PlayerSkipAudioBlock(Player *p);
int MobiclipInit(Player *p, Reader *r, u32 a);
int PlayerInit(Player *p, Reader *r, u32 a);
void PlayerRelease(Player *p);
extern "C" void func_02092890(); // a label in the hand-written decoder block

int MobiclipInit(Player *p, Reader *r, u32 a) {
    func_02000b9c(0x2000bdc);
    return PlayerInit(p, r, a);
}

Player *MobiclipCreate(Reader *src, u32 a) {
    Reader *r;
    Player *p;
    r = new Reader;
    if (r == 0) {
        return 0;
    }
    if (ReaderInit(r, (u32) src) == 0) {
        delete r;
        return 0;
    }
    p = new Player;
    if (MobiclipInit(p, r, a) == 0) {
        delete p;
        return 0;
    }
    return p;
}

Reader *ReaderDelete(Reader *r) {
    func_0203b64c(r);
    return r;
}

void MobiclipDelete(Player *p) {
    if (p != 0) {
        delete p;
    }
}


int PlayerGetAudioRate(Player *p);
int PlayerGetAudioChannelCount(Player *p);
int PlayerGetFrameRate(Player *p);
int PlayerGetFrameCount(Player *p);
int PlayerGetAudioBlockLength(Player *p);

int MobiclipGetAudioRate(Player *p) {
    if (p == 0) return 0;
    return PlayerGetAudioRate(p);
}
int MobiclipGetAudioChannelCount(Player *p) {
    if (p == 0) return 0;
    return PlayerGetAudioChannelCount(p);
}
int MobiclipGetFrameRate(Player *p) {
    if (p == 0) return 0;
    return PlayerGetFrameRate(p);
}
int MobiclipGetFrameCount(Player *p) {
    if (p == 0) return 0;
    return PlayerGetFrameCount(p);
}
int MobiclipGetAudioBlockLength(Player *p) {
    if (p == 0) return 0;
    return PlayerGetAudioBlockLength(p);
}
int MobiclipNextFrame(Player *p) {
    if (p == 0) return 0;
    return PlayerNextFrame(p) == 1;
}
int MobiclipDecodeFrame(Player *p) {
    if (p == 0) return 0;
    return PlayerDecodeFrame(p) == 1;
}
int MobiclipConvertFrame(Player *p, u32 a, u32 b, int c) {
    if (p == 0) return 0;
    return PlayerConvertFrame(p, a, b, c) == 1;
}
int MobiclipSkipFrame(Player *p) {
    if (p == 0) return 0;
    return PlayerSkipFrame(p) == 1;
}
int MobiclipGetAudioBlockCount(Player *p) {
    if (p == 0) return 0;
    return PlayerGetAudioBlockCount(p);
}
int MobiclipReadAudioBlock(Player *p, void *buffer) {
    if (p == 0) return 0;
    return PlayerReadAudioBlock(p, buffer) == 1;
}
int MobiclipSkipAudioBlock(Player *p) {
    if (p == 0) return 0;
    return PlayerSkipAudioBlock(p) == 1;
}
ReaderBase::~ReaderBase() {}
void *Player::operator new(unsigned long size) { return MobiclipAlloc(size); }
void Player::operator delete(void *p) { MobiclipFree(p); }

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
    PlayerRelease(this);
}

int PlayerInit(Player *p, Reader *r, u32 n) {
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
    p->mCtx = (Ctx *) MobiclipAlloc(0x454);
    MI_CpuClearFast(p->mCtx, 0x454);
    p->mDecodeFunc = GetDecodeFunc();
    p->mCtx->mUnk_3c = func_0203bea0();
    p->mCtx->mUnk_40 = (u32) func_02092890;
    p->mCtx->mUnk_44 = func_0203bf80() + 0x40;
    p->mConvertWidth = p->mWidth;
    p->mConvertHeight = p->mHeight;
    p->mConvertTable = GetConvertTable();
    p->mCtx->mWidth = p->mWidth;
    p->mCtx->mHeight = p->mHeight;
    if (p->mWidth > 0x100) {
        return 0;
    }
    if (n < 6) {
        return 0;
    }
    p->mNumPlanes = n;
    p->mPlaneY = (u8 **) MobiclipAlloc(n * 4);
    if (p->mPlaneY == 0) {
        return 0;
    }
    p->mPlaneC = (u8 **) MobiclipAlloc(p->mNumPlanes * 4);
    if (p->mPlaneC == 0) {
        return 0;
    }
    for (u32 i = 0; i < p->mNumPlanes; i++) {
        p->mPlaneY[i] = (u8 *) MobiclipAlloc(p->mHeight << 8);
        if (p->mPlaneY[i] == 0) {
            return 0;
        }
        p->mPlaneC[i] = (u8 *) MobiclipAlloc((p->mHeight >> 1) << 8);
        if (p->mPlaneC[i] == 0) {
            return 0;
        }
        MI_CpuClearFast(p->mPlaneY[i], p->mHeight << 8);
        MI_CpuFill8(p->mPlaneC[i], 0x80, (p->mHeight >> 1) << 8);
    }
    p->mUnk_64 = (u32 *) MobiclipAlloc(p->mNumPlanes * 4);
    if (p->mUnk_64 == 0) {
        return 0;
    }
    p->mUnk_98 = 0;
    p->mPlaneIndices[0] = 0;
    for (int i = 1; i < 6; i++) {
        p->mPlaneIndices[i] = p->mNumPlanes - i;
    }
    p->mConvertPlaneIndex = 0;
    p->mDecodedFrames = 0;
    p->mUnk_9c = 0;
    for (int i = 0; i < 2; i++) {
        p->mBuf[i] = (u8 *) MobiclipAlloc(p->mUnk_24 + 0x400);
        if (p->mBuf[i] == 0) {
            return 0;
        }
    }
    if (p->mAudioType != 0) {
        p->mAudio = (u8 *) MobiclipAlloc(p->mChannels * 0x14f8);
        if (p->mAudio == 0) {
            return 0;
        }
        MI_CpuClearFast(p->mAudio, p->mChannels * 0x14f8);
    }
    int saved = p->mReader->mPos;
    p->mReader->Seek((void *) (p->mUnk_2c + p->mBase));
    p->mUnk_94 = (u8 *) MobiclipAlloc(p->mUnk_30 << 3);
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
    p->mFrameSize0 = w >> 14;
    p->mAudioBlockCount0 = w & 0x3fff;
    p->mFrameOffset0 = 0;
    p->mReader->Read(p->mBuf[0], p->mFrameSize0 + 4);
    p->mBufIndex = 1;
    return 1;
}

void PlayerRelease(Player *p) {
    if (p->mReader != 0) {
        p->mReader->Close();
        if (p->mReader != 0) {
            delete p->mReader;
        }
    }
    MobiclipFree(p->mCtx);
    if (p->mPlaneY != 0) {
        for (u32 i = 0; i < p->mNumPlanes; i++) {
            MobiclipFree(p->mPlaneY[i]);
        }
        MobiclipFree(p->mPlaneY);
    }
    if (p->mPlaneC != 0) {
        for (u32 i = 0; i < p->mNumPlanes; i++) {
            MobiclipFree(p->mPlaneC[i]);
        }
        MobiclipFree(p->mPlaneC);
    }
    if (p->mUnk_64 != 0) {
        MobiclipFree(p->mUnk_64);
    }
    if (p->mUnk_68 != 0) {
        MobiclipFree(p->mUnk_68);
    }
    if (p->mUnk_6c != 0) {
        MobiclipFree(p->mUnk_6c);
    }
    for (int i = 0; i < 2; i++) {
        if (p->mBuf[i] != 0) {
            MobiclipFree(p->mBuf[i]);
        }
        p->mBuf[i] = 0;
    }
    if (p->mUnk_94 != 0) {
        MobiclipFree(p->mUnk_94);
    }
    if (p->mAudio != 0) {
        MobiclipFree(p->mAudio);
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

int PlayerGetAudioRate(Player *p) { return p->mAudioRate; }

int PlayerGetAudioChannelCount(Player *p) { return p->mChannels; }

int PlayerGetFrameRate(Player *p) { return p->mRate; }

int PlayerGetFrameCount(Player *p) { return p->mFrameCount; }

int PlayerGetAudioBlockLength(Player *p) {
    if (p->mAudioRate == 0) return 0;
    return p->mUnk_28 != 0 ? 0x80 : 0x100;
}

