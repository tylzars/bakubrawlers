// Game.cpp saw six singleton classes with an empty constructor that no other file has (their objects join guard
// first here, node first everywhere else); the headers declare it only under this macro.
#define GAME_SINGLETON_CTORS

#include "System/FrameUpdate.hpp"
#include "Scene/SceneBase.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "Game/UnkStruct_020d43f0.hpp"
#include <nitro/gx.h>
#include <nitro/os.h>

const char *gLanguageSuffixes[] = {NULL, "F", "G", "I", "S", "D", "V", "J"};
#include "System/FrameTask.hpp"
#include "Graphics/SpriteManager.hpp"
#include "System/InputManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "System/SoundManager.hpp"
#include "System/WirelessManager.hpp"
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

#include "Scene/SceneManager.hpp"
class SceneRunner : public SceneBase {
public:
    /* 0c */ s32 mOverlay;
    /* 10 */ s16 mNextScene;
    /* 12 */ u8 mOverlayState;
    /* 13 */

    SceneRunner();
    virtual ~SceneRunner() {}
    virtual void Update();
    static SceneRunner &Get() {
        static SceneRunner instance;
        return instance;
    }
};

#include "Graphics/Camera.hpp"
#include "Unknown/UnkStruct_020c17a8.hpp"
#include "System/GameSystem.hpp"
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
extern "C" void RTC_Init(void);
void GX_DispOn(void);
void OS_SetIrqFunction(u32 mask, void (*func)(void));
void OS_EnableIrqMask(u32 mask);
void GX_VBlankIntr(BOOL enable);
void GX_HBlankIntr(BOOL enable);
int func_0200d354(void *a, void *b);
u32 func_0200af3c(u32 a, u32 b);
u64 OS_GetTick(void);
u64 _ll_udiv(u64 a, u64 b);
int func_0200f79c(void);
void func_0200f7ac(void);
int func_0200d1e0(u32 a, u32 b);
void func_0200d3e0(u32 a, u32 b, u32 c);
}
u32 func_02021c60(u32 a);
void func_020234c8(void);
void func_020219dc(void);
void func_02022b0c(void);
void func_02022b28(void);
void func_02025e18(void);
void func_02023884(void);
void func_02025d74(void);
void func_0202555c(void);
extern u32 data_020a4f24;
extern "C" void G2x_SetMasterBrightness_(vu16 *reg, int value);
void UpdateFrame();
void func_020254bc();
void func_020255f8(void (*callback)());
void SetFrameInterval(u8 frames);

// Keys that, all down (IsDown) for three passes of Run, reset the system (OS_ResetSystem(1)): when
// WirelessManager::mUnk_1228 is non-zero, after Run waits for it to reach 0; when it is 0, only if no save is running
// (UnkStruct_020c17a8::mBusy == 0), otherwise the count restarts.
#define RESET_KEYS 0x30c


const char *GameSystem::GetLanguageSuffix() {
    return gLanguageSuffixes[data_020d433c.mUnk_79];
}

// The singletons' destructors are inline here only: like the inline ones from the headers, they come out as weak
// copies after Init in the order it first uses each singleton, which is the original order.
inline FrameTask::~FrameTask() {}
inline InputManager::~InputManager() {}
inline TouchManager::~TouchManager() {}
inline TextManager::~TextManager() {}
inline SoundManager::~SoundManager() {}
inline CameraManager::~CameraManager() {}
inline UnkStruct_020c17a8::~UnkStruct_020c17a8() {}

void GameSystem::Init() {
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
    RTC_Init();
    reg_GX_POWCNT = (u16) ((reg_GX_POWCNT & ~0x20e) | 0xe | 0x200);
    G2x_SetMasterBrightness_((vu16 *) 0x400006c, 0x10);
    G2x_SetMasterBrightness_((vu16 *) 0x400106c, 0x10);
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
    mInputEnabled = 1;
    mResetHoldFrames = 0;
    mWirelessWaitFrames = 0;
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
    mFrameInterval = 1;
    mMinInterval = 1;
    mMaxInterval = 1;
    UnkStruct_020c17a8::Get().func_02023b80();
    mHalted = 0;
}

static inline u8 IsAtLeast3(const u8 *value) {
    return (*value >= 3) & 1;
}

