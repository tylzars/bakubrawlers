#include "Graphics/Camera.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Scene/SceneManager.hpp"
#include "System/FrameClock.hpp"
#include "System/FrameTask.hpp"
#include "System/FrameUpdate.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "System/WirelessManager.hpp"
#include "Text/TextManager.hpp"
#include "Unknown/UnkStruct_020c17a8.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include <nitro/gx.h>
#include <nitro/os.h>

class SceneRunner : public SceneBase {
public:
    SceneRunner();
    virtual ~SceneRunner() {}
    virtual void Update();
    static SceneRunner &Get() {
        static SceneRunner instance;
        return instance;
    }
};

class MicManager {
public:
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u8 mActive;
    /* 21 */ PAD(0x21, 0x28);
    MicManager() {
        mActive = 0;
    }
    ~MicManager() {}
    static MicManager &Get() {
        static MicManager instance;
        return instance;
    }
    void func_0202d794();
    void func_0202d94c();
};

extern "C" {
void OS_Init(void);
void OS_ResetSystem(u32 unk1);
void OS_InitThread(void);
void GX_Init(void);
void func_02001b34(void);
void func_0200a78c(s32 unk1);
void func_0200f4c8(void);
void OS_InitTick(void);
void OS_InitAlarm(void);
void func_0200db0c(void);
void GX_DispOn(void);
void OS_SetIrqFunction(u32 mask, void (*func)(void));
void OS_EnableIrqMask(u32 mask);
void GX_VBlankIntr(BOOL enable);
void GX_HBlankIntr(BOOL enable);
int func_0200d354(void *a, void *b);
u32 func_0200af3c(u32 a, u32 b);
u32 func_02021c60(u32 a);
void func_020234c8(void);
void func_020219dc(void);
void func_02022b0c(void);
void func_02022b28(void);
void func_02025e18(void);
void func_02023884(void);
void func_02025d74(void);
void func_0202555c(void);
u64 OS_GetTick(void);
u64 _ll_udiv(u64 a, u64 b);
int func_0200f79c(void);
void func_0200f7ac(void);
int func_0200d1e0(u32 a, u32 b);
void func_0200d3e0(u32 a, u32 b, u32 c);
}
extern u32 data_020a4f24;
extern const char *data_020a544c[];
void func_02024598(SoundManager *unused);
void func_02026848(CameraManager *unused);
void func_02001eec(vu16 *reg, int value);
void UpdateFrame();
void func_020254bc();
void func_020255f8(void (*callback)());
extern u8 data_020c427e;
void SetFrameInterval(u8 frames);


const char *UnkStruct_020c1884::func_0202030c() {
    return data_020a544c[data_020d433c.mUnk_79];
}

