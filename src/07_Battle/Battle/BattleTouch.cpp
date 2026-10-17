#include "Battle/BattleBase.hpp"
#include "Battle/BattleUtil.hpp"

#include "Game/UnkStruct_020d433c.hpp"
#include "System/TouchManager.hpp"
#include "System/WirelessManager.hpp"

extern "C" s32 FX_Atan2Idx(s32 y, s32 x);

// Battle message of the communication buffer data_0220f540 (the layout used by the battle).
struct BattleNetMessage {
    /* 00 */ u8 mUnk_00;
    /* 01 */ PAD(0x01, 0x04);
    /* 04 */ u16 mUnk_04[4];
    /* 0c */ u16 mUnk_0c[4];
    /* 14 */ u16 mUnk_14[2];
    /* 18 */ u8 mUnk_18;
};

u8 func_ov007_02229ee4(s32 x, s32 y, s32 width, s32 height, u16 unk5, u8 *flag) {
    u8 hit = 0;
    if (TouchManager::Get().mDown == 0) {
        *flag = 0;
        return 0;
    }
    s32 touchX = (s16) TouchManager::Get().mX;
    s32 touchY = (s16) TouchManager::Get().mY;
    s32 downX = TouchManager::Get().mUnk_08;
    if (func_ov007_022252d0(downX, TouchManager::Get().mUnk_0a, touchX, touchY) < unk5) {
        if (*flag != 0) {
            (*flag)--;
            hit = 1;
        }
        return hit;
    }
    if (!TouchManager::Get().IsDownIn((s16) x, (s16) y, (s16) width, (s16) height)) {
        if (*flag != 0) {
            (*flag)--;
            hit = 1;
        }
        return hit;
    }
    *flag = 5;
    return 1;
}

static u32 sTouchTicks;

s32 func_ov007_0222a118(s32 x, s32 y, s32 radius) {
    s32 angle = 0;
    if (TouchManager::Get().mDown != 0) {
        sTouchTicks++;
    } else {
        sTouchTicks = 0;
    }
    if (sTouchTicks >= 2) {
        s32 dx = TouchManager::Get().mX - x;
        s32 dy = TouchManager::Get().mY - y;
        if (dx * dx + dy * dy <= radius * radius) {
            angle = FX_Atan2Idx(dy << 12, dx << 12);
            s32 startX = (u16) TouchManager::Get().mUnk_08 - x;
            angle -= FX_Atan2Idx(((u16) TouchManager::Get().mUnk_0a - y) << 12, startX << 12);
            if (angle > 0x8000) {
                angle -= 0x10000;
            } else if (angle < -0x8000) {
                angle += 0x10000;
            }
        } else {
            sTouchTicks = 0;
        }
    }
    return angle;
}
