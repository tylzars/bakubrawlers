#include "UI/SelectWindow.hpp"

#include "Graphics/CellAnimation.hpp"
#include "Graphics/SpriteManager.hpp"

SelectWindow::SelectWindow(u32 itemCount, const WindowItemRecord *items, s32 unk3) {
    mCellCount = 0;
    mUnk_061   = 0;
    mUnk_062   = 0;
    mUnk_063   = -1;
    mUnk_064   = 1;
    mUnk_065   = 0;
    mUnk_066   = 0;
    mUnk_07e   = 0;
    mResult    = 0;
    mUnk_0e0   = 0;
    mCursor    = new SelectWindowCursor(itemCount, items, unk3);
    u8 i             = 0;
    WindowItem *item = mCursor->mItems;
    do {
        mCellPos[item->mId].x = item->mX;
        mCellPos[item->mId].y = item->mY;
        mUnk_068[item->mId]   = 0;
        i++;
        item++;
    } while (i < itemCount);
    for (i = 0; i < 22; i++) {
        mCells[i] = NULL;
    }
}

SelectWindow::~SelectWindow() {
    func_0205bd10();
    func_0205bcf4();
}

void SelectWindow::func_0205bcf4() {
    delete mCursor;
    mCursor = NULL;
}

void SelectWindow::func_0205bd10() {
    u8 i = 0;
    do {
        CellAnimation *cell = mCells[i];
        if (cell != NULL) {
            delete cell;
        }
        mCells[i] = NULL;
        i++;
    } while (i < 22);
}

// non-matching: 3 words: the original loads button mUnk_08/mUnk_09 into r2/r1 and stores r2 first.
void SelectWindow::SetCells(u32 count, const WindowCellRecord *cells) {
    u8 i                 = 0;
    SpriteScreen &screen = SpriteManager::Get().Screen(1);
    screen.func_02027624(3);
    do {
        const WindowCellRecord *cell = &cells[i];
        if (cell->mPath != NULL) {
            mCells[mCellCount]            = new CellAnimation(cell->mPath, cell->mUnk_04, 1, 1);
            mCells[mCellCount]->mCharBase = screen.func_020276f4(cell->mUnk_08);
            mCells[mCellCount]->mUnk_11   = cell->mUnk_0c;
        }
        WindowButton *button = &mButtons[mCellCount];
        i++;
        button->mUnk_00  = cell->mUnk_10;
        button->mEnabled = cell->mUnk_14;
        button->mUnk_08  = cell->mUnk_18[0];
        button->mUnk_09  = cell->mUnk_18[1];
        button->mUnk_0a  = cell->mUnk_18[2];
        mCellCount++;
    } while (i < count);
}

void SelectWindow::Draw() {
    u8 i = 0;
    do {
        if (mButtons[i].mEnabled != 0) {
            if (mCells[i] != NULL) {
                mCells[i]->Update();
                mCells[i]->Draw(1, mCellPos[i].x, mCellPos[i].y, mButtons[i].mUnk_00);
            }
        } else if (mUnk_07e != 0 && mCells[i] != NULL) {
            mCells[i]->Update();
        }
        i++;
    } while (i < mCellCount);
}

void SelectWindow::func_0205c74c(u32 count, WindowItem *items, s32 selection) {
    mCursor->mSelection = -1;
    mCursor->mSelection = selection;
    SelectWindowCursor *cursor = mCursor;
    cursor->mItems             = items;
    cursor->mCount             = count;
    WindowItem *item           = mCursor->mItems;
    u8 i                = 0;
    do {
        mCellPos[item->mId].x = item->mX;
        mCellPos[item->mId].y = item->mY;
        mUnk_068[item->mId]   = 0;
        i++;
        item++;
    } while (i < count);
    mUnk_066 = 1;
}

void SelectWindow::func_0205c7c8(int selection) {
    mCursor->mSelection = -1;
    mCursor->mSelection = selection;
    WindowItem *item    = mCursor->mItems;
    u8 i                = 0;
    do {
        mCellPos[item->mId].x = item->mX;
        mCellPos[item->mId].y = item->mY;
        mUnk_068[item->mId]   = 0;
        i++;
        item++;
    } while (i < mCursor->mCount);
    mUnk_066 = 1;
    mUnk_0e0 = 0;
}

