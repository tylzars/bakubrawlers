#include "Game/UnkStruct_020d433c.hpp"
#include "Scene/SceneBase.hpp"
#include "System/FrameTask.hpp"
#include "Scene/SceneManager.hpp"
#include <nitro/os.h>


// Header of an overlay as read by the overlay loader (0x2c bytes).
struct OverlayInfo {
    /* 00 */ u32 mId;
    /* 04 */ u32 mRamAddress;
    /* 08 */ u32 mRamSize;
    /* 0c */ u32 mBssSize;
    /* 10 */ PAD(0x10, 0x2c);
};

// Fields of the scene context that the runner uses (the node's SceneInfo area).
struct SceneRunnerContext {
    /* 00 */ PAD(0x00, 0x12);
    /* 12 */ s16 mRequest;
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

    void StopSampling();
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

    void LoadOverlay(u32 overlay);
    void SwitchToScene(s32 scene);
};

#include "Scene/ArenaWindowScene.hpp"
#include "Scene/GameOverScene.hpp"
#include "Scene/MapScene.hpp"
#include "Scene/MyroomScene.hpp"
#include "Scene/ParkScene.hpp"
#include "Scene/StoreScene.hpp"
#include "Scene/TournamentScene.hpp"
#include "Scene/TutorialScene.hpp"
#include "Scene/LogoScene.hpp"
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

extern "C" void FS_LoadOverlayInfo(OverlayInfo *info, u32 processor, u32 id);
extern "C" void FS_LoadOverlay(u32 processor, u32 id);
extern "C" void FS_RunOverlayDestructorsById(u32 processor, u32 id);
extern void RecreateGameHeap(void *lo);
extern void AddGameHeapBlock();
extern "C" void func_02007230();
extern void ClearFrameCallbacks();
extern void RemoveHBlankHook(void (*hook)());
extern void RewindUploadQueue();

void SceneRunner::LoadOverlay(u32 overlay) {
    OverlayInfo info;
    FrameTask::Get().CloseHandles();
    AddGameHeapBlock();
    if (mOverlayState == 2) {
        FS_RunOverlayDestructorsById(0, mOverlay);
    }
    mOverlay = overlay;
    FS_LoadOverlayInfo(&info, 0, overlay);
    FS_LoadOverlay(0, mOverlay);
    mOverlayState = 2;
    RecreateGameHeap((void *) (info.mRamAddress + (info.mRamSize + info.mBssSize)));
    FrameTask::Get().OpenHandles();
}

SceneRunner::SceneRunner() : SceneBase(0, 3) {
    mOverlay      = -1;
    mOverlayState = 0;
}

void SceneRunner::SwitchToScene(s32 scene) {
    FrameTask::Get().StopAllChannels();
    if ((scene < 1) | (scene >= 14)) {
        scene = 1;
    }
    RemoveHBlankHook(0);
    func_02007230();
    ClearFrameCallbacks();
    SceneManager::Get().DeleteLayer();
    SceneManager::Get().Reset();
    TextManager::Get().mUse4bppFont = 0;
    if (!WirelessManager::Get().func_0202f238()) {
        FrameTask::Get().CloseHandles();
        AddGameHeapBlock();
        FrameTask::Get().OpenHandles();
    }
    RewindUploadQueue();
    mContext->mRequest = 14;
    ((SceneRunnerContext *) mContext)->mState = scene;
    if (data_020d433c.mUnk_84 == 0) {
        SoundManager::Get().RestoreHeapState();
    }
    if (scene == 5) {
        return;
    }
    MicManager::Get().StopSampling();
}

void SceneRunner::Update() {
    SceneRunnerContext *context = (SceneRunnerContext *) mContext;
    switch (context->mState) {
        case 0:
            TouchManager::Get().ReadCalibration();
            func_02007230();
            if (TouchManager::Get().ReadCalibration()) {
                TouchManager::Get().ApplyCalibrationAndEnable();
            }
            context->mState++;
            mContext->mRequest = 14;
            break;
        case 1:
            context->mState++;
            break;
        case 2:
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV000_ID);
            new LogoScene();
            context->mState = 14;
            break;
        case 3:
            break;
        case 4:
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV000_ID);
            new TitleScene();
            context->mState = 14;
            break;
        case 5: {
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV007_ID);
            data_020d433c.mBattlePlayers = new BattlePlayerList();
            context->mState = 14;
            break;
        }
        case 6:
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV003_ID);
            new StoreScene();
            context->mState = 14;
            break;
        case 7:
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV008_ID);
            new MyroomScene();
            context->mState = 14;
            break;
        case 8:
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV001_ID);
            new ArenaWindowScene();
            context->mState = 14;
            break;
        case 9:
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV005_ID);
            new MapScene();
            context->mState = 14;
            break;
        case 10:
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV005_ID);
            new TutorialScene();
            context->mState = 14;
            break;
        case 11:
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV006_ID);
            new GameOverScene();
            context->mState = 14;
            break;
        case 12:
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV004_ID);
            new ParkScene();
            context->mState = 14;
            break;
        case 13:
            SceneRunner::Get().LoadOverlay((u32) SDK_OVERLAY_OV002_ID);
            new TournamentScene();
            context->mState = 14;
            break;
        case 14:
            mNextScene = context->mRequest;
            if (mNextScene == 14) {
                return;
            }
            FrameTask::Get().CloseHandles();
            context->mState = 15;
            break;
        case 15:
            if (func_0203dd0c(&FrameTask::Get(), 0)) {
                return;
            }
            SwitchToScene(mNextScene);
            break;
    }
}
