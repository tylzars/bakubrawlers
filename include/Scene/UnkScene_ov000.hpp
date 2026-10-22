#pragma once

#include "global.h"
#include "types.h"

#include "Scene/SceneBase.hpp"

class MessageScreen;

// Filled by OS_GetOwnerInfo; the first byte is a kind (1 or 2 skips the scene).
struct OwnerInfo {
    /* 00 */ u8 mKind;
    /* 01 */ PAD(0x01, 0x54);
    /* 54 */
};

extern "C" void OS_GetOwnerInfo(OwnerInfo *info);

// Overlay 0 scene with vtable 0x02226e4c. OwnerInfo::mKind 1 or 2 sets gGameData.mLanguage to 0 / 1 and calls
// RequestScene(1) at once; any other kind sets 0, shows MessageScreen(MESSAGE_SCREEN_TITLE_8) until IsPressed(1) or a
// stylus press, then calls RequestScene(1). mObj10 is never set (only deleted).
class UnkScene_02226e4c : public SceneBase {
public:
    /* 00 (base) */
    /* 0c */ MessageScreen *mObj0c;
    /* 10 */ MessageScreen *mObj10;
    /* 14 */ u8 mState;
    /* 15 */

    UnkScene_02226e4c();

    /* 00 */ virtual ~UnkScene_02226e4c() override;
    /* 08 */ virtual void Update() override;
    /* 0c */

    void InitDisplay(); // 0221c524
};