void SelectWindow::func_0205c844(u32 unk1, u32 unk2) {
    s8 oldId = GetSelectedId();
    mCursor->mSelection = unk1;
    s8 sel = mCursor->mSelection;
    mCells[unk1]->SetAnimation(mButtons[unk1].mUnk_09);
    if (oldId != -1 && unk1 != oldId && mCursor->func_0203055c(sel)) {
        mCells[oldId]->SetAnimation(mButtons[oldId].mUnk_08);
    }
    mResult   = unk2;
    mUnk_061  = 2;
}

BOOL SelectWindow::func_0205bdf4() {
    u8 i             = 0;
    BOOL done        = TRUE;
    WindowItem *item = mCursor->mItems;
    switch (mUnk_062) {
    case 0:
        do {
            s8 id = item->mId;
            if (mUnk_068[id] == 0 && item->mSlideIn != 0) {
                u8 type = item->mSlideIn;
                if (type == 4) {
                    mCellPos[id].x = -item->mWidth;
                } else if (type == 3) {
                    mCellPos[id].x = 0x100;
                } else if (type == 1) {
                    mCellPos[id].y = 0xc0;
                } else if (type == 2) {
                    mCellPos[id].y = -item->mHeight;
                }
            }
            i++;
            if (item->mSlideIn != 0) {
                mButtons[item->mId].mEnabled = TRUE;
            }
            item++;
        } while (i < mCursor->mCount);
        mUnk_062 = 1;
        break;
    case 1:
        do {
            u8 type = item->mSlideIn;
            if (type == 4) {
                s8 id = item->mId;
                if (mUnk_068[id] != 0) {
                    mCellPos[id].x += (s8)((mUnk_0d8 + 0x20 - mCellPos[id].x) / 7);
                    if (mUnk_0d8 <= mCellPos[item->mId].x) {
                        mCellPos[item->mId].x = mUnk_0d8;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 3) {
                s8 id = item->mId;
                if (mUnk_068[id] != 0) {
                    mCellPos[id].x += (s8)((mUnk_0d8 - 0x20 - mCellPos[id].x) / 7);
                    if (mUnk_0d8 >= mCellPos[item->mId].x) {
                        mCellPos[item->mId].x = mUnk_0d8;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 1) {
                s8 id = item->mId;
                if (mUnk_068[id] != 0) {
                    mCellPos[id].y += (s8)((mUnk_0da - 0x20 - mCellPos[id].y) / 7);
                    if (mUnk_0da >= mCellPos[item->mId].y) {
                        mCellPos[item->mId].y = mUnk_0da;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 2) {
                s8 id = item->mId;
                if (mUnk_068[id] != 0) {
                    mCellPos[id].y += (s8)((mUnk_0da + 0x20 - mCellPos[id].y) / 7);
                    if (mUnk_0da <= mCellPos[item->mId].y) {
                        mCellPos[item->mId].y = mUnk_0da;
                    } else {
                        done = FALSE;
                    }
                }
            }
            i++;
            item++;
        } while (i < mCursor->mCount);
        if (done) {
            i    = 0;
            item = mCursor->mItems;
            do {
                u8 type = item->mSlideIn;
                if (type == 4) {
                    s8 id = item->mId;
                    if (mUnk_068[id] == 0) {
                        mCellPos[id].x += (s8)((item->mX + 0x20 - mCellPos[id].x) / 7);
                        if (item->mX <= mCellPos[item->mId].x) {
                            mCellPos[item->mId].x = item->mX;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 3) {
                    s8 id = item->mId;
                    if (mUnk_068[id] == 0) {
                        mCellPos[id].x += (s8)((item->mX - 0x20 - mCellPos[id].x) / 7);
                        if (item->mX >= mCellPos[item->mId].x) {
                            mCellPos[item->mId].x = item->mX;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 1) {
                    s8 id = item->mId;
                    if (mUnk_068[id] == 0) {
                        mCellPos[id].y += (s8)((item->mY - 0x20 - mCellPos[id].y) / 7);
                        if (item->mY >= mCellPos[item->mId].y) {
                            mCellPos[item->mId].y = item->mY;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 2) {
                    s8 id = item->mId;
                    if (mUnk_068[id] == 0) {
                        mCellPos[id].y += (s8)((item->mY + 0x20 - mCellPos[id].y) / 7);
                        if (item->mY <= mCellPos[item->mId].y) {
                            mCellPos[item->mId].y = item->mY;
                        } else {
                            done = FALSE;
                        }
                    }
                }
                i++;
                item++;
            } while (i < mCursor->mCount);
        }
        if (done) {
            mUnk_062 = 2;
        }
        break;
    case 2:
        mUnk_062 = i;
        return done;
    }
    return FALSE;
}

// non-matching: second loop: the original keeps the counter in lr and the item pointer in r3.
void SelectWindow::func_0205c2e0() {
    u8 i             = 0;
    BOOL done        = TRUE;
    WindowItem *item = mCursor->mItems;
    switch (mUnk_062) {
    case 0:
        mUnk_062 = 1;
        break;
    case 1:
        do {
            u8 type = item->mSlideOut;
            if (type == 4) {
                s8 id = item->mId;
                if (mUnk_068[id] == 0) {
                    mCellPos[id].x += (s8)((0x120 - mCellPos[id].x) / 7);
                    if (mCellPos[item->mId].x >= 0x100) {
                        mCellPos[item->mId].x = 0x100;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 3) {
                s8 id = item->mId;
                if (mUnk_068[id] == 0) {
                    mCellPos[id].x += (s8)(-(item->mWidth + 0x20 + mCellPos[id].x) / 7);
                    if (-item->mWidth >= mCellPos[item->mId].x) {
                        mCellPos[item->mId].x = -item->mWidth;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 1) {
                s8 id = item->mId;
                if (mUnk_068[id] == 0) {
                    mCellPos[id].y += (s8)(-(item->mHeight + 0x20 + mCellPos[id].y) / 7);
                    if (-item->mHeight >= mCellPos[item->mId].y) {
                        mCellPos[item->mId].y = -item->mHeight;
                    } else {
                        done = FALSE;
                    }
                }
            } else if (type == 2) {
                s8 id = item->mId;
                if (mUnk_068[id] == 0) {
                    mCellPos[id].y += (s8)((0xe0 - mCellPos[id].y) / 7);
                    if (mCellPos[item->mId].y >= 0xc0) {
                        mCellPos[item->mId].y = 0xc0;
                    } else {
                        done = FALSE;
                    }
                }
            }
            i++;
            item++;
        } while (i < mCursor->mCount);
        if (done) {
            i    = 0;
            item = mCursor->mItems;
            do {
                u8 type = item->mSlideOut;
                if (type == 4) {
                    s8 id = item->mId;
                    if (mUnk_068[id] != 0) {
                        mCellPos[id].x += (s8)((mUnk_0d8 + 0x20 - mCellPos[id].x) / 7);
                        if (mUnk_0d8 <= mCellPos[item->mId].x) {
                            mCellPos[item->mId].x = mUnk_0d8;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 3) {
                    s8 id = item->mId;
                    if (mUnk_068[id] != 0) {
                        mCellPos[id].x += (s8)((mUnk_0d8 - 0x20 - mCellPos[id].x) / 7);
                        if (mUnk_0d8 >= mCellPos[item->mId].x) {
                            mCellPos[item->mId].x = mUnk_0d8;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 1) {
                    s8 id = item->mId;
                    if (mUnk_068[id] != 0) {
                        mCellPos[id].y += (s8)((mUnk_0da - 0x20 - mCellPos[id].y) / 7);
                        if (mUnk_0da >= mCellPos[item->mId].y) {
                            mCellPos[item->mId].y = mUnk_0da;
                        } else {
                            done = FALSE;
                        }
                    }
                } else if (type == 2) {
                    s8 id = item->mId;
                    if (mUnk_068[id] != 0) {
                        mCellPos[id].y += (s8)((mUnk_0da + 0x20 - mCellPos[id].y) / 7);
                        if (mUnk_0da <= mCellPos[item->mId].y) {
                            mCellPos[item->mId].y = mUnk_0da;
                        } else {
                            done = FALSE;
                        }
                    }
                }
                i++;
                item++;
            } while (i < mCursor->mCount);
        }
        if (done) {
            mUnk_062 = 2;
        }
        break;
    case 2:
        do {
            if (item->mSlideIn != 0) {
                s8 id = item->mId;
                if (mUnk_068[id] == 0) {
                    mButtons[id].mEnabled = FALSE;
                }
            }
            i++;
            item++;
        } while (i < mCursor->mCount);
        mUnk_062 = 0;
        break;
    }
}

// non-matching: 4 bytes long: spill-slot order of items/newPrev, a reload of mCursor->mItems, register choice in states 2 and 5.
BOOL SelectWindow::Update(BOOL input) {
    int result = 0;
    u32 event  = 0;
    if (mUnk_064 == 0) {
        return 0;
    }
    switch (mUnk_061) {
    case 0:
        if (func_0205bdf4()) {
            mUnk_061 = 1;
        }
        break;
    case 1: {
        int oldSel        = mCursor->mSelection;
        WindowItem *items = mCursor->mItems;
        int oldId         = oldSel == -1 ? -1 : items[oldSel].mId;
        int oldPrev       = mCursor->mUnk_05;
        int oldPrevId     = oldPrev == -1 ? -1 : mCursor->mItems[oldPrev].mId;
        if (mUnk_065 == 0) {
            event = mCursor->func_0202f468(input);
        }
        int newSel    = mCursor->mSelection;
        int newId     = newSel == -1 ? -1 : mCursor->mItems[newSel].mId;
        int newPrev   = mCursor->mUnk_05;
        int newPrevId = newPrev == -1 ? -1 : mCursor->mItems[newPrev].mId;
        if (mUnk_07e == 0 && newId != -1 && mButtons[newId].mEnabled == 0) {
            mCursor->mSelection = oldSel;
            return 0;
        }
        switch (event) {
        case 1:
            if (newId == -1) {
                return 0;
            }
            mCells[newId]->SetAnimation(mButtons[newId].mUnk_09);
            if (oldId != -1 && newId != oldId) {
                mCells[oldId]->SetAnimation(mButtons[oldId].mUnk_08);
            }
            mResult  = 1;
            mUnk_065 = 1;
            mUnk_061 = 2;
            break;
        case 2:
            if (newId == -1) {
                return 0;
            }
            mCells[newId]->SetAnimation(mButtons[newId].mUnk_09);
            if (oldId != -1 && newId != oldId) {
                CellAnimation *cell = mCells[oldId];
                u8 anim             = mButtons[oldId].mUnk_08;
                if (anim != cell->mUnk_0e) {
                    cell->SetAnimation(anim);
                }
            }
            mResult  = 2;
            mUnk_065 = 1;
            mUnk_061 = 2;
            break;
        case 3:
            if (newId == -1) {
                return 0;
            }
            mCells[newId]->SetAnimation(mButtons[newId].mUnk_09);
            if (oldId != -1 && newId != oldId && mCursor->func_0203055c(newSel)) {
                mCells[oldId]->SetAnimation(mButtons[oldId].mUnk_08);
            }
            mUnk_063 = oldSel;
            mResult  = 3;
            mUnk_061 = 2;
            break;
        case 5:
            mUnk_0e0 = 1;
            if (newPrev != oldPrev) {
                if (newPrevId != -1 && items[newPrev].mUnk_18 == 3) {
                    CellAnimation *cell = mCells[newPrevId];
                    u8 anim             = mButtons[newPrevId].mUnk_0a;
                    if (anim != cell->mUnk_0e) {
                        cell->SetAnimation(anim);
                    }
                }
                if (oldPrevId != -1 && newPrevId != oldPrevId && oldPrev != newSel) {
                    mCells[oldPrevId]->SetAnimation(mButtons[oldPrevId].mUnk_08);
                }
            }
            break;
        case 4:
            mUnk_0e0 = 1;
            if (newSel != oldSel) {
                if (newId != -1) {
                    CellAnimation *cell = mCells[newId];
                    u8 anim             = mButtons[newId].mUnk_0a;
                    if (anim != cell->mUnk_0e) {
                        cell->SetAnimation(anim);
                    }
                }
                if (oldId != -1 && newId != oldId) {
                    mCells[oldId]->SetAnimation(mButtons[oldId].mUnk_08);
                }
            }
            if (newPrev != oldPrev) {
                if (newPrevId != -1) {
                    CellAnimation *cell = mCells[newPrevId];
                    u8 anim             = mButtons[newPrevId].mUnk_0a;
                    if (anim != cell->mUnk_0e) {
                        cell->SetAnimation(anim);
                    }
                }
                if (oldPrevId != -1 && newPrevId != oldPrevId && oldPrev != newSel) {
                    mCells[oldPrevId]->SetAnimation(mButtons[oldPrevId].mUnk_08);
                }
            }
            break;
        case 0:
            mUnk_0e0 = 0;
            if (newSel != oldSel) {
                if (newId != -1) {
                    CellAnimation *cell = mCells[newId];
                    u8 anim             = mButtons[newId].mUnk_0a;
                    if (anim != cell->mUnk_0e) {
                        cell->SetAnimation(anim);
                    }
                }
                if (oldId != -1 && newId != oldId) {
                    mCells[oldId]->SetAnimation(mButtons[oldId].mUnk_08);
                }
            }
            if (newPrev != oldPrev) {
                if (newPrevId != -1) {
                    CellAnimation *cell = mCells[newPrevId];
                    u8 anim             = mButtons[newPrevId].mUnk_0a;
                    if (anim != cell->mUnk_0e) {
                        cell->SetAnimation(anim);
                    }
                }
                if (oldPrevId != -1 && newPrevId != oldPrevId && oldPrev != newSel) {
                    mCells[oldPrevId]->SetAnimation(mButtons[oldPrevId].mUnk_08);
                }
            }
            break;
        case 7:
            mUnk_0e0 = 0;
            if (newPrev != oldPrev && oldPrevId != -1 && newPrevId != oldPrevId && oldPrev != newSel) {
                mCells[oldPrevId]->SetAnimation(mButtons[oldPrevId].mUnk_08);
            }
            break;
        }
        break;
    }
    case 2: {
        BOOL flag = FALSE;
        if (mCells[GetSelectedId()]->mUnk_12 != 0) {
            flag = TRUE;
        }
        if (mCells[GetSelectedId()]->mUnk_10 == 1) {
            flag = TRUE;
        }
        if (flag) {
            mUnk_0e0 = 0;
            if (mCursor->mItems[mCursor->mSelection].mFlags & 8) {
                mUnk_061 = 5;
                result   = mResult;
            } else {
                result   = mResult;
                mUnk_061 = 3;
            }
        }
        break;
    }
    case 3:
        func_0205c2e0();
        if (mUnk_062 == 2) {
            if (mCursor->mSelection != -1) {
                mCells[GetSelectedId()]->SetAnimation(mButtons[GetSelectedId()].mUnk_08);
            }
        } else if (mUnk_062 == 0) {
            mUnk_066 = 0;
            mUnk_061 = 4;
        }
        break;
    case 4:
        if (mUnk_066 != 0) {
            mUnk_065 = 0;
            mUnk_061 = 0;
        }
        break;
    case 5: {
        if (mUnk_063 != mCursor->mSelection) {
            int id = mUnk_063 == -1 ? -1 : mCursor->mItems[mUnk_063].mId;
            if (id != -1) {
                mCells[GetSelectedId()]->SetAnimation(mButtons[GetSelectedId()].mUnk_08);
            }
        }
        mCursor->mSelection = mUnk_063;
        mUnk_063            = -1;
        SelectWindowCursor *cursor = mCursor;
        cursor->mUnk_05    = -1;
        cursor->mUnk_07    = -1;
        cursor->mUnk_08    = 0;
        mUnk_061            = 1;
        break;
    }
    }
    return result;
}