void UnkStruct_020c1884::func_02020328() {
    u32 heap;
    OS_Init();
    OS_InitThread();
    data_020a4f24 = 3;
    GX_Init();
    func_02001b34();
    func_0200a78c(-1);
    func_0200f4c8();
    OS_InitTick();
    OS_InitAlarm();
    func_0200db0c();
    reg_GX_POWCNT = (u16) ((reg_GX_POWCNT & ~0x20e) | 0xe | 0x200);
    func_02001eec((vu16 *) 0x400006c, 0x10);
    func_02001eec((vu16 *) 0x400106c, 0x10);
    GX_DispOn();
    reg_GXS_DB_DISPCNT |= 0x10000;
    func_0202555c();
    OS_SetIrqFunction(1, UpdateFrame);
    func_020255f8(NULL);
    OS_SetIrqFunction(2, func_020254bc);
    OS_EnableIrqMask(0x40003);
    GX_VBlankIntr(TRUE);
    GX_HBlankIntr(TRUE);
    u16 prev = *(vu16 *) 0x4000208;
    *(vu16 *) 0x4000208 = 1;
    while (func_0200d354(this, &mUnk_04) == 1) {
    }
    heap = func_0200af3c(0, 0);
    func_0200af3c(func_02021c60(heap), heap);
    FrameTask::Get().Init();
    func_020234c8();
    SpriteManager::Get().func_02026d48();
    InputManager::Get().Init();
    TouchManager::Get().func_020225d0();
    TextManager::Get().func_0202c02c();
    SoundManager::Get().func_020244cc();
    WirelessManager::Get().func_0202ed2c();
    MicManager::Get().func_0202d794();
    FrameTask::Get().func_0203dbc4();
    mUnk_09 = 1;
    mUnk_08 = 0;
    mUnk_15 = 0;
    func_020219dc();
    SceneManager &manager = SceneManager::Get();
    UnkStruct_SceneContext *context = (UnkStruct_SceneContext *) manager.Init(&SceneRunner::Get());
    SceneRunner::Get().mContext = context;
    func_02022b0c();
    func_02022b28();
    SpriteManager::Get().func_02026dec(0, 1);
    reg_GX_DISPCNT &= ~0x38000000;
    reg_GX_DISPCNT &= ~0x7000000;
    reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xffcfffef) | 0x10 | 0x100000;
    reg_GXS_DB_DISPCNT = (reg_GXS_DB_DISPCNT & 0xffcfffef) | 0x200010;
    reg_G2_BG0CNT = (u16) (reg_G2_BG0CNT & ~0x40);
    reg_G2_BG1CNT = (u16) (reg_G2_BG1CNT & ~0x40);
    reg_G2_BG2CNT = (u16) (reg_G2_BG2CNT & ~0x40);
    reg_G2_BG3CNT = (u16) (reg_G2_BG3CNT & ~0x40);
    reg_G2S_DB_BG0CNT = (u16) (reg_G2S_DB_BG0CNT & ~0x40);
    reg_G2S_DB_BG1CNT = (u16) (reg_G2S_DB_BG1CNT & ~0x40);
    reg_G2S_DB_BG2CNT = (u16) (reg_G2S_DB_BG2CNT & ~0x40);
    reg_G2S_DB_BG3CNT = (u16) (reg_G2S_DB_BG3CNT & ~0x40);
    CameraManager::Get().func_020264e0();
    func_02025e18();
    mUnk_0d = 1;
    mUnk_0b = 1;
    mUnk_0c = 1;
    UnkStruct_020c17a8::Get().func_02023b80();
    mUnk_14 = 0;
}

// The singletons' destructors are defined here, in the order the original emits them.
FrameTask::~FrameTask() {}
InputManager::~InputManager() {}
TouchManager::~TouchManager() {}
TextManager::~TextManager() {}
SoundManager::~SoundManager() {}
CameraManager::~CameraManager() {}
UnkStruct_020c17a8::~UnkStruct_020c17a8() {}

static inline u8 IsAtLeast3(const u8 *value) {
    return (*value >= 3) & 1;
}

