#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"
#include "UI/SelectWindow.hpp"
#include "System/InputManager.hpp"
#include "System/SoundManager.hpp"
#include "System/TouchManager.hpp"

SelectWindowCursor::SelectWindowCursor(u32 count, const void *table, u32 unk3) {
    mCount     = count;
    mUnk_05    = -1;
    mSelection = unk3;
    mUnk_07    = -1;
    mUnk_08    = 0;
    mScrollItem    = 0;
    mDragging    = 0;
    mScrollItemY    = 0;
    mScrollAreaY    = 0;
    mScrollAreaHeight    = 0;
    mShownRowCount    = 0;
    mRowCount    = 0;
    mScrollMax    = 0;
    mScroll    = 0;
    mItems     = (WindowItem *) table;
}

u32 SelectWindowCursor::func_0202f468(u32 unk1) {
    s8 previous = mSelection;
    u32 result  = func_0202f61c(unk1);
    if (unk1 != 0) {
        switch (result) {
            case 1:
                SoundManager::Get().StartSeqArc(0);
                break;
            case 2:
                SoundManager::Get().StartSeqArc(1);
                break;
            case 3:
                SoundManager::Get().StartSeqArc(0);
                break;
            case 5:
            case 7:
                break;
            case 6:
                SoundManager::Get().StartSeqArc(2);
                mSelection = -1;
                break;
            default:
                if (mSelection != previous) {
                    SoundManager::Get().StartSeqArc(5);
                }
                break;
        }
    }
    return result;
}

