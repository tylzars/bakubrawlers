#include "UI/SelectWindow.hpp"

void SelectWindowCursor::func_02030474(u32 unk1, u32 unk2, u32 unk3, u32 unk4) {
    mUnk_0c = unk1;
    s32 d   = unk2 - unk1;
    if (d < 0) {
        d = 0;
    }
    mUnk_0d = d;
    mUnk_0e = unk3;
    mUnk_0f = *(u8 *) &unk4;
    s32 e   = *(u8 *) &unk4 - mUnk_0e;
    if (e < 0) {
        e = 0;
    }
    mUnk_10 = e;
    mUnk_0a = 0;
    mUnk_0b = 0;
    mScroll = 0;
    u8 i = 0;
    do {
        if (mItems[i].mFlags & 0x10) {
            mUnk_09 = i;
            break;
        }
        i++;
    } while (i < mCount);
    func_02030508();
}

void SelectWindowCursor::SetScroll(u32 scroll) {
    if (scroll > mUnk_10) {
        scroll = mUnk_10;
    }
    mScroll = scroll;
    func_02030508();
}

void SelectWindowCursor::func_02030508() {
    if (mUnk_10 == 0) {
        mUnk_0b = mUnk_0c;
        return;
    }
    mUnk_0b = mUnk_0c + mScroll * (mUnk_0d - mItems[mUnk_09].mHeight) / mUnk_10;
}

BOOL SelectWindowCursor::func_0203055c(int index) {
    if (index == -1) {
        return FALSE;
    }
    BOOL result = FALSE;
    u8 i        = 0;
    WindowItem *item = &mItems[index];
    for (; i < 4; i++) {
        if (item->mNeighbors[i] > -1) {
            result = TRUE;
        }
    }
    return result;
}