void UnkStruct_020c1884::func_02020a48() {
    u64 prev;
    u32 elapsed = 0;
    mFrame = 0;
    for (;;) {
        if (data_020d43f0[0].mPlayFrames < 0xcdfe5c4) {
            data_020d43f0[0].mPlayFrames++;
        }
        mUnk_0d = elapsed / 16000 + 1;
        if (mUnk_0d < mUnk_0b) {
            mUnk_0d = mUnk_0b;
        }
        if (mUnk_0d > mUnk_0c) {
            mUnk_0d = mUnk_0c;
        }
        SetFrameInterval(mUnk_0d);
        mUnk_0a = 0;
        if (mUnk_0d > 1) {
            CameraManager::Get().func_0202655c();
            func_02026848(&CameraManager::Get());
            CameraManager::Get().mUnk_01 = 0;
            do {
                InputManager::Get().Update(mUnk_09);
                TouchManager::Get().Update(mUnk_09);
                FrameTask::Get().func_0203dc04();
                func_02024598(&SoundManager::Get());
                SceneManager::Get().Update();
                mFrame++;
                InputManager::Get();
                SpriteManager::Get().Screen(0).func_02027750();
                SpriteManager::Get().Screen(1).func_02027750();
                mUnk_0a++;
            } while (mUnk_0a < mUnk_0d - 1);
        }
        InputManager::Get().Update(mUnk_09);
        TouchManager::Get().Update(mUnk_09);
        FrameTask::Get().func_0203dc04();
        func_02024598(&SoundManager::Get());
        CameraManager::Get().mUnk_01 = 1;
        CameraManager::Get().func_0202655c();
        func_02026848(&CameraManager::Get());
        SceneManager::Get().Update();
        mFrame++;
        InputManager::Get();
        if (IsAtLeast3(&mUnk_08)) {
            WirelessManager::Get();
            if (data_020c4258 != 0) {
                while (WirelessManager::Get(), data_020c4258 != 0) {
                    WirelessManager::Get();
                    if (data_020c4258 != 8) {
                        WirelessManager::Get().func_0202edd0();
                    }
                    FrameTask::Get().func_0203dc10();
                    WirelessManager::Get().func_0202f244();
                }
                func_02001eec((vu16 *) 0x400006c, -16);
                func_02001eec((vu16 *) 0x400106c, -16);
                OS_ResetSystem(1);
            } else {
                if (UnkStruct_020c17a8::Get().mBusy != 0) {
                    mUnk_08 = 0;
                } else {
                    func_02001eec((vu16 *) 0x400006c, -16);
                    func_02001eec((vu16 *) 0x400106c, -16);
                    OS_ResetSystem(1);
                }
            }
        } else {
            if (InputManager::Get().IsDown(0x30c) == 0x30c) {
                mUnk_08++;
                if (mUnk_08 > 3) {
                    mUnk_08 = 3;
                }
            } else {
                mUnk_08 = 0;
            }
        }
        func_02021700();
        func_02023884();
        func_02025d74();
        SpriteManager::Get().func_02026cc0();
        u64 tick = OS_GetTick();
        FrameTask::Get().func_0203dc10();
        elapsed = (u32) ((tick - prev) * 64000 / 33514);
        prev = OS_GetTick();
        WirelessManager::Get().func_0202f244();
        mUnk_15 = 0;
        WirelessManager::Get();
        if (IsWirelessMode(&data_020c4258)) {
            while (WirelessManager::Get(), data_020c427e == 0) {
                mUnk_15++;
                FrameTask::Get().func_0203dc10();
                WirelessManager::Get().func_0202f244();
                WirelessManager::Get();
                if (!IsWirelessMode(&data_020c4258)) {
                    break;
                }
            }
        }
        if (mUnk_14 != 0) {
            for (;;) {
                func_02021700();
                FrameTask::Get().func_0203dc10();
                WirelessManager::Get().func_0202f244();
                func_02024598(&SoundManager::Get());
            }
        }
    }
}

void UnkStruct_020c1884::func_02021700() {
    if ((*(vu16 *) 0x27fffa8 & 0x8000) >> 15) {
        MicManager::Get().func_0202d94c();
        if (UnkStruct_020c17a8::Get().mBusy != 0) {
            if (mUnk_09 != 0) {
                while (func_0200d1e0(2, 0) != 0) {
                }
                mUnk_09 = 0;
            }
        } else {
            if (WirelessManager::Get().mUnk_1228 != 0) {
                if (mUnk_09 != 0) {
                    while (func_0200d1e0(2, 0) != 0) {
                    }
                    mUnk_09 = 0;
                }
            } else {
                func_0200d3e0(0xc, 0, 0);
                MicManager::Get().func_0202d94c();
            }
        }
    } else if (mUnk_09 == 0) {
        while (func_0200d1e0(0, mUnk_00) != 0) {
        }
        while (func_0200d1e0(1, mUnk_04) != 0) {
        }
        MicManager::Get().func_0202d94c();
        mUnk_09 = 1;
    }
    if (func_0200f79c() != 0) {
        func_0200f7ac();
    }
    if (mUnk_09 == 0) {
        MicManager::Get().func_0202d94c();
    }
}

extern "C" void func_01ff9284(void *buffer);
extern void *data_020baf00;

void main() {
    u8 buffer[0x304];
    u8 state[0x188];
    UnkStruct_020c1884::Get().func_02020328();
    func_01ff9284(buffer);
    data_020baf00 = state;
    UnkStruct_020c1884::Get().func_02020a48();
}