u32 SelectWindowCursor::func_0202f61c(u32 playSound) {
    u32 mode   = 0;
    u32 result = 0;
    if (mDragging != 0) {
        if (mScrollMax != 0) {
            s32 pos;
            if ((pos = mScrollAreaHeight) >= (s16) (TouchManager::Get().mY - mScrollAreaY)) {
                if ((s16) (TouchManager::Get().mY - mScrollAreaY) < 0) {
                    pos = 0;
                } else {
                    pos = (s16) (TouchManager::Get().mY - mScrollAreaY);
                }
            }
            s32 range  = mScrollAreaHeight - mItems[mScrollItem].mHeight;
            s32 scroll = mScrollMax;
            s32 q      = scroll * ((u8) pos + range / (scroll * 2)) / range;
            if (q <= scroll) {
                if (q < 0) {
                    q = 0;
                }
                scroll = q;
            }
            mScroll = scroll;
            RefreshScrollItemY();
            if (TouchManager::Get().mTouching != 0) {
                mDragging = 0;
            }
        }
    } else {
    if (TouchManager::Get().mPressed != 0) {
        mode = 1;
    } else if (TouchManager::Get().mTouching != 0) {
        mode = 2;
    } else if (TouchManager::Get().mDown != 0) {
        mode = 3;
    }
    mUnk_05 = -1;
    switch (mode) {
        case 0: {
            BOOL a = FALSE;
            s8 cand = mSelection;
            s32 dir = -1;
            BOOL b = FALSE;
            BOOL l = FALSE;
            BOOL r = FALSE;
            if (InputManager::Get().IsHeld(1)) {
                a = TRUE;
            } else if (InputManager::Get().IsHeld(2)) {
                b = TRUE;
            } else if (InputManager::Get().IsTrigger(KEY_UP)) {
                dir = 0;
            } else if (InputManager::Get().IsTrigger(KEY_DOWN)) {
                dir = 1;
            } else if (InputManager::Get().IsTrigger(KEY_LEFT)) {
                dir = 2;
            } else if (InputManager::Get().IsTrigger(KEY_RIGHT)) {
                dir = 3;
            } else if (InputManager::Get().IsHeld(0x100)) {
                r = TRUE;
            } else if (InputManager::Get().IsHeld(0x200)) {
                l = TRUE;
            }
            if (a) {
                s8 i             = 0;
                WindowItem *item = mItems;
                do {
                    if (item->mFlags & 4) {
                        mSelection = i;
                        break;
                    }
                    item++;
                    i++;
                } while (mCount > i);
                if (mSelection != -1) {
                    item = &mItems[mSelection];
                    if (item->mUnk_18 != 0) {
                        u32 flags = item->mFlags;
                        if (flags & 1) {
                            result = 1;
                        } else if (flags & 2) {
                            result = 2;
                        } else if (flags & 0x800) {
                            result = 6;
                        } else {
                            result = 3;
                        }
                        break;
                    }
                }
            } else if (b) {
                BOOL found       = FALSE;
                s8 i             = 0;
                WindowItem *item = mItems;
                do {
                    if (item->mFlags & 2) {
                        mSelection = i;
                        found      = TRUE;
                        break;
                    }
                    item++;
                    i++;
                } while (mCount > i);
                if (found) {
                    result = 2;
                    break;
                }
                WindowItem *item2 = mItems;
                s8 i2             = 0;
                do {
                    if (item2->mFlags & 0x100) {
                        mSelection = i2;
                        found      = TRUE;
                        break;
                    }
                    item2++;
                    i2++;
                } while (mCount > i2);
                if (found) {
                    if (mItems[mSelection].mUnk_18 != 0) {
                        u32 flags = mItems[mSelection].mFlags;
                        if (flags & 1) {
                            result = 1;
                        } else if (flags & 2) {
                            result = 2;
                        } else if (flags & 0x800) {
                            result = 6;
                        } else {
                            result = 3;
                        }
                        break;
                    }
                }
            } else if (l) {
                BOOL found       = FALSE;
                s8 i             = 0;
                WindowItem *item = mItems;
                do {
                    if (item->mFlags & 0x200) {
                        mSelection = i;
                        found      = TRUE;
                        break;
                    }
                    item++;
                    i++;
                } while (mCount > i);
                if (found) {
                    if (mItems[mSelection].mUnk_18 != 0) {
                        u32 flags = mItems[mSelection].mFlags;
                        if (flags & 1) {
                            result = 1;
                        } else if (flags & 2) {
                            result = 2;
                        } else if (flags & 0x800) {
                            result = 6;
                        } else {
                            result = 3;
                        }
                        break;
                    }
                }
            } else if (r) {
                BOOL found       = FALSE;
                s8 i             = 0;
                WindowItem *item = mItems;
                do {
                    if (item->mFlags & 0x400) {
                        mSelection = i;
                        found      = TRUE;
                        break;
                    }
                    item++;
                    i++;
                } while (mCount > i);
                if (found) {
                    if (mItems[mSelection].mUnk_18 != 0) {
                        u32 flags = mItems[mSelection].mFlags;
                        if (flags & 1) {
                            result = 1;
                        } else if (flags & 2) {
                            result = 2;
                        } else if (flags & 0x800) {
                            result = 6;
                        } else {
                            result = 3;
                        }
                        break;
                    }
                }
            } else if (dir >= 0) {
                cand = -1;
                if (mSelection > -1) {
                    cand = mItems[mSelection].mNeighbors[dir];
                } else {
                    s8 i = 0;
                    do {
                        if (HasNeighbor(i)) {
                            cand = i;
                            break;
                        }
                        i++;
                    } while (mCount > i);
                }
                if (cand == -1) {
                    cand = mSelection;
                }
            }
            if (cand != mSelection) {
                if (mItems[cand].mFlags & 0x8000) {
                    do {
                        u8 next = mItems[cand].mNeighbors[dir];
                        if (next == -1) {
                            cand = mSelection;
                            break;
                        }
                        if (cand == next) {
                            cand = mSelection;
                            break;
                        }
                        cand = (s8) next;
                    } while ((mItems[cand].mFlags & 0x8000) && cand != mSelection);
                }
                mSelection = cand;
                mUnk_05    = cand;
            }
            break;
        }
        case 1: {
            mUnk_07 = -1;
            mUnk_08 = 0;
            u8 i    = 0;
            do {
                if (!(mItems[i].mFlags & 0x8000)) {
                    if (mItems[i].mFlags & 0x10) {
                        if (mScrollMax != 0) {
                            if (TouchManager::Get().IsPressedIn(mItems[i].mX, mScrollAreaY, mItems[i].mWidth, mScrollAreaHeight)) {
                                mDragging = 1;
                                break;
                            }
                        }
                    } else if (TouchManager::Get().IsPressedIn(mItems[i].mX, mItems[i].mY, mItems[i].mWidth,
                                                                 mItems[i].mHeight)) {
                        if (mScrollMax != 0) {
                            if (mItems[i].mFlags & 0x20) {
                                if (mScroll != 0) {
                                    mScroll--;
                                    RefreshScrollItemY();
                                    if (playSound != 0) {
                                        SoundManager::Get().StartSeqArc(5);
                                    }
                                }
                            } else if (mItems[i].mFlags & 0x40) {
                                if (mScroll < mScrollMax) {
                                    mScroll++;
                                    RefreshScrollItemY();
                                    if (playSound != 0) {
                                        SoundManager::Get().StartSeqArc(5);
                                    }
                                }
                            }
                        }
                        s8 sel  = i;
                        mUnk_07 = sel;
                        mUnk_08 = mUnk_07 == mSelection ? 1 : 0;
                        mUnk_05 = sel;
                        if (HasNeighbor(mUnk_05)) {
                            mSelection = mUnk_05;
                        }
                        result = 4;
                        if (mItems[i].mFlags & 0x800) {
                            result = 7;
                        }
                        break;
                    }
                }
                i++;
            } while (i < mCount);
            break;
        }
        case 2: {
            u8 i = 0;
            do {
                if (!(mItems[i].mFlags & 0x8000)) {
                    if (TouchManager::Get().IsReleasedIn(mItems[i].mX, mItems[i].mY, mItems[i].mWidth,
                                                          mItems[i].mHeight)
                        && mUnk_07 == i) {
                        u32 kind = mItems[i].mUnk_18;
                        if (kind == 1 || (kind == 2 && mUnk_08 != 0)) {
                            mSelection = i;
                            u32 flags  = mItems[mSelection].mFlags;
                            if (flags & 1) {
                                result = 1;
                            } else if (flags & 2) {
                                result = 2;
                            } else if (flags & 0x800) {
                                result = 6;
                            } else {
                                result = 3;
                            }
                        }
                        break;
                    }
                }
                i++;
            } while (i < mCount);
            mUnk_07 = -1;
            mUnk_08 = 0;
            break;
        }
        case 3: {
            u8 i = 0;
            do {
                if (!(mItems[i].mFlags & 0x8000)) {
                    if (TouchManager::Get().IsDownIn(mItems[i].mX, mItems[i].mY, mItems[i].mWidth,
                                                          mItems[i].mHeight)) {
                        if (mScrollMax != 0) {
                            if (TouchManager::Get().IsTriggerIn(mItems[i].mX, mItems[i].mY, mItems[i].mWidth,
                                                                  mItems[i].mHeight)) {
                                if (mItems[i].mFlags & 0x20) {
                                    if (mScroll != 0) {
                                        mScroll--;
                                        RefreshScrollItemY();
                                        if (playSound != 0) {
                                            SoundManager::Get().StartSeqArc(5);
                                        }
                                    }
                                } else if (mItems[i].mFlags & 0x40) {
                                    if (mScroll < mScrollMax) {
                                        mScroll++;
                                        RefreshScrollItemY();
                                        if (playSound != 0) {
                                            SoundManager::Get().StartSeqArc(5);
                                        }
                                    }
                                }
                            }
                        }
                        mUnk_05 = i;
                        result  = 5;
                        if (mItems[i].mFlags & 0x800) {
                            result = 7;
                        }
                        break;
                    }
                }
                i++;
            } while (i < mCount);
            if (mUnk_05 != mUnk_07) {
                mUnk_07 = -1;
                mUnk_08 = 0;
            }
            break;
        }
    }
    }
    return result;
}

