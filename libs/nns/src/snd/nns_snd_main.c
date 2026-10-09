// NitroSystem sound library: NNS_Snd init / shutdown / master volume and channel and capture reservation. 0x020366f8-0x02036918
#include <nnsys/private/nns_types.h>

typedef struct {
    s8 mainVolume;           // 0x00
    u8 pad1[3];
    u32 one;                 // 0x04
    u32 pad8;
    u32 initialised;         // 0x0c
    void (*cb0)(void);       // 0x10
    u32 cb0Arg;              // 0x14
    u32 pad18;
    void (*cb1)(void);       // 0x1c
    u32 cb1Arg;              // 0x20
} SndMain;
extern SndMain data_02211600;
extern u8 data_02211610[];
extern u8 data_0221161c[];

typedef struct {
    u32 lockedMask;          // 0x00
    u32 usedBits;            // 0x04
    u32 reservedMask;        // 0x08
} SndRes;
extern SndRes data_02213a00;

extern void func_020082e8(void);
extern void func_0200da20(void *);
extern void func_0200da38(void *);
extern void func_02037884(void);
extern void func_02036bf8(void);
extern int func_0200841c(int);
extern void func_02036cac(void);
extern void func_0203789c(void);
extern void func_020394c8(void);
extern void func_020085ec(int);
extern void func_0200820c(u32);
extern void func_02037a14(void);
extern void func_02008034(u32, u32, u32, u32);
extern u32 func_02008814(void);
extern void func_020087a8(u32);
extern void func_02037a74(void);
extern void func_020080d8(u32, u32);
extern void func_020080f8(u32, u32);
void func_020367bc(void);
void func_020367f4(void);
void func_020368fc(void);

void func_020366f8(void) {   // NNS_SndInit
    if (data_02211600.initialised) return;
    data_02211600.initialised = 1;
    func_020082e8();
    data_02211600.cb0 = func_020367bc;
    data_02211600.cb0Arg = 0;
    data_02211600.cb1 = func_020367f4;
    data_02211600.cb1Arg = 0;
    func_0200da20(data_02211610);
    func_0200da38(data_0221161c);
    func_020368fc();
    func_02037884();
    func_02036bf8();
    data_02211600.mainVolume = -1;
    data_02211600.one = 1;
}

void func_02036780(void) {   // shutdown: wait for the sound thread, stop everything
    int i = 0;
    while (func_0200841c(i) != 0) {}
    func_02036cac();
    func_0203789c();
    func_020394c8();
    func_020085ec(0);
}

void func_020367b0(u32 volume) {   // NNS_SndSetMasterVolume
    func_0200820c(volume);
}

void func_020367bc(void) {   // start callback
    u32 r;
    func_02037a14();
    func_02008034(0, 0, 0, 0);
    r = func_02008814();
    func_020085ec(1);
    func_020087a8(r);
}

void func_020367f4(void) {
    func_02037a74();
}

int func_02036800(u32 mask) {   // NNS_SndLockChannel
    if (mask == 0) return 1;
    if (mask & data_02213a00.reservedMask) return 0;
    func_020080d8(mask, 0);
    data_02213a00.reservedMask |= mask;
    return 1;
}

void func_02036848(u32 mask) {   // NNS_SndUnlockChannel
    if (mask == 0) return;
    func_020080f8(mask, 0);
    data_02213a00.reservedMask &= ~mask;
}

void func_02036878(u32 mask) {
    data_02213a00.lockedMask &= ~mask;
}

int func_02036894(void) {   // take the lowest free slot of 8
    u32 bit = 1;
    int i;
    for (i = 0; i < 8; i++, bit <<= 1) {
        if (!(data_02213a00.usedBits & bit)) {
            data_02213a00.usedBits |= bit;
            return i;
        }
    }
    return -1;
}

void func_020368dc(int slot) {   // release a slot
    data_02213a00.usedBits &= ~(1 << slot);
}

void func_020368fc(void) {
    data_02213a00.reservedMask = 0;
    data_02213a00.lockedMask = 0;
    data_02213a00.usedBits = 0;
}
