#include "System/InputManager.hpp"

#include <nitro/mi.h>

#define REG_KEYINPUT (*(vu16 *) 0x04000130)
#define KEY_EXT_SHARED (*(vu16 *) 0x027fffa8)
#define KEY_MASK 0x2fff

void InputManager::Init() {
    MI_CpuClear16(this, sizeof(InputManager));
    mRepeatDelay    = 30;
    mRepeatInterval = 5;
}

void InputManager::Update(BOOL enabled) {
    u16 keys = (u16) (((REG_KEYINPUT | KEY_EXT_SHARED) ^ KEY_MASK) & KEY_MASK);
    if (enabled == 0) {
        keys = 0;
    }
    mHeld     = keys & (keys ^ mKeys);
    mReleased = ~keys & (keys ^ mKeys);
    mKeys     = keys;

    u16 i = INPUT_KEY_COUNT;
    do {
        i--;
        if (keys & (1 << i)) {
            if (mHoldCount[i] < 0xff) {
                mHoldCount[i]++;
            }
        }
        if (mReleased & (1 << i)) {
            mHoldCount[i]   = 0;
            mRepeatCount[i] = 0;
        }
    } while (i > 0);

    mTrigger = 0;
    i        = INPUT_KEY_COUNT;
    do {
        i--;
        if (mHoldCount[i] > mRepeatDelay) {
            if (mRepeatCount[i] == 0) {
                mTrigger |= 1 << i;
            }
            mRepeatCount[i]++;
            if (mRepeatCount[i] > mRepeatInterval) {
                mRepeatCount[i] = 0;
            }
        }
    } while (i > 0);
    mTrigger |= mHeld;
}