void GameSystem::Run() {
    u64 prev;
    u32 elapsed = 0;
    mFrame = 0;
    for (;;) {
        if (data_020d43f0[0].mPlayFrames < 0xcdfe5c4) {
            data_020d43f0[0].mPlayFrames++;
        }
        mFrameInterval = elapsed / 16000 + 1;
        if (mFrameInterval < mMinInterval) {
            mFrameInterval = mMinInterval;
        }
        if (mFrameInterval > mMaxInterval) {
            mFrameInterval = mMaxInterval;
        }
        SetFrameInterval(mFrameInterval);
        mExtraFrames = 0;
        if (mFrameInterval > 1) {
            CameraManager::Get().func_0202655c();
            CameraManager::Get().func_02026848();
            CameraManager::Get().mUnk_01 = 0;
            do {
                InputManager::Get().Update(mInputEnabled);
                TouchManager::Get().Update(mInputEnabled);
                FrameTask::Get().func_0203dc04();
                SoundManager::Get().func_02024598();
                SceneManager::Get().Update();
                mFrame++;
                InputManager::Get();
                SpriteManager::Get().Screen(0).func_02027750();
                SpriteManager::Get().Screen(1).func_02027750();
                mExtraFrames++;
            } while (mExtraFrames < mFrameInterval - 1);
        }
        InputManager::Get().Update(mInputEnabled);
        TouchManager::Get().Update(mInputEnabled);
        FrameTask::Get().func_0203dc04();
        SoundManager::Get().func_02024598();
        CameraManager::Get().mUnk_01 = 1;
        CameraManager::Get().func_0202655c();
        CameraManager::Get().func_02026848();
        SceneManager::Get().Update();
        mFrame++;
        InputManager::Get();
        if (IsAtLeast3(&mResetHoldFrames)) {
            if (WirelessManager::Get().mUnk_1228 != 0) {
                while (WirelessManager::Get().mUnk_1228 != 0) {
                    if (WirelessManager::Get().mUnk_1228 != 8) {
                        WirelessManager::Get().func_0202edd0();
                    }
                    FrameTask::Get().func_0203dc10();
                    WirelessManager::Get().func_0202f244();
                }
                G2x_SetMasterBrightness_((vu16 *) 0x400006c, -16);
                G2x_SetMasterBrightness_((vu16 *) 0x400106c, -16);
                OS_ResetSystem(1);
            } else {
                if (UnkStruct_020c17a8::Get().mBusy != 0) {
                    mResetHoldFrames = 0;
                } else {
                    G2x_SetMasterBrightness_((vu16 *) 0x400006c, -16);
                    G2x_SetMasterBrightness_((vu16 *) 0x400106c, -16);
                    OS_ResetSystem(1);
                }
            }
        } else {
            if (InputManager::Get().IsDown(RESET_KEYS) == RESET_KEYS) {
                mResetHoldFrames++;
                if (mResetHoldFrames > 3) {
                    mResetHoldFrames = 3;
                }
            } else {
                mResetHoldFrames = 0;
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
        mWirelessWaitFrames = 0;
        if (IsWirelessMode((const u8 *) &WirelessManager::Get().mUnk_1228)) {
            while (WirelessManager::Get().mUnk_124e == 0) {
                mWirelessWaitFrames++;
                FrameTask::Get().func_0203dc10();
                WirelessManager::Get().func_0202f244();
                if (!IsWirelessMode((const u8 *) &WirelessManager::Get().mUnk_1228)) {
                    break;
                }
            }
        }
        if (mHalted != 0) {
            for (;;) {
                func_02021700();
                FrameTask::Get().func_0203dc10();
                WirelessManager::Get().func_0202f244();
                SoundManager::Get().func_02024598();
            }
        }
    }
}

void GameSystem::func_02021700() {
    if ((*(vu16 *) 0x27fffa8 & 0x8000) >> 15) {
        MicManager::Get().func_0202d94c();
        if (UnkStruct_020c17a8::Get().mBusy != 0) {
            if (mInputEnabled != 0) {
                while (func_0200d1e0(2, 0) != 0) {
                }
                mInputEnabled = 0;
            }
        } else {
            if (WirelessManager::Get().mUnk_1228 != 0) {
                if (mInputEnabled != 0) {
                    while (func_0200d1e0(2, 0) != 0) {
                    }
                    mInputEnabled = 0;
                }
            } else {
                func_0200d3e0(0xc, 0, 0);
                MicManager::Get().func_0202d94c();
            }
        }
    } else if (mInputEnabled == 0) {
        while (func_0200d1e0(0, mUnk_00) != 0) {
        }
        while (func_0200d1e0(1, mUnk_04) != 0) {
        }
        MicManager::Get().func_0202d94c();
        mInputEnabled = 1;
    }
    if (func_0200f79c() != 0) {
        func_0200f7ac();
    }
    if (mInputEnabled == 0) {
        MicManager::Get().func_0202d94c();
    }
}

GameSystem::~GameSystem() {}

extern "C" void func_01ff9284(void *buffer);
extern void *data_020baf00;

// Placed by the linker right after the start-up code (0x02000c04), apart from the rest of this file.
void main() {
    u8 buffer[0x304];
    u8 state[0x188];
    GameSystem::Get().Init();
    func_01ff9284(buffer);
    data_020baf00 = state;
    GameSystem::Get().Run();
}

void NullScene::Update() {}
