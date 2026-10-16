#pragma once

#include "global.h"
#include "types.h"

// Save statuses. GetSlotStatus and WriteSlotMarker return them; the two save scenes in FileLoader.cpp pass them to
// RequestScene, and the parent reads them back from its context's mUnk_12 (TitleScene, SaveWindow). There 0
// means "no result yet" and stays a literal.
#define SAVE_STATUS_ERROR 1    // a card read or write call failed (GetSlotStatus: only the read of the second copy)
#define SAVE_STATUS_NO_MAGIC 2 // neither 0x19691015 nor the save magic 0x20071019 in the first word of either copy
#define SAVE_STATUS_CORRUPT 3  // load scene: neither copy passed the CRC and magic-word check (FileLoader.cpp:226); its
                               // read result (:221) is not tested, so a failed read also ends here
#define SAVE_STATUS_OK 4       // save magic found / both writes done / CRC and magic words match
#define SAVE_STATUS_MARKER 5   // the marker 0x19691015 that WriteSlotMarker writes found in a copy

// Save-storage helpers used by the two save scenes in FileLoader.cpp (Init, ComputeCrc, GetSlotStatus,
// WriteSlotMarker). Singleton (instance 0x020c17a8), constructed on first use and destroyed at exit. mBusy is set while
// one of those scenes runs; GameSystem::Run and func_02021700 test it (Game.cpp:251, :303). Every caller passes slot 0.
class SaveManager {
public:
    /* 00 */ u8 mBusy; // set while a save operation is running
    /* 01 */
    // One byte: Game's copy of the instance (0x020c17a8) is followed by a guard at 0x020c17ac.

#ifdef GAME_SINGLETON_CTORS
    // As Game.cpp saw the class: an empty constructor, so its copies of the singleton objects join guard first.
    SaveManager() {}
#endif
    ~SaveManager();

    static SaveManager &Get() {
        static SaveManager instance;
        return instance;
    }

    void Init();
    u16 ComputeCrc(const void *data, u32 size);
    // Reads the first word of both copies of the slot. Only the second read's result is tested (FileLoader.cpp:53-57).
    // Every caller passes slot 0.
    u32 GetSlotStatus(u32 unk1);
    u32 WriteSlotMarker(u32 unk1);
};
