#include "Game/UnkStruct_020d433c.hpp"
#include "Scene/SceneBase.hpp"
#include "System/FrameTask.hpp"
#include "Scene/SceneManager.hpp"
#include <nitro/os.h>


// Header of an overlay as read by the overlay loader (0x2c bytes).
struct UnkStruct_OverlayInfo {
    /* 00 */ u32 mId;
    /* 04 */ u32 mRamAddress;
    /* 08 */ u32 mRamSize;
    /* 0c */ u32 mBssSize;
    /* 10 */ PAD(0x10, 0x2c);
};

// Fields of the scene context that the runner uses (the node's SceneInfo area).
struct UnkStruct_RunnerContext {
    /* 00 */ PAD(0x00, 0x12);
    /* 12 */ s16 mNextScene;
    /* 14 */ u8 mState;
};

// Microphone singleton (instance 0x020c189c).
class MicManager {
public:
    /* 00 */ PAD(0x00, 0x20);
    /* 20 */ u8 mActive;
    /* 21 */ PAD(0x21, 0x28);

    MicManager() {
        mActive = 0;
    }
    ~MicManager();

    static MicManager &Get() {
        static MicManager instance;
        return instance;
    }

    void func_0202d8cc();
};

// The scene that switches between the game's scenes: loads the overlay of the next one and constructs it.
class SceneRunner : public SceneBase {
public:
    /* 0c */ s32 mOverlay; // overlay currently loaded, -1 for none
    /* 10 */ s16 mNextScene;
    /* 12 */ u8 mOverlayState; // 2 when mOverlay is loaded
    /* 13 */

    SceneRunner();
    virtual ~SceneRunner() {}
    virtual void Update();

    static SceneRunner &Get() {
        static SceneRunner instance;
        return instance;
    }

    void func_02024818(u32 overlay);
    void func_02024968(s32 scene);
};

#include "Scene/ArenaWindowScene.hpp"
#include "Scene/GameOverScene.hpp"
#include "Scene/MapScene.hpp"
#include "Scene/MyroomScene.hpp"
#include "Scene/ParkScene.hpp"
#include "Scene/StoreScene.hpp"
#include "Scene/TournamentScene.hpp"
#include "Scene/TutorialScene.hpp"
#include "Scene/UnkScene_ov000.hpp"
#include "System/SoundManager.hpp"
#include "System/WirelessManager.hpp"
#include "System/TouchManager.hpp"
#include "Text/TextManager.hpp"
#include "Scene/TitleScene.hpp"

extern u8 data_0220e544[];


// Constructs the BattlePlayerList (0x2b4 bytes) in place and returns it.

// Overlay ids, absolute symbols defined by the linker script.
extern char SDK_OVERLAY_OV000_ID[];
extern char SDK_OVERLAY_OV001_ID[];
extern char SDK_OVERLAY_OV002_ID[];
extern char SDK_OVERLAY_OV003_ID[];
extern char SDK_OVERLAY_OV004_ID[];
extern char SDK_OVERLAY_OV005_ID[];
extern char SDK_OVERLAY_OV006_ID[];
extern char SDK_OVERLAY_OV007_ID[];
extern char SDK_OVERLAY_OV008_ID[];

extern "C" void func_0200b0d4(UnkStruct_OverlayInfo *info, u32 processor, u32 id);
extern "C" void func_0200b4e8(u32 processor, u32 id);
extern "C" void func_0200b53c(u32 processor, u32 id);
extern void func_02021d38(void *lo);
extern void func_02021e10();
extern "C" void func_02007230();
extern void func_0202555c();
extern void func_020255f8(void (*hook)());
extern void func_02025df8();

void SceneRunner::func_02024818(u32 overlay) {
    UnkStruct_OverlayInfo info;
    FrameTask::Get().func_0203dc28();
    func_02021e10();
    if (mOverlayState == 2) {
        func_0200b53c(0, mOverlay);
    }
    mOverlay = overlay;
    func_0200b0d4(&info, 0, overlay);
    func_0200b4e8(0, mOverlay);
    mOverlayState = 2;
    func_02021d38((void *) (info.mRamAddress + (info.mRamSize + info.mBssSize)));
    FrameTask::Get().func_0203dbc4();
}

SceneRunner::SceneRunner() : SceneBase(0, 3) {
    mOverlay      = -1;
    mOverlayState = 0;
}

void SceneRunner::func_02024968(s32 scene) {
    FrameTask::Get().func_0203dd90();
    if ((scene < 1) | (scene >= 14)) {
        scene = 1;
    }
    func_020255f8(0);
    func_02007230();
    func_0202555c();
    SceneManager::Get().DeleteLayer();
    SceneManager::Get().Reset();
    TextManager::Get().mUnk_5b = 0;
    if (!WirelessManager::Get().func_0202f238()) {
        FrameTask::Get().func_0203dc28();
        func_02021e10();
        FrameTask::Get().func_0203dbc4();
    }
    func_02025df8();
    mContext->mUnk_12 = 14;
    ((UnkStruct_RunnerContext *) mContext)->mState = scene;
    if (data_020d433c.mUnk_84 == 0) {
        SoundManager::Get().func_020245a4();
    }
    if (scene == 5) {
        return;
    }
    MicManager::Get().func_0202d8cc();
}

void SceneRunner::Update() {
    UnkStruct_RunnerContext *context = (UnkStruct_RunnerContext *) mContext;
    switch (context->mState) {
        case 0:
            TouchManager::Get().func_02022ae0();
            func_02007230();
            if (TouchManager::Get().func_02022ae0()) {
                TouchManager::Get().func_02022af0();
            }
            context->mState++;
            mContext->mUnk_12 = 14;
            break;
        case 1:
            context->mState++;
            break;
        case 2:
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV000_ID);
            new UnkScene_02226e60();
            context->mState = 14;
            break;
        case 3:
            break;
        case 4:
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV000_ID);
            new TitleScene();
            context->mState = 14;
            break;
        case 5: {
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV007_ID);
            data_020d433c.mBattlePlayers = new BattlePlayerList();
            context->mState = 14;
            break;
        }
        case 6:
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV003_ID);
            new StoreScene();
            context->mState = 14;
            break;
        case 7:
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV008_ID);
            new MyroomScene();
            context->mState = 14;
            break;
        case 8:
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV001_ID);
            new ArenaWindowScene();
            context->mState = 14;
            break;
        case 9:
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV005_ID);
            new MapScene();
            context->mState = 14;
            break;
        case 10:
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV005_ID);
            new TutorialScene();
            context->mState = 14;
            break;
        case 11:
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV006_ID);
            new GameOverScene();
            context->mState = 14;
            break;
        case 12:
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV004_ID);
            new ParkScene();
            context->mState = 14;
            break;
        case 13:
            SceneRunner::Get().func_02024818((u32) SDK_OVERLAY_OV002_ID);
            new TournamentScene();
            context->mState = 14;
            break;
        case 14:
            mNextScene = context->mNextScene;
            if (mNextScene == 14) {
                return;
            }
            FrameTask::Get().func_0203dc28();
            context->mState = 15;
            break;
        case 15:
            if (func_0203dd0c(&FrameTask::Get(), 0)) {
                return;
            }
            func_02024968(mNextScene);
            break;
    }
}