void SelectWindowCursor::SetupScroll(u32 unk1, u32 unk2, u32 unk3, u32 unk4) {
    mScrollAreaY = unk1;
    s32 d   = unk2 - unk1;
    if (d < 0) {
        d = 0;
    }
    mScrollAreaHeight = d;
    mShownRowCount = unk3;
    mRowCount = *(u8 *) &unk4;
    s32 e   = *(u8 *) &unk4 - mShownRowCount;
    if (e < 0) {
        e = 0;
    }
    mScrollMax = e;
    mDragging = 0;
    mScrollItemY = 0;
    mScroll = 0;
    u8 i = 0;
    do {
        if (mItems[i].mFlags & 0x10) {
            mScrollItem = i;
            break;
        }
        i++;
    } while (i < mCount);
    RefreshScrollItemY();
}

void SelectWindowCursor::SetScroll(u32 scroll) {
    if (scroll > mScrollMax) {
        scroll = mScrollMax;
    }
    mScroll = scroll;
    RefreshScrollItemY();
}

void SelectWindowCursor::RefreshScrollItemY() {
    if (mScrollMax == 0) {
        mScrollItemY = mScrollAreaY;
        return;
    }
    mScrollItemY = mScrollAreaY + mScroll * (mScrollAreaHeight - mItems[mScrollItem].mHeight) / mScrollMax;
}

BOOL SelectWindowCursor::HasNeighbor(int index) {
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
